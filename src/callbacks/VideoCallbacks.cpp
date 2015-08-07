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

#include <platform/threads/threads.h>
#include <queue>
#include <cstring>

using namespace MOONLIGHT;
using namespace PLATFORM;

static AVCodec* codec = NULL;
static AVCodecContext* codec_context = NULL;
static AVFrame* picture = NULL;

static struct SwsContext *sws_context = NULL;
static AVPicture* pic = NULL;

static AVCodecParserContext* parser = NULL;
static CHelper_libKODI_game* frontend = NULL;

void* decode_and_render(void* args);

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
  thread_t thread;
  ThreadsCreate(thread, decode_and_render, NULL);
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
typedef std::vector<uint8_t> decode_unit;
static std::queue<decode_unit> data_queue;
static std::queue<decode_unit> render_queue;
static CMutex mutex;
static CEvent event;

void* decode_and_render(void* args)
{
  char buf[1024];
  int got_picture = 0;
  int len = 0;
  AVPacket packet;
  decode_unit buffer;
  av_init_packet(&packet);

  for(;;) {
    // Grab lock, grab data, unlock
    if(render_queue.empty()) {
      mutex.Lock();
      if(data_queue.empty()) {
        mutex.Unlock();
        event.Wait(); // wait for more data on the queue
        continue;
      }
      else {
        render_queue.swap(data_queue);
      }
      mutex.Unlock();
    }

    decode_unit temp = render_queue.front();
    if(got_picture) {
      buffer = temp;
    }
    else {
      buffer.insert(buffer.end(), temp.begin(), temp.end());
    }
    render_queue.pop();

    packet.data = buffer.data();
    packet.size = buffer.size();

    len = avcodec_decode_video2(codec_context, picture, &got_picture, &packet);
    if(len < 0) {
      esyslog("Error while decoding frame");
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
    }
  }
}

const int MAX_PACKET_LENGTH = 64;
int decoder_renderer_submit_decode_unit(PDECODE_UNIT decodeUnit)
{
  int len = 0;
  bool packet_dropped = false;
  // Read data into the stored frame buffer
  decode_unit buffer;
  PLENTRY entry = decodeUnit->bufferList;
  while (entry != NULL) {
    std::copy(entry->data, entry->data + entry->length, std::back_inserter(buffer));
    entry = entry->next;
  }
  mutex.Lock();
  if(data_queue.size() < MAX_PACKET_LENGTH) {
    data_queue.push(buffer);
    event.Signal();
  }
  else {
    isyslog("discarding packet");
    packet_dropped = true;
  }
  mutex.Unlock();
  if(packet_dropped) {
    return DR_NEED_IDR;
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
