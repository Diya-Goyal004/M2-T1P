#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <vector>
#include <chrono>
#include <pthread.h>

using namespace std;

// Size of the square matrices
const int SQUARE_MATRIX_SIZE = 100; 
// Number of threads for parallel computation
const int NUM_THREADS = 2;    

// Struct to hold data for each thread
struct MatrixThreadInfo {
    // Pointer to matrix A
    vector<vector<int>>* A; 
    // Pointer to matrix B
    vector<vector<int>>* B; 
    // Pointer to result matrix C
    vector<vector<int>>* C;  
    // Index of the first row to compute
    int firstRow; 
    // Index of the last row to compute
    int lastRow;              
};

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

// Function executed by each thread to compute dot products
void* calculateDotProducts(void* arg) {
    MatrixThreadInfo* data = static_cast<MatrixThreadInfo*>(arg);

    // Loop through the assigned rows and compute dot products
    for (int i = data->firstRow; i < data->lastRow; ++i) {
        for (int j = 0; j < SQUARE_MATRIX_SIZE; ++j) {
            for (int k = 0; k < SQUARE_MATRIX_SIZE; ++k) {
                (*data->C)[i][j] += (*data->A)[i][k] * (*data->B)[k][j];
            }
        }
    }

    return nullptr;
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

    // Initialize pthread variables
    pthread_t threads[NUM_THREADS];
    MatrixThreadInfo threadData[NUM_THREADS];
    int rowsPerThread = SQUARE_MATRIX_SIZE / NUM_THREADS;

    // Create threads for parallel computation
    for (int i = 0; i < NUM_THREADS; ++i) {
        threadData[i].A = &A;
        threadData[i].B = &B;
        threadData[i].C = &C;
        threadData[i].firstRow = i * rowsPerThread;
        threadData[i].lastRow = (i == NUM_THREADS - 1) ? SQUARE_MATRIX_SIZE : (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, calculateDotProducts, &threadData[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Stop timing the matrix multiplication process
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    // Output the time taken for matrix multiplication
    cout << endl << "Execution time for matrix multiplication: " << duration.count() << " microseconds" << endl;

    // Write the result matrix to a file
    ofstream outFile("result.txt");
    if (outFile.is_open()) {
        for (int i = 0; i < SQUARE_MATRIX_SIZE; ++i) {
            for (int j = 0; j < SQUARE_MATRIX_SIZE; ++j) {
                outFile << C[i][j] << " ";
            }
            outFile << endl;
        }
        outFile.close();
        cout << "The resulting matrix has been written to 'result.txt'" << endl;
    } else {
        cerr << "Unable to open file 'result.txt'" << endl;
    }

    return 0;
}
