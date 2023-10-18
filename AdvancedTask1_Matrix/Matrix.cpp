#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

class Matrix {
private:
    int rows;
    int cols;
    double** data;
public:
    // 构造函数
    Matrix(int rows, int cols) : rows(rows), cols(cols) {
        // 动态申请内存
        data = new double*[rows];   //先构建一维数组
        for (int i = 0; i < rows; i++) {
            data[i] = new double[cols] {0};   //再逐个置入数组变成二维并赋初值0
        }
    }
    // 设置矩阵元素
    void setValue(int row, int col) {
        int count = 1;
        if (row >= 0 && col >= 0) {
        cout << "Please input " << row << "*" << col << " elements." << endl;
            while(count < row*col)
                for(int i = 0; i < row; i++)
                    for(int j = 0; j < col; j++){
                        cin >> data[i][j];
                        count++;
                    }
        }
    }
    // 获取矩阵元素
    int getValue(int row, int col) {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            return data[row][col];
        }
        return 0; // 非法输入返回0
    }
    // 矩阵加法
    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            cout << "维度不匹配，无法进行加法运算！" << endl;
            return Matrix(0, 0);
        }
        Matrix result(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }
    // 矩阵减法
    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            cout << "维度不匹配，无法进行减法运算！" << endl;
            return Matrix(0, 0);
        }
        Matrix result(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result.data[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return result;
    }
    // 矩阵乘法
    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            cout << "维度不匹配，无法进行乘法运算！" << endl;
            return Matrix(0, 0);
        }
        Matrix result(rows, other.cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.cols; j++) {
                for (int k = 0; k < cols; k++) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }
    //构建单位矩阵
    Matrix identity(int rows, int cols){
        Matrix iden_mat(rows+1, cols+1);    //多构建一行和一列用于后续求逆换行和换列
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){
                if(i == j)  iden_mat.data[i][j] = 1;
            }
        }
        return iden_mat;
    }
    //矩阵求逆
    Matrix inversion(){
        if (rows != cols){
            cout << "该矩阵不可逆！" << endl;
            return Matrix(0,0);
        }
        else {
            // 构造增广矩阵
            int n = rows;
            Matrix augmented(n, 2 * n);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    augmented.data[i][j] = data[i][j];
                }
                augmented.data[i][i+n] = 1;
            }
            // 初等行变换
            for (int i = 0; i < n; i++) {
                // 将当前列主元素设为1
                double pivot = augmented.getValue(i, i);
                for (int j = i; j < 2 * n; j++) {
                    augmented.data[i][j] = (augmented.getValue(i, j) / pivot);
                }
                // 将当前列其它元素设为0
                for (int k = 0; k < n; k++) {
                if (k != i) {
                    double factor = augmented.getValue(k, i);
                    for (int j = i; j < 2 * n; j++) {
                            augmented.data[k][j] = (augmented.getValue(k, j) - factor * augmented.getValue(i, j));
                        }
                    }
                }
            }
            // 提取逆矩阵
            Matrix inversion(n, n);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    inversion.data[i][j] = (augmented.getValue(i, j + n));
                }
            }
            return inversion;
        }
    }
    // 输出矩阵内容
    void print() const{
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << setw(3) << data[i][j] << " ";     //设置输出寛度
            }
            cout << endl;
        }
    }
};
int main() {
    // 创建两个矩阵并赋值
    int row, col;
    cout << "Please input the Matrix's row and col." << endl;
    cin >> row;
    cin >> col;
    Matrix m1(row, col);
    m1.setValue(row, col);
    Matrix m2(row, col);
    m2.setValue(row, col);
    //Matrix mat(row, col);
    // 输出原矩阵
    cout << "矩阵 m1:" << endl;
    m1.print();
    cout << endl;
    cout << "矩阵 m2:" << endl;
    m2.print();
    cout << endl;
    // 矩阵加法
    cout << "矩阵加法 m1+m2：" << endl;
    Matrix addition = m1 + m2;
    addition.print();
    cout << endl;
    // 矩阵减法
    cout << "矩阵减法 m1-m2：" << endl;
    Matrix subtraction = m1 - m2;
    subtraction.print();
    cout << endl;
    // 矩阵乘法
    cout << "矩阵乘法 m1*m2：" << endl;
    Matrix multiplication = m1 * m2;
    multiplication.print();
    cout << endl;
    // 矩阵求逆
    cout << "矩阵求逆：" << endl;
    Matrix m1_inver = m1.inversion();
    Matrix m2_inver = m2.inversion();
    cout << "矩阵 m1 求逆:" << endl;
    m1_inver.print();
    cout << endl;
    cout << "矩阵 m2 求逆:" << endl;
    m2_inver.print();
    cout << endl;
    system("pause");
}