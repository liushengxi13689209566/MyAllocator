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
void Memory<T, BlockSize>::allocate_block()
{
	/**通过 new 分配内存*/
	data_pointer_ NewBlock = reinterpret_cast<data_pointer_>(operator new(BlockSize));
	//将新分配的内存块放到 block 链表首部　
	reinterpret_cast<Lnode_pointer_>(NewBlock)->next = cur_block_;
	cur_block_ = reinterpret_cast<Lnode_pointer_>(NewBlock);

	data_pointer_ body =
}

} // namespace Tattoo