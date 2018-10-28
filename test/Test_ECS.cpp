#include <gtest/gtest.h>

#include "PE/ECS/ECS.h"

using namespace PE;

TEST(testECS, initTest) {

    auto manager = ECS::MakeECS();

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}