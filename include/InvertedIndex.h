#pragma once
#include <vector>
#include <iostream>
#include <exception>
#include <map>
#include <sstream> // check
#include <cctype>
#include <thread>
#include <mutex>

struct Entry
{
    size_t doc_id, count;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator==(const Entry &other) const
    {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

// разделяет строку на отбельные слова
std::vector<std::string> splitString(std::string &str);

class InvertedIndex
{
private:
    std::vector<std::string> docs;                             // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
    std::mutex acces_frec_dict;

    // метод добавляет слова строки в словарь по индексу в векторе docs
    void fillWords(int current_doc);

public:
    InvertedIndex() = default;
    /**
    * Обновить или заполнить базу документов, по которой будем совершать
    поиск
    * @param texts_input содержимое документов
    */
    void UpdateDocumentBase(std::vector<std::string> input_docs);
    /**
 Метод определяет количество вхождений слова word в загруженной базе
документов
* @param word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
    const std::vector<Entry> &GetWordCount(const std::string &word);

    // дружественная функция, разделяет строку на вектор отдельных слов
};