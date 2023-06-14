#include <iostream>
#include <random>
#include <time.h>
using namespace std;

typedef int ElemType;

typedef struct {
    ElemType *r;
    int len;
} SqTable;

void InitList(SqTable &L, int len) {
    // 0号单元不用
    L.r = (ElemType*)malloc((len+1)*sizeof(ElemType));
    L.len = len;
}

void CopyList(SqTable L, SqTable &newL) {
    newL.r = (ElemType*)malloc((L.len+1)*sizeof(ElemType));
    newL.len = L.len;
    memcpy(newL.r, L.r, (L.len+1)*sizeof(ElemType));
}

void PrintList(SqTable L) {
    for (int i = 1;i <= L.len;i++) {
        printf("%d ", L.r[i]);
    }
}

// 求一个整数的p次方
int intpow(int n, unsigned int p) {
    int res = 1;
    for (unsigned int i=0; i<p; ++i)
        res *= n;
    return res;
}

// 生成一个随机整数，其取值范围是[0, bound]
int randb(int bound) {
    int r = 0;
    unsigned int power = 0;
    do {
        r *= RAND_MAX;
        r += rand(); ++power;
    } while (intpow(RAND_MAX, power) < bound);
    return r % (bound+1);
}

// 随机打乱一个数组
void RandomShuffleList(SqTable L) {
    ElemType* array = L.r + 1; int n = L.len;
    for (int i=n-1; i>0; --i) {
        int j = randb(i); // 0<=j<=i
        ElemType tmp = array[i]; array[i] = array[j]; array[j] = tmp;
    }
}

// 交换函数
void swap(SqTable& L, int i, int j) {
    int temp;
    temp = L.r[i];
    L.r[i] = L.r[j];
    L.r[j] = temp;
}

// 选择排序
void SelSort(SqTable& L) {
    for (int i = 1;i < L.len;i++) {
        int low = i;
        for (int j = L.len;j > i;j--)
            if (L.r[j] < L.r[low]) low = j;
        swap(L, i, low);
    }
}

// 冒泡排序
void BubbleSort(SqTable& L) {
    for (int i = 1;i < L.len;i++) {
        for (int j = L.len;j > i;j--) {
            if (L.r[j] < L.r[j - 1]) {
                swap(L, j, j - 1);
            }
        }
    }
}

// 插入排序

void InsSort(SqTable& L) {
    int i,j;
    for (i = 2;i <= L.len;i++) {
        if (L.r[i] > L.r[i - 1])continue;
        L.r[0] = L.r[i];
        for (j = i - 1;L.r[0] < L.r[j];j--)
            L.r[j + 1] = L.r[j];
        L.r[j + 1] = L.r[0];
    }
}

// 快速排序

int FindPivot(SqTable& L, int i, int j) { return (i + j) / 2; }

int partition(SqTable& L, int i, int r, int pivot) {
    int l = i - 1;
    do {
        while (L.r[++l] < pivot);
        while (r >= i && L.r[--r] > pivot);
        swap(L, l, r);
    } while (l < r);
    swap(L, l, r);
    return l;
}

void QuickSort(SqTable& L, int i, int j) {
    int pivotIndex = (L, i, j);
    swap(L, pivotIndex, j);
    int k = partition(L, i, j, L.r[j]);
    swap(L, k, j);
    if ((k - i) > 1) QuickSort(L, i, k - 1);
    if ((j - k) > 1) QuickSort(L, k + 1, j);
}

// 堆排序

void HeapAdjust(SqTable& L, int s, int t) {
    int w = L.r[s];
    for (int i = 2 * s;i <= t;i *= 2) {
        if (i < t)i = (L.r[i] > L.r[i + 1]) ? i : i + 1;
        if (w >L.r[i])break;
        L.r[s] = L.r[i];
        s = i;
    }
    L.r[s] = w;
}

void HeapSort(SqTable& L) {
    for (int i = L.len / 2;i >= 1;i--)
        HeapAdjust(L, i, L.len);
    for (int i = L.len;i > 1;i--) {
        swap(L, 1, i);
        HeapAdjust(L, 1, i - 1);
    }
}

// 归并排序

void Merge(int Rs[], int Rt[], int s, int m, int t) {
    int i, j, k;
    for (i = s, j = m + 1, k = i;i <= m && j <= t;k++) {
        if (Rs[i] <= Rs[j]) Rt[k] = Rs[i++];
        else Rt[k] = Rs[j++];
    }
    for (;i <= m;) Rt[k++] = Rs[i++];
    for (;j <= t;)Rt[k++] = Rs[j++];
}

void MSortPass(int Rs[], int Rt[], int s, int t) {
    if (s < t) {
        int m = (s + t) / 2;
        MSortPass(Rs, Rt, s, m);
        MSortPass(Rs, Rt, m + 1, t);
        Merge(Rs, Rt, s, m, t);
        for(int i=s;i<=t;i++)Rs[i] = Rt[i];
    }
}

void MergeSort(SqTable& L) {
    int* temp = new int[L.len + 1];
    MSortPass(L.r, temp, 1, L.len);
    delete []temp;
}


void Statistc(int N,float* result) {
    SqTable L;
    InitList(L, N);
    for (int i=1; i<=N; ++i) L.r[i] = i;
    RandomShuffleList(L);
    clock_t begin, end;

     // 排序算法1
     SqTable L1;
     CopyList(L, L1);
     begin = clock(); // 计时器开始
     BubbleSort(L1);
     end = clock(); // 计时器结束
     printf("BubbleSort time: %g seconds\n", (float)(end - begin) / CLOCKS_PER_SEC);
     result[0] += (float)(end - begin) / CLOCKS_PER_SEC;

    // 排序算法2
    SqTable L2;
    CopyList(L, L2);
    begin = clock(); // 计时器开始
    QuickSort(L2, 1, N);
    end = clock(); // 计时器结束
    printf("QuickSort time: %g seconds\n", (float)(end - begin) / CLOCKS_PER_SEC);
    result[1] += (float)(end - begin) / CLOCKS_PER_SEC;


    // 排序算法3
    SqTable L3;
    CopyList(L, L3);
    begin = clock(); // 计时器开始
    HeapSort(L3);
    end = clock(); // 计时器结束
    printf("HeapSort time: %g seconds\n", (float)(end - begin) / CLOCKS_PER_SEC);
    result[2] += (float)(end - begin) / CLOCKS_PER_SEC;

    // 排序算法4
    SqTable L4;
    CopyList(L, L4);
    begin = clock(); // 计时器开始
    MergeSort(L4);
    end = clock(); // 计时器结束
    printf("HeapSort time: %g seconds\n", (float)(end - begin) / CLOCKS_PER_SEC);
    result[3] += (float)(end - begin) / CLOCKS_PER_SEC;

     // 排序算法5
     SqTable L5;
     CopyList(L, L5);
     begin = clock(); // 计时器开始
     SelSort(L5);
     end = clock(); // 计时器结束
     printf("SelSort time: %g seconds\n", (float)(end - begin) / CLOCKS_PER_SEC);
     result[4] += (float)(end - begin) / CLOCKS_PER_SEC;

     // 排序算法6
     SqTable L6;
     CopyList(L, L6);
     begin = clock(); // 计时器开始
     InsSort(L6);
     end = clock(); // 计时器结束
     printf("InsSort time: %g seconds\n", (float)(end - begin) / CLOCKS_PER_SEC);
     result[5] += (float)(end - begin) / CLOCKS_PER_SEC;
     // ...
}

int main() {
    int range[3] = { 10000,100000,200000 };
    int times = 1;
    float* result = new float[6];
    for (int N : range) {
        for (int i = 0;i < times;i++)
            result[i] = 0;
        for (int i = 0;i < times;i++)
            Statistc(N, result);
        printf("N=%d时排序%d次平均用时为:\n", N, times);
        printf("BubbleSort time: %g seconds\n", result[0] / times);
        printf("QuickSort time: %g seconds\n", result[1] / times);
        printf("HeapSort time: %g seconds\n", result[2] / times);
        printf("MergeSort time: %g seconds\n", result[3] / times);
        printf("SelSort time: %g seconds\n", result[4] / times);
        printf("InsSort time: %g seconds\n\n", result[5] / times);
    }
}

/*
运行结果：

N=10000时排序5次平均用时为:
BubbleSort time: 0.3164 seconds
QuickSort time: 0.0006 seconds
HeapSort time: 0.0012 seconds
MergeSort time: 0.001 seconds
SelSort time: 0.1108 seconds
InsSort time: 0.0566 seconds

N=100000时排序5次平均用时为:
QuickSort time: 0.0158 seconds
HeapSort time: 0.0216 seconds
MergeSort time: 0.0156 seconds

N=1000000时排序5次平均用时为:
QuickSort time: 0.1868 seconds
HeapSort time: 0.272 seconds
MergeSort time: 0.2062 seconds

*/
