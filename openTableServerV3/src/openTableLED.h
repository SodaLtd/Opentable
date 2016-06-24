#pragma once

#include "openTableSerialDevice.h"

class openTableLED : public openTableSerialDevice {
public:
	openTableLED() {};
	~openTableLED() {};
	//
	//
	//
	void setStation(int station, bool on);
protected:
	virtual void processBuffer() {};
};