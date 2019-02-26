#pragma once

#include <string>
namespace thelite2d {
	class Object {
	public:
		Object()=default;
		virtual ~Object() = default;

		Object(std::string name) : objectName{ std::move(name) } {};
		
		virtual void setObjectName(const std::string &name) { objectName = name; };
		virtual std::string getObjectName() { return objectName; };
	private:
		std::string objectName;
	};

	
}