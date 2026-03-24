%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Token {
    char type[30];
    char value[100];
    int line_no;
    struct Token *next;
} Token;

typedef struct SymbolTable {
    char name[100];
    char type[20];
    int line_no;
    struct SymbolTable *next;
} SymbolTable;

Token *token_head = NULL;
Token *token_tail = NULL;
SymbolTable *symbol_head = NULL;

int line_count = 1;
int keyword_count = 0;
int identifier_count = 0;
int operator_count = 0;
int literal_count = 0;
int punctuation_count = 0;

void add_token(char *type, char *value, int line) {
    Token *new_token = (Token*)malloc(sizeof(Token));
    strcpy(new_token->type, type);
    strcpy(new_token->value, value);
    new_token->line_no = line;
    new_token->next = NULL;
    
    if(token_head == NULL) {
        token_head = new_token;
        token_tail = new_token;
    } else {
        token_tail->next = new_token;
        token_tail = new_token;
    }
}

void insert_symbol(char *name, char *type) {
    SymbolTable *temp = symbol_head;
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
    new_node->next = symbol_head;
    symbol_head = new_node;
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

void display_tokens() {
    printf("\n========== TOKEN STREAM ==========\n\n");
    Token *temp = token_head;
    int current_line = 1;
    
    printf("Line %d: ", current_line);
    while(temp != NULL) {
        if(temp->line_no != current_line) {
            printf("\n");
            current_line = temp->line_no;
            printf("Line %d: ", current_line);
        }
        printf("<%s, %s> ", temp->type, temp->value);
        temp = temp->next;
    }
    printf("\n");
}

void display_symbol_table() {
    printf("\n========== SYMBOL TABLE ==========\n\n");
    printf("%-20s %-15s %-10s\n", "Name", "Type", "Line No");
    printf("--------------------------------------------------\n");
    SymbolTable *temp = symbol_head;
    while(temp != NULL) {
        printf("%-20s %-15s %-10d\n", temp->name, temp->type, temp->line_no);
        temp = temp->next;
    }
}

void display_counts() {
    printf("\n========== TOKEN STATISTICS ==========\n\n");
    printf("%-20s : %d\n", "Keywords", keyword_count);
    printf("%-20s : %d\n", "Identifiers", identifier_count);
    printf("%-20s : %d\n", "Operators", operator_count);
    printf("%-20s : %d\n", "Literals", literal_count);
    printf("%-20s : %d\n", "Punctuations", punctuation_count);
    printf("%-20s : %d\n", "Total Tokens", keyword_count + identifier_count + operator_count + literal_count + punctuation_count);
    printf("%-20s : %d\n", "Total Lines", line_count);
}

%}

DIGIT    [0-9]
LETTER   [a-zA-Z_]
ID       {LETTER}({LETTER}|{DIGIT})*
NUMBER   {DIGIT}+
FLOAT    {DIGIT}+\.{DIGIT}+

%%

"int"       { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"float"     { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"double"    { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"char"      { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"void"      { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"if"        { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"else"      { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"while"     { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"for"       { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"do"        { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"return"    { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"class"     { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"public"    { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"private"   { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"protected" { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"using"     { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"namespace" { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"include"   { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"iostream"  { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"cout"      { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"cin"       { add_token("KEYWORD", yytext, line_count); keyword_count++; }
"std"       { add_token("KEYWORD", yytext, line_count); keyword_count++; }

"++"        { add_token("OPERATOR", yytext, line_count); operator_count++; }
"--"        { add_token("OPERATOR", yytext, line_count); operator_count++; }
"+"         { add_token("OPERATOR", yytext, line_count); operator_count++; }
"-"         { add_token("OPERATOR", yytext, line_count); operator_count++; }
"*"         { add_token("OPERATOR", yytext, line_count); operator_count++; }
"/"         { add_token("OPERATOR", yytext, line_count); operator_count++; }
"%"         { add_token("OPERATOR", yytext, line_count); operator_count++; }
"="         { add_token("OPERATOR", yytext, line_count); operator_count++; }
"=="        { add_token("OPERATOR", yytext, line_count); operator_count++; }
"!="        { add_token("OPERATOR", yytext, line_count); operator_count++; }
"<"         { add_token("OPERATOR", yytext, line_count); operator_count++; }
">"         { add_token("OPERATOR", yytext, line_count); operator_count++; }
"<="        { add_token("OPERATOR", yytext, line_count); operator_count++; }
">="        { add_token("OPERATOR", yytext, line_count); operator_count++; }
"&&"        { add_token("OPERATOR", yytext, line_count); operator_count++; }
"||"        { add_token("OPERATOR", yytext, line_count); operator_count++; }
"!"         { add_token("OPERATOR", yytext, line_count); operator_count++; }
"<<"        { add_token("OPERATOR", yytext, line_count); operator_count++; }
">>"        { add_token("OPERATOR", yytext, line_count); operator_count++; }

";"         { add_token("PUNCTUATION", yytext, line_count); punctuation_count++; }
","         { add_token("PUNCTUATION", yytext, line_count); punctuation_count++; }
"("         { add_token("PUNCTUATION", yytext, line_count); punctuation_count++; }
")"         { add_token("PUNCTUATION", yytext, line_count); punctuation_count++; }
"{"         { add_token("PUNCTUATION", yytext, line_count); punctuation_count++; }
"}"         { add_token("PUNCTUATION", yytext, line_count); punctuation_count++; }
"["         { add_token("PUNCTUATION", yytext, line_count); punctuation_count++; }
"]"         { add_token("PUNCTUATION", yytext, line_count); punctuation_count++; }
":"         { add_token("PUNCTUATION", yytext, line_count); punctuation_count++; }
"::"        { add_token("PUNCTUATION", yytext, line_count); punctuation_count++; }
"#"         { add_token("PUNCTUATION", yytext, line_count); punctuation_count++; }

{ID}        { 
                if(is_valid_identifier(yytext)) {
                    add_token("IDENTIFIER", yytext, line_count); 
                    insert_symbol(yytext, "IDENTIFIER");
                    identifier_count++;
                } else {
                    add_token("INVALID_IDENTIFIER", yytext, line_count);
                }
            }

{FLOAT}     { add_token("FLOAT_LITERAL", yytext, line_count); literal_count++; }
{NUMBER}    { add_token("INTEGER_LITERAL", yytext, line_count); literal_count++; }

\"[^\"]*\"  { add_token("STRING_LITERAL", yytext, line_count); literal_count++; }
\'[^\']\'   { add_token("CHAR_LITERAL", yytext, line_count); literal_count++; }

"//".*      { }
"/*"([^*]|\*+[^*/])*\*+"/"  { }

\n          { line_count++; }
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
            printf("\n Error: Cannot open file %s\n", argv[1]);
            return 1;
        }
        printf("\n Reading from file: %s\n", argv[1]);
        yyin = file;
        yylex();
        fclose(file);
    } else {
        printf("\n Enter your code (Press Ctrl+D on Linux/Mac or Ctrl+Z on Windows to finish):\n");
        printf("─────────────────────────────────────────────────────────\n");
        yylex();
    }
    
    display_tokens();
    display_symbol_table();
    display_counts();
    
    return 0;
}
