#include "Preferences.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>

using namespace std;

Preferences::Preferences()
{
	init(RES_720_60, false);
}

Preferences::Preferences(RES_TYPE res, bool fullscreen)
{
	init(res, fullscreen);
}

Preferences::~Preferences()
{
	if(m_resolution) {
		delete m_resolution;
	}
}

void Preferences::init(RES_TYPE res, bool fullscreen)
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

void Preferences::setResolution(RES_TYPE res)
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

