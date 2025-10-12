# Build script for the C++ project with Parallel Encryption Support

param(
    [string]$Target = "main",
    [switch]$Clean,
    [switch]$Help
)

function Show-Help {
    Write-Host "Build script for C++ File Encryption Project" -ForegroundColor Green
    Write-Host ""
    Write-Host "Usage: .\build.ps1 [-Target <target>] [-Clean] [-Help]" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Targets:" -ForegroundColor Cyan
    Write-Host "  main        - Build main application with parallel support (default)"
    Write-Host "  parallel    - Build parallel encryption demo"
    Write-Host "  cryption    - Build standalone cryption tool"
    Write-Host "  all         - Build all targets"
    Write-Host "  clean       - Clean all build artifacts"
    Write-Host ""
    Write-Host "Switches:" -ForegroundColor Cyan
    Write-Host "  -Clean      - Clean before building"
    Write-Host "  -Help       - Show this help message"
    Write-Host ""
    Write-Host "Examples:" -ForegroundColor Yellow
    Write-Host "  .\build.ps1"
    Write-Host "  .\build.ps1 -Target all"
    Write-Host "  .\build.ps1 -Target parallel"
    Write-Host "  .\build.ps1 -Clean"
}

function Clean-BuildArtifacts {
    Write-Host "Cleaning build artifacts..." -ForegroundColor Yellow
    
    $patterns = @(
        "*.o", 
        "src/app/Procesess/*.o", 
        "src/app/fileHanding/*.o", 
        "src/app/encryptDecrypt/*.o", 
        "*.exe"
    )
    
    foreach ($pattern in $patterns) {
        Remove-Item -Path $pattern -ErrorAction SilentlyContinue
    }
    
    Write-Host "Clean completed." -ForegroundColor Green
}

function Test-Compiler {
    try {
        g++ --version | Out-Null
        return $true
    }
    catch {
        Write-Error "g++ compiler not found. Please install MinGW-w64 or similar."
        return $false
    }
}

function Compile-SourceFile {
    param(
        [string]$SourceFile,
        [string]$OutputFile,
        [string[]]$CxxFlags
    )
    
    Write-Host "Compiling $SourceFile..." -ForegroundColor Blue
    
    $cmd = @("g++") + $CxxFlags + @("-c", $SourceFile, "-o", $OutputFile)
    & $cmd[0] $cmd[1..($cmd.Length-1)]
    
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Failed to compile $SourceFile"
        return $false
    }
    return $true
}

function Link-Objects {
    param(
        [string[]]$ObjectFiles,
        [string]$OutputFile,
        [string[]]$CxxFlags
    )
    
    Write-Host "Linking $OutputFile..." -ForegroundColor Blue
    
    $cmd = @("g++") + $CxxFlags + $ObjectFiles + @("-o", $OutputFile)
    & $cmd[0] $cmd[1..($cmd.Length-1)]
    
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Failed to link $OutputFile"
        return $false
    }
    return $true
}

function Build-MainTarget {
    Write-Host "Building main application with parallel support..." -ForegroundColor Green
    
    $cxxflags = @("-std=c++20", "-g", "-Wall", "-I.", "-Isrc/app/encryptDecrypt", "-Isrc/app/fileHanding", "-Isrc/app/Procesess", "-pthread")
    
    # Source files for main target
    $sources = @(
        @("main.cpp", "main.o"),
        @("src/app/Procesess/ProcessManagement.cpp", "src/app/Procesess/ProcessManagement.o"),
        @("src/app/fileHanding/IO.cpp", "src/app/fileHanding/IO.o"),
        @("src/app/fileHanding/ReadENV.cpp", "src/app/fileHanding/ReadENV.o"),
        @("src/app/encryptDecrypt/Cryption.cpp", "src/app/encryptDecrypt/Cryption.o"),
        @("src/app/encryptDecrypt/ParallelCryption.cpp", "src/app/encryptDecrypt/ParallelCryption.o")
    )
    
    # Compile all source files
    foreach ($source in $sources) {
        if (!(Compile-SourceFile -SourceFile $source[0] -OutputFile $source[1] -CxxFlags $cxxflags)) {
            return $false
        }
    }
    
    # Link the executable
    $objects = $sources | ForEach-Object { $_[1] }
    if (!(Link-Objects -ObjectFiles $objects -OutputFile "encrypt_decrypt" -CxxFlags $cxxflags)) {
        return $false
    }
    
    Write-Host "Main application built successfully: encrypt_decrypt.exe" -ForegroundColor Green
    return $true
}

function Build-ParallelDemo {
    Write-Host "Building parallel encryption demo..." -ForegroundColor Green
    
    $cxxflags = @("-std=c++20", "-g", "-Wall", "-I.", "-Isrc/app/encryptDecrypt", "-Isrc/app/fileHanding", "-Isrc/app/Procesess", "-pthread")
    
    # Source files for parallel demo
    $sources = @(
        @("src/app/encryptDecrypt/ParallelCryptionDemo.cpp", "src/app/encryptDecrypt/ParallelCryptionDemo.o"),
        @("src/app/Procesess/ProcessManagement.cpp", "src/app/Procesess/ProcessManagement.o"),
        @("src/app/fileHanding/IO.cpp", "src/app/fileHanding/IO.o"),
        @("src/app/fileHanding/ReadENV.cpp", "src/app/fileHanding/ReadENV.o"),
        @("src/app/encryptDecrypt/Cryption.cpp", "src/app/encryptDecrypt/Cryption.o"),
        @("src/app/encryptDecrypt/ParallelCryption.cpp", "src/app/encryptDecrypt/ParallelCryption.o")
    )
    
    # Compile all source files
    foreach ($source in $sources) {
        if (!(Compile-SourceFile -SourceFile $source[0] -OutputFile $source[1] -CxxFlags $cxxflags)) {
            return $false
        }
    }
    
    # Link the executable
    $objects = $sources | ForEach-Object { $_[1] }
    if (!(Link-Objects -ObjectFiles $objects -OutputFile "parallel_demo" -CxxFlags $cxxflags)) {
        return $false
    }
    
    Write-Host "Parallel demo built successfully: parallel_demo.exe" -ForegroundColor Green
    return $true
}

function Build-CryptionTarget {
    Write-Host "Building standalone cryption tool..." -ForegroundColor Green
    
    $cxxflags = @("-std=c++20", "-g", "-Wall", "-I.", "-Isrc/app/encryptDecrypt", "-Isrc/app/fileHanding", "-Isrc/app/Procesess")
    
    # Source files for cryption target
    $sources = @(
        @("src/app/encryptDecrypt/CryptionMain.cpp", "src/app/encryptDecrypt/CryptionMain.o"),
        @("src/app/encryptDecrypt/Cryption.cpp", "src/app/encryptDecrypt/Cryption.o"),
        @("src/app/fileHanding/IO.cpp", "src/app/fileHanding/IO.o"),
        @("src/app/fileHanding/ReadENV.cpp", "src/app/fileHanding/ReadENV.o")
    )
    
    # Compile all source files
    foreach ($source in $sources) {
        if (!(Compile-SourceFile -SourceFile $source[0] -OutputFile $source[1] -CxxFlags $cxxflags)) {
            return $false
        }
    }
    
    # Link the executable
    $objects = $sources | ForEach-Object { $_[1] }
    if (!(Link-Objects -ObjectFiles $objects -OutputFile "cryption" -CxxFlags $cxxflags)) {
        return $false
    }
    
    Write-Host "Cryption tool built successfully: cryption.exe" -ForegroundColor Green
    return $true
}

# Main execution logic
if ($Help) {
    Show-Help
    exit 0
}

if (!(Test-Compiler)) {
    exit 1
}

# Create directories if they don't exist
New-Item -ItemType Directory -Force -Path "src/app/Procesess", "src/app/fileHanding", "src/app/encryptDecrypt" | Out-Null

if ($Clean -or $Target -eq "clean") {
    Clean-BuildArtifacts
    if ($Target -eq "clean") {
        exit 0
    }
}

$success = $true

switch ($Target.ToLower()) {
    "main" {
        $success = Build-MainTarget
    }
    "parallel" {
        $success = Build-ParallelDemo
    }
    "cryption" {
        $success = Build-CryptionTarget
    }
    "all" {
        Write-Host "Building all targets..." -ForegroundColor Green
        $success = (Build-MainTarget) -and (Build-ParallelDemo) -and (Build-CryptionTarget)
    }
    default {
        Write-Error "Unknown target: $Target. Use -Help for available targets."
        exit 1
    }
}

if ($success) {
    Write-Host ""
    Write-Host "Build completed successfully!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Available executables:" -ForegroundColor Cyan
    if (Test-Path "encrypt_decrypt.exe") { Write-Host "  encrypt_decrypt.exe - Main application with parallel support" }
    if (Test-Path "parallel_demo.exe") { Write-Host "  parallel_demo.exe - Parallel encryption demonstration" }
    if (Test-Path "cryption.exe") { Write-Host "  cryption.exe - Standalone cryption tool" }
    Write-Host ""
    Write-Host "Usage examples:" -ForegroundColor Yellow
    Write-Host "  .\encrypt_decrypt.exe     # Run main application"
    Write-Host "  .\parallel_demo.exe       # Run parallel demo"
    Write-Host "  .\cryption.exe           # Run cryption tool"
} else {
    Write-Error "Build failed!"
    exit 1
}