# Thomas Daft Punk Helmet Display
Authors: Ryan and Chelsea Pawling

This software for Arduino nano powers a 6x32 LED matrix display for Thomas' Daft Punk helmet.
Various display programs can be triggered using bluetooth connection to an android phone.


## Operation
<insert operation instructions here>

## Diagrams
<insert diagrams here>

## Software
### LED Matrix Studio
Used to generate animations for the LED matrix display.

link: http://maximumoctopus.com/electronics/builder.htm

### Arduino bluetooth controller App
Used to control the LED matrix display from an android phone.

link: https://play.google.com/store/apps/details?id=com.satech.arduinocontroller


## Parts

### Arduino Nano
reference: https://store.arduino.cc/usa/arduino-nano

### HC-5 Bluetooth Module
HC-05 can connect to Arduino to send serial data from a bluetooth connection.
The serial data can be a string to scroll across the display, or a command to animate certain program on the display.

Example connection diagram:

![alt text](https://i.stack.imgur.com/H7LNg.jpg "HC-05 connection to Arduino")

### Max 7219 LED Drivers (8x8)
Four Max 7219s are used to drive the matrix.

datasheet: https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf

### Anker power bank
The high-capacity of the Anker power bank is great for this use-case.  A battery pack will quickly drain.
The power bank plugs directly into the regulated usb input of the Arduino.
