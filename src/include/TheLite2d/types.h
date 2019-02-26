#pragma once
#include <chrono>
#include <cmath>
#include <iostream>
#include <SDL.h>

namespace thelite2d {
	using  FloatingPointType = double;
	//using  MillisecondsType = std::chrono::milliseconds;
	using  SecondsFloatType = FloatingPointType;
	const FloatingPointType UNDEFINED_VALUE = INFINITY;
	constexpr long convertFloatToLong(SecondsFloatType v) { return static_cast<long>(v);  }
	constexpr int convertFloatToInt(SecondsFloatType v) { return static_cast<int>(v);  }

	//template <typedef T>
	//constexpr SecondsFloatType convertToFloat<T>(T v) { return static_cast<SecondsFloatType>(v); }
	constexpr SecondsFloatType convertToFloat(int v) { return static_cast<SecondsFloatType>(v); }
#ifndef __ANDROID__
	constexpr SecondsFloatType convertToFloat(__int64 v) { return static_cast<SecondsFloatType>(v); }
#endif
	struct Point2d {
		Point2d() : x{ 0 }, y{ 0 } {};
		Point2d(FloatingPointType x, FloatingPointType y) : x{ x }, y{ y } {};
		//Point2d(int x, int y) : x{ FloatingPointType(x) }, y{ FloatingPointType(y) } {};

		FloatingPointType x, y;

		FloatingPointType modulus() const {
			return sqrt(x*x + y*y);
		}

		FloatingPointType distanceTo(const Point2d &b) {
			return (*this - b).modulus();
		}

		Point2d operator *(Point2d factor) const{
			return { x * factor.x,y * factor.y };
		}

		Point2d operator /(Point2d factor) const {
			return { x / factor.x,y / factor.y };
		}

		Point2d &operator *=(Point2d factor) {
			x *= factor.x;
			y *= factor.y;
			return *this;
		}

		Point2d operator +(Point2d point) const{
			return { x + point.x,y + point.y };
		}
		
		Point2d &operator +=(Point2d point) {
			x += point.x;
			y += point.y;
			return *this;
		}

		bool operator ==(const Point2d &point) const {
			return  ( x == point.x) && (y == point.y);
		}

		bool operator !=(const Point2d &point) const {
			return  (x != point.x) || (y != point.y);
		}

		Point2d operator -(Point2d point) const {
			return { x - point.x,y - point.y };
		}
		template<typename T>
		Point2d operator /(T value) const {
			return { x/value , y/value };
		}

		template<typename T>
		Point2d operator *(T value) const {
			return { x * value , y * value };
		}
		
		void merge(Point2d value) {
			if (value.x != UNDEFINED_VALUE) x = value.x;
			if (value.y != UNDEFINED_VALUE) y = value.y;

		}

		void mask(Point2d value) {
			if (x == UNDEFINED_VALUE || value.x == UNDEFINED_VALUE) x = UNDEFINED_VALUE;
			if (y == UNDEFINED_VALUE || value.y == UNDEFINED_VALUE) y = UNDEFINED_VALUE;

		}
		
	};

	inline std::ostream& operator<<(std::ostream& os, const Point2d& obj)
	{
		os << "(" <<  obj.x<< "," << obj.y << ")";
		return os;
	}
	
	const Point2d UndefinedPoint2d = Point2d{ UNDEFINED_VALUE, UNDEFINED_VALUE };

	using ScaleFactor = Point2d;
	

	using Dimensions = Point2d;
	const Dimensions UndefinedDimensions = Dimensions{ UNDEFINED_VALUE, UNDEFINED_VALUE };


	struct Rectangle {
		Point2d    xy;
		Dimensions wh;


		Rectangle() {};
		Rectangle(const SDL_Rect &rect) { 
			xy.x = static_cast<FloatingPointType>(rect.x); 
			xy.y = static_cast<FloatingPointType>(rect.y); 
			wh.x = static_cast<FloatingPointType>(rect.w); 
			wh.y = static_cast<FloatingPointType>(rect.h); 
		};
		Rectangle(Point2d position, Dimensions dimensions) : xy{ std::move(position) }, wh{ std::move(dimensions) } {};
		

		 SDL_Rect covertToSdlRect() const {
			SDL_Rect rect;
			rect.x = convertFloatToLong(xy.x);
			rect.y = convertFloatToLong(xy.y);
			rect.w = convertFloatToLong(wh.x);
			rect.h = convertFloatToLong(wh.y);
			return rect;
		}

		 bool isInside(const Point2d &point) const {
			 return (point.x >= xy.x ) && 
				 (point.y >= xy.y ) && 
				 (xy.x + wh.x >= point.x) && 
				 (xy.y + wh.y >= point.y);
		 }
	};

	struct Present {
		SecondsFloatType timeSeconds;
		SecondsFloatType beforetimeSeconds;
		std::chrono::milliseconds elapsedMs;
	};

	struct SegmentPercent {

		constexpr  SegmentPercent(FloatingPointType c, FloatingPointType b) :
			start{ c }, end{ b } {};

		FloatingPointType start;
		FloatingPointType end;
		


		constexpr FloatingPointType getElapsed() const {
			return end - start;
		}
		constexpr bool isInside(FloatingPointType percent) const {
			return (percent >= start) && (percent <= end);
		}
		
		constexpr bool thereAreIntersection(const SegmentPercent &delta) const {
			return isInside(delta.end) || isInside(delta.start);
		}

		constexpr bool operator==(const SegmentPercent &delta) const {
			return (delta.end == end) && (delta.start == delta.start);
		}

		SegmentPercent operator +(const SegmentPercent &rsh) const {
			return { start + rsh.start,end+ rsh.end  };
		}

		SegmentPercent &operator +=(const SegmentPercent &rsh) {
			start += rsh.start;
			end += rsh.end;
			return *this;
		}

		SegmentPercent operator +(const FloatingPointType &rsh) const {
			return { start + rsh,end + rsh };
		}

		SegmentPercent &operator +=(const FloatingPointType &rsh) {
			start += rsh;
			end += rsh;
			return *this;
		}

		SegmentPercent operator *(const FloatingPointType &rsh) const {
			return { start * rsh,end * rsh };
		}

		SegmentPercent operator /(const FloatingPointType &rsh) const {
			return { start / rsh,end / rsh };
		}

	};

	inline std::ostream& operator<<(std::ostream& os, const SegmentPercent& obj)
	{
		os << "( cur:" << obj.end << " old:" << obj.start << "delta:" << obj.getElapsed() << ")";
		return os;
	}


	enum class  PivotMode {
		ON_TOP_LEFT,
		ON_MIDDLE,
		ON_BOTTOM_MIDDLE,
		ON_TOP_MIDDLE,
		ON_MIDDLE_LEFT,
		ON_MIDDLE_RIGHT,
		CUSTOM		//< It needs a value that say where is pivot point
	};

	enum class FittingMethod {
		NONE,
		KEEP_ASPECT_RATIO,
		KEEP_ASPECT_RATIO_AND_MOVE_TO_CENTER,

	};
	namespace constants {
		const FloatingPointType ALPHA_MIN = 0.0;
		const FloatingPointType ALPHA_MAX = 1.0;
	}
	struct RectangleWidgetParams {
		Point2d position = { 0,0 };
		Dimensions dimensions = { 0,0 };
		//bool clipping = true;
		// Rectangle position
		ScaleFactor scale = { 1.0, 1.0 };
		bool visibility = true;
		FloatingPointType alpha = constants::ALPHA_MAX;
		FloatingPointType angle = 0;

		PivotMode pivotMode = PivotMode::ON_MIDDLE;
		Point2d pivotPosition = { 0,0 };
		Point2d calculatedPivotPosition = { 0,0 };

		
		bool flipHorizontal = false;
		bool flipVertical = false;

		void applyTranslation(const Point2d &translate) { position += translate; };
		void applyScaling(const ScaleFactor &applyScale) {
			position *= applyScale;
			calculatedPivotPosition *= applyScale;
			dimensions *= applyScale;
		};
		void applyTransformation(const RectangleWidgetParams &transformationData) {
			
			
			Point2d translate = (transformationData.position - transformationData.calculatedPivotPosition);
			applyScaling(transformationData.scale);
			applyTranslation(translate);
			alpha = alpha * transformationData.alpha;
			
			// TODO: rotation transformation
			//angle = angle + transformationData.angle;
		}

		Point2d transform(const Point2d &where) const {
			return (where * scale) + (position - calculatedPivotPosition);
		}

		Point2d inverseTransform(const Point2d &where) const {
			return  ((where - (position - calculatedPivotPosition)) / scale);

		}
	};

	constexpr Uint8 convertColorComponent(FloatingPointType v)  {
		if (v <= 0) return 0;
		if (v >= 1) return 255;
		return static_cast<Uint8>(255.0 *v);
	}

	struct Color {
		FloatingPointType r;
		FloatingPointType g;
		FloatingPointType b;
		FloatingPointType a;

		SDL_Color covertToSdlColor() const {
			SDL_Color color;
			color.r = convertColorComponent(r);
			color.g = convertColorComponent(g);
			color.b = convertColorComponent(b);
			color.a = convertColorComponent(a);
			return color;
		}

		Color operator +(const Color &rsh) const {
			return { r + rsh.r,b + rsh.b, g + rsh.g, a + rsh.a };
		}
		Color operator -(const Color &rsh) const {
			return { r - rsh.r,b - rsh.b, g - rsh.g, a - rsh.a };
		}

		Color &operator +=(const Color &rsh) {
			r += rsh.r;
			b += rsh.b;
			g += rsh.g;
			a += rsh.a;
			return *this;
		}



		Color operator *(const Color &rsh) const {
			return { r * rsh.r,b * rsh.b, g * rsh.g, a * rsh.a };
		}

		Color operator /(const Color &rsh) const {
			return { r / rsh.r,b / rsh.b, g / rsh.g, a / rsh.a };
		}
	};

}