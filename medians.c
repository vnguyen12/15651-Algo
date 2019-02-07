/*
 * The algorithm uses QuickSelect method to find the 1st element. As proven in
 * class, this takes an expected time of O(n). Then, it proceeds to find 2nd,
 * 4th, 8th, ... 2^kth element where 2^k <= n, and discard all elements to the
 * left of each before finding the next rank.
 *
 * Since these elements are also medians of their respective subarrays,
 * when used as the pivots, the array size decreases by half every times.
 *
 * So the expected running time is T(n) = T(n/2) + O(n).
 *
 * Applying the master formula, T(n) = O(n).
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>

int randomPartition(int arr[], int l, int r);

// This function returns k'th smallest element in arr[l..r] using
// QuickSelect method. ASSUMPTION: ELEMENTS IN ARR[] ARE DISTINCT
int kthSmallest(int arr[], int l, int r, int k)
{
    // If k is smaller than number of elements in array
    if (k > 0 && k <= r - l + 1)
    {
        // Partition the array around a random element and
        // get position of pivot element in sorted array
        int pos = randomPartition(arr, l, r);

        // If position is same as k
        if (pos-l == k-1)
            return arr[pos];
        // If position is more, recur for left subarray
        if (pos-l > k-1)
          return kthSmallest(arr, l, pos-1, k);
        // Else recur for right subarray
        return kthSmallest(arr, pos+1, r, k-pos+l-1);
    }

    // If k is more than number of elements in array
    return INT_MAX;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Standard partition process of QuickSort().  It considers the last
// element as pivot and moves all smaller element to left of it and
// greater elements to right. This function is used by randomPartition()
int partition(int arr[], int l, int r)
{
    int x = arr[r], i = l,j;
    for (j = l; j <= r - 1; j++)
    {
        if (arr[j] <= x)
        {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[r]);
    return i;
}

// Picks a random pivot element between l and r and partitions
// arr[l..r] arount the randomly picked element using partition()
int randomPartition(int arr[], int l, int r)
{
    int n = r-l+1;
    int pivot = rand() % n;
    swap(&arr[l + pivot], &arr[r]);
    return partition(arr, l, r);
}

// Main method
int main()
{
	int i, size;
	scanf("%d", &size);
	int arr[size];
	for(i = 0; i < size; i++)
		scanf("%d", &arr[i]);
	int k = log2(size);
	printf("%d\n", k);
	for(i = 0; pow(2,i) <= size; i++)
		printf("%d ",kthSmallest(arr, 0, size-1, pow(2,i)));

  return 0;
}
