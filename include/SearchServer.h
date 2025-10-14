#pragma once
#include "InvertedIndex.h"
#include <unordered_set>
#include <algorithm>

struct RelativeIndex
{
    size_t doc_id;
    float rank;
    bool operator==(const RelativeIndex &other) const
    {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};
class SearchServer
{
private:
    InvertedIndex _index;

public:
    /**
    * @param idx в конструктор класса передаётся ссылка на класс
    InvertedIndex,
    * чтобы SearchServer мог узнать частоту слов встречаемых в
    запросе
    */
    SearchServer(InvertedIndex &idx);
    /**
    * Метод обработки поисковых запросов
    * @param queries_input поисковые запросы взятые из файла
    requests.json
    * @return возвращает отсортированный список релевантных ответов для
    заданных запросов
    */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string> &queries_input, int responsesLimit);
};

// разбивает строку на список уникальных слов
std::unordered_set<std::string> getUniqSet(std::string str);