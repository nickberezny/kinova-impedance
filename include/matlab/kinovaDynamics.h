//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// kinovaDynamics.h
//
// Code generation for function 'kinovaDynamics'
//

#ifndef KINOVADYNAMICS_H
#define KINOVADYNAMICS_H

// Include files
#include "kinovaDynamics_spec.h"
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
KINOVADYNAMICS_DLL_EXPORT extern void kinovaDynamics(const double q[6],
                                                     const double qd[6],
                                                     const double q2d[6],
                                                     double tau[6]);

#endif
// End of code generation (kinovaDynamics.h)
