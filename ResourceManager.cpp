#include <iostream>
#include <thread>

#include "ResourceManagerTool.h"

void Test_01()
{
    ResourceManagerTool TestTool;
    size_t CntA = TestTool.AddResource<int>();
    TestTool.AcquireResource<int>(CntA);
    TestTool.AcquireResource<int>(CntA);
    std::cout << CntA << " " << TestTool.GetUseCount() << std::endl;

    size_t CntB = TestTool.AddResource<int>();
    TestTool.AcquireResource<int>(CntB);
    std::cout << CntB << " " << TestTool.GetUseCount() << std::endl;

    TestTool.SetReleaseDelay(static_cast<std::chrono::milliseconds>(0));

    TestTool.ExecuGC();

    std::cout << TestTool.GetUseCount() << std::endl;

    TestTool.ReleaseResource(CntA);
    TestTool.ReleaseResource(CntB);

    TestTool.ExecuGC();

    std::cout << TestTool.GetUseCount() << std::endl;

    TestTool.ReleaseResource(CntA);

    TestTool.ExecuGC();

    std::cout << TestTool.GetUseCount() << std::endl;
}

void Test_02()
{
    ResourceManagerTool tool;
    // 测试1：添加资源并验证资源计数
    std::cout << "=== 测试1：添加资源 ===" << std::endl;
    size_t id1 = tool.AddResource<int>();
    std::cout << "添加资源 ID1: " << id1 << ", 当前资源数: " << tool.GetUseCount() << " (应为1)" << std::endl;

    size_t id2 = tool.AddResource<double>();
    std::cout << "添加资源 ID2: " << id2 << ", 当前资源数: " << tool.GetUseCount() << " (应为2)" << std::endl;

    // 测试2：获取资源并验证资源计数不变
    std::cout << "\n=== 测试2：获取资源 ===" << std::endl;
    auto ptr1 = tool.AcquireResource<int>(id1);
    auto ptr2 = tool.AcquireResource<double>(id2);
    std::cout << "成功获取资源。当前资源数: " << tool.GetUseCount() << " (仍为2)" << std::endl;

    // 测试3：释放资源并立即执行GC（延迟0毫秒）
    std::cout << "\n=== 测试3：释放资源并立即GC ===" << std::endl;
    tool.ReleaseResource(id1);
    tool.ReleaseResource(id2);
    tool.SetReleaseDelay(std::chrono::milliseconds(0));
    tool.ExecuGC();
    std::cout << "GC执行后资源数: " << tool.GetUseCount() << " (应为0)" << std::endl;

    // 测试4：验证延迟释放机制
    std::cout << "\n=== 测试4：延迟释放测试 ===" << std::endl;
    size_t id3 = tool.AddResource<std::string>();
    tool.AcquireResource<std::string>(id3);
    // 调整延迟为100毫秒并等待足够时间
    tool.SetReleaseDelay(std::chrono::milliseconds(100));
    tool.ReleaseResource(id3); // 重置释放时间
    std::this_thread::sleep_for(std::chrono::duration<double>(1.5)); // 等待超过延迟
    tool.ExecuGC();
    std::cout << "延迟100ms并等待后GC，资源数: " << tool.GetUseCount() << " (应为0)" << std::endl;

    // 测试5：多次获取同一资源并释放
    std::cout << "\n=== 测试5：多次获取与释放 ===" << std::endl;
    size_t id4 = tool.AddResource<int>();
    auto ptr4_1 = tool.AcquireResource<int>(id4);
    auto ptr4_2 = tool.AcquireResource<int>(id4); // UseCount增至2
    std::cout << "两次获取后资源数: " << tool.GetUseCount() << " (应为1)" << std::endl;

    tool.ReleaseResource(id4); // UseCount减至1
    tool.ReleaseResource(id4); // UseCount减至0
    tool.SetReleaseDelay(std::chrono::milliseconds(0));
    tool.ExecuGC();
    std::cout << "两次释放并GC后资源数: " << tool.GetUseCount() << " (应为0)" << std::endl;

    // 测试6：获取不存在的资源
    std::cout << "\n=== 测试6：获取无效资源 ===" << std::endl;
    size_t invalidId = 999;
    auto invalidPtr = tool.AcquireResource<int>(invalidId);
    std::cout << "获取无效ID结果: " << (invalidPtr ? "错误" : "正确返回空") << std::endl;
}

void Test_03()
{
    ResourceManagerTool TestTool;
    size_t CntA = TestTool.AddResource<std::string>("XXX");
    auto* x = TestTool.AcquireResource<std::string>(CntA);
    std::cout << *x << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
}

int main()
{
    Test_03();
    return 0;
}
