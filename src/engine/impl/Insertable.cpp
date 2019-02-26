#include "TheLite2d/impl/Insertable.h"
using namespace thelite2d;
using namespace thelite2d::impl;
void thelite2d::impl::Insertable::setParent(IContainer *newParent)
{
	parent = newParent;
}

IContainer * thelite2d::impl::Insertable::getParent() const
{
	return parent;
}
