#include "Callbacks.h"
#include "log/Log.h"

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
		av_freep(codec_context);
		codec_context = NULL;
	}
	if (picture) {
		av_freep(picture);
		picture = NULL;
	}
}

int decoder_renderer_submit_decode_unit(PDECODE_UNIT decodeUnit)
{
	int got_picture = 0;
	int len = 0;

	isyslog("START DECODE UNIT");
	PLENTRY entry = decodeUnit->bufferList;
	while (entry != NULL) {
		AVPacket packet;
		av_init_packet(&packet);
		packet.data = (uint8_t*)entry->data;
		packet.size = entry->length;
		len = avcodec_decode_video2(codec_context, picture, &got_picture, &packet);
		if(len < 0) {
			esyslog("Error while decoding frame");
		}
		if (got_picture) {
			isyslog("GREAT SUCCESS");
		}
		entry = entry->next;
	}
  return 0;
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
