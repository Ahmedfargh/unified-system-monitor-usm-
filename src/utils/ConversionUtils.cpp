#include <utils/ConversionUtils.hpp>
#include <string>

namespace sysmon::utils {

double KbToGb(double kb) {
    if (kb <= 0.0) return 0.0;
    return kb / (1024.0 * 1024.0);
}

double ParseKbStringToGb(std::string_view rawValue) {
    if (rawValue.empty()) return 0.0;

    size_t space = rawValue.find_first_of(" \t");
    std::string_view numberStr = (space == std::string_view::npos)
                                     ? rawValue
                                     : rawValue.substr(0, space);

    if (numberStr.empty()) return 0.0;

    try {
        double kb = std::stod(std::string(numberStr));
        return KbToGb(kb);
    } catch (...) {
        return 0.0;
    }
}

} // namespace sysmon::utils
