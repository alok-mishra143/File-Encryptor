<div align="center">

# File Encryptor

### High Performance C++20 File Encryption & Decryption

Traditional and Parallel file encryption with configurable multi-threading, chunk-based processing, batch operations, and performance benchmarking.

![C++](https://img.shields.io/badge/C%2B%2B-20-blue?style=for-the-badge&logo=c%2B%2B)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows)
![Build](https://img.shields.io/badge/Build-MinGW--w64-success?style=for-the-badge)
![License](https://img.shields.io/badge/License-Educational-orange?style=for-the-badge)

</div>

---

## Overview

**File Encryptor** is a C++20 project demonstrating both traditional and parallel approaches to file encryption.

It focuses on:

- High-performance chunk-based processing
- Multi-threaded encryption/decryption
- Batch and directory processing
- Configurable worker threads
- Performance benchmarking
- Clean modular architecture

> **Note**
>
> This project is intended for learning parallel programming and file processing. The implemented cipher is **not cryptographically secure**.

---

# Features

- Traditional single-threaded encryption/decryption
- Parallel chunk-based processing
- Configurable thread count
- Configurable chunk size
- Batch file processing
- Recursive directory processing
- File extension filtering
- Throughput statistics
- Interactive CLI
- Performance demo application
- Windows build scripts
- Modular C++20 architecture

---

# Screenshots

> Add screenshots here

| Menu | Parallel Demo |
|------|---------------|
| ![](docs/images/menu.png) | ![](docs/images/demo.png) |

---

# Project Structure

```text
.
├── docs/
│   ├── ARCHITECTURE.md
│   └── PARALLEL_IMPLEMENTATION.md
│
├── src/
│   └── app/
│       ├── encryptDecrypt/
│       ├── fileHanding/
│       └── Procesess/
│
├── main.cpp
├── build.ps1
├── build.bat
├── makefile
├── .env
└── README.md
```

---

# Architecture

```text
                User
                  │
                  ▼
        Interactive CLI (main.cpp)
                  │
                  ▼
        Encryption Controller
                  │
      ┌───────────┴────────────┐
      ▼                        ▼
 Sequential Engine      Parallel Engine
                               │
                    Split File into Chunks
                               │
                ┌──────────────┼──────────────┐
                ▼              ▼              ▼
            Thread 1       Thread 2      Thread N
                │              │              │
                └──────────────┼──────────────┘
                               ▼
                       Write Processed Data
                               │
                               ▼
                           Output File
```

For detailed diagrams see:

```
docs/ARCHITECTURE.md
```

---

# Project Components

| Component | Purpose |
|-----------|---------|
| `main.cpp` | Interactive CLI |
| `encryptDecrypt/` | Sequential & parallel encryption logic |
| `fileHanding/` | File utilities and `.env` handling |
| `Procesess/` | Thread and task management |
| `parallel_demo` | Performance demonstrations |
| `cryption` | Standalone testing executable |

---

# Prerequisites

- Windows
- PowerShell
- MinGW-w64
- g++ with C++20 support

Create a `.env` file in the project root.

```text
123
```

The file should contain only the numeric encryption key.

---

# Build

## PowerShell (Recommended)

```powershell
# Clean and build
./build.ps1 -Clean -Target main
```

Build every executable:

```powershell
./build.ps1 -Target all
```

---

## Make

```powershell
make
```

---

## Batch

```powershell
./build.bat
```

---

# Executables

| Executable | Description |
|------------|-------------|
| `encrypt_decrypt.exe` | Interactive application |
| `parallel_demo.exe` | Parallel performance demo |
| `cryption.exe` | Standalone encryption driver |

---

# Running

## Interactive Menu

```powershell
./encrypt_decrypt.exe
```

---

## Parallel Demo

```powershell
./parallel_demo.exe
```

---

## Cryption Driver

```powershell
./cryption.exe "<file>,ENCRYPT"
```

Example

```powershell
./cryption.exe "test/test1.txt,ENCRYPT"
```

---

# Parallel Processing

```
Input File
     │
     ▼
Split into Chunks
     │
     ▼
+---------+---------+---------+---------+
| Chunk 1 | Chunk 2 | Chunk 3 | Chunk 4 |
+---------+---------+---------+---------+
     │         │         │         │
     ▼         ▼         ▼         ▼
 Thread1   Thread2   Thread3   Thread4
     │         │         │         │
     └─────────┴─────────┴─────────┘
               ▼
        Merge Processed Data
               ▼
           Output File
```

---

# Performance Metrics

The project records statistics including

- Files processed
- Bytes processed
- Processing time
- Throughput
- Thread count
- Chunk size

This allows benchmarking different configurations.

---

# Sequential vs Parallel

| Feature | Sequential | Parallel |
|----------|------------|----------|
| Single Thread | Yes | No |
| Multi-threading | No | Yes |
| Chunk Processing | No | Yes |
| Directory Processing | Yes | Yes |
| Batch Processing | Yes | Yes |
| Performance Statistics | Basic | Detailed |
| Configurable Threads | No | Yes |

---

# Documentation

| File | Description |
|------|-------------|
| `docs/ARCHITECTURE.md` | Complete architecture diagrams |
| `PARALLEL_IMPLEMENTATION.md` | Parallel implementation details |

---

# Important Notes

- Sequential emoji encryption and parallel byte-shift encryption produce different file formats.
- They are **not compatible** with one another.
- Do **not** encrypt a file with one mode and decrypt it using the other.

---

# Technologies

- C++20
- STL Threads
- Mutex
- Atomic Operations
- File Streams
- PowerShell
- MinGW-w64

---

# Future Improvements

- AES encryption
- ChaCha20 support
- Progress bars
- Cross-platform support
- Compression
- Memory-mapped files
- SIMD optimization
- Benchmark suite
- GUI application

---

# License

This project is provided for **educational purposes**.

The encryption algorithm is intentionally simple and **should not be used for securing sensitive data**.

---

<div align="center">

**Built with C++20**

⭐ Star the repository if you found it useful.

</div>
