#include "Callbacks.h"
#include "log/Log.h"
#include "MoonlightEnvironment.h"
#include "kodi/libKODI_game.h"
#include "Limelight.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/mem.h>
}

using namespace MOONLIGHT;

static AVCodec* codec = NULL;
static AVCodecContext* codec_context = NULL;
static AVFrame* picture = NULL;
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
  parser = av_parser_init(AV_CODEC_ID_H264);
  if(!parser) {
	  esyslog("Cannot create h264 parser");
  }

  frontend = CMoonlightEnvironment::Get().GetFrontend();
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
		av_freep(picture);
		picture = NULL;
	}
	if(frontend) {
	  frontend = NULL;
	}
}

void decode_frame(uint8_t* data, int size) {
  int got_picture = 0;
  int len = 0;
  AVPacket packet;
  av_init_packet(&packet);
  packet.data = data;
  packet.size = size;
  len = avcodec_decode_video2(codec_context, picture, &got_picture, &packet);
  if(len < 0) {
    esyslog("Error while decoding frame");
  }
  if (got_picture == 0) {
    return;
  }
  if(frontend) {
    isyslog("Dump video out");
    frontend->VideoFrame(picture->data[0], picture->width, picture->height, GAME_RENDER_FMT_YUV420P);
  }
}

static std::vector<uint8_t> buffer;

int decoder_renderer_submit_decode_unit(PDECODE_UNIT decodeUnit)
{
	int len = 0;
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
	uint8_t* data = NULL;
	int size = 0;
	len = av_parser_parse2(parser, codec_context, &data, &size, &buffer[0], buffer.size(), 0, 0, AV_NOPTS_VALUE);
	if(size == 0 && len >= 0) {
	  return DR_OK;
	}

	if(len) {
	  decode_frame(&buffer[0], size);
	  buffer.erase(buffer.begin(), buffer.begin() + len);
	  return DR_OK;
	}

  buffer.erase(buffer.begin(), buffer.end());
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
