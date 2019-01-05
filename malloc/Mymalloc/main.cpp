/*************************************************************************
	> File Name: MyMalloc.cpp
	> Author: Liu Shengxi 
	> Mail: 13689209566@163.com
	> Created Time: 2019年01月05日 星期六 21时45分56秒
 ************************************************************************/

#include "malloc.h"
#include <stdio.h>
using namespace std;
#define RUNS 10000

int main(void)
{
	MyMalloc(1);
	int i;
	int **arr = (int **)MyMalloc(RUNS * sizeof(int *));

	if (arr == NULL)
	{
		printf("Memory failed to allocate!\n");
		return 1;
	}

	for (i = 0; i < RUNS; i++)
	{
		arr[i] = (int *)MyMalloc(sizeof(int));
		if (arr[i] == NULL)
		{
			printf("Memory failed to allocate!\n");
			return 1;
		}

		*(arr[i]) = i + 1;
	}

	for (i = 0; i < RUNS; i++)
	{
		if (*(arr[i]) != i + 1)
		{
			printf("Memory failed to contain correct data after many allocations!\n");
			return 2;
		}
	}

	for (i = 0; i < RUNS; i++)
	{
		MyFree(arr[i]);
	}

	MyFree(arr);
	printf("Memory was allocated, used, and freed!\n");
	return 0;
}
