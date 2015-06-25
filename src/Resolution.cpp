#include "Resolution.h"


Resolution::Resolution(RES_TYPE type) {
	switch(type)
	{
	case RES_720_30: 
		setResolution(1280, 720, 30, 5);
		break;
	case RES_720_60: 
		setResolution(1280, 720, 60, 10);
		break;
	case RES_768_30: 
		setResolution(1366, 768, 30, 5);
		break;
	case RES_768_60: 
		setResolution(1366, 768, 60, 10);
		break;
	case RES_900_30: 
		setResolution(1600, 900, 30, 10);
		break;
	case RES_900_60: 
		setResolution(1600, 900, 60, 15);
		break;
	case RES_1050_30: 
		setResolution(1680, 1050, 30, 10);
		break;
	case RES_1050_60: 
		setResolution(1680, 1050, 60, 20);
		break;
	case RES_1080_30: 
		setResolution(1920, 1080, 30, 10);
		break;
	case RES_1080_60: 
		setResolution(1920, 1080, 60, 20);
		break;
	}
}

void Resolution::setResolution(int width, int height, int framerate, int bitrate)
{
	m_width = width;
	m_height = height;
	m_framerate = framerate;
	m_bitrate = bitrate;
}