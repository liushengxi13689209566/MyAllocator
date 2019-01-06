#include <stdio.h>
#include "../malloc.h"

#define TOTAL_ALLOCS 200000
#define ALLOC_SIZE 1024 * 1024

int main()
{
    MyMalloc(1);

    int i;
    void *ptr = NULL;

    for (i = 0; i < TOTAL_ALLOCS; i++)
    {
        ptr = MyMalloc(ALLOC_SIZE);
        if (ptr == NULL)
        {
            printf("Memory failed to allocate!\n");
            return 1;
        }

        MyFree(ptr);
    }

    printf("Memory was allocated and MyFreed!\n");
    return 0;
}
