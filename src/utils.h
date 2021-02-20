#pragma once

#include <iostream>
#include <fstream>
#include <string> 
#include <vector>

using namespace std;

vector<char> readFile(const string &filename) {
    ifstream file(filename, ios::binary | ios::ate);

    if (file) {
        size_t fileSize = (size_t) file.tellg();
        vector<char> fileBuffer(fileSize);
        file.seekg(0);
        file.read(fileBuffer.data(), fileSize);
        file.close();
        return fileBuffer;
    } else {
        throw runtime_error("failed to open file!!!");
    }
}