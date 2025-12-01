/* 
# OVERVIEW
DMX transmitter using Raspberry Pi Pico.
A work light switch to allow cheap LED PARs to be used as either work lights (white, full brightness) or run lights (blueish purple, dimmed). They can also be turned off through DMX using this project.
The idea is to put this circuit in an enclosure with labeled buttons and mount it to the wall near the lights being controlled, and use it like a fancy light switch.

# TECHNICAL DETAILS
DMX is transmitted using TTL on GPIO 0. You'd then use a MAX485 TTL to RS232 converter chip in transmit mode to turn GPIO 0's signal to a differential signal suitible for reliable, long DMX runs.
Uses Pico-DMX library: https://github.com/jostlowe/Pico-DMX
Created by Eric Smith with help from GitHub Copilot (GPT-4.1)
Original codebase: 11-30-2025
Latest version: 11-30-2025

See repo's README for more information.
*/

#include <Arduino.h>
#include <DmxOutput.h>

#define DMX_PIN 0
#define UNIVERSE_LENGTH 6
#define BUTTON_WORK_PIN 16
#define BUTTON_RUN_PIN 17
#define BUTTON_OFF_PIN 18

DmxOutput DmxOut; // Instantiate DmxOutput as DmxOut

uint8_t universe[UNIVERSE_LENGTH + 1];  // Make array size fixed

enum LightMode {
  MODE_OFF,
  MODE_WORK,
  MODE_RUN
};

LightMode currentMode = MODE_OFF; // turn lights off when Pico is powered on

// Create a function to set lights in one of the 3 modes (work, run, off)
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
    case MODE_RUN:
      // Run lights (blue only as pseudo-blacklight)
      universe[1] = 175; // DIMMER
      universe[2] = 69;   // RED
      universe[3] = 0;   // GREEN
      universe[4] = 230; // BLUE
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
  pinMode(BUTTON_RUN_PIN, INPUT_PULLUP);  // Run Lights Button
  pinMode(BUTTON_OFF_PIN, INPUT_PULLUP);    // Off Button
  setLights(currentMode); // Set initial light state
}

void loop() {
  // Buttons are active LOW
  static LightMode lastMode = MODE_OFF;

  if (digitalRead(BUTTON_WORK_PIN) == LOW) {
    currentMode = MODE_WORK;
  }
  else if (digitalRead(BUTTON_RUN_PIN) == LOW) {
    currentMode = MODE_RUN;
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