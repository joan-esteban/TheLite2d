#pragma once
#include "SDLWrapper.h"
#include "types.h"
namespace thelite2d {
	class SDLWrapper;

	class IDrawCommand {
	public:
		virtual ~IDrawCommand() = default;

		virtual void execute(SDLWrapper *sdl) const = 0;
		//virtual void applyTranslation(const Point2d &translate) = 0;
		//virtual void applyScaling(const ScaleFactor &scale) = 0;
		virtual void applyTransformation(const RectangleWidgetParams &transformationData) = 0;
	};
}