#ifndef __GAMETIMER__H__
#define __GAMETIMER__H__

#include <chrono>
#include <ctime>

using namespace std::chrono;

class GameTimer {
public:
	GameTimer() {
		mLastTime = high_resolution_clock::now();
		mStartTime = high_resolution_clock::now();
	}

	void Tick() {
		auto currentTime = high_resolution_clock::now();
		auto deltaTime = currentTime - mLastTime;
		mLastTime = currentTime;

		double deltaTimeSec = duration<double>(deltaTime).count();

		mFps = 1.0 / deltaTimeSec;
	}

	double runTime() {
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - mStartTime);
		return duration.count() / 1000.0;
	}

	double GetFps() const { return mFps; };

private:
	time_point<high_resolution_clock> mLastTime;
	double mFps;
	time_point<high_resolution_clock> mStartTime;

};

#endif