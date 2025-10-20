#ifndef SOLUTIONSLOGGER_H
#define SOLUTIONSLOGGER_H
#include <fstream>
#include <string>
#include <Solution.h>
#include <vector>

class SolutionsLogger
{
    public:
        SolutionsLogger(const std::string& path);
        virtual ~SolutionsLogger();
        void Log(std::vector<Solution*> solutions,const std::string& note);

    protected:

    private:

        std::ofstream _file;
};

#endif // SOLUTIONSLOGGER_H
