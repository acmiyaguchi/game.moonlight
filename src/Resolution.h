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

enum RES_TYPE {
	RES_720_30, 
	RES_720_60, 
	RES_768_30, 
	RES_768_60,
	RES_900_30, 
	RES_900_60,
	RES_1050_30, 
	RES_1050_60,
	RES_1080_30, 
	RES_1080_60
};

class Resolution
{
public:
	Resolution(RES_TYPE type);

	int getWidth() const;

	int getHeight() const;

	int getFramerate() const;

	int getBitrate() const;
	
private:
	void setResolution(int width, int height, int framerate, int bitrate);
	int m_width;
	int m_height;
	int m_framerate;
	int m_bitrate;
};