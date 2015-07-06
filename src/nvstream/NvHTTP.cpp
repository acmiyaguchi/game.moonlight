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
#include "PairingManager.h"
#include <sstream>
#include <tinyxml.h>

using namespace MOONLIGHT;
using curl::curl_easy;

namespace
{
  const char *pCertFile = "./client.pem";
  const char *pKeyFile = "./key.pem";
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

  m_pm = new PairingManager(this);

  // Options for curl
  m_curl.add(curl_pair<CURLoption, long>(CURLOPT_SSL_VERIFYHOST, 0L));
  m_curl.add(curl_pair<CURLoption, long>(CURLOPT_SSLENGINE_DEFAULT, 1L));
  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_SSLCERTTYPE, "PEM"));
  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_SSLCERT, pCertFile));
  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_SSLKEYTYPE, "PEM"));
  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_SSLKEY, pKeyFile));
  m_curl.add(curl_pair<CURLoption, long>(CURLOPT_SSL_VERIFYPEER, 0L));
  m_curl.add(curl_pair<CURLoption, long>(CURLOPT_FAILONERROR, 1L));
}

NvHTTP::~NvHTTP()
{
  delete m_pm;
}

std::string NvHTTP::getXmlString(std::string str, std::string tagname)
{
  TiXmlDocument doc;
  doc.Parse(str.c_str(), 0, TIXML_ENCODING_UTF8);

  TiXmlElement* child = doc.FirstChild("root")->ToElement();

  const char* attribute;
  for (child; child; child = child->NextSiblingElement())
  {
    attribute = child->Attribute(tagname.c_str());
    if (attribute)
    {
      break;
    }
  }
  std::string ret(attribute);
  return ret;
}

std::string NvHTTP::getServerInfo(std::string uid)
{
  return "";
}

PairState NvHTTP::getPairState(std::string serverInfo)
{
  return PairState::FAILED;
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
  std::stringstream data;

  // watch the data coming into curl
  curl_writer writer(data);
  curl_writer easy(writer);

  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_URL, url));
  try {
    m_curl.perform();
  }
  catch (curl_easy_exception error) {
    error.print_traceback();
  }

  return data.str();
}
