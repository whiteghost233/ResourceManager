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
    std::shared_ptr<Actor> Get_3 = TestTool.GetResourceActorWithLink(Cnt_3);

    size_t Cnt_4 = TestTool.CreateResourceActor();
    TestTool.GetResourceActorWithLink(Cnt_4);

    std::shared_ptr<Actor> Actor_2 = TestTool.GetResourceActorWithLink(Cnt_2);
    size_t Cnt_5 = TestTool.CreateResourceActor(Actor_2);

    size_t Cnt_6 = TestTool.CreateResourceActor();
    TestTool.ADDToRoot(Cnt_6);
    TestTool.RemoveFromRoot(Cnt_6);

    size_t Cnt_7 = TestTool.CreateResourceActor();
    std::shared_ptr<Actor> Get_7 = TestTool.GetResourceActorWithLink(Cnt_7);

    std::this_thread::sleep_for(std::chrono::milliseconds(1200));

    Get_3.reset();
    TestTool.DislinkResourceActor(Cnt_3);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

int main()
{
    Test_04();
    return 0;
}
