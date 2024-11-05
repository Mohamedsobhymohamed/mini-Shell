%{
#include "y.tab.h"
%}

%%

\n              { return NEWLINE; }
[ \t]           { /* skip whitespace */ }
">>"            { return GREATGREAT; }
">"             { return GREAT; }
"<"             { return LESS; }
"|"             { return PIPE; }
"&"             { return AMPERSAND; }
[a-zA-Z0-9/._-]+ { yylval.string_val = strdup(yytext); return WORD; }
.               { return yytext[0]; }

%%

