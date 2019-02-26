#include <sstream>
#include "TheLite2d/TilesCompound.h"

thelite2d::Dimensions thelite2d::TilesCompound::getDimensions() const {
	thelite2d::Dimensions dim = textures[0].getDimensions();
	thelite2d::Dimensions total = dim * thelite2d::Dimensions{ convertToFloat(tilesWidth), convertToFloat(tilesHeight) };
	return  total;
}

bool thelite2d::TilesCompound::getDrawCommands(const Present & present, std::list<std::unique_ptr<IDrawCommand>>& drawCommands)
{
	executeController(present, *this);
	if (!getVisible()) return false;
	Dimensions tileDimensions = textures[0].getDimensions();
	Dimensions total = getDimensions();
	if (thelite2d::config::debugMode) {
		std::stringstream ss;
		ss << " TILES (" << getPosition() << ")" << " " << tilesWidth << " x " << tilesHeight;
		drawCommands.push_back(std::unique_ptr<IDrawCommand>(new drawcmd::Text{ ss.str(), SDL_Color{ 64,64,0,255 } }));
		drawCommands.push_back(std::unique_ptr<IDrawCommand>(new drawcmd::Text{ getFullData(),ss.str(), SDL_Color{ 64,64,0,255 } }));

	}

	for (int i=0; i< tilesWidth; i++)
		for (int j = 0; j < tilesHeight; j++) {
			auto drawData = getFullData();
			Point2d pos{ i*tileDimensions.x, j*tileDimensions.y };
			drawData.calculatedPivotPosition.x -= (total.x * (float(i)/ tilesWidth) ) ;
			drawData.calculatedPivotPosition.y -= (total.y * (float(j) / tilesHeight));
			//drawData.calculatedPivotPosition.x = pos.x - drawData.calculatedPivotPosition.x;
			//drawData.calculatedPivotPosition.y = pos.y - drawData.calculatedPivotPosition.y;
			drawData.dimensions = tileDimensions;
			//auto drawCommand = std::dynamic_pointer_cast<IDrawCommand> (std::make_shared<drawcmd::Blit>(textures.at(0), drawData));
			drawCommands.push_back(std::unique_ptr<IDrawCommand>(new drawcmd::Blit{ textures.at(0), drawData }));
		}
	
	return true;
}



