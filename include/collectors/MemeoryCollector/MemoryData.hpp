#pragma once
#include <string>

namespace sysmon {

struct MemeoryInfo {
    // Core RAM
    double total_memory = 0.0;     // MemTotal (GB)
    double free_memory = 0.0;      // MemFree (GB)
    double mem_avaliable = 0.0;    // MemAvailable (GB)
    double used_memory = 0.0;      // Calculated: total_memory - mem_avaliable (GB)
    double used_percentage = 0.0;  // (used_memory / total_memory) * 100.0

    // Buffers and Caches
    double buffers = 0.0;          // Buffers (GB)
    double cached = 0.0;           // Cached (GB)
    double s_reclaimable = 0.0;    // SReclaimable (GB) - Kernel slab cache that can be reclaimed
    double shmem = 0.0;            // Shmem (GB) - Shared memory (tmpfs, etc.)

    // Swap Metrics
    double swap_total = 0.0;       // SwapTotal (GB)
    double swap_free = 0.0;        // SwapFree (GB)
    double swap_used = 0.0;        // Calculated: swap_total - swap_free (GB)
    double swap_cached = 0.0;      // SwapCached (GB)
    double swap_percentage = 0.0;  // (swap_used / swap_total) * 100.0

    // Memory Pressure & Kernel Allocations
    double active = 0.0;           // Active (GB) - recently used memory
    double inactive = 0.0;         // Inactive (GB) - memory eligible for reclamation
    double dirty = 0.0;            // Dirty (GB) - waiting to be written to disk
    double writeback = 0.0;        // Writeback (GB) - actively being written to disk
    double slab = 0.0;             // Slab (GB) - in-kernel data structure cache
    double page_tables = 0.0;      // PageTables (GB) - dedicated to page tables
    double committed_as = 0.0;     // Committed_AS (GB) - total estimated workload memory

    // GPU / Platform metrics (if present)
    double gpu_active = 0.0;
    double gpu_reclame = 0.0;
};

using MemoryInfo = MemeoryInfo;

} // namespace sysmon