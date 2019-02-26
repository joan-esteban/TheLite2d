#pragma once
#include <memory>
#include "Texture.h"

namespace thelite2d {
	class Texture;

	class ITexturable {
	public:
		virtual ~ITexturable() = default;
		virtual const Texture &getTexture() const = 0;
		virtual void setTexture(Texture)  = 0;
	};
}