import os
import pytest
import subprocess
import time as t

EXE = './proj1'

TEST_DIR_PUBLIC = './public-tests'
LOG_FILE_PUBLIC = './public-tests/tests.log'

TEST_DIR_PRIVATE = './private'
LOG_FILE_PRIVATE = './private/tests.log'


def run_command(command):
    """
    Function used to run a shell command and returning its return code and outup.

    :param command: The command to be run on the shell
    :return: The return code of the function and its output
    """ 
    result = subprocess.run(command, capture_output=True, text=True, shell=True)
    return result.returncode, result.stdout


def get_test_files_public():
    """
    Function for listing all the available public test input files

    :return: Returns a list of all the public tests files
    """
    return [file for file in os.listdir(TEST_DIR_PUBLIC) if file.endswith('.in')]


def get_test_files_private():
    """
    Function for listing all the available private test input files

    :return: Returns a list of all the private tests files
    """
    return [file for file in os.listdir(TEST_DIR_PRIVATE) if file.endswith('.in')]


@pytest.mark.parametrize('test_file_public', get_test_files_public()) #It is used to run the public test function for each public test file
def test_public(test_file_public):
    """
    The pytest test function for the public_tests.
    :param test_file_public: the actual test file provived by the pytest parametrize
    :return: The result of the assert
    """
    start_time = t.time() # The start time

    input_file = os.path.join(TEST_DIR_PUBLIC, test_file_public)
    output_file = os.path.join(TEST_DIR_PUBLIC, test_file_public.replace('.in', '.out'))
    diff_file = os.path.join(TEST_DIR_PUBLIC, test_file_public.replace('.in', '.diff'))

    # Run the command to generate the diff file
    command = f"{EXE} < {input_file} | diff - {output_file} > {diff_file}"
    returncode, output = run_command(command)

    end_time = t.time() # The end time
    time = end_time - start_time # The time passed

    # Write the result to the log file
    with open(LOG_FILE_PUBLIC, 'a') as log:
        if returncode == 0:
            log.write(f"test {test_file_public} PASSED in {time} seconds\n")
        else:
            log.write(f"test {test_file_public} FAILED in {time} seconds\n")

    assert returncode == 0, f"test {test_file_public} FAILED"


@pytest.mark.parametrize('test_file_private', get_test_files_private()) #It is used to run the public test function for each private test file
def test_private(test_file_private):
    """
    The pytest test function for the private_tests.
    :param test_file_public: the actual test file provived by the pytest parametrize
    :return: The result of the assert
    """
    start_time = t.time() # The start time

    input_file = os.path.join(TEST_DIR_PRIVATE, test_file_private)
    output_file = os.path.join(TEST_DIR_PRIVATE, test_file_private.replace('.in', '.out'))
    diff_file = os.path.join(TEST_DIR_PRIVATE, test_file_private.replace('.in', '.diff'))

    # Run the command to generate the diff file
    command = f"{EXE} < {input_file} | diff - {output_file} > {diff_file}" # Builds the command, first runs, then uses diff
    returncode, output = run_command(command) #Runs the command built on the above line

    end_time = t.time() # The end time
    time = end_time - start_time # The time passed

    # Write the result to the log file
    with open(LOG_FILE_PRIVATE, 'a') as log:
        if returncode == 0:
            log.write(f"test {test_file_private} PASSED in {time} seconds\n")
        else:
            log.write(f"test {test_file_private} FAILED in {time} seconds\n")

    assert returncode == 0, f"test {test_file_private} FAILED"