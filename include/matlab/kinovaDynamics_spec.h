//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// kinovaDynamics_spec.h
//
// Code generation for function 'kinovaDynamics'
//

#ifndef KINOVADYNAMICS_SPEC_H
#define KINOVADYNAMICS_SPEC_H

// Include files
#if defined(BUILDING_EXE_KINOVADYNAMICS)
#if defined(_WIN32) || defined(__LCC__)
#define KINOVADYNAMICS_DLL_EXPORT __declspec(dllimport)
#else
#define KINOVADYNAMICS_DLL_EXPORT __attribute__((visibility("default")))
#endif
#elif defined(BUILDING_KINOVADYNAMICS)
#if defined(_WIN32) || defined(__LCC__)
#define KINOVADYNAMICS_DLL_EXPORT __declspec(dllexport)
#else
#define KINOVADYNAMICS_DLL_EXPORT __attribute__((visibility("default")))
#endif
#else
#define KINOVADYNAMICS_DLL_EXPORT
#endif

#endif
// End of code generation (kinovaDynamics_spec.h)
