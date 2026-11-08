#pragma once

namespace sysmon {
    class ICollector {
    public:
        virtual ~ICollector() = default;
        virtual void Collect() = 0;
        virtual void GetSample() = 0;
        virtual bool HasPermissionTo() = 0;
    };
}