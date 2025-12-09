#include "Appointment.h"
#include <sstream>
#include <vector>

static std::vector<std::string> splitAppt(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::string Appointment::serialize() const {
    std::ostringstream oss;
    // id|patientId|doctorId|dateTime|status
    oss << id << '|' << patientId << '|' << doctorId << '|' << dateTime << '|' << status;
    return oss.str();
}

Appointment Appointment::fromLine(const std::string &line) {
    auto parts = splitAppt(line, '|');
    if (parts.size() < 5) {
        return Appointment{};
    }
    int id = std::stoi(parts[0]);
    int patientId = std::stoi(parts[1]);
    int doctorId = std::stoi(parts[2]);
    std::string dt = parts[3];
    std::string st = parts[4];
    return Appointment(id, patientId, doctorId, dt, st);
}
