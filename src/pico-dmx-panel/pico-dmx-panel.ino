/* 
DMX transmitter using Raspberry Pi Pico

DMX is transmitted over 1 signal wire (and ground) in the Pico setup.
You'd need to use a 3-pin fixture and determine if you could short DATA- and GND.
Currently, this program will transmit 1 universe of 3 channels, and set Channel 1 to 174 when the button is pressed.


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
Modified for latching mode & 3-button control (Work Lights, Blacklight, Off)
11-30-2025
*/

#include <Arduino.h>
#include <DmxOutput.h>

#define DMX_PIN 0
#define UNIVERSE_LENGTH 6
#define BUTTON_WORK_PIN 16
#define BUTTON_BLACK_PIN 17
#define BUTTON_OFF_PIN 18

DmxOutput DmxOut; // Instantiate DmxOutput

uint8_t universe[UNIVERSE_LENGTH + 1];  // Make array size fixed

enum LightMode {
  MODE_OFF,
  MODE_WORK,
  MODE_BLACKLIGHT
};

LightMode currentMode = MODE_OFF;

void setLights(LightMode mode) {
  switch (mode) {
    case MODE_WORK:
      // Work Lights (full white)
      universe[1] = 255; // DIMMER
      universe[2] = 255; // RED
      universe[3] = 255; // GREEN
      universe[4] = 255; // BLUE
      universe[5] = 0;   // STROBE
      universe[6] = 0;   // CONTROL
      break;
    case MODE_BLACKLIGHT:
      // Blacklight (blue only as pseudo-blacklight)
      universe[1] = 255; // DIMMER
      universe[2] = 0;   // RED
      universe[3] = 0;   // GREEN
      universe[4] = 255; // BLUE (Max BLUE)
      universe[5] = 0;   // STROBE
      universe[6] = 0;   // CONTROL
      break;
    case MODE_OFF:
    default:
      // All Off
      universe[1] = 0;   // DIMMER
      universe[2] = 0;   // RED
      universe[3] = 0;   // GREEN
      universe[4] = 0;   // BLUE
      universe[5] = 0;   // STROBE
      universe[6] = 0;   // CONTROL
      break;
  }
}

void setup() {
  DmxOut.begin(DMX_PIN);      // Bind DMX output to pin 0
  pinMode(BUTTON_WORK_PIN, INPUT_PULLUP);   // Work Lights Button
  pinMode(BUTTON_BLACK_PIN, INPUT_PULLUP);  // Blacklight Button
  pinMode(BUTTON_OFF_PIN, INPUT_PULLUP);    // Off Button
  setLights(currentMode); // Set initial light state
}

void loop() {
  // Buttons are active LOW
  static LightMode lastMode = MODE_OFF;

  if (digitalRead(BUTTON_WORK_PIN) == LOW) {
    currentMode = MODE_WORK;
  }
  else if (digitalRead(BUTTON_BLACK_PIN) == LOW) {
    currentMode = MODE_BLACKLIGHT;
  }
  else if (digitalRead(BUTTON_OFF_PIN) == LOW) {
    currentMode = MODE_OFF;
  }

  // Only update DMX if mode changed
  if (currentMode != lastMode) {
    setLights(currentMode);
    lastMode = currentMode;
  }

  DmxOut.write(universe, UNIVERSE_LENGTH + 1); // Send DMX data
  while (DmxOut.busy()) {
    // Wait for output to finish
  }
  delay(10);
}