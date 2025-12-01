# Overview
DMX transmitter using Raspberry Pi Pico.
A work light switch to allow cheap LED PARs to be used as either work lights (white, full brightness) or run lights (blueish purple, dimmed). They can also be turned off through DMX using this project.
The idea is to put this circuit in an enclosure with labeled buttons and mount it to the wall near the lights being controlled, and use it like a fancy light switch.

# Technical Details
DMX is transmitted using TTL on GPIO 0. You'd then use a MAX485 TTL to RS232 converter chip in transmit mode to turn GPIO 0's signal to a differential signal suitible for reliable, long DMX runs.

# Credits
Uses [Pico-DMX library](https://github.com/jostlowe/Pico-DMX) by GitHub user [jostlowe](https://github.com/jostlowe).\
Created by Eric Smith with help from GitHub Copilot (GPT-4.1)