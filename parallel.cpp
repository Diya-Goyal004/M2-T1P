#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

const int MAX_SIZE = 100;

// Structure to hold data for each thread
struct ThreadData {
    int thread_id;
    int N;
    int (*A)[MAX_SIZE];
    int (*B)[MAX_SIZE];
    int (*C)[MAX_SIZE];
};

// Function executed by each thread to perform matrix multiplication
void* multiplyMatrices(void* arg) {
    // Cast argument to ThreadData pointer
    ThreadData* data = static_cast<ThreadData*>(arg);

    // Determine the range of rows to be processed by this thread
    int start_row = (data->thread_id * data->N) / 4;
    int end_row = ((data->thread_id + 1) * data->N) / 4;

    // Perform matrix multiplication
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < data->N; ++j) {
            data->C[i][j] = 0;
            for (int k = 0; k < data->N; ++k) {
                data->C[i][j] += data->A[i][k] * data->B[k][j];
            }
        }
    }

    // Exit the thread
    pthread_exit(nullptr);
}

int main() {
    // Random number generation
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9);

    int N;
    cout << "Enter the size of the matrices (N): ";
    cin >> N;

    // Validate matrix size
    if (N <= 0 || N > MAX_SIZE) {
        cout << "Invalid size entered!";
        return 1;
    }

    int A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];

    // Initialize matrices A and B with random values
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = dis(gen);
            B[i][j] = dis(gen);
        }
    }

    // Thread data and thread creation
    pthread_t threads[4];
    ThreadData thread_data[4];
    
    auto start = high_resolution_clock::now();

    // Create threads
    for (int i = 0; i < 4; ++i) {
        thread_data[i].thread_id = i;
        thread_data[i].N = N;
        thread_data[i].A = A;
        thread_data[i].B = B;
        thread_data[i].C = C;

        pthread_create(&threads[i], nullptr, multiplyMatrices, &thread_data[i]);
    }

    // Join threads
    for (int i = 0; i < 4; ++i) {
        pthread_join(threads[i], nullptr);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);

    // Write matrix C to the file
    ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        cout << "Unable to open file for writing!";
        return 1;
    }
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
