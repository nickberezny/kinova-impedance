#include <fstream>   // Required for file stream operations (std::ifstream)
#include <iostream>  // Required for input/output operations (std::cout, std::cerr)
#include <string>    // Required for string manipulation (std::string, std::getline)
#include <vector>    // Required for storing data (std::vector)
#include <sstream>   // Required for string stream operations (std::stringstream)

std::vector<std::vector<double>> readJointTrajectory()
{
    std::string filename = "trajectories/november5.csv"; // Replace with your CSV file name
    std::ifstream file(filename);

    std::vector<std::vector<double>> data;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return data; // Indicate an error
    }

    std::string line;
    ; // To store the parsed CSV data

    // Read the file line by line
    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line); // Create a stringstream from the current line
        std::string cell;

        // Extract individual cells using comma as a delimiter
        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stod(cell));
        }

        data.push_back(row);
    }

    file.close(); // Close the file stream
    /*
    // Optional: Print the parsed data to verify
    for (const auto& row : data) {
        for (const auto& cell : row) {
            std::cout << cell << ","; // Print each cell followed by a tab
        }
        std::cout << std::endl; // Newline after each row
    }

    */
    return data; // Indicate successful execution
}


