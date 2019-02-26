#pragma once
namespace thelite2d {
	namespace actions {
		template < typename TYPE_DATA >
		class ISetter {
		public:
			virtual TYPE_DATA getInitialValue(Object &objectToControl) = 0;
			virtual void setValue(Object &objectToControl, TYPE_DATA v) = 0;
		};
	}
}