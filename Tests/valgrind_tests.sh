#!/bin/bash

# Directory containing the test files
TEST_DIR="private"

# Executable to be tested
EXE="./proj1"
# Log file to save valgrind output
LOG_FILE="valgrind_output.log"
# Clear the log file if it exists
> "$LOG_FILE"

# Loop through all .in files in the tests directory
for test_file in "$TEST_DIR"/*.in; do
   echo "Running valgrind for $test_file"
   valgrind "$EXE" < "$test_file" >> "$LOG_FILE" 2>&1
done

echo "All tests completed. Valgrind output saved to $LOG_FILE."