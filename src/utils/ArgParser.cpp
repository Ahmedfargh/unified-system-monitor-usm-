#include <utils/ArgParser.hpp>
#include <iostream>
#include <string_view>
#include <vector>

namespace sysmon {

Config ArgParser::Parse(int argc, char* argv[]) {
    Config config;

    // If no arguments provided, default to displaying all metrics
    if (argc <= 1) {
        config.showCpu = true;
        config.showMemory = true;
        return config;
    }

    std::vector<std::string_view> args(argv + 1, argv + argc);

    for (size_t i = 0; i < args.size(); ++i) {
        std::string_view arg = args[i];

        if (arg == "-h" || arg == "--help") {
            config.showHelp = true;
            return config; // Early exit on help
        } else if (arg == "-v" || arg == "--version") {
            config.showVersion = true;
            return config; // Early exit on version
        } else if (arg == "-c" || arg == "--cpu") {
            config.showCpu = true;
        } else if (arg == "-m" || arg == "--mem" || arg == "--memory") {
            config.showMemory = true;
        } else if (arg == "-a" || arg == "--all") {
            config.showCpu = true;
            config.showMemory = true;
        } else {
            config.hasError = true;
            config.errorMessage = "Unknown option: '" + std::string(arg) + "'";
            return config;
        }
    }

    // If flags were given but none selected any collector (e.g. invalid sequence)
    if (!config.showCpu && !config.showMemory) {
        config.showCpu = true;
        config.showMemory = true;
    }

    return config;
}

void ArgParser::PrintHelp(const char* programName) {
    std::cout << "Usage: " << programName << " [OPTIONS]\n\n"
              << "A modular Linux system monitor written in C++17.\n\n"
              << "Options:\n"
              << "  -a, --all         Display all metrics (CPU, Memory) [Default]\n"
              << "  -c, --cpu         Display CPU information (/proc/cpuinfo)\n"
              << "  -m, --mem, --memory\n"
              << "                    Display Memory information (/proc/meminfo)\n"
              << "  -v, --version     Display version information and exit\n"
              << "  -h, --help        Display this help message and exit\n\n"
              << "Examples:\n"
              << "  " << programName << "             # Run all collectors\n"
              << "  " << programName << " --cpu       # Display CPU metrics only\n"
              << "  " << programName << " -m          # Display Memory metrics only\n"
              << "  " << programName << " -c -m       # Display both CPU and Memory\n";
}

void ArgParser::PrintVersion() {
    std::cout << "sysmon (System Monitor) version 0.1.0\n"
              << "Built with C++17 for Linux systems.\n";
}

} // namespace sysmon
