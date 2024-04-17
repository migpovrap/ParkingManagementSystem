import os
import pytest
import subprocess

EXE = './proj1'

TEST_DIR_PUBLIC = './public-tests'
LOG_FILE_PUBLIC = './public-tests/tests.log'

TEST_DIR_PRIVATE = './private'
LOG_FILE_PRIVATE = './private/tests.log'

def run_command(command):
    result = subprocess.run(command, capture_output=True, text=True, shell=True)
    return result.returncode, result.stdout

def get_test_files_public():
    return [file for file in os.listdir(TEST_DIR_PUBLIC) if file.endswith('.in')]

def get_test_files_private():
    return [file for file in os.listdir(TEST_DIR_PRIVATE) if file.endswith('.in')]

@pytest.mark.parametrize('test_file_public', get_test_files_public())
def test_public(test_file_public):
    input_file = os.path.join(TEST_DIR_PUBLIC, test_file_public)
    output_file = os.path.join(TEST_DIR_PUBLIC, test_file_public.replace('.in', '.out'))
    diff_file = os.path.join(TEST_DIR_PUBLIC, test_file_public.replace('.in', '.diff'))

    # Run the command to generate the diff file
    command = f"{EXE} < {input_file} | diff - {output_file} > {diff_file}"
    returncode, output = run_command(command)

    # Write the result to the log file
    with open(LOG_FILE_PUBLIC, 'a') as log:
        if returncode == 0:
            log.write(f"test {test_file_public} PASSED\n")
        else:
            log.write(f"test {test_file_public} FAILED\n")

    assert returncode == 0, f"test {test_file_public} FAILED"

@pytest.mark.parametrize('test_file_private', get_test_files_private())
def test_private(test_file_private):
    input_file = os.path.join(TEST_DIR_PRIVATE, test_file_private)
    output_file = os.path.join(TEST_DIR_PRIVATE, test_file_private.replace('.in', '.out'))
    diff_file = os.path.join(TEST_DIR_PRIVATE, test_file_private.replace('.in', '.diff'))

    # Run the command to generate the diff file
    command = f"{EXE} < {input_file} | diff - {output_file} > {diff_file}"
    returncode, output = run_command(command)

    # Write the result to the log file
    with open(LOG_FILE_PRIVATE, 'a') as log:
        if returncode == 0:
            log.write(f"test {test_file_private} PASSED\n")
        else:
            log.write(f"test {test_file_private} FAILED\n")

    assert returncode == 0, f"test {test_file_private} FAILED"