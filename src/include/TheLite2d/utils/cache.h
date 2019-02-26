#pragma once
#include <memory>
#include <map>
namespace thelite2d{
namespace utils{
	// This is just a cache to use already loaded elements, in future it can manage destruction of object
	template <class KEY, class OBJ>
	class CacheShared {
	public:
		void insert(const KEY &key, std::shared_ptr<OBJ> object) {
			cache[key] = object;
		}
		std::shared_ptr<OBJ> get(const KEY &key) const {
			auto i = cache.find(key);
			if (i == cache.end()) return {};
			const std::weak_ptr<OBJ> &ptr = i->second;
			if (ptr.lock()) {
				return std::shared_ptr<OBJ>{ ptr };
			} else return {};
		}

	private:
		std::map<KEY, std::weak_ptr<OBJ> > cache;
	};

	template <class KEY, class OBJ>
	class Cache {
	public:
		void insert(const KEY &key, OBJ object) {
			cache[key] = object;
		}
		bool exists(const KEY &key) const {
			auto i = cache.find(key);
			if (i == cache.end()) return false;
			return true;
		}
		OBJ get(const KEY &key) const {
			auto i = cache.find(key);
			if (i == cache.end()) return {};
			return i->second;
		}

	private:
		std::map<KEY, OBJ > cache;
	};

}
}