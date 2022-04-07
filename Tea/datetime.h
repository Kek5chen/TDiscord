#pragma once

#include <ctime>
#include <cstdio>

tm time_from_string(char* timestamps) {
	int y, M, d, h, m;
	float s;
	sscanf(timestamps, "%d-%d-%dT%d:%d:%f+00:00", &y, &M, &d, &h, &m, &s);

	tm time;

	time.tm_year = y;
	time.tm_mon = m;
	time.tm_mday = d;
	time.tm_hour = h;
	time.tm_min = m;
	time.tm_sec = (int)s;

	return time;
}