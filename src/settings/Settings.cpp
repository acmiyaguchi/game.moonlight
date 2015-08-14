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
#include "Settings.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <openssl/rand.h>

using namespace MOONLIGHT;

#define UID_CHARS 16
namespace {
  const char *uid_filename = "uniqueid.dat";
}

Settings::Settings()
{
  init(RES_720_60, false);
}

Settings::Settings(ResolutionType res, bool fullscreen)
{
  init(res, fullscreen);
}

Settings::~Settings()
{
  if (m_resolution)
  {
    delete m_resolution;
  }
}

void Settings::init(ResolutionType res, bool fullscreen)
{
  char buf[UID_CHARS+1];
  FILE *fd = fopen(uid_filename, "r");
  if (fd == NULL) {
    unsigned char unique_data[8];
    RAND_bytes(unique_data, 8);
    for (int i = 0; i < 8; i++) {
      sprintf(buf + (i * 2), "%02x", unique_data[i]);
    }
    buf[UID_CHARS] = 0;
    fd = fopen(uid_filename, "w");
    fwrite(buf, UID_CHARS, 1, fd);
  }
  else {
    fread(buf, UID_CHARS, 1, fd);
    buf[UID_CHARS] = 0;
  }
  fclose(fd);

  m_resolution = new Resolution(res);
  m_bitrate = m_resolution->getBitrate();
  m_fullscreen = fullscreen;
  m_host = "GeForce PC host";
  m_uid = buf;
}

std::string Settings::getHost() const
{
  return m_host;
}

void Settings::setHost(char* host)
{
  m_host = host;
}

Resolution Settings::getResolution() const
{
  return *m_resolution;
}

int Settings::getBitrate() const
{
  return m_bitrate;
}

bool Settings::getFullscreen() const
{
  return m_fullscreen;
}

void Settings::setResolution(ResolutionType res)
{
  delete m_resolution;
  m_resolution = new Resolution(res);
}

void Settings::setBitrate(int bitrate)
{
  m_bitrate = bitrate;
}

void Settings::setFullscreen(bool fullscreen)
{
  m_fullscreen = fullscreen;
}

const std::string Settings::getUniqueId() const
{
  return m_uid;
}
