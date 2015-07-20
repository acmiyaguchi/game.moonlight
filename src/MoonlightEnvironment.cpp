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

#include "MoonlightEnvironment.h"
#include "MoonlightClient.h"
#include "kodi/libXBMC_addon.h"
#include "kodi/libKODI_game.h"

using namespace ADDON;
using namespace MOONLIGHT;


CMoonlightEnvironment::CMoonlightEnvironment(void)
  : m_kodi(NULL), m_frontend(NULL), m_client(NULL)
{
}

CMoonlightEnvironment& CMoonlightEnvironment::Get(void)
{
  static CMoonlightEnvironment _instance;
  return _instance;
}

void CMoonlightEnvironment::Initialize(CHelper_libXBMC_addon* kodi, CHelper_libKODI_game* frontend, CMoonlightClient* client)
{
  m_kodi = kodi;
  m_frontend = frontend;
  m_client = client;
}

void MOONLIGHT::CMoonlightEnvironment::Deinitialize(void)
{
}

