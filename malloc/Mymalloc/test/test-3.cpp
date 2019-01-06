#include <stdio.h>
#include "../malloc.h"

#define START_MALLOC_SIZE 1024 * 1024 * 128
#define STOP_MALLOC_SIZE 1024

void dummy() { return; }

void *reduce(void *ptr, int size)
{
    if (size > STOP_MALLOC_SIZE)
    {
        void *ptr1 = MyRealloc(ptr, size / 2);
        void *ptr2 = MyMalloc(size / 2);

        if (ptr1 == NULL || ptr2 == NULL)
        {
            printf("Memory failed to allocate!\n");
            return NULL;
        }

        ptr1 = reduce(ptr1, size / 2);
        ptr2 = reduce(ptr2, size / 2);

        if (*((int *)ptr1) != size / 2 || *((int *)ptr2) != size / 2)
        {
            printf("Memory failed to contain correct data after many allocations!\n");
            return NULL;
        }

        void *old_ptr1 = ptr1;
        ptr1 = MyRealloc(ptr1, size);
        MyFree(ptr2);

        if (*((int *)ptr1) != size / 2)
        {
            printf("Memory failed to contain correct data after MyRealloc()!\n");
            printf("Expected %i found %i (old %i)\n", (size / 2), *((int *)ptr1), *((int *)old_ptr1));
            dummy();
            return NULL;
        }

        *((int *)ptr1) = size;
        return ptr1;
    }
    else
    {
        *((int *)ptr) = size;
        return ptr;
    }
}

int main()
{
    MyMalloc(1);

    int size = START_MALLOC_SIZE;
    while (size > STOP_MALLOC_SIZE)
    {
        void *ptr = MyMalloc(size);
        ptr = reduce(ptr, size / 2);
        MyFree(ptr);

        size /= 2;
    }

    printf("Memory was allocated, used, and MyFreed!\n");
    return 0;
}
