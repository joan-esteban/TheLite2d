#pragma once
#include "IDrawable.h"
#include "IContainer.h"
#include "IEventConsumer.h"
namespace thelite2d {
	class KeyboardDebug : public Object, public IInsertable, public IDrawable, public IEventConsumer {
		// Heredado vía IInsertable
		virtual void setParent(IContainer *) override;
		virtual IContainer * getParent() const override;

		// Heredado vía IDrawable
		virtual bool getDrawCommands(const Present & present, std::list<std::unique_ptr<IDrawCommand>>& drawCommands) override;

		// Heredado vía IEventConsumer
		virtual bool onEvent(const events::Event * event) override;

	private:
		IContainer * parent = nullptr;
	};
}