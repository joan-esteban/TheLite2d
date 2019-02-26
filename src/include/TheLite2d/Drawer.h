#pragma once

#include <list>
#include <cassert>
#include <mutex>

#include "Texture.h"
//#include "IDrawable.h"
#include "IDrawable.h"
#include "IController.h"
#include "IEventConsumer.h"
#include "IContainer.h"
#include "impl/Positionable.h"
#include "impl/Controllable.h"

// https://msdn.microsoft.com/en-us/library/6b3sy7ae.aspx
// c4250 : class1' : inherits 'class2::member' via dominance
#pragma warning( disable: 4250)
namespace thelite2d {
	class IMouseReactable;

	//class IDrawer : virtual public Object, IControllable, virtual public IPositionable, virtual public IDrawable, virtual public IEventConsumer, virtual public IContainer 
	class Drawer : public Object, public impl::Controlable, public impl::Positionable,public IInsertable, public IDrawable, public IEventConsumer, public IContainer {
	public:
		Drawer() : impl::Controlable{ this } {};
		Drawer(std::string name) : Object{ std::move(name) }, impl::Controlable{ this } {};
		virtual ~Drawer();
		// Heredado v�a IDrawable
		virtual bool getDrawCommands(const Present & present, std::list<std::unique_ptr<IDrawCommand>>& drawCommands) override;
		// Heredado vía IDrawer
		virtual void setClipping(bool v) { clipping = v; };
		virtual bool onEvent(const events::Event *event) override;
		void transformEvent(events::Event *event) const;

		template<class INTERFACE>
		void add(std::shared_ptr<INTERFACE> &drawable, ADD_ORDER order = ADD_ORDER::ON_BOTTOM) {
			auto realDrawable = std::dynamic_pointer_cast<IInsertable>(drawable);
			assert(realDrawable != nullptr);
			add(realDrawable, order);
			//drawables.push_back(realDrawable);
		}

		// 
		std::weak_ptr<IMouseReactable> getOnTopMouseReactableAtPosition(events::Event &event, bool eventOnLocalCoordinates) const;
		

		virtual void add(std::shared_ptr<IInsertable> &drawable, ADD_ORDER order) override;

		template<class INTERFACE>
		void remove(std::shared_ptr<INTERFACE> &drawable) {
			auto realDrawable = std::dynamic_pointer_cast<IInsertable>(drawable);
			assert(realDrawable != nullptr);
			remove(realDrawable);
		}

		virtual void remove(std::shared_ptr<IInsertable> &drawable) override;

		virtual void remove(IInsertable *insertable) override;

		virtual void clear() override;

		Dimensions calculateDimensionsPositionablesContained() const;

		void setParent(IContainer *newParent) override;
		
		IContainer *getParent() const override;

		template <class T  = IInsertable>
		std::list< std::shared_ptr<T> >getItems() const {
			std::list< std::shared_ptr<T> > result;
			for (auto item : items) {
				auto obj = std::dynamic_pointer_cast<T>(item);
				if (obj) result.push_back(obj);
			}
			return result;
		}

		std::list< std::shared_ptr<IInsertable> >getItems() { return items; };


		static std::shared_ptr<Drawer> createInstance(std::string name = "") {
			return std::shared_ptr<Drawer>(new Drawer{ std::move(name) });
		}

	protected:

		void removePending();
		// This expect mutex to be taken
		void _add(std::shared_ptr<IInsertable> &drawable, ADD_ORDER order);
		void _remove(IInsertable *obj);
		void addPending();

		mutable std::mutex mutexItems;
		std::list< std::shared_ptr<IInsertable> > items;
		/*
		Point2d position = { 0,0 };
		bool visibility = true;
		FloatingPointType alpha = constants::ALPHA_MAX;
		FloatingPointType angle = 0;
		PivotMode pivotMode = PivotMode::ON_MIDDLE;
		Point2d pivotPosition = { 0,0 };
	
		*/
		bool controllerFirstExecution = true;
		std::shared_ptr<IController > controller = nullptr;

		
		bool clipping = true;

		std::list< std::pair< std::shared_ptr<IInsertable>, ADD_ORDER > > pendingToAdd;
		std::list< IInsertable * > pendingToRemove;
		IContainer *parent = nullptr;
		
	};

}
