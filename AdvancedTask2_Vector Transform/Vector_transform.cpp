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
        data = new double*[rows];  //先构建一维数組
        for (int i = 0; i < rows; i++) {    
            data[i] = new double[cols] {0};    //再逐个置入数组变成二维并賦初值0
        }
    }
    // 设置矩阵元素
    void setValue(int row, int col, double value) {
        if (row >= 0 && col >= 0) {
            data[row][col] = value;
        }
    }
    // 获取矩阵元素
    double getValue(int row, int col) {
        if (row >= 0 && col >= 0 ) {
            return data[row][col];
        }
        return 0; // 非法输入返回0
    }
    // 矩阵加法
    Matrix operator+(const Matrix& other) const { //第一個參數存在隱形的this對象
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
    // 输出矩阵内容
    void print() const{
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << setw(7) << data[i][j] << " ";     //设置輸出寛度
            }
            cout << endl;
        }
    }
};
int main() {
    // 创建旋轉R, 平移t和向量a三个矩阵并赋值
    Matrix a(3,1);
    a.setValue(0, 0, 29);
    a.setValue(1, 0, 6);
    a.setValue(2, 0, 2.5);
    Matrix R(3,3);
    R.setValue(0, 0, 0.22);
    R.setValue(0, 1, -0.43);
    R.setValue(0, 2, 0.88);
    R.setValue(1, 0, 0.62);
    R.setValue(1, 1, 0.75);
    R.setValue(1, 2, 0.22);
    R.setValue(2, 0, -0.75);
    R.setValue(2, 1, 0.5);
    R.setValue(2, 2, 0.43);
    Matrix t(3,1);
    t.setValue(0, 0, -1);
    t.setValue(1, 0, 9);
    t.setValue(2, 0, 2.5);
    // 输出原矩阵
    cout << "向量a:" << endl;
    a.print();
    cout << endl;
    cout << "旋轉R:" << endl;
    R.print();
    cout << endl;
    cout << "平移t:" << endl;
    t.print();
    cout << endl;
    
    //把向量a轉化為齊次坐标形式a_homo
    Matrix a_homo(4,1);
    for(int i = 0; i < 3; i++){
        a_homo.setValue(i,0,a.getValue(i,0));
    }
    a_homo.setValue(3,0,1);

    //构建R的"齊次坐标"形式便於后續的R*a运算
    Matrix R_homo(4,4);
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            R_homo.setValue(i,j,R.getValue(i,j));
        }
    }
    R_homo.setValue(3,3,1);

    //构建t的"齊次坐标"形式便於后續的a+t运算
    Matrix t_homo(4,4);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(i == j) t_homo.setValue(i,j,1);
            if(j == 3 && i < 3) t_homo.setValue(i,j,t.getValue(i,0));
        }
    }
    Matrix t_1(4,1);
    for(int i = 0; i < 3; i++){
        t_1.setValue(i,0,t.getValue(i,0));
    }

    Matrix a_rotated = R_homo * a_homo;
    cout << "a經過旋轉R變換：\n" << "[a_rotated] = [R] * [a_homo] = " << endl;
    R_homo.print();
    cout << "*" << endl;
    a_homo.print();
    cout << "=" << endl;
    a_rotated.print();
    cout << endl;

    Matrix a_transformed = t_homo * a_rotated;
    cout << "a_rotated經過平移t變換：\n" << "PS:嚴格來說實际上是在进行齊次變換后的 t_homo * a_rotated 运算：" << "[a_transformed] = [a_rotated] + [t] = " << endl;
    a_rotated.print();
    cout << "+" << endl;
    t_1.print();
    cout << "=" << endl;
    t_homo.print();
    cout << "*" << endl;
    a_rotated.print();
    cout << "=" << endl;
    a_transformed.print();
    cout << endl;

    //將結果从齊次坐标恢复到三維坐标
    for(int i = 0; i < 3; i++)
        a.setValue(i,0,a_transformed.getValue(i,0));

    cout << "最終运算結果：\na' =" << endl;
    a.print();
    cout << endl;
    return 0;
}

//在这个实现中，我们创建了一个Matrix类，包含了矩阵的基本操作和运算功能。其中，数据成员有行数rows、列数cols和一个二维指针data用于存储矩阵的元素。在构造函数中，我们动态申请了内存空间来存储矩阵元素，并在析构函数中释放了这些内存。
//我们实现了setValue和getValue方法来设置和获取矩阵的元素。重载了+、-和*运算符，实现了矩阵的加法、减法和乘法运算。在运算过程中，我们检查了两个矩阵的维度是否匹配，如果不匹配则输出错误信息并返回一个空的Matrix对象。通过print方法，我们可以按矩阵的形状输出矩阵的内容。
//在主函数中，我们创建了两个3x3的矩阵m1和m2，并使用setValue方法给矩阵赋值。然后分别输出原矩阵m1和m2的内容。接着进行矩阵的加法、减法和乘法运算，并将结果分别存储在addition、subtraction和multiplication对象中。最后依次输出运算结果的内容。
//这样，我们就可以通过运行代码来验证矩阵类的功能和运算结果。