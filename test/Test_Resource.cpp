#include <gtest/gtest.h>

#include "PE/Utils/Utils.h"
#include "PE/Resource/Resource.h"

using namespace PE;

struct TestRes : public Resource::IResource<TestRes> {
    TestRes() {
        Utils::log("Created new TestRes");
    }

    virtual ~TestRes() {
        Utils::log("Removed TestRes");
    }

    void load(const std::string & path) {
        Utils::log("Loaded: " + path);
    };

    int test() {
        return 5;
    }
};


TEST(testResource, initTest) {
    auto res_manager = Resource::MakeManager();

    res_manager->registerResource<TestRes>();
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


struct Dep {
    int a;
};

struct TestRes2 : public Resource::IResource<TestRes2> {

    TestRes2() {
        Utils::log("Createt with empty constructor !!!!");
    }


    TestRes2(Dep dep) : m_dep(dep) {
        Utils::log("Created new with dep " + std::to_string(dep.a));
    }

    TestRes2(TestRes dep) : m_dep2(dep) {
        Utils::log("Created new with dep TestRes" + std::to_string(dep.test()));
    }

    virtual ~TestRes2() {
        Utils::log("Removed TestRes2");
    }

    void load(const std::string & path) {
        Utils::log("Loaded: " + path);
    };

    Dep m_dep;
    TestRes m_dep2;
};

TEST(testResource, initTest2) {
    auto res_manager = Resource::MakeManager();

    res_manager->registerResource<TestRes>();
    auto res = res_manager->load<TestRes>("somepath.jpg");

    //Dep dep{5};
    TestRes dep2;

    res_manager->registerResource<TestRes2>(dep2);
    auto res2 = res_manager->load<TestRes2>("somepath2.jpg");


}



int main(int argc, char **argv) {
    //Utils::Locator::provide(new Utils::ConsoleLogger());
    Utils::Locator::provide(new Utils::NullLogger());

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}