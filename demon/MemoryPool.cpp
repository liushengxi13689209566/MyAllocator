/*************************************************************************
	> File Name: MemoryPool.cpp
	> Author: Liu Shengxi 
	> Mail: 13689209566@163.com
	> Created Time: 2018年12月25日 星期二 20时41分15秒
 ************************************************************************/

#include "MemoryPool.h"

namespace Tattoo
{
//默认构造函数
template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::MemoryPool() noexcept
{
	cur_block_ = nullptr;
	cur_Lnode_ = nullptr;
	last_Lnode_ = nullptr;
	free_Lnode_ = nullptr;
}
//拷贝构造函数
template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::MemoryPool(const MemoryPool &pool) noexcept : MemoryPool()
{
}
//移动构造函数
template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::MemoryPool(MemoryPool &&pool) noexcept
{
	cur_block_ = pool.cur_block_;
	pool.cur_block_ = nullptr;
	cur_Lnode_ = pool.cur_Lnode_;
	last_Lnode_ = pool.last_Lnode_;
	free_Lnode_ = pool.free_Lnode_;
}
template <typename T, size_t BlockSize>
template <class U>
MemoryPool<T, BlockSize>::MemoryPool(const MemoryPool<U> &pool) noexcept : MemoryPool()
{
}
//移动赋值操作符
template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize> &MemoryPool<T, BlockSize>::operator=(MemoryPool &&pool) noexcept
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
template <typename T, size_t BlockSize>
MemoryPool<T, BlockSize>::~MemoryPool() noexcept
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

/*返回地址*/
template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::address(reference r) const noexcept
{
	return &r;
}
template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::const_pointer
MemoryPool<T, BlockSize>::address(const_reference r) const noexcept
{
	return &r;
}

/*申请内存快放进内存池*/
template <typename T, size_t BlockSize>
void MemoryPool<T, BlockSize>::allocate_block()
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
// 返回指向分配新元素所需内存的指针
template <typename T, size_t BlockSize>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::allocate(size_t n, const_pointer hint)
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
template <typename T, size_t BlockSize>
inline void MemoryPool<T, BlockSize>::deallocate(pointer p, size_t n)
{
	if (p != nullptr)
	{
		reinterpret_cast<Lnode_pointer_>(p)->next = free_Lnode_;
		free_Lnode_ = reinterpret_cast<Lnode_pointer_>(p);
	}
}
// 计算可以容纳的最多元素
template <typename T, size_t BlockSize>
inline size_t MemoryPool<T, BlockSize>::max_size() const noexcept
{
	size_t max_blocks = -1 / BlockSize;
	return (BlockSize - sizeof(data_pointer_)) / sizeof(size_t) * max_blocks;
}
//这应该与可变参数模板有关
// 在已分配内存上构造对象
template <typename T, size_t BlockSize>
template <class U, class... Args>
inline void MemoryPool<T, BlockSize>::construct(U *p, Args &&... args)
{
	new (p) U(std::forward<Args>(args)...);
}

// 销毁对象
template <typename T, size_t BlockSize>
template <class U>
inline void MemoryPool<T, BlockSize>::destory(U *p)
{
	p->~U(); //手动调用元素 T 的析构函数
}
// 创建新元素
template <typename T, size_t BlockSize>
template <class... Args>
inline typename MemoryPool<T, BlockSize>::pointer
MemoryPool<T, BlockSize>::NewElement(Args &&... args)
{
	pointer result = allocate();
	construct<value_type>(result, std::forward<Args>(args)...);
	return result;
}
// 删除元素
template <typename T, size_t BlockSize>
inline void MemoryPool<T, BlockSize>::DelElement(pointer p)
{
	if (p != nullptr)
	{
		p->~value_type();
		deallocate(p);
	}
}
} // namespace Tattoo