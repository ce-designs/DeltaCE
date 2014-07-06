/* 
* DeltaCE.h
*
* Created:	6-7-2014 11:34:38
* Author:	CE-Designs
* Purpose:	
*/


#ifndef __DELTACE_H__
#define __DELTACE_H__

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif


#define MIN_ATTENUATION 0		// the lower possible value for attenuation (0dB Attenuation == no attuenation)	
#define PULSETIME 20			// the time in milliseconds for how long the pulses are applied to the relays

#define DEFAULT_ATTENUATION 100 // default volume at power up (100 == -50dB)
#define DEFAULT_STEPSIZE 1		// this is actually double the value StepSize (1 == 0.5dB)

#define DEVIDER_VALUE_1 20		// used for doing calculations when the maximum attuenation is 25.5dB with a minimal step size of 0.1dB
#define DEVIDER_VALUE_2	2		// used for doing calculations when the maximum attuenation is 127.5dB with a minimal step size of 0.5dB
#define DEVIDER_VALUE_3 4		// used for doing calculations when the maximum attuenation is 255dB with a minimal step size of 1dB

class DeltaCE
{
//variables
public:

protected:	

private:	
	
	// this is the minimum step size that the relay attenuator is capable of (depending on the used resistors values)
	// For more info see http://www.amb.org/audio/delta1/ (look for resistor values)
	uint8_t minimumStepSize;
	
	// this is the maximal possible attuenation (depending on the used resistors values)
	// this value is actual double the value of the maximal attenuation
	int maximalAttuenation;		

	uint8_t pcf8574Addr1;	// for holding the address of PCF8574 device no.1
	uint8_t pcf8574Addr2;	// for holding the address of PCF8574 device no.2
		
	int attnu;				// holds the doubled attenuation value in -dB
	uint8_t stepSize;		// the value for the step size for raising or lowering volume (must be the same or a multiple of the minimumStepSize!)
	
	uint8_t devider;		// used for calculating the correct values

//functions
public:
	// user configurable maximal attenuation, minimal step size and the addresses of pcf8574 devices
	DeltaCE(float maxAttnu, float minStepSize, uint8_t addr1, uint8_t addr2);

	// user configurable maximal attenuation, minimal step size, desired step size and the addresses of pcf8574 devices 
	DeltaCE(float maxAttnu, float minStepSize, float stepSize, uint8_t addr1, uint8_t addr2);
	
	void begin();			
	
	void increaseVolume();	// decreases the volume by the chosen step size 
	void decreaseVolume();	// increases the volume by the chosen step size
	
	float getAttenuation() { return (float)attnu / this->devider; }	// return the current attenuation value in dB as a floating point number
	void setAttenuation(float value);	
		
protected:

private:
	void setPcfAdresses(uint8_t addr1, uint8_t addr2);
	void setDevider(float value);
	void applySettings();
	void setAllPcfpinsLOW();
	void setPcfPins(uint8_t address, uint8_t value);

}; //DeltaCE

#endif //__DELTACE_H__
