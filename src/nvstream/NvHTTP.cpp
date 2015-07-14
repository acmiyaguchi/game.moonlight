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
#include <locale>

using namespace MOONLIGHT;
using curl::curl_easy;

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

  // Options for curl
  m_curl.add(curl_pair<CURLoption, long>(CURLOPT_SSL_VERIFYHOST, 0L));
  m_curl.add(curl_pair<CURLoption, long>(CURLOPT_SSLENGINE_DEFAULT, 1L));
  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_SSLCERTTYPE, "PEM"));
  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_SSLCERT, certFileName.c_str()));
  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_SSLKEYTYPE, "PEM"));
  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_SSLKEY, keyFileName.c_str()));
  m_curl.add(curl_pair<CURLoption, long>(CURLOPT_SSL_VERIFYPEER, 0L));
  m_curl.add(curl_pair<CURLoption, long>(CURLOPT_FAILONERROR, 1L));
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
  if(result) {
    isyslog("Properly parsed");
  }
  else {
    esyslog("Did not properly load xml\n");
    return "";
  }

  for (auto tool = doc.child("root"); tool; tool.next_sibling())
  {
    isyslog("Name: %s Value: %s\n", tool.name(), tool.value());
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
  getXmlString(serverInfo, "appversion");
}

PairState NvHTTP::getPairState()
{
  return m_pm->getPairState(getServerInfo(m_uid));
}

PairState NvHTTP::getPairState(std::string serverInfo)
{
  return m_pm->getPairState(serverInfo);
}

int NvHTTP::getCurrentGame(std::string serverInfo)
{
  return -1;
}

PairState NvHTTP::pair(std::string pin)
{
  return m_pm->pair(m_uid, pin);
}

std::string NvHTTP::openHttpConnection(std::string url, bool enableReadTimeout)
{
  isyslog("Opening url: %s", url.c_str());
  std::stringstream data;

  // watch the data coming into curl
  curl_writer writer(data);
  curl_writer easy(writer);

  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_URL, url));
  try {
    m_curl.perform();
  }
  catch (curl_easy_exception & error) {
    error.print_traceback();
  }

  return data.str();
}

