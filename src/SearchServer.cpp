#include "SearchServer.h"

std::vector<RelativeIndex>::iterator findInVecRelativeIndex(std::vector<RelativeIndex> &vecRelativeIndex, size_t inDoc_id)
{
    auto it = vecRelativeIndex.begin();
    for (it; it != vecRelativeIndex.end(); ++it)
    {
        if (inDoc_id == it->doc_id)
            return it;
    }
    return it;
}

std::unordered_set<std::string> getUniqSet(std::string str)
{
    std::unordered_set<std::string> uniqSet;
    std::string word;
    deletePunct(str);
    std::istringstream iss(str);
    while (iss >> word)
    {
        uniqSet.insert(word);
    }
    return uniqSet;
}
SearchServer::SearchServer(InvertedIndex &idx)
{
    _index(idx);
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input, int responsesLimit = 5)
{
    std::vector<std::vector<RelativeIndex>> result;
    std::unordered_set<std::string> uniqWordsQuery;
    RelativeIndex oneDocResult;
    std::vector<RelativeIndex> oneQueryResult;
    int absoluteRelevance = 0;
    float maxAbsoluteRelevance = 0;
    for (auto str : queries_input) // проходимся по списку запросов
    {
        uniqWordsQuery = getUniqSet(str);
        for (auto word : uniqWordsQuery) // проходимся по списку слов в запросе
        {
            auto WordVecEntry = _index.GetWordCount(word);
            for (auto wordEntry : WordVecEntry) // проходимся по документам, где есть это слово
            {
                if (oneQueryResult.empty())
                {
                    oneDocResult.doc_id = wordEntry.doc_id;
                    oneDocResult.rank = (float)wordEntry.count;
                    oneQueryResult.push_back(oneDocResult);
                }
                else
                {
                    auto tempIterator = findInVecRelativeIndex(oneQueryResult, wordEntry.doc_id);
                    if (tempIterator == oneQueryResult.end())
                    {
                        oneDocResult.doc_id = wordEntry.doc_id;
                        oneDocResult.rank = (float)wordEntry.count;
                        oneQueryResult.push_back(oneDocResult);
                    }
                    else
                    {
                        tempIterator->rank += wordEntry.count; // сравнить с maxAbsoluteRelevance
                        if (tempIterator->rank > maxAbsoluteRelevance)
                            maxAbsoluteRelevance = tempIterator->rank;
                    }
                }

                if (oneDocResult.rank > maxAbsoluteRelevance)
                    maxAbsoluteRelevance = oneDocResult.rank;
            }
        }
        // поделить на maxAbsoluteRelevance
        for (auto &it : oneQueryResult)
        {
            it.rank /= maxAbsoluteRelevance;
        }
        maxAbsoluteRelevance = 0;

        std::sort(oneQueryResult.begin(), oneQueryResult.end(), [](RelativeIndex &a, RelativeIndex &b)
                  { return a.rank > b.rank; });

        if (oneQueryResult.size() > responsesLimit) // responce limit
            oneQueryResult.resize(responsesLimit);
        result.push_back(oneQueryResult);

        oneQueryResult.clear();
    }

    return result;
}