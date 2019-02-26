#pragma once
#include <chrono>
#include "types.h"

namespace thelite2d {
	class Timer {
	public:
		Timer() {
			startingTimeMs = getCurrentMs();
			checkPointTimeMs = startingTimeMs;
			currentPresent = { getTotalElapsedSecs(), getTotalElapsedSecs(), getElapsedMs() };
		}

		std::chrono::milliseconds getCurrentMs() const {
			return std::chrono::milliseconds{ SDL_GetTicks() };
		}

		std::chrono::milliseconds getTotalElapsedMs() const {
			return getCurrentMs() - startingTimeMs;
		}

		SecondsFloatType getTotalElapsedSecs() const {
			return (convertToFloat(getTotalElapsedMs().count()) ) / 1000.0;
		}

		std::chrono::milliseconds getElapsedMs() {
			auto currentMs = getCurrentMs();
			auto diffMs = currentMs - checkPointTimeMs;
			checkPointTimeMs = currentMs;
			return diffMs;
		}
		void next() {
			SecondsFloatType elapsedSeconds = (convertToFloat(getElapsedMs().count())) / 1000.0;
			currentPresent = { currentPresent.timeSeconds + elapsedSeconds , currentPresent.timeSeconds };
			//currentPresent = { getTotalElapsedSecs(), currentPresent.timeSeconds };
		}

		void next(std::chrono::milliseconds elapsedMs) {
			SecondsFloatType elapsedSeconds = (convertToFloat(elapsedMs.count())) / 1000.0;
			currentPresent = {  currentPresent.timeSeconds + elapsedSeconds , currentPresent.timeSeconds };
		}
		Present current() {
			return currentPresent;
		}

	protected:
		std::chrono::milliseconds startingTimeMs = std::chrono::milliseconds{ 0 };
		std::chrono::milliseconds checkPointTimeMs = std::chrono::milliseconds{ 0 };
		Present currentPresent;
	};
}