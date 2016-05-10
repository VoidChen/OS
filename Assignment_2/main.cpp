/*
 * Operating Systems Programming Assignment 2
 * Main
 */
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "matrix.h"
#define ROWS 1000
#define COLS 1000
using namespace std;
using namespace std::chrono;

int main(){
    srand(time(NULL));
    high_resolution_clock::time_point start, end;

    //Initial matrix
    Matrix *M, *N, *O, *P, *R;
    M = new Matrix(ROWS, COLS, true);
    N = new Matrix(COLS, ROWS, true);
    O = new Matrix(ROWS, COLS, true);
    P = new Matrix(COLS, ROWS, true);

    // Single-thread approach test
    start = high_resolution_clock::now();
    R = addition(multiplication_st(M, N), multiplication_st(O, P));
    end = high_resolution_clock::now();
    cout << "Single-thread approach execution time: " << fixed << duration_cast<duration<double>>(end-start).count() << " seconds." << endl;

    // Multiple-thread approach test
    start = high_resolution_clock::now();
    R = addition(multiplication_mt(M, N), multiplication_mt(O, P));
    end = high_resolution_clock::now();
    cout << "Multiple-thread approach execution time: " << fixed << duration_cast<duration<double>>(end-start).count() << " seconds." << endl;

    return 0;
}
