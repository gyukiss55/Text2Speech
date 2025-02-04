// MergeFiles.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include <string>
#include <map>

namespace fs = std::filesystem;

int EnumerateCurrentDirectory() {
    std::string path = "E:\\Work\\GitHub\\_MyGit\\Python_TTS_Test1\\Python_TTS_Test1\\inputMP3"; // Current directory
    std::string m1 = "m1.mp3"; // Current directory
    std::string m2 = "m2.mp3";

    std::vector<std::string> fileNames;
    std::vector<int> fileIndeces;
    std::map<int, size_t> indexOrder;
    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            std::string path1 = entry.path().string();
            size_t pos = path1.find_last_of('w');
            std::string fn1 = path1.substr(pos + 2);
            std::cout << fn1 << std::endl;
            int num = std::stoi(fn1);
            indexOrder[num] = fileIndeces.size();
            fileIndeces.push_back(num);
            fileNames.push_back(path1);
            std::cout << num << ". " << entry.path() << std::endl;
        }
        for (int i = 2; i <= indexOrder.size(); i+=2) {
            std::cout << i << ". " << fileNames[indexOrder[i]] << std::endl;
            std::string command = "copy /b ";
            if (i != 2) {
                command += path + "\\" + m1 + " + ";
            }
            command += fileNames[indexOrder[i]] + " + " + fileNames[indexOrder[i - 1]] + " " + path + "\\" + m2;
            std::cout << "command:" << command << std::endl;
            int result = system(command.c_str());
            if (result == 0) {
                //std::cout << "Files merged successfully!" << std::endl;
            }
            else {
                std::cerr << "Error executing command." << std::endl;
            }
            command = "copy /b " + path + "\\" + m2 + " " + path + "\\" + m1;
            std::cout << "command:" << command << std::endl;
            result = system(command.c_str());
            if (result == 0) {
                //std::cout << "Files merged successfully!" << std::endl;
            }
            else {
                std::cerr << "Error executing command." << std::endl;
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}


int mergeFiles() {
    // Define the DOS command
    const char* command = "copy /b 1.mp3+2.mp3 r.mp3";

    // Execute the command
    int result = system(command);

    // Check if the command executed successfully
    if (result == 0) {
        std::cout << "Files merged successfully!" << std::endl;
    }
    else {
        std::cerr << "Error executing command." << std::endl;
    }

    return 0;
}

int main()
{
    std::cout << "Hello merge Files!\n";
    EnumerateCurrentDirectory();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
