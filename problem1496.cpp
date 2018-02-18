// http://acm.timus.ru/problem.aspx?space=1&num=1496

#include <iostream>
#include <set>

using Set = std::set<std::string>;

int main()
{
    int count;
    std::cin >> count;
    Set teams, spammers;
    for (int i = 0; i < count; ++i)
    {
       std::string team;
       std::cin >> team;
       auto result = teams.insert(team);
       if (!result.second)
          spammers.insert(team);
    }
    for (const auto& spammer : spammers)
        std::cout << spammer << std::endl;
    return 0;
}
