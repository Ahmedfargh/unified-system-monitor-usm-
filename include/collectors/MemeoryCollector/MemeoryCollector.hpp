#pragma once
#include "../ICollector.hpp"
#include "MemoryData.hpp"

namespace sysmon {

class MemeoryCollector : public ICollector {
public:
    ~MemeoryCollector() override = default;
    void Collect() override;
    void GetSample() override;
    const MemeoryInfo& GetInfo() const; // Mark const
    bool HasPermissionTo() override;
    
private:
    MemeoryInfo info_; 

};
}
