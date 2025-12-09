#include "Doctor.h"
#include <sstream>

std::string Doctor::serialize() const {
    std::ostringstream oss;
    // type|id|name|username|password|specialization
    oss << 'D' << '|' << id << '|' << name << '|' << username << '|' << password << '|' << specialization;
    return oss.str();
}
