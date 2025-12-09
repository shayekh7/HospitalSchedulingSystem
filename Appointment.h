#pragma once
#include <string>

class Appointment {
    int id;
    int patientId;
    int doctorId;
    std::string dateTime;  // simple string: "YYYY-MM-DD HH:MM"
    std::string status;    // Pending / Confirmed / Cancelled

public:
    Appointment() = default;
    Appointment(int id,
                int patientId,
                int doctorId,
                const std::string &dateTime,
                const std::string &status = "Pending")
        : id(id), patientId(patientId), doctorId(doctorId),
          dateTime(dateTime), status(status) {}

    int getId() const { return id; }
    int getPatientId() const { return patientId; }
    int getDoctorId() const { return doctorId; }
    const std::string &getDateTime() const { return dateTime; }
    const std::string &getStatus() const { return status; }

    void setStatus(const std::string &s) { status = s; }

    std::string serialize() const;
    static Appointment fromLine(const std::string &line);
};
