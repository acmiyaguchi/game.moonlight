#include "Callbacks.h"
#include "log/Log.h"
#include "MoonlightEnvironment.h"
#include "kodi/libKODI_game.h"
#include "Limelight.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/mem.h>
#include <libswscale/swscale.h>
}

#include <cstring>

using namespace MOONLIGHT;

static AVCodec* codec = NULL;
static AVCodecContext* codec_context = NULL;
static AVFrame* picture = NULL;

static struct SwsContext *sws_context = NULL;
static AVPicture* pic = NULL;

static AVCodecParserContext* parser = NULL;
static CHelper_libKODI_game* frontend = NULL;

void decoder_renderer_setup(int width, int height, int redrawRate, void* context, int drFlags)
{
  isyslog("VideoCallbacks::Setup");
  avcodec_register_all();
  codec = avcodec_find_decoder(AV_CODEC_ID_H264);
  if (!codec) {
	  esyslog("Cannot find the h264 codec");
  }
  codec_context = avcodec_alloc_context3(codec);
  if(codec->capabilities & CODEC_CAP_TRUNCATED) {
	  codec_context->flags |= CODEC_FLAG_TRUNCATED;
  }
  if(avcodec_open2(codec_context, codec, NULL) < 0) {
	  esyslog("Could not open codec");
  }
  picture = avcodec_alloc_frame();

  pic = new AVPicture();

  sws_context = sws_getContext(width, height, PIX_FMT_YUV420P, width, height, PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
  pic->linesize[0] = width * 4;
  pic->data[0]     = new uint8_t[pic->linesize[0] * height];

  parser = av_parser_init(AV_CODEC_ID_H264);
  if(!parser) {
	  esyslog("Cannot create h264 parser");
  }

  frontend = CMoonlightEnvironment::Get().GetFrontend();

  int size = width*height*3/2;
  uint8_t image[size];
  std::memset(image, 154, size);

  frontend->VideoFrame(image, 800, 600, GAME_RENDER_FMT_YUV420P);
}

void decoder_renderer_cleanup()
{
	isyslog("VideoCallbacks::Cleanup");
	if (parser) {
		av_parser_close(parser);
		parser = NULL;
	}
	if (codec_context) {
		avcodec_close(codec_context);
		codec_context = NULL;
	}
	if (picture) {
		picture = NULL;
	}

	if (pic) {
	  delete [] pic->data[0];
	  delete pic;
	}

	if(frontend) {
	  frontend = NULL;
	}
}

static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
    char *filename)
{
  FILE *f;
  int i;

  f=fopen(filename,"w");
  fprintf(f,"P5\n%d %d\n%d\n",xsize,ysize,255);
  for(i=0;i<ysize;i++)
    fwrite(buf + i * wrap,1,xsize,f);
  fclose(f);
}

static int frame_count = 0;
static std::vector<uint8_t> buffer;
int decoder_renderer_submit_decode_unit(PDECODE_UNIT decodeUnit)
{
  int len = 0;
  char buf[1024];
  // Read data into the stored frame buffer
  PLENTRY entry = decodeUnit->bufferList;
  while (entry != NULL) {
    std::copy(entry->data, entry->data + entry->length, std::back_inserter(buffer));
    entry = entry->next;
  }

  // Check if we have a completed frame
  if (buffer.empty()) {
    return DR_OK;
  }

  int got_picture = 0;
  AVPacket packet;

  av_init_packet(&packet);
  packet.data = buffer.data();
  packet.size = buffer.size();
  len = avcodec_decode_video2(codec_context, picture, &got_picture, &packet);
  if(len < 0) {
    esyslog("Error while decoding frame");
    buffer.clear();
    return DR_NEED_IDR;
  }
  if (got_picture) {
    isyslog("Frame Count: %i", frame_count++);
    if(frontend) {
      sws_scale(sws_context, picture->data, picture->linesize, 0, picture->height, pic->data, pic->linesize);
      frontend->VideoFrame(pic->data[0], picture->width, picture->height, GAME_RENDER_FMT_0RGB8888);
    }
    else {
      // Dump the latest image to a file
      snprintf(buf, sizeof(buf), "test.pgm");
      pgm_save(picture->data[0], picture->linesize[0], picture->width, picture->height, buf);
    }
    buffer.clear();
  }
  return DR_OK;
}

DECODER_RENDERER_CALLBACKS MOONLIGHT::getDecoderCallbacks()
{
  DECODER_RENDERER_CALLBACKS callbacks = {
      decoder_renderer_setup,
      decoder_renderer_cleanup,
      decoder_renderer_submit_decode_unit
  };
  return callbacks;
}
