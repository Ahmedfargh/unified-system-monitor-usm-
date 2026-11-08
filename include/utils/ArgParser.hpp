#pragma once
#include <string>
#include <vector>

namespace sysmon {

struct Config {
    bool showCpu = false;
    bool showMemory = false;
    bool showHelp = false;
    bool showVersion = false;
    bool hasError = false;
    std::string errorMessage;
};

class ArgParser {
public:
    static Config Parse(int argc, char* argv[]);
    static void PrintHelp(const char* programName);
    static void PrintVersion();
};

} // namespace sysmon
