// Copyright (C) 2009 Paragon Decision Technology B.V. and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// $Id$
//
// Author: Marcel Hunting

#ifndef _DEFINED_CBC_INTERFACE_H
#define _DEFINED_CBC_INTERFACE_H

#ifdef WIN32
#include <windows.h>
#endif // WIN32
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <list>
#include <assert.h>

#include "IAimmsOSI.h"                      // Header file of AIMMS open solver interface

#ifdef CBC_PARALLEL
#include "MutexImpl.h"                      // For mutex
#endif

#include "CbcModel.hpp"
#include "OsiClpSolverInterface.hpp"        // Using CLP as the LP solver
#include "CbcEventHandler.hpp"
#include "CbcBranchActual.hpp"              // For CbcSOS


#define BUG                assert( 0 == 1 )

#define MAX_DIR_LEN        1000
#define EPS_REP            1.0e-150
#define AIMMS_INF          1.0e20
#define CBC_INF            1.0e30
#define CBC_INT_MAX        COIN_INT_MAX
#define CBC_MM_MAX_ROWS    100              // Maximal number of row updates for
                                            // matrix manipulation in each call
#define CBC_MM_MAX_COLS    100              // Maximal number of column updates for
                                            // matrix manipulation in each call
#define CBC_MM_MAX_ELEM    100              // Maximal number of matrix elements updates
                                            // for matrix manipulation in each call

#if CBC_VERSION_MAJOR == 2 && CBC_VERSION_MINOR == 8 && CBC_VERSION_RELEASE == 0
#define CBC_VERSION_NO        280
#else
#define CBC_VERSION_NO        275
#endif

#if CBC_VERSION_NO == 280
#define CBC_STATUS_FILE_NAME_ASC  "cbc280.sta"
#define CBC_STATUS_FILE_NAME_UNI  _T("cbc280.sta")
#define CBC_LOG_FILE_NAME     "cbc280.log"
#define CBC_HELP_FILE_NAME    "AimmsCbc280.chm"
#else
#define CBC_STATUS_FILE_NAME_ASC  "cbc.sta"
#define CBC_STATUS_FILE_NAME_UNI  _T("cbc.sta")
#define CBC_LOG_FILE_NAME     "cbc.log"
#define CBC_HELP_FILE_NAME    "AimmsCbc.chm"
#endif

#define CBC_LP_MODEL          0
#define CBC_MIP_MODEL         1
#define CBC_RMIP_MODEL        2

#define CBC_OBJECTIVE_SENSE_MINIMIZE    1.0
#define CBC_OBJECTIVE_SENSE_MAXIMIZE   -1.0
#define CBC_OBJECTIVE_SENSE_NONE        0.0

#define CBC_STATUS_SUPERBASIC    0   // Superbasic
#define CBC_STATUS_BASIC         1   // Basic
#define CBC_STATUS_NONBASIC_UB   2   // Non-basic at upper bound
#define CBC_STATUS_NONBASIC_LB   3   // Non-basic at lower bound

//#define CBC_PRINT_POSTSOLVE_MPS


typedef struct cbc_handle_record {
    _LONG_T  ncols;                 // Number of columns
    _LONG_T  nrows;                 // Number of rows
    _LONG_T  nonzeros;              // Number of nonzeros
    
    _LONG_T  ncols_upd;             // Number of updated columns
    _LONG_T  nrows_upd;             // Number of updated rows
    _LONG_T  nonzeros_upd;          // Number of updated nonzeros
    
    _LONG_T  model_status;          // Model status number
    _LONG_T  solver_status;         // Solver status number
    _LONG_T  model_type;            // Model type: LP or MIP or ...
    _LONG_T  direction;             // Optimization direction
                                    // -1 : minimize objective function
                                    //  0 : find a feasible solution
                                    //  1 : maximize objective function
#if CBC_VERSION_NO >= 280
    _LONG_T  prev_direction;        // Optimization direction of previous solve
                                    // -1 : minimize objective function
                                    //  0 : find a feasible solution
                                    //  1 : maximize objective function
#endif
    _LONG_T  obj_col_no;            // Number of objective column (if any)
    _LONG_T  basis;                 // If 1, the provided basis is used
    _LONG_T  update;                // If 1, an updated version is solved
    bool     solved;                // If 1, model is successfully solved
    double   marginal_zero;         // Special value denoting a marginal value of zero
                                    // of a non-basic row or column
    
    double   obj_constant;          // Value of constant in AIMMS objective
    double   objval;                // Objective function value
    double   obj_best;              // Objective function value of best integer solution MIP
    double   mip_best_poss;         // LP value of best remaining node in MIP
    double   obj_multiplier;        // Objective multiplier
    int      iter;                  // Total number of iterations performed
    int      nodes;                 // Total number of nodes in branch-and-bound tree
    int      nodes_left;            // Total number of nodes left in branch-and-bound tree
    _LONG_T  phase;                 // Solver phase
    bool     is_feasible;           // If 1, problem is feasible (or relaxed problem in
                                    // case of MIP)
    bool     mip_is_feasible;       // If 1, MIP problem is feasible
    size_t   start_mem_avail;       // Virtual memory available at start of solve
    bool     finished;              // If 1, solve is finished
    bool     mip_solve_as_lp;       // If 1, then MIP problem is solved as RMIP
    
    _LONG_T  cb_flags;              // Callback flags
    _LONG_T  cb_iter;               // If callback flag AOSI_CB_ITERATION is set then
                                    // callback should be called each cb_iter iter
    bool     cb_lp_var_error;       // If 1 then we already passed error to AIMMS for
                                    // retrieving current LP values
    
	size_t   mps_type;              // Type of problem in case of MPS solve
	
#ifdef CBC_PARALLEL
	PDTThread::MutexControl   *m_Mutex;
#endif
} cbc_handle_struct;


// Debugging and logging variables.

extern int     cbc_seq_number;               // Sequence number of output files
extern char    cbc_project_dir[512];         // Buffer for AIMMS project directory

// Debug related variables.

#ifdef DEBUG
extern FILE   *cbc_logfile;                  // Logfile associated with this API
extern int     cbc_solve_no;                 // Solving number
extern int     cbc_last_opt_tracing;         // Last value of a cbc_opt_tracing
#endif

// CBC functions.

extern void __cdecl saveSolution(const ClpSimplex * lpSolver,std::string fileName);


// Include OSI files.

#include "aimmsosi_compat.h"
#include "AimmsOSI-SolverCbcImpl.h"


#ifdef WIN32
#define SPRINTF _stprintf
#define STRICMP _stricmp
#else // WIN32
#define SPRINTF _stprintf
#define STRICMP strcasecmp
#endif // WIN32

#ifdef UNICODE
    #define STRCPY_UNI_TO_ASC( _asc, _uni )  \
            wcstombs( _asc, _uni, _tcsclen(_uni)+1 )
#else // UNICODE
    #define STRCPY_UNI_TO_ASC( _asc, _uni ) \
            strcpy( _asc, _uni )
#endif // UNICODE

#endif // _DEFINED_CBC_INTERFACE_H

