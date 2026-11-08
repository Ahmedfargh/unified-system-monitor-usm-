#include <collectors/MemeoryCollector/MemeoryCollector.hpp>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <fstream>

using std::string;
using std::cout;
using std::cerr;
using std::ifstream;
using std::map;

namespace sysmon {

static double get_kb_as_gb(const map<string, string>& buffer, const string& key) {
    auto it = buffer.find(key);
    if (it == buffer.end() || it->second.empty()) {
        return 0.0;
    }
    const string& value = it->second;
    size_t space = value.find_first_of(" \t");
    string number = (space == string::npos) ? value : value.substr(0, space);
    if (number.empty()) return 0.0;

    try {
        double kb = std::stod(number);
        return kb / (1024.0 * 1024.0);
    } catch (...) {
        return 0.0;
    }
}

void MemeoryCollector::Collect() {
    string mem_info_file = "/proc/meminfo";
    ifstream file_stream(mem_info_file);
    if (!file_stream.is_open()) {
        cerr << "Error: Memory info file could not be opened at /proc/meminfo\n";
        return;
    }

    map<string, string> buffer;
    string line;
    while (std::getline(file_stream, line)) {
        auto colon = line.find(':');
        if (colon == string::npos) {
            continue;
        }
        string key = line.substr(0, colon);
        size_t pos = colon + 1;
        while (pos < line.size() && (line[pos] == ' ' || line[pos] == '\t')) ++pos;
        string value = line.substr(pos);
        buffer[key] = value;
    }
    file_stream.close();

    // Core RAM
    info_.total_memory = get_kb_as_gb(buffer, "MemTotal");
    info_.free_memory = get_kb_as_gb(buffer, "MemFree");
    info_.mem_avaliable = get_kb_as_gb(buffer, "MemAvailable");
    info_.used_memory = (info_.total_memory > info_.mem_avaliable)
                            ? (info_.total_memory - info_.mem_avaliable)
                            : 0.0;
    info_.used_percentage = (info_.total_memory > 0.0)
                                ? (info_.used_memory / info_.total_memory) * 100.0
                                : 0.0;

    // Buffers and Caches
    info_.buffers = get_kb_as_gb(buffer, "Buffers");
    info_.cached = get_kb_as_gb(buffer, "Cached");
    info_.s_reclaimable = get_kb_as_gb(buffer, "SReclaimable");
    info_.shmem = get_kb_as_gb(buffer, "Shmem");

    // Swap Metrics
    info_.swap_total = get_kb_as_gb(buffer, "SwapTotal");
    info_.swap_free = get_kb_as_gb(buffer, "SwapFree");
    info_.swap_used = (info_.swap_total > info_.swap_free)
                          ? (info_.swap_total - info_.swap_free)
                          : 0.0;
    info_.swap_cached = get_kb_as_gb(buffer, "SwapCached");
    info_.swap_percentage = (info_.swap_total > 0.0)
                                ? (info_.swap_used / info_.swap_total) * 100.0
                                : 0.0;

    // Memory Pressure & Kernel
    info_.active = get_kb_as_gb(buffer, "Active");
    info_.inactive = get_kb_as_gb(buffer, "Inactive");
    info_.dirty = get_kb_as_gb(buffer, "Dirty");
    info_.writeback = get_kb_as_gb(buffer, "Writeback");
    info_.slab = get_kb_as_gb(buffer, "Slab");
    info_.page_tables = get_kb_as_gb(buffer, "PageTables");
    info_.committed_as = get_kb_as_gb(buffer, "Committed_AS");

    // GPU / Platform
    info_.gpu_active = get_kb_as_gb(buffer, "GPUActive");
    info_.gpu_reclame = get_kb_as_gb(buffer, "GPUReclaim");
}

void MemeoryCollector::GetSample() {
    cout << "[MemoryCollector] Sample: " << std::fixed << std::setprecision(2)
         << info_.used_memory << " / " << info_.total_memory << " GB Used ("
         << info_.used_percentage << "%)\n";
}

const MemeoryInfo& MemeoryCollector::GetInfo() const {
    return info_;
}

bool MemeoryCollector::HasPermissionTo() {
    string process_path = "/proc/meminfo";
    if (access(process_path.c_str(), R_OK) == 0) {
        return true;
    }
    cerr << "Access denied: " << std::strerror(errno) << "\n";
    return false;
}

} // namespace sysmon