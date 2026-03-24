#include <stdio.h>
#include <string.h>
#include <ctype.h>

char expr[200];
int pos;
int tempCount = 1;
char lastTemp[10];

char* newTemp() {
    static char t[10];
    sprintf(t, "t%d", tempCount++);
    return t;
}

void E();
void T();
void F();

void F() {
    if (expr[pos] == '(') {
        pos++;
        E();
        pos++;
    } else {
        int i = 0;
        while (isalnum(expr[pos]))
            lastTemp[i++] = expr[pos++];
        lastTemp[i] = '\0';
    }
}

void T() {
    char left[10];
    F();
    strcpy(left, lastTemp);
    while (expr[pos] == '*' || expr[pos] == '/') {
        char op = expr[pos++];
        F();
        char right[10];
        strcpy(right, lastTemp);
        char* t = newTemp();
        printf("%s = %s %c %s\n", t, left, op, right);
        strcpy(left, t);
    }
    strcpy(lastTemp, left);
}

void E() {
    char left[10];
    T();
    strcpy(left, lastTemp);
    while (expr[pos] == '+' || expr[pos] == '-') {
        char op = expr[pos++];
        T();
        char right[10];
        strcpy(right, lastTemp);
        char* t = newTemp();
        printf("%s = %s %c %s\n", t, left, op, right);
        strcpy(left, t);
    }
    strcpy(lastTemp, left);
}

int main() {
    int n;
    scanf("%d", &n);
    getchar();

    for (int i = 0; i < n; i++) {
        char line[200];
        fgets(line, sizeof(line), stdin);

        char lhs[10];
        char rhs[200];
        char* eq = strchr(line, '=');
        int llen = eq - line;
        strncpy(lhs, line, llen);
        lhs[llen] = '\0';

        int k = 0;
        for (int x = 0; lhs[x]; x++)
            if (lhs[x] != ' ') lhs[k++] = lhs[x];
        lhs[k] = '\0';


        int j = 0;
        for (int x = 0; (eq+1)[x]; x++)
            if ((eq+1)[x] != ' ' && (eq+1)[x] != '\n' && (eq+1)[x] != '\r')
                expr[j++] = (eq+1)[x];
        expr[j] = '\0';

        pos = 0;
        tempCount = 1;

        printf("Expression %d: %.*s= %s\n", i+1, llen, line, eq+1);
        E();
        printf("%s = %s\n\n", lhs, lastTemp);
    }

    return 0;
}
