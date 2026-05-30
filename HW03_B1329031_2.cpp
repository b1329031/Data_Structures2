#include <iostream>  
#include <ctime>     
#include <cstdlib>   
#include <iomanip>   

using namespace std;

// 產生隨機資料
void generate(int* arr, int n) {
    srand((unsigned)time(NULL));                
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 50000 + 1;            // 產生 1 到 50000 之間的隨機整數並存入陣列
    }
}

// 插入排序 Insertion Sort
void insertion_sort(int* arr, int n) {

    for (int i = 1; i < n; i++) {
        int key = arr[i];      
        int j = i - 1;      

        while (j >= 0 && arr[j] > key) {            // n^2
            arr[j + 1] = arr[j]; 
            j--;                 
        }
        arr[j + 1] = key;      
    }
}

// 合併排序 Merge Sort 
void Merge(int *init, int *result, const int l, const int m, const int n) {         //把兩段已排好的資料合併成一段寫進result

    int i1 = l, i2 = m + 1, iResult = l;                        //iresult 為寫入result的位置

    while (i1 <= m && i2 <= n) {
        if (init[i1] <= init[i2]) result[iResult++] = init[i1++];           //每次比兩個指標，小的放進result    
        else result[iResult++] = init[i2++];
    }
   
    while (i1 <= m) result[iResult++] = init[i1++];
    while (i2 <= n) result[iResult++] = init[i2++];
}

void MergePass(int *init, int *result, const int n, const int s) {          //決定合併範圍,用步長s把整個陣列從頭到尾每兩段合併
    int i = 0;
    
    for (i = 0; i <= n - 2 * s; i += 2 * s) {                               //每次跳兩格
        Merge(init, result, i, i + s - 1, i + 2 * s - 1);
    }
    
    if (i + s < n) Merge(init, result, i, i + s - 1, n - 1);                //看剩餘元素夠不夠合併
    else for (int j = i; j < n; j++) result[j] = init[j];
}

void MergeSort(int *a, const int n) {
    int *temp = new int[n];                         // 合併的結果寫在暫存

    for (int l = 1; l < n; l *= 2) {                // for loop 的深度是 logn，mergepass 每個元素都走過所以乘n

        MergePass(a, temp, n, l);                                     
        for (int i = 0; i < n; i++) 
            a[i] = temp[i];                         // 暫存再複製到a
    }
    delete[] temp; 
}



// 堆積排序 Heap Sort 
void adjust(int* arr, int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2; 

    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]); 
        adjust(arr, n, largest);    
    }
}

void heap_sort(int* arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) adjust(arr, n, i);         

    for (int i = n - 1; i > 0; i--) {                   //把樹頂和最後一個交換，縮小adjust範圍
        swap(arr[0], arr[i]); 
        adjust(arr, i, 0);                              //高度logn, adjust n次
    }
}

// 印出資料
void print_data(int* arr, int n) {
    int show = n < 20 ? n : 20;                                 // 最多只顯示前 20 個元素
    for (int i = 0; i < show; i++) cout << arr[i] << " ";
    if (n > 20) cout << "... (共" << n << "個)";
    cout << "\n";
}

int main() {
    // 效能測試 (Benchmark) 區塊
    int sizes[] = {10, 100, 1000, 10000, 100000};   // 定義要測試的五種資料量
    int num_sizes = 5;                           
    const int MAXN = 100000;                        // 最大資料量

    // 預先配置好四個動態陣列
    int* data = new int[MAXN]; 
    int* a    = new int[MAXN]; 
    int* b    = new int[MAXN]; 
    int* c    = new int[MAXN]; 

    cout << fixed << setprecision(6); // 設定小數點顯示 6 位數
    cout << "\n===== Benchmark =====\n";
    cout << left << setw(10) << "n" << setw(15) << "Merge Sort" << setw(15) << "Heap Sort" << setw(15) << "Insertion Sort" << "\n";

    // Benchmark計時
    for (int si = 0; si < num_sizes; si++) {      //依序取出五種資料量，並產生對應數量的隨機數字。
        int n = sizes[si];   
        generate(data, n);   

        // 測試 Merge Sort 耗時
        for (int i = 0; i < n; i++) a[i] = data[i]; // 複製資料
        clock_t start = clock();                    // 開始計時
        MergeSort(a, n);
        clock_t end = clock();                      // 結束計時
        double merge_time = (double)(end - start) / CLOCKS_PER_SEC;

        // 測試 Heap Sort 耗時
        for (int i = 0; i < n; i++) b[i] = data[i];
        start = clock();
        heap_sort(b, n);
        end = clock();
        double heap_time = (double)(end - start) / CLOCKS_PER_SEC;

        // 測試 Insertion Sort 耗時
        for (int i = 0; i < n; i++) c[i] = data[i];
        start = clock();
        insertion_sort(c, n);
        end = clock();
        double insert_time = (double)(end - start) / CLOCKS_PER_SEC;

        // 輸出對齊好的測試結果
        cout << left << setw(10) << n << setw(15) << merge_time << setw(15) << heap_time << setw(15) << insert_time << "\n";
    }

    delete[] data; delete[] a; delete[] b; delete[] c;

   
    int n, choice;
    cout << "\n請輸入資料量：";
    cin >> n;

    cout << "選擇排序方式：\n  1 = Merge Sort\n  2 = Heap Sort\n  3 = Insertion Sort\n請輸入選擇：";
    cin >> choice;

    int* arr = new int[n];      // 根據使用者輸入的 n 配置陣列，因為大小是執行時才知道，所以用 new 動態配置。
    generate(arr, n);           // 產生隨機資料

    cout << "\n原始資料：\n";
    print_data(arr, n);         // 印出前 20 筆

    // 根據使用者的選擇執行對應排序
    if      (choice == 1) MergeSort(arr, n);
    else if (choice == 2) heap_sort(arr, n);
    else if (choice == 3) insertion_sort(arr, n);
    else { cout << "無效選擇\n"; delete[] arr; return 0; }

    cout << "排序後結果：\n";
    print_data(arr, n); // 印出排序後的結果

    delete[] arr; // 結束前釋放記憶體
    return 0;     // 程式正常結束
}