# autoShutter
### Automatic control for my dormroom shutter
Controls a shutter opening by a servomotor using an ESP32 microcontroller, wifi
and a photoresistor. An optional control for a 240v plug is made to turn on and
off a fan.

developed in C++, html, javascript with atom.

# Servomotor data

  - MG229 servo 0-180 degrees hi torque 5V supply
  - Sw ESP32Servo.h by kevin Harrington
  - 0 degree shutter closed, 90 degree open, 180 flip over

# Revisions:

        - 26.05.2024  V0.1 First commit, only wifi command. servo need to be turned off. slow 50ms timeout
*2024 Ivan Mella, ivan.m48@gmail.com*
