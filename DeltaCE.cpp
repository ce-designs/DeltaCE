/* 
* DeltaCE.cpp
*
* Created: 6-7-2014 11:34:34
* Author: CE-Designs
*/

#include "DeltaCE.h"
#include <Wire.h>

#ifdef __AVR__
#include <avr/pgmspace.h>
#define WIRE Wire
#else
#define PROGMEM
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define WIRE Wire1
#endif

// constructor: user configurable maximal attenuation, minimal step size and the addresses of pcf8574 devices
DeltaCE::DeltaCE(float maxAttnu, float minStepSize, uint8_t addr1, uint8_t addr2)
{
	setDevider(minStepSize);
	this->maximalAttuenation = (uint8_t)maxAttnu*this->devider;
	this->minimumStepSize = (uint8_t)minStepSize*this->devider;
	this->stepSize = uint8_t(minStepSize*this->devider);
	setPcfAdresses(addr1, addr2);
} //DeltaCE

// constructor: user configurable maximal attenuation, minimal step size, desired step size and the addresses of pcf8574 devices
DeltaCE::DeltaCE(float maxAttnu, float minStepSize, float stepSize, uint8_t addr1, uint8_t addr2)
{
	setDevider(minStepSize);
	this->maximalAttuenation = (uint8_t)maxAttnu*this->devider;
	this->minimumStepSize = (uint8_t)minStepSize*this->devider;
	this->stepSize = uint8_t(stepSize*this->devider);
	setPcfAdresses(addr1, addr2);	
} //DeltaCE


void DeltaCE::setDevider(float value)
{	
	if (value == 0.1)
	{
		this->devider = DEVIDER_VALUE_1;
	}
	else if (value == 0.5)
	{
		this->devider = DEVIDER_VALUE_2;
	}
	else
	{
		this->devider = DEVIDER_VALUE_3;
	}	
}
// begins the relay attenuator pulling all pins of both PCF8574 device low
void DeltaCE::begin()
{
	setAllPcfpinsLOW();
}

// sets the attenuation value and applies the volume settings 
void DeltaCE::setAttenuation(float value)
{
	this->attnu = (uint8_t)value*this->devider; 
	applySettings();
}

// sets the addresses of both PCF8574 devices
void DeltaCE::setPcfAdresses(uint8_t addr1, uint8_t addr2)
{
	this->pcf8574Addr1 = addr1;
	this->pcf8574Addr2 = addr2;
}

// increases the volume by the chosen step size
void DeltaCE::increaseVolume()
{
	// check if the the signal is unattenuated 
	if (this->attnu == MIN_ATTENUATION)
	{
		return; // already unattenuated, so do nothing
	}
	// check if the attuenation level does become lower the max when proceeding
	if ((int)(this->attnu - this->stepSize) <= MIN_ATTENUATION)
	{
		this->attnu = MIN_ATTENUATION;	// set to the min, otherwise it would become lower than minimal	
	}
	else
	{
		this->attnu -= this->stepSize;
	}		
	applySettings();
}

// decreases the volume by the chosen step size
void DeltaCE::decreaseVolume()
{
	// check if the the signal is maximal attenuated 
	if (this->attnu == maximalAttuenation)
	{
		return; // no further attuenation possible, so do nothing
	}
	// check if the attuenation level would exceed the max when proceeding 
	if ((int)(this->attnu + this->stepSize) >= maximalAttuenation)
	{
		this->attnu = maximalAttuenation; // set to the max, otherwise it would exceed the max
	}
	else
	{
		this->attnu += this->stepSize;
	}
	applySettings();
}

/* 
*	applies the volume settings to set the correct level of attenuation
*
*	this function applies some logic to get
*	the correct byte that is used to set the the pins of
*	both the PCF8574 devices
*/
void DeltaCE::applySettings()
{
	byte pcfVal = this->attnu;
	
	// divide by two because only 127.5 dB attenuation is possible
	if (this->devider != DEVIDER_VALUE_1)
	{
		pcfVal = this->attnu / this->devider;
	}	
	
	// shift all bits and discard the last bit
	if (this->devider == DEVIDER_VALUE_2)
	{
		pcfVal = pcfVal <<= 1;
	}	
	else if	(this->devider == DEVIDER_VALUE_1)
	{
		pcfVal = pcfVal >>= 1;
	}	
	
	// if the there is a remainder than the first bit needs to be
	// flipped in order to actuate the first decibel relay
	if (this->attnu % 2)
	{
		pcfVal ^= bit(0);
	}
	
	// write value to PCF8574 no.1
	setPcfPins(this->pcf8574Addr1, pcfVal);
	
	delay(PULSETIME);		// wait for the relays to attenuate
	
	// write the value with all bits flipped to PCF8574 no.2
	setPcfPins(this->pcf8574Addr2, ~pcfVal);
	
	delay(PULSETIME);		// wait for the relays to attenuate
	
	setAllPcfpinsLOW();		// set all pins of both PCF8574 no.1 and no.2 low	
}

// sets all pins of both PCF8574 no.1 and no.2 low
void DeltaCE::setAllPcfpinsLOW()
{
	setPcfPins(this->pcf8574Addr1, B00000000);	// set all PCF8574 no.1 pins low
	setPcfPins(this->pcf8574Addr2, B00000000);	// set all PCF8574 no.2 pins low
}

// sets the state of all pins of the PCF8574 with the provided address
void DeltaCE::setPcfPins(uint8_t address, uint8_t value)
{
	WIRE.beginTransmission(address);	// begin the transmission to PCF8574
	WIRE.write(value);					// set the pins
	WIRE.endTransmission();				// end the transmission
}