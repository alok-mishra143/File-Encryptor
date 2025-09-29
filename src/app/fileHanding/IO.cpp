#include <bits/stdc++.h>
#include "IO.hpp"
#define endl "\n"
using namespace std;

IO::IO(const string &file_path){
    file_stream.open(file_path,ios::in | ios::out | ios::binary);

    if(!file_stream.is_open()) {
        cerr << "Error opening file: " << file_path << endl;
        exit(EXIT_FAILURE);
    }
}

fstream IO::getFileStream(){
    return move(file_stream);
}

IO::~IO(){
    if(file_stream.is_open()){
        file_stream.close();
    }
}