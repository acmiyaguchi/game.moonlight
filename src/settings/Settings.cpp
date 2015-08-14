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
#include "log/Log.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <openssl/rand.h>

using namespace MOONLIGHT;

#define SETTING_HOST		"host"
#define SETTING_RESOLUTION	"resolution"
#define SETTING_FULLSCREEN	"fullscreen"
#define SETTING_FRAMERATE	"framerate"
#define SETTING_BITRATE		"bitrate"

#define UID_CHARS 16
namespace {
  const char *uid_filename = "uniqueid.dat";
}

Settings::Settings()
  :m_initialized(false)
{
  init(RES_720_60, false);
}

Settings& MOONLIGHT::Settings::Get() {
	static Settings _instance;
	return _instance;
}

Settings::~Settings()
{
  if (m_resolution)
  {
    delete m_resolution;
  }
}

void Settings::SetSetting(const std::string& strName, const void* value) {
	if (strName == SETTING_HOST)
	{
		m_host = static_cast<const char*>(value);
		dsyslog("Setting \"%s\" set to %s", SETTING_HOST, m_host.c_str());
	}
	else if (strName == SETTING_RESOLUTION)
	{
		int res = atoi(static_cast<const char*>(value));
		switch(res)
		{
		case 720:
			m_resolution->setDimensions(1280, 720);
			break;
		case 768:
			m_resolution->setDimensions(1366, 768);
			break;
		case 900:
			m_resolution->setDimensions(1600, 900);
			break;
		case 1050:
			m_resolution->setDimensions(1680, 1050);
			break;
		case 1080:
			m_resolution->setDimensions(1920, 1080);
			break;
		default:
			dsyslog("invalid dimension of %i", res);
		}
		dsyslog("Setting \"%s\" set to %i x %i",
				SETTING_RESOLUTION, m_resolution->getWidth(), m_resolution->getHeight());
	}
	else if (strName == SETTING_FULLSCREEN)
	{
		m_fullscreen = *static_cast<const bool*>(value);
		dsyslog("Setting \"%s\" set to %s", SETTING_FULLSCREEN, m_fullscreen ? "true" : "false");
	}
	else if (strName == SETTING_FRAMERATE)
	{
		int framerate = atoi(static_cast<const char*>(value));
		m_resolution->setFramerate(framerate);
		dsyslog("Setting \"%s\" set to %i", SETTING_FRAMERATE, m_resolution->getFramerate());
	}
	else if (strName == SETTING_BITRATE)
	{
		int bitrate = atoi(static_cast<const char*>(value));
		m_resolution->setBitrate(bitrate);
		dsyslog("Setting \"%s\" set to %i", SETTING_BITRATE, m_resolution->getBitrate());
	}

	m_initialized = true;
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
  m_fullscreen = fullscreen;
  m_host = "GeForce PC host";
  m_uid = buf;
}
