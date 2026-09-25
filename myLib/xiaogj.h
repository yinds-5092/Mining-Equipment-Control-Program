#ifndef _XIAOGJ_H_
#define _XIAOGJ_H_
void insertionsort(unsigned char *arr,unsigned int len);
//*******************************************************************/
//功能：数组插值排序
//形参：*arr；待排序数组的指针，len；排序的数的长度
//返回：无
//详解：
//*******************************************************************/

void insertionsort(unsigned char *arr,unsigned int len)//功能：数组插值排序
{
	unsigned int i;
	for(i=1; i<len; i++ )
	{

		signed int key1 = arr[i];
		unsigned int j = i - 1;
		while(key1<arr[j])
		{
			arr[j+1] = arr[j];
			j--;
		}
		if(j != i-1)
		{
			arr[j+1] = key1;
		}
		
		
	}
}

#endif