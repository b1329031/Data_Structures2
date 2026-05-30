#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;
using namespace chrono;

// ===== 產生隨機資料 =====
void generate(int* arr, int n) {
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(1, 50000);
    for (int i = 0; i < n; i++) arr[i] = dist(rng);
}

// ===== Insertion Sort =====
void insertion_sort(int* arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}

// ===== Merge Sort =====
void Merge(int *init, int *result, const int l, const int m, const int n) {
    int i1 = l, i2 = m + 1, iResult = l;
    while (i1 <= m && i2 <= n) {
        if (init[i1] <= init[i2]) {
            result[iResult++] = init[i1++];
        } else {
            result[iResult++] = init[i2++];
        }
    }
    while (i1 <= m) result[iResult++] = init[i1++];
    while (i2 <= n) result[iResult++] = init[i2++];
}
void MergePass(int *init, int *result, const int n, const int s) {
    int i = 0;
    for (i = 0; i <= n - 2 * s; i += 2 * s) {
        Merge(init, result, i, i + s - 1, i + 2 * s - 1);
    }
    if (i + s < n) {
        Merge(init, result, i, i + s - 1, n - 1);
    } else {
        for (int j = i; j < n; j++) {
            result[j] = init[j];
        }
    }
}
void MergeSort(int *a, const int n) {
    int *temp = new int[n];
    for (int l = 1; l < n; l *= 2) {
        MergePass(a, temp, n, l);                       // a 合併後寫進 temp
        for (int i = 0; i < n; i++) a[i] = temp[i];     // 複製回 a
    }
    delete[] temp;
}

// ===== Heap Sort =====
void adjust(int* arr, int n, int i) {
    int largest = i, l = 2*i+1, r = 2*i+2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        adjust(arr, n, largest);
    }
}

void heap_sort(int* arr, int n) {
    for (int i = n/2-1; i >= 0; i--) adjust(arr, n, i);
    for (int i = n-1; i > 0; i--) {
        swap(arr[0], arr[i]);
        adjust(arr, i, 0);
    }
}

// ===== 印出資料 =====
void print_data(int* arr, int n) {
    int show = n < 20 ? n : 20;
    for (int i = 0; i < show; i++) cout << arr[i] << " ";
    if (n > 20) cout << "... (共" << n << "個)";
    cout << "\n";
}

int main() {
    // ===== Benchmark =====
    int sizes[] = {10, 100, 1000, 10000, 100000};
    int num_sizes = 5;
    const int MAXN = 100000;

    int* data = new int[MAXN];
    int* a    = new int[MAXN];
    int* b    = new int[MAXN];
    int* c    = new int[MAXN];

    cout << fixed << setprecision(6);
    cout << "\n===== Benchmark =====\n";
    cout << left << setw(10) << "n"
         << setw(15) << "Merge Sort"
         << setw(15) << "Heap Sort"
         << setw(15) << "Insertion Sort" << "\n";

    for (int si = 0; si < num_sizes; si++) {
        int n = sizes[si];
        generate(data, n);

        for (int i = 0; i < n; i++) a[i] = data[i];
        auto t1 = high_resolution_clock::now();
        MergeSort(a, n);
        auto t2 = high_resolution_clock::now();
        double merge_time = duration<double>(t2-t1).count();

        for (int i = 0; i < n; i++) b[i] = data[i];
        auto t3 = high_resolution_clock::now();
        heap_sort(b, n);
        auto t4 = high_resolution_clock::now();
        double heap_time = duration<double>(t4-t3).count();

        for (int i = 0; i < n; i++) c[i] = data[i];
        auto t5 = high_resolution_clock::now();
        insertion_sort(c, n);
        auto t6 = high_resolution_clock::now();
        double insert_time = duration<double>(t6-t5).count();

        cout << left << setw(10) << n
             << setw(15) << merge_time
             << setw(15) << heap_time
             << setw(15) << insert_time << "\n";
    }

    delete[] data;
    delete[] a;
    delete[] b;
    delete[] c;


    int n, choice;
    cout << "請輸入資料量：";
    cin >> n;

    cout << "選擇排序方式：\n";
    cout << "  1 = Merge Sort\n";
    cout << "  2 = Heap Sort\n";
    cout << "  3 = Insertion Sort\n";
    cout << "請輸入選擇：";
    cin >> choice;

    int* arr = new int[n];
    generate(arr, n);

    cout << "\n原始資料：\n";
    print_data(arr, n);

    if      (choice == 1) MergeSort(arr, n);
    else if (choice == 2) heap_sort(arr, n);
    else if (choice == 3) insertion_sort(arr, n);
    else { cout << "無效選擇\n"; delete[] arr; return 0; }

    cout << "排序後結果：\n";
    print_data(arr, n);

    delete[] arr;
    return 0;
}
