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
#include "MoonlightClient.h"
#include "log/Log.h"
#include "nvstream/NvHTTP.h"
#include "nvstream/PairingManager.h"
#include "settings/Preferences.h"
#include <iostream>
#include <stddef.h>

using namespace MOONLIGHT;

CMoonlightClient::CMoonlightClient()
{

}

void CMoonlightClient::start()
{
  std::string host = "10.0.0.7"; //192.168.1.85
  STREAM_CONFIGURATION config;
  config.width = 800;
  config.height = 600;
  config.fps = 60;
  config.bitrate = 1024;
  config.packetSize = 1024;

  isyslog("CMoonlightClient::start: Starting moonlight");
  LiStartConnection(host.c_str(), &config, NULL, NULL, NULL, NULL, NULL, 0, 0);

}

void CMoonlightClient::stop()
{
  LiStopConnection();
}

void CMoonlightClient::pair(std::string uid, std::string host)
{
  std::string message;
  NvHTTP http(host.c_str(), uid);
  std::string serverInfo = http.getServerInfo(uid);
  if (http.getPairState(serverInfo) == PairState::PAIRED)
  {
    message = "Already paired";
  }
  else
  {
    std::string pin = PairingManager::generatePinString();
    isyslog("Pin to pair: %s\n", pin.c_str());

    PairState pair_state = http.pair(pin);
    switch (pair_state)
    {
    case PairState::PIN_WRONG:
      message = "Incorrect PIN";
      break;
    case PairState::FAILED:
      message = "Pairing failed";
      break;
    case PairState::PAIRED:
      message = "Paired successfully";
      break;
    }
  }
  isyslog("%s", message.c_str());

  if (http.getPairState(serverInfo) == PairState::PAIRED)
  {
    auto appList = http.getAppList();
    if(!appList.empty()) {
      auto app = appList[0];
      isyslog("AppTitle: %s ID: %i", app.getAppName().c_str(), app.getAppId());
    }
  }
  else {
    isyslog("Lies, you didn't actually pair.");
  }
}

void CMoonlightClient::init()
{

}
