#include "Random.h"

static std::random_device RandDev;
static std::mt19937 RNG{RandDev()};

int Random::Int(const uint64_t Max)
{
	std::uniform_int_distribution<> distrib(0, Max);
	return distrib(RNG);
}

int Random::RangeInt(const uint64_t Min, const uint64_t Max)
{
	std::uniform_int_distribution<> distrib(Min, Max);
	return distrib(RNG);
}


float Random::Float(const float Max)
{
	std::uniform_real_distribution<float> distrib(0.0f, Max);
	return distrib(RNG);
}

float Random::RangeFloat(const float Min, const float Max)
{
	std::uniform_real_distribution<float> Distr(Min, Max);
	return Distr(RNG);
}


double Random::Double(const double Max)
{
	std::uniform_real_distribution<double> Distr(0.0f, Max);
	return Distr(RNG);
}

double Random::RangeDouble(const double Min, const double Max)
{
	std::uniform_real_distribution<double> Distr(Min, Max);
	return Distr(RNG);
}


bool Random::Bool() {
	std::bernoulli_distribution BoolDist(0.5);
	return BoolDist(RNG);
}


float Random::Normal() {
	return RangeFloat(0.f, 1.f);
}