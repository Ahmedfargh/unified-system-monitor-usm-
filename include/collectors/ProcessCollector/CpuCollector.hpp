#pragma once
#include "../ICollector.hpp"
#include "CpuData.hpp"

namespace sysmon {

class CpuCollector : public ICollector {
public:
    ~CpuCollector() override = default;
    void Collect() override;
    void GetSample() override;
    const CpuInfo& GetInfo() const; // Mark const
    bool HasPermissionTo() override;
    
private:
    CpuInfo info_; 

};
}
