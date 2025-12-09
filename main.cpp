#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>

#include "User.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Exceptions.h"

const std::string USERS_FILE = "data/users.txt";
const std::string APPTS_FILE = "data/appointments.txt";

void pause() {
    std::cout << "Press ENTER to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void loadUsers(std::vector<std::unique_ptr<User>> &users) {
    std::ifstream in(USERS_FILE);
    if (!in.is_open()) {
        // No users file yet, create a default admin later
        return;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        auto user = User::fromLine(line);
        if (user) {
            users.push_back(std::move(user));
        }
    }
}

void saveUsers(const std::vector<std::unique_ptr<User>> &users) {
    std::ofstream out(USERS_FILE);
    if (!out.is_open()) {
        throw FileException("Cannot open " + USERS_FILE + " for writing");
    }
    for (const auto &u : users) {
        out << u->serialize() << "\n";
    }
}

void loadAppointments(std::vector<Appointment> &appts) {
    std::ifstream in(APPTS_FILE);
    if (!in.is_open()) {
        return;
    }
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        appts.push_back(Appointment::fromLine(line));
    }
}

void saveAppointments(const std::vector<Appointment> &appts) {
    std::ofstream out(APPTS_FILE);
    if (!out.is_open()) {
        throw FileException("Cannot open " + APPTS_FILE + " for writing");
    }
    for (const auto &a : appts) {
        out << a.serialize() << "\n";
    }
}

int nextUserId(const std::vector<std::unique_ptr<User>> &users) {
    int maxId = 0;
    for (const auto &u : users) {
        if (u->getId() > maxId) maxId = u->getId();
    }
    return maxId + 1;
}

int nextAppointmentId(const std::vector<Appointment> &appts) {
    int maxId = 0;
    for (const auto &a : appts) {
        if (a.getId() > maxId) maxId = a.getId();
    }
    return maxId + 1;
}

User* findUserByUsername(const std::vector<std::unique_ptr<User>> &users,
                         const std::string &username) {
    for (const auto &u : users) {
        if (u->getUsername() == username) {
            return u.get();
        }
    }
    return nullptr;
}

User* login(std::vector<std::unique_ptr<User>> &users) {
    std::string username, password;
    std::cout << "\n=== Login ===\n";
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    User *u = findUserByUsername(users, username);
    if (!u || !u->checkPassword(password)) {
        throw AuthException("Invalid username or password");
    }
    std::cout << "Login successful. Welcome, " << u->getName() << "!\n";
    return u;
}

void registerPatient(std::vector<std::unique_ptr<User>> &users) {
    std::string name, username, password;
    std::cout << "\n=== Patient Registration ===\n";
    std::cout << "Full name: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "Username: ";
    std::cin >> username;
    if (findUserByUsername(users, username)) {
        std::cout << "Username already exists.\n";
        return;
    }
    std::cout << "Password: ";
    std::cin >> password;

    int id = nextUserId(users);
    users.push_back(std::make_unique<Patient>(id, name, username, password));
    std::cout << "Patient registered with ID: " << id << "\n";
}

void createDefaultAdmin(std::vector<std::unique_ptr<User>> &users) {
    if (!users.empty()) return;
    std::cout << "No users found. Creating default admin (username: admin, password: admin).\n";
    users.push_back(std::make_unique<Admin>(1, "Super Admin", "admin", "admin"));
}

void adminMenu(std::vector<std::unique_ptr<User>> &users,
               std::vector<Appointment> &appts,
               Admin *admin) {
    (void)admin;
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n=== Admin Menu ===\n";
        std::cout << "1. Add doctor\n";
        std::cout << "2. View all users\n";
        std::cout << "3. View all appointments\n";
        std::cout << "0. Logout\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string name, username, password, spec;
            std::cout << "Doctor full name: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            std::cout << "Username: ";
            std::cin >> username;
            if (findUserByUsername(users, username)) {
                std::cout << "Username already exists.\n";
                continue;
            }
            std::cout << "Password: ";
            std::cin >> password;
            std::cout << "Specialization: ";
            std::cin.ignore();
            std::getline(std::cin, spec);

            int id = nextUserId(users);
            users.push_back(std::make_unique<Doctor>(id, name, username, password, spec));
            std::cout << "Doctor added with ID: " << id << "\n";
        } else if (choice == 2) {
            std::cout << "\n--- Users ---\n";
            for (const auto &u : users) {
                std::string role;
                switch (u->getType()) {
                    case UserType::Patient: role = "Patient"; break;
                    case UserType::Doctor: role = "Doctor"; break;
                    case UserType::Admin: role = "Admin"; break;
                }
                std::cout << "ID: " << u->getId()
                          << " | Name: " << u->getName()
                          << " | Username: " << u->getUsername()
                          << " | Role: " << role << "\n";
            }
        } else if (choice == 3) {
            std::cout << "\n--- Appointments ---\n";
            for (const auto &a : appts) {
                std::cout << "ID: " << a.getId()
                          << " | PatientID: " << a.getPatientId()
                          << " | DoctorID: " << a.getDoctorId()
                          << " | Time: " << a.getDateTime()
                          << " | Status: " << a.getStatus() << "\n";
            }
        }
    }
}

void listDoctors(const std::vector<std::unique_ptr<User>> &users) {
    std::cout << "\n--- Doctors ---\n";
    for (const auto &u : users) {
        if (u->getType() == UserType::Doctor) {
            const Doctor *d = dynamic_cast<const Doctor*>(u.get());
            if (d) {
                std::cout << "ID: " << d->getId()
                          << " | Name: " << d->getName()
                          << " | Specialization: " << d->getSpecialization()
                          << "\n";
            }
        }
    }
}

void patientMenu(std::vector<std::unique_ptr<User>> &users,
                 std::vector<Appointment> &appts,
                 Patient *patient) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n=== Patient Menu ===\n";
        std::cout << "1. View doctors\n";
        std::cout << "2. Book appointment\n";
        std::cout << "3. View my appointments\n";
        std::cout << "0. Logout\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        if (choice == 1) {
            listDoctors(users);
        } else if (choice == 2) {
            listDoctors(users);
            int doctorId;
            std::cout << "Enter doctor ID: ";
            std::cin >> doctorId;

            std::string dt;
            std::cout << "Enter date & time (YYYY-MM-DD HH:MM): ";
            std::cin.ignore();
            std::getline(std::cin, dt);

            int apptId = nextAppointmentId(appts);
            appts.emplace_back(apptId, patient->getId(), doctorId, dt, "Pending");
            std::cout << "Appointment booked with ID: " << apptId << "\n";
        } else if (choice == 3) {
            std::cout << "\n--- My Appointments ---\n";
            for (const auto &a : appts) {
                if (a.getPatientId() == patient->getId()) {
                    std::cout << "ID: " << a.getId()
                              << " | DoctorID: " << a.getDoctorId()
                              << " | Time: " << a.getDateTime()
                              << " | Status: " << a.getStatus() << "\n";
                }
            }
        }
    }
}

void doctorMenu(std::vector<Appointment> &appts, Doctor *doctor) {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n=== Doctor Menu ===\n";
        std::cout << "1. View my appointments\n";
        std::cout << "2. Update appointment status\n";
        std::cout << "0. Logout\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "\n--- My Appointments ---\n";
            for (const auto &a : appts) {
                if (a.getDoctorId() == doctor->getId()) {
                    std::cout << "ID: " << a.getId()
                              << " | PatientID: " << a.getPatientId()
                              << " | Time: " << a.getDateTime()
                              << " | Status: " << a.getStatus() << "\n";
                }
            }
        } else if (choice == 2) {
            int apptId;
            std::cout << "Enter appointment ID: ";
            std::cin >> apptId;

            auto it = std::find_if(appts.begin(), appts.end(),
                                   [apptId](const Appointment &a) { return a.getId() == apptId; });
            if (it == appts.end()) {
                std::cout << "Appointment not found.\n";
                continue;
            }
            std::string status;
            std::cout << "Enter new status (Pending/Confirmed/Cancelled): ";
            std::cin >> status;
            it->setStatus(status);
            std::cout << "Status updated.\n";
        }
    }
}

int main() {
    std::vector<std::unique_ptr<User>> users;
    std::vector<Appointment> appointments;

    try {
        loadUsers(users);
        createDefaultAdmin(users);
        loadAppointments(appointments);
    } catch (const std::exception &e) {
        std::cerr << "Startup error: " << e.what() << "\n";
        return 1;
    }

    int choice = -1;
    while (choice != 0) {
        std::cout << "\n=== Hospital Scheduling System ===\n";
        std::cout << "1. Login\n";
        std::cout << "2. Register as patient\n";
        std::cout << "0. Exit\n";
        std::cout << "Choice: ";
        std::cin >> choice;

        if (choice == 1) {
            try {
                User *user = login(users);
                if (user->getType() == UserType::Admin) {
                    adminMenu(users, appointments, dynamic_cast<Admin*>(user));
                } else if (user->getType() == UserType::Doctor) {
                    doctorMenu(appointments, dynamic_cast<Doctor*>(user));
                } else if (user->getType() == UserType::Patient) {
                    patientMenu(users, appointments, dynamic_cast<Patient*>(user));
                }
            } catch (const AuthException &e) {
                std::cout << e.what() << "\n";
            } catch (const std::exception &e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        } else if (choice == 2) {
            registerPatient(users);
        }
    }

    try {
        saveUsers(users);
        saveAppointments(appointments);
    } catch (const std::exception &e) {
        std::cerr << "Shutdown error: " << e.what() << "\n";
    }

    return 0;
}
