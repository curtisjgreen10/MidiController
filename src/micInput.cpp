/*
 * micInput.cpp
 *
 *  Created on: Feb 6, 2021
 *      Author: cjgree13
 */


#include <Windows.h>
#include <MMSystem.h>
#include <iostream>
#include "micInput.h"

using namespace std;

MicInput::MicInput(int micNum)
{
	// set mic number, should be 1-4
	micNumber = micNum;
}

int MicInput::getMicNumber()
{
	return micNumber;
}

int MicInput::recordWavData()
{
	return 0;
}
