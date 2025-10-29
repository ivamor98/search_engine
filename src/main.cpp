#include <iostream>
#include <thread>
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
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    try
    {
        newRequests = convertJSON.GetRequests();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    InvertedIndex invertedIndex;
    invertedIndex.UpdateDocumentBase(filesContent);

    SearchServer searchServer(invertedIndex);
    try
    {
        convertJSON.putAnswers(searchServer.search(newRequests, convertJSON.GetResponsesLimit()));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}