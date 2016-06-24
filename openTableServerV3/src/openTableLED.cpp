#include "openTableLED.h"

void openTableLED::setStation(int station, bool on) {
	if ( !m_connection.isInitialized() ) return;
	char command = 0;
	switch(station) {
	case 0:
		command = on ? 'a' : 'b';
		break;
	case 2:
		command = on ? 'c' : 'd';
		break;
	case 1:
		command = on ? 'e' : 'f';
		break;
	case 3:
		command = on ? 'g' : 'h';
		break;

	}
	if (command != 0) {
		m_connection.writeByte(command);
	}
}
