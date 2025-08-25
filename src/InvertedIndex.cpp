#include "InvertedIndex.h"

std::vector<std::string> splitString(std::string str)
{
    std::vector<std::string> vWords;
    std::string word;
    while (!str.empty())
    {
        for (size_t position = 0; position < str.size(); ++position)
        {
            if (isspace(str[position]) || ispunct(str[position]))
            {
                word = str.substr(0, position);
                if (position != 0)
                    vWords.push_back(word);
                str = str.substr(position + 1);
                break;
            }
            if (position + 1 == str.size())
            {
                word = str;
                vWords.push_back(word);
                str.clear();
                break;
            }
        }
    }
    return vWords;
}
void InvertedIndex::fillWords(int current_doc)
{
    std::string tempWord;
    std::vector<Entry> tempVecEntry;
    std::vector<std::string> vWords;
    Entry tempEntry;

    vWords = splitString(docs[current_doc]);
    for (auto tempWord : vWords)
    {
        acces_frec_dict.lock();
        auto it_freq_dictionary = freq_dictionary.find(tempWord);
        if (it_freq_dictionary == freq_dictionary.end()) // если слова нет в словаре, заводим новый элемент
        {
            tempEntry.doc_id = current_doc;
            tempEntry.count = 1;
            tempVecEntry.clear();
            tempVecEntry.push_back(tempEntry);
            freq_dictionary.insert(std::make_pair(tempWord, tempVecEntry));
        }
        else // если слово есть в словаре, увеличиваем счетчик слов, если в текущем документе такое слово уже были,
             // или добавляемновый элемент в конец ветора
        {

            bool find_doc = false;
            for (auto &it_vec_entry : it_freq_dictionary->second)
            {
                if (it_vec_entry.doc_id == current_doc)
                {

                    ++it_vec_entry.count;
                    find_doc = true;
                    break;
                }
            }
            if (!find_doc)
            {
                it_freq_dictionary->second.push_back({(size_t)current_doc, 1}); // проверить правильно ли приводится
            }
        }
        acces_frec_dict.unlock();
    }
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
{
    if (!docs.empty()) // нужна ли строка?
        docs.clear();
    docs = input_docs;
    freq_dictionary.clear();
    std::vector<std::thread *> threads;

    for (int current_doc = 0; current_doc < docs.size(); ++current_doc)
    {
        std::thread *newThread = new std::thread(&InvertedIndex::fillWords, this, current_doc);
        threads.push_back(newThread);
    }
    for (auto current_thead : threads)
    {
        current_thead->join();
        delete current_thead;
        current_thead = nullptr;
    }
}
