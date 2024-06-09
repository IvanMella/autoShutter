# autoShutter
### Automatic control for my dormroom shutter
Controls my bedroom shutter by a servomotor using an ESP32 microcontroller, wifi
controller and light sensing. It also control an optional 240ac fan via a SSR

developed in C++, html, javascript with atom.

# Servomotor data

  - MG229 servo 0-180 degrees hi torque 5V supply
  - Sw ESP32Servo.h by kevin Harrington
  - 0 degree shutter closed, 90 degree open, 180 flip over

# Revisions:

        - 26.05.2024  V0.1 First commit, only wifi command. servo need to be turned off. slow 50ms timeout
*2024 Ivan Mella, ivan.m48@gmail.com*
