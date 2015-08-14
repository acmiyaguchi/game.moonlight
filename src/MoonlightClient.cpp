/*
 *      Copyright (C) 2015 Anthony Miyaguchi
 *      Copyright (C) 2015 Team XBMC
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
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
#include "MoonlightClient.h"
#include "log/Log.h"
#include "nvstream/NvHTTP.h"
#include "nvstream/PairingManager.h"
#include "settings/Settings.h"
#include "callbacks/Callbacks.h"

#include <iostream>
#include <stddef.h>

using namespace MOONLIGHT;

CMoonlightClient::CMoonlightClient(std::string host)
  : m_host(host)
{
  m_http = new NvHTTP(m_host.c_str(), Settings::Get().getUniqueId());
}

MOONLIGHT::CMoonlightClient::~CMoonlightClient()
{
  delete m_http;
}

void CMoonlightClient::start()
{
  STREAM_CONFIGURATION config;
  config.width = 800;
  config.height = 600;
  config.fps = 30;
  config.bitrate = 3500;
  config.packetSize = 1024;

  DECODER_RENDERER_CALLBACKS video_cb = getDecoderCallbacks();
  CONNECTION_LISTENER_CALLBACKS conn_cb = getConnectionCallbacks();
  AUDIO_RENDERER_CALLBACKS audio_cb = getAudioCallbacks();

  isyslog("CMoonlightClient::start: Starting moonlight");

  auto appList = m_http->getAppList();
  if(appList.empty()) {
    esyslog("Empty app list");
    return;
  }

  auto app = appList[0];
  isyslog("AppTitle: %s ID: %i", app.getAppName().c_str(), app.getAppId());

  isyslog("CMoonlightClient::start: Launching app %s", app.getAppName().c_str());
  bool launched = m_http->launchApp(&config, app.getAppId(), false, false);
  int num_retries = 5;
  if(!launched) {
	  for(int i = 0; i < num_retries; i++) {
		  isyslog("CMoonlightClient::start: retrying launch...");
		  launched = m_http->launchApp(&config, app.getAppId(), false, false);
		  if (launched) {
			  break;
		  }
	  }
	  if(!launched) {
		  return;
	  }
  }

  LiStartConnection(m_host.c_str(), &config, &conn_cb, &video_cb, &audio_cb, NULL, 0, 0);
}

void CMoonlightClient::stop()
{
  isyslog("CMoonlightClient::stop: Stopping application and closing gamestream");
  LiStopConnection();
  m_http->quitApp();
}

bool CMoonlightClient::pair()
{
  std::string serverInfo = m_http->getServerInfo(Settings::Get().getUniqueId());
  if(serverInfo.empty()) {
	  return false;
  }
  if (m_http->getPairState(serverInfo) == PairState::PAIRED)
  {
     isyslog("Already paired");
  }
  else if (m_http->getCurrentGame(serverInfo) != 0) {
    isyslog("Computer is currently in a game. You must close the game before pairing.");
    return false;
  }
  else
  {
    std::string pin = PairingManager::generatePinString();
    isyslog("Pin to pair: %s\n", pin.c_str());

    PairState pair_state = m_http->pair(pin);
    switch (pair_state)
    {
    case PairState::PIN_WRONG:
      esyslog("Incorrect PIN");
      return false;
    case PairState::FAILED:
      esyslog("Pairing failed");
      return false;
    case PairState::PAIRED:
      isyslog("Paired successfully");
      break;
    }
  }

  return true;
}
