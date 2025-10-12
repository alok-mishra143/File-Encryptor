#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "IO.hpp"
#define endl "\n"
using namespace std;

class ReadEnv{
    public:
     string getEnv(){
        string envPath=".env";
        IO io(envPath);
        fstream  f_stream=io.getFileStream();
        stringstream buffer;
        buffer << f_stream.rdbuf();
        string content = buffer.str();
        f_stream.close();
        return content;
     }
};