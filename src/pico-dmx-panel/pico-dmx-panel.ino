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

bool lightOn = false;         // Latch - true = ON, false = OFF
bool lastButtonState = HIGH;  // Last button reading (using INPUT_PULLUP, so HIGH = not pressed)

void setup() {
  DmxOut.begin(DMX_PIN);      // Bind DMX output to pin 0
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Configure button pin with internal pull-up
}

void loop() {
  // Read button (active LOW: pressed = LOW)
  bool buttonState = digitalRead(BUTTON_PIN);

  // Detect button press (transition: HIGH → LOW)
  if (buttonState == LOW && lastButtonState == HIGH) {
    lightOn = !lightOn; // toggle light state
    delay(200);         // debounce: simple, optional (fine-tune as needed)
  }

  // Set DMX universe based on latch state
  if (lightOn) {
    universe[1] = 255; // DIMMER - ON
    universe[2] = 255; // RED - ON
    universe[3] = 255; // GREEN - ON
    universe[4] = 255; // BLUE - ON
    universe[5] = 0;   // STROBE - OFF
    universe[6] = 0;   // CONTROL - OFF/unknown
  } else {
    universe[1] = 0; // DIMMER - OFF
    universe[2] = 0; // RED - OFF
    universe[3] = 0; // GREEN - OFF
    universe[4] = 0; // BLUE - OFF
    universe[5] = 0; // STROBE - OFF
    universe[6] = 0; // CONTROL - OFF/unknown
  }

  DmxOut.write(universe, UNIVERSE_LENGTH + 1); // Send DMX data
  while (DmxOut.busy()) {
    // Wait for output to finish
  }

  lastButtonState = buttonState; // Store for edge detection
  delay(1);
}