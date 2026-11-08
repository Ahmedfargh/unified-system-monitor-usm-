#include <collectors/MemeoryCollector/MemeoryCollector.hpp>
#include <utils/StringUtils.hpp>
#include <utils/ConversionUtils.hpp>
#include <utils/FileUtils.hpp>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <fstream>

using std::string;
using std::cout;
using std::cerr;
using std::ifstream;
using std::map;

namespace sysmon {

static double get_entry_as_gb(const map<string, string>& buffer, const string& key) {
    auto it = buffer.find(key);
    if (it == buffer.end()) {
        return 0.0;
    }
    return utils::ParseKbStringToGb(it->second);
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
    string key;
    string value;
    while (std::getline(file_stream, line)) {
        if (utils::SplitKeyValue(line, key, value, ':')) {
            buffer[key] = value;
        }
    }
    file_stream.close();

    // Core RAM
    info_.total_memory = get_entry_as_gb(buffer, "MemTotal");
    info_.free_memory = get_entry_as_gb(buffer, "MemFree");
    info_.mem_avaliable = get_entry_as_gb(buffer, "MemAvailable");
    info_.used_memory = (info_.total_memory > info_.mem_avaliable)
                            ? (info_.total_memory - info_.mem_avaliable)
                            : 0.0;
    info_.used_percentage = (info_.total_memory > 0.0)
                                ? (info_.used_memory / info_.total_memory) * 100.0
                                : 0.0;

    // Buffers and Caches
    info_.buffers = get_entry_as_gb(buffer, "Buffers");
    info_.cached = get_entry_as_gb(buffer, "Cached");
    info_.s_reclaimable = get_entry_as_gb(buffer, "SReclaimable");
    info_.shmem = get_entry_as_gb(buffer, "Shmem");

    // Swap Metrics
    info_.swap_total = get_entry_as_gb(buffer, "SwapTotal");
    info_.swap_free = get_entry_as_gb(buffer, "SwapFree");
    info_.swap_used = (info_.swap_total > info_.swap_free)
                          ? (info_.swap_total - info_.swap_free)
                          : 0.0;
    info_.swap_cached = get_entry_as_gb(buffer, "SwapCached");
    info_.swap_percentage = (info_.swap_total > 0.0)
                                ? (info_.swap_used / info_.swap_total) * 100.0
                                : 0.0;

    // Memory Pressure & Kernel
    info_.active = get_entry_as_gb(buffer, "Active");
    info_.inactive = get_entry_as_gb(buffer, "Inactive");
    info_.dirty = get_entry_as_gb(buffer, "Dirty");
    info_.writeback = get_entry_as_gb(buffer, "Writeback");
    info_.slab = get_entry_as_gb(buffer, "Slab");
    info_.page_tables = get_entry_as_gb(buffer, "PageTables");
    info_.committed_as = get_entry_as_gb(buffer, "Committed_AS");

    // GPU / Platform
    info_.gpu_active = get_entry_as_gb(buffer, "GPUActive");
    info_.gpu_reclame = get_entry_as_gb(buffer, "GPUReclaim");
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
    return utils::CanReadFile("/proc/meminfo");
}

} // namespace sysmon