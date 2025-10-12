# Parallel File Encryption/Decryption Implementation

## Overview

Successfully implemented a comprehensive parallel file encryption/decryption system for your C++ project. The implementation provides significant performance improvements through concurrent processing and multi-threading.

## What Was Implemented

### 1. Enhanced ProcessManagement Class

- **Thread Pool Architecture**: Implemented a thread pool with configurable number of worker threads
- **Thread-Safe Queue**: Added proper synchronization with mutexes and condition variables
- **Parallel Chunk Processing**: Files are split into chunks and processed concurrently
- **Windows Compatibility**: Replaced POSIX `fork()` with C++ standard threading

### 2. New ParallelCryption Class

- **Multi-threaded Encryption/Decryption**: Process files using multiple threads simultaneously
- **Configurable Chunk Sizes**: Optimize performance based on file size and system capabilities
- **Batch Processing**: Handle multiple files concurrently
- **Directory Processing**: Recursively process entire directories with parallel execution
- **Performance Monitoring**: Real-time statistics and throughput measurement

### 3. Enhanced Build System

Updated `build.ps1` with multiple targets:

- `main` - Enhanced main application with parallel support
- `parallel` - Dedicated parallel encryption demonstration
- `cryption` - Standalone cryption tool
- `all` - Build all targets
- `clean` - Clean build artifacts

### 4. Feature-Rich Main Application

Interactive menu system with options for:

1. Traditional single-threaded processing
2. Parallel multi-threaded directory processing
3. Parallel single file processing
4. Performance demonstrations
5. Directory-wide operations with filtering

## Key Features

### Performance Optimizations

- **Concurrent I/O**: Multiple threads handle different file chunks simultaneously
- **Adaptive Chunk Size**: Configurable chunk sizes for optimal performance
- **Hardware Utilization**: Uses all available CPU cores by default
- **Memory Efficient**: Processes large files without loading entire content into memory

### Thread Safety

- **Mutex Protection**: Thread-safe access to shared resources
- **Atomic Operations**: Lock-free counters for performance tracking
- **Proper Synchronization**: Condition variables for efficient thread communication
- **Resource Management**: Automatic cleanup and proper thread lifecycle management

### Cross-Platform Compatibility

- **C++20 Standard**: Modern C++ features for better performance and safety
- **Windows-Native**: Optimized for Windows environments
- **Standard Threading**: Uses std::thread instead of platform-specific APIs

## Performance Benefits

### Parallel vs Sequential Processing

For larger files, the parallel implementation provides:

- **Scalable Performance**: Performance scales with available CPU cores
- **Reduced Processing Time**: Significant speed improvements for large files
- **Better Resource Utilization**: Efficient use of multi-core processors
- **Concurrent File Handling**: Multiple files processed simultaneously

### Benchmark Results

The implementation includes performance monitoring that shows:

- Files processed per second
- Data throughput (MB/s)
- Processing time comparisons
- Chunk size optimization metrics

## Usage Examples

### Building the Project

```powershell
# Build main application
.\build.ps1 -Target main

# Build parallel demo
.\build.ps1 -Target parallel

# Build all targets
.\build.ps1 -Target all

# Clean and build
.\build.ps1 -Clean -Target main
```

### Running the Applications

```powershell
# Enhanced main application
.\encrypt_decrypt.exe

# Parallel demonstration
.\parallel_demo.exe
```

### Programmatic Usage

```cpp
// Initialize with 4 threads
ParallelCryption crypto(4);

// Encrypt a single file with 1MB chunks
crypto.encryptFile("large_file.dat", 1024 * 1024);

// Decrypt multiple files
vector<string> files = {"file1.txt", "file2.dat", "file3.bin"};
crypto.decryptMultipleFiles(files);

// Process entire directory
crypto.encryptDirectory("./documents", ".txt", true);

// View performance statistics
crypto.printStatistics();
```

## Architecture Improvements

### Before (Original)

- Single-threaded sequential processing
- One file at a time
- POSIX fork() dependency (Windows incompatible)
- Basic task queue
- No performance monitoring

### After (Enhanced)

- Multi-threaded parallel processing
- Concurrent file and chunk processing
- Cross-platform C++ threading
- Advanced thread pool with proper synchronization
- Comprehensive performance monitoring and statistics
- Configurable chunk sizes and thread counts
- Batch and directory processing capabilities

## Files Created/Modified

### New Files

- `src/app/encryptDecrypt/ParallelCryption.hpp` - Parallel encryption class header
- `src/app/encryptDecrypt/ParallelCryption.cpp` - Parallel encryption implementation
- `src/app/encryptDecrypt/ParallelCryptionDemo.cpp` - Demonstration program

### Enhanced Files

- `src/app/Procesess/ProcessManagement.hpp` - Added thread pool and parallel methods
- `src/app/Procesess/ProcessManagement.cpp` - Implemented parallel processing logic
- `main.cpp` - Enhanced with interactive menu and parallel options
- `build.ps1` - Comprehensive build system with multiple targets
- `makefile` - Updated for parallel processing support

## Conclusion

The parallel file encryption/decryption implementation transforms your project from a basic single-threaded tool into a high-performance, scalable encryption system. The architecture is designed for:

- **Performance**: Significant speed improvements through parallelization
- **Scalability**: Efficiently uses available system resources
- **Usability**: Easy-to-use interface with multiple processing options
- **Maintainability**: Clean, modular code structure
- **Extensibility**: Easy to add new features and optimizations

The system is production-ready and provides both backward compatibility with the original functionality while adding powerful new parallel processing capabilities.
