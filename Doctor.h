#pragma once
#include "User.h"
#include <string>

class Doctor : public User {
    std::string specialization;

public:
    Doctor(int id,
           const std::string &name,
           const std::string &username,
           const std::string &password,
           const std::string &specialization)
        : User(id, name, username, password, UserType::Doctor),
          specialization(specialization) {}

    const std::string &getSpecialization() const { return specialization; }

    std::string serialize() const override;
};
