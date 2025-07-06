#include <iostream>
// #include <nlohmann/json.hpp>
#include "ConverterJSON.h"

int main()
{
    ConverterJSON convertJSON;
    std::vector<std::string> filesContent;
    std::vector<std::string> newRequests;
    try
    {
        filesContent = convertJSON.GetTextDocuments();
        for (auto str : filesContent)
            std::cout << str << "\n"
                      << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    try
    {
        std::cout << convertJSON.GetResponsesLimit();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    try
    {
        newRequests = convertJSON.GetRequests();
        for (auto oneRequest : newRequests)
            std::cout << oneRequest << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}