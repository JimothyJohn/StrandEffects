/*
  LEDeffects.cpp - Class wrappers for FastLED library
  that allow for easily configureable asynchronous effects
  Created by Nick Armenta, April 26, 2020.
  Released into the public domain.
*/

/*
Nick's to-do list:
Primary:
- Sync to bpm
Secondary:
- Make shift bi-directional
*/

#include "Arduino.h"
#include "LEDeffects.h" 

// Segment is a band of LED's of length w that begin at st and end at en and updates every stp milliseconds
Segment::Segment(uint16_t st, uint16_t w) {
	width = w;
	// Output for user
	cur_red, tgt_red = 0; // Red values
	cur_green, tgt_green = 0; // Green values
	cur_blue, tgt_blue = 0; // Blue values
	cur_hue, tgt_hue = 0; // Hue values
	cur_sat, tgt_sat = 0; // Sat values
	cur_val, tgt_val = 0; // Val values
	loc0 = st; // Initial segment start
	loc1 = st+w; // Initial segment end
	// Private for internal
	_stpTime = 10; // Time between status updates
	_mClock = millis(); // Start motion clock
	_mstpTime = _stpTime;  // Reset motion step
	_cClock = millis(); // Start color clock
	_cstpTime = _stpTime;  // Reset motion step
	_idx = 0; // Motion index
	_idxdir = true;  // Motion index direction
	_its = 0; // Color index
	_itsdir = true;  // Color index direction
	_counter = 0; // Aux counter
}

// Set stripe to specific r, g, b cur_value
void Segment::SetRGB(uint8_t r, uint8_t g, uint8_t b) {
	cur_red = r;
	cur_green = g;
	cur_blue = b;
}

// Extract current target RGB values
void Segment::GetRGB() {
	tgt_red = cur_red;
	tgt_green = cur_green;
	tgt_blue = cur_blue;
}

// Set stripe to specific h, s, v cur_value
void Segment::SetHSV(uint8_t h, uint8_t s, uint8_t v) {
	cur_hue = h;
	cur_sat = s;
	cur_val = v;
}

// Extract current target HSV values
void Segment::GetHSV() {
	tgt_hue = cur_hue;
	tgt_sat = cur_sat;
	tgt_val = cur_val;
}

// Shift in a direction by increments of dir per cycle
void Segment::Shift(float sTime, uint16_t distance) {
	int dir = Segment::StepCalc(sTime, distance, &_mstpTime);
	// If enough time has elapsed 
	if(ClockCheck(&_mClock, _mstpTime)) {
		// Shift in given direction		
		if(abs(_idx)<distance) {
			_idx += dir;
			loc0 = limitcap(dir, loc0, 2, (61-width));
			loc1 = loc0 + width;
		}
	}
}

// Move dir distance per cycle forever 
void Segment::Scan(float sTime, int dir) {
	// If enough time has elapsed
	if(ClockCheck(&_mClock, sTime*1000)) {
		loc0 = limitreset(dir, loc0, 2, (61-width));
		loc1 = loc0+width;
	}
}

// Move continuosly back and forth over a distance in dir increments
void Segment::Bounce(float sTime, uint16_t distance) {
	int dir = Segment::StepCalc(sTime, distance, &_mstpTime);
	// If enough time has elapsed 
	if(ClockCheck(&_mClock, _mstpTime)) {
		// Shift in given direction
		if(_idxdir) {
			_idx += dir;
			loc0 += dir;
		} else {
			_idx -= dir;
			loc0 -= dir;
		}
		loc1 = loc0 + width;
		// Switch direction if at end of travel
		if(abs(_idx)>(distance-abs(dir)-width) || abs(_idx)<abs(dir)) {
			_idxdir = !_idxdir;
		}
	}
}

// Jump to new location every sTime seconds
void Segment::Jump(float sTime) {
	// If enough time has elapsed
	if(ClockCheck(&_mClock, sTime*1000)) {
		loc0 = random(2, 61-width);
		loc1 = loc0+width;
	}
}

// Adjust RGB cur_values by offset per cycle
void Segment::Phase(float sTime, int r_offset, int g_offset, int b_offset) {
	// If enough time has elapsed 
	if(ClockCheck(&_cClock, sTime*1000)) {
		// Phase in given direction
		cur_red = limitcap(r_offset, cur_red);
		cur_green = limitcap(g_offset, cur_green);
		cur_blue = limitcap(b_offset, cur_blue);
	}
}

// Alternate between threshold r, g, b cur_values and 0 every sTime seconds
void Segment::Flash(float sTime, uint8_t r_thresh, uint8_t g_thresh, uint8_t b_thresh) {	
	// If enough time has elapsed 
	if(ClockCheck(&_cClock, sTime*1000)) {
		// Switch light polarity
		if(cur_red==0 && cur_green==0 && cur_blue==0) {
			// On
			cur_red = r_thresh;
			cur_green = g_thresh;
			cur_blue = b_thresh;
		} else {
			cur_red = cur_green = cur_blue = 0;	// Off	
		}
	}
}

// Increase by itsdir per cycle until intensity is reached then reverse direction
void Segment::Pulse(float sTime, uint8_t intensity) {
	int dir = Segment::StepCalc(sTime, intensity, &_cstpTime);
	// If enough time has elapsed 
	if(ClockCheck(&_cClock, _cstpTime)) {
		// If over threshold switch direction
		if(cur_val>(tgt_val+intensity-dir) || cur_val<=tgt_val) {
			_itsdir = !_itsdir;
		}
		// Set internal indexer to set value
		if(_itsdir) {
			cur_val = limitcap(dir, cur_val, tgt_val, (tgt_val+intensity));
		} else {
			cur_val = limitcap(-dir, cur_val, tgt_val, (tgt_val+intensity));
		}
	}
}

// Brighten segment by intensity level
void Segment::Brighten(float sTime, uint8_t intensity, bool repeat=false) {
	int dir = Segment::StepCalc(sTime, intensity, &_cstpTime);
	// If enough time has elapsed
	if(ClockCheck(&_cClock, _cstpTime)) {
		// If not at required intensity, increase
		if(cur_val<(tgt_val+intensity-dir)) {
			cur_val = limitcap(dir, cur_val);
		} else if(repeat) {
			// Reset value if repeating
			cur_val = tgt_val;
		}
	}
}

// Brighten segment to target intensity
void Segment::BrightenTo(float sTime, uint8_t intensity) {
	int dir = Segment::StepCalc(sTime, intensity, &_cstpTime);
	// If enough time has elapsed 
	if(cur_val<intensity) {
		if(ClockCheck(&_cClock, _stpTime)) {
			// If not at required intensity, increase
			cur_val = limitcap(dir, cur_val);
		}
	}
}

// Dim segment by intensity level
void Segment::Dim(float sTime, uint8_t intensity, bool repeat=false) {
	int dir = Segment::StepCalc(sTime, intensity, &_cstpTime);
	// If enough time has elapsed
	if(ClockCheck(&_cClock, _stpTime)) {
		// If not at required intensity, increase
		if(cur_val>(tgt_val-intensity+dir)) {
			cur_val = limitcap(-dir, cur_val);
		} else if(repeat) {
			// Reset value if repeating
			cur_val = tgt_val;
		}
	}
}

// Dim segment to target intensity
void Segment::DimTo(float sTime, uint8_t intensity) {
	int dir = Segment::StepCalc(sTime, intensity, &_cstpTime);
	// If enough time has elapsed 
	if(cur_val>intensity) {
		if(ClockCheck(&_cClock, _stpTime)) {
			// If not at required intensity, increase
			cur_val = limitcap(-dir, cur_val);
		}
	}
}

// Move continuosly back and forth over a distance in dir increments
void Segment::Saturate(float sTime, uint8_t intensity) {
	int dir = Segment::StepCalc(sTime, intensity, &_cstpTime);
	// If enough time has elapsed 
	if(ClockCheck(&_cClock, _stpTime)) {
		// If over threshold cap cur_values
		if(abs(_its+dir)>intensity) {
			cur_sat = intensity; // decrement
		} else {
			_its += dir; // increment _its
		}
		//If maxed then clip
		cur_sat = limitcap(dir, cur_sat);
	}
}

// Move continuosly back and forth over a distance in dir increments
void Segment::RainbowCycle(float sTime, uint8_t hue_start=0, uint8_t hue_end=255) {
	int dir = Segment::StepCalc(sTime, (hue_end-hue_start), &_cstpTime);
	// If enough time has elapsed 
	if(ClockCheck(&_cClock, _stpTime)) {
		// If over threshold cap cur_values
		cur_hue = limitreset(dir, cur_hue, hue_start, hue_end);
	}
}



// Brighten segment by intensity level
int Segment::StepCalc(float sTime, int incs, uint16_t *stp) {
	// stpTime is ms per increment
	*stp = round(sTime*1000/abs(incs));
	int dir = 1;
	// if stpTime is less than update time
	if(*stp<_stpTime) {
		dir = round(_stpTime / *stp);
		*stp = 10;
	}
	return dir;
}

// Checks the current clock to see if it's greater than stp and if so resets it
bool ClockCheck(long *cur_clock, uint16_t stp) {
	// Correct if not in milliseconds
	if(stp<10) {
		stp *= 1000;
	}
	if((millis()-*cur_clock)>stp) {
		*cur_clock = millis();
		return true;
	} else {
		return false;
	}
}

// Resets channel if at limit, defaults to 8-bit
uint16_t limitreset(int dir, uint16_t value, int low=0, int high=255) {
	if((value+dir)>high) {
		return low; // reset
	} else if((value+dir)<low) {
		return high; // reset
	} else {
		return (value+dir); // increment cur_hue
	}
}

// Clips channel if at limit, defaults to 8-bit
uint16_t limitcap(int dir, uint16_t value, int low=0, int high=255) {
	if((value+dir)>high) {
		return high; // reset
	} else if((value+dir)<low) {
		return low; // reset
	} else {
		return (value+dir); // increment channel
	}
}
