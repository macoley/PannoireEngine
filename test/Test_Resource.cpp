#include <gtest/gtest.h>

#include "PE/Utils/Utils.h"
#include "PE/Resource/Resource.h"

using namespace PE;

struct TestRes : public Resource::IResource {
    TestRes() {
        Utils::log("Created new");
    }

    virtual ~TestRes() {
        Utils::log("Removed");
    }

    void load(const std::string & path) override {
        Utils::log("Loaded: " + path);
    };
};

TEST(testResource, initTest) {
    auto res_manager = Resource::MakeManager();

    auto res = res_manager->load<TestRes>("somepath.jpg");
    EXPECT_EQ(1, res.getRefCount());
    EXPECT_EQ(1, res_manager->getSize<TestRes>());

    {
        auto res2 = res;

        EXPECT_EQ(2, res.getRefCount());
        EXPECT_EQ(1, res_manager->getSize<TestRes>());

        {
            auto res3 = res_manager->load<TestRes>("somepath.jpg");
            EXPECT_EQ(1, res_manager->getSize<TestRes>());

            auto res4 = res_manager->load<TestRes>("somepathother.jpg");
            EXPECT_EQ(2, res_manager->getSize<TestRes>());
        }
    }

    EXPECT_EQ(1, res_manager->getSize<TestRes>());
    EXPECT_EQ(1, res.getRefCount());
}


int main(int argc, char **argv) {
    Utils::Locator::provide(new Utils::ConsoleLogger());

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}