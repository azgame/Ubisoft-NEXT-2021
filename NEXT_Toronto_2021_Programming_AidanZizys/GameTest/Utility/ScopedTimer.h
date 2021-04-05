#ifndef SCOPEDTIMER_H
#define SCOPEDTIMER_H

#include <chrono>
#include <string>
#include <iostream>

#include "..\App\app.h"

class ScopedTimer
{
public:
	ScopedTimer(std::string FunctionName, std::string FileName, int LineNum) :
		m_profile(FunctionName + " on line " + std::to_string(LineNum) + " of " + FileName), m_fileName(FileName)
	{
		m_start = std::chrono::high_resolution_clock::now();
	}

	~ScopedTimer()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start);

		// std::cout << m_profile + " took: " + std::to_string(elapsed.count() / 1000.0f) + " m/s." << std::endl;
		std::string msg = m_profile + " took: " + std::to_string(elapsed.count() / 1000.0f) + " m/s.";
		App::Print(100, 50, msg.c_str());
	}

	float GetTime()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start);

		return elapsed.count() / 1000.0f;
	}

private:
	std::string m_fileName;
	std::string m_profile;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};


#endif