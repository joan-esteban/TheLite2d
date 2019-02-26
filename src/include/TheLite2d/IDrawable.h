#pragma once
#include <list>
#include "IDrawCommand.h"
#include "types.h"

namespace thelite2d {

	class IDrawable {
	public:
		virtual ~IDrawable() = default;
		// TODO: Change std::shared_ptr<IDrawCommand> by std::unique_ptr<IDrawCommand>
		virtual bool getDrawCommands(const Present &present, std::list< std::unique_ptr<IDrawCommand> > &drawCommands) = 0;
		// SetParent is not a weak_ptr because there is no owner relationship, parent must setParent(nullptr) if it's destroyed
		
	};

	

} // namespace