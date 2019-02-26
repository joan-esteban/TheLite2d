#include "TheLite2d/impl/MouseReactable.h"
using namespace thelite2d::impl;

MouseReactable::OnClickFunction thelite2d::impl::MouseReactable::setOnClick(OnClickFunction cb) {
	auto old = cbClick;
	cbClick = cb; 
	return old;
}
MouseReactable::OnFocusedFunction thelite2d::impl::MouseReactable::setOnFocused(OnFocusedFunction cb) {
	auto old = cbFocused;
	cbFocused = cb;
	return old;
}
MouseReactable::OnUnfocusedFunction thelite2d::impl::MouseReactable::setOnUnfocused(OnUnfocusedFunction  cb) {
	auto old = cbUnfocused;
	cbUnfocused = cb;
	return old;
}
MouseReactable::OnDragFunction thelite2d::impl::MouseReactable::setOnDrag(OnDragFunction cb) {
	auto old = cbDrag;
	cbDrag = cb;
	return old;
}

MouseReactable::OnDragFinishFunction thelite2d::impl::MouseReactable::setOnDragFinish(OnDragFinishFunction cb) {
	auto old = cbDragFinish;
	cbDragFinish = cb;
	return old;
}


void thelite2d::impl::MouseReactable::onClick(IMouseReactable* obj, const Point2d &where){
	if (cbClick) cbClick(obj, where);
}

void thelite2d::impl::MouseReactable::onFocused(IMouseReactable* obj, const Point2d &where){
	if (cbFocused) cbFocused(obj, where);
}

void thelite2d::impl::MouseReactable::onUnfocused(IMouseReactable* obj, const Point2d &where){
	if (cbUnfocused) cbUnfocused(obj,where);
}

void thelite2d::impl::MouseReactable::onDrag(IMouseReactable* obj, const Point2d & orig, const Point2d & current){
	if (cbDrag) cbDrag(obj,orig, current);
}

void thelite2d::impl::MouseReactable::onDragFinish(IMouseReactable* obj, const Point2d & orig, const Point2d & current) {
	if (cbDragFinish) cbDragFinish(obj, orig, current);
}


