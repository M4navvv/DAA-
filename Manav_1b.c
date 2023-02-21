#include <stdio.h>
#include <time.h>

void selectionSort(int *arr, int len) {
int min_i, temp;
for(int i=0; i<len; i++) {
min_i = i;
for(int j=i+1; j<len; j++) {
if(arr[j]<arr[min_i]) {
min_i = j;
}
}
if(i!=min_i) {
temp = arr[min_i];
arr[min_i] = arr[i];
arr[i] = temp;
}
}
}

void insertionSort(int *arr, int len) {
int key;
for(int i=1; i<len; i++) {
key = arr[i];
for(int j=0; j<i; j++) {
if(arr[j]>key) {
for(int k=i; k>j; k--) {
arr[k] = arr[k-1];
}
arr[j] = key;
break;
}
}
}
}

void printArray(int *arr, int len) {
for(int i=0; i<len; i++) {
printf("%d ",arr[i]);
}
printf("\n");
}

int deepCopy(int *source, int *dest, int len) {
for(int i=0; i<len; i++) {
dest[i] = source[i];
}
}

int main() {
FILE *fptr = fopen("rand_num.txt","r");
clock_t start, end;
double time1, time2;
for(int i=100; i<=100000; i+=100) {
int arr1[i];
int arr2[i];
for(int j=0; j<i; j++) {
fscanf(fptr, "%d", &arr1[j]);
}
deepCopy(arr1, arr2, i);
fseek(fptr, 0, SEEK_SET);
start = clock();
selectionSort(arr1, i);
end = clock();
time1 = (end - start) * 1.0 / CLOCKS_PER_SEC;
start = clock();
insertionSort(arr2, i);
end = clock();
time2 = (end - start) * 1.0 / CLOCKS_PER_SEC;
printf("%6d | %10f | %f\n", i, time1, time2);
}
fclose(fptr);
}
