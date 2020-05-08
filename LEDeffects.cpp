/*
  LEDeffects.cpp - Class wrappers for FastLED library
  that allow for easily configureable asynchronous effects
  Created by Nick Armenta, April 26, 2020.
  Released into the public domain.
*/

#ifndef LEDeffects_h
#define LEDeffects_h

#include "Arduino.h"

class Segment {
	public:
		Segment(uint16_t st, uint16_t w);
		void Shift(float sTime, uint16_t distance);
		void Scan(float sTime, int dir);
		void Phase(float sTime, int r_offset, int g_offset, int b_offset);
		void Bounce(float sTime, uint16_t distance);
		void Jump(float sTime);
		void SetRGB(uint8_t r, uint8_t g, uint8_t b);
		void GetRGB();
		void Flash(float sTime, uint8_t r_thresh, uint8_t g_thresh, uint8_t b_thresh);
		void SetHSV(uint8_t h, uint8_t s, uint8_t v);
		void GetHSV();
		void Pulse(float sTime, uint8_t intensity);
		void Saturate(float sTime, uint8_t intensity);
		void Brighten(float sTime, uint8_t intensity, bool repeat=false);
		void BrightenTo(float sTime, uint8_t intensity);
		void Dim(float sTime, uint8_t intensity, bool repeat=false);
		void DimTo(float sTime,  uint8_t intensity);
		void RainbowCycle(float sTime, uint8_t hue_start=0, uint8_t hue_end=255);
		int StepCalc(float sTime, int incs, uint16_t *stp);
		uint16_t width;
		uint8_t cur_red;
		uint8_t cur_green;
		uint8_t cur_blue;
		uint8_t tgt_red;
		uint8_t tgt_green;
		uint8_t tgt_blue;
		uint8_t cur_hue;
		uint8_t cur_sat;
		uint8_t cur_val;
		uint8_t tgt_hue;
		uint8_t tgt_sat;
		uint8_t tgt_val;
		uint16_t loc0;
		uint16_t loc1;
	
	private:
		uint16_t _stpTime;
		long _mClock;
		uint16_t _mstpTime;
		long _cClock;
		uint16_t _cstpTime;
		int _idx;
		bool _idxdir;
		int _its;
		bool _itsdir;
		uint16_t _counter;
};

bool ClockCheck(long *cur_clock, uint16_t stp);
uint16_t limitreset(int dir, uint16_t value, int low=0, int high=255);
uint16_t limitcap(int dir, uint16_t value, int low=0, int high=255);

#endif
