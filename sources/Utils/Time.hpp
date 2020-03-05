/*
** wolf, 2020
** Time.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <sys/time.h>

////////////////////////////////////////////////////////////////////////////////

class Time
{
public:
	~Time() = default;

	static double now()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return (double)tv.tv_sec + (double)tv.tv_usec / 1e6;
	}

};