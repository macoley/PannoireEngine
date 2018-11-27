#include <gtest/gtest.h>

#include <iostream>
#include <bitset>
#include <chrono>

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
    auto entity1 = manager->createEntity();
    auto entity2 = manager->createEntity();
    auto entity3 = manager->createEntity();

    manager->assignComponent<TestComponent>(entity1, 1, 2, 3);
    manager->assignComponent<TestComponent>(entity2);
    manager->assignComponent<TestComponent>(entity3);

    manager->assignComponent<TestComponent2>(entity1);
    manager->assignComponent<TestComponent2>(entity3);

    auto& component = manager->getComponent<TestComponent>(entity1);

    EXPECT_EQ(2, component.b);

    component.b = 3;

    EXPECT_EQ(3, component.b);

    auto view = manager->view<TestComponent>();
    auto view2 = manager->view<TestComponent, TestComponent2>();

    view.each([](const ECS::Entity entity, TestComponent& comp) {

    });

    view2.each([](const ECS::Entity entity, TestComponent& comp1,  TestComponent2& comp2) {
        Utils::log("Entity: " + std::bitset<64>(entity).to_string());
        Utils::log("TestComponent: " + std::to_string(comp1.a));
        comp1.b = 666;
    });

    EXPECT_EQ(666, manager->getComponent<TestComponent>(entity3).b);

    manager->destroyEntity(entity1);
    manager->destroyEntity(entity2);
    manager->destroyEntity(entity3);

    //manager->getComponent<TestComponent>(entity2);
}


TEST(testECS, test2) {
    auto manager = ECS::MakeECS();
    auto entity1 = manager->createEntity();
    auto entity2 = manager->createEntity();
    auto entity3 = manager->createEntity();

    manager->assignComponent<TestComponent>(entity1);
    manager->assignComponent<TestComponent>(entity2);
    manager->assignComponent<TestComponent>(entity3);

    manager->destroyEntity(entity1);

    auto entity4 = manager->createEntity();

    Utils::log("Entity1: " + std::bitset<64>(entity1).to_string());
    Utils::log("Entity4: " + std::bitset<64>(entity4).to_string());

    manager->assignComponent<TestComponent>(entity4);
}


int main(int argc, char **argv) {
    Utils::Locator::provide(new Utils::ConsoleLogger());

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}