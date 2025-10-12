#include "ParallelCryption.hpp"
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

void demonstrateParallelEncryption() {
    cout << "\n=== Parallel File Encryption/Decryption Demo ===" << endl;
    
    // Initialize with 4 threads (or hardware concurrency)
    ParallelCryption crypto(4);
    
    // Example 1: Single file encryption/decryption
    cout << "\n--- Single File Processing ---" << endl;
    string testFile = "test/test1.txt";
    
    if(crypto.encryptFile(testFile)) {
        cout << "File encrypted successfully!" << endl;
        
        // Decrypt it back
        if(crypto.decryptFile(testFile)) {
            cout << "File decrypted successfully!" << endl;
        }
    }
    
    // Example 2: Multiple files processing
    cout << "\n--- Multiple Files Processing ---" << endl;
    vector<string> files = {
        "test/test1.txt"
        // Add more files here if available
    };
    
    crypto.encryptMultipleFiles(files);
    crypto.decryptMultipleFiles(files);
    
    // Example 3: Directory processing
    cout << "\n--- Directory Processing ---" << endl;
    string testDir = "test";
    
    // Encrypt all .txt files in test directory
    crypto.encryptDirectory(testDir, ".txt", false); // non-recursive
    crypto.decryptDirectory(testDir, ".txt", false);
    
    // Print performance statistics
    crypto.printStatistics();
}

void comparePerformance() {
    cout << "\n=== Performance Comparison ===" << endl;
    
    string testFile = "test/test1.txt";
    
    // Test with different chunk sizes
    vector<size_t> chunkSizes = {
        1024,        // 1KB
        64 * 1024,   // 64KB
        1024 * 1024, // 1MB
        4 * 1024 * 1024 // 4MB
    };
    
    for(size_t chunkSize : chunkSizes) {
        cout << "\nTesting with chunk size: " << (chunkSize / 1024) << " KB" << endl;
        
        ParallelCryption crypto(4);
        
        auto start = chrono::high_resolution_clock::now();
        crypto.encryptFile(testFile, chunkSize);
        crypto.decryptFile(testFile, chunkSize);
        auto end = chrono::high_resolution_clock::now();
        
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Processing time: " << duration.count() << " ms" << endl;
        
        crypto.printStatistics();
    }
}

int main() {
    try {
        demonstrateParallelEncryption();
        comparePerformance();
        
    } catch(const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}