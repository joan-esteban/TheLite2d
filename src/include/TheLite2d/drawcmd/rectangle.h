#pragma once
#include "TheLite2d/IDrawCommand.h"
#include "TheLite2d/Texture.h"
#include "TheLite2d/IPositionable.h"
namespace thelite2d {
	namespace drawcmd {
		// This class make a blit on screen
		class Rectangle : public IDrawCommand {
		public:
			Rectangle(RectangleWidgetParams pos, SDL_Color color = { 128,128,128,255 }, bool filled = false ) : 
				color{ std::move(color) }, pos{ std::move(pos) }, filled{ filled }  {};
			virtual ~Rectangle() = default;

			// Heredado vía IDrawCommand
			virtual void execute(SDLWrapper * sdl) const override;

			void applyTransformation(const RectangleWidgetParams &transformationData) override { pos.applyTransformation(transformationData); };

		private:
			SDL_Color color;
			bool filled = false;
			RectangleWidgetParams pos;
		};
	}
}