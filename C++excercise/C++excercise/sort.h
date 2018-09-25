#include "stdafx.h"

#ifndef SORT
#define SORT

/// <summary>
/// ֱ�Ӳ�������
/// </summary>
/// <param name="arr">����</param>
/// <param name="len">���鳤��</param>
void DirectInsertSort(int *arr, int len){
	for (int i = 1; i < len; i++)
	{
		int temp = arr[i];
		int j = i - 1;
		while (j>=0&&arr[j]>temp)
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = temp;
	}
}

/// <summary>
/// �۰��������
/// </summary>
/// <param name="arr">����</param>
/// <param name="len">���鳤��</param>
void HalfInsertSort(int *arr, int len){
	for (int i = 1; i < len; i++)
	{
		int temp = arr[i];	// ��ʱ����
		int left = 0, right = i - 1;
		while (left<=right)
		{
			int mid = (left + right) / 2;
			if (arr[mid]>temp)
			{
				right = mid - 1;
			} 
			else
			{
				left = mid + 1;
			}
		}
		for (int j = i - 1; j >= left;j--)
		{
			arr[j + 1] = arr[j];
		}
		arr[left] = temp;
		
	}
}

/// <summary>
/// ϣ������
/// </summary>
/// <param name="arr">����</param>
/// <param name="len">���鳤��</param>
void ShellInsertSort(int *arr, int len){
	int d = len / 2;
	while (d>=1)
	{
		for (int i= d; i < len; i++)
		{
			int temp = arr[i];
			int j = i;
			while (j>=d&&arr[j-d]>temp)
			{
				arr[j] = arr[j-d];
				j -= d;
			}
			arr[j] = temp;
		}
		d /= 2;
	}
}

/// <summary>
/// ð������
/// </summary>
/// <param name="arr">����</param>
/// <param name="len">���鳤��</param>
void BubbleSort(int *arr, int len){
	int temp;
	for (int i = len-1; i >=1; i--)
	{
		int flag = 0;
		for (int j = 0; j < i; j++)
		{
			if (arr[j]>arr[j+1])
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
				flag = 1;
			}
		}
		if (flag==0)
			break;
	}
}

/// <summary>
/// ��������ָ��㷨
/// </summary>
/// <param name="array">����</param>
/// <param name="left">���λ��</param>
/// <param name="right">�Ҷ�λ��</param>
/// <returns>������λ��</returns>
int Partition(int *array, int left, int right){
	int pivot = array[left];
	while (left<right)
	{
		while (left<right&&array[right]>pivot)
		{
			right--;
		}
		array[left] = array[right];

		while (left<right&&array[left]<pivot)
		{
			left++;
		}
		array[right] = array[left];
	}
	array[left] = pivot;
	return right;
}

/// <summary>
/// �������������㷨
/// </summary>
/// <param name="array">����</param>
/// <param name="left">���λ��</param>
/// <param name="right">�Ҷ�λ��</param>
/// <returns>������λ��</returns>
void QuickSort(int *array, int left, int right){
	if (left<right)
	{
		int p = Partition(array, left, right);
		QuickSort(array, left, p - 1);
		QuickSort(array, p + 1, right);
	}
}
#endif // !SORT


void sift(int a[], int root, int len)//�ѵ�������
{
	int child = 2 * root;
	while (child <= len)//ע��ú����Ĳ����Ǵ�root��len�����Դ˴�����д=
	{
		if (child < len&&a[child] < a[child + 1])//ע��˴�������child<len��֮���Բ�ȡ=��Ϊchild=child+1�����
		{
			child = child + 1;    //�õ���ǰ���ڵ���ӽڵ���ֵ�ϴ�Ľڵ�
		}
		if (a[root] < a[child])
		{
			swap(a[root], a[child]);
			root = child;
			child = 2 * root;
		}
		else
		{
			break;//����ط�֮���Կ���ֱ��break����Ϊ�ѵ���������ǰ����������Ƕѣ�ֻ����������֮������µ��������������ǰ��㲻���㣬������������һ���������㡣
		}
	}
}
void cre_heap(int a[], int n)
{
	for (int i = n / 2; i >= 1; i--)//�����һ����Ҷ�ӽ�㿪ʼ���Ե����ϵ��ƣ�֮�����Ǵӷ�Ҷ�ӽ�㿪ʼ������Ϊ�ú����ǽ���ǰ��������ӽ��Ƚϴ�С����Ҷ�ӽ�����ӽ��
	{

		sift(a, i, n);//�����Ѻ����Ĳ���Ϊ��array_name,root,len.
	}
}
void heap_sort(int a[], int n)
{
	cre_heap(a, n);//������ĵ�һ����������
	for (int i = n; i >= 1; i--)
	{
		swap(a[1], a[i]);
		sift(a, 1, i - 1);
	}
}

