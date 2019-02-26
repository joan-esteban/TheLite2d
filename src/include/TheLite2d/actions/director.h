#pragma once
#include "TheLite2d/types.h"
#include "TheLite2d/timer.h"

namespace thelite2d {
	namespace actions {
		// This class 
		class IDirector {
		public:
			virtual ~IDirector() = default;
			virtual void update(const Present & present) = 0;
			virtual SegmentPercent getPercentage() const = 0;
			virtual bool haveReachEnd() = 0;
			virtual void reset() = 0;
		};

		class DirectorTime : public IDirector {
		public:
			const FloatingPointType UNINITIALIZED_TIME = -1;
			DirectorTime(FloatingPointType timeToReach) : timeToReach{ timeToReach }  {};
			
			void update(const Present & present)override {
				if (initialTime==UNINITIALIZED_TIME) initialTime = present.timeSeconds;
				auto elapsedTime = present.timeSeconds - initialTime;
				percent.end = (elapsedTime / timeToReach);
				percent.start= ( (present.beforetimeSeconds - initialTime) / timeToReach);
				
				doned = (timeToReach < elapsedTime);
			}
			
			virtual SegmentPercent getPercentage() const override {
				return percent;
			}
			virtual bool haveReachEnd() override {
				return doned;
			}
			virtual void reset() override {
				initialTime = UNINITIALIZED_TIME;
				doned = false;
			}

			virtual void setFactor(FloatingPointType factor) {
				this->factor = factor;
			}

		private:
			FloatingPointType initialTime = UNINITIALIZED_TIME;
			FloatingPointType timeToReach = 0;
			FloatingPointType factor = 1;
			SegmentPercent percent = { 0,0 };
			bool doned = false;
		};

		class DirectorForced : public IDirector {
		public:
			void update(const Present & present)override {
				// This director is set with a forced value, so there is no calculations here
			}
			virtual SegmentPercent getPercentage() const override {
				return percent;
			}
			virtual bool haveReachEnd() override {
				return doned;
			}

			virtual void reset() override {
	
			}

			void set(SegmentPercent percent, bool doned) {
				this->percent = percent;
				this->doned = doned;
			}
		private:
			SegmentPercent percent = { 0,0};
			bool doned = false;
		};
	}
}