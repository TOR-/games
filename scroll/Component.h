#pragma once

class Component
{
	public:
		// the lower the update order, the earlier the component updates
		Component(class Actor* owner, int updateOrder = 100);
		virtual ~Component();
		virtual void Update(float dt);
		int get_updateOrder() const { return _updateOrder; }
	protected:
		class Actor* _owner;
		int _updateOrder;
};
