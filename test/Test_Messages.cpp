#include <gtest/gtest.h>

#include "PE/Utils/Utils.h"
#include "PE/Utils/Listener.h"
#include "PE/Utils/ListenerBus.h"


using namespace PE;

struct FileData {
    uint32_t i;
};

enum class EventsEnum {
    FILE_CHANGED
};

struct TestStruct : public Utils::Listener<TestStruct, EventsEnum>
{
    void onMsg(EventsEnum msg, const std::any &);
};

void TestStruct::onMsg(const EventsEnum msg, const std::any & data) {
    Utils::log("File Changed!");

    auto i = std::any_cast<FileData>(data).i;
    EXPECT_EQ(i, 5);
}


TEST(testMessages, initTest) {

    TestStruct test;
    test.addMsg<EventsEnum::FILE_CHANGED>(FileData{5});

    test.dispatch();
}

struct TestStruct2
{
    void update(EventsEnum msg, const std::any &);

    static void staticUpdate(EventsEnum msg, const std::any & data);
};

void TestStruct2::update(EventsEnum msg, const std::any & data) {
    Utils::log("File Changed in MEMBER!");

    auto i = std::any_cast<FileData>(data).i;
    EXPECT_EQ(i, 5);
}

void TestStruct2::staticUpdate(EventsEnum msg, const std::any &data) {
    Utils::log("File Changed in STATIC!");

    auto i = std::any_cast<FileData>(data).i;
    EXPECT_EQ(i, 5);
}


TEST(testMessages, listenerBus) {

    Utils::ListenerBus<EventsEnum> bus;

    TestStruct2 testStruct;
    bus.addSubscriber(&TestStruct2::staticUpdate);
    //bus.addSubscriber<TestStruct2>(&TestStruct2::update, testStruct);

    bus.addMsg<EventsEnum::FILE_CHANGED>(FileData{5});
    bus.dispatch();

}


int main(int argc, char **argv) {
    Utils::Locator::provide(new Utils::ConsoleLogger());

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
