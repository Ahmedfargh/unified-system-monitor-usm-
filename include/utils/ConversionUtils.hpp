#pragma once
#include <string>
#include <string_view>

namespace sysmon::utils {

// Converts Kilobytes (kB) to Gigabytes (GB)
double KbToGb(double kb);

// Parses a string containing a number and optional unit (e.g. "16151936 kB") to Gigabytes
double ParseKbStringToGb(std::string_view rawValue);

} // namespace sysmon::utils
