#include<iostream>
#include<cstdlib>
#include<ctime>
#define ROWS 500
#define COLS 1000
#define RANDOM_ELEMENT_MOD 1000
using namespace std;

class Matrix{
    public:
        int row;
        int col;
        int **save;

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

        void print(){
            for(int i = 0; i < row; ++i){
                for(int j = 0; j < col; ++j)
                    cout << save[i][j] <<" ";
                cout << endl;
            }
        }

};

Matrix* multiplication(Matrix *a, Matrix *b){
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

    Matrix *M, *N, *O, *P, *R;
    M = new Matrix(ROWS, COLS, true);
    N = new Matrix(COLS, ROWS, true);
    O = new Matrix(ROWS, COLS, true);
    P = new Matrix(COLS, ROWS, true);

    clock_t start, end;
    start = clock();
    R = addition(multiplication(M, N), multiplication(O, P));
    end = clock();

    cout << "Used time: " << (double)(end-start)/CLOCKS_PER_SEC << " second." <<endl;

    return 0;
}
