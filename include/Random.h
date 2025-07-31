#pragma once

#include <random>

namespace Random {
	int Int(const uint64_t Max);
	int RangeInt(const uint64_t Min, const uint64_t Max);

	float Float(const float Max);
	float RangeFloat(const float Min, const float Max);

	double Double(const double Max);
	double RangeDouble(const double Min, const double Max);

	bool Bool();
	float Normal();
}