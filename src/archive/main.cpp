//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// main.cpp
//
// Code generation for function 'main'
//

/*************************************************************************/
/* This automatically generated example C++ main file shows how to call  */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/

// Include files
#include <iostream>
#include <chrono>
#include "main.h"
#include "kinovaDynamics.h"
#include "kinovaDynamics_initialize.h"
#include "kinovaDynamics_terminate.h"
#include "rt_nonfinite.h"

// Function Declarations
static void argInit_6x1_real_T(double result[6]);

static double argInit_real_T();

// Function Definitions
static void argInit_6x1_real_T(double result[6])
{
  // Loop over the array to initialize each element.
  for (int idx0{0}; idx0 < 6; idx0++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx0] = argInit_real_T();
  }
}

static double argInit_real_T()
{
  return 0.0;
}

int main(int, char **)
{
  // Initialize the application.
  // You do not need to do this more than one time.
  kinovaDynamics_initialize();
  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_kinovaDynamics();
  // Terminate the application.
  // You do not need to do this more than one time.
  kinovaDynamics_terminate();
  return 0;
}

void main_kinovaDynamics()
{





  double q_tmp[6];
  double tau[6];
  // Initialize function 'kinovaDynamics' input arguments.
  // Initialize function input argument 'q'.
  argInit_6x1_real_T(q_tmp);
  // Initialize function input argument 'qd'.
  // Initialize function input argument 'q2d'.
  // Call the entry-point 'kinovaDynamics'.
  
long long microseconds = 0;
long long max = 0; 

  for(int i = 0; i < 1000; i++)
  {
  	q_tmp[0] = (double)i/1000.0;
	q_tmp[1] = (double)i/1000.0;
	q_tmp[2] = (double)i/1000.0;
	q_tmp[3] = (double)i/1000.0;
	q_tmp[4] = (double)i/1000.0;
	q_tmp[5] = (double)i/1000.0;
	auto start = std::chrono::high_resolution_clock::now();
	kinovaDynamics(q_tmp, q_tmp, q_tmp, tau);
	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
	elapsed).count();
	if(microseconds>max) max = microseconds;
  }
  
        
  std::cout << max << std::endl;
  
  for(int i = 0; i<6; i++)
  {
  	std::cout << tau[i] << ",";
  }
  std::cout << std::endl;
}

// End of code generation (main.cpp)
