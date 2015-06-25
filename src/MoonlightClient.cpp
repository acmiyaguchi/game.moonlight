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
#include "MoonlightClient.h"
#include <stddef.h>

void CMoonlightClient::start()
{
	IP_ADDRESS host = 3232235861; //192.168.1.85
	STREAM_CONFIGURATION config;
	config.width = 800;
	config.height = 600;
	config.fps = 60;
	config.bitrate = 1024;
	config.packetSize = 1024;
	LiStartConnection(host, &config, NULL, NULL, NULL, NULL, NULL, 0, 0);
}

void CMoonlightClient::stop()
{
	LiStopConnection();
}

void CMoonlightClient::pair()
{

}