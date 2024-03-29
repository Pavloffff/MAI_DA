#include "ArgParser.h"

std::pair<bool, std::set<std::string>> Dialog::ArgParser::Parse(int argc, char **argv)
{
    bool check = true;
    std::set<std::string> argSet;
    try {
        for (int i = 1; i < argc; i++) {
            if (defaultFlags.find(argv[i]) == defaultFlags.end()) {
                if (strcmp(argv[i], "-") == 0) {
                    argSet.insert(argv[i]);
                    continue;
                }
                std::ifstream fileExists(argv[i]);
                if (!fileExists.good()) {
                    std::string msg = argv[0] + std::string(": invalid operation ");
                    msg += (std::string(argv[i]) + "\ntry " + std::string(argv[0]));
                    msg += " -h for more information";
                    if (std::string(argv[i]).substr(0, 1) == "-") {
                        throw Dialog::Exception(msg);
                    } else {
                        check = false;
                        std::cerr << argv[0] << ": [" << argv[i] << std::string("]: No such file or directory") << std::endl;
                        fileExists.close();
                        continue;
                    }
                }
                argSet.insert(argv[i]);
                fileExists.close();
            } else {
                argSet.insert(argv[i]);
            }
        }
        return {check, argSet};
    } catch (const Dialog::Exception &ex) {
        Dialog::Exception::LogException(ex);
        exit(-1);
    } 
}

std::set<std::string> Dialog::ArgParser::DefaultFlags()
{
    return defaultFlags;
}

Dialog::ArgParser::ArgParser()
{
}

Dialog::ArgParser::~ArgParser()
{
}
