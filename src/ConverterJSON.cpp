#include "ConverterJSON.h"

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
    // test for debug
    std::ofstream testDebug;
    testDebug.open("testDebug.txt");

    std::ifstream fConfig;
    fConfig.open("config.json");
    if (!fConfig.is_open())
    {
        throw std::runtime_error("Cofig file is missing");
    }
    if (!nlohmann::json::accept(fConfig))
    {
        throw std::runtime_error("Config file has invalid JSON format");
    }
    fConfig.seekg(0);
    fConfig >> configJSON;
    fConfig.close();
    if (!configJSON.contains("config"))
        throw std::runtime_error("Config file is empty");
    std::string currentContent;

    for (auto it = configJSON["files"].begin(); it != configJSON["files"].end(); ++it)
    {
        fConfig.open((std::string)*it);

        if (!fConfig.is_open())
        {
            throw std::runtime_error("File " + (std::string)*it + " is missing");
        }

        while (!fConfig.eof())
            std::getline(fConfig, currentContent);
        textDocuments.push_back(currentContent);
        fConfig.close();
    }

    return textDocuments;
};

int ConverterJSON::GetResponsesLimit()
{
    if (configJSON.empty())
        throw std::runtime_error("Config file was not loaded. Use function GetTextDocuments first");
    if (!configJSON["config"].contains("max_responses"))
        throw std::runtime_error("Config file has no key \"max_responses\"");
    ResponsesLimit = configJSON["config"]["max_responses"];
    return ResponsesLimit;
};

std::vector<std::string> ConverterJSON::GetRequests()
{
    std::ifstream fReqursts("requests.json");
    if (!fReqursts.is_open())
        throw std::runtime_error("Requests file is missing");
    nlohmann::json requestsJSON;
    if (!requestsJSON.accept(fReqursts))
        throw std::runtime_error("Requests file has invalid JSON format");
    fReqursts.seekg(0);
    fReqursts >> requestsJSON;
    fReqursts.close();
    if (!requestsJSON.contains("requests"))
        throw std::runtime_error("Requests file has no key \"requests\"");
    for (auto it = requestsJSON["requests"].begin(); it != requestsJSON["requests"].end(); ++it)
    {
        requests.push_back(*it);
    }
    return requests;
};
