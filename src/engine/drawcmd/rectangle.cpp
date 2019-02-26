#include "TheLite2d/drawcmd/rectangle.h"

void thelite2d::drawcmd::Rectangle::execute(SDLWrapper * sdl) const
{
	if (!sdl) return;
	SDL_SetRenderDrawBlendMode(sdl->getSdlRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(sdl->getSdlRenderer(), color.r, color.g,color.b, color.a);
	//SDL_SetRenderDrawColor(sdl->getSdlRenderer(), 0, 0, 255, 64);
	thelite2d::Rectangle rect{ (pos.position - pos.calculatedPivotPosition) , pos.dimensions * pos.scale };
	SDL_Rect destRect = rect.covertToSdlRect();
	Uint8 alpha = static_cast<Uint8>(255.0 * pos.alpha);
	if (filled) SDL_RenderFillRect(sdl->getSdlRenderer(), &destRect);
	 else SDL_RenderDrawRect(sdl->getSdlRenderer(), &destRect);

}