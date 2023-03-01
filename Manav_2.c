#include <stdio.h>
#include <stdlib.h>
#include <time.h>    // required to use clock function if executing program on linux
#ifdef _WIN32        // to check if OS is windows (_WIN32 is a macro defined on every windows based gcc compiler)
#include <windows.h> // required to use windows api for time measurement if executing on windows
#endif

/* clock function of time.h on linux provides precision upto microseconds,
but on windows precision upto only milliseconds is supported by clock function
on windows, the windows api has much better functions for measuring time. */

// prototypes:
void mergeSort(int *, int);
void quickSort(int *, int);
void mergeSortActual(int *, int, int);
void merge(int *, int, int, int);
void quickSortActual(int *, int, int);
int partition(int *, int, int);
void deepCopy(int *, int *, int);
void printArray(int *, int);
void generateRandomNumbers();
void LinuxMain();
#ifdef _WIN32
void WindowsMain();
#endif

void printArray(int *arr, int len)
{
    for (int i = 0; i < len; i++)
        printf("%d ", arr[i]);
    printf("\n");
}


void mergeSort(int *arr, int len)
{
    int mid = (len-1)/2;
    mergeSortActual(arr, 0, mid);
    mergeSortActual(arr, mid+1, len-1);
    merge(arr, 0, mid, len-1);
}

void mergeSortActual(int *arr, int low, int high)
{
    if(low < high)
    {
        int mid = (low+high)/2;
        mergeSortActual(arr, low, mid);
        mergeSortActual(arr, mid+1, high);
        merge(arr, low, mid, high);
    }
}

void merge(int *arr, int low, int mid, int high)
{
    int len = high - low + 1;
    int temp[len];
    int i = low;
    int j = mid+1;
    int temp_idx = 0;
    while(i <= mid && j <= high)
    {
        if(arr[i] < arr[j]) 
        {
            temp[temp_idx++] = arr[i];
            i++;
        }
        else 
        {
            temp[temp_idx++] = arr[j];
            j++;
        }
    }
    if(i>mid)
    {
        while(j <= high)
            temp[temp_idx++] = arr[j++];
    }
    else if(j>high)
    {
        while(i <= mid)
            temp[temp_idx++] = arr[i++];
    }
    for(int i=high; i>=low; i--)
        arr[i] = temp[--temp_idx];
}


void quickSort(int *arr, int len)
{
    int part = partition(arr, 0, len-1);
    quickSortActual(arr, 0, part);
    quickSortActual(arr, part+1, len-1);
}

void quickSortActual(int *arr, int low, int high)
{
    if(low < high)
    {
        int part = partition(arr, low, high);
        quickSortActual(arr, low, part);
        quickSortActual(arr, part+1, high);
    }
}

int partition(int *arr, int low, int high) 
{
    int pivot = arr[low];
    int i = low, temp;
    for(int j=low+1; j<=high; j++)
    {
        if(arr[j] < pivot)
        {
            i++;
            temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
        }
    }
    temp = arr[low];
    arr[low] = arr[i];
    arr[i] = temp;
    return i;
}

int main()
{
    //generateRandomNumbers();
#ifdef _WIN32
    WindowsMain();
#else
    LinuxMain();
#endif
    return 0;

    // printf("Enter length of array: ");
    // int size;
    // scanf("%d",&size);
    // int arr[size];
    // for(int i=0; i<size; i++)
    // {
    //     printf("arr[%d] = ",i);
    //     scanf("%d",&arr[i]);
    // }
    // printf("Input array: ");
    // printArray(arr, size);
    // quickSort(arr, size);
    // printf("After sorting: ");
    // printArray(arr, size);

}


void deepCopy(int *source, int *dest, int len)
{
    for (int i = 0; i < len; i++)
        dest[i] = source[i];
}

void generateRandomNumbers()
{
    FILE *fptr = fopen("rand_num.txt", "w");
    time_t cur_time;
    srand((unsigned int)time(&cur_time));
    for (int i = 0; i < 100000; i++)
        fprintf(fptr, "%d\n", rand());
    fclose(fptr);
}

#ifdef _WIN32
void WindowsMain()
{
    FILE *rand_num = fopen("rand_num.txt", "r");
    FILE *dest = fopen("output.txt", "w");
    fprintf(dest, "size   | selection-sort-time | insertion-sort-time\n");
    double time1, time2;
    LARGE_INTEGER clock_freq, start, end;
    QueryPerformanceFrequency(&clock_freq);
    for (int size = 100; size <= 100000; size += 100)
    {
        int arr1[size];
        int arr2[size];
        for (int j = 0; j < size; j++)
            fscanf(rand_num, "%d", &arr1[j]);
        fseek(rand_num, 0, SEEK_SET);
        deepCopy(arr1, arr2, size);

        QueryPerformanceCounter(&start);
        mergeSort(arr1, size);
        QueryPerformanceCounter(&end);
        time1 = (double)(end.QuadPart - start.QuadPart) * 1.0 / clock_freq.QuadPart;

        QueryPerformanceCounter(&start);
        quickSort(arr2, size);
        QueryPerformanceCounter(&end);
        time2 = (double)(end.QuadPart - start.QuadPart) * 1.0 / clock_freq.QuadPart;

        fprintf(dest, "%6d | %19f | %19f\n", size, time1, time2);
        printf("Size %d done!\n", size);
    }
    fclose(rand_num);
    fclose(dest);
}
#endif

void LinuxMain()
{
    FILE *rand_num = fopen("rand_num.txt", "r");
    FILE *dest = fopen("output.txt", "w");
    fprintf(dest, "size\tmerge-sort-time\tquick-sort-time\n");
    double time1, time2;
    clock_t start, end;
    for (int size = 100; size <= 100000; size += 100)
    {
        int arr1[size];
        int arr2[size];
        for (int j = 0; j < size; j++)
            fscanf(rand_num, "%d", &arr1[j]);
        fseek(rand_num, 0, SEEK_SET);
        deepCopy(arr1, arr2, size);

        start = clock();
        mergeSort(arr1, size);
        end = clock();
        time1 = (double)(end - start) * 1.0 / CLOCKS_PER_SEC;

        start = clock();
        quickSort(arr2, size);
        end = clock();
        time2 = (double)(end - start) * 1.0 / CLOCKS_PER_SEC;

        fprintf(dest, "%d\t%f\t%f\n", size, time1, time2);
        printf("Size %d done!\n", size);
    }
    fclose(rand_num);
    fclose(dest);
}

