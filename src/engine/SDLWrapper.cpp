#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
//#include <SDL_mixer.h>
#include "TheLite2d/SDLWrapper.h"
#include "TheLite2d/log.h"
#include "TheLite2d/config.h"

using namespace thelite2d;

#define SDLTTF 

bool SDLWrapper::initialize(Dimensions dimensions) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SPDLOG_ERROR(getLogger(),"SDL_Init error");
        return false;
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        SPDLOG_ERROR(getLogger(),"IMG_init error");
        return false;
    }
#ifdef SDLTTF
    SPDLOG_INFO(getLogger(),"TTF_Init");
    int ttf_res = TTF_Init();
	if (ttf_res == -1) {
		SPDLOG_ERROR(getLogger(), "Error init TTF {0}", TTF_GetError());
		return false;
	}
	//debugFont = TtfFont::createInstance("clacon.ttf", 20);
#endif
#ifdef SDL_MIXER
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		SPDLOG_ERROR(getLogger(), "ErrorMixed {0}", SDL_GetError());
		return false;
	}
#endif
    //Get devicedisplay mode
    SDL_DisplayMode displayMode;
    if( SDL_GetCurrentDisplayMode( 0, &displayMode ) == 0 ) {
        screenRect.w = displayMode.w;
        screenRect.h = displayMode.h;
    }
	// Override dimensions with incomming dimensions if set
	if (dimensions.x != UNDEFINED_VALUE) screenRect.w = dimensions.x;
	if (dimensions.y != UNDEFINED_VALUE) screenRect.h = dimensions.y;

	SPDLOG_INFO(getLogger(),"Creating window {0} {1}", screenRect.w, screenRect.h);
    //Create window
    sdlWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenRect.w, screenRect.h, SDL_WINDOW_SHOWN );
    if( sdlWindow == nullptr ) {
        SPDLOG_ERROR(getLogger(),"Window could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }
	SPDLOG_INFO(getLogger(),"SDL_CreateRenderer");

	//Create renderer for window
    sdlRenderer = SDL_CreateRenderer( sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if( sdlRenderer == nullptr ) {
        SPDLOG_ERROR(getLogger(),"Renderer could not be created! SDL Error: %s\n",SDL_GetError() );
        return false;
    }

    //Initialize renderer color
    SDL_SetRenderDrawColor( sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    return true;
}

void SDLWrapper::execute(std::list<std::unique_ptr<IDrawCommand>> &drawCommands) {
	SPDLOG_DEBUG(getLogger(),"execute enter");
	cursor = { 0,float(screenRect.h - 20) };
	//SDL_SetRenderDrawColor(getSdlRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_SetRenderDrawColor(getSdlRenderer(), backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	SDL_RenderClear(getSdlRenderer());
	SDL_SetRenderDrawColor(getSdlRenderer(), 0xFF, 0x00, 0x00, 0xFF);
	// Reverse iteration, so most depth draw are done first and overlayed by others
	for (std::list<std::unique_ptr<IDrawCommand>>::reverse_iterator drawCommand = drawCommands.rbegin(); drawCommand != drawCommands.rend(); ++drawCommand) {
		drawCommand->get()->execute(this);
	}
	SDL_RenderPresent(getSdlRenderer());
	SPDLOG_DEBUG(getLogger(),"execute exit");
}


int SDLWrapper::addNewEvents(std::list<std::unique_ptr<events::Event> > &events) {
	SDL_Event e;
	int n = 0;
	while (SDL_PollEvent(&e)) {
		n++;
		events.push_back(events::convert(e));
	}
	return n;
}

void SDLWrapper::finalize(){
    SDL_DestroyRenderer( sdlRenderer );
    SDL_DestroyWindow( sdlWindow );
	//if (debugFont) TTF_CloseFont(debugFont);
    sdlWindow = NULL;
    sdlRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}
void SDLWrapper::setDebugFont(std::shared_ptr<TtfFont> font) {
	debugFont = std::move(font);
}

void SDLWrapper::writeText(const std::string &msg, const Point2d &xy, const SDL_Color &color) const{
	if (!debugFont.get()) return;
	auto textSurface = TTF_RenderText_Solid(debugFont->getFont(), msg.c_str(), color);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(getSdlRenderer() , textSurface);
	SDL_Rect dest; 
	if (xy == UndefinedPoint2d) {
		dest.x = convertFloatToLong(cursor.x);
		dest.y = convertFloatToLong(cursor.y);
		cursor += cursorStep;
	}
	else {
		dest.x = convertFloatToLong(xy.x);
		dest.y = convertFloatToLong(xy.y);
	}
	dest.w = textSurface->w;
	dest.h = textSurface->h;

	SDL_RenderCopy(getSdlRenderer(), texture, NULL, &dest);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(textSurface);
}

