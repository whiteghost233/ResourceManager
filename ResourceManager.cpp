#include <iostream>
#include <thread>

#include "ResourceManagerTool.h"

void Test_04()
{
    ResourceManagerTool TestTool;

    size_t Cnt_1 = TestTool.CreateResourceActor();

    size_t Cnt_2 = TestTool.CreateResourceActor();
    TestTool.ADDToRoot(Cnt_2);

    size_t Cnt_3 = TestTool.CreateResourceActor();
    TestTool.GetResourceActor(Cnt_3);

    Actor* Actor_2 = TestTool.GetResourceActor(Cnt_2);
    size_t Cnt_4 = TestTool.CreateResourceActor(Actor_2);
    TestTool.GetResourceActor(Cnt_4);

    size_t Cnt_5 = TestTool.CreateResourceActor();
    TestTool.GetResourceActor(Cnt_5);
    TestTool.ReleaseResource(Cnt_5);

    size_t Cnt_6 = TestTool.CreateResourceActor();
    TestTool.GetResourceActor(Cnt_6);
    TestTool.GetResourceActor(Cnt_6);
    TestTool.ReleaseResource(Cnt_6);

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

int main()
{
    Test_04();
    return 0;
}
