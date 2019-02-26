#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>

#include "TheLite2d/utils/cache.h"
using namespace testing;
using thelite2d::utils::CacheShared;
using std::string;

class CacheTest : public Test {

public:
	CacheShared<string, string> cacheStrings;
};

TEST_F(CacheTest, get_not_exists_item) {
	auto item = cacheStrings.get("hola");
	ASSERT_THAT(item.get(), Eq(nullptr));
}


TEST_F(CacheTest, insert_item) {
	auto str = std::make_shared<string>();
	cacheStrings.insert("key",str);
	auto item = cacheStrings.get("key");
	ASSERT_THAT(item, Eq(str));
}

TEST_F(CacheTest, get_dead_ptr_return_nullptr) {
	{
		auto str = std::make_shared<string>();
		cacheStrings.insert("key", str);
	}
	auto item = cacheStrings.get("key");
	ASSERT_THAT(item, Eq(nullptr));
}

TEST_F(CacheTest, no_viene_a) {
	std::weak_ptr<string> empty =  {};
	ASSERT_THAT(empty.expired(), Eq(true));
}