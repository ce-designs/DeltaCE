DeltaCE
=======

A Arduino library for controlling the DeltaCE relay attenuator

=======

This library supports the following versions of the DeltaCE relay attenuator:
- DeltaCE 255
  - Maximal 255dB attenuation
  - Step size of 1dB
- DeltaCE 127.5
  - Maximal 127.5dB attenuation
  - Step size of 0.5dB
- DeltaCE 25.5
  - Maximal 25.5dB attenuation
  - Step size of 0.1dB

=======
User configurable settings:
- Maximum attuenation (25.55, 127.5 or 255dB)
- Minimal step size (depending on the maximum attenuation)
- Desired step size (must the the same or a multiple of the minimal step size!)
- I2C address of PCF8541 device no.1
- I2C address of PCF8541 device no.2

Functions for controlling the relay attenuator:
- Increase volume (increases the volume with the provided desired set size value)
- Decrease volume (decreases the volume with the provided desired set size value)
- Set attuenation (set's the attuenation to the provided value)

=======
The DeltaCE relay attenuator is basicly a clone of the The d1 Relay-based R-2R Stereo Attenuator from AMB Laboratories. All credits for the design go to AMB Laboratories!

http://www.amb.org/audio/delta1/

This DeltaCE library is completely developed by CE-Designs and is tested with de DeltaCE relay attenuator, but not with the original d1 Relay-based R-2R Stereo Attenuator. In theory the library should work fine with it, since the DeltaCE is almost an exact clone.

Feel free to use, copy or modify the code.

Don't forget to read the MIT-licence from the licence file.
