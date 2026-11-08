#pragma once
#include <string>

namespace sysmon::utils {

// Checks if a file exists and has read permissions
bool CanReadFile(const std::string& filepath);

} // namespace sysmon::utils
