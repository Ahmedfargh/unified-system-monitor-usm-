#include <iostream>
#include <memory>
#include <iomanip>
#include <collectors/ProcessCollector/CpuCollector.hpp>
#include <collectors/MemeoryCollector/MemeoryCollector.hpp>
#include <utils/ArgParser.hpp>

using std::cout;
using std::cerr;
using sysmon::ArgParser;
using sysmon::Config;
using sysmon::CpuCollector;
using sysmon::CpuInfo;
using sysmon::MemeoryCollector;
using sysmon::MemeoryInfo;

void DisplayCpu(CpuCollector& collector) {
    if (!collector.HasPermissionTo()) {
        cerr << "Warning: Permission check failed for /proc/cpuinfo.\n";
        return;
    }

    collector.Collect();
    collector.GetSample();

    const CpuInfo& info = collector.GetInfo();
    cout << "\n--- CPU Info (/proc/cpuinfo) ---\n";
    cout << "Model Name : " << info.model_name << "\n";
    cout << "Vendor ID  : " << info.vendor_id << "\n";
    cout << "CPU Family : " << info.cpu_family << "\n";
    cout << "Model      : " << info.model << "\n";
    cout << "Cores      : " << info.core_count << "\n";
}

void DisplayMemory(MemeoryCollector& collector) {
    if (!collector.HasPermissionTo()) {
        cerr << "Warning: Permission check failed for /proc/meminfo.\n";
        return;
    }

    collector.Collect();
    collector.GetSample();

    const MemeoryInfo& info = collector.GetInfo();
    cout << std::fixed << std::setprecision(2);
    cout << "\n--- Memory Info (/proc/meminfo) ---\n";
    cout << "Physical RAM:\n";
    cout << "  Total Memory     : " << info.total_memory << " GB\n";
    cout << "  Used Memory      : " << info.used_memory << " GB (" << info.used_percentage << "%)\n";
    cout << "  Available Memory : " << info.mem_avaliable << " GB\n";
    cout << "  Free Memory      : " << info.free_memory << " GB\n";

    cout << "\nBuffer & Cache:\n";
    cout << "  Buffers          : " << info.buffers << " GB\n";
    cout << "  Cached           : " << info.cached << " GB\n";
    cout << "  Slab Reclaimable : " << info.s_reclaimable << " GB\n";
    cout << "  Shared Memory    : " << info.shmem << " GB\n";

    cout << "\nSwap Memory:\n";
    cout << "  Swap Total       : " << info.swap_total << " GB\n";
    cout << "  Swap Used        : " << info.swap_used << " GB (" << info.swap_percentage << "%)\n";
    cout << "  Swap Free        : " << info.swap_free << " GB\n";
    cout << "  Swap Cached      : " << info.swap_cached << " GB\n";

    cout << "\nKernel & Memory Pressure:\n";
    cout << "  Active Memory    : " << info.active << " GB\n";
    cout << "  Inactive Memory  : " << info.inactive << " GB\n";
    cout << "  Dirty Memory     : " << info.dirty << " GB\n";
    cout << "  Writeback        : " << info.writeback << " GB\n";
    cout << "  Kernel Slab      : " << info.slab << " GB\n";
    cout << "  Page Tables      : " << info.page_tables << " GB\n";
    cout << "  Committed AS     : " << info.committed_as << " GB\n";
}

int main(int argc, char* argv[]) {
    Config config = ArgParser::Parse(argc, argv);

    if (config.hasError) {
        cerr << config.errorMessage << "\n";
        cerr << "Run '" << argv[0] << " --help' for usage instructions.\n";
        return 1;
    }

    if (config.showHelp) {
        ArgParser::PrintHelp(argv[0]);
        return 0;
    }

    if (config.showVersion) {
        ArgParser::PrintVersion();
        return 0;
    }

    cout << "=== System Monitor ===\n";

    if (config.showCpu) {
        CpuCollector cpuCollector;
        DisplayCpu(cpuCollector);
    }

    if (config.showMemory) {
        MemeoryCollector memCollector;
        DisplayMemory(memCollector);
    }

    return 0;
}