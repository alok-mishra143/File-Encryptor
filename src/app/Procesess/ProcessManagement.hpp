#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include "Task.hpp"
#include<queue>
#include<memory>
#include<thread>
#include<vector>
#include<mutex>
#include<condition_variable>
#include<atomic>
#include<future>

class ProcessManagement {
     
    public:
    ProcessManagement(size_t numThreads = std::thread::hardware_concurrency());
    ~ProcessManagement();
    
    bool submitToQueue(unique_ptr<Task> task);
    void executeTasks();
    
    // New parallel processing methods
    bool submitParallelTask(unique_ptr<Task> task);
    void shutdown();
    bool isProcessing() const;
    
    // Parallel file encryption/decryption
    bool encryptFileParallel(const string& filePath, size_t chunkSize = 1024 * 1024); // 1MB chunks
    bool decryptFileParallel(const string& filePath, size_t chunkSize = 1024 * 1024);

    private:
    queue<unique_ptr<Task>> taskQueue;
    
    // Thread pool components
    vector<thread> workers;
    mutable mutex queueMutex;  // Made mutable for const functions
    condition_variable condition;
    atomic<bool> stop;
    
    // Worker thread function
    void workerThread();
    
    // Parallel processing helper methods
    vector<future<bool>> processFileInChunks(const string& filePath, Action action, size_t chunkSize);
    bool processChunk(const string& filePath, size_t start, size_t size, Action action, int key);
};

#endif