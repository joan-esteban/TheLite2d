#include "TheLite2d/drawcmd/text.h"

void thelite2d::drawcmd::Text::execute(SDLWrapper * sdl) const
{
	if (!sdl) return;
	Rectangle rect{ (pos.position - pos.calculatedPivotPosition) , pos.dimensions * pos.scale };
	SDL_Rect destRect = rect.covertToSdlRect();
	Uint8 alpha = static_cast<Uint8>(255.0 * pos.alpha);
	sdl->writeText(msg, rect.xy, color);

}