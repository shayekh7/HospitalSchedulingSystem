#pragma once
#include "User.h"

class Patient : public User {
public:
    Patient(int id,
            const std::string &name,
            const std::string &username,
            const std::string &password)
        : User(id, name, username, password, UserType::Patient) {}

    std::string serialize() const override;
};
