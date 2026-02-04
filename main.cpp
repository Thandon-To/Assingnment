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