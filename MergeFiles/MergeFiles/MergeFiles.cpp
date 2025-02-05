// MergeFiles.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include <string>
#include <map>
#include <windows.h>

#include <stdexcept>

namespace fs = std::filesystem;


std::string wstringToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();


    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()),
        nullptr, 0, nullptr, nullptr);
    if (sizeNeeded <= 0) {
        throw std::runtime_error("Conversion failed");
    }

    std::string utf8Str(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()),
        &utf8Str[0], sizeNeeded, nullptr, nullptr);
    return utf8Str;
}

std::wstring utf8ToWString(const std::string& utf8Str)
{
    if (utf8Str.empty())
        return std::wstring();

    // Get the length of the wide string (in wchar_t's), including the null terminator.
    int wcharsNeeded = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
    if (wcharsNeeded == 0)
    {
        throw std::runtime_error("MultiByteToWideChar failed to get buffer size.");
    }

    // Create a wstring of the required size.
    // The resulting string will include a null terminator at the end.
    std::wstring wstr(wcharsNeeded, L'\0');

    // Do the actual conversion.
    int result = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wstr[0], wcharsNeeded);
    if (result == 0)
    {
        throw std::runtime_error("MultiByteToWideChar failed to convert string.");
    }

    // Remove the null terminator added by MultiByteToWideChar.
    wstr.resize(wcharsNeeded - 1);
    return wstr;
}

int EnumerateCurrentDirectory(char * str) {
    std::cout << "work path: " << str << std::endl;
    std::wstring path = utf8ToWString (str); // work directory
    std::wstring m1 = L"m1.mp3"; // file1 directory
    std::wstring m2 = L"m2.mp3"; // file2 directory

    std::vector<std::wstring> fileNames;
    std::vector<int> fileIndeces;
    std::map<int, size_t> indexOrder;
    try {
        for (const auto& entry : fs::directory_iterator(wstringToUtf8(path))) {
            std::wstring path1 = entry.path().wstring();
            size_t pos = path1.find_last_of('w');
            std::wstring fn1 = path1.substr(pos + 2);
            std::wcout << fn1 << std::endl;
            int num = std::stoi(fn1);
            indexOrder[num] = fileIndeces.size();
            fileIndeces.push_back(num);
            fileNames.push_back(path1);
            std::cout << num << ". " << entry.path() << std::endl;
        }
        for (int i = 2; i <= indexOrder.size(); i+=2) {
            std::wcout << i << L". " << fileNames[indexOrder[i]] << std::endl;
            std::wstring command = L"copy /b ";
            if (i != 2) {
                command += path + L"\\" + m1 + L" + ";
            }
            command += fileNames[indexOrder[i]] + L" + " + fileNames[indexOrder[i - 1]] + L" " + path + L"\\" + m2;
            std::wcout << L"command:" << command << std::endl;
            int result = system(wstringToUtf8(command).c_str());
            if (result == 0) {
                //std::cout << "Files merged successfully!" << std::endl;
            }
            else {
                std::cerr << "Error executing command." << std::endl;
            }
            command = L"copy /b " + path + L"\\" + m2 + L" " + path + L"\\" + m1;
            std::wcout << L"command:" << command << std::endl;
            result = system(wstringToUtf8(command).c_str());
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

/*
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
*/


int main(int argc, char* argv[])
{
    if (argc !=2)
        std::cout << "Usage: " << argv[0] << " <workFolder> \n";
    else
        EnumerateCurrentDirectory(argv[1]);
}

