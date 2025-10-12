#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <iostream>
#include <sstream>
#include "../fileHanding/IO.hpp"

using namespace std;

enum class Action{
    ENCRYPT,
    DECRYPT,
};

struct Task {
    string filePath;
    fstream f_stream;
    Action action;

    Task(fstream &&stream ,Action act,string filePath)
    : filePath(move(filePath)), f_stream(move(stream)), action(act) {}

    string toString(){
        ostringstream oss;
        oss<<filePath<<","<<(Action::ENCRYPT==action?"ENCRYPT":"DECRYPT");
        return oss.str();
    }

    static Task fromString(const string &taskData){
        istringstream iss(taskData);
        string filePath, actionStr;
      
        if(getline(iss,filePath,',') && getline(iss,actionStr)){
            Action action = (actionStr == "ENCRYPT") ? Action::ENCRYPT : Action::DECRYPT;
            IO io(filePath);
            fstream f_stream = io.getFileStream();  // Remove unnecessary move
            if(f_stream.is_open()){
                return Task(move(f_stream), action, move(filePath));
            } else {
                throw runtime_error("Failed to open file: " + filePath);
            }
        }
        else {
            throw invalid_argument("Invalid task data format: " + taskData);
        }
    }
};




#endif