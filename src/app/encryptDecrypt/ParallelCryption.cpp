#include "ParallelCryption.hpp"
#include <iostream>
#include <filesystem>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::filesystem;
using namespace std::chrono;

ParallelCryption::ParallelCryption(size_t numThreads) 
    : processManager(make_unique<ProcessManagement>(numThreads)),
      filesProcessed(0),
      bytesProcessed(0) {
    resetStatistics();
    cout << "ParallelCryption initialized with " << numThreads << " threads" << endl;
}

ParallelCryption::~ParallelCryption() {
    if(processManager) {
        processManager->shutdown();
    }
}

bool ParallelCryption::encryptFile(const string& filePath, size_t chunkSize) {
    cout << "Encrypting file: " << filePath << endl;
    
    try {
        if(!exists(filePath)) {
            cerr << "File does not exist: " << filePath << endl;
            return false;
        }
        
        size_t fileSize = file_size(filePath);
        updateStats(fileSize);
        
        bool result = processManager->encryptFileParallel(filePath, chunkSize);
        
        if(result) {
            lock_guard<mutex> lock(statsMutex);
            filesProcessed++;
            cout << "Successfully encrypted: " << filePath << endl;
        } else {
            cerr << "Failed to encrypt: " << filePath << endl;
        }
        
        return result;
        
    } catch(const exception& e) {
        cerr << "Error encrypting file " << filePath << ": " << e.what() << endl;
        return false;
    }
}

bool ParallelCryption::decryptFile(const string& filePath, size_t chunkSize) {
    cout << "Decrypting file: " << filePath << endl;
    
    try {
        if(!exists(filePath)) {
            cerr << "File does not exist: " << filePath << endl;
            return false;
        }
        
        size_t fileSize = file_size(filePath);
        updateStats(fileSize);
        
        bool result = processManager->decryptFileParallel(filePath, chunkSize);
        
        if(result) {
            lock_guard<mutex> lock(statsMutex);
            filesProcessed++;
            cout << "Successfully decrypted: " << filePath << endl;
        } else {
            cerr << "Failed to decrypt: " << filePath << endl;
        }
        
        return result;
        
    } catch(const exception& e) {
        cerr << "Error decrypting file " << filePath << ": " << e.what() << endl;
        return false;
    }
}

bool ParallelCryption::encryptMultipleFiles(const vector<string>& filePaths, size_t chunkSize) {
    cout << "Starting batch encryption of " << filePaths.size() << " files" << endl;
    
    vector<future<bool>> futures;
    
    // Launch parallel encryption tasks
    for(const auto& filePath : filePaths) {
        auto future = async(launch::async, [this, filePath, chunkSize]() {
            return encryptFile(filePath, chunkSize);
        });
        futures.push_back(move(future));
    }
    
    // Wait for all tasks to complete
    bool allSuccess = true;
    for(auto& future : futures) {
        if(!future.get()) {
            allSuccess = false;
        }
    }
    
    cout << "Batch encryption " << (allSuccess ? "completed successfully" : "completed with errors") << endl;
    return allSuccess;
}

bool ParallelCryption::decryptMultipleFiles(const vector<string>& filePaths, size_t chunkSize) {
    cout << "Starting batch decryption of " << filePaths.size() << " files" << endl;
    
    vector<future<bool>> futures;
    
    // Launch parallel decryption tasks
    for(const auto& filePath : filePaths) {
        auto future = async(launch::async, [this, filePath, chunkSize]() {
            return decryptFile(filePath, chunkSize);
        });
        futures.push_back(move(future));
    }
    
    // Wait for all tasks to complete
    bool allSuccess = true;
    for(auto& future : futures) {
        if(!future.get()) {
            allSuccess = false;
        }
    }
    
    cout << "Batch decryption " << (allSuccess ? "completed successfully" : "completed with errors") << endl;
    return allSuccess;
}

bool ParallelCryption::encryptDirectory(const string& directoryPath, const string& fileExtension, 
                                       bool recursive, size_t chunkSize) {
    cout << "Encrypting directory: " << directoryPath << endl;
    
    try {
        auto files = getFilesInDirectory(directoryPath, fileExtension, recursive);
        if(files.empty()) {
            cout << "No files found to encrypt in directory: " << directoryPath << endl;
            return true;
        }
        
        return encryptMultipleFiles(files, chunkSize);
        
    } catch(const exception& e) {
        cerr << "Error encrypting directory " << directoryPath << ": " << e.what() << endl;
        return false;
    }
}

bool ParallelCryption::decryptDirectory(const string& directoryPath, const string& fileExtension, 
                                       bool recursive, size_t chunkSize) {
    cout << "Decrypting directory: " << directoryPath << endl;
    
    try {
        auto files = getFilesInDirectory(directoryPath, fileExtension, recursive);
        if(files.empty()) {
            cout << "No files found to decrypt in directory: " << directoryPath << endl;
            return true;
        }
        
        return decryptMultipleFiles(files, chunkSize);
        
    } catch(const exception& e) {
        cerr << "Error decrypting directory " << directoryPath << ": " << e.what() << endl;
        return false;
    }
}

vector<string> ParallelCryption::getFilesInDirectory(const string& directoryPath, 
                                                    const string& fileExtension, 
                                                    bool recursive) const {
    vector<string> files;
    
    try {
        if(recursive) {
            for(const auto& entry : recursive_directory_iterator(directoryPath)) {
                if(entry.is_regular_file()) {
                    string filePath = entry.path().string();
                    if(fileExtension.empty() || filePath.ends_with(fileExtension)) {
                        files.push_back(filePath);
                    }
                }
            }
        } else {
            for(const auto& entry : directory_iterator(directoryPath)) {
                if(entry.is_regular_file()) {
                    string filePath = entry.path().string();
                    if(fileExtension.empty() || filePath.ends_with(fileExtension)) {
                        files.push_back(filePath);
                    }
                }
            }
        }
        
        cout << "Found " << files.size() << " files to process" << endl;
        
    } catch(const exception& e) {
        cerr << "Error scanning directory " << directoryPath << ": " << e.what() << endl;
    }
    
    return files;
}

void ParallelCryption::updateStats(size_t fileSize) {
    lock_guard<mutex> lock(statsMutex);
    bytesProcessed += fileSize;
}

void ParallelCryption::printStatistics() const {
    lock_guard<mutex> lock(statsMutex);
    
    auto now = steady_clock::now();
    auto elapsed = duration_cast<milliseconds>(now - startTime);
    
    cout << "\n=== Parallel Cryption Statistics ===" << endl;
    cout << "Files processed: " << filesProcessed << endl;
    cout << "Bytes processed: " << bytesProcessed << " (" << 
            (bytesProcessed / 1024.0 / 1024.0) << " MB)" << endl;
    cout << "Time elapsed: " << elapsed.count() << " ms" << endl;
    
    if(elapsed.count() > 0) {
        double throughputMBps = (bytesProcessed / 1024.0 / 1024.0) / (elapsed.count() / 1000.0);
        cout << "Throughput: " << throughputMBps << " MB/s" << endl;
    }
    
    cout << "=================================" << endl;
}

void ParallelCryption::resetStatistics() {
    lock_guard<mutex> lock(statsMutex);
    filesProcessed = 0;
    bytesProcessed = 0;
    startTime = steady_clock::now();
}