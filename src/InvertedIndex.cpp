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
        }
    }
    return vWords;
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
{
    // if (!docs.empty()) // нужна ли строка?
    docs.clear();
    docs = input_docs;
    freq_dictionary.clear();
    // test
    for (auto oneRequest : docs)
        std::cout << "____________\n"
                  << oneRequest << std::endl;
    //
    std::string tempWord;
    std::vector<Entry> tempVecEntry;
    Entry tempEntry;
    for (size_t current_doc = 0; current_doc < docs.size(); ++current_doc)
    {
        // std::vector<std::string> vWords = splitString(docs[current_doc]); для второго метода
        while (!docs[current_doc].empty())
        {
            for (size_t position = 0; position < docs[current_doc].size(); ++position)
            {
                if (isspace(docs[current_doc][position]) || ispunct(docs[current_doc][position]) || position == docs[current_doc].size() - 1)
                {
                    tempWord = docs[current_doc].substr(0, position);
                    if (position != 0)
                    {
                        auto it_freq_dictionary = freq_dictionary.find(tempWord);
                        if (it_freq_dictionary == freq_dictionary.end())
                        {
                            tempEntry.doc_id = current_doc;
                            tempEntry.count = 1;
                            tempVecEntry.clear();
                            tempVecEntry.push_back(tempEntry);
                            freq_dictionary.insert(std::make_pair(tempWord, tempVecEntry)); // что-то недобавляет
                        }
                        else
                        {
                            if (it_freq_dictionary->second.back().doc_id == current_doc)
                                ++it_freq_dictionary->second.back().count;
                            else
                                it_freq_dictionary->second.push_back({current_doc, 1});
                        }
                    }
                    docs[current_doc] = docs[current_doc].substr(position + 1);
                    break;
                }
            }
        }
    }
}
