/*************************************************************************
	> File Name: Mymalloc/malloc.h
	> Author: Liu Shengxi 
	> Mail: 13689209566@163.com
	> Created Time: 2019年01月05日 星期六 21时45分03秒
 ************************************************************************/

#ifndef _MYMALLOC_H
#define _MYMALLOC_H
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <list>
#include <iostream>

class Block
{
  public:
	size_t size = 0;
	bool free = true;
	int debug = 0;
};

const int BLOCK_SIZE = sizeof(Block);
std::list<Block *> mem_list;

Block *RequestSpace(size_t size) //需要处理分配异常
{
	class Block *block;
	block = reinterpret_cast<Block *>(sbrk(0));
	void *request = sbrk(size + BLOCK_SIZE);
	assert(reinterpret_cast<void *>(block) == request);
	if (request == reinterpret_cast<void *>(-1))
	{
		throw std::bad_alloc();
		return NULL;
	}
	block->size = size;
	block->free = true;
	// block->size = size;
	return block;
}
Block *FindFreeBlock(size_t size)
{
	for (auto t : mem_list)
	{
		if ((t->free) && (t->size >= size))
			return t;
	}
}

void *MyMalloc(size_t size)
{
	if (size <= 0)
		return NULL;
	class Block *block;
	int mem_size = mem_list.size();
	if (mem_size == 0)
	{
		block = RequestSpace(size);
	}
	else
	{
		block = FindFreeBlock(size);
		if (!block) //有空余的空间，但是没有合适的空间可以分配给申请的大小
		{
			block = RequestSpace(size);
		}
		else //找到一块合适的内存块
		{
			block->free = true;
			// block->debug = xxxxxx;
		}
	}
	return (block + 1); //这是为什么呐？？？？？？？？？？？？？？？？？？？？？？
}
void MyFree(void *ptr)
{
}

#endif
