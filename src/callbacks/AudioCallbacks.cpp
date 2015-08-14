#include "Callbacks.h"
#include "log/Log.h"
#include "MoonlightEnvironment.h"
#include "kodi/libKODI_game.h"

#include <opus/opus.h>

using namespace MOONLIGHT;

#define SAMPLE_RATE 48000
#define CHANNEL_COUNT 2
#define FRAME_SIZE 240

static CHelper_libKODI_game* frontend = NULL;
static OpusDecoder* m_decoder = NULL;
static short pcmBuffer[FRAME_SIZE * CHANNEL_COUNT];

void audio_renderer_setup()
{
  isyslog("Initialize audio");
  m_decoder = opus_decoder_create(SAMPLE_RATE, CHANNEL_COUNT, NULL);
  frontend = CMoonlightEnvironment::Get().GetFrontend();
}

void audio_renderer_cleanup()
{
  if (m_decoder) {
	opus_decoder_destroy(m_decoder);
	m_decoder = NULL;
  }
}

void audio_renderer_decode_and_play_sample(char* sampleData, int sampleLength)
{
  int decodeLen = opus_decode(m_decoder, (const unsigned char*)sampleData, sampleLength, pcmBuffer, FRAME_SIZE, 0);
  if (decodeLen > 0) {
    frontend->AudioFrames((uint8_t*)pcmBuffer, decodeLen*CHANNEL_COUNT*sizeof(short), decodeLen, GAME_AUDIO_FMT_S16NE);
  }
  else {
	esyslog("Opus decode error: %d", decodeLen);
  }
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
