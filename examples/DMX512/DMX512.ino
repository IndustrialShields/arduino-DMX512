// DMX512 library example
// by Industrial Shields

#include <DMX512.h>

uint8_t channels[] = {0, 80, 160};
const uint16_t numChannels = sizeof(channels) / sizeof(uint8_t);

////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600UL);

  DMX512.begin();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  if (DMX512.idle()) {
    // Wait some time between commands
    delay(100);

    // Update channels brightness
    for (uint16_t i = 0; i < numChannels; ++i) {
      channels[i] += 5;
    }

    // Update channels
    DMX512.write(channels, numChannels);
  }
}
