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
#include <sstream>

using namespace MOONLIGHT;
using curl::curl_easy;

namespace
{
  const char *pCertFile = "./client.pem";
  const char *pKeyFile = "./key.pem";
}

NvHTTP::NvHTTP()
{
  m_curl.add(curl_pair<CURLoption, long>(CURLOPT_SSL_VERIFYHOST, 0L));
  m_curl.add(curl_pair<CURLoption, long>(CURLOPT_SSLENGINE_DEFAULT, 1L));
  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_SSLCERTTYPE, "PEM"));
  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_SSLCERT, pCertFile));
  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_SSLKEYTYPE, "PEM"));
  m_curl.add(curl_pair<CURLoption, string>(CURLOPT_SSLKEY, pKeyFile));
  m_curl.add(curl_pair<CURLoption, long>(CURLOPT_SSL_VERIFYPEER, 0L));
  m_curl.add(curl_pair<CURLoption, long>(CURLOPT_FAILONERROR, 1L));

}

std::string NvHTTP::getXmlString(std::string str, std::string tagname)
{
  return "";
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
  return PairState::FAILED;
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
