# File Encryptor

A C++20 file encryption/decryption tool with both traditional (single-threaded) and parallel (multi-threaded, chunked) modes. Includes a demonstration app and Windows-friendly build tooling.

## Features

- Interactive menu (`encrypt_decrypt.exe`)
- Parallel encrypt/decrypt by chunk with configurable thread count and chunk size
- Batch and directory processing with optional extension filter and recursion
- Performance statistics (files, bytes, elapsed time, throughput)
- Demo runner to compare chunk sizes and workflows

## Project layout

- `main.cpp` – interactive CLI
- `src/app/encryptDecrypt/` – cryption logic (emoji sequential, parallel orchestrator, demos)
- `src/app/fileHanding/` – IO utilities and `.env` reader
- `src/app/Procesess/` – task model and process/thread management
- `makefile`, `build.ps1`, `build.bat` – build scripts
- `docs/ARCHITECTURE.md` – detailed architecture with Mermaid diagrams
- `PARALLEL_IMPLEMENTATION.md` – notes on the parallel implementation

## Prerequisites

- Windows with PowerShell
- g++ (MinGW-w64) supporting C++20
- A `.env` file in the repository root containing a numeric key, e.g.

```
123
```

## Build

Recommended on Windows:

```powershell
.# Clean and build main app
./build.ps1 -Clean -Target main

# Or build all targets
./build.ps1 -Target all
```

Alternative using make (MinGW make):

```powershell
make
```

Batch file (builds main app only):

```powershell
./build.bat
```

## Run

Interactive app:

```powershell
./encrypt_decrypt.exe
```

Parallel demo:

```powershell
./parallel_demo.exe
```

Standalone cryption driver (for testing `executeCryption`):

```powershell
./cryption.exe "<filePath>,ENCRYPT"
```

## Usage tips

- Ensure test data exists in `test/` (e.g., `test/test1.txt`) for demos.
- The parallel mode processes files in-place with a byte shift; the sequential emoji mode rewrites files as UTF-8 emojis. They are not compatible formats—don’t mix them for the same file lineage.

## Documentation

- Detailed architecture and diagrams: `docs/ARCHITECTURE.md`
- Implementation notes: `PARALLEL_IMPLEMENTATION.md`

## License

This project is provided for educational purposes. The implemented ciphers are not secure for real-world encryption.
