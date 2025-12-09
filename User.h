#pragma once
#include <string>
#include <memory>

enum class UserType {
    Patient,
    Doctor,
    Admin
};

class User {
protected:
    int id;
    std::string name;
    std::string username;
    std::string password;
    UserType type;

public:
    User(int id,
         const std::string &name,
         const std::string &username,
         const std::string &password,
         UserType type)
        : id(id), name(name), username(username), password(password), type(type) {}

    virtual ~User() = default;

    int getId() const { return id; }
    const std::string &getName() const { return name; }
    const std::string &getUsername() const { return username; }
    UserType getType() const { return type; }

    bool checkPassword(const std::string &pw) const { return pw == password; }

    virtual std::string serialize() const = 0;

    static std::unique_ptr<User> fromLine(const std::string &line);
};
