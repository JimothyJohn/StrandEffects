# StrandEffects
Effect classes for FastLED

## Function List
Shift(seconds, distance) - Moves "distance" in "seconds" once\n
Scan(seconds, distance) - Continuously moves "distance" in "seconds"
Phase(seconds, r, g, b) - Increments RGB channels by "r/g/b" every "seconds"
Bounce(seconds, distance) - Moves "distance" in "seconds" and returns repeatedly
Jump(seconds) - Move to random spot on strand every "seconds"
SetRGB(r, g, b) - Sets RGB values to "r/g/b"
GetRGB() - Extracts current RGB values to setpoint
Flash(seconds, r, g, b) - Alternates between RGB value or "r/g/b" and 0
SetHSV(h, s, v) - Sets HSV values to "h/s/v"
GetHSV() - Extracts current HSV values to setpoint
Pulse(float sTime, intensity) - Increase and decreases by "intensity" every "seconds"
Saturate(seconds, intensity) - Increase saturation by "intensity" in "seconds"
Brighten(seconds, intensity) - Increase value by "intensity" in "seconds"
BrightenTo(seconds, intensity) - Increase value to "intensity" in "seconds"
Dim(seconds, intensity) - Decrease value by "intensity" in "seconds"
DimTo(seconds, intensity) - Decrease value to "intensity" in "seconds"
RainbowCycle(seconds) - Cycle hue through rainbow every "seconds"
