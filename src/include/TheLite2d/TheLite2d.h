#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <chrono>
#include <memory>

#include "TheLite2d/log.h"
#include "TheLite2d/SDLWrapper.h"
#include "TheLite2d/SDLEvents.h"
#include "TheLite2d/Texture.h"
#include "TheLite2d/TextureTiles.h"
#include "TheLite2d/TextureTtf.h"
#include "TheLite2d/Sprite.h"
#include "TheLite2d/Button.h"
#include "TheLite2d/Drawer.h"
#include "TheLite2d/types.h"
#include "TheLite2d/timer.h"
#include "TheLite2d/Drawer.h"
#include "TheLite2d/TilesCompound.h"
#include "TheLite2d/actions/SerieGroup.h"
#include "TheLite2d/actions/Blink.h"
#include "TheLite2d/actions/ParallelGroup.h"
#include "TheLite2d/actions/Move.h"
#include "TheLite2d/actions/Fade.h"
#include "TheLite2d/actions/Rotate.h"
#include "TheLite2d/actions/Zoom.h"
#include "TheLite2d/actions/interpolater.h"
#include "TheLite2d/actions/Execute.h"
#include "TheLite2d/actions/Sequencer.h"
#include "TheLite2d/actions/FrameRate.h"

#include "TheLite2d/utils/FindAssets.h"
#include "TheLite2d/utils/Files.h"

#include "TheLite2d/Runner.h"