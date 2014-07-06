#include <Wire.h>
#include "DeltaCE.h"

// DeltaCE object instantiation

#define MAX_ATTENUATION 127.5	// max 127.5dB attuenation (other possible values are 255dB and 25.5dB)
#define	MIN_STEP_SIZE 0.5		// the lowest possible step size for attuenation of the audio signal
#define STEP_SIZE 0.5			// the desired step size (must be the same or a multiple of the minimum step size!)
#define PCF8574_ADDR_1 0x20		// address of PCF8574 device no.1
#define PCF8574_ADDR_2 0x21		// address of PCF8574 device no.2

#define DEF_ATTENUATION 50		// the default attenuation at start-up

//DeltaCE delta(MAX_ATTENUATION, MIN_STEP_SIZE, PCF8574_ADDR_1, PCF8574_ADDR_2); // uses minimal step size as step size

DeltaCE delta(MAX_ATTENUATION, MIN_STEP_SIZE, STEP_SIZE, PCF8574_ADDR_1, PCF8574_ADDR_2);

// initialization
void setup()
{
	Wire.begin();	// join i2c bus
	
	delta.begin();	// must be set as early as possible, but after Wire.begin()
	
	delta.setAttenuation(DEF_ATTENUATION);	// set the attuenation level (must be lower than the maximal attuenation!)
}

// main loop
void loop()
{
	// raise the volume by 10 steps
	for (byte x = 0; x < 10; x++)
	{
		delta.increaseVolume();
	}
	
	// lower the volume by 10 steps
	for (byte x = 0; x < 10; x++)
	{
		delta.decreaseVolume();
	}

}

