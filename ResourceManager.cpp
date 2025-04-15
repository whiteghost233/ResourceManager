// ResourceManager.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "ResourceManagerTool.h"

int main()
{
    ResourceManagerTool RMTA;
    std::cout << RMTA.GetCount() << std::endl;

    ResourceManagerTool RMTB(10);
    std::cout << RMTB.GetCount() << std::endl;

    return 0;
}
