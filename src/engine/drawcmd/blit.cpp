#include "TheLite2d/drawcmd/blit.h"
#include "TheLite2d/log.h"
void thelite2d::drawcmd::Blit::execute(SDLWrapper * sdl) const
{
	//auto textureData = texture.lock();
	SDL_Texture *sdlTexture = nullptr;
	auto textureShared = texture.getSdltexture().lock();
	if (!textureShared) {
		SPDLOG_INFO(getLogger(), "Texture outdated!, cant blit!");
		return;
	};
	sdlTexture = textureShared.get();

	if (!sdl) return;
	Rectangle rect{ (pos.position - pos.calculatedPivotPosition) , pos.dimensions * pos.scale };
	SDL_Rect destRect = rect.covertToSdlRect();
	SDL_Point center; 
	center.x = pos.calculatedPivotPosition.x;
	center.y = pos.calculatedPivotPosition.y;

	Uint8 alpha = static_cast<Uint8>(255.0 * pos.alpha);
	/*
	SDL_SetRenderDrawBlendMode(sdl->getSdlRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(sdl->getSdlRenderer(), 0, 0, 255, 64);
	SDL_RenderFillRect(sdl->getSdlRenderer(), &destRect);
	*/
	SDL_SetTextureAlphaMod(sdlTexture, alpha);
	//SDL_SetTextureColorMod(sdlTexture, 128, 0, 0);
	const SDL_Rect *srctRect = texture.sourceSdlRect();
	SDL_RendererFlip flipMode = SDL_FLIP_NONE;
	if (pos.flipHorizontal) flipMode = SDL_RendererFlip(flipMode| SDL_FLIP_HORIZONTAL);
	if (pos.flipVertical) flipMode = SDL_RendererFlip(flipMode |  SDL_FLIP_VERTICAL);

	if (srctRect) {
		//SDL_Rect srctRect = textureSource.cove;rtToSdlRect();
		SDL_RenderCopyEx(sdl->getSdlRenderer(), sdlTexture, srctRect, &destRect, pos.angle, &center, flipMode);
	}
	else SDL_RenderCopyEx(sdl->getSdlRenderer(), sdlTexture, NULL, &destRect, pos.angle, &center, flipMode);


}

