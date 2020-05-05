#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <omp.h>

#include "generator.h"
class size_mismatch : public std::exception{
public:
    const char* what() noexcept{
        return "Size mismatch matrices";
    }
};
struct helper_1{};
struct helper_2{};
extern helper_1 _1;
extern helper_2 _2;
template<
        class T,
        template<class>class GeneratePolicy = num_generator
        >
class matrix{

    T *data;
    int real_size;
    int row,col;
    typedef GeneratePolicy<T> generator;


public:

    int size1()const{ return row; }
    int size2()const{ return col; }


    matrix(int row,int col,T value = T()) : real_size(row*col),row(row),col(col){
        data = new T[real_size];
        std::fill(data,data + real_size,value);
    }

    matrix(const matrix &other) : real_size(other.real_size),row(other.row),col(other.col){
        data = new T[real_size];
        std::copy(other.data,other.data + other.real_size,data);
    }
    matrix(double **other,int m,int n) : real_size(m+n),row(m),col(n){
        data = new T[real_size];
        for(int i = 0;i < m;i++)
            for(int j = 0;j < n;j++)
                this->operator ()(i,j) = other[i][j];
    }
    matrix(){
        data = nullptr;
    }

    matrix& operator=(const matrix &other){
        if(data)free();
        this->real_size = other.real_size;
        this->row = other.row;
        this->col = other.col;
        data = new T[real_size];
        std::copy(other.data,other.data + other.real_size,data);
        return *this;
    }

    matrix operator+(const matrix &other){
        if(other.col != col || other.row != row)
            throw size_mismatch();
        matrix res(row,col);
        for(int i = 0;i < real_size;i++)
            res.data[i] = data[i] + other.data[i];
        return res;
    }

    T& operator()(int i,int j){
        return data[i*col + j];
    }
    std::vector<T> operator ()(int row,helper_2){
        std::vector<T>result;
        for(int i = 0;i < col;i++)
            result.push_back(data[row*col + i]);
        return result;
    }
    std::vector<T> operator ()(helper_1,int col){
        std::vector<T>result;
        for(int i = 0;i < row;i++)
            result.push_back(data[i*this->col + col]);
        return result;
    }
    void resize(int row,int col){
        free();
        real_size = row*col;this->row = row;this->col = col;
        data = new T[real_size];
        clear();
    }

    friend std::ostream& operator<<(std::ostream &out,const matrix &m){
        for(int i = 0;i < m.row;i++)
        {
            for(int j = 0;j < m.col;j++)
                out << std::setw(7) << m.data[i*m.col + j];
            out << std::endl;
        }

        return out;
    }
    static matrix<T> eye(int m){
        matrix<T>result(m,m);
        for(int i = 0;i < m;i++)
            result(i,i) = 1;
        return result;
    }
    static matrix generate(int row,int col){
        matrix m(row,col);
        std::generate(m.data,m.data+m.real_size,&generator::generate);
        return m;
    }
    void clear(){
        std::fill(data,data + real_size,0);
    }
    ~matrix(){
        free();
    }
    static matrix<T>transpose(matrix<T>other){
        matrix<T>result(other.size2() ,other.size1());
        for(int i = 0;i < other.size1();i++)
            for(int j = 0;j < other.size2();j++)
                result(j,i) = other(i,j);
        return result;
    }

private:
    void free(){
        if(data)delete[] data;
    }
};
#endif // MATRIX_H
