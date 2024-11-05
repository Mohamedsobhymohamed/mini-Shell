#include "command.h"
#include "shell.tab.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <cstdlib>
#include <vector>

Command Command::_currentCommand;
SimpleCommand* Command::_currentSimpleCommand;

SimpleCommand::SimpleCommand() {}

void SimpleCommand::insertArgument(char* argument) {
    _arguments.push_back(argument);
}

Command::Command() 
    : _outFile(nullptr), _inputFile(nullptr), _errFile(nullptr), 
      _appendOutput(false), _background(false) {}

void Command::insertSimpleCommand(SimpleCommand* simpleCommand) {
    _simpleCommands.push_back(simpleCommand);
}

void Command::clear() {
    for (auto cmd : _simpleCommands) {
        for (char* arg : cmd->_arguments) {
            free(arg);
        }
        delete cmd;
    }
    _simpleCommands.clear();
    
    free(_outFile);
    free(_inputFile);
    free(_errFile);

    _outFile = nullptr;
    _inputFile = nullptr;
    _errFile = nullptr;
    _appendOutput = false;
    _background = false;
}

void Command::execute() {
    if (_simpleCommands.empty()) {
        prompt();
        return;
    }

    // Handle exit command
    if (strcmp(_simpleCommands[0]->_arguments[0], "exit") == 0) {
        std::cout << "Goodbye!" << std::endl;
        exit(0);
    }

    // Handle cd command
    if (strcmp(_simpleCommands[0]->_arguments[0], "cd") == 0) {
        if (_simpleCommands[0]->_arguments.size() > 1) {
            if (chdir(_simpleCommands[0]->_arguments[1]) != 0) {
                perror("chdir");
            }
        } else {
            const char* homeDir = getenv("HOME");
            if (homeDir) {
                chdir(homeDir);
            }
        }
        clear();
        prompt();
        return;
    }

    // Initialize file descriptors for input/output
    int in_fd = _inputFile ? open(_inputFile, O_RDONLY) : dup(STDIN_FILENO);
    int out_fd, err_fd;

    if (_outFile) {
        int flags = O_WRONLY | O_CREAT | (_appendOutput ? O_APPEND : O_TRUNC);
        out_fd = open(_outFile, flags, 0666);
    } else {
        out_fd = dup(STDOUT_FILENO);
    }

    err_fd = _errFile ? open(_errFile, O_WRONLY | O_CREAT | O_TRUNC, 0666) : dup(STDERR_FILENO);

    int prev_fd = in_fd;

    for (size_t i = 0; i < _simpleCommands.size(); ++i) {
        int pipe_fd[2];

        // Create a pipe if it's not the last command
        if (i < _simpleCommands.size() - 1) {
            pipe(pipe_fd);
        }

        pid_t pid = fork();
        if (pid == 0) { // Child process
            dup2(prev_fd, STDIN_FILENO);
            if (i < _simpleCommands.size() - 1) {
                dup2(pipe_fd[1], STDOUT_FILENO);
            } else {
                dup2(out_fd, STDOUT_FILENO);
            }
            dup2(err_fd, STDERR_FILENO);
            
            _simpleCommands[i]->_arguments.push_back(nullptr); // Null-terminate
            execvp(_simpleCommands[i]->_arguments[0], _simpleCommands[i]->_arguments.data());
            perror("execvp");
            exit(1);
        } else if (pid > 0) { // Parent process
            close(prev_fd); // Close the previous input file descriptor
            if (i < _simpleCommands.size() - 1) {
                close(pipe_fd[1]); // Close the write end of the pipe
                prev_fd = pipe_fd[0]; // Use read end for next command
            }
        } else {
            perror("fork");
            exit(1);
        }
    }

    close(in_fd);
    close(out_fd);
    close(err_fd);
    
    if (!_background) {
        waitpid(-1, nullptr, 0); // Wait for all background processes
    }
    
    clear();
    prompt();
}

void Command::prompt() {
    printf("myshell> ");
    fflush(stdout);
}

void log_termination(int) {
    FILE* log = fopen("process_log.txt", "a");
    fprintf(log, "Child process terminated\n");
    fclose(log);
}

int main() {
    signal(SIGINT, SIG_IGN);
    signal(SIGCHLD, log_termination);
    Command::_currentCommand.prompt();
    yyparse(); // Start parsing input
    return 0;
}

