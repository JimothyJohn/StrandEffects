/*
  LEDeffects_h.h - Library for single-strand LED effects
  Created by Nick Armenta, April 26, 2020.
  Released into the public domain.
*/

#ifndef LEDeffects_h
#define LEDeffects_h

#include "Arduino.h"
   
class Segment {
	public:
		Segment(uint16_t st, uint16_t en, uint16_t w, uint16_t stp, uint8_t r, uint8_t g, uint8_t b);
		void Shift(int dir, uint16_t distance);
		void Scan(int dir);
		void Phase(int r_offset, int g_offset, int b_offset);
		void Bounce(int dir, uint16_t distance);
		void SetRGB(uint8_t r, uint8_t g, uint8_t b);
		void Flash(uint8_t r_thresh, uint8_t g_thresh, uint8_t b_thresh);
		uint16_t START;
		uint16_t END;	
		uint16_t width;
		uint16_t stpTime;
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		uint16_t loc0;
		uint16_t loc1;
	
	private:
		uint16_t _clock;
		int _idx;
		bool _dir;
};

#endif