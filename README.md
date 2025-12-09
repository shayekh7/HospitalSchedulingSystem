# HospitalSchedulingSystem

Simple console-based Hospital Scheduling System in C++.

## Features

- User types: Admin, Doctor, Patient
- Persistent storage using plain text files:
  - `data/users.txt`
  - `data/appointments.txt`
- Login system and basic role-based menus:
  - **Admin**
    - Add new doctors
    - View all users
    - View all appointments
  - **Doctor**
    - View their own appointments
    - Update appointment status
  - **Patient**
    - Register new account
    - View doctor list
    - Book appointments
    - View own appointments

## Default admin

On first run, if there is no `users.txt`, the system creates:

- Username: `admin`
- Password: `admin`

## Build

Example using `g++`:

```bash
g++ -std=c++17 -Iinclude src/*.cpp main.cpp -o HospitalSchedulingSystem
```

Run the program from the project root so relative paths to `data/` work:

```bash
./HospitalSchedulingSystem
```
