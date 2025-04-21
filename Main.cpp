#include <iostream>
#include <thread>

#include "ResourceManager.h"

void Test_01()
{
    ResourceManager TestTool;

    size_t Cnt_1 = TestTool.CreateResourceResource();

    size_t Cnt_2 = TestTool.CreateResourceResource();
    TestTool.ADDToRoot(Cnt_2);

    size_t Cnt_3 = TestTool.CreateResourceResource();
    std::shared_ptr<Resource> Get_3 = TestTool.GetResourceResourceWithLink(Cnt_3);

    size_t Cnt_4 = TestTool.CreateResourceResource();
    TestTool.GetResourceResourceWithLink(Cnt_4);

    std::shared_ptr<Resource> Resource_2 = TestTool.GetResourceResourceWithLink(Cnt_2);
    size_t Cnt_5 = TestTool.CreateResourceResource(Resource_2);

    size_t Cnt_6 = TestTool.CreateResourceResource();
    TestTool.ADDToRoot(Cnt_6);
    TestTool.RemoveFromRoot(Cnt_6);

    size_t Cnt_7 = TestTool.CreateResourceResource();
    std::shared_ptr<Resource> Get_7 = TestTool.GetResourceResourceWithLink(Cnt_7);

    std::this_thread::sleep_for(std::chrono::milliseconds(1200));

    Get_3.reset();
    TestTool.DislinkResourceResource(Cnt_3);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

int main()
{
    Test_01();
    return 0;
}
