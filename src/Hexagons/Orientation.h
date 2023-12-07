#pragma once

struct Orientation {
	const double f0, f1, f2, f3;
	const double b0, b1, b2, b3;
	const double start_angle; // in multiples of 60°
	Orientation(double f0_, double f1_, double f2_, double f3_,
				double b0_, double b1_, double b2_, double b3_,
				double start_angle_);
};