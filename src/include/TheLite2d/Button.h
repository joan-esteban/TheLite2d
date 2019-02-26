#pragma once
#include <memory>
#include "Texture.h"
#include "Sprite.h"
#include "IEventConsumer.h"
#include "Mouse.h"
#include "impl/MouseReactable.h"
namespace thelite2d {
	class Button : public Sprite, public impl::MouseReactable {
	public:
		Button(Texture texture);
		//Button(const Button &but); = default
		// Heredado via IDrawable
		// bool getDrawCommands(const Present &present, std::list<std::unique_ptr<IDrawCommand>> &drawCommands) override 
		void setBackgroundColor(SDL_Color color) { bgColor = color; };
		virtual bool getDrawCommands(const Present & present, std::list<std::unique_ptr<IDrawCommand>>& drawCommands) override;

		static std::shared_ptr<Button> createInstance(Texture texture) {
			return std::shared_ptr<Button>(new Button{ std::move(texture) });
		}

		static std::shared_ptr<Button> createInstance(const Button &but) {
			return std::shared_ptr<Button>(new Button{ but });
		}

	
		virtual bool isInside(const Point2d & where) override;

		// Heredado vía IMouseReactable
		virtual Point2d convertToLocalCoordinates(const Point2d & parentCoordinates, IContainer *reachParent) override;

		void setEnabled(bool v) { isEnabled = v; }
		bool getEnabled() const { return isEnabled; }
		void setCaptureEverything(bool v) { captureEverything = v; }
	private:
		bool isEnabled = true;
		bool captureEverything = false;
		SDL_Color bgColor = { 0,0,0,0 };

	};
}