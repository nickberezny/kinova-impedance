#include <iostream>
#include <chrono>
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

void kinovaInverseDynamics(double * q, double * qd, double * qdd, double * tau)
{

    argInit_6x1_real_T(q);
    argInit_6x1_real_T(qd);
    argInit_6x1_real_T(qdd);

    kinovaDynamics(q, qd, qdd, tau);

    return;

}
