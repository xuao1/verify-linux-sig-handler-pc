#include <stdio.h>

int main() {
    int *ptr = NULL; // 创建一个NULL指针
    printf("尝试解引用NULL指针...\n");
    printf("%d\n", *ptr); // 解引用NULL指针，这将导致段错误
    return 0;
}