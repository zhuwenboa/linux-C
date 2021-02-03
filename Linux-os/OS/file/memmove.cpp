#include<iostream>
#include<vector>
#include<string.h>

using namespace std;

void * mem(void* dst, const void* src, size_t count)
{
   void * ret = dst;
   std::cout << "dst = " << dst << "\n";
   std::cout << "src = " << src << "\n";
   if (dst <= src || (char *)dst >= ((char *)src + count))
   {  
      
      // 若dst和src区域没有重叠，则从起始处开始逐一拷贝
      std::cout << "memcpy running\n";
      while (count--)
      {
         *(char *)dst = *(char *)src;
         dst = (char *)dst + 1;
         src = (char *)src + 1;
      }
   }
   else
   {  // 若dst和src区域交叉，则从尾部开始向起始位置拷贝，这样可以避免数据冲突
      dst = (char *)dst + count - 1;
      src = (char *)src + count - 1;
      while (count--)
      {
         *(char *)dst = *(char *)src;
         dst = (char *)dst - 1;
         src = (char *)src - 1;
      }
   }
   return(ret);
}

int main()
{
    char src[5] = {'1', '2', '3', '4', '5'};
    mem(src+2, src, 3);
    for(int i = 0; i < 5; ++i)
        std::cout << src[i] << "\n";    
}