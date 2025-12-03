#include<vector>
#include <Eigen/Dense>


#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "../include/filters.hpp"


void butterworth(double * x, double * y, Eigen::VectorXd &X, Eigen::VectorXd  &Y, Eigen::VectorXd a, Eigen::VectorXd b)
{
	//move data back
	//get new filtered value at Y.begin


    for(int i = 7; i > 0; i--)
    {
        X(i) = X(i-1);
    }

    X(0) = *x;

    *y = b.dot(X) - a.dot(Y);

    for(int i = 6; i > 0; i--)
	{
        Y(i) = Y(i-1);
    }

    Y(0) = *y;

	//X.pop_back();

	//Y.pop_back();
	//Y.insert(Y.begin(), b*X-a*Y);
	return;
}

/*
int main() {
    std::ifstream file("filters/filt5.csv"); 

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::vector<std::vector<std::string>> csv_data;
    std::string line;
    std::vector<double> row_data;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;
        

        while (std::getline(ss, field, ',')) {
            row_data.push_back(std::stod(field));
        }
        //csv_data.push_back(row_data);
    }

    file.close();

    // Example of printing the loaded data
    for (const auto& num : row_data) {
	    std::cout << num << ", ";
	}
        std::cout << std::endl;
  

    return 0;
}
*/