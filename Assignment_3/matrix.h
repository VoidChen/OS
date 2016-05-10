/*
 * Operating Systems Programming Assignment 3
 * Matrix
 */
#include <iostream>
#include <cstdlib>
#include "threadpool.h"
using namespace std;

// Matrix class
class Matrix{
    public:
        int row;
        int col;
        double **save;

        // Initial matrix
        Matrix(int row, int col, bool random_fill = false){
            this->row = row;
            this->col = col;
            save = new double*[row];
            for(int i = 0; i < row; ++i){
                save[i] = new double[col];
                for(int j = 0; j < col; ++j)
                    save[i][j] = random_fill ? (double)rand()/RAND_MAX : 0;
            }
        }

        // Print matrix
        void print(){
            for(int i = 0; i < row; ++i){
                for(int j = 0; j < col; ++j)
                    cout << save[i][j] << " ";
                cout << endl;
            }
        }
};

// Matrix multiplication (Single-thread approach)
Matrix* multiplication_st(Matrix *a, Matrix *b){
    Matrix *result = new Matrix(a->row, b->col);
    for(int i = 0; i < a->row; ++i){
        for(int j = 0; j < b->col; ++j){
            result->save[i][j] = 0;
            for(int k = 0; k < a->col; ++k)
                result->save[i][j] += a->save[i][k] * b->save[k][j];
        }
    }
    return result;
}

// Calculate row
class CalcRow: public Job{
    private:
        Matrix *a;
        Matrix *b;
        Matrix *result;
        int row;

    public:
        CalcRow(Matrix *a, Matrix *b, Matrix *result, int row){
            this->a = a;
            this->b = b;
            this->result = result;
            this->row = row;
        }

        void run(){
            for(int i = 0; i < b->col; ++i){
                result->save[row][i] = 0;
                for(int j = 0; j < a->col; ++j)
                    result->save[row][i] += a->save[row][j] * b->save[j][i];
            }
        }
};

// Matrix multiplication (Multiple-thread approach)
Matrix* multiplication_mt(Matrix *a, Matrix *b){
    Matrix *result = new Matrix(a->row, b->col);

    //Create thread pool
    Pool *pool = new Pool(4);

    //Submit jobs
    for(int i = 0; i < a->row; ++i)
        pool->submit(new CalcRow(a, b, result, i));

    //Wait jobs complete
    for(int i = 0; i < a->row; ++i)
        pool->job_join(i);

    return result;
}

// Matrix addition
Matrix* addition(Matrix *a, Matrix *b){
    Matrix *result = new Matrix(a->row, a->col);
    for(int i = 0; i < a->row; ++i){
        for(int j = 0; j < a->col; ++j)
            result->save[i][j] = a->save[i][j] + b->save[i][j];
    }
    return result;
}
