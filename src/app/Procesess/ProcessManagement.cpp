#include "ProcessManagement.hpp"
#include <iostream>
#include <cstring>
#include <memory>
#include <thread>
#include <future>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include "../fileHanding/ReadENV.cpp"  // Include ReadENV implementation

//  this is for Windows
#define NOMINMAX
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#undef byte  // Resolve conflict with std::byte

// Using C++ threads for cross-platform parallel processing

#include "../encryptDecrypt/Cryption.hpp"

using namespace std;
#define endl "\n"

ProcessManagement::ProcessManagement(size_t numThreads) : stop(false) {
    // Create worker threads
    for(size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ProcessManagement::workerThread, this);
    }
    cout << "ProcessManagement initialized with " << numThreads << " worker threads" << endl;
}

ProcessManagement::~ProcessManagement() {
    shutdown();
}

void ProcessManagement::shutdown() {
    {
        unique_lock<mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();
    
    for(thread &worker : workers) {
        if(worker.joinable()) {
            worker.join();
        }
    }
    cout << "ProcessManagement shutdown complete" << endl;
}

void ProcessManagement::workerThread() {
    while(true) {
        unique_ptr<Task> task;
        
        {
            unique_lock<mutex> lock(queueMutex);
            condition.wait(lock, [this]{ return stop || !taskQueue.empty(); });
            
            if(stop && taskQueue.empty()) {
                return;
            }
            
            if(!taskQueue.empty()) {
                task = move(taskQueue.front());
                taskQueue.pop();
            }
        }
        
        if(task) {
            cout << "Worker thread executing task: " << task->toString() << endl;
            executeCryption(task->toString());
            cout << "Worker thread completed task" << endl;
        }
    }
}

bool ProcessManagement::isProcessing() const {
    lock_guard<mutex> lock(queueMutex);
    return !taskQueue.empty();
}
bool ProcessManagement::submitToQueue(unique_ptr<Task> task){
    {
        unique_lock<mutex> lock(queueMutex);
        if(stop) {
            return false;
        }
        taskQueue.push(move(task));
    }
    condition.notify_one();
    cout << "Task submitted to queue" << endl;
    return true;
}

bool ProcessManagement::submitParallelTask(unique_ptr<Task> task) {
    return submitToQueue(move(task));
}

void ProcessManagement::executeTasks(){
    while(!taskQueue.empty()){
        unique_ptr<Task>taskToExecute = move(taskQueue.front());
        taskQueue.pop();

        cout<<"Executing Task: "<<taskToExecute->toString()<<endl;

        //!todo implement the actual encryption/decryption logic here

        executeCryption(taskToExecute->toString());

    }
}

// Parallel file encryption
bool ProcessManagement::encryptFileParallel(const string& filePath, size_t chunkSize) {
    cout << "Starting parallel encryption of: " << filePath << endl;
    
    auto futures = processFileInChunks(filePath, Action::ENCRYPT, chunkSize);
    
    // Wait for all chunks to complete
    bool success = true;
    for(auto& future : futures) {
        if(!future.get()) {
            success = false;
        }
    }
    
    cout << "Parallel encryption " << (success ? "completed successfully" : "failed") << endl;
    return success;
}

// Parallel file decryption
bool ProcessManagement::decryptFileParallel(const string& filePath, size_t chunkSize) {
    cout << "Starting parallel decryption of: " << filePath << endl;
    
    auto futures = processFileInChunks(filePath, Action::DECRYPT, chunkSize);
    
    // Wait for all chunks to complete
    bool success = true;
    for(auto& future : futures) {
        if(!future.get()) {
            success = false;
        }
    }
    
    cout << "Parallel decryption " << (success ? "completed successfully" : "failed") << endl;
    return success;
}

// Process file in parallel chunks
vector<future<bool>> ProcessManagement::processFileInChunks(const string& filePath, Action action, size_t chunkSize) {
    vector<future<bool>> futures;
    
    try {
        // Get file size
        ifstream file(filePath, ios::binary | ios::ate);
        if(!file.is_open()) {
            throw runtime_error("Cannot open file: " + filePath);
        }
        
        size_t fileSize = file.tellg();
        file.close();
        
        if(fileSize == 0) {
            cout << "Warning: File is empty: " << filePath << endl;
            return futures;
        }
        
        // Read environment key
        ReadEnv env;
        string envKey = env.getEnv();
        int key = stoi(envKey);
        
        cout << "Processing file of size " << fileSize << " bytes in chunks of " << chunkSize << " bytes" << endl;
        
        // Create tasks for each chunk
        for(size_t offset = 0; offset < fileSize; offset += chunkSize) {
            size_t currentChunkSize = min(chunkSize, fileSize - offset);
            
            // Launch async task for this chunk
            auto future = async(launch::async, [this, filePath, offset, currentChunkSize, action, key]() {
                return processChunk(filePath, offset, currentChunkSize, action, key);
            });
            
            futures.push_back(move(future));
        }
        
        cout << "Created " << futures.size() << " parallel processing tasks" << endl;
        
    } catch(const exception& e) {
        cerr << "Error processing file in chunks: " << e.what() << endl;
    }
    
    return futures;
}

// Process a specific chunk of the file
bool ProcessManagement::processChunk(const string& filePath, size_t start, size_t size, Action action, int key) {
    try {
        // Open file for reading and writing
        fstream file(filePath, ios::binary | ios::in | ios::out);
        if(!file.is_open()) {
            cerr << "Cannot open file for chunk processing: " << filePath << endl;
            return false;
        }
        
        // Seek to the chunk start position
        file.seekg(start);
        if(file.fail()) {
            cerr << "Failed to seek to position " << start << endl;
            return false;
        }
        
        // Read the chunk
        vector<char> buffer(size);
        file.read(buffer.data(), size);
        streamsize bytesRead = file.gcount();
        
        if(bytesRead == 0) {
            cout << "Warning: No bytes read from chunk at position " << start << endl;
            return true;
        }
        
        // Process the chunk based on action
        for(streamsize i = 0; i < bytesRead; ++i) {
            if(action == Action::ENCRYPT) {
                buffer[i] = (buffer[i] + key) % 256;
            } else {
                buffer[i] = (buffer[i] - key + 256) % 256;
            }
        }
        
        // Write the processed chunk back
        file.seekp(start);
        file.write(buffer.data(), bytesRead);
        file.flush();
        
        if(file.fail()) {
            cerr << "Failed to write processed chunk at position " << start << endl;
            return false;
        }
        
        cout << "Successfully processed chunk: " << start << "-" << (start + bytesRead - 1) << 
                " (" << bytesRead << " bytes)" << endl;
        
        return true;
        
    } catch(const exception& e) {
        cerr << "Error processing chunk at position " << start << ": " << e.what() << endl;
        return false;
    }
}

