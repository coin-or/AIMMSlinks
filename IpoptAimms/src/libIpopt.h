// Copyright (C) 2009 Paragon Decision Technology B.V. and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// $Id$
//
// Authors: Yidong Lang, L.T. Biegler, Chuck Teeter, Marcel Hunting

#ifndef _DEFINED_LIBIPOPT_H
#define _DEFINED_LIBIPOPT_H

#ifdef WIN32
#include <windows.h>
#endif
#include <stddef.h>
#include <assert.h>

#include "IAimmsOSI.h"							// Header file of AIMMS open solver interface
#include "AimmsIpoptConfig.h"					// Used to handle different linear solvers that may be available

#include "IpoptConfig.h"

// Define _AIMMS390_ if you want to build the AIMMS-IPOPT link for AIMMS 3.9. In that case you should
// use the 'IAimmsOSI.h' and 'aimmsosi_compat.h' files of AIMMS 3.9.

//#define _AIMMS390_

// Some IPOPT-related constants.

#define IPOPT_STATUS_FILE_NAME    "ipopt.sta"

#define IPOPT_INF                 1.0e19      // The default upper bound for IPOPT.  In the future this could be 
											  // connected to the IPOPT option nlp_upper_bound_inf

#define IPOPT_MAXINT              2147483647  // This is the range of a signed, 32-bit long

#define IPOPT_MARG_INF            0.99e19
#define IPOPT_MARG_ZERO           1.0e-5      // If absolute value of marginal is smaller
                                              // than this value then we round it to 0.0

#define ALMOST_ZERO				  1.0e-13
#define BYTES_IN_MB               1048576.0   // 1024 * 1024
#define MAX_DIR_LEN               1000

#define BUG                       assert( 0 == 1 )


#define USE_AIMMS_MEMORY

#ifdef USE_AIMMS_MEMORY
  #define IPOPTmalloc( _size )  \
     m_mp->AllocateMemory( _size )
  #define IPOPTfree( _ptr )     \
     m_mp->FreeMemory( _ptr )
#else
  #define IPOPTmalloc( _size )  \
     malloc( _size );
  #define IPOPTfree( _ptr )     \
     free( _ptr );
#endif


// This global structure is used to transfer problem-related information from AIMMS to IPOPT.
// Following solution by IPOPT, it is used to transfer solution data back to AIMMS.
typedef struct IPOPT_handle_record 
{
    // Actual parameters for IPOPT
    
    _LONG_T     ncols;                 // Number of variables
    _LONG_T     nrows;                 // Number of constraints
    _LONG_T     nonzeros;              // Number of non-zeros in Jacobian
    _LONG_T     n_nl_cols;
    _LONG_T     n_nl_rows;
    _LONG_T     n_lin_elem;            // Number of linear nonzeros
    _LONG_T     nhess;
    _LONG_T     hess_present;          // If 1, Hessian is available
    int         n;
    int         m;
    _LONG_T     ncols_fix;             // Number of fixed variables

    _LONG_T     direction;
    _LONG_T     obj_colno;
    _LONG_T     obj_rowno;
    _LONG_T     basis;
    double      obj_constant;          // Value of constant in AIMMS objective
    bool        obj_is_nonlinear;      // If 1, objective is nonlinear, otherwise linear
    bool        is_linear_model;       // If 1, model is linear
    bool        is_quadratic_model;    // If 1, model has quadratic objective or constraints

    _LONG_T     model_status;          // Model status number
    _LONG_T     solver_status;         // Solver status number
    double      obj;                   // Objective function value
    _LONG_T     iter;                  // Total number of iterations performed
    double      sum_inf;               // Sum primal infeasibilities
    double      constr_viol;           // Current constraint violation
    int         dom_err_cnt;           // Number of domain evaluation errors
    int         mem_in_use;            // Current total memory in use by IPOPT

    double     *model_area;            // Pointer to buffer area containing final variables and slacks,
                                       // dual variables, reduced costs etc.
    double     *obj_area;              // Pointer to buffer area containing objective row elements
    double     *jac_area;              // Pointer to buffer area containing Jacobian elements
    double     *linear_area;           // YDL: Pointer for linear terms
    double     *solution;              // Pointer to buffer for solution returned by IPOPT
 
} IPOPT_handle_struct;


// Needed for definition of SmartPtr<IpoptApplication>, which is used in the 
// IPOPT_Set_Options() function.
#include "IpIpoptApplication.hpp"
#include "IpoptProblemInstance.hpp"

// Include OSI files.

#include "aimmsosi_compat.h"
#include "AimmsOSI-SolverIpoptImpl.h"


#ifdef WIN32
#define SPRINTF _stprintf
#define STRICMP _stricmp
#else // WIN32
#define SPRINTF sprintf
#define STRICMP strcasecmp
#endif // WIN32

#endif // _DEFINED_LIBIPOPT_H
