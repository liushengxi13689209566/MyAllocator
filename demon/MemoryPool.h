/*************************************************************************
	> File Name: MemoryPool.h
	> Author: Liu Shengxi 
	> Mail: 13689209566@163.com
	> Created Time: 2018年12月25日 星期二 19时13分32秒
 ************************************************************************/

#ifndef _MEMORYPOOL_H
#define _MEMORYPOOL_H
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <utility>

namespace Tattoo
{

template <class T, size_t BlockSize = 4096>
class MemoryPool
{
  public:
	typedef T value_type;
	typedef T *pointer;
	typedef T &reference;
	typedef const T *const_pointer;
	typedef const T &const_reference;
	typedef ptrdiff_t difference_type;
	// ptrdiff_t是C / C++ 标准库中定义的一个与机器相关的数据类型。ptrdiff_t类型变量通常用来保存两个指针减法操作的结果

	// 来源于 allocate 的标准
	template <typename U>
	struct rebind
	{
		typedef MemoryPool<U> other;
	};
	MemoryPool() noexcept;								//默认构造函数
	MemoryPool(const MemoryPool &) noexcept;			//拷贝构造函数
	MemoryPool(MemoryPool &&) noexcept;					//移动构造函数
	MemoryPool &operator=(const MemoryPool &) = delete; //赋值操作符
	MemoryPool &operator=(MemoryPool &&) noexcept;		//移动赋值操作符

	template <class U> //实现重新绑定
	MemoryPool(const MemoryPool<U> &) noexcept;

	~MemoryPool() noexcept;
	/*元素取址*/
	pointer address(reference r) const noexcept;
	const_pointer address(const_reference r) const noexcept;

	/*分配与回收一个元素的空间*/
	pointer allocate(size_t n = 1, const_pointer h = 0);
	void deallocate(pointer p, size_t n = 1);

	/*最大大小*/
	size_t max_size() const noexcept;

	// 基于内存池的元素构造和析构
	template <class U, class... Args>	  /*可变参模板*/
	void construct(U *p, Args &&... args); /*引用*/

	template <class U>
	void destory(U *p);

	// 自带申请内存和释放内存的构造和析构
	template <class... Args>
	pointer NewElement(Args &&... args);
	void DelElement(pointer p);

  private:
	// Lnode_ 在创建对象的时候存放对象的值，当这个对象被释放时这块内存作为一个 Lnode_ *指针放入 free 的链表。
	// 所以 Lnode_ 既可以用来存放对象，又可以用来构造链表
	union Lnode_ {
		value_type element;
		Lnode_ *next;
	};
	typedef char *data_pointer_;
	typedef Lnode_ *Lnode_pointer_;
	/*待完成，cur_block_ 写为 block_head */
	Lnode_pointer_ cur_block_;  // 指向当前内存块链表
	Lnode_pointer_ cur_Lnode_;  // 指向当前元素链表
	Lnode_pointer_ last_Lnode_; //指向当前元素链表的最后一个对象
	Lnode_pointer_ free_Lnode_; //指向当前元素链表的最空闲对象

	size_t alig_mem(data_pointer_ p, size_t align) const noexcept;
	void allocate_block(); //申请内存快放进内存池

	static_assert(BlockSize >= 2 * sizeof(Lnode_), "BlockSize too small !!! ");
};

} // namespace Tattoo
#endif
