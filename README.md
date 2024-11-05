Custom Shell Implementation
Overview

This project is a simple Unix-like shell built in C++ using Flex and Bison. It supports essential shell features, including command execution, pipelines, I/O redirection, and background processing. This project demonstrates core concepts in command parsing, process management, and inter-process communication in a Unix environment.
Features

    Basic Command Execution: Supports common commands like ls, grep, cat, etc.
    Pipelines: Chain commands with |, e.g., ls -al | grep txt.
    I/O Redirection:
        Output redirection: >, >>
        Input redirection: <
    Background Execution: Run commands in the background using &.
    Child Process Logging: Logs each terminated child process to a log file (process_log.txt).

Files

    command.h: Header file defining the Command and SimpleCommand classes.
    command.cc: Main logic for executing commands, including handling pipelines, redirection, and background processes.
    shell.l: Lexical analyzer defined with Flex to tokenize user input.
    shell.y: Grammar rules defined with Bison to parse commands and identify components like redirection and pipelines.
    Makefile: Automates the build process, including compilation with Flex and Bison.

Installation

To compile the shell, follow these steps:

bash

# Clone the repository
git clone <repository-url>

# Navigate to the directory
cd <directory-name>

# Build the shell
make

Usage

Start the shell by running:

bash

./shell

Example commands:

    Basic Commands: ls -al
    Pipeline: ls -al | grep txt
    Output Redirection: ls -al > output.txt
    Append Output: echo "text" >> output.txt
    Input Redirection: cat < input.txt
    Background Execution: sleep 10 &

Exiting

To exit the shell, use:

bash

exit

Logging

Child process terminations are logged in process_log.txt for debugging and tracking purposes.
