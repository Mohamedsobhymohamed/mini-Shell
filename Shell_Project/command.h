#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>

struct SimpleCommand {
    std::vector<char *> _arguments;

    SimpleCommand();
    void insertArgument(char * argument);
};

struct Command {
    std::vector<SimpleCommand *> _simpleCommands;
    char * _outFile;
    char * _inputFile;
    char * _errFile;
    bool _appendOutput;
    bool _background;

    Command();
    void insertSimpleCommand(SimpleCommand * simpleCommand);
    void clear();
    void execute();
    void prompt();
    static Command _currentCommand;
    static SimpleCommand *_currentSimpleCommand;
};

#endif

