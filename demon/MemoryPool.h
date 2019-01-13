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
	//默认构造函数
	MemoryPool() noexcept
	{
		cur_block_ = nullptr;
		cur_Lnode_ = nullptr;
		last_Lnode_ = nullptr;
		free_Lnode_ = nullptr;
	}
	//拷贝构造函数
	MemoryPool(const MemoryPool &pool) noexcept : MemoryPool()
	{
	}
	//移动构造函数
	MemoryPool(MemoryPool &&pool) noexcept
	{
		cur_block_ = pool.cur_block_;
		pool.cur_block_ = nullptr;
		cur_Lnode_ = pool.cur_Lnode_;
		last_Lnode_ = pool.last_Lnode_;
		free_Lnode_ = pool.free_Lnode_;
	}													//移动构造函数
	MemoryPool &operator=(const MemoryPool &) = delete; //赋值操作符
	//移动赋值操作符
	MemoryPool &operator=(MemoryPool &&pool) noexcept
	{
		if (this != &pool)
		{
			std::swap(cur_block_, pool.cur_block_);
			cur_Lnode_ = pool.cur_Lnode_;
			last_Lnode_ = pool.last_Lnode_;
			free_Lnode_ = pool.free_Lnode_;
		}
		return *this;
	}

	template <class U> //实现重新绑定
	MemoryPool(const MemoryPool<U> &pool) noexcept : MemoryPool()
	{
	}

	~MemoryPool() noexcept
	{
		Lnode_pointer_ curr = cur_block_;
		Lnode_pointer_ next = nullptr;
		while (curr)
		{
			next = curr->next;
			// 转化为 void 指针，是因为 void 类型不需要调用析构函数,只释放空间
			operator delete(reinterpret_cast<void *>(curr));
			curr = next;
		}
	}
	/*元素取址*/
	pointer address(reference r) const noexcept
	{
		return &r;
	}
	const_pointer address(const_reference r) const noexcept
	{
		return &r;
	}

	/*分配与回收一个元素的空间*/
	pointer allocate(size_t n = 1, const_pointer h = 0)
	{
		if (free_Lnode_ != nullptr)
		{
			pointer result = reinterpret_cast<pointer>(free_Lnode_);
			free_Lnode_ = free_Lnode_->next;
			return result;
		}
		else
		{
			// 之前申请的内存用完了，分配新的 block
			if (cur_Lnode_ >= last_Lnode_)
				allocate_block();
			// 从分配的 block 中划分出去
			return reinterpret_cast<pointer>(cur_Lnode_++);
		}
	}
	// 将元素内存归还给 free 内存链表
	void deallocate(pointer p, size_t n = 1)
	{
		if (p != nullptr)
		{
			reinterpret_cast<Lnode_pointer_>(p)->next = free_Lnode_;
			free_Lnode_ = reinterpret_cast<Lnode_pointer_>(p);
		}
	}

	/*最大大小*/
	// 计算可以容纳的最多元素
	size_t max_size() const noexcept
	{
		size_t max_blocks = -1 / BlockSize;
		return (BlockSize - sizeof(data_pointer_)) / sizeof(size_t) * max_blocks;
	}

	// 基于内存池的元素构造和析构
	//这应该与可变参数模板有关
	// 在已分配内存上构造对象
	template <class U, class... Args> /*可变参模板*/
	void construct(U *p, Args &&... args)
	{
		new (p) U(std::forward<Args>(args)...);
	}
	/*引用*/

	// 销毁对象
	template <class U>
	void destroy(U *p)
	{
		p->~U(); //手动调用元素 T 的析构函数
	}

	// 自带申请内存和释放内存的构造和析构
	// 创建新元素
	template <class... Args>
	pointer NewElement(Args &&... args)
	{
		pointer result = allocate();
		construct<value_type>(result, std::forward<Args>(args)...);
		return result;
	}
	void DelElement(pointer p)
	{
		if (p != nullptr)
		{
			p->~value_type();
			deallocate(p);
		}
	}

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
	Lnode_pointer_ free_Lnode_; //指向当前元素链表的空闲对象

	size_t alig_mem(data_pointer_ p, size_t align) const noexcept;
	//申请内存快放进内存池
	void allocate_block()
	{
		/**通过 new 分配内存*/
		data_pointer_ NewBlock = reinterpret_cast<data_pointer_>(operator new(BlockSize));
		//将新分配的内存块放到 block 链表首部　
		reinterpret_cast<Lnode_pointer_>(NewBlock)->next = cur_block_;
		cur_block_ = reinterpret_cast<Lnode_pointer_>(NewBlock);

		data_pointer_ body = NewBlock + sizeof(Lnode_);

		cur_Lnode_ = reinterpret_cast<Lnode_pointer_>(body);
		last_Lnode_ = reinterpret_cast<Lnode_pointer_>(NewBlock + BlockSize - sizeof(Lnode_) + 1);
	}

	static_assert(BlockSize >= 2 * sizeof(Lnode_), "BlockSize too small !!! ");
};

} // namespace Tattoo
#endif
