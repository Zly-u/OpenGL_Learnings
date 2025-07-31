#pragma once

#include <cstdio>
#include <format>

namespace Log
{
	template<typename... Args>
	__forceinline void print(const std::string_view&& InFormatString, Args&&... PassedArgs)
	{
		std::printf("%s", std::vformat(InFormatString, std::make_format_args(PassedArgs...)).c_str());
	}

	template<typename... Args>
	__forceinline void println(const std::string_view&& InFormatString, Args&&... PassedArgs)
	{
		std::printf("%s\n", std::vformat(InFormatString, std::make_format_args(PassedArgs...)).c_str());
	}
}
