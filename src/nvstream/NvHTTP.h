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
#pragma once

#include "PairingManager.h"
#include <string>

namespace MOONLIGHT
{
  class PairingManager;
  class CertKeyPair;

  class NvHTTP
  {
  public:
    NvHTTP(const char* host, std::string uid);

    virtual ~NvHTTP();

    std::string getXmlString(std::string str, std::string tagname);

    std::string getServerInfo(std::string uid);

    std::string getServerVersion(std::string serverInfo);

    PairState getPairState();

    PairState getPairState(std::string serverInfo);

    std::string getAppListRaw();

    int getCurrentGame(std::string serverInfo);

    PairState pair(std::string pin);

    std::string openHttpConnection(std::string url, bool enableReadTimeout);

    std::string baseUrlHttps;
    std::string baseUrlHttp;
  private:
    PairingManager* m_pm;
    CertKeyPair*    m_cert;
    std::string     m_uid;
  };
}
