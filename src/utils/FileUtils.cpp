#include <utils/FileUtils.hpp>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>

namespace sysmon::utils {

bool CanReadFile(const std::string& filepath) {
    if (access(filepath.c_str(), R_OK) == 0) {
        return true;
    }
    std::cerr << "Access denied to " << filepath << ": " << std::strerror(errno) << "\n";
    return false;
}

} // namespace sysmon::utils
