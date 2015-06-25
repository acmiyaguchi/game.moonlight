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