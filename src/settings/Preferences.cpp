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
#include "Preferences.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>

using namespace MOONLIGHT;

Preferences::Preferences()
{
  init(RES_720_60, false);
}

Preferences::Preferences(ResolutionType res, bool fullscreen)
{
  init(res, fullscreen);
}

Preferences::~Preferences()
{
  if (m_resolution)
  {
    delete m_resolution;
  }
}

void Preferences::init(ResolutionType res, bool fullscreen)
{
  srand(time(0));
  char buf[20];
  sprintf(buf, "%016x", rand());

  m_resolution = new Resolution(res);
  m_bitrate = m_resolution->getBitrate();
  m_fullscreen = fullscreen;
  m_host = "GeForce PC host";
  m_uid = buf;
}

std::string Preferences::getHost() const
{
  return m_host;
}

void Preferences::setHost(char* host)
{
  m_host = host;
}

Resolution Preferences::getResolution() const
{
  return *m_resolution;
}

int Preferences::getBitrate() const
{
  return m_bitrate;
}

bool Preferences::getFullscreen() const
{
  return m_fullscreen;
}

void Preferences::setResolution(ResolutionType res)
{
  delete m_resolution;
  m_resolution = new Resolution(res);
}

void Preferences::setBitrate(int bitrate)
{
  m_bitrate = bitrate;
}

void Preferences::setFullscreen(bool fullscreen)
{
  m_fullscreen = fullscreen;
}

const std::string Preferences::getUniqueId() const
{
  return m_uid;
}
