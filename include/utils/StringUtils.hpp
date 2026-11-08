#pragma once
#include <string>
#include <string_view>

namespace sysmon::utils {

// Removes leading and trailing whitespace/tabs/newlines from a string
std::string Trim(std::string_view str);

// Splits a line into key and value by delimiter (default ':')
// Returns true if delimiter was found and key is non-empty
bool SplitKeyValue(std::string_view line, std::string& key, std::string& value, char delimiter = ':');

} // namespace sysmon::utils
