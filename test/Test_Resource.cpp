#include <gtest/gtest.h>

#include "PE/Resource/ResourceManager.h"
#include "PE/Render/Texture.h"
#include "PE/Render/Shader.h"
#include "PE/Engine/Properties.h"

using namespace PE;

TEST(testResource, initTest) {
    auto res_manager = std::make_unique<Resource::ResourceManager>();

    EXPECT_EQ(0, res_manager->getResourceAmount<Render::Texture>());

    {
        auto texture = res_manager->load<Render::Texture>("container.jpg");
        EXPECT_EQ(1, texture.getRefCount());
        EXPECT_EQ(1, res_manager->getResourceAmount<Render::Texture>());
        {
            auto texture2 = texture;
            EXPECT_EQ(2, texture2.getRefCount());
            EXPECT_EQ(2, texture.getRefCount());

            EXPECT_EQ(1, res_manager->getResourceAmount<Render::Texture>());
        }
        EXPECT_EQ(1, texture.getRefCount());

        EXPECT_EQ(1, res_manager->getResourceAmount<Render::Texture>());
    }

    EXPECT_EQ(0, res_manager->getResourceAmount<Render::Texture>());

}

TEST(testResource, propertiesTest) {
    auto res_manager = std::make_unique<Resource::ResourceManager>();

    auto config = res_manager->load<Engine::Properites>();
    config.get().set("width", 450);
    config.get().set("height", 100);
    config.get().save("config.yml");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}