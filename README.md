# System Monitor (`sysmon`)

A lightweight, modular Linux system monitoring tool written in modern C++ (C++17). `sysmon` reads directly from the Linux `/proc` pseudo-filesystem to extract real-time hardware and resource statistics with zero external dependencies.

---

## Features

- **Modular Architecture**: Built around a pure virtual `ICollector` interface for clean polymorphism and extensibility.
- **CPU Collector**: Parses `/proc/cpuinfo` to extract:
  - Processor model name and vendor ID
  - CPU family and model identifiers
  - Total logical core count
- **Memory Collector**: Parses `/proc/meminfo` into GB metrics:
  - **RAM**: Total, Used, Available, Free, and Usage Percentage
  - **Buffers & Cache**: Buffers, Page Cache, Reclaimable Slab, Shared Memory (tmpfs)
  - **Swap**: Total Swap, Used Swap, Free Swap, Swap Cache, and Swap Percentage
  - **Kernel & Memory Pressure**: Active/Inactive memory, Dirty, Writeback, Kernel Slab, Page Tables, and Committed Address Space
- **Permission Checking**: Built-in verification (`HasPermissionTo()`) to validate read access before attempting file I/O on kernel endpoints.
- **Command-Line Argument Management**: Built-in, type-safe CLI argument parser supporting both short and long options with help and version outputs.
- **Developer Ready**: Preconfigured for IDEs and language servers (`clangd`, VS Code) with compile commands and workspace settings.

---

## Directory Structure

```text
system-monitor/
├── include/
│   ├── collectors/
│   │   ├── ICollector.hpp                     # Pure virtual collector interface
│   │   ├── ProcessCollector/
│   │   │   ├── CpuCollector.hpp              # CPU collector declaration
│   │   │   └── CpuData.hpp                   # CpuInfo data structure
│   │   └── MemeoryCollector/
│   │       ├── MemeoryCollector.hpp          # Memory collector declaration
│   │       └── MemoryData.hpp                # MemeoryInfo data structure
│   └── utils/
│       └── ArgParser.hpp                     # CLI argument parser declaration
├── src/
│   ├── CpuCollector/
│   │   └── CpuCollector.cpp                  # CPU parsing implementation (/proc/cpuinfo)
│   ├── MemeoryCollector/
│   │   └── MemeoryCollector.cpp              # Memory parsing implementation (/proc/meminfo)
│   └── utils/
│       └── ArgParser.cpp                     # CLI parsing implementation
├── .clangd                                   # clangd language server configuration
├── compile_commands.json                     # Compilation database for IDEs
├── Makefile                                  # Build configuration
├── main.cpp                                  # Application entry point
└── README.md                                 # Documentation
```

---

## Prerequisites

- **OS**: Linux (kernel with `/proc` filesystem mounted)
- **Compiler**: GCC (`g++` 9+) or Clang with **C++17** support
- **Build Tool**: GNU Make

---

## Building the Project

Compile the binary with `make`:

```bash
make
```

To clean up object files and binary artifacts:

```bash
make clean
```

To perform a clean rebuild:

```bash
make clean && make
```

---

## Command-Line Usage

Run the compiled executable `./main` with any of the following options:

```bash
./main [OPTIONS]
```

### Available Options

| Short Option | Long Option | Description |
| :--- | :--- | :--- |
| `-a` | `--all` | Display all collectors (CPU & Memory) *(Default if no flags given)* |
| `-c` | `--cpu` | Display CPU information only |
| `-m` | `--mem`, `--memory` | Display Memory information only |
| `-v` | `--version` | Display version information and exit |
| `-h` | `--help` | Display usage instructions and exit |

---

### Examples

#### 1. Display All Metrics (Default)
```bash
./main
```
*Output:*
```text
=== System Monitor ===
[CpuCollector] Collecting CPU info...
[CpuCollector] Sample: Intel(R) Core(TM) i7-8850H CPU @ 2.60GHz | Cores: 12

--- CPU Info (/proc/cpuinfo) ---
Model Name : Intel(R) Core(TM) i7-8850H CPU @ 2.60GHz
Vendor ID  : GenuineIntel
CPU Family : 6
Model      : 158
Cores      : 12

--- Memory Info (/proc/meminfo) ---
Total Memory     : 15.40 GB
Available Memory : 6.13 GB
Free Memory      : 0.66 GB
Buffers          : 0.38 GB
Cached           : 6.61 GB
Swap Total       : 11.63 GB
Swap Free        : 11.62 GB
Swap Cached      : 0.00 GB
```

#### 2. Display CPU Metrics Only
```bash
./main --cpu
```

#### 3. Display Memory Metrics Only
```bash
./main -m
```

#### 4. Help & Usage Information
```bash
./main --help
```
*Output:*
```text
Usage: ./main [OPTIONS]

A modular Linux system monitor written in C++17.

Options:
  -a, --all         Display all metrics (CPU, Memory) [Default]
  -c, --cpu         Display CPU information (/proc/cpuinfo)
  -m, --mem, --memory
                    Display Memory information (/proc/meminfo)
  -v, --version     Display version information and exit
  -h, --help        Display this help message and exit
```

---

## Extending the Project

To add a new collector (e.g. `DiskCollector` or `NetworkCollector`):

1. **Define the data struct**: Create a header inside `include/collectors/<CollectorName>/<Name>Data.hpp`.
2. **Inherit from `ICollector`**: Implement the interface in `include/collectors/<CollectorName>/<Name>Collector.hpp`:
   ```cpp
   #pragma once
   #include "../ICollector.hpp"

   namespace sysmon {
       class MyCollector : public ICollector {
       public:
           ~MyCollector() override = default;
           void Collect() override;
           void GetSample() override;
           bool HasPermissionTo() override;
       };
   }
   ```
3. **Implement logic**: Add the `.cpp` file under `src/<CollectorName>/`. The `Makefile` automatically discovers and compiles all `.cpp` files in any subdirectory.
4. **Add a CLI flag**: Add the flag in [`include/utils/ArgParser.hpp`](include/utils/ArgParser.hpp) and [`src/utils/ArgParser.cpp`](src/utils/ArgParser.cpp).
# unified-system-monitor-usm-
