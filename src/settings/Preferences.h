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

#include <string>
#include "Resolution.h"

class Preferences
{
public:
	Preferences();

	Preferences(ResolutionType res, bool fullscreen);

	virtual ~Preferences();

	std::string getHost() const;

	void setHost(char* host);

	Resolution getResolution() const;

	int getBitrate() const;

	bool getFullscreen() const;

	void setResolution(ResolutionType res);

	void setBitrate(int bitrate);

	void setFullscreen(bool fullscreen);

	const std::string getUniqueId() const;

private:
	void init(ResolutionType res, bool fullscreen);

	Resolution* m_resolution;
	int m_bitrate;
	bool m_fullscreen;
	std::string m_host;
	std::string m_uid;
};