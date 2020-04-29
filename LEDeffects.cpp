/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "LEDeffects.h"

// Segment is a band of LED's of size w that begin at st and end at en
// Segment is initialized to r, g, b values and update every stp milliseconds
Segment::Segment(uint16_t st, uint16_t en, uint16_t w, uint16_t stp, uint8_t r, uint8_t g, uint8_t b) {
	START = st;
	END = en;	
	if(w==0) {
		width = en-st;
	} else {
		width = w;
	}
	stpTime = stp;
	red = r;
	green = g;
	blue = b;
	// Output for user
	loc0 = START;
	loc1 = START+width;
	_clock = millis(); // Start clock
	_idx = 0;
	_dir = true;
}

// Shift in a direction by increments of dir per cycle
void Segment::Shift(int dir, uint16_t distance) {
	// If enough time has elapsed 
	if((millis()-_clock)>stpTime) {
		_clock = millis(); // Reset clock		
		// Shift in given direction		
		if(abs(_idx)<distance) {
			_idx += dir;
			loc0 += dir;
			loc1 = loc0 + width;
		}
	}
}

// Move dir distance per cycle forever 
void Segment::Scan(int dir) {
	// If enough time has elapsed
	if((millis()-_clock)>stpTime) {
		_clock = millis(); // Reset clock		
		// Shift in given direction		
		loc0+=dir;
		if(loc0>END-width) {
			loc0 = START;
		}
		loc1 = loc0+width;
	}
}

// Move continuosly back and forth over a distance in dir increments
void Segment::Bounce(int dir, uint16_t distance) {
	// If enough time has elapsed 
	if((millis()-_clock)>stpTime) {
		_clock = millis(); // Reset clock		
		// Shift in given direction
		if(_dir) {
			_idx += dir;
			loc0 += dir;
		} else {
			_idx -= dir;
			loc0 -= dir;
		}
		loc1 = loc0 + width;
		// Switch direction if at end of travel
		if(abs(_idx)>(distance-abs(dir)-width) || abs(_idx)<abs(dir)) {
			_dir = !_dir;
		}
	}
}

// Adjust RGB values
void Segment::Phase(int r_offset, int g_offset, int b_offset) {
	if((millis()-_clock)>stpTime) {
		_clock = millis(); // Reset clock
		// Shift in given direction
		if(red+r_offset<0) {
			red = 0;
		} else if (red+r_offset>255) {
			red = 255;
		} else {
			red += r_offset;
		}
		
		if(green+g_offset<0) {
			green = 0;
		} else if (green+g_offset>255) {
			green = 255;
		} else {
			green += g_offset;
		}
		
		if(blue+b_offset<0) {
			blue = 0;
		} else if (blue+b_offset>255) {
			blue = 255;
		} else {
			blue += b_offset;
		}
	}
}

// Set stripe to specific r, g, b value
void Segment::SetRGB(uint8_t r, uint8_t g, uint8_t b) {
	red = r;
	green = g;
	blue = b;
}

// Flash alternates between maximum r, g, b values and 0
void Segment::Flash(uint8_t r_thresh, uint8_t g_thresh, uint8_t b_thresh) {	
	if((millis()-_clock)>stpTime) {
		_clock = millis(); // Reset clock
		// Shift in given direction
		if(red==0 && green==0 && blue==0) {
			red = r_thresh;
			green = g_thresh;
			blue = b_thresh;
		} else {
			red = green = blue = 0;			
		}
	}
}

// Flash alternates between maximum r, g, b values and 0
void Segment::Rainbow(uint8_t thresh, uint8_t inc) {	
	bool r, g, b;
	if(red>0 && green>0 && blue>0) {
		red = thresh;
	}
	
	if((millis()-_clock)>stpTime) {
		_clock = millis(); // Reset clock
		// Shift in given direction
		if(r==True) {
			red-=inc;
			green+=inc;
			if(green>round(thresh/2)) {
				r=False;
				g=True
			}
		}
		if(g==True) {
			green-=inc;
			blue+=inc;
			if(blue>round(thresh/2)) {
				g=False;
				b=True;
			}
		}
		if(b==True) {
			blue-=inc;
			blue+=inc;
			if(blue>round(thresh/2)) {
				g=False;
				b=True;
			}
		}
	}
}