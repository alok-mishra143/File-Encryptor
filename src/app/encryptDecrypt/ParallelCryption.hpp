#ifndef PARALLEL_CRYPTION_HPP
#define PARALLEL_CRYPTION_HPP

#include <string>
#include <vector>
#include <memory>
#include "../Procesess/ProcessManagement.hpp"

using namespace std;

class ParallelCryption {
public:
    ParallelCryption(size_t numThreads = std::thread::hardware_concurrency());
    ~ParallelCryption();
    
    // Parallel encryption/decryption methods
    bool encryptFile(const string& filePath, size_t chunkSize = 1024 * 1024);
    bool decryptFile(const string& filePath, size_t chunkSize = 1024 * 1024);
    
    // Batch processing methods
    bool encryptMultipleFiles(const vector<string>& filePaths, size_t chunkSize = 1024 * 1024);
    bool decryptMultipleFiles(const vector<string>& filePaths, size_t chunkSize = 1024 * 1024);
    
    // Directory processing methods
    bool encryptDirectory(const string& directoryPath, const string& fileExtension = "", 
                         bool recursive = true, size_t chunkSize = 1024 * 1024);
    bool decryptDirectory(const string& directoryPath, const string& fileExtension = "", 
                         bool recursive = true, size_t chunkSize = 1024 * 1024);
    
    // Performance monitoring
    void printStatistics() const;
    void resetStatistics();
    
private:
    unique_ptr<ProcessManagement> processManager;
    
    // Performance tracking
    mutable mutex statsMutex;
    size_t filesProcessed;
    size_t bytesProcessed;
    chrono::steady_clock::time_point startTime;
    
    // Helper methods
    vector<string> getFilesInDirectory(const string& directoryPath, 
                                     const string& fileExtension, 
                                     bool recursive) const;
    void updateStats(size_t fileSize);
};

#endif