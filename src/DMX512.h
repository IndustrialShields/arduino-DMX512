#ifndef __DMX512_H__
#define __DMX512_H__

#include <Arduino.h>

#if !defined(MDUINO_PLUS)
#error "Device not supported"
#endif

class DMX512Class {
	public:
		explicit DMX512Class();

	public:
		void begin();
		bool busy();
		bool write(const uint8_t *channels, uint16_t numChannels);

		inline bool idle() { return !busy(); }

	private:
		void writeBreak();
		void writeConfiguration();
};

extern DMX512Class DMX512;

#endif // __DMX512_H__
