//http://acm.timus.ru/problem.aspx?space=1&num=1545

#include <iostream>
#include <map>

using Dictionary = std::multimap<char, char>;

int main()
{
    // entering count of words in dictionary
    int count;
    std::cin >> count;
    Dictionary dictionary;
    for (int i = 0; i < count; ++i)
    {
       char firstLetter;
       std::cin >> firstLetter;
       char secondLetter;
       std::cin >> secondLetter;
       dictionary.emplace(firstLetter, secondLetter);
    }
    char start;
    std::cin >> start;
    auto range = dictionary.equal_range(start);
    for (Dictionary::iterator it = range.first; it != range.second; ++it)    
      std::cout << start << it->second << std::endl;
    return 0;
}
