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
#include "NvHTTP.h"
#include "log/Log.h"
#include "CertKeyPair.h"
#include "PairingManager.h"
#include <sstream>
#include <fstream>
#include "pugixml.hpp"
#include "http.h"
#include "Limelight.h"
#include <openssl/rand.h>

using namespace MOONLIGHT;

namespace
{
  std::string certFileName = "client.pem";
  std::string p12FileName = "client.p12";
  std::string keyFileName = "key.pem";
  const int HTTPS_PORT = 47984;
  const int HTTP_PORT = 47989;
  const int CONNECTION_TIMEOUT = 3000;
  const int READ_TIMEOUT = 5000;
}

NvHTTP::NvHTTP(const char* host, std::string uid) :
  m_uid(uid)
{
  std::stringstream ss;
  ss << "https://" << host << ":" << HTTPS_PORT;
  baseUrlHttps = ss.str();

  ss.str("");
  ss << "http://" << host << ":" << HTTP_PORT;
  baseUrlHttp = ss.str();

  m_cert = new CertKeyPair(certFileName, p12FileName, keyFileName);
  m_pm = new PairingManager(this, m_cert);
  http_init();
}

NvHTTP::~NvHTTP()
{
  delete m_pm;
  delete m_cert;
}

std::string NvHTTP::getXmlString(std::string str, std::string tagname)
{
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_buffer(str.c_str(),str.size(), pugi::parse_default, pugi::encoding_auto);
  if(!result) {
    esyslog("Did not properly load xml\n");
    return "";
  }

  pugi::xml_node node = doc.child("root").child(tagname.c_str());
  if(node) {
    isyslog("%s found with value %s", tagname.c_str(), node.child_value());
    return std::string(node.child_value());
  }

  return "";
}

std::string NvHTTP::getServerInfo(std::string uid)
{
  std::stringstream url;
  url << baseUrlHttps << "/serverinfo?uniqueid=" << m_uid;

  std::string resp = openHttpConnection(url.str(), true);

  // TODO: Incomplete function, need to default to http in the case that the
  // client is unpaired with the host

  return resp;
}

std::string NvHTTP::getServerVersion(std::string serverInfo)
{
  return getXmlString(serverInfo, "appversion");
}

PairState NvHTTP::getPairState()
{
  return m_pm->getPairState(getServerInfo(m_uid));
}

PairState NvHTTP::getPairState(std::string serverInfo)
{
  return m_pm->getPairState(serverInfo);
}

std::string NvHTTP::getAppListRaw()
{
  std::stringstream url;
  url << baseUrlHttps << "/applist?uniqueid=" << m_uid;
  return openHttpConnection(url.str(), true);
}

int NvHTTP::getCurrentGame(std::string serverInfo)
{
  std::string game = getXmlString(serverInfo, "currentgame");
  return atoi(game.c_str());
}

PairState NvHTTP::pair(std::string pin)
{
  return m_pm->pair(m_uid, pin);
}

std::string NvHTTP::openHttpConnection(std::string url, bool enableReadTimeout)
{
  isyslog("Opening connection to %s", url.c_str());
  std::stringstream ss;
  http_data* data = http_create_data();
  http_request((char*)url.c_str(), data);
  ss.write(data->memory, data->size);
  http_free_data(data);
  return ss.str();
}

NvApp MOONLIGHT::NvHTTP::getAppById(int id)
{
  return NvApp("");
}

std::vector<NvApp> MOONLIGHT::NvHTTP::getAppList()
{
  std::stringstream url;
  url << baseUrlHttps << "/applist?uniqueid=" << m_uid;
  std::string resp = openHttpConnection(url.str(), true);
  return getAppList(resp);
}

int MOONLIGHT::NvHTTP::launchApp(STREAM_CONFIGURATION* config, int appId, bool sops, bool localaudio)
{
  initializeConfig(config);
  uint32_t rikey = 1;
  std::stringstream url;
  url << baseUrlHttps << "/launch?uniqueid=" << m_uid
      << "&appid=" << appId
      << "&mode=" << config->width << "x" << config->height << "x" << config->fps
      << "&additionalStates=1&sops=" << (int)sops
      << "&rikey=" << m_pm->bytesToHex((unsigned char*)config->remoteInputAesKey, 16)
      << "&rikeyid=" << rikey
      << "&localAudioPlayMode=" << (int)localaudio;
  std::string resp = openHttpConnection(url.str(), false);

}

bool MOONLIGHT::NvHTTP::resumeApp(STREAM_CONFIGURATION* config)
{
  initializeConfig(config);
  uint32_t rikey = 1;
  std::stringstream url;
  url << baseUrlHttps << "/resume?uniqueid=" << m_uid
      << "&rikey=" << m_pm->bytesToHex((unsigned char*)config->remoteInputAesKey, 16)
      << "&rikeyid=" << rikey;
  std::string resp = openHttpConnection(url.str(), false);
}

void MOONLIGHT::NvHTTP::initializeConfig(STREAM_CONFIGURATION* config)
{
  RAND_bytes((unsigned char*)config->remoteInputAesKey, 16);
  memset(config->remoteInputAesIv, 0, 16);
}

std::vector<NvApp> MOONLIGHT::NvHTTP::getAppList(std::string input)
{
  std::vector<NvApp> appList;
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_buffer(input.c_str(), input.size(), pugi::parse_default, pugi::encoding_auto);
  if(!result) {
    esyslog("Did not properly load xml\n");
    return appList;
  }

  pugi::xml_node node = doc.child("root").child("App");
  for(; node; node = node.next_sibling()) {
    isyslog("App Found: %s", node.child_value("AppTitle"));
    std::string title = node.child_value("AppTitle");
    std::string id = node.child_value("ID");
    std::string running = node.child_value("IsRunning");
    NvApp app(title);
    app.setAppId(id);
    app.setIsRunning(running);
    appList.push_back(app);
  }

  return appList;
}


bool MOONLIGHT::NvHTTP::quitApp()
{
  return false;
}

void MOONLIGHT::NvHTTP::unpair()
{
}
