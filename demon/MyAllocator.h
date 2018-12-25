/*************************************************************************
	> File Name: MyAllocator.h
	> Author: Liu Shengxi 
	> Mail: 13689209566@163.com
	> Created Time: 2018年12月25日 星期二 16时20分44秒
 ************************************************************************/

#ifndef _MYALLOCATOR_H
#define _MYALLOCATOR_H

template <class T>
class MyAllocator
{
  public:
	MyAllocator();
};
using MY_ALLOC = MyAllocator<T>;

#endif
