#pragma once
#include "User.h"

class Admin : public User {
public:
    Admin(int id,
          const std::string &name,
          const std::string &username,
          const std::string &password)
        : User(id, name, username, password, UserType::Admin) {}

    std::string serialize() const override;
};
