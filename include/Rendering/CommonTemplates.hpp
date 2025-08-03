#pragma once

#include <algorithm>
#include <cstddef>


template<std::size_t N>
struct StringLiteral
{
	constexpr StringLiteral(const char (&str)[N])
	{
		std::copy_n(str, N, String);
	}

	char String[N];
	static constexpr std::size_t Size = N - 1;
};
