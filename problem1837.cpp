// http://acm.timus.ru/problem.aspx?space=1&num=1837

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <set>
#include <assert.h>

//#define DEBUG

#ifdef DEBUG
const char* test = "%s/test%d";
const char* result = "%s/result%d";
#endif

using Team = std::vector<std::string>;
using Teams = std::vector<Team>;

struct Member
{
    Member(const std::string& aName, int aIsenbaevNumber = 0)
        : mName(aName)
        , mIsenbaevNumber(aIsenbaevNumber)
    {
    }

    std::string mName;
    int mIsenbaevNumber;
};

struct CmpName
{
    bool operator () (const Member& left, const Member& right) const
    {
        return left.mName < right.mName;
    }
};

using Members = std::set<Member, CmpName>;
using Teammates = std::set<Member, CmpName>;

Team SplitTeam(const std::string& team, const char separator = ' ')
{
    size_t start = 0;
    size_t end;
    Team t;
    do
    {
        end = team.find(separator, start);
        size_t len = end - start;
        std::string name = team.substr(start, len);
        t.emplace_back(name);
        start += len + 1;
    }
    while (end != std::string::npos);
    return t;
}

void PrintMembers(const Members& members, std::ostream& stream)
{
#ifdef DEBUG
    std::cout << "Print members:" << std::endl;
    for (auto member: members)
        std::cout <<
            member.mName <<
                " " <<
                     (member.mIsenbaevNumber == -1 ?
                         "undefined" : std::to_string(member.mIsenbaevNumber)) << std::endl;
#endif
    for (auto member: members)
        stream <<
            member.mName <<
                " " <<
                     (member.mIsenbaevNumber == -1 ?
                         "undefined" : std::to_string(member.mIsenbaevNumber)) << std::endl;
}

void ProcessTeam(Members& aMembers, const std::string& aName, int aIsenbaevNumber, const Team& aTeam, Teammates& aTeamMates)
{
#ifdef DEBUG
    std::cout << aName << " with minimal rank found in teams" << std::endl;
#endif
    auto itMember = aMembers.find(Member{aName});
    if (itMember == aMembers.end())
    {
#ifdef DEBUG
        std::cout << "Adding " << aName << " to set with rank " << aIsenbaevNumber << std::endl;
#endif
        aTeamMates.insert(Member{aName, aIsenbaevNumber});
    }
    else
        aTeamMates.insert(Member{aName, itMember->mIsenbaevNumber});
    for (auto member : aTeam)
    {
        if (aName == member)
            continue;
#ifdef DEBUG
        std::cout << member << " found in same team, adding to teammates" << std::endl;
#endif

        auto itMember = aMembers.find(Member{member});
        if (itMember == aMembers.end())
        {
#ifdef DEBUG
            std::cout << "Adding " << member << " to set with rank " << aIsenbaevNumber + 1 << std::endl;
#endif
            aTeamMates.insert(Member{member, aIsenbaevNumber + 1});
        }
        else
            aTeamMates.insert(Member{member, itMember->mIsenbaevNumber});
    }
}

Teammates FindNameInTeams(Members& aMembers, Teams& aTeams, const std::string& aName, int aIsenbaevNumber)
{
    Teammates teamMates;

    for (auto itTeams = aTeams.begin(); itTeams != aTeams.end();)
    {
        auto itTeam = std::find(itTeams->begin(), itTeams->end(), aName);
        if (itTeam != itTeams->end())
        {
            ProcessTeam(aMembers, aName, aIsenbaevNumber, *itTeams, teamMates);

#ifdef DEBUG
            std::cout << "Deleting the whole team: [";
            for (auto m : *itTeams)
                std::cout << m << ",";
            std::cout << "]" << std::endl;
#endif
            itTeams = aTeams.erase(itTeams);
        }
        else
            ++itTeams;
    }

#ifdef DEBUG
    std::cout << "Teammates: [";
    for (auto m : teamMates)
        std::cout << m.mName << "(" << m.mIsenbaevNumber << "),";
    std::cout << "]" << std::endl;
#endif
    return teamMates;
}

void FindUndefinedAndAddThemToMembers(Members& aMembers, Teams& aTeams)
{
    for (auto team : aTeams)
    {
        for (auto member : team)
        {
#ifdef DEBUG
            std::cout << member << " undefined" << std::endl;
#endif
            auto itMember = aMembers.find(Member{member, 0});
            if (itMember == aMembers.end())
            {
#ifdef DEBUG
                std::cout << "Adding " << member << " with number -1" << std::endl;
#endif
                aMembers.insert(Member{member, -1});
            }
        }
    }
}

Teammates Process(Teammates& aTeamMates, Members& aMembers, Teams& aTeams)
{
    Teammates newTeamMates;
    for (auto teamMate: aTeamMates)
    {
        auto teamMates = FindNameInTeams(aMembers, aTeams, teamMate.mName, teamMate.mIsenbaevNumber);
        for (auto member: teamMates)
        {
            newTeamMates.insert(member);
        }
    }
    return newTeamMates;
}

void DumpTeamMates(Members& aMembers, const Teammates& aTeamMates)
{
    for (auto teamMate : aTeamMates)
    {
        auto itMember = aMembers.find(Member{teamMate});
        if (itMember == aMembers.end())
        {
#ifdef DEBUG
            std::cout << "Adding " << teamMate.mName << "(" << teamMate.mIsenbaevNumber << ") to set with rank " << teamMate.mIsenbaevNumber << std::endl;
#endif
            aMembers.insert(Member{teamMate.mName, teamMate.mIsenbaevNumber});
        }
    }
}

void DoJob(Members& aMembers, Teams& aTeams)
{
    Teammates teamMates = {Member{"Isenbaev", 0}};
    teamMates = Process(teamMates, aMembers, aTeams);
    if (!teamMates.empty())
    {
        while (!teamMates.empty())
        {
            DumpTeamMates(aMembers, teamMates);
            teamMates = Process(teamMates, aMembers, aTeams);
        }
        DumpTeamMates(aMembers, teamMates);
    }
    FindUndefinedAndAddThemToMembers(aMembers, aTeams);
}

#ifdef DEBUG
bool Run(const char* test, const char* result)
{
    Teams teams;
    {
        std::ifstream stream;
        stream.open(test);
        if (!stream.is_open())
            return false;
        std::string teamsCountStr;
        std::getline(stream, teamsCountStr);
        int teamsCount = atoi(teamsCountStr.c_str());
        for (int i = 0; i < teamsCount; ++i)
        {
            std::string team;
            std::getline(stream, team);
            Team t = SplitTeam(team);
            teams.push_back(t);
        }
    }
    Members members;

    DoJob(members, teams);

    {
        std::ifstream stream;
        stream.open(result);
        assert (stream.is_open());
        std::string resultComp;
        while (!stream.eof())
        {
            std::string resultCompStr;
            std::getline(stream, resultCompStr);
            resultComp += resultCompStr;
            resultComp += "\n";
        }
        resultComp.pop_back();

        std::stringstream str;
        PrintMembers(members, str);
        std::string comp{str.str()};
        if (comp.length() > 0 && comp.back() == '\n')
            comp.pop_back();
        assert(comp == resultComp);
    }
    return true;
}
#else
void Run()
{
    Teams teams;
    {
        std::string teamsCountStr;
        std::getline(std::cin, teamsCountStr);
        int teamsCount = atoi(teamsCountStr.c_str());
        for (int i = 0; i < teamsCount; ++i)
        {
            std::string team;
            std::getline(std::cin, team);
            Team t = SplitTeam(team);
            teams.push_back(t);
        }
    }
    Members members;

    DoJob(members, teams);

    PrintMembers(members, std::cout);
}
#endif

int main()
{
#ifdef DEBUG
    for (int i = 0; i < 100; ++i)
    {
        char testFileName[100] = {0};
        char resultFileName[100] = {0};
        sprintf(testFileName, test, "tests", i);
        sprintf(resultFileName, result, "tests", i);
        if (!Run(testFileName, resultFileName))
            break;
    }
#else
    Run();
#endif

    return 0;
}
