#include <iostream>
#include <fstream>
#include <random> // Use <random> for random number generation
#include <chrono> // Include the chrono library for time measurements
#include <omp.h> // Include OpenMP for parallelization

using namespace std;
using namespace std::chrono;

const int MAX_SIZE = 100;

// Function to multiply matrices A and B and store the result in matrix C
void multiplyMatrices(int A[][MAX_SIZE], int B[][MAX_SIZE], int C[][MAX_SIZE], int N) {
    // Parallelize the outer loop using OpenMP
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    // Use random_device to seed the random number generator for better quality randomness
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9); // Distribution from 0 to 9

    int N;
    cout << "Enter the size of the matrices (N): ";
    cin >> N;

    // Check if N is within the maximum size
    if (N <= 0 || N > MAX_SIZE) {
        cout << "Invalid size entered!";
        return 1;
    }

    int A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];

    // Initialize matrices A and B with random values
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = dis(gen); // Generate random value between 0 and 9
            B[i][j] = dis(gen); // Generate random value between 0 and 9
        }
    }

    // Measure execution time in nanoseconds
    auto start = high_resolution_clock::now(); // Start clock

    // Perform matrix multiplication
    multiplyMatrices(A, B, C, N);

    auto stop = high_resolution_clock::now(); // Stop clock
    auto duration = duration_cast<nanoseconds>(stop - start); // Calculate duration

    // Write the result to a file
    ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        cout << "Unable to open file for writing!";
        return 1;
    }

    // Write matrix C to the file
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            outputFile << C[i][j] << " ";
        }
        outputFile << endl;
    }

    outputFile.close();

    cout << "Matrix multiplication result has been written to output.txt\n";
    cout << "Execution time (excluding initialization and file writing): " << duration.count() << " nanoseconds\n";

    return 0;
}
