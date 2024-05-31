//
// Created by vinee on 31.05.24.
//
#include <iostream>
#include <vector>
#include <thread>
#include <functional>

/*

Methods and Concepts Used
        std::vector: Used to manage dynamic arrays.

Methods: std::vector::emplace_back, std::vector::size, std::vector::operator[]
std::thread: Used to create and manage threads.

emplace_back: Adds a new element to the end of the vector

Methods: std::thread::join, std::thread::emplace_back (via std::vector)
Range-based For Loop: Used to iterate through elements of the std::vector.

Syntax: for (auto& t : threads)
Lambda Expressions (for passing the function to the threads)

Instead of lambdas, we used a named function parallel_for.
Multithreading Synchronization:

Concepts: Creating threads to perform tasks concurrently and joining them to synchronize.
*/


// Function to perform work on a segment of the array
void parallel_for(int start, int end, std::vector<int>& data) {
    for (int i = start; i < end; ++i) {
        // Simulate some work on data[i]
        data[i] *= data[i]; // Example: square the element
    }
}

int main() {
    const int num_threads = 4; // Number of threads
    const int data_size = 100; // Size of the data array
    std::vector<int> data(data_size);

    // Initialize the data array
    for (int i = 0; i < data_size; ++i) {
        data[i] = i;
    }

    // Calculate the size of each chunk
    int chunk_size = data_size / num_threads;

    // Vector to hold the threads
    std::vector<std::thread> threads;

    // Launch threads to perform the work
    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? data_size : start + chunk_size;
        threads.emplace_back(parallel_for, start, end, std::ref(data));
    }

    // Join the threads with the main thread
    for (auto& t : threads) {
        t.join();
    }

    // Print the results
    for (int i = 0; i < data_size; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
