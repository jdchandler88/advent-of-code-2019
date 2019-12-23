#include <stdio.h>
#include <stdlib.h>
typedef struct A {
    int a;
} A;

int main() {

    struct A* a = malloc(sizeof(A));
    a->a = 1;

    printf("a=%i\n", a->a);
    a->a++;
    printf("a=%i\n", a->a);

}