/* 
DMX transmitter using Raspberry Pi Pico

DMX is transmitted over 1 signal wire (and ground) in the Pico setup.
You'd need to use a 3-pin fixture and determine if you could short DATA- and GND.
Currently, this program will transmit 1 universe of 3 channels, and set Channel 1 to 174 when the button is pressed.

TODO: Make latching rather than momentary

-- Supposed DMX chart for cheap knockoff SlimPARS (to be used in catwalk to light booth)
1	Dimmer
2	Red
3	Green
4	Blue
5	Strobe
6	Control

-- INFO --
Uses Pico-DMX library: https://github.com/jostlowe/Pico-DMX
Created by DmxOut Smith with help from GitHub Copilot (GPT-4.1)
6-26-2025
*/

#include <Arduino.h>
#include <DmxOutput.h>

#define DMX_PIN 0
#define UNIVERSE_LENGTH 6
#define BUTTON_PIN 16


DmxOutput DmxOut; // Instantiate DmxOutput

uint8_t universe[UNIVERSE_LENGTH + 1];  // Make array size fixed

void setup() {
  DmxOut.begin(DMX_PIN);      // Bind DMX output to pin 0
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configure button pin with internal pull-up
}

void loop() {
  // Button is active LOW: pressed = LOW, released = HIGH
  if (digitalRead(BUTTON_PIN) == LOW) { // If button pressed, turn light ON
    universe[1] = 255; // DIMMER - Button pressed: send 255 on chan 1
    universe[2] = 255; // RED - Button pressed: send 255 on chan 2
    universe[3] = 255; // GREEN - Button pressed: send 255 on chan 3
    universe[4] = 255; // BLUE - Button pressed: send 255 on chan 4
    universe[5] = 0; // STROBE - Button pressed: send 0 on chan 5
    universe[6] = 0; // CONTROL - Button pressed: send *** on chan 6 // CONTROL channel. Not sure what to transmit here yet.
  } else { // If button released, turn light OFF
    universe[1] = 0; // DIMMER - Button pressed: send 0 on chan 1
    universe[2] = 0; // RED - Button pressed: send 0 on chan 2
    universe[3] = 0; // GREEN - Button pressed: send 0 on chan 3
    universe[4] = 0; // BLUE - Button pressed: send 0 on chan 4
    universe[5] = 0; // STROBE - Button pressed: send 0 on chan 5
    universe[6] = 0; // CONTROL - Button pressed: send *** on chan 6 // CONTROL channel. Not sure what to transmit here yet.
  }

  DmxOut.write(universe, UNIVERSE_LENGTH + 1); // Send DMX data
  while (DmxOut.busy()) {
    // Wait for output to finish
  }
  delay(1);
}