#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "TheLite2d/utils/Files.h"

using namespace testing;
using namespace thelite2d;


TEST(Files_basename) {
	utils::Files files;
	ASSERT_THAT(files.basename(""), Eq(""));
	ASSERT_THAT(files.basename("D:/work/kk.txt"), Eq("D:/work/"));
	ASSERT_THAT(files.basename("D:\\work\\kk.txt"), Eq("D:\\work\\"));
	ASSERT_THAT(files.basename("./kk.txt"), Eq("./"));
	ASSERT_THAT(files.basename(".\\kk.txt"), Eq(".\\"));
	ASSERT_THAT(files.basename("./"), Eq("./"));

}

