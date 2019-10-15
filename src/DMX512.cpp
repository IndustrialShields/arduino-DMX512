/*
   Copyright (c) 2019 Boot&Work Corp., S.L. All rights reserved

   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DMX512.h"

DMX512Class DMX512;

static uint16_t remainingBytes = 0;
static const uint8_t *channels = nullptr;
static uint32_t finishTime = 0UL;

////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(USART3_TX_vect) {
	--remainingBytes;
	if (remainingBytes == 0) {
		UCSR3B &= 0b11110111;
		finishTime = millis();
	} else {
		UDR3 = *channels++;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
DMX512Class::DMX512Class() {

}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DMX512Class::begin() {
	UCSR3B = 0b01000000;
	UCSR3C = 0b00001110;
	UCSR3A |= 0b00000010;
	UBRR3H = 0;
	UBRR3L = 7;
	pinMode(14, OUTPUT);
	pinMode(RS485_DE, OUTPUT);
	pinMode(RS485_RE, OUTPUT);
	digitalWrite(RS485_RE, HIGH);
	digitalWrite(RS485_DE, HIGH);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool DMX512Class::busy() {
	return (remainingBytes == 0) && (millis() - finishTime < 64);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DMX512Class::writeBreak() {
	UCSR3B &= 0b11110111;
	digitalWrite(14, LOW);
	delayMicroseconds(86);
	digitalWrite(14, HIGH);
	delayMicroseconds(7);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DMX512Class::writeConfiguration() {
	while (!(UCSR3A & 0b00100000));
	UDR3 = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool DMX512Class::write(const uint8_t *channelsArray, uint16_t numChannels) {
	if (!channelsArray && (numChannels > 0)) {
		return false;
	}

	remainingBytes = numChannels + 1;
	channels = channelsArray;
	writeBreak();
	UCSR3B |= 0b00001000; 
	writeConfiguration();

	return true;
}

