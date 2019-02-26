#pragma once
#include "TheLite2d/Mouse.h"
namespace thelite2d {
	namespace impl {
		class MouseReactable : public IMouseReactable {
		public:
			using OnClickFunction = std::function<void(IMouseReactable* obj,const Point2d &)>;
			using OnFocusedFunction = std::function<void(IMouseReactable* obj, const Point2d &)>;
			using OnUnfocusedFunction = std::function<void(IMouseReactable* obj, const Point2d &)>;
			using OnDragFunction = std::function<void(IMouseReactable* obj, const Point2d & orig, const Point2d & current)>;
			using OnDragFinishFunction = std::function<void(IMouseReactable* obj, const Point2d & orig, const Point2d & current)>;

			virtual ~MouseReactable() = default;
			
			OnClickFunction setOnClick(OnClickFunction cb);
			OnFocusedFunction setOnFocused(OnFocusedFunction cb);
			OnUnfocusedFunction setOnUnfocused(OnUnfocusedFunction  cb);
			OnDragFunction setOnDrag(OnDragFunction cb);
			OnDragFunction setOnDragFinish(OnDragFinishFunction cb);
		
			// Heredado vía IMouseReactable
			virtual void onClick(IMouseReactable* obj,const Point2d &where) override;

			virtual void onFocused(IMouseReactable* obj, const Point2d &where) override;

			virtual void onUnfocused(IMouseReactable* obj, const Point2d &where) override;

			virtual void onDrag(IMouseReactable* obj, const Point2d & orig, const Point2d & current) override;

			virtual void onDragFinish(IMouseReactable* obj, const Point2d &orig, const Point2d &current) override;
			// This must be implemented by derived class
			//virtual bool isInside(const Point2d & where) override;

			// This must be implemented by derived class
			//virtual Point2d convertToLocalCoordinates(const Point2d & parentCoordinates, IContainer * reachParent) override;
		private:
			OnClickFunction cbClick = nullptr;
			OnFocusedFunction cbFocused = nullptr;
			OnUnfocusedFunction cbUnfocused = nullptr;
			OnDragFunction cbDrag = nullptr;
			OnDragFinishFunction cbDragFinish = nullptr;
		};
	}
}