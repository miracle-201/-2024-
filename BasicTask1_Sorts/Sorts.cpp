#include <iostream>
#include <cstdlib>
#include <ctime>

#define SIZE 10
int count = 0;  //定义全局变量用于計算快速排序轮数

// 冒泡排序
void bubbleSort(int arr[], int n) {
    int temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                //std::swap(arr[j], arr[j + 1]);  或使用函数进行移位
            }
        }
        std::cout << "第 " << i + 1 << " 轮排序：";
        for (int k = 0; k < n; k++) {
            std::cout << arr[k] << " ";
        }
        std::cout << std::endl;
    }
}
// 插入排序
void insertionSort(int arr[], int n) {
    int i, j, temp;
    for (i = 1; i < n; i++) {       //由i到n為未排序区域
        if(arr[i] < arr[i-1])
        {
            temp = arr[i];          //存起要換位的元素
            for(j = i-1; j >= 0 && temp < arr[j]; j--)
            {
                arr[j+1] = arr[j];  //元素後移
            }
            arr[j+1] = temp;        //放回元素至合适位置
        }        
        std::cout << "第 " << i << " 轮排序：";
        for (int k = 0; k < n; k++) {
            std::cout << arr[k] << " ";
        }
        std::cout << std::endl;
    }
}
// 快速排序
void quickSort(int arr[], int low, int high) {
    int i, j, key;
    if (low < high) {   //有兩个或以上数据
        i = low, j = high, key = arr[low];
        do{
            while(i < j && arr[j] >= key)   j--;    //j从左到右扫描找到小于key的元素
            if(i < j){      //confirm i和j 未相遇
                arr[i] = arr[j];    //把小数移到左边
                i++;
                while(i < j && arr[i] <= key)   i++;    //i从右到左扫描找到大于key的元素
                if(i < j){  //confirm i和j未相遇
                    arr[j] = arr[i];    //把大数移到左边
                    j--;
                }
            }
            
        } while(i != j);    //i,j未相遇
        arr[i] = key;
        count++;    //快速排序轮次加一
        std::cout << "第 " << count << " 轮排序：";
        for (int i = 0; i < SIZE; i++) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
        int pi = i;
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
int main() {
    srand(time(0));     //设置随机数种子
    int arr[SIZE];
    // 生成随机数填充数组
    for (int i = 0; i < SIZE; i++) {
        arr[i] = rand() % 100; // 生成0-99之间的随机数
    }
    // 输出初始数组
    std::cout << "初始数组：";
    for (int i = 0; i < SIZE; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    // 冒泡排序
    int arr1[SIZE];
    std::copy(std::begin(arr), std::end(arr), std::begin(arr1));
    std::cout << "冒泡排序：" << std::endl;
    bubbleSort(arr1, SIZE);
    // 插入排序
    int arr2[SIZE];
    std::copy(std::begin(arr), std::end(arr), std::begin(arr2));
    std::cout << "插入排序：" << std::endl;
    insertionSort(arr2, SIZE);
    // 快速排序
    int arr3[SIZE];
    std::copy(std::begin(arr), std::end(arr), std::begin(arr3));
    std::cout << "快速排序：" << std::endl;
    quickSort(arr3, 0, SIZE - 1);
    // 输出最终数组
    std::cout << "最终数组：";
    for (int i = 0; i < SIZE; i++) {
        std::cout << arr3[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}