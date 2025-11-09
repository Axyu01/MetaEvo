#include "SolutionsLogger.h"
#include "Solution.h"
#include <iostream>

SolutionsLogger::SolutionsLogger(const std::string& path)
{
    _file.open(path, std::ios::out | std::ios::trunc);
    if (!_file.is_open()) {
        std::cerr << "Error: Could not open file " << path << " for writing.\n";
    } else {
        // CSV header
        _file << "Value;Genome;Note;\n";
    }
}

SolutionsLogger::~SolutionsLogger()
{
    if (_file.is_open())
        _file.close();
}
void SolutionsLogger::Log(Solution* s,const std::string& note)
{
    _file <<s->Value<<";<";
        for (int i = 0; i < s->Size-1; i++)
        {
            int city = s->Representation[i];
            _file << city << "-";
        }
        _file << s->Representation[s->Size-1] << ">;";
        _file << note <<";\n";
}
void SolutionsLogger::LogSummary(std::vector<Solution*> solutions,const std::string& note)
{
    int size = solutions.size();
    double sum=0;
    double best = solutions[0]->Value;
    double worst = solutions[size-1]->Value;

    if (!_file.is_open()) return;

    for (Solution* s : solutions)
    {
        double value = s->Value;
        sum +=value;
    }
    _file << "Avarage;" <<sum/size<<";\n";

    Log(solutions[0],note);
    Log(solutions[size-1],note);

}
void SolutionsLogger::Log(std::vector<Solution*> solutions, const std::string& note)
{
    if (!_file.is_open()) return;

    for (Solution* s : solutions)
    {
        Log(s,note);
    }
}
void SolutionsLogger::Log(const std::string& line)
{
    if (!_file.is_open()) return;

    _file << line << ";\n";
}
