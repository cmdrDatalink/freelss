/*
 ****************************************************************************
 *  Copyright (c) 2014 Uriah Liggett <hairu526@gmail.com>                   *
 *	This file is part of FreeLSS.                                           *
 *                                                                          *
 *  FreeLSS is free software: you can redistribute it and/or modify         *
 *  it under the terms of the GNU General Public License as published by    *
 *  the Free Software Foundation, either version 3 of the License, or       *
 *  (at your option) any later version.                                     *
 *                                                                          *
 *  FreeLSS is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *  GNU General Public License for more details.                            *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with FreeLSS.  If not, see <http://www.gnu.org/licenses/>.       *
 ****************************************************************************
*/

#include "Main.h"
#include "RelayLaser.h"
#include "Thread.h"
#include "Settings.h"

namespace scanner
{

RelayLaser::RelayLaser() :
	m_rightLaserPin(-1),
	m_leftLaserPin(-1),
	m_laserDelay(-1),
	m_laserOnValue(-1),
	m_laserOffValue(-1),
	m_rightLaserOn(false),
	m_leftLaserOn(false)
{
	Settings * settings = Settings::get();
	m_rightLaserPin = settings->readInt(Settings::GENERAL_SETTINGS, Settings::RIGHT_LASER_PIN);
	m_leftLaserPin = settings->readInt(Settings::GENERAL_SETTINGS, Settings::LEFT_LASER_PIN);
	m_laserDelay = settings->readInt(Settings::GENERAL_SETTINGS, Settings::LASER_DELAY);
	m_laserOnValue = settings->readInt(Settings::GENERAL_SETTINGS, Settings::LASER_ON_VALUE);
	m_laserOffValue = m_laserOnValue ? 0 : 1;
}

RelayLaser::~RelayLaser()
{
	turnOff(Laser::ALL_LASERS);
}

void RelayLaser::initialize()
{
	Settings * settings = Settings::get();
	int rightLaserPin = settings->readInt(Settings::GENERAL_SETTINGS, Settings::RIGHT_LASER_PIN);
	int leftLaserPin = settings->readInt(Settings::GENERAL_SETTINGS, Settings::LEFT_LASER_PIN);
	int laserOnValue = settings->readInt(Settings::GENERAL_SETTINGS, Settings::LASER_ON_VALUE);
	int laserOffValue = laserOnValue ? 0 : 1;

	// Setup the pin but disable the laser
	pinMode(rightLaserPin, OUTPUT);
	pinMode(leftLaserPin, OUTPUT);
	digitalWrite (rightLaserPin, laserOffValue);
	digitalWrite (leftLaserPin, laserOffValue);
}

void RelayLaser::turnOn(Laser::LaserSide laser)
{
	if (laser == Laser::RIGHT_LASER || laser == Laser::ALL_LASERS)
	{
		digitalWrite (m_rightLaserPin, m_laserOnValue);
		m_rightLaserOn = true;
	}

	if (laser == Laser::LEFT_LASER || laser == Laser::ALL_LASERS)
	{
		digitalWrite (m_leftLaserPin, m_laserOnValue);
		m_leftLaserOn = true;
	}

	Thread::usleep(m_laserDelay);
}

void RelayLaser::turnOff(Laser::LaserSide laser)
{
	if (laser == Laser::RIGHT_LASER || laser == Laser::ALL_LASERS)
	{
		digitalWrite (m_rightLaserPin, m_laserOffValue);
		m_rightLaserOn = false;
	}

	if (laser == Laser::LEFT_LASER || laser == Laser::ALL_LASERS)
	{
		digitalWrite (m_leftLaserPin, m_laserOffValue);
		m_leftLaserOn = false;
	}
}

bool RelayLaser::isOn(Laser::LaserSide laser)
{
	bool on = false;

	if (laser == Laser::ALL_LASERS)
	{
		on = m_rightLaserOn && m_leftLaserOn;
	}
	else if (laser == Laser::RIGHT_LASER)
	{
		on = m_rightLaserOn;
	}
	else if (laser == Laser::LEFT_LASER)
	{
		on = m_leftLaserOn;
	}

	return on;
}

}
