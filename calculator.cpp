#include <stdio.h>
#include <string.h>

// --- [ส่วนของคนที่ 1] ประกาศโครงสร้างและหัวฟังก์ชัน ---
struct Node {
    double data;
    char op;
    struct Node *next;
    struct Node *back;
};

void AddNode(struct Node **walk, double d, char o);
void ShowAll(struct Node *walk);
void ParseInput(char *input, struct Node **start);
void SolveLogic(struct Node **start);
void DelNode(struct Node *now);

// --- [ส่วนของคนที่ 1] Main ---
int main() {
    struct Node *start = NULL;
    char input[100];

    printf("--- Calculator (No ctype/stdlib) ---\n");
    printf("Input Expression: ");
    scanf("%s", input);

    ParseInput(input, &start);

    printf("\n[Initial List]: ");
    ShowAll(start);

    SolveLogic(&start);

    return 0;
}

// --- [ส่วนของคนที่ 2] Linked List Manager ---
void AddNode(struct Node **walk, double d, char o) {
    struct Node *temp = NULL;
    while (*walk != NULL) {
        temp = *walk;
        walk = &(*walk)->next;
    }
    *walk = new struct Node; // ใช้ new ของ C++ ไม่ต้องพึ่ง stdlib
    (*walk)->data = d;
    (*walk)->op = o;
    (*walk)->next = NULL;
    (*walk)->back = temp;
}

void ShowAll(struct Node *walk) {
    while (walk != NULL) {
        if (walk->op != '=')
            printf("%.2f %c ", walk->data, walk->op);
        else
            printf("%.2f", walk->data);
        walk = walk->next;
    }
    printf("\n");
}

// --- [ส่วนของคนที่ 3] Input Parser (จุดที่แก้เยอะสุด) ---
void ParseInput(char *input, struct Node **start) {
    char numStr[20];
    int j = 0;
    int len = strlen(input);

    for (int i = 0; i < len; i++) {
        // 1. แก้ isdigit -> เช็ค ASCII เอง
        // ถ้าเป็นตัวเลข (0-9) หรือ จุดทศนิยม (.)
        if ((input[i] >= '0' && input[i] <= '9') || input[i] == '.') {
            numStr[j++] = input[i];
            numStr[j] = '\0';
        } 
        else {
            // 2. แก้ atof -> ใช้ sscanf แทน
            double val;
            sscanf(numStr, "%lf", &val); 
            
            AddNode(start, val, input[i]);
            j = 0;
        }
    }
    
    if (j > 0) {
        double val;
        sscanf(numStr, "%lf", &val); // แปลงเลขก้อนสุดท้าย
        AddNode(start, val, '=');
    }
}

// --- [ส่วนของคนที่ 4] Logic Solver ---
void DelNode(struct Node *now) {
    struct Node *del = now->next;
    now->next = del->next;
    if (del->next != NULL) del->next->back = now;
    now->op = del->op;
    delete del; // delete เป็นคำสั่ง C++ ไม่ต้องใช้ stdlib
}

void SolveLogic(struct Node **start) {
    struct Node *walk;
    printf("\n--- Calculation Process ---\n");

    // Loop 1: * / %
    walk = *start;
    while (walk != NULL && walk->next != NULL) {
        char o = walk->op;
        if (o == '*' || o == '/' || o == '%') {
            double val1 = walk->data;
            double val2 = walk->next->data;

            if (o == '*') walk->data = val1 * val2;
            else if (o == '/') walk->data = val1 / val2;
            else if (o == '%') walk->data = (int)val1 % (int)val2;

            DelNode(walk);
            printf("Step: "); ShowAll(*start);
        } else {
            walk = walk->next;
        }
    }

    // Loop 2: + -
    walk = *start;
    while (walk != NULL && walk->next != NULL) {
        char o = walk->op;
        if (o == '+' || o == '-') {
            double val1 = walk->data;
            double val2 = walk->next->data;

            if (o == '+') walk->data = val1 + val2;
            else if (o == '-') walk->data = val1 - val2;

            DelNode(walk);
            printf("Step: "); ShowAll(*start);
        } else {
            walk = walk->next;
        }
    }
    printf("------------------------\n");
    printf("Final Answer = %.2f\n", (*start)->data);
}