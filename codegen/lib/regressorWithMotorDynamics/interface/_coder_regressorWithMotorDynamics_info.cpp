//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: _coder_regressorWithMotorDynamics_info.cpp
//
// MATLAB Coder version            : 23.2
// C/C++ source code generated on  : 24-Nov-2025 14:46:44
//

// Include Files
#include "_coder_regressorWithMotorDynamics_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

// Function Declarations
static const mxArray *emlrtMexFcnResolvedFunctionsInfo();

// Function Definitions
//
// Arguments    : void
// Return Type  : const mxArray *
//
static const mxArray *emlrtMexFcnResolvedFunctionsInfo()
{
  const mxArray *nameCaptureInfo;
  const char_T *data[4]{
      "789cc553cb4ec240141d0c125da0ac8c5f414013176e792831200a44126a7068af30a133"
      "6366a684fa136ef914977e889fe0ce1f10e8034a3229b111efe6f6f4"
      "747a1e4d51aa564f21848e9037b38cb7b33ecef97b0f4567934f697630fb281d3917f06f"
      "fe36395330551e60984278d2e29430cc54db7d012440727b02d69279",
      "2636b40985d63a682c10adae512158508bebd208cc71cba1488ce4caa1bd0ec23e3e3479"
      "d35bf6d1d3f491dbe07b95c7d2a5d19120a4d120e6d82873d3a1c094"
      "34ae881a3903c372e7b510b36f629b0c045684b3e09e34040ce7d5482e1ee60fd7b9e2a2"
      "ec53791ac9334d98e734264fc04b85998585d50f9df53bf7c54225f0",
      "f194d04746ebc3632cee0c6c58e57e4fa8d7d5ea45f9df7d47ec283e0466e85acbd3b8de"
      "8eb7cca1fb3fb3e860b9679dcf25b52bbdf3efc3af5dea05f35f7a49"
      "ffbf138d5e6e83afd78abc79815fc76ea17c2bbb3713a89edd5daf7c346374e27c200dfe"
      "ebf7ff009add8db1",
      ""};
  nameCaptureInfo = nullptr;
  emlrtNameCaptureMxArrayR2016a(&data[0], 1712U, &nameCaptureInfo);
  return nameCaptureInfo;
}

//
// Arguments    : void
// Return Type  : mxArray *
//
mxArray *emlrtMexFcnProperties()
{
  mxArray *xEntryPoints;
  mxArray *xInputs;
  mxArray *xResult;
  const char_T *propFieldName[9]{"Version",
                                 "ResolvedFunctions",
                                 "Checksum",
                                 "EntryPoints",
                                 "CoverageInfo",
                                 "IsPolymorphic",
                                 "PropertyList",
                                 "UUID",
                                 "ClassEntryPointIsHandle"};
  const char_T *epFieldName[8]{
      "Name",     "NumberOfInputs", "NumberOfOutputs", "ConstantInputs",
      "FullPath", "TimeStamp",      "Constructor",     "Visible"};
  xEntryPoints =
      emlrtCreateStructMatrix(1, 1, 8, (const char_T **)&epFieldName[0]);
  xInputs = emlrtCreateLogicalMatrix(1, 3);
  emlrtSetField(xEntryPoints, 0, "Name",
                emlrtMxCreateString("regressorWithMotorDynamics"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(3.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(xEntryPoints, 0, "FullPath",
                emlrtMxCreateString(
                    "C:\\Users\\Nick\\Documents\\Github\\dynamic_"
                    "calibration\\dynamics\\regressorWithMotorDynamics.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739892.354525463));
  emlrtSetField(xEntryPoints, 0, "Constructor",
                emlrtMxCreateLogicalScalar(false));
  emlrtSetField(xEntryPoints, 0, "Visible", emlrtMxCreateLogicalScalar(true));
  xResult =
      emlrtCreateStructMatrix(1, 1, 9, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("23.2.0.2428915 (R2023b) Update 4"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("DpwmnHwQ1ZphWJLtrzEFkB"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

//
// File trailer for _coder_regressorWithMotorDynamics_info.cpp
//
// [EOF]
//
