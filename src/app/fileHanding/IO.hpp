#ifndef IO_HPP
#define IO_HPP

#include<bits/stdc++.h>
using namespace std;
class IO{
    public:
    IO(const string &file_path);
    ~IO();
    fstream getFileStream();

    private:
     fstream file_stream;
};

#endif