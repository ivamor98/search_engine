#include "InvertedIndex.h"

void deletePunct(std::string &str)
{
    for (auto it = str.begin(); it != str.end(); ++it) // удаляет знаки препинания. меняет исходную строку
    {
        if (ispunct(*it))
            *it = ' ';
    }
}

std::vector<std::string> splitString(std::string &str)
{
    std::vector<std::string> vWords;
    std::string word;
    deletePunct(str);
    std::istringstream iss(str);
    while (iss >> word)
    {
        vWords.push_back(word);
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
    docs = input_docs;
    freq_dictionary.clear();
    std::vector<std::thread> threads;

    for (int current_doc = 0; current_doc < docs.size(); ++current_doc)
    {
        threads.emplace_back(&InvertedIndex::fillWords, this, current_doc);
    }
    for (auto &current_thead : threads)
    {
        current_thead.join();
    }
}

const std::vector<Entry> &InvertedIndex::GetWordCount(const std::string &word)
{
    auto it_freq_dictionary = freq_dictionary.find(word);
    if (it_freq_dictionary == freq_dictionary.end())
    {
        static std::vector<Entry> emptyVec = {};
        return emptyVec;
        // return std::vector<Entry>{};  // так не проходит тест TestInvertedIndexMissingWord
    }
    return it_freq_dictionary->second;
}

InvertedIndex &InvertedIndex::operator=(InvertedIndex &other)
{
    if (&other == this)
        return *this;
    docs = other.docs;
    freq_dictionary = other.freq_dictionary;
    return *this;
}

InvertedIndex &InvertedIndex::operator()(InvertedIndex &other)
{
    if (&other == this)
        return *this;
    docs = other.docs;
    freq_dictionary = other.freq_dictionary;
    return *this;
}
