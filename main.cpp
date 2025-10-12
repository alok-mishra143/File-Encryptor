#include <iostream>
#include<filesystem>
#include<algorithm>
#include "./src/app/Procesess/ProcessManagement.hpp"
#include "./src/app/Procesess/Task.hpp"
#include "./src/app/fileHanding/IO.hpp"
#include "./src/app/encryptDecrypt/ParallelCryption.hpp"
using namespace std;
#define endl "\n"

namespace fs=std::filesystem;

void showMenu() {
    cout << "\n=== File Encryption/Decryption Tool ===" << endl;
    cout << "1. Traditional processing (single-threaded)" << endl;
    cout << "2. Parallel processing (multi-threaded)" << endl;
    cout << "3. Parallel single file processing" << endl;
    cout << "4. Parallel directory processing" << endl;
    cout << "5. Performance demo" << endl;
    cout << "6. Exit" << endl;
    cout << "Choose an option (1-6): ";
}

void traditionalProcessing() {
    string directory;
    string action;

    cout<<"Enter the directory path: "<<endl;
    getline(cin,directory);
    cout<<"Enter the action (encrypt/decrypt): "<<endl;
    getline(cin,action);

    try{
        if (fs::exists(directory) && fs::is_directory(directory)) {
            ProcessManagement pm;
            for (const auto& entry : fs::recursive_directory_iterator(directory)) {
                if (fs::is_regular_file(entry.path())) {
                    string filePath = entry.path().string();
                    cout << "Processing file: " << filePath << endl;

                    IO io(filePath);
                    fstream f_stream = io.getFileStream();  // Remove unnecessary move

                    if(f_stream.is_open()){
                        // Convert action to uppercase for comparison
                        string upperAction = action;
                        transform(upperAction.begin(), upperAction.end(), upperAction.begin(), ::toupper);
                        Action taskAction  =(upperAction=="ENCRYPT")?Action::ENCRYPT:Action::DECRYPT;

                        auto task=make_unique<Task>(move(f_stream),taskAction,filePath);
                         pm.submitToQueue(move(task));
                    }
                    else {
                        cerr << "Failed to open file: " << filePath << endl;
                    }
                }
            }
            pm.executeTasks();
        } else {
            cerr << "Invalid directory path." << endl;
        }
    } catch(const std::exception& e){
        cerr<<"Error: "<<e.what()<<endl;
    }
}

void parallelSingleFile() {
    string filePath;
    string action;
    
    cout << "Enter the file path: ";
    getline(cin, filePath);
    cout << "Enter the action (encrypt/decrypt): ";
    getline(cin, action);
    
    try {
        ParallelCryption crypto(4); // Use 4 threads
        
        string upperAction = action;
        transform(upperAction.begin(), upperAction.end(), upperAction.begin(), ::toupper);
        
        bool success = false;
        if(upperAction == "ENCRYPT") {
            success = crypto.encryptFile(filePath);
        } else if(upperAction == "DECRYPT") {
            success = crypto.decryptFile(filePath);
        } else {
            cout << "Invalid action. Use 'encrypt' or 'decrypt'." << endl;
            return;
        }
        
        if(success) {
            cout << "Operation completed successfully!" << endl;
            crypto.printStatistics();
        } else {
            cout << "Operation failed!" << endl;
        }
        
    } catch(const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void parallelDirectoryProcessing() {
    string directory;
    string action;
    string fileExtension;
    string recursiveStr;
    
    cout << "Enter the directory path: ";
    getline(cin, directory);
    cout << "Enter the action (encrypt/decrypt): ";
    getline(cin, action);
    cout << "Enter file extension filter (or press Enter for all files): ";
    getline(cin, fileExtension);
    cout << "Process subdirectories recursively? (y/n): ";
    getline(cin, recursiveStr);
    
    bool recursive = (recursiveStr == "y" || recursiveStr == "Y" || recursiveStr == "yes");
    
    try {
        ParallelCryption crypto(thread::hardware_concurrency()); // Use all available cores
        
        string upperAction = action;
        transform(upperAction.begin(), upperAction.end(), upperAction.begin(), ::toupper);
        
        bool success = false;
        if(upperAction == "ENCRYPT") {
            success = crypto.encryptDirectory(directory, fileExtension, recursive);
        } else if(upperAction == "DECRYPT") {
            success = crypto.decryptDirectory(directory, fileExtension, recursive);
        } else {
            cout << "Invalid action. Use 'encrypt' or 'decrypt'." << endl;
            return;
        }
        
        if(success) {
            cout << "Directory processing completed successfully!" << endl;
            crypto.printStatistics();
        } else {
            cout << "Directory processing completed with errors!" << endl;
        }
        
    } catch(const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void performanceDemo() {
    cout << "Running parallel encryption performance demonstration..." << endl;
    
    try {
        ParallelCryption crypto(4);
        
        // Create a test file if it doesn't exist
        string testFile = "test/test1.txt";
        if(!fs::exists(testFile)) {
            cout << "Test file not found. Please ensure " << testFile << " exists." << endl;
            return;
        }
        
        cout << "Testing parallel encryption/decryption performance..." << endl;
        
        auto start = chrono::high_resolution_clock::now();
        
        // Encrypt and decrypt multiple times to show performance
        for(int i = 0; i < 3; ++i) {
            cout << "\nRound " << (i+1) << ":" << endl;
            crypto.encryptFile(testFile);
            crypto.decryptFile(testFile);
        }
        
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        
        cout << "\nPerformance test completed in " << duration.count() << " ms" << endl;
        crypto.printStatistics();
        
    } catch(const exception& e) {
        cerr << "Error during performance demo: " << e.what() << endl;
    }
}

int main(int argc, char* argv[]) {
    int choice;
    
    do {
        showMenu();
        cin >> choice;
        cin.ignore(); // Clear the newline character from the buffer
        
        switch(choice) {
            case 1:
                traditionalProcessing();
                break;
            case 2:
                parallelDirectoryProcessing();
                break;
            case 3:
                parallelSingleFile();
                break;
            case 4:
                parallelDirectoryProcessing();
                break;
            case 5:
                performanceDemo();
                break;
            case 6:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
        
        if(choice != 6) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
        
    } while(choice != 6);
    
    return 0;
}