#include "User.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include <sstream>
#include <vector>

static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::unique_ptr<User> User::fromLine(const std::string &line) {
    auto parts = split(line, '|');
    if (parts.size() < 5) return nullptr;

    char typeChar = parts[0].empty() ? 'P' : parts[0][0];
    int id = std::stoi(parts[1]);
    std::string name = parts[2];
    std::string username = parts[3];
    std::string password = parts[4];

    switch (typeChar) {
        case 'P':
            return std::make_unique<Patient>(id, name, username, password);
        case 'D': {
            std::string specialization = parts.size() > 5 ? parts[5] : "";
            return std::make_unique<Doctor>(id, name, username, password, specialization);
        }
        case 'A':
            return std::make_unique<Admin>(id, name, username, password);
        default:
            return nullptr;
    }
}
