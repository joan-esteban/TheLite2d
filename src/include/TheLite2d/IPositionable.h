#pragma once

#include "types.h"

namespace thelite2d {

	

	

	class IPositionable {
	public:
		virtual ~IPositionable() = default;

		virtual void setPosition(const Point2d &where) = 0;
		// That are natural dimensions (if it's a texture must be the size)
		virtual void setDimensions(const Dimensions & newDimensions) = 0;
		virtual void setScale(const ScaleFactor & newScale) = 0;
		// This change scale to fit newDimensions
		virtual void setScaledDimensions(const Dimensions & newDimensions, FittingMethod fittingMethod = FittingMethod::NONE) = 0;
		virtual void setPivot(PivotMode pivotMode) = 0;
		virtual void setPivot(const Point2d &where) = 0;

		virtual void setVisible(bool isVisible) = 0;
		virtual void setAlpha(FloatingPointType newAlphaValue) = 0;
		virtual void setAngle(FloatingPointType newAngle) = 0;
		virtual void setFlipHorizontal(bool v) = 0;
		virtual void setFlipVertical(bool v) = 0;

		
		virtual Point2d getPosition() const = 0;
		virtual Dimensions getDimensions() const = 0;
		virtual Dimensions getScaledDimensions() const = 0;
		virtual ScaleFactor getScale() const = 0;
		virtual PivotMode getPivotMode() const = 0;
		virtual Point2d getPivotPoint() const = 0;

		virtual bool getVisible() const = 0;
		virtual FloatingPointType getAlpha() const = 0;
		virtual FloatingPointType getAngle() const = 0;

		virtual bool getFlipHorizontal() const = 0;
		virtual bool getFlipVertical() const = 0;


		virtual RectangleWidgetParams getFullData() const = 0;
		virtual void setFullData(RectangleWidgetParams &data) = 0;

	};
}