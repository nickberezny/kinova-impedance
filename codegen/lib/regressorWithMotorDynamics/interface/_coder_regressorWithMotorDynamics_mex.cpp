//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_regressorWithMotorDynamics_mex.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 24-Nov-2025 14:46:44
//

// Include Files
#include "_coder_regressorWithMotorDynamics_mex.h"
#include "_coder_regressorWithMotorDynamics_api.h"

// Function Definitions
//
// Arguments    : int32_T nlhs
//                mxArray *plhs[]
//                int32_T nrhs
//                const mxArray *prhs[]
// Return Type  : void
//
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
                 const mxArray *prhs[])
{
  mexAtExit(&regressorWithMotorDynamics_atexit);
  // Module initialization.
  regressorWithMotorDynamics_initialize();
  // Dispatch the entry-point.
  unsafe_regressorWithMotorDynamics_mexFunction(nlhs, plhs, nrhs, prhs);
  // Module termination.
  regressorWithMotorDynamics_terminate();
}

//
// Arguments    : void
// Return Type  : emlrtCTX
//
emlrtCTX mexFunctionCreateRootTLS()
{
  emlrtCreateRootTLSR2022a(&emlrtRootTLSGlobal, &emlrtContextGlobal, nullptr, 1,
                           nullptr, "windows-1252", true);
  return emlrtRootTLSGlobal;
}

//
// Arguments    : int32_T nlhs
//                mxArray *plhs[1]
//                int32_T nrhs
//                const mxArray *prhs[3]
// Return Type  : void
//
void unsafe_regressorWithMotorDynamics_mexFunction(int32_T nlhs,
                                                   mxArray *plhs[1],
                                                   int32_T nrhs,
                                                   const mxArray *prhs[3])
{
  emlrtStack st{
      nullptr, // site
      nullptr, // tls
      nullptr  // prev
  };
  const mxArray *b_prhs[3];
  const mxArray *outputs;
  st.tls = emlrtRootTLSGlobal;
  // Check for proper number of arguments.
  if (nrhs != 3) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 3, 4,
                        26, "regressorWithMotorDynamics");
  }
  if (nlhs > 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 26,
                        "regressorWithMotorDynamics");
  }
  // Call the function.
  b_prhs[0] = prhs[0];
  b_prhs[1] = prhs[1];
  b_prhs[2] = prhs[2];
  regressorWithMotorDynamics_api(b_prhs, &outputs);
  // Copy over outputs to the caller.
  emlrtReturnArrays(1, &plhs[0], &outputs);
}

//
// File trailer for _coder_regressorWithMotorDynamics_mex.cpp
//
// [EOF]
//
