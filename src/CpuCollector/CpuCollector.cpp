#include <collectors/ProcessCollector/CpuCollector.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <sstream>

using std::string;
using std::cout;
using std::cerr;
using std::ifstream;
using std::stoi;
namespace sysmon {

// Helper to remove leading and trailing whitespace / tabs
static string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void CpuCollector::Collect() {
    cout << "[CpuCollector] Collecting CPU info...\n";
    string file_ = "/proc/cpuinfo";
    ifstream file(file_);
    if (!file.is_open()) {
        cerr << "Error: CPU file could not be opened at /proc/cpuinfo\n";
        return;
    }

    info_.core_count = 0;

    string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        size_t colon_pos = line.find(':');
        if (colon_pos == string::npos) {
            continue;
        }

        string key = trim(line.substr(0, colon_pos));
        string value = trim(line.substr(colon_pos + 1));

        if (key == "processor") {
            info_.core_count++;
            try {
                info_.processor = stoi(value);
            } catch (...) {}
        } else if (key == "vendor_id" && info_.vendor_id.empty()) {
            info_.vendor_id = value;
        } else if (key == "cpu family" && info_.cpu_family == 0) {
            try {
                info_.cpu_family = stoi(value);
            } catch (...) {}
        } else if (key == "model" && info_.model == 0) {
            try {
                info_.model = stoi(value);
            } catch (...) {}
        } else if (key == "model name" && info_.model_name.empty()) {
            info_.model_name = value;
        }
    }
    file.close();
}

void CpuCollector::GetSample() {
    cout << "[CpuCollector] Sample: " << info_.model_name
         << " | Cores: " << info_.core_count << "\n";
}

const CpuInfo& CpuCollector::GetInfo() const {
    return info_;
}

bool CpuCollector::HasPermissionTo() {
    string process_path = "/proc/cpuinfo";
    if (access(process_path.c_str(), R_OK) == 0) {
        return true;
    }
    cerr << "Access denied: " << std::strerror(errno) << "\n";
    return false;
}

} // namespace sysmon