#pragma once
#include "TheLite2d/types.h"
namespace thelite2d {

	template <typename T>
	class Interpolater { 
	public:
		// Some can be initialize from current position/value to a destination
		virtual bool isNeedInitialValue() const = 0;
		virtual void setInitialValue(T xo) = 0;
		virtual T getDelta(FloatingPointType percent) = 0;

	};


	template <typename T>
	struct UniformLineMotion : public Interpolater<T> {
		UniformLineMotion(T xo, T xf) : xo{ xo }, xf{ xf }, needInitialValue{ false } {
			calculateConstants();
		}
		UniformLineMotion(T xf) : xf{ xf }, needInitialValue{ true } {
		};

		bool isNeedInitialValue() const override { return needInitialValue; }

		void setInitialValue(T newInitialValue) override {
			this->xo = newInitialValue;
			this->needInitialValue = false;
			calculateConstants();
		}

		void calculateConstants() {
			vi = (xf -xo);
		}

		T getDelta(FloatingPointType percent) override {
			return xo + (vi * percent);
		}
		bool needInitialValue = true;
		T xo, xf;
		T vi;
	};


	// movimiento rectilíneo uniformemente acelerado https://www.fisicalab.com/apartado/mrua-ecuaciones#contenidos (UniformlyAcceleratedRectilinearMotion)
	template <typename T>
	struct UARMotion : public Interpolater<T> {
		UARMotion(T xo, T xf, T vf) : xo{ xo }, xf{ xf }, vf{ vf }, needInitialValue{ false }  {
			calculateConstants();
		};
		UARMotion(T xf, T vf) :  xf{ xf }, vf{ vf }, needInitialValue{ true } {
		};

		UARMotion(T xf) : xf{ xf }, vf{}, needInitialValue{ true } {
		};

		bool isNeedInitialValue() const override { return needInitialValue;  }
		void setInitialValue(T initialValue) override {
			this->xo = initialValue;
			this->needInitialValue = false;
			calculateConstants();
		}
		void calculateConstants() {
			FloatingPointType timeToReach = 1.0;
			a = (xf - xo - vf * timeToReach) / (-(timeToReach*timeToReach) + ((timeToReach* timeToReach) / 2));
			vi = vf - a * timeToReach;
		}
		T getDelta(FloatingPointType percent) override {
			return xo + (vi * percent) + (a * percent * percent) / 2;
		}
	protected:
		bool needInitialValue = true;
		T xo, xf, vf;
		T a, vi;
	};

	template <typename T>
	struct CopyInterpolater : public Interpolater<T> {
		CopyInterpolater( std::shared_ptr< actions::ISetter<T> > setter,
						  std::weak_ptr<Object> follow, T offset) :
			needInitialValue{ true }, offset{ offset }, follow{ follow }, setter{ setter } {
			
		};
		

		bool isNeedInitialValue() const override { return needInitialValue; }
		void setInitialValue(T initialValue) override {
			this->xo = initialValue;
			this->needInitialValue = false;
			
		}
		
		T getDelta(FloatingPointType percent) override {
			auto shared = follow.lock();
			if (!shared) return last;
			last = setter->getInitialValue(*shared.get()) + offset;
			return last;
		}
	protected:
		bool needInitialValue = true;
		std::weak_ptr<Object> follow;
		T xo;
		T offset;
		T last;
		std::shared_ptr< actions::ISetter<T> > setter;
	};

}