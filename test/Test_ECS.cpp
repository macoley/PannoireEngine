#include <gtest/gtest.h>
#include <iostream>

#include "PE/ECS/ECS.h"
#include "PE/ECS/View.h"


using namespace PE;

struct TestComponent {
    int a, b, c;
};

struct TestComponent2 {
    int a;
};

TEST(testECS, initTest) {

    auto manager = ECS::MakeECS();
    auto entity = manager->createEntity();
    manager->assignComponent<TestComponent>(entity, 1, 2, 3);
    manager->assignComponent<TestComponent2>(entity, 1);
    auto& component = manager->getComponent<TestComponent>(entity);

    EXPECT_EQ(2, component.b);

    component.b = 3;

    EXPECT_EQ(3, component.b);

    auto view = manager->view<TestComponent>();
    auto view2 = manager->view<TestComponent, TestComponent2>();
}

int main(int argc, char **argv) {
    Utils::Locator::provide(new Utils::ConsoleLogger());

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}