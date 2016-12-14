#pragma once
#include <string>
#include <spdlog/spdlog.h>
#include <vector>


inline auto make_logger(const std::string &name)
{
	try
	{
		return spdlog::stdout_logger_mt(name, true);
	}
	catch (spdlog::spdlog_ex &) {
		return spdlog::get(name);
	}
}

struct FibonacciNumber
{
	int n;
	int64_t number;
};


using FibonacciNumbers = std::vector<FibonacciNumber>;