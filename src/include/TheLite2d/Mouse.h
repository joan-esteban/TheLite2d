#pragma once
#include "IEventConsumer.h"
#include "IContainer.h"
#include "Drawer.h"
#include "log.h"

namespace thelite2d {
	class IMouseReactable {
	public:
		virtual ~IMouseReactable() = default;

		virtual void onClick(IMouseReactable* obj, const Point2d &where) = 0;
		virtual void onFocused(IMouseReactable* obj, const Point2d &where) = 0;
		virtual void onUnfocused(IMouseReactable* obj, const Point2d &where) = 0;
		virtual void onDrag(IMouseReactable* obj, const Point2d &orig, const Point2d &current) = 0;
		virtual void onDragFinish(IMouseReactable* obj, const Point2d &orig, const Point2d &current) = 0;
		virtual bool isInside(const Point2d &where) = 0;
		// A point of screen is converted to local coordinates
		virtual Point2d convertToLocalCoordinates(const Point2d & parentCoordinates, IContainer *reachParent) = 0;

	};
	/*
	// TODO: Drag must a be a individual interface
	class IMouseClickable{
	public:
		virtual ~IMouseClickable() = default;
		virtual void onClick(IMouseReactable* obj, const Point2d &where) = 0;

	}
	
	class IMouseDragable {
	public:
		virtual ~IMouseDragable() = default;
		virtual void onStartDrag(IMouseDragable* obj, const Point2d &orig, const Point2d &current) = 0;
		virtual void onDragging(IMouseDragable* obj, const Point2d &orig, const Point2d &current) = 0;
		virtual void onDropDrag(IMouseDragable* obj, const Point2d &orig, const Point2d &current) = 0;
		virtual void onCancelDrag(IMouseDragable* obj, const Point2d &orig, const Point2d &current) = 0;
	};
	*/

	class Mouse : public Object,public IInsertable, public IDrawable, public IEventConsumer {
	public:
		virtual ~Mouse() = default;

		// Heredado vía IInsertable
		virtual void setParent(IContainer *) override;

		virtual IContainer * getParent() const override;


		// Heredado vía IDrawable
		virtual bool getDrawCommands(const Present & present, std::list<std::unique_ptr<IDrawCommand>>& drawCommands) override;


		// Heredado vía IEventConsumer
		virtual bool onEvent(const events::Event * event) override;

		// Returns if event is absorbed
		bool MouseDown(Drawer *drawer, const events::Event *event);
		// Returns if event is absorbed
		bool MouseUp(Drawer *drawer, const events::Event *event);
		// Returns if event is absorbed
		bool MouseMotion(Drawer *drawer, const events::Event *event);

		void setNoFocus(const events::Event *event);

	protected:
		bool isValidFocusedItem();
		// This is item focused, all events until button up are sended to it
		std::shared_ptr<IMouseReactable> focused = nullptr;
		Point2d mouseDownPosition = { 0,0 };
		bool isClicked = false;
		bool isDraging = false;
		IContainer *parent = nullptr;
		Point2d lastMousePosition = UndefinedPoint2d;
		const FloatingPointType DRAG_THRESHOLD = 2.0;

		

	};
}