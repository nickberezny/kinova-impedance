//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_regressorWithMotorDynamics_api.h
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 24-Nov-2025 14:46:44
//

#ifndef _CODER_REGRESSORWITHMOTORDYNAMICS_API_H
#define _CODER_REGRESSORWITHMOTORDYNAMICS_API_H

// Include Files
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"
#include <algorithm>
#include <cstring>

// Variable Declarations
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

// Function Declarations
void regressorWithMotorDynamics(real_T q[6], real_T qd[6], real_T q2d[6],
                                real_T Y[396]);

void regressorWithMotorDynamics_api(const mxArray *const prhs[3],
                                    const mxArray **plhs);

void regressorWithMotorDynamics_atexit();

void regressorWithMotorDynamics_initialize();

void regressorWithMotorDynamics_terminate();

void regressorWithMotorDynamics_xil_shutdown();

void regressorWithMotorDynamics_xil_terminate();

#endif
//
// File trailer for _coder_regressorWithMotorDynamics_api.h
//
// [EOF]
//
