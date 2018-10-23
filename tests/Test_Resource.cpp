#include "gtest/gtest.h"

#include "PE/ECS/ECS.h"
#include "PE/Resource/ResourceManager.h"
#include "PE/Render/Texture.h"
#include "PE/Render/Shader.h"
#include "PE/Component/Types.h"
#include "PE/Render/Context.h"

using namespace PE;

TEST(testResource, initTest) {
    auto res_manager = std::make_unique<Resource::ResourceManager>();
    EXPECT_EQ(1000, 1000);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}