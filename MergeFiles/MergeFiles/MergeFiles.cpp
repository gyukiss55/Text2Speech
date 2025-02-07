// MergeFiles.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <windows.h>

#include <stdexcept>

namespace fs = std::filesystem;
std::vector<char> buffer;

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

int MergeFile(std::string inputFn, std::ofstream&  outFileStream)
{
    std::ifstream inFileStream(inputFn, std::ios::binary);
    if (!inFileStream) {
        std::cerr << "Error opening file for reading: " << inputFn << std::endl;
        return 1;
    }
    // Determine the file size by seeking to the end
    inFileStream.seekg(0, std::ios::end);
    std::streamsize size = inFileStream.tellg();
    inFileStream.seekg(0, std::ios::beg);

    // Resize the buffer to hold the file content
    buffer.resize(static_cast<size_t>(size));

    // Read the file content into the buffer
    if (!inFileStream.read(buffer.data(), size)) {
        std::cerr << "Error reading data from file." << std::endl;
        return 1;
    }
    // Close the input file
    inFileStream.close();

    outFileStream.write(buffer.data(), buffer.size());
    if (!outFileStream) {
        std::cerr << "Error writing data to copy file." << std::endl;
        return 1;
    }

    return 0;
}

int EnumerateCurrentDirectory(char * workFolder) {
    std::cout << "work path: " << workFolder << std::endl;
    std::wstring path = utf8ToWString (workFolder); // work directory
    std::wstring m1 = L"m1.mp3"; // file1 directory
    std::wstring m2 = L"m2.mp3"; // file2 directory
    std::string destFileName = workFolder + std::string("\\result.mp3"); // file2 directory

    std::vector<std::wstring> fileNames;
    std::vector<int> fileIndeces;
    std::map<int, size_t> indexOrder;
    try {
        for (const auto& entry : fs::directory_iterator(wstringToUtf8(path))) {
            std::wstring path1 = entry.path().wstring();
            size_t pos = path1.find_last_of('w');
            if (pos > path1.length() || (path1.substr(pos + 2)[0]) < L'0' || (path1.substr(pos + 2)[0]) > L'9')
                continue;
            std::wstring fn1 = path1.substr(pos + 2);
            std::wcout << fn1 << std::endl;
            int num = std::stoi(fn1);
            indexOrder[num] = fileIndeces.size();
            fileIndeces.push_back(num);
            fileNames.push_back(path1);
            std::cout << num << ". " << entry.path() << std::endl;
        }
        std::ofstream  outFileStream(destFileName, std::ios::binary);
        for (int i = 2; i <= indexOrder.size(); i+=2) {
            std::wcout << i << L". " << fileNames[indexOrder[i]] << std::endl;
            MergeFile(wstringToUtf8(fileNames[indexOrder[i]]), outFileStream);
            MergeFile(wstringToUtf8(fileNames[indexOrder[i - 1]]), outFileStream);
        }
        outFileStream.close();
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}


int main(int argc, char* argv[])
{
    if (argc !=2)
        std::cout << "Usage: " << argv[0] << " <workFolder> \n";
    else
        EnumerateCurrentDirectory(argv[1]);
}

