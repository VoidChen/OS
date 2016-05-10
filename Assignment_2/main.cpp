/*
 * Operating Systems Programming Assignment 1
 */
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "matrix.h"
#define ROWS 500
#define COLS 1000
using namespace std;
using namespace std::chrono;

int main(){
    srand(time(NULL));
    high_resolution_clock::time_point start, end;

    //Initial matrix
    Matrix *M, *N, *O, *P, *R, *R2;
    M = new Matrix(ROWS, COLS, true);
    N = new Matrix(COLS, ROWS, true);
    O = new Matrix(ROWS, COLS, true);
    P = new Matrix(COLS, ROWS, true);

    // Single-thread approach test
    start = high_resolution_clock::now();
    R = addition(multiplication_st(M, N), multiplication_st(O, P));
    end = high_resolution_clock::now();
    cout << "Single-thread approach execution time: " << fixed << duration_cast<duration<double>>(end-start).count() << " seconds." << endl;
    //R->print();

    // Multiple-thread approach test
    start = high_resolution_clock::now();
    R2 = addition(multiplication_mt(M, N), multiplication_mt(O, P));
    end = high_resolution_clock::now();
    cout << "Multiple-thread approach execution time: " << fixed << duration_cast<duration<double>>(end-start).count() << " seconds." << endl;
    //R2->print();

    return 0;
}
