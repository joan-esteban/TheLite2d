#pragma once
#include "TheLite2d/IContainer.h"
namespace thelite2d {
	namespace impl {

		class Insertable : public IInsertable {
			// Heredado vía IInsertable
			virtual void setParent(IContainer *) override;
			virtual IContainer * getParent() const override;
		private:
			IContainer * parent = nullptr;
		};
	}
}