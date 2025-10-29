#include "ConverterJSON.h"

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
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
        {
            std::getline(fConfig, currentContent);
            textDocuments.push_back(currentContent);
        }
        fConfig.close();
    }
    std::cout << configJSON["config"]["name"] << " starts working..." << std::endl;
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

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers)
{
    nlohmann::json jsonAnswers{{"answers", {}}};

    for (size_t i{}, size{answers.size()}; i < size; ++i)
    {
        std::string currentReauest{};
        std::stringstream ssBuf;

        ssBuf << "request" << std::setfill('0') << std::setw(3) << (i + 1);
        ssBuf >> currentReauest;

        if (answers[i].size() == 1)
        {
            jsonAnswers["answers"][currentReauest]["result"] = true;
            jsonAnswers["answers"][currentReauest]["docid"] = answers[i].at(0).doc_id;
            jsonAnswers["answers"][currentReauest]["rank"] = answers[i].at(0).rank;
        }
        else if (answers[i].size() > 1)
        {
            jsonAnswers["answers"][currentReauest]["result"] = true;

            for (size_t j{}, size{answers[i].size()}; j < size; j++)
            {
                jsonAnswers["answers"][currentReauest]["relevance"] += {{"docid", answers[i].at(j).doc_id},
                                                                        {"rank", answers[i].at(j).rank}};
            }
        }
        else
            jsonAnswers["answers"][currentReauest]["result"] = false;
    }

    std::ofstream ofAnswers("answers.json");

    if (!ofAnswers.is_open())
        throw std::runtime_error("Can't open or create output file");

    ofAnswers << jsonAnswers;

    ofAnswers.close();
}
