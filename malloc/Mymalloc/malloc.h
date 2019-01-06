/*************************************************************************
	> File Name: Mymalloc/malloc.h
	> Author: Liu Shengxi 
	> Mail: 13689209566@163.com
	> Created Time: 2019年01月05日 星期六 21时45分03秒
 ************************************************************************/

#ifndef _MALLOC_H
#define _MALLOC_H
#include <cassert>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <list>
#include <iostream>

class Block
{
  public:
	size_t size = 0;
	bool free = false;
	int debug = 0;
};

const int BLOCK_SIZE = sizeof(Block);
std::list<Block *> mem_list;

Block *RequestSpace(size_t size) //需要处理分配异常
{
	class Block *block;
	block = reinterpret_cast<Block *>(sbrk(0));			//得到当前堆顶部的指针
	void *request = sbrk(size + BLOCK_SIZE);			//sbrk(foo)递增堆大小foo并返回指向堆的上一个顶部的指针
	assert(reinterpret_cast<void *>(block) == request); //不明白这是为什么？？
	if (request == reinterpret_cast<void *>(-1))
	{
		throw std::bad_alloc();
		return NULL;
	}
	mem_list.push_back(block); //将其放到链表末尾
	block->size = size;
	block->free = false;
	// block->xxxx = xxx;
	return block;
}
Block *FindFreeBlock(size_t size)
{
	for (auto t : mem_list)
	{
		if ((t->free) && (t->size >= size))
			return t;
	}
	return NULL;
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
			block->free = false;
			// block->debug = xxxxxx;
		}
	}
	return (block + 1);
	/*这是为什么？
	答：这是因为他申请内存的时候都是sizeof(Block) + size,所以+1，直接让其指向原始内存　*/
}
void *MyCalloc(size_t n, size_t size)
{
	size_t sum_size = n * size;
	void *ptr = MyMalloc(sum_size);
	memset(ptr, 0, sum_size);
	return ptr;
}
class Block *GetBlobkPtr(void *ptr)
{
	return (class Block *)ptr - 1;
}
void MyFree(void *ptr)
{
	if (!ptr)
		return;
	else
	{
		class Block *block_ptr = GetBlobkPtr(ptr);
		assert(block_ptr->free == false);
		block_ptr->free = true;
	}
}
void *MyRealloc(void *ptr, size_t size)
{
	if (!ptr)
	{
		return MyMalloc(size);
	}
	class Block *block = GetBlobkPtr(ptr);
	if (block->size >= size)
		return ptr; //空间足够了
	void *new_ptr = MyMalloc(size);
	memcpy(new_ptr, ptr, block->size);
	MyFree(ptr);
	return new_ptr;
}

#endif
