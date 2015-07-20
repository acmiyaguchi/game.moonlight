#include "Callbacks.h"
#include "log/Log.h"

using namespace MOONLIGHT;

void audio_renderer_setup()
{
  isyslog("Initialize audio");
}

void audio_renderer_cleanup()
{
}

void audio_renderer_decode_and_play_sample(char* sampleData, int sampleLength)
{
}

AUDIO_RENDERER_CALLBACKS MOONLIGHT::getAudioCallbacks()
{
  AUDIO_RENDERER_CALLBACKS callbacks = {
      audio_renderer_setup,
      audio_renderer_cleanup,
      audio_renderer_decode_and_play_sample
  };
  return callbacks;
}
