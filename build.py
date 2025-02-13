#!/usr/bin/env python3

######################################################
# Will build for target or host
# 
# Arguments:
#
#   Cleaning:
#       -c
#           Feature:
#               [X] Removes all binaries
#               [X] Removes build directory
# 
#   Building for HOST:
#
#       Features for all options:
#           [X] x86 binary       
#
#       -r
#           Features: 
#               [ ] Unit Tests
#               [ ] Debug Statements
#       -d
#           Features: 
#               [ ] Unit Tests
#               [X] Debug Statements
#
#       -u
#           Features: 
#               [X] Unit Tests
#               [ ] Debug Statements
#       -v
#           Features: 
#               [X] Unit Tests
#               [X] Debug Statements
#   
#   BUILDING FOR TARGET:
#
#       -t
#           Features:
#               [ ] A cookie
#               [X] A working binary for milk (maybe)
#
#       -td DEBUG VERSION FOR TARGET
#           Features:  
#               [X] Debug 
#               [X] A working binary for milk (maybe)
# 
# Usage:
# ./build.py -<ARGUMENT>
#   Example:
#       ./build.py -h <= Builds a binary that will run on host machine without debug statements nor runs unit tests
#       ./build.py -c <= Will remove the binary (and cmake build) created by ./build.py -h
#
########################################################
import subprocess
import sys
import os

def print_help_menu():
    print("""
Arguments:

    Cleaning Project:
    -c:     Cleans project\n

    Building for HOST:
    -p:     Without Unit Testing and Debug statments
    -d:     Debug statements only
    -u:     Unit Tests only
    -v:     Unit Tests and Debug statements

    Building for TARGET:
    -t:     Builds for the MILKV-DUO
    -td:    DEBUG version for MILKV

Help:
    --help

Example:
    ./build.py -c <= Cleans binaries and deletes build dir
    ./build.py -d <= Builds the host debug binary version 
""")

if(len(sys.argv) != 2):
    print("Usage Error: ./build.py <ARGUMENT>")
    print_help_menu()
    sys.exit()

if(sys.argv[1] == "--help"):
    print_help_menu()
    sys.exit()

def ensure_build_directory():
    if not os.path.exists("build"):
        os.makedirs("build")

#rebuilds build every time
subprocess.run(["sudo", "rm", "-rf", "build/"])

#PRODUCTION VERSION
if(sys.argv[1] == '-p' or sys.argv[1] == '-P'):
    ensure_build_directory()
    current_directory = os.getcwd()
    os.chdir(f"{current_directory}/build")
    subprocess.run(["cmake", "..", "-DHOST=ON"])
    subprocess.run(["cmake", "--build", "."])
    os.chdir("../bin")
    subprocess.run(["./Executable"])

    sys.exit()

#CLEAN
elif(sys.argv[1] == '-c' or sys.argv[1] == '-C'):
    current_directory = os.getcwd()
    os.chdir(f"{current_directory}/bin")

    files_to_delete = ["Executable", "test"]
    
    for file_name in files_to_delete:
        if os.path.exists(file_name):
            subprocess.run(["rm", "-rf", file_name])
            print(f"Deleted {file_name}")

    os.chdir("..")
    subprocess.run(["sudo", "rm", "-rf", "build/"])

    sys.exit()

#DEBUG VERSION
elif(sys.argv[1] == "-d" or sys.argv[1] == "-D"):
    ensure_build_directory()
    
    current_directory = os.getcwd()
    
    os.chdir(f"{current_directory}/build")
    
    subprocess.run(["cmake", "..", "-DDEBUG=ON", "-DHOST=ON"])
    subprocess.run(["cmake", "--build", "."])
    
    os.chdir("../bin")
    subprocess.run(["./Executable"])

    sys.exit()

#UNIT TESTING VERSION
elif(sys.argv[1] == "-u" or sys.argv[1] == "-U"):
    ensure_build_directory()
    
    current_directory = os.getcwd()
    
    os.chdir(f"{current_directory}/build")
    
    subprocess.run(["cmake", "..", "-DCOMPILE_TEST=ON", "-DHOST=ON"])
    subprocess.run(["cmake", "--build", "."])
    
    os.chdir("../bin")
    subprocess.run(["./test"])

    sys.exit()

#UNIT TESTING + DEBUG VERSION
elif(sys.argv[1] == "-v" or sys.argv[1] == "-V"):
    ensure_build_directory()
    
    current_directory = os.getcwd()
    
    os.chdir(f"{current_directory}/build")
    
    subprocess.run(["cmake", "..", "-DCOMPILE_TEST=ON", "-DDEBUG=ON", "-DHOST=ON"])
    subprocess.run(["cmake", "--build", "."])
    
    os.chdir("../bin")
    
    subprocess.run(["./test"])

#TARGET (milk) VERSION
elif(sys.argv[1] == "-t" or sys.argv[1] == "-T"):
    ensure_build_directory()

    image_name = "morse-code-project"

    build_command = [
        "docker",
        "run", "--rm", "-v",
        f"{os.getcwd()}:/app",
        "-t", f"{image_name}", 
        "bash", "-c",
        "cd build && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/app/milkv_duo.cmake ..",
    ]

    subprocess.run(build_command)

    compile_command = [
        "docker",
        "run", "--rm", "-v",
        f"{os.getcwd()}:/app",
        "-t", f"{image_name}",
        "bash", "-c",
        "cd build && cmake --build .",
    ]

    subprocess.run(compile_command)

    sys.exit()
#TARGET (milk) VERSION
elif(sys.argv[1] == "-td" or sys.argv[1] == "-TD"):
    ensure_build_directory()

    image_name = "morse-code-project"

    build_command = [
        "docker",
        "run", "--rm", "-v",
        f"{os.getcwd()}:/app",
        "-t", f"{image_name}", 
        "bash", "-c",
        "cd build && cmake -DDEBUG=ON -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/app/milkv_duo.cmake ..",
    ]

    subprocess.run(build_command)

    compile_command = [
        "docker",
        "run", "--rm", "-v",
        f"{os.getcwd()}:/app",
        "-t", f"{image_name}",
        "bash", "-c",
        "cd build && cmake --build .",
    ]

    subprocess.run(compile_command)

    sys.exit()

else:
    print("Usage Error: ./build.py <ARGUMENT>")
    print_help_menu()
    sys.exit()

