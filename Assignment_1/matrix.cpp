#include<iostream>
#include<thread>
#include<cstdlib>
#include<ctime>
#define ROWS 200
#define COLS 10000
#define RANDOM_ELEMENT_MOD 1000
using namespace std;

// Matrix class
class Matrix{
    public:
        int row;
        int col;
        int **save;

        // Init matrix
        Matrix(int row, int col, bool random_fill = false){
            this->row = row;
            this->col = col;
            save = new int*[row];
            for(int i = 0; i < row; ++i){
                save[i] = new int[col];
                for(int j = 0; j < col; ++j){
                    save[i][j] = random_fill ? rand()%RANDOM_ELEMENT_MOD : 0;
                }
            }
        }

        // Print matrix
        void print(){
            for(int i = 0; i < row; ++i){
                for(int j = 0; j < col; ++j)
                    cout << save[i][j] <<" ";
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
void calc(Matrix *a, Matrix *b, Matrix *result, int row){
    for(int i = 0; i < b->col; ++i){
        result->save[row][i] = 0;
        for(int j = 0; j < a->col; ++j)
            result->save[row][i] += a->save[row][j] * b->save[j][i];
    }
}

// Matrix multiplication (Multiple-thread approach)
Matrix* multiplication_mt(Matrix *a, Matrix *b){
    Matrix *result = new Matrix(a->row, b->col);
    thread *thread_list = new thread[a->row];

    //Create thread
    for(int i = 0; i < a->row; ++i)
        thread_list[i] = thread(calc, a, b, result, i);

    //Wait thread complete
    for(int i = 0; i < a->row; ++i)
        thread_list[i].join();

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

int main(){
    srand(time(NULL));
    clock_t start, end;

    //Init matrix
    Matrix *M, *N, *O, *P, *R;
    M = new Matrix(ROWS, COLS, true);
    N = new Matrix(COLS, ROWS, true);
    O = new Matrix(ROWS, COLS, true);
    P = new Matrix(COLS, ROWS, true);

    // Single-thread approach test
    start = clock();
    R = addition(multiplication_st(M, N), multiplication_st(O, P));
    end = clock();
    cout << "Single-thread approach Used time: " << fixed << (double)(end-start)/CLOCKS_PER_SEC << " second." << endl;

    // Multiple-thread approach test
    start = clock();
    R = addition(multiplication_mt(M, N), multiplication_mt(O, P));
    end = clock();
    cout << "Multiple-thread approach used time: " << fixed << (double)(end-start)/CLOCKS_PER_SEC << " second." << endl;

    cout << "Hardware thread contexts: " << thread::hardware_concurrency() << endl;
    return 0;
}
