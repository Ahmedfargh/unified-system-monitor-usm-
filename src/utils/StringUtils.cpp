#include <utils/StringUtils.hpp>

namespace sysmon::utils {

std::string Trim(std::string_view str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string_view::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return std::string(str.substr(first, (last - first + 1)));
}

bool SplitKeyValue(std::string_view line, std::string& key, std::string& value, char delimiter) {
    size_t colon_pos = line.find(delimiter);
    if (colon_pos == std::string_view::npos) {
        return false;
    }

    key = Trim(line.substr(0, colon_pos));
    value = Trim(line.substr(colon_pos + 1));
    return !key.empty();
}

} // namespace sysmon::utils
