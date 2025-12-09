#include "Admin.h"
#include <sstream>

std::string Admin::serialize() const {
    std::ostringstream oss;
    // type|id|name|username|password|
    oss << 'A' << '|' << id << '|' << name << '|' << username << '|' << password << '|';
    return oss.str();
}
