/*
 *      Copyright (C) 2015 Anthony Miyaguchi
 *      Copyright (C) 2015 Team XBMC
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#include "Callbacks.h"
#include "log/Log.h"
#include "MoonlightEnvironment.h"
#include "kodi/libKODI_game.h"
#include "Limelight.h"

#include <platform/threads/threads.h>
#include <queue>

using namespace MOONLIGHT;
using namespace PLATFORM;

typedef std::vector<uint8_t> decode_unit;
const int MAX_PACKET_LENGTH = 64;

static CHelper_libKODI_game* frontend = NULL;

static std::queue<decode_unit> data_queue;
static std::queue<decode_unit> render_queue;
static CMutex mutex;
static CEvent event;
static int m_width = 0;
static int m_height = 0;

void* decode_and_render(void* args);

void decoder_renderer_setup(int width, int height, int redrawRate, void* context, int drFlags)
{
  isyslog("VideoCallbacks::Setup");

  thread_t thread;
  ThreadsCreate(thread, decode_and_render, NULL);

  m_width = width;
  m_height = height;
  frontend = CMoonlightEnvironment::Get().GetFrontend();
}

void decoder_renderer_cleanup()
{
	isyslog("VideoCallbacks::Cleanup");

	if(frontend) {
	  frontend = NULL;
	}
}

void* decode_and_render(void* args)
{
  for(;;) {
    // Grab lock, grab data, unlock
    if(render_queue.empty())
    {
      mutex.Lock();
      if(data_queue.empty())
      {
        mutex.Unlock();
        event.Wait(); // wait for more data on the queue
        continue;
      }
      else
      {
        render_queue.swap(data_queue);
      }
      mutex.Unlock();
    }

    decode_unit buffer = render_queue.front();
    render_queue.pop();

    frontend->VideoFrameH264(buffer.data(), buffer.size(), m_width, m_height);
  }
}

int decoder_renderer_submit_decode_unit(PDECODE_UNIT decodeUnit)
{
  int len = 0;

  decode_unit buffer;
  PLENTRY entry = decodeUnit->bufferList;
  while (entry != NULL)
  {
    std::copy(entry->data, entry->data + entry->length, std::back_inserter(buffer));
    entry = entry->next;
  }

  mutex.Lock();
  if(data_queue.size() < MAX_PACKET_LENGTH)
  {
    data_queue.push(buffer);
    event.Signal();
  }
  else
  {
    isyslog("Discarding packet due to queue overflow");
  }
  mutex.Unlock();

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
