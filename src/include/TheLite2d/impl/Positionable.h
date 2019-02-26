#pragma once
#include "TheLite2d/IPositionable.h"

/// This is a helper class to don't repeat this code
namespace thelite2d {
	namespace impl {
		class Positionable : virtual public IPositionable {
		public:
			// Heredado vía IPositionable
			virtual void setPosition(const Point2d & where) override;
			virtual void setDimensions(const Dimensions & newDimensions) override;
			virtual void setScale(const ScaleFactor & newScale) override;
			virtual void setScaledDimensions(const Dimensions & newDimensions, FittingMethod fittingMethod = FittingMethod::NONE) override;
			virtual void setPivot(PivotMode pivotMode) override;
			virtual void setPivot(const Point2d & where) override;
			virtual void setVisible(bool isVisible) override;
			virtual void setAlpha(FloatingPointType newAlphaValue) override;
			virtual void setAngle(FloatingPointType newAngle) override;

			virtual void setFlipHorizontal(bool v) override;
			virtual void setFlipVertical(bool v) override;

			virtual Point2d getPosition() const override;
			virtual Dimensions getDimensions() const override;
			virtual Dimensions getScaledDimensions() const override;
			virtual ScaleFactor getScale() const override;
			virtual PivotMode getPivotMode() const override;
			virtual Point2d getPivotPoint() const override;
			virtual bool getVisible() const override;
			virtual FloatingPointType getAlpha() const override;
			virtual FloatingPointType getAngle() const override;
			virtual bool getFlipHorizontal() const override;
			virtual bool getFlipVertical() const override;

			virtual RectangleWidgetParams getFullData() const override;
			virtual void setFullData(RectangleWidgetParams &data)  override;
		protected:
			Point2d calcultePivotPoint() const;
		private:
			RectangleWidgetParams data;

			// Heredado vía IPositionable
			
			
		};
	}
}