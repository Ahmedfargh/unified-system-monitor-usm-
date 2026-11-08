#include <collectors/ProcessCollector/CpuCollector.hpp>
#include <utils/StringUtils.hpp>
#include <utils/FileUtils.hpp>
#include <iostream>
#include <string>
#include <fstream>

using std::string;
using std::cout;
using std::cerr;
using std::ifstream;
using std::stoi;

namespace sysmon {

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
    string key;
    string value;
    while (std::getline(file, line)) {
        if (!utils::SplitKeyValue(line, key, value, ':')) {
            continue;
        }

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
    return utils::CanReadFile("/proc/cpuinfo");
}

} // namespace sysmon