#include <stdio.h>
#include "../malloc.h"

int main()
{
    int *ptr = (int *)MyMalloc(sizeof(int));
    if (ptr == NULL)
    {
        printf("Failed to MyMalloc a single int\n");
        return 1;
    }

    *ptr = 1;
    *ptr = 100;

    MyFree(ptr);

    printf("MyMalloc'd an int, assigned to it, and MyFree'd it\n");

    int *ptr2 = (int *)MyMalloc(sizeof(int));
    if (ptr2 == NULL)
    {
        printf("Failed to MyMalloc a single int\n");
        return 1;
    }

    *ptr2 = 2;
    *ptr2 = 200;

    MyFree(ptr2);
    printf("MyMalloc'd an int, assigned to it, and MyFree'd it #2\n");

    MyMalloc(1); // Screw up alignment.

    int *ptr3 = (int *)MyMalloc(sizeof(int));
    if (ptr3 == NULL)
    {
        printf("Failed to MyMalloc a single int\n");
        return 1;
    }

    *ptr3 = 3;
    *ptr3 = 300;

    MyFree(ptr3);
    printf("MyMalloc'd an int, assigned to it, and MyFree'd it #3\n");

    return 0;
}
