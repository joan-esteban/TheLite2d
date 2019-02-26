#pragma once
#include <memory>
#include "IDrawable.h"
#include "Object.h"
namespace thelite2d {
	class IContainer;

	class IInsertable {
	public:
		virtual ~IInsertable() = default;
		virtual void setParent(IContainer *) = 0;
		virtual IContainer *getParent() const = 0;
	};

	class IContainer  {
	public:
		enum class ADD_ORDER {
			ON_TOP,
			ON_BOTTOM
		};
		virtual ~IContainer() = default;
		virtual void add(std::shared_ptr<IInsertable> &drawable, ADD_ORDER order = ADD_ORDER::ON_BOTTOM) = 0;
		virtual void remove(std::shared_ptr<IInsertable> &drawable) = 0;
		virtual void remove(IInsertable *insertable) = 0;
		virtual void clear() = 0;

	};
}