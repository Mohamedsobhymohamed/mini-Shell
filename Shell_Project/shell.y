%{
#include "command.h"
#include <stdio.h>
#include <stdlib.h>

extern "C" {
    int yylex();
    void yyerror(const char *s);
}

%}

%token <string_val> WORD
%token GREAT GREATGREAT LESS PIPE AMPERSAND NEWLINE

%union {
    char *string_val;
}

%% 

goal:
    commands
    ;

commands:
    command
    | commands command
    ;

command:
    simple_command iomodifier_list background_opt NEWLINE {
        Command::_currentCommand.execute();
    }
    | simple_command iomodifier_list PIPE command {
        Command::_currentCommand.insertSimpleCommand(Command::_currentSimpleCommand);
    }
    | NEWLINE
    ;

simple_command:
    command_word arg_list {
        Command::_currentCommand.insertSimpleCommand(Command::_currentSimpleCommand);
    }
    ;

command_word:
    WORD {
        Command::_currentSimpleCommand = new SimpleCommand();
        Command::_currentSimpleCommand->insertArgument($1);
    }
    ;

arg_list:
    /* empty */
    | arg_list argument
    ;

argument:
    WORD { Command::_currentSimpleCommand->insertArgument($1); }
    ;

iomodifier_list:
    /* empty */
    | iomodifier_list iomodifier
    ;

iomodifier:
    GREAT WORD { Command::_currentCommand._outFile = $2; Command::_currentCommand._appendOutput = false; }
    | GREATGREAT WORD { Command::_currentCommand._outFile = $2; Command::_currentCommand._appendOutput = true; }
    | LESS WORD { Command::_currentCommand._inputFile = $2; }
    ;

background_opt:
    /* empty */
    | AMPERSAND { Command::_currentCommand._background = true; }
    ;

%% 

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

