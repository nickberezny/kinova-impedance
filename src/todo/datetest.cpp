#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>  // For std::ofstream
#include <sys/stat.h> // For mkdir
#include <cassert>
#include <cstdlib>


    

int main()
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "log/%d-%m-%Y-%H-%M-%S");
    auto str = oss.str();

    int check = mkdir(str.c_str(), 0777);

    std::cout << str << std::endl;

    oss << "/log.txt";

    std::ofstream outputFile;
    outputFile.open(oss.str()); // Open a file named "output.txt"

    if (outputFile.is_open()) {
        outputFile << "This is the first line." << std::endl;
        outputFile << "This is the second line with a number: " << 42 << std::endl;
        outputFile.close(); // Close the file
        std::cout << "Data written to output.txt successfully." << std::endl;
    } else {
        std::cerr << "Error: Unable to open file output.txt" << std::endl;
    }

    return 0;
}   