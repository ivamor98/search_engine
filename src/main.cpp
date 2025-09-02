#include <iostream>
#include <thread>
// #include <nlohmann/json.hpp>
#include "ConverterJSON.h"
#include "InvertedIndex.h"

int main()
{
    std::cout << "treads: " << std::thread::hardware_concurrency() << std::endl;
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

    // test invert
    InvertedIndex invertedIndex;
    invertedIndex.UpdateDocumentBase(filesContent);
    try
    {
        auto test = invertedIndex.GetWordCount("m");
        std::cout << "m count: " << test[0].count << " in doc_id: " << test[0].doc_id << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}