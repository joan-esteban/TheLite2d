#pragma once
#include "TheLite2d/IDrawCommand.h"
#include "TheLite2d/Texture.h"
#include "TheLite2d/IPositionable.h"
namespace thelite2d {
	namespace drawcmd {
		// This class make a blit on screen
		class Blit : public IDrawCommand {
		public:
			Blit(Texture texture, RectangleWidgetParams pos) : texture{ std::move(texture) }, pos{ std::move(pos) } {};
			Blit(Texture texture, RectangleWidgetParams pos, Rectangle textureSource) : texture{ std::move(texture) }, pos{ std::move(pos) }, useSource{ true }, textureSource{ std::move(textureSource) }  {};
			virtual ~Blit() = default;

			// Heredado vía IDrawCommand
			virtual void execute(SDLWrapper * sdl) const override;
			// TODO : delete
			//void applyTranslation(const Point2d &translate) override { pos.applyTranslation(translate); };
			// TODO : delete
			//void applyScaling(const Point2d &applyScale) override { pos.applyScaling(applyScale); };
			void applyTransformation(const RectangleWidgetParams &transformationData) override { pos.applyTransformation(transformationData);  };

			//virtual void applyTransformation(const PositionRectangle &transformationData) override; 
		private:
			Texture texture;
			RectangleWidgetParams pos;
			bool useSource = false;
			Rectangle textureSource;

			
			
		};
	}
}