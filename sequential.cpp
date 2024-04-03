#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib> 
#include <chrono>

using namespace std;

const int SQUARE_MATRIX_SIZE = 1000; // Size of the square matrices

// Function to generate random values for a matrix
void generateRandomMatrix(vector<vector<int>>& matrix) {
    for (int row = 0; row < SQUARE_MATRIX_SIZE; ++row) {
        for (int col = 0; col < SQUARE_MATRIX_SIZE; ++col) {
            // Generating random numbers between 0 and 99
            matrix[row][col] = rand() % 100; 
        }
    }
}

// Function to display a matrix
void displayMatrix(const vector<vector<int>>& matrix) {
    for (int row = 0; row < SQUARE_MATRIX_SIZE; ++row) {
        for (int col = 0; col < SQUARE_MATRIX_SIZE; ++col) {
            cout << matrix[row][col] << " ";
        }
        cout << endl;
    }
}

// Function to perform matrix multiplication
void multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    for (int i = 0; i < SQUARE_MATRIX_SIZE; ++i) {
        for (int j = 0; j < SQUARE_MATRIX_SIZE; ++j) {
            for (int k = 0; k < SQUARE_MATRIX_SIZE; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    // Initialize matrices A, B, and C
    vector<vector<int>> A(SQUARE_MATRIX_SIZE, vector<int>(SQUARE_MATRIX_SIZE));
    vector<vector<int>> B(SQUARE_MATRIX_SIZE, vector<int>(SQUARE_MATRIX_SIZE));
    vector<vector<int>> C(SQUARE_MATRIX_SIZE, vector<int>(SQUARE_MATRIX_SIZE, 0)); // Initialize C with zeros

    // Generate random values for matrices A and B
    generateRandomMatrix(A);
    generateRandomMatrix(B);

    // Display matrices A and B
    cout << "Matrix A:" << endl;
    displayMatrix(A);
    cout << endl << "Matrix B:" << endl;
    displayMatrix(B);

    // Start timing the matrix multiplication process
    auto start = chrono::high_resolution_clock::now();

    // Perform matrix multiplication sequentially
    multiplyMatrices(A, B, C);

    // Stop timing the matrix multiplication process
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    // Output the time taken for matrix multiplication
    cout << endl << "Execution time for matrix multiplication: " << duration.count() << " microseconds" << endl;

    // Write the result matrix to a file
    ofstream outFile("result_sequential.txt");
    if (outFile.is_open()) {
        for (int i = 0; i < SQUARE_MATRIX_SIZE; ++i) {
            for (int j = 0; j < SQUARE_MATRIX_SIZE; ++j) {
                outFile << C[i][j] << " ";
            }
            outFile << endl;
        }
        outFile.close();
        cout << "The resulting matrix has been written to 'result_sequential.txt'" << endl;
    } else {
        cerr << "Unable to open file 'result_sequential.txt'" << endl;
    }

    return 0;
}
