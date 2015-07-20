#include "Callbacks.h"
#include "log/Log.h"

using namespace MOONLIGHT;

void decoder_renderer_setup(int width, int height, int redrawRate, void* context, int drFlags)
{
  isyslog("Initialize the decoder");
}

void decoder_renderer_cleanup()
{
}

int decoder_renderer_submit_decode_unit(PDECODE_UNIT decodeUnit)
{
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
