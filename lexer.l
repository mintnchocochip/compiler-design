%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct SymbolTable {
    char name[100];
    char type[20];
    int line_no;
    struct SymbolTable *next;
} SymbolTable;

SymbolTable *head = NULL;
int line_count = 1;

int keyword_count = 0;
int identifier_count = 0;
int operator_count = 0;
int literal_count = 0;
int punctuation_count = 0;

void insert_symbol(char *name, char *type) {
    SymbolTable *temp = head;
    while(temp != NULL) {
        if(strcmp(temp->name, name) == 0) {
            return;
        }
        temp = temp->next;
    }
    
    SymbolTable *new_node = (SymbolTable*)malloc(sizeof(SymbolTable));
    strcpy(new_node->name, name);
    strcpy(new_node->type, type);
    new_node->line_no = line_count;
    new_node->next = head;
    head = new_node;
}

void display_symbol_table() {
    printf("\n========== SYMBOL TABLE ==========\n");
    printf("%-20s %-15s %-10s\n", "Name", "Type", "Line No");
    printf("--------------------------------------------------\n");
    SymbolTable *temp = head;
    while(temp != NULL) {
        printf("%-20s %-15s %-10d\n", temp->name, temp->type, temp->line_no);
        temp = temp->next;
    }
}

int is_valid_identifier(char *str) {
    if(str[0] != '_' && (str[0] < 'a' || (str[0] > 'z' && str[0] < 'A') || str[0] > 'Z')) {
        return 0;
    }
    for(int i = 1; i < strlen(str); i++) {
        if(str[i] != '_' && (str[i] < 'a' || (str[i] > 'z' && str[i] < 'A') || (str[i] > 'Z' && str[i] < '0') || str[i] > '9')) {
            return 0;
        }
    }
    return 1;
}

%}

DIGIT    [0-9]
LETTER   [a-zA-Z_]
ID       {LETTER}({LETTER}|{DIGIT})*
NUMBER   {DIGIT}+
FLOAT    {DIGIT}+\.{DIGIT}+

%%

"int"       { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"float"     { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"double"    { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"char"      { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"void"      { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"if"        { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"else"      { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"while"     { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"for"       { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"do"        { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"return"    { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"class"     { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"public"    { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"private"   { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"protected" { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"using"     { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"namespace" { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"include"   { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"iostream"  { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"cout"      { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"cin"       { printf("<KEYWORD, %s> ", yytext); keyword_count++; }
"std"       { printf("<KEYWORD, %s> ", yytext); keyword_count++; }

"++"        { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"--"        { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"+"         { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"-"         { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"*"         { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"/"         { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"%"         { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"="         { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"=="        { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"!="        { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"<"         { printf("<OPERATOR, %s> ", yytext); operator_count++; }
">"         { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"<="        { printf("<OPERATOR, %s> ", yytext); operator_count++; }
">="        { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"&&"        { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"||"        { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"!"         { printf("<OPERATOR, %s> ", yytext); operator_count++; }
"<<"        { printf("<OPERATOR, %s> ", yytext); operator_count++; }
">>"        { printf("<OPERATOR, %s> ", yytext); operator_count++; }

";"         { printf("<PUNCTUATION, %s> ", yytext); punctuation_count++; }
","         { printf("<PUNCTUATION, %s> ", yytext); punctuation_count++; }
"("         { printf("<PUNCTUATION, %s> ", yytext); punctuation_count++; }
")"         { printf("<PUNCTUATION, %s> ", yytext); punctuation_count++; }
"{"         { printf("<PUNCTUATION, %s> ", yytext); punctuation_count++; }
"}"         { printf("<PUNCTUATION, %s> ", yytext); punctuation_count++; }
"["         { printf("<PUNCTUATION, %s> ", yytext); punctuation_count++; }
"]"         { printf("<PUNCTUATION, %s> ", yytext); punctuation_count++; }
":"         { printf("<PUNCTUATION, %s> ", yytext); punctuation_count++; }
"::"        { printf("<PUNCTUATION, %s> ", yytext); punctuation_count++; }
"#"         { printf("<PUNCTUATION, %s> ", yytext); punctuation_count++; }

{ID}        { 
                if(is_valid_identifier(yytext)) {
                    printf("<IDENTIFIER, %s> ", yytext); 
                    insert_symbol(yytext, "IDENTIFIER");
                    identifier_count++;
                } else {
                    printf("<INVALID_IDENTIFIER, %s> ", yytext);
                }
            }

{FLOAT}     { printf("<FLOAT_LITERAL, %s> ", yytext); literal_count++; }
{NUMBER}    { printf("<INTEGER_LITERAL, %s> ", yytext); literal_count++; }

\"[^\"]*\"  { printf("<STRING_LITERAL, %s> ", yytext); literal_count++; }
\'[^\']\'   { printf("<CHAR_LITERAL, %s> ", yytext); literal_count++; }

"//".*      { }
"/*"([^*]|\*+[^*/])*\*+"/"  { }

\n          { printf("\n"); line_count++; }
[ \t]+      { }
.           { }

%%

int yywrap() {
    return 1;
}

int main(int argc, char *argv[]) {
    FILE *file;
    
    if(argc > 1) {
        file = fopen(argv[1], "r");
        if(!file) {
            printf("Error: Cannot open file %s\n", argv[1]);
            return 1;
        }
        yyin = file;
        printf("========== TOKEN STREAM ==========\n\n");
        yylex();
        fclose(file);
    } else {
        printf("Enter your code (Ctrl+D or Ctrl+Z to end):\n\n");
        printf("========== TOKEN STREAM ==========\n\n");
        yylex();
    }
    
    display_symbol_table();
    
    printf("\n========== TOKEN COUNTS ==========\n");
    printf("Keywords:      %d\n", keyword_count);
    printf("Identifiers:   %d\n", identifier_count);
    printf("Operators:     %d\n", operator_count);
    printf("Literals:      %d\n", literal_count);
    printf("Punctuations:  %d\n", punctuation_count);
    printf("Total Tokens:  %d\n", keyword_count + identifier_count + operator_count + literal_count + punctuation_count);
    printf("Total Lines:   %d\n", line_count);
    
    return 0;
}
