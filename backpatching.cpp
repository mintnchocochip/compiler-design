#include <stdio.h>
#include <string.h>

#define MAXI 200
#define MAXS 32
#define MAXL 64

typedef struct {
    char op[MAXS], a1[MAXS], a2[MAXS], res[MAXS];
    int target, isJump;
} Instr;

typedef struct { int idx[MAXL], n; } List;

static Instr code[MAXI];
static int nextinstr_ = 1;

static List makelist(int i){ List L; L.n=1; L.idx[0]=i; return L; }
static List merge(List a, List b){
    List r; r.n=0;
    for(int i=0;i<a.n;i++) r.idx[r.n++]=a.idx[i];
    for(int i=0;i<b.n;i++) r.idx[r.n++]=b.idx[i];
    return r;
}
static void backpatch(List L, int target){
    for(int k=0;k<L.n;k++) code[L.idx[k]].target = target;
}

static int emit(const char* op, const char* a1, const char* a2, const char* res){
    int i = nextinstr_++;
    strncpy(code[i].op, op?op:"", MAXS);
    strncpy(code[i].a1, a1?a1:"", MAXS);
    strncpy(code[i].a2, a2?a2:"", MAXS);
    strncpy(code[i].res,res?res:"",MAXS);
    code[i].target = -1;
    code[i].isJump = 0;
    return i;
}
static int emit_if(const char* relop, const char* x, const char* y){
    int i = emit(relop, x, y, "goto");
    code[i].isJump = 1;
    return i;
}
static int emit_goto(){
    int i = emit("goto", "", "", "");
    code[i].isJump = 1;
    return i;
}

static void print_tac(){
    for(int i=1;i<nextinstr_;i++){
        if(code[i].isJump && strcmp(code[i].op,"goto")==0){
            printf("%2d: goto %d\n", i, code[i].target);
        } else if(code[i].isJump){
            printf("%2d: if %s %s %s goto %d\n", i, code[i].a1, code[i].op, code[i].a2, code[i].target);
        } else if(strcmp(code[i].op,":=")==0){
            printf("%2d: %s := %s\n", i, code[i].res, code[i].a1);
        } else if(strcmp(code[i].op,"=[]")==0){
            printf("%2d: %s := %s[%s]\n", i, code[i].res, code[i].a1, code[i].a2);
        } else {
            printf("%2d: %s := %s %s %s\n", i, code[i].res, code[i].a1, code[i].op, code[i].a2);
        }
    }
}

static void print_quad(){
    printf("\nQUADS\n");
    printf("Idx  %-6s %-6s %-6s %-8s\n","op","a1","a2","res");
    for(int i=1;i<nextinstr_;i++){
        printf("%-4d %-6s %-6s %-6s %-8s", i, code[i].op, code[i].a1, code[i].a2, code[i].res);
        if(code[i].isJump) printf("  T=%d", code[i].target);
        printf("\n");
    }
}

static void print_triple(){
    printf("\nTRIPLES\n");
    printf("Idx  %-6s %-6s %-6s\n","op","a1","a2");
    for(int i=1;i<nextinstr_;i++){
        printf("%-4d %-6s %-6s %-6s", i, code[i].op, code[i].a1, code[i].a2);
        if(code[i].isJump) printf("  T=%d", code[i].target);
        printf("\n");
    }
}

static void print_indirect(){
    printf("\nINDIRECT TRIPLES\n");
    printf("Ptr  -> Idx\n");
    int ptr=1;
    for(int i=1;i<nextinstr_;i++) printf("%-4d -> %-4d\n", ptr++, i);
}

static void build_given(){
    emit("*","4","i","S1");
    emit("=[]","x","S1","S2");
    emit("*","4","j","S3");
    emit("=[]","y","S3","S4");
    emit("*","S2","S4","S5");
    emit("+","result","S5","S6");
    emit(":=","S6","","result");
    emit("+","j","1","S7");
    emit(":=","S7","","j");
    int i10 = emit_if("<=","j","10"); code[i10].target = 3;
    emit("+","i","1","S8");
    emit(":=","S8","","i");
    int i13 = emit_if("<=","i","20"); code[i13].target = 1;
}

static void backpatch_demo(){
    nextinstr_ = 1;

    int ifi = emit_if("<","a","b");
    List truelist = makelist(ifi);

    int gof = emit_goto();
    List falselist = makelist(gof);

    int M1 = nextinstr_;
    emit(":=","1","","x");
    int goend = emit_goto();
    List nextlist = makelist(goend);

    int M2 = nextinstr_;
    emit(":=","2","","x");

    backpatch(truelist, M1);
    backpatch(falselist, M2);
    backpatch(nextlist, nextinstr_);

    print_tac();
    print_quad();
}

int main(){
    build_given();
    printf("TAC\n");
    print_tac();
    print_quad();
    print_triple();
    print_indirect();

    printf("\nBACKPATCH TAC\n");
    backpatch_demo();
    return 0;
}
