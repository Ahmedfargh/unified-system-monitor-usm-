#pragma once
#include <vector>
#include <string>
using std::string;
namespace sysmon {

struct CpuInfo {
    int core_count = 0;
    std::vector<double> core_utils; 
    int processor = 0;
    string vendor_id;
    int cpu_family = 0;
    int model = 0;
    string model_name;
    
};

}