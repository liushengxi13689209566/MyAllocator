/*************************************************************************
	> File Name: test.cpp
	> Author: Liu Shengxi 
	> Mail: 13689209566@163.com
	> Created Time: 2019年01月06日 星期日 10时31分06秒
 ************************************************************************/

#include<iostream>
#include<cassert>
using namespace std;
int main(void) {
    int i=8 ,j= 10;
    int  *t1 = &i ;
    void  *t2 = &j ;
    assert((void *)t1 == t2);
}

