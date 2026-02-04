#include <stdio.h>
#include <string.h>

// 1. ประกาศโครงสร้างข้อมูล (เพื่อนทุกคนต้องใช้ตัวนี้)
struct Node {
    double data;
    char op;
    struct Node *next;
    struct Node *back;
};

// 2. ประกาศชื่อฟังก์ชันไว้ (เพื่อให้เพื่อนคนอื่นเขียนไส้ในต่อได้)
void AddNode(struct Node **walk, double d, char o); // งานคนทื่ 2
void ShowAll(struct Node *walk);                    // งานคนทื่ 2
void ParseInput(char *input, struct Node **start);  // งานคนที่ 3
void SolveLogic(struct Node **start);               // งานคนที่ 4

// 3. เขียน Main Function
int main() {
    struct Node *start = NULL;
    char input[100];

    printf("--- Group Calculator Project ---\n");
    printf("Input Expression: ");
    scanf("%s", input);

    // เรียกใช้งานฟังก์ชันของเพื่อนๆ
    // (ตอนนี้มันจะยังทำงานไม่ได้จนกว่าเพื่อนจะเขียนเสร็จ แต่เราวางโครงไว้ก่อน)
    ParseInput(input, &start);   // เรียกคนที่ 3
    
    printf("\n[Initial List]: ");
    ShowAll(start);              // เรียกคนที่ 2

    SolveLogic(&start);          // เรียกคนที่ 4

    return 0;
}

void AddNode(struct Node **walk, double d, char o) {
    struct Node *temp = NULL;

    // 1. วนลูปหาตัวสุดท้าย (ตัวที่ next เป็น NULL)
    while (*walk != NULL) {
        temp = *walk;          // เก็บตัวก่อนหน้าไว้เชื่อมขา back
        walk = &(*walk)->next; // ขยับไปชี้ที่ next ของตัวถัดไป
    }

    // 2. จองพื้นที่สร้างโหนดใหม่
    *walk = new struct Node;
    (*walk)->data = d;
    (*walk)->op = o;
    (*walk)->next = NULL;      // ตัวสุดท้ายต้องชี้ไป NULL
    (*walk)->back = temp;      // เชื่อมขากลับไปหาตัวก่อนหน้า
}

// ฟังก์ชันแสดงผลข้อมูลทั้งหมดใน List
void ShowAll(struct Node *walk) {
    while (walk != NULL) {
        // ถ้าเป็นตัวจบ (=) ให้แสดงแค่ตัวเลข
        if (walk->op == '=') {
            printf("%.2f", walk->data);
        } 
        // ถ้าเป็นตัวทั่วไป ให้แสดงเลขพร้อมเครื่องหมาย
        else {
            printf("%.2f %c ", walk->data, walk->op);
        }
        walk = walk->next; // ขยับไปตัวถัดไป
    }
    printf("\n");
}

// --- งานของคนที่ 3 (แปลงข้อความ) ---
void ParseInput(char *input, struct Node **start) {
    char numStr[20];
    int j = 0;
    int len = strlen(input);

    for (int i = 0; i < len; i++) {
        // เช็คว่าเป็นตัวเลข 0-9 หรือจุดทศนิยม
        if ((input[i] >= '0' && input[i] <= '9') || input[i] == '.') {
            numStr[j++] = input[i];
            numStr[j] = '\0';
        } 
        else {
            // เจอเครื่องหมายคำนวณ ให้เก็บตัวเลขก่อนหน้าลง List
            double val;
            sscanf(numStr, "%lf", &val);
            AddNode(start, val, input[i]); // เรียกใช้ของคนที่ 2
            j = 0;
        }
    }

    // เก็บตัวเลขก้อนสุดท้าย (ที่ไม่มีเครื่องหมายต่อท้าย)
    if (j > 0) {
        double val;
        sscanf(numStr, "%lf", &val);
        AddNode(start, val, '=');
    }
}

// --- งานของคนที่ 4 (คำนวณเลขและยุบโหนด) ---
void DelNode(struct Node *now) {
    struct Node *del = now->next; // ตัวที่จะลบคือตัวถัดไป (ตัวขวามือ)
    now->next = del->next;        // ข้ามหัวตัวที่จะลบ ไปชี้ตัวถัดไปอีกที
    if (del->next != NULL) {
        del->next->back = now;    // ให้ตัวนู้นชี้กลับมาหาเรา
    }
    now->op = del->op;            // ขโมยเครื่องหมายของตัวที่โดนลบมาใส่ตัวเรา
    delete del;                   // ลบตัวนั้นทิ้งออกจากหน่วยความจำ
}

void SolveLogic(struct Node **start) {
    struct Node *walk;
    printf("\n--- Calculation Process ---\n");

    // Loop 1: จัดการ คูณ หาร หารเอาเศษ (*, /, %) ก่อน
    walk = *start;
    while (walk != NULL && walk->next != NULL) {
        char o = walk->op;
        if (o == '*' || o == '/' || o == '%') {
            double v1 = walk->data;
            double v2 = walk->next->data;
            
            if (o == '*') walk->data = v1 * v2;
            else if (o == '/') walk->data = v1 / v2;
            else if (o == '%') walk->data = (int)v1 % (int)v2;

            DelNode(walk); // ยุบโหนดหลังคำนวณเสร็จ
            printf("Step: "); ShowAll(*start); // โชว์สเต็ปการคิด
        } else {
            walk = walk->next;
        }
    }

    // Loop 2: จัดการ บวก ลบ (+, -) ทีหลัง
    walk = *start;
    while (walk != NULL && walk->next != NULL) {
        char o = walk->op;
        if (o == '+' || o == '-') {
            double v1 = walk->data;
            double v2 = walk->next->data;
            
            if (o == '+') walk->data = v1 + v2;
            else if (o == '-') walk->data = v1 - v2;

            DelNode(walk);
            printf("Step: "); ShowAll(*start);
        } else {
            walk = walk->next;
        }
    }
    printf("------------------------\n");
    printf("Final Answer = %.2f\n", (*start)->data);
}