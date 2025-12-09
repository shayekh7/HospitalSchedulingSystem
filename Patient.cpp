#include "Patient.h"
#include <sstream>

std::string Patient::serialize() const {
    std::ostringstream oss;
    // type|id|name|username|password|
    oss << 'P' << '|' << id << '|' << name << '|' << username << '|' << password << '|';
    return oss.str();
}
