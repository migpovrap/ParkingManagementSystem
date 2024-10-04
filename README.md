## Projeto de IAED (LEIC-T 2024)

### Parking Management System

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

## Installation

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
Below are the commands you can use with the Parking Management System:

1. **Terminate the Program (`q`):**
   - **Input:** `q`
   - **Output:** None

2. **Create or List Parking Lots (`p`):**
   - **Input:** `p [<nome-parque> <capacidade> <valor-15> <valor-15-apos-1hora> <valor-max-diario>]`
   - **Output without arguments:** `<nome-parque> <capacidade> <lugares-disponiveis>`
   - **Output with arguments:** None
   - **Errors:**
     - `parking already exists.` if the parking lot name already exists.
     - `invalid capacity.` if the capacity is ≤ 0.
     - `invalid cost.` if any cost is ≤ 0 or costs are not increasing.
     - `too many parks.` if the maximum number of parks is reached.

3. **Register Vehicle Entry (`e`):**
   - **Input:** `e <nome-parque> <matrícula> <data> <hora>`
   - **Output:** `<nome-parque> <lugares-disponiveis>`
   - **Errors:**
     - `no such parking.` if the parking lot does not exist.
     - `parking is full.` if the parking lot is full.
     - `invalid licence plate.` if the license plate is invalid.
     - `invalid vehicle entry.` if the vehicle is already inside a parking lot.
     - `invalid date.` if the date/time is invalid or earlier than the last recorded entry/exit.

4. **Register Vehicle Exit (`s`):**
   - **Input:** `s <nome-parque> <matrícula> <data> <hora>`
   - **Output:** `<matrícula> <data-entrada> <hora-entrada> <data-saída> <hora-saída> <valor-pago>`
   - **Errors:**
     - `no such parking.` if the parking lot does not exist.
     - `invalid licence plate.` if the license plate is invalid.
     - `invalid vehicle exit.` if the vehicle is not in the specified parking lot.
     - `invalid date.` if the date/time is invalid or earlier than the last recorded entry/exit.

5. **List Vehicle Entries and Exits (`v`):**
   - **Input:** `v <matrícula>`
   - **Output:** `<nome-parque> <data-entrada> <hora-entrada> <data-saída> <hora-saída>`
   - **Errors:**
     - `invalid licence plate.` if the license plate is invalid.
     - `no entries found in any parking.` if there are no recorded entries for the license plate.

6. **Show Parking Lot Billing (`f`):**
   - **Input:** `f <nome-parque> [<data>]`
   - **Output with one argument:** `<data> <valor-facturado>`
   - **Output with two arguments:** `<matrícula> <hora-saída> <valor-pago>`
   - **Errors:**
     - `no such parking.` if the parking lot does not exist.
     - `invalid date.` if the date is invalid or after the last recorded entry/exit.

7. **Remove Parking Lot (`r`):**
   - **Input:** `r <nome-parque>`
   - **Output:** `<nome-parque>`
   - **Errors:**
     - `no such parking.` if the parking lot does not exist.
