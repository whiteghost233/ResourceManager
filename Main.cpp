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

    //Starting a thread takes approximately 200 milliseconds, so the first GC will still GC out 1, 4 and 6
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //The main thread 'sleep_for' takes longer than starting the GC thread. Therefore, during the first GC,
    //'Get_3.reset()' is not executed, and '3' will not be handled by the first GC
    Get_3.reset();
    TestTool.DislinkResourceResource(Cnt_3);
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

int main()
{
    Test_01();
    return 0;
}
