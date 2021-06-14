#pragma once
#include <chrono>

	class Clock
	{
	private:
		std::chrono::system_clock::time_point tp1;
		std::chrono::system_clock::time_point tp2;
		std::chrono::milliseconds elapsedTime;
	public:
		Clock()
		{
			tp1 = tp2 = std::chrono::system_clock::now();
		}
		void waitForFrame(int fps)
		{
			while((tp2 - tp1) < (std::chrono::milliseconds(1000/fps)))
				tp2 = std::chrono::system_clock::now();
			tp1 = tp2;
		}
		void waitForTime(int sec)
		{
			while ((tp2 - tp1) < (std::chrono::seconds(sec)))
				tp2 = std::chrono::system_clock::now();
			tp1 = tp2;
		}
	};
