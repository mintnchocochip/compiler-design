%{
    /*
     * SECTION 1: C Declarations
     * Include headers and declare variables here.
     */
    #include <stdio.h>
%}

/*
 * SECTION 2: Lexical Rules
 * Format: Pattern { Action }
 * Lex uses Top-to-Bottom priority and Longest Match rule.
 */
%%

"int"|"float"|"char"|"if"|"else"|"while"|"return"|"void"   { printf("KEYWORD     : %s\n", yytext); }

[a-zA-Z_][a-zA-Z0-9_]*                                     { printf("IDENTIFIER  : %s\n", yytext); }

[0-9]+(\.[0-9]+)?                                          { printf("NUMBER      : %s\n", yytext); }

\"[^\"]*\"                                                 { printf("STRING      : %s\n", yytext); }

"+"|"-"|"*"|"/"|"="|"<"|">"|"=="|"<="|">="|"!="            { printf("OPERATOR    : %s\n", yytext); }

"("|")"|"{"|"}"|"["|"]"|";"|","                            { printf("PUNCTUATION : %s\n", yytext); }

"#"[a-zA-Z0-9_<>\.]+                                       { printf("PREPROCESSOR: %s\n", yytext); }

[ \t\n]+                                                   { /* Ignore spaces, tabs, and newlines */ }

.                                                          { printf("UNKNOWN     : %s\n", yytext); }

%%

/*
 * SECTION 3: C User Code
 * yywrap() returns 1 to tell Lex to stop at EOF.
 */
int yywrap() {
    return 1;
}

int main() {
    /* Hardcode the input file name so you don't need argc/argv */
    extern FILE *yyin;
    yyin = fopen("lex_input.txt", "r");

    if (!yyin) {
        printf("Error: Could not open 'lex_input.txt'\n");
        return 1;
    }

    printf("--- LEXICAL ANALYSIS OUTPUT ---\n");

    yylex();

    return 0;
}
