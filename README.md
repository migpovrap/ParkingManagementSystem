## Projeto de IAED (LEIC-T 2024)

### Parking Management System

**Author:** Miguel Raposo

### Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)

### Introduction
The Parking Management System is a project developed in C for the IAED class. The system aims to manage parking spaces efficiently, allowing users to register, park, and retrieve their vehicles with ease.

### Features
- **Vehicle Registration:** Register vehicles with specific details.
- **Parking Allocation:** Allocate parking spaces to registered vehicles.
- **Vehicle Retrieval:** Retrieve parked vehicles from the system.
- **Parking Statistics:** Display statistics such as available spaces and parked vehicles.
- **Logging:** Maintain logs of all parking activities.

## Installation Instructions

To set up the build environment for the Parking Management System, follow these steps:

1. **Install build dependencies:**
    ```sh
    sudo apt-get update && sudo apt-get install -y build-essential python-is-python3 pip
    ```

2. **Install Python dependencies:**
    ```sh
    python -m pip install --upgrade pip
    pip install pytest
    pip install lizard
    ```

3. **Build the project with GCC:**
    ```sh
    gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o parksystem *.c
    ```

Make sure to run these commands in the root directory of the repository.

### Usage
To use the Parking Management System, run the compiled program:
```bash
./parksystem
```
