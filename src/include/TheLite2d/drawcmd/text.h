#pragma once
#include "TheLite2d/IDrawCommand.h"
#include "TheLite2d/Texture.h"
#include "TheLite2d/IPositionable.h"
namespace thelite2d {
	namespace drawcmd {
		// This class make a blit on screen
		class Text : public IDrawCommand {
		public:
			// If absPosition true this text is not affected by another components and always is written on desired position
			Text(RectangleWidgetParams pos, std::string msg, SDL_Color color = { 128,128,128,128 }, bool absPosition = false ) : 
				msg{ std::move(msg) }, pos{ std::move(pos) }, color{ std::move(color) }, isAbsoultePosition{ absPosition }  {};

			Text(std::string msg, SDL_Color color = { 128,128,128,128 }) :
				msg{ std::move(msg) }, color{ std::move(color) }, isAbsoultePosition{ true } {
				pos.position = UndefinedPoint2d;
			};
			virtual ~Text() = default;

			// Heredado vía IDrawCommand
			virtual void execute(SDLWrapper * sdl) const override;
			// TODO : delete
			//void applyTranslation(const Point2d &translate) override { if (isAbsoultePosition) return;  pos.applyTranslation(translate); };
			// TODO : delete
			//void applyScaling(const Point2d &applyScale) override { if (isAbsoultePosition) return;  pos.applyScaling(applyScale); };
			void applyTransformation(const RectangleWidgetParams &transformationData) override { if (isAbsoultePosition) return; pos.applyTransformation(transformationData); };
			//virtual void applyTransformation(const PositionRectangle &transformationData) override; 
		private:
			std::string msg;
			RectangleWidgetParams pos;
			SDL_Color color;
			bool isAbsoultePosition = false;
		};
	}
}