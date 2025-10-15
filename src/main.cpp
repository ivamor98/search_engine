#include <iostream>
#include <thread>
// #include <nlohmann/json.hpp>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

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

    // test invert
    InvertedIndex invertedIndex;
    invertedIndex.UpdateDocumentBase(filesContent);
    try
    {
        auto test = invertedIndex.GetWordCount("m");
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // test Seatch Server
    SearchServer searchServer(invertedIndex);
    // std::unordered_set<std::string> testUniqSet = getUniqSet("  one two thee4 one ,,,566");
    auto it = searchServer.search(convertJSON.GetRequests(), convertJSON.GetResponsesLimit());

    return 0;
}