// http://acm.timus.ru/problem.aspx?space=1&num=1196

#include <iostream>
#include <vector>
#include <algorithm>

using Vector = std::vector<int>;

int main()
{
    Vector professorDates;
    int professorDatesCount;
    std::cin >> professorDatesCount;
    for (int i = 0; i < professorDatesCount; ++i)
    {
       int number;
       std::cin >> number;
       professorDates.push_back(number);
    }
    Vector studentDates;
    int studentDatesCount;
    std::cin >> studentDatesCount;
    for (int i = 0; i < studentDatesCount; ++i)
    {
       int number;
       std::cin >> number;
       studentDates.push_back(number);
    }
    int dateCounter = 0;
    for (auto studentDate : studentDates)
      if (std::binary_search(professorDates.begin(), professorDates.end(), studentDate))
        ++dateCounter;
    std::cout << dateCounter << std::endl;
    return 0;
}
