#include <algorithm>
#include "TheLite2d/impl/Positionable.h"
#include "TheLite2d/types.h"

using namespace thelite2d;

void thelite2d::impl::Positionable::setPosition(const Point2d & where)
{
	data.position.merge(where);
}

void thelite2d::impl::Positionable::setDimensions(const Dimensions & newDimensions)
{
	data.dimensions.merge(newDimensions);
}

void thelite2d::impl::Positionable::setScale(const ScaleFactor & newScale)
{
	data.scale.merge(newScale);
}

void thelite2d::impl::Positionable::setScaledDimensions(const Dimensions & newDimensions, FittingMethod fittingMethod)
{
	Dimensions current = getDimensions();
	switch (fittingMethod) {
	case FittingMethod::NONE:
		setScale( newDimensions / current );
		break;
	case FittingMethod::KEEP_ASPECT_RATIO_AND_MOVE_TO_CENTER:
	case FittingMethod::KEEP_ASPECT_RATIO:
		auto factor = newDimensions / current;
		FloatingPointType factorMin = std::min(factor.x, factor.y);
		ScaleFactor newScale = { (factorMin), (factorMin) };
		Point2d translate = (newDimensions - (current * newScale)) / 2;
		setScale(newScale);
		if (fittingMethod == FittingMethod::KEEP_ASPECT_RATIO_AND_MOVE_TO_CENTER ) setPosition(getPosition() + translate);
		break;

	}

}


void thelite2d::impl::Positionable::setPivot(PivotMode newPivotMode)
{
	data.pivotMode = newPivotMode;
}

void thelite2d::impl::Positionable::setPivot(const Point2d & where)
{
	data.pivotMode = PivotMode::CUSTOM;
	data.pivotPosition.merge(where);
}

void thelite2d::impl::Positionable::setVisible(bool isVisible)
{
	data.visibility = isVisible;
}

void thelite2d::impl::Positionable::setAlpha(FloatingPointType newAlphaValue)
{
	if (newAlphaValue == UNDEFINED_VALUE) return;
	data.alpha = newAlphaValue;
}

void thelite2d::impl::Positionable::setAngle(FloatingPointType newAngle)
{
	if (newAngle == UNDEFINED_VALUE) return;
	data.angle = newAngle;
}

void thelite2d::impl::Positionable::setFlipHorizontal(bool v) {
	data.flipHorizontal = v;
}

void thelite2d::impl::Positionable::setFlipVertical(bool v) {
	data.flipVertical = v;
}


Point2d thelite2d::impl::Positionable::getPosition() const
{
	return data.position;
}

Dimensions thelite2d::impl::Positionable::getDimensions() const
{
	return data.dimensions;
}

Dimensions thelite2d::impl::Positionable::getScaledDimensions() const
{
	return getDimensions() * getScale() ;
}

Dimensions thelite2d::impl::Positionable::getScale() const
{
	return data.scale;
}

PivotMode thelite2d::impl::Positionable::getPivotMode() const
{
	return data.pivotMode;
}

Point2d thelite2d::impl::Positionable::getPivotPoint() const
{
	return data.pivotPosition;
}

bool thelite2d::impl::Positionable::getVisible() const
{
	return data.visibility;
}

FloatingPointType thelite2d::impl::Positionable::getAlpha() const
{
	return data.alpha;
}

FloatingPointType thelite2d::impl::Positionable::getAngle() const
{
	return data.angle;
}

Point2d thelite2d::impl::Positionable::calcultePivotPoint() const
{
	switch (getPivotMode() ) {
	case PivotMode::CUSTOM:
		return getPivotPoint();
	case PivotMode::ON_MIDDLE:
		return getScaledDimensions() / 2;
	case PivotMode::ON_BOTTOM_MIDDLE:
		return { getScaledDimensions().x / 2, getScaledDimensions().y };
	case PivotMode::ON_TOP_MIDDLE:
		return { getScaledDimensions().x / 2, 0 };
	case PivotMode::ON_MIDDLE_LEFT:
		return { 0, getScaledDimensions().y/2 };
	case PivotMode::ON_MIDDLE_RIGHT:
		return { getScaledDimensions().x , getScaledDimensions().y / 2 };
	default:
		return { 0.0, 0.0 };
	}
}

bool thelite2d::impl::Positionable::getFlipHorizontal() const {
	return data.flipHorizontal;
};
bool thelite2d::impl::Positionable::getFlipVertical() const {
	return data.flipVertical;
};

RectangleWidgetParams thelite2d::impl::Positionable::getFullData() const
{
	RectangleWidgetParams data; 
	data.alpha = getAlpha();
	data.angle = getAngle();
	data.position = getPosition();
	data.dimensions = getDimensions();
	data.pivotMode = getPivotMode();
	data.pivotPosition = getPivotPoint();
	data.calculatedPivotPosition = calcultePivotPoint();
	data.scale = getScale(); 
	data.visibility = getVisible();
	data.flipHorizontal = getFlipHorizontal();
	data.flipVertical = getFlipVertical();
	return data;
}

void  thelite2d::impl::Positionable::setFullData(RectangleWidgetParams &data) {
	setAlpha(data.alpha);
	setAngle(data.angle);
	setPosition(data.position);
	setScale(data.scale);

}
