Student: Pozdnikin Mykhailo
Subject: CSDT

Project name: Ship for carpfishing
Programming language: C, C++

Additional tools/utils: Make, CMake

Control modes: Manual, Wi-fi, BT

Actions: move forward, move backward, move leftward, move rightward, turn led on and off

HW-interface: SPI

Data driven format: xml

In order to run the sketch (transiever\transiever.ino and receiver\receiver.ino), you need next hardware:
Arduino nano - 2 pcs
NRF24L01 - 2 pcs
Thumb joystick - 1 pcs
led - 5 pcs or motors - 2 pcs
NPN transistor (for ex tip41c) - 4 pcs
PNP transistor (for ex tip42c) - 4 pcs
10 kOhm resistor - 3 pcs (only if you use leds instead of motors)

Also, you have to install Arduino IDE to compile and program your Arduino.
Then, open library manager (Ctrl+Shift+I) and install RF24 library.
Choose COM-port, that Arduino is connected to (Tools-Port).
After that, you can upload the sketch onto your board (click on "Upload" button).
