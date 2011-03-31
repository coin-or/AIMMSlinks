// Copyright (C) 2009 Paragon Decision Technology B.V. and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// $Id$
//
// Authors: Yidong Lang, L.T. Biegler, Chuck Teeter, Marcel Hunting

// Suppress warning messages associated with older CRT library functions such as sprintf() and strcpy()
#define _CRT_SECURE_NO_WARNINGS
#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif

// Includes necessary IPOPT header files
#include <stdio.h>
#include <math.h>
#include "libIpopt.h"							// Ipopt Interface header file


//@< The function |GetSolverInfo()| @> @;
//@< The function |IpoptMathProgramInstance()| @> @;
//@< The function |IpoptMathProgramInstance::Delete()| @> @;
//@< The function |IpoptSolverInfo()| @> @;
//@< The function |IpoptSolverInfo::Delete()| @> @;

//@< The function |IPOPT_Init_Handle()| @> @;
//@< The function |IPOPT_Error()| @> @;

//@< The function |RegisterAsSolver()| @> @;
//@< The function |GetInitialInfo()| @> @;
//@< The function |GetOptionsInfo()| @> @;
//@< The function |GetVersion()| @> @;
//@< The function |GetHelpFile()| @> @;
//@< The function |GetLogFile()| @> @;
//@< The function |Shutdown()| @> @;

//@< The function |Reset()| @> @;
//@< The function |DoSolve()| @> @;
//@< The function |GetColumnSolution()| @> @;
//@< The function |GetRowSolution()| @> @;
//@< The function |FindIIS()| @> @;
//@< The function |GetIIS()| @> @;
//@< The function |GetSensitivityData()| @> @;
//@< The function |SetCallbackMoments()| @> @;
//@< The function |DoTuning()| @> @;
//@< The function |DoMpsSolve()| @> @;
//@< The function |CalculateSubgradient()| @> @;

//******** Solve model ********
//@< The function |IPOPT_Solve_Model()| @> @;
//@< The function |IPOPT_Initialize_Solve_Parameters()| @> @;
//@< The function |IPOPT_Get_Model()| @> @;
//@< The function |IPOPT_Get_Objective_Data()| @> @;
//@< The function |IPOPT_Get_Jacobian_Map()| @> @;
//@< The function |IPOPT_Progress_Info()| @> @;
//@< The function |IPOPT_Get_Solution_Status()| @> @;
//@< The function |IPOPT_Is_Feasible_Solution()| @> @;

//******** Tracing ********
//@< The function |print_parameters()| @> @;
//@< The function |print_columns()| @> @;
//@< The function |print_objective()| @> @;
//@< The function |print_rows()| @> @;
//@< The function |print_elem()| @> @;
//@< The function |print_solution_stats()| @> @;



// Global variables.

char    ipopt_project_dir[512];            // Buffer for AIMMS project directory

// Debug related and logging variables.

#ifdef DEBUG
FILE   *IPOPT_logfile = NULL;              // Logfile associated with this API
int     ipopt_solve_no = 0;                // Solving number
#endif



//@< The function |GetSolverInfo()| @> @;
extern "C" AimmsOSI_API(ISolverInfo *) GetSolverInfo(IAimmsSolverConnection *gen)
{
    return new IpoptSolverInfo(gen);
}


//@< The function |IpoptMathProgramInstance()| @> @;
IpoptMathProgramInstance::IpoptMathProgramInstance(IAimmsMathProgramInfo *mp, IAimmsSolverConnection *gen)
        : m_gen(gen)
        , m_mp(mp)
        , m_evaluator(mp->GetNonLinearEvaluator())
        , m_callback(mp->GetSolverCallback())
        , IPOPT_mp_init(false)
{
    memset( &IPOPT_handle, 0, sizeof(IPOPT_handle) );
    
    ipopt_opt_priority = AOSI_PRIO_ERROR;
}


//@< The function |IpoptMathProgramInstance::Delete()| @> @;
void IpoptMathProgramInstance::Delete(void)
{
    IPOPT_Init_Handle();
    
    delete this;
}


//@< The function |IpoptSolverInfo()| @> @;
ISolverMathProgramInstance *IpoptSolverInfo::CreateMathProgramInstance(IAimmsMathProgramInfo *cb)
{
	return new IpoptMathProgramInstance(cb, m_gen);
}


//@< The function |IpoptSolverInfo::Delete()| @> @;
void IpoptSolverInfo::Delete(void)
{       
    delete this;
}


//@< The function |IPOPT_Init_Handle()| @> @;
void IpoptMathProgramInstance::IPOPT_Init_Handle( void )
{
    if ( IPOPT_handle.model_area     ) {
		IPOPTfree( IPOPT_handle.model_area     );
	}
	
    if ( IPOPT_handle.solution       ) {
		IPOPTfree( IPOPT_handle.solution       );
	}

	// CET: Added because this wasn't being done in the IPOPT 3.0 link code
	if ( IPOPT_handle.linear_area    ) {
		IPOPTfree( IPOPT_handle.linear_area    );
	}
	
	if ( IPOPT_handle.obj_area       ) {
		IPOPTfree( IPOPT_handle.obj_area       );
	}
	
	if ( IPOPT_handle.jac_area       ) {
		IPOPTfree( IPOPT_handle.jac_area       );
	}
 
    memset( &IPOPT_handle, 0, sizeof( IPOPT_handle_struct ) );
    
    IPOPT_handle.model_status  = AOSI_MODELSTAT_NOT_SOLVED;
    IPOPT_handle.solver_status = AOSI_SOLVERSTAT_NOT_CALLED;
    IPOPT_handle.obj           = AOSI_NA_REP;
    IPOPT_handle.mem_in_use    = 0;
}


//@< The function |IPOPT_Error()| @> @;
void IpoptMathProgramInstance::IPOPT_Error( const char* errmsg )
{
    char      buffer[256];
    _LONG_T   prior;

    prior = AOSI_PRIO_ERROR;

    if ( ipopt_opt_priority >= prior ) {
        sprintf( buffer, "IPOPT error: %s\n", errmsg );
                         m_gen->PassMessage( prior, buffer );
    }

    IPOPT_handle.model_status  = AOSI_MODELSTAT_NO_SOLUTION;
    IPOPT_handle.solver_status = AOSI_SOLVERSTAT_SETUP_ERROR;
}


//@< The function |RegisterAsSolver()| @> @;
int IpoptSolverInfo::RegisterAsSolver( char* name, _LONG_T* model_flags )
{
    // This function returns the name of the solver and the model types that
    // the solver can handle. The latter is done by setting the appropriate
    // model flags.

    // This function is (only) called when the solver is added to the 'Solver
    // Configuration' of AIMMS.
    
    // We are not interested in the sub-sub version number but only the sub version
    // number (e.g., 3.9 instead of 3.9.1).
    
    char version[16];
    
    sprintf( version, "%s\0", IPOPT_VERSION );
    
    char *str1 = strstr( version, "." );
    if ( str1 ) {
    	if ( strlen(str1) >= 2 ) {
    		char *str2 = strstr( &str1[1], "." );
    		if ( str2 ) {
    			str2[0] = '\0';
    		}
    	}
    }

	// Set the solver name for AIMMS.
    sprintf( name, "IPOPT %s\0", version );
    
	// The supported model types are LP, NLP, Quadratic, and Quadratically constrained programs.
    *model_flags = AOSI_MODEL_LP               |
                   AOSI_MODEL_NLP              |
                   AOSI_MODEL_QUADRATIC        |
                   AOSI_MODEL_CONSTR_QUADRATIC ;
	
    return AOSI_SUCCESS;
}


//@< The function |GetInitialInfo()| @> @;
_LONG_T IpoptSolverInfo::GetInitialInfo( _LONG_T* info_int, double* info_dbl, char* msg )
{
    // This function provides information to AIMMS about the solver capabilities
    // and about the solver options.

    // This function is called when this solver is made default for some model type
    // (possibly when AIMMS is started up). It is also called when the solver is
    // added to the 'Solver Configuration' of AIMMS.
    
    // On input, info_int[ AOSI_IINFO_VERSION ] and info_int[ AOSI_IINFO_VERSION + 1 ]
    // contain the current AOSI_VERSION number. If the check belows fails then this
    // interface version does not match the current AIMMS version.
	
    if ( info_int[ AOSI_IINFO_VERSION ] != info_int[ AOSI_IINFO_VERSION + 1 ] ||
         info_int[ AOSI_IINFO_VERSION ] != AOSI_VERSION                       ) {
        sprintf( msg, "Solver version (%d) does not match AIMMS version (%d)",
                      AOSI_VERSION, info_int[ AOSI_IINFO_VERSION ] );
        m_gen->PassMessage( AOSI_PRIO_ALWAYS, msg );
        info_int[ AOSI_IINFO_VERSION ] = AOSI_VERSION;
        return AOSI_SUCCESS;
    }

    // AIMMS checks if the included IAimmsOSI.h file matches the current AIMMS
    // version. Therefore, you should fill in AOSI_VERSION.
    
    info_int[ AOSI_IINFO_VERSION       ] = AOSI_VERSION;
	
	// Specify the solver capability flags:
	// - IPOPT cannot handle updates and therefore the capability flag AOSI_CAPAB_UPDATES
    //   is not set.
    // - IPOPT gives no information about intermediate solutions and therefore the
    //   capability flag AOSI_CAPAB_SOLUTION is not set.

    info_int[ AOSI_IINFO_CAPAB_FLAGS   ] = AOSI_CAPAB_RANGED_ROWS   |
                                           AOSI_CAPAB_OBJ_SPARSE    |
                                           AOSI_CAPAB_SOLUTION      |
                                           AOSI_CAPAB_QP_NOT_SPEC   |
                                           AOSI_CAPAB_MATRIX_MANIP  |
                                           AOSI_CAPAB_USE_HESSIAN   |
                                           AOSI_CAPAB_OBJ_ROW_REDUC |
                                           AOSI_CAPAB_MULTI_PROB    |
                                           AOSI_CAPAB_NO_EMPTY_JAC  ;
    
    info_int[ AOSI_IINFO_CAPAB2_FLAGS  ] = AOSI_CAPAB2_NEW_OSI      ;

    info_int[ AOSI_IINFO_NOPT_CAT      ] = IPOPT_CAT_MAX;		// Must be >= 1
    info_int[ AOSI_IINFO_NOPT_INT      ] = IPOPT_OPT_INT_MAX;
    info_int[ AOSI_IINFO_NOPT_DBL      ] = IPOPT_OPT_DBL_MAX;
    
    if ( ! ipopt_options_ordered ) {
    	IPOPT_order_options();
    }

    // Some solvers (not IPOPT) report messages about certain constraints and
    // variables. For those solvers a column row mapping is useful. To construct such
    // a mapping info_int[ AOSI_IINFO_START_COL ] and info_int[ AOSI_IINFO_START_ROW ]
    // are used. (See: the Solvers General option 'Solver Column Row Mapping'.)
   
    info_int[ AOSI_IINFO_START_COL     ] = 0;             // Not used
    info_int[ AOSI_IINFO_START_ROW     ] = 0;             // Not used

    info_dbl[ AOSI_DINFO_ZERO_TOL      ] =  1.0e-12;
    info_dbl[ AOSI_DINFO_INF           ] =  IPOPT_INF;
    info_dbl[ AOSI_DINFO_MIN_INF       ] = -IPOPT_INF;
    info_dbl[ AOSI_DINFO_INT_INF       ] =  IPOPT_INF;    // Not used
    info_dbl[ AOSI_DINFO_INT_MIN_INF   ] = -IPOPT_INF;    // Not used
    info_dbl[ AOSI_DINFO_SEMI_CONT_INF ] =  IPOPT_INF;    // Not used
    
    return AOSI_SUCCESS;
}


//@< The function |GetOptionsInfo()| @> @;
_LONG_T IpoptSolverInfo::GetOptionsInfo( _LONG_T *info_int, double *info_dbl )
{
    // This initialization routine is called by AIMMS to obtain information about
    // the solver options.

    // This function is called when the 'Project Options' tree is opened.

    info_int[ AOSI_IINFO_VERSION  ] = AOSI_VERSION;
    info_int[ AOSI_IINFO_NOPT_CAT ] = IPOPT_CAT_MAX;       // Must be > 0
    info_int[ AOSI_IINFO_NOPT_INT ] = IPOPT_OPT_INT_MAX;
    info_int[ AOSI_IINFO_NOPT_DBL ] = IPOPT_OPT_DBL_MAX;
    
    if ( ! ipopt_options_ordered ) {
    	IPOPT_order_options();
    }

    return AOSI_SUCCESS;
}


//@< The function |GetVersion()| @> @;
void IpoptSolverInfo::GetVersion(
    char    *version,           // version must be of length < 256
    _LONG_T  size )             // size of the buffer including '\0'
{
	// Version must be of length < 256
    char   buf[256];		// Size of the buffer including '\0'

    // This function places the version number of the solver (or the solver
    // interface), if any, in a character buffer provided by AIMMS. The
    // returned string will be used in the 'About AIMMS' menu option.

    sprintf( buf, "%s (DLL)", IPOPT_VERSION );

    strncpy( version, buf, size );
    version[ size - 1 ] = '\0';
}


//@< The function |GetHelpFile()| @> @;
void IpoptSolverInfo::GetHelpFile(
    char    *filename,         // filename must be of length < 512
    _LONG_T  size )            // size of the buffer including '\0'
{
    // There if no help file for IPOPT (yet).

	strncpy( filename, "aimmsipopt.chm", size );
	filename[ size - 1 ] = '\0';
}


//@< The function |GetLogFile()| @> @;
_LONG_T IpoptSolverInfo::GetLogFile( _TCHAR *file_name )
{
#ifdef _AIMMS390_
	strcpy( (char *) file_name, IPOPT_STATUS_FILE_NAME );
#else
	_TCHAR    dir[MAX_DIR_LEN];
    _LONG_T   len = MAX_DIR_LEN;

    m_gen->GetLogDirNameWchar( dir, &len );
    assert( len != -1 ); 
    
    SPRINTF( file_name, _T("%s/%s"), dir, _T(IPOPT_STATUS_FILE_NAME) );
#endif
    
    return AOSI_SUCCESS;
}


//@< The function |Shutdown()| @> @;
void IpoptSolverInfo::Shutdown( void )
{
#ifdef DEBUG
    if ( IPOPT_logfile ) 
	{
        fclose( IPOPT_logfile );
        IPOPT_logfile = NULL;
    }
#endif
}


//@< The function |Reset()| @> @;
void IpoptMathProgramInstance::Reset( void )
{
    // This function removes the optimization task that uniquely corresponds to the
    // math program instance from the solver environment and frees the associated
    // dynamically allocated memory chunks. This function is called when AIMMS has
    // no need for further access to the math program instance.

    // Initialize handle. Includes freeing dynamically allocated memory.
    
    IPOPT_Init_Handle();
    
	// Initialize the solver options. When a new problem is solved, a new IPOPT
    // problem instance is created and the options in IPOPT are initialized on
    // their default value. Therefore we have to reset the current option values
    // stored.
    
    IPOPT_Init_Options();
    
    IPOPT_mp_init = false;
}


//@< The function |DoSolve()| @> @;
void IpoptMathProgramInstance::DoSolve(
    _LONG_T  type,            // NEW or UPDATE
    _LONG_T  options,         // 1 if options have to be adapted
    _LONG_T *int_param,       // Integer parameter array of size AOSI_IPARAM_MAX
    double  *dbl_param,       // Double parameter array of size AOSI_DPARAM_MAX
    size_t  *int_stat,        // Integer statistics array of size AOSI_ISTAT_MAX
    double  *dbl_stat )       // Double statistics array of size AOSI_DSTAT_MAX
{
    // The main solve routine. This function should pass the model from AIMMS to
    // the solver, call the solver to solve the problem, and retrieve and pass
    // problem statistics. The arrays int_param and dbl_param (input) contain
    // information about the problem; the arrays int_stat and dbl_stat (output)
    // contain the problem statistics passed to AIMMS.
    
    int   res, mem_used;

    // Initialize statistics.

    int_stat[ AOSI_ISTAT_MODEL_STAT     ] = AOSI_MODELSTAT_NOT_SOLVED;		// Model status
    int_stat[ AOSI_ISTAT_SOLVER_STAT    ] = AOSI_SOLVERSTAT_NOT_CALLED;		// Solver status
    int_stat[ AOSI_ISTAT_TOTAL_ITER     ] = 0;								// Total iterations
    int_stat[ AOSI_ISTAT_NODES          ] = 0;								// The number of MIP nodes (not used)
    int_stat[ AOSI_ISTAT_MEM_USED       ] = 0;								// Total memory used (in bytes)
    int_stat[ AOSI_ISTAT_RESTART_OFF    ] = 1;								// Restart is not currently possible
	int_stat[ AOSI_ISTAT_POSTSOLVE_WARN ] = 0;								// Postsolve warning

    dbl_stat[ AOSI_DSTAT_OBJ            ] = AOSI_NA_REP;


	// Initialize the solver options.
	if ( ! IPOPT_mp_init ) {
		IPOPT_Init_Options();
		
		IPOPT_mp_init = true;
	}

	// Initialize the IPOPT_handle structure and set its members
	// to AIMMS default values.
	IPOPT_Initialize_Solve_Parameters( int_param, dbl_param );

    // Get and store modified global solver and IPOPT specific options.
    if ( options ) {
        IPOPT_Get_Options();
    }


#ifdef DEBUG
    // If requested, open log file.
    
    ipopt_solve_no ++;
    
    if ( ipopt_opt_tracing ) {
        if ( ! IPOPT_logfile ) {
            IPOPT_logfile = fopen( "ipopt.log", "w" );
            if ( IPOPT_logfile == NULL ) {
                IPOPT_Error( "Unable to open log file for tracing" );
                int_stat[ AOSI_ISTAT_MODEL_STAT  ] = AOSI_MODELSTAT_NOT_SOLVED;
                int_stat[ AOSI_ISTAT_SOLVER_STAT ] = AOSI_SOLVERSTAT_NOT_CALLED;
                return;
            } else {
                fprintf( IPOPT_logfile, "Logfile belonging to IPOPT-API\n" );
                fprintf( IPOPT_logfile, "IPOPT version: %s\n", IPOPT_VERSION );
                fflush( IPOPT_logfile );
            }
        }
        
        print_parameters( IPOPT_logfile, options, int_param, dbl_param );
    }
#endif

    // Load model and solve it. If a solvers can not handle updates (i.e. the
    // flag AOSI_CAPAB_UPDATES is NOT set in Aimms2SolverGetInitialInfo) then type will
    // never be AOSI_MODEL_UPDATE (or AOSI_DUAL_MODEL_UPDATE/AOSI_VAR_MODEL_UPDATE).

	// CET: There is a special case to be handled here. Even though 'type' should theoretically never
	// be AOSI_MODEL_UPDATE on the basis of the AOSI_CAPAB_UPDATES flag not being set, it can be
	// if this is the second (or later) solve in an AIMMS procedure. For example, if in the MainInitialization
	// procedure, an LP solve is used to initialize the nonlinear variables, the value of 'type' will indeed be
	// AOSI_MODEL_UPDATE. In this special case, we'll need to reset 'type' to 'AOSI_MODEL_NEW'. This logic will
	// need to change later in the event that IPOPT is capable of handling matrix updates.

	if ( type == AOSI_MODEL_UPDATE ) {
		type = AOSI_MODEL_NEW;
	}

    switch ( type ) {
	  case AOSI_MODEL_NEW:
      case AOSI_DUAL_MODEL_NEW:
      case AOSI_VAR_MODEL_NEW:
        res = IPOPT_Solve_Model( int_param, dbl_param, &mem_used );
        if ( res == AOSI_FAILURE ) {	
			return;
		}
        break;
          
      case AOSI_DUAL_MODEL_END:
      case AOSI_VAR_MODEL_END:
        return;

      default:
        IPOPT_Error( "Illegal model type supplied" );
        int_stat[ AOSI_ISTAT_MODEL_STAT  ] = AOSI_MODELSTAT_NOT_SOLVED;
        int_stat[ AOSI_ISTAT_SOLVER_STAT ] = AOSI_SOLVERSTAT_NOT_CALLED;
        return;

    }

    // Compute statistics.
    
    int_stat[ AOSI_ISTAT_MODEL_STAT  ] = IPOPT_handle.model_status;
    int_stat[ AOSI_ISTAT_SOLVER_STAT ] = IPOPT_handle.solver_status;
    int_stat[ AOSI_ISTAT_TOTAL_ITER  ] = IPOPT_handle.iter;
    int_stat[ AOSI_ISTAT_NODES       ] = 0;
    int_stat[ AOSI_ISTAT_MEM_USED    ] = IPOPT_handle.mem_in_use;
    int_stat[ AOSI_ISTAT_RESTART_OFF ] = 1;           // Restart is not possible
    
    // Like most solvers, IPOPT does not take care of the constant in the AIMMS
    // objective function. AIMMS will add the objective constant to the value
    // for AOSI_DSTAT_OBJ.

    dbl_stat[ AOSI_DSTAT_OBJ         ] = IPOPT_handle.obj;

    if ( type == AOSI_MODEL_NEW ) {
#ifdef _WIN64
        sprintf( IPOPT_msg, "Memory in use by IPOPT %s: %I64d bytes.",
#else
        sprintf( IPOPT_msg, "Memory in use by IPOPT %s: %ld bytes.",
#endif
                            IPOPT_VERSION, int_stat[ AOSI_ISTAT_MEM_USED ] );
        m_gen->PassMessage( AOSI_PRIO_REMARK, IPOPT_msg );
    }

#ifdef DEBUG
    print_solution_stats();
#endif
}


//@< The function |GetColumnSolution()| @> @;
void IpoptMathProgramInstance::GetColumnSolution(
    _LONG_T    first_col,       // Retrieve columns first_col - last_col inclusive
    _LONG_T    last_col,
    _LONG_T    type_flag,       // Indicates which info should be returned
    double    *lev,				// Level values
    double    *mar,				// Marginal values
    _LONG_T   *basic )          // 1 if basic
{
    _LONG_T    i, ncols, level, marginals, base, ncols_model;
    double    *x, *mult_x_L, *mult_x_U, rc;
	
    ncols     = last_col - first_col + 1;
    
    level     = ( type_flag & AOSI_RETURN_SOLUTION_LEV   );
    marginals = ( type_flag & AOSI_RETURN_SOLUTION_MAR   );
    base      = ( type_flag & AOSI_RETURN_SOLUTION_BASIC );

    if ( level ) {
        memset( lev  , 0, ncols * sizeof( double ) );
	}

    if ( marginals ) {
        memset( mar  , 0, ncols * sizeof( double ) );
	}

    if ( base ) {
        memset( basic, 0, ncols * sizeof( _LONG_T   ) );
	}
        
    if ( first_col >= IPOPT_handle.ncols ) {
        return;     // Columns out of range
    }

    if ( IPOPT_handle.solution ) {
        ncols_model = IPOPT_handle.ncols;
        
        x        = (double *) IPOPT_handle.solution;
        mult_x_L = (double *) ( x        + ncols_model );
        mult_x_U = (double *) ( mult_x_L + ncols_model );

        for ( i=first_col; i<=last_col; i++ ) {
            if ( level ) {
                lev[ i - first_col ] = x[ i ];
            }
        
            if ( marginals ) {
                if ( fabs( mult_x_U[ i ] ) < IPOPT_MARG_INF ) {
                    if ( fabs( mult_x_L[ i ] ) < IPOPT_MARG_INF ) {
                        rc = mult_x_L[ i ] - mult_x_U[ i ];
                    } else {
                        rc = mult_x_U[ i ];                  
                    }
                        
                    if ( fabs( rc ) <= IPOPT_MARG_ZERO ) {
                        mar[ i - first_col ] = 0.0;
                    } 
					
					else if ( IPOPT_handle.direction == AOSI_DIRECTION_MAX ) {
                        mar[ i - first_col ] = - rc;
                    } else {
                        mar[ i - first_col ] = rc;
                    }
                } else {
                    assert( fabs( mult_x_L[ i ] ) < IPOPT_MARG_INF );
                    
                    if ( fabs( mult_x_L[i] ) <= IPOPT_MARG_ZERO ) {
                        mar[ i - first_col ] = 0.0;
                    } else if ( IPOPT_handle.direction == AOSI_DIRECTION_MAX ) {
                        mar[ i - first_col ] = - mult_x_L[ i ];
                    } else {
                        mar[ i - first_col ] = mult_x_L[ i ];
                    }
                }
            }
        }
    }
    
#ifdef DEBUG
    char      colname[64];
    _LONG_T   len;

    if ( ipopt_opt_tracing && level ) {
        for ( i=first_col; i<=last_col; i++ ) {
            len = sizeof( colname );
            m_mp->GetRowColumnName( AOSI_NAME_COLUMN, i, colname, &len );
                 
            fprintf( IPOPT_logfile, "column level[%2ld] = %-14.7g   (%s)\n",
                                    i, lev[i-first_col], colname );
        }
        fflush( IPOPT_logfile );
    }
    
    if ( ipopt_opt_tracing && marginals ) {
        fprintf( IPOPT_logfile, "\n" );
        
        for ( i=first_col; i<=last_col; i++ ) {
            len = sizeof( colname );
            m_mp->GetRowColumnName( AOSI_NAME_COLUMN, i, colname, &len );
                 
            fprintf( IPOPT_logfile, "column margi[%2ld] = %-14.7g   (%s)\n",
                                    i, mar[i-first_col], colname );
        }
        fflush( IPOPT_logfile );
    }
#endif

}


//@< The function |GetRowSolution()| @> @;
void IpoptMathProgramInstance::GetRowSolution(
    _LONG_T     first_row,       // Retrieve rows first_row - last_row inclusive
    _LONG_T     last_row,
    _LONG_T     type_flag,       // Indicates which info should be returned
    double     *lev,             // Level values
    double     *mar,             // Marginal values
    _LONG_T    *basic )          // 1 if basic
{
    _LONG_T    i, nrows, level, marginals, base, ncols_model, nrows_model;
    double    *x, *mult_x_L, *mult_x_U, *g, *mult_g;
    
    nrows     = last_row - first_row + 1;
   
    level     = ( type_flag & AOSI_RETURN_SOLUTION_LEV   );
    marginals = ( type_flag & AOSI_RETURN_SOLUTION_MAR   );
    base      = ( type_flag & AOSI_RETURN_SOLUTION_BASIC );
  
    if ( level ) {
        memset( lev  , 0, nrows * sizeof( double ) );
	}

    if ( marginals ) {
        memset( mar  , 0, nrows * sizeof( double ) );
	}

    if ( base ) {
        memset( basic, 0, nrows * sizeof( _LONG_T   ) );
	}
        
    if ( first_row >= IPOPT_handle.nrows ) {
        return;     // Rows out of range
    }

    if ( IPOPT_handle.solution ) {
        ncols_model = IPOPT_handle.ncols;
        nrows_model = IPOPT_handle.nrows;

        x        = (double *) IPOPT_handle.solution;
        mult_x_L = (double *) ( x        + ncols_model );
        mult_x_U = (double *) ( mult_x_L + ncols_model );
        g        = (double *) ( mult_x_U + ncols_model );
        mult_g   = (double *) ( g        + nrows_model );
  
        for ( i=first_row; i<=last_row; i++ ) {
            if ( level ) {
                lev[ i - first_row ] = g[ i ];
            }
            
            if ( marginals ) {
                if ( fabs( mult_g[ i ] ) <= IPOPT_MARG_ZERO ) {
                    mar[ i - first_row ] = 0.0;
                } else {
                    mar[ i - first_row ] = mult_g[ i ];
                }
            }
        }
    }
    
#ifdef DEBUG
    char   rowname[64];
    _LONG_T   len;

    if ( ipopt_opt_tracing && marginals ) {
        fprintf( IPOPT_logfile, "\n" );
        
        for ( i=first_row; i<=last_row; i++ ) {
            len = sizeof( rowname );
            m_mp->GetRowColumnName( AOSI_NAME_ROW, i, rowname, &len );
                 
            fprintf( IPOPT_logfile, "row marginal[%2ld] = %-14.7g   (%s)\n",
                                    i, mar[i-first_row], rowname );
        }
        fflush( IPOPT_logfile );
    }
#endif
}


//@< The function |FindIIS()| @> @;
void IpoptMathProgramInstance::FindIIS( _LONG_T *no_rows, _LONG_T *no_cols )
{
    // IPOPT has no support for calculating the IIS.
    
    *no_rows = 0;
    *no_cols = 0;
}


//@< The function |GetIIS()| @> @;
_LONG_T IpoptMathProgramInstance::GetIIS( _LONG_T no_rows, _LONG_T *rownos, _LONG_T no_cols,
                                          _LONG_T *colnos )
{
    // IPOPT has no support for calculating the IIS.
    
    return AOSI_FAILURE;
}


//@< The function |GetSensitivityData()| @> @;
_LONG_T IpoptMathProgramInstance::GetSensitivityData( _LONG_T first, _LONG_T last, _LONG_T type,
                                                      double *lower, double *upper )
{
	// IPOPT cannot calculate the smallest and largest values for the RHS of a
	// constraint so that the optimal basis remains constant, and therefore the
    // capability flag AOSI_CAPAB_RHS_SENSE is not set.
    
    // IPOPT cannot calculate the smallest and largest values for the objective
	// coefficient of a variable so that the optimal basis remains constant,
    // and therefore the capability flag AOSI_CAPAB_OBJ_SENSE is not set.
    
    // Note that shadow price ranges and variables ranges are calculated by AIMMS
    // (using the solver).
       
    return AOSI_FAILURE;
}


//@< The function |SetCallbackMoments()| @> @;
_LONG_T IpoptMathProgramInstance::SetCallbackMoments( _LONG_T flags, _LONG_T *info )
{
    // IPOPT does not support direct callbacks.
    
    return AOSI_SUCCESS;
}


//@< The function |DoTuning()| @> @;
void IpoptMathProgramInstance::DoTuning(
	_LONG_T  options,
	_LONG_T  int_param[],
	double   dbl_param[],
	size_t   int_stat[],
	double   dbl_stat[],
	_LONG_T  nr_int_opt,
	_LONG_T  int_opt_stat[],
	_LONG_T  int_opt_tuned[],
	_LONG_T  nr_dbl_opt,
	_LONG_T  dbl_opt_stat[],
	double   dbl_opt_tuned[],
	_LONG_T  nr_files,
	_LONG_T  start_pos[],
	_LONG_T  filenames_len,
	char    *filenames )
{
	// Not supported by IPOPT.
}


//@< The function |DoMpsSolve()| @> @;
void IpoptMathProgramInstance::DoMpsSolve(
	_LONG_T  options,
	_LONG_T  int_param[],
	double   dbl_param[],
	size_t   int_stat[],
	double   dbl_stat[],
	_LONG_T  filename_len,
	_TCHAR  *filenameW,
	_LONG_T  create_sol_file )
{
	// Not supported by IPOPT.
}


//@< The function |CalculateSubgradient()| @> @;
_LONG_T IpoptMathProgramInstance::CalculateSubgradient(
	double   delta_rhs[],
	_LONG_T  delta_rhs_len,
	double   subgr[],
	_LONG_T  subgr_len )
{
	// Not supported by IPOPT.
	
	return AOSI_FAILURE;
}


//@< The function |IPOPT_Solve_Model()| @> @;
int IpoptMathProgramInstance::IPOPT_Solve_Model( _LONG_T* int_param, double* dbl_param, int* mem_used )
{
	int nResult;
	
	// Get the actual problem from AIMMS
	nResult = IPOPT_Get_Model();
	if ( nResult ) {		
		// An error occurred wile retrieving the model
	    *mem_used = IPOPT_handle.mem_in_use;
	    return AOSI_FAILURE;
	}
	
	// Create a new instance of the problem to be solved, using a SmartPtr
	SmartPtr<TNLP> NLPProblem = new IpoptProblemInstance( this );
	 
	// Create a new instance of IpoptApplication
	SmartPtr<IpoptApplication> app = IpoptApplicationFactory();
	
	// Set the IPOPT options that have changed from the default values
	// CET: TODO: Do something useful with this return value. Even though 
	// the setting of some individual options could fail, the default options 
	// for the settings will still be used by IPOPT. We can check this value
	// to see if the setting of any individual options failed for some reason.
	nResult = IPOPT_Set_Options( app );
	if ( nResult ) return AOSI_FAILURE;
	 
	// Intialize the IpoptApplication object and process the options.
	ApplicationReturnStatus status;
	status = app->Initialize();
	
	if ( status != Solve_Succeeded ) {
		return AOSI_FAILURE;
	}
	
	// Set the solver phase in the progress window.
	m_callback->PassPhaseInfo( AOSI_PHASE_BARRIER );
	
	// Update the progress window for the first time just before the solve.
	IPOPT_Progress_Info( /* end */ false );
	
	// Ask Ipopt to solve the problem
	status = app->OptimizeTNLP( NLPProblem );
	
	if ( status == Invalid_Option ) {
		IPOPT_handle.model_status  = AOSI_MODELSTAT_NO_SOLUTION;
	  	IPOPT_handle.solver_status = AOSI_SOLVERSTAT_SETUP_ERROR;
	  	IPOPT_handle.obj = AOSI_NA_REP;
      
	  	// Send message back to AIMMS
	  	sprintf( IPOPT_msg, "Invalid option value." );
	  	m_gen->PassMessage( AOSI_PRIO_ALWAYS, IPOPT_msg );
	}
	
	// Update the progress window for the last time after the solve.
	IPOPT_Progress_Info( /* end */ true );
	
	return AOSI_SUCCESS; 
}


//@< The function |IPOPT_Initialize_Solve_Parameters()| @> @;
int IpoptMathProgramInstance::IPOPT_Initialize_Solve_Parameters( _LONG_T* int_param, double* dbl_param )
{
	int   model_type;
	
    IPOPT_Init_Handle();
   
    IPOPT_handle.ncols        = int_param[ AOSI_IPARAM_COLS            ];
    IPOPT_handle.n_nl_cols    = int_param[ AOSI_IPARAM_NL_COLS         ];
    IPOPT_handle.nrows        = int_param[ AOSI_IPARAM_ROWS            ];
    IPOPT_handle.n_nl_rows    = int_param[ AOSI_IPARAM_NL_ROWS         ];
    IPOPT_handle.nonzeros     = int_param[ AOSI_IPARAM_NONZEROS        ];
    IPOPT_handle.nhess        = int_param[ AOSI_IPARAM_HESSIAN_ELEM    ];
    IPOPT_handle.hess_present = int_param[ AOSI_IPARAM_HESSIAN         ];
    IPOPT_handle.direction    = int_param[ AOSI_IPARAM_DIRECTION       ];

    IPOPT_handle.obj_colno    = int_param[ AOSI_IPARAM_OBJ_COL_NO      ];
    IPOPT_handle.basis        = int_param[ AOSI_IPARAM_BASIS           ];
    
    IPOPT_handle.obj_constant = dbl_param[ AOSI_DPARAM_OBJECTIVE_CONST ];
    
    // Determine whether objective is nonlinear.
    
    if ( int_param[ AOSI_IPARAM_OBJ_TYPE ] == AOSI_FLAGS_IS_NONLINEAR ||
         int_param[ AOSI_IPARAM_OBJ_TYPE ] == AOSI_FLAGS_IS_QUADRATIC ) {
        IPOPT_handle.obj_is_nonlinear = true;
    } else {
    	IPOPT_handle.obj_is_nonlinear = false;
    }
    
    // Is model linear or quadratic?
    
    model_type = m_mp->GetModelType();
    
    if ( model_type == AOSI_MODEL_LP ) {
    	IPOPT_handle.is_linear_model    = true;
    	IPOPT_handle.is_quadratic_model = false;
    } else if ( model_type == AOSI_MODEL_QUADRATIC        ||
                model_type == AOSI_MODEL_CONSTR_QUADRATIC ) {
    	IPOPT_handle.is_linear_model    = false;
		IPOPT_handle.is_quadratic_model = true;
	} else {
		IPOPT_handle.is_linear_model    = false;
		IPOPT_handle.is_quadratic_model = false;
	}
	
	IPOPT_handle.dom_err_cnt = 0;
    
    // Initialize current constraint violation to INF.
    IPOPT_handle.constr_viol = IPOPT_INF;

    return 0;
}


//@< The function |IPOPT_Get_Model()| @> @;
int IpoptMathProgramInstance::IPOPT_Get_Model( void )
{

    int       i, ncols, nrows, nelem, nlin, mem_size, nhess, mem_size_sol;
    double   *lb, *ub, *lev, *row_lb, *row_ub, *row_lev, *fv, *coef, *obj_coef;
    _LONG_T  *cols, *rows, *col_flag, *row_flag, *nlflag, number;
    int      *rows_lin, *cols_lin, *lin_ind;
    double   *coef_lin;
    int      *hrows, *hcols;
    int       cnt_lin;

    ncols = IPOPT_handle.ncols;           // Number of columns
    nrows = IPOPT_handle.nrows;           // Number of rows
    nelem = IPOPT_handle.nonzeros;        // Number of nonzero in Jacobian
    nhess = IPOPT_handle.nhess;           // Number of nonzero in Hessian
    
    mem_size = ncols * sizeof( double  )	// lb
             + ncols * sizeof( double  )	// ub
             + ncols * sizeof( double  )	// lev
             + ncols * sizeof( _LONG_T )	// col_flag
             + nrows * sizeof( double  )	// row_lb
             + nrows * sizeof( double  )	// row_ub
             + nrows * sizeof( double  )	// row_lev
             + nrows * sizeof( double  )	// fv
             + nrows * sizeof( _LONG_T )	// row_flag
             + nelem * sizeof( _LONG_T )	// cols
             + nelem * sizeof( _LONG_T )	// rows
             + nelem * sizeof( double  )	// coef
             + nelem * sizeof( _LONG_T )	// nlflag

             // YDL : 10-12-2004 Hessian information included here.
             + nhess * sizeof( int     )    // hcols
             + nhess * sizeof( int     )    // hrows
             + 1000  * sizeof( double  );   // for debuging

    // Area model_area includes information of model:
    // - bounds and current values of variable bounds
    // - current values and nonlinearities of constraints
    // - Jacobian information
    // - Hessian information

    IPOPT_handle.model_area = (double *) IPOPTmalloc( mem_size );
    
    if ( IPOPT_handle.model_area == NULL ) {
        sprintf( IPOPT_msg, "Not enough memory for loading model; amount of memory: "
                            "%ld bytes\n", mem_size + IPOPT_handle.mem_in_use );
        IPOPT_Error( IPOPT_msg );
        return 1;
    }
    
    IPOPT_handle.mem_in_use += mem_size;
    
	// Initialize the pointers to point to applicable parts of the retrieved model
    lb          = (double  *) IPOPT_handle.model_area;       // Cols
    ub          = (double  *) ( lb       + ncols );
    lev         = (double  *) ( ub       + ncols );
    row_lb      = (double  *) ( lev      + ncols );          // Rows
    row_ub      = (double  *) ( row_lb   + nrows );
    row_lev     = (double  *) ( row_ub   + nrows );
    fv          = (double  *) ( row_lev  + nrows );
    coef        = (double  *) ( fv       + nrows );          // Matrix coefs
    col_flag    = (_LONG_T *) ( coef     + nelem );          // Cols
    row_flag    = (_LONG_T *) ( col_flag + ncols );          // Rows
    cols        = (_LONG_T *) ( row_flag + nrows );          // Matrix coefs
    rows        = (_LONG_T *) ( cols     + nelem );
    nlflag      = (_LONG_T *) ( rows     + nelem );

    // YDL : for Hessian information
    hcols       = (int     *) ( nlflag   + nelem );   
    hrows       = (int     *) ( hcols    + nhess );

    // Allocate memory for solution.

    mem_size_sol = ncols * sizeof( double )    // x
                 + ncols * sizeof( double )    // mult_x_L
                 + ncols * sizeof( double )    // mult_x_U
                 + nrows * sizeof( double )    // g
                 + nrows * sizeof( double );   // mult_g

    IPOPT_handle.solution = (double *) IPOPTmalloc( mem_size_sol );
    
    if ( IPOPT_handle.solution == NULL ) {
        sprintf( IPOPT_msg, "Not enough memory for loading model; amount of memory: "
                            "%ld bytes\n", mem_size_sol + IPOPT_handle.mem_in_use );
        IPOPT_Error( IPOPT_msg );
        return 1;
    }
    
    IPOPT_handle.mem_in_use += mem_size_sol;
    
    // Allocate buffer for objective coefficients.
    
    mem_size = ncols * sizeof( double )    // obj_coef
             + ncols * sizeof( int    );   // obj_flag
    
    IPOPT_handle.obj_area = (double *) IPOPTmalloc( mem_size );
    
    if ( IPOPT_handle.obj_area == NULL ) {
        sprintf( IPOPT_msg, "Not enough memory for loading model; amount of memory: "
                            "%ld bytes\n", mem_size + IPOPT_handle.mem_in_use );
        IPOPT_Error( IPOPT_msg );
        return 1;
    }
    
    obj_coef = (double *) IPOPT_handle.obj_area;
    
    IPOPT_handle.mem_in_use += mem_size;
    
    // Retrieve matrix from AIMMS row wise.
    m_mp->SetMatrixOrder( AOSI_ROW_WISE );
    
    if ( ncols != m_mp->GetColumnData( /* is_update */ 0, ncols, &number,
                                       lb, lev, ub,
                                       obj_coef,
                                       /* col_prior */ NULL,
                                       /* no_elm    */ NULL,
                                       /* no_q_elm  */ NULL,
                                       col_flag ) ) {
        IPOPT_Error( "Retrieving column values failed" );
        return 1;
    }

#ifdef DEBUG
    print_columns( ncols, number, lb, lev , ub );
#endif
 
    for ( i=0; i<ncols; i++ ) {
        if ( lb[ i ] < -IPOPT_INF ) lb[ i ] = -IPOPT_INF;
        if ( ub[ i ] >  IPOPT_INF ) ub[ i ] =  IPOPT_INF;
    }

    if ( nrows != m_mp->GetRowData( /* is_update */ 0, nrows, &number,
                                    row_lb, row_lev, row_ub,
                                    /* no_elm    */ NULL,
                                    /* fv        */ NULL,
                                    row_flag ) ) {
        IPOPT_Error( "Retrieving row values failed" );
        return 1;
    }
    
    for ( i=0; i<nrows; i++ ) {
        if ( row_lb[ i ] < -IPOPT_INF ) row_lb[ i ] = -IPOPT_INF;
        if ( row_ub[ i ] >  IPOPT_INF ) row_ub[ i ] =  IPOPT_INF;
    }

#ifdef DEBUG
    print_rows( nrows, number, row_lb, row_ub, fv );
#endif
 
    if ( nelem != m_mp->GetMatrixData( /* is_update */ 0, nelem, rows, cols, coef, nlflag ) ) {
        IPOPT_Error( "Retrieving matrix nonzeros failed" );
        return 1;
    }
 
#ifdef DEBUG
    print_elem( nelem, cols, rows, coef, nlflag );
#endif
  
    // Get number of linear and nonlinear coefficients.

    nlin = 0;    // Number of linear matrix coefficients

    for ( i=0; i<nelem; i++ ) {
        if ( ! nlflag[ i ] ) {
            nlin ++;
        }
    }
    
    IPOPT_handle.n_lin_elem = nlin;
    
    // Get pattern of Hessian.
    if ( nhess > 0 ) {
        if ( m_evaluator->GetHessianSparsityPattern( nhess, hrows, hcols ) == AOSI_FAILURE ) {
            IPOPT_Error( "Retrieving Hessian pattern failed" );
            return 1;
        }
    }
 
    ////////////////////////////////////////////////////////////////////////////
    // YDL :  Allocate additional memories.
    ////////////////////////////////////////////////////////////////////////////
    
    nlin = IPOPT_handle.n_lin_elem;
    
    // Allocate additional memory for the linear coefficients.
    
    mem_size = nlin * sizeof( double )    // coef_lin
             + nlin * sizeof( int    )    // rows_lin
             + nlin * sizeof( int    )    // cols_lin
             + nlin * sizeof( int    );   // lin_ind
              
    IPOPT_handle.linear_area = (double *) IPOPTmalloc( mem_size );

    if ( IPOPT_handle.linear_area == NULL ) {
        sprintf( IPOPT_msg, "Not enough memory for loading model; amount of memory: "
                            "%ld bytes\n", mem_size + IPOPT_handle.mem_in_use );
        IPOPT_Error( IPOPT_msg );
        return 1;
    }
    
    IPOPT_handle.mem_in_use += mem_size;

    coef_lin = (double *) IPOPT_handle.linear_area;
    rows_lin = (int    *) ( coef_lin + nlin );
    cols_lin = (int    *) ( rows_lin + nlin );
    lin_ind  = (int    *) ( cols_lin + nlin );

    // Collect linear nonzeros.
    
    cnt_lin = 0;

    for ( i=0; i<nelem; i++ ) {
        if ( ! nlflag[ i ] ) {
			// Linear nonzero
            rows_lin[ cnt_lin ] = rows[ i ];
            cols_lin[ cnt_lin ] = cols[ i ];
            coef_lin[ cnt_lin ] = coef[ i ];
            lin_ind[ cnt_lin ]  = i;

            cnt_lin ++;
        }
    }
  
    assert( cnt_lin == nlin );
    
    // Allocate buffer for Jacobian elements (excl. objective).
    
    mem_size = (nrows+1) * sizeof( int    )    // jac_start
             + nelem     * sizeof( int    )    // jac_map
             + nrows     * sizeof( bool   );   // lin_row_ind
    
    IPOPT_handle.jac_area = (double *) IPOPTmalloc( mem_size );
    
    if ( IPOPT_handle.jac_area == NULL ) {
        sprintf( IPOPT_msg, "Not enough memory for loading model; amount of memory: "
                            "%ld bytes\n", mem_size + IPOPT_handle.mem_in_use );
        IPOPT_Error( IPOPT_msg );
        return 1;
    }
    
    IPOPT_handle.mem_in_use += mem_size;
    
    // Collect data for linear elements that will be used to speed-up the evaluation
    // of the Jacobian.
    
    // Get linear elements in objective.

    if ( IPOPT_handle.direction != AOSI_DIRECTION_FEAS ) {
    	IPOPT_Get_Objective_Data( col_flag );
    }
    
    // Get linear elements in rest of matrix.
    
    IPOPT_Get_Jacobian_Map( rows, cols, coef, nlflag, row_flag );

    return 0;
}


//@< The function |IPOPT_Get_Objective_Data()| @> @;
void IpoptMathProgramInstance::IPOPT_Get_Objective_Data( _LONG_T *col_flag )
{
	int      *obj_flag, i, ncols;
    double   *obj_coef;
    
    // Collect the linear Jacobian elements in the objective. This information is used
    // for the evaluation of the objective value (in eval_f) and the Jacobian for the
    // objective (in eval_grad_f).
    
    ncols = IPOPT_handle.ncols;           // Number of columns
	
	obj_coef = (double *) IPOPT_handle.obj_area;
    obj_flag = (int    *) ( obj_coef + ncols );
    
    for ( i=0; i<ncols; i++ ) {
    	obj_flag[i] = ( col_flag[i] & AOSI_FLAGS_IS_NONLINEAR ) ? 1 : 0;
    }

#ifdef DEBUG
    print_objective( ncols, obj_coef, obj_flag );
#endif
}


//@< The function |IPOPT_Get_Jacobian_Map()| @> @;
void IpoptMathProgramInstance::IPOPT_Get_Jacobian_Map( _LONG_T *rows, _LONG_T *cols, double *coef,
                                                       _LONG_T *nlflag, _LONG_T *row_flag )
{
	int      *jac_start, *jac_map, i, nrows, nelem, cnt = 0;
	bool     *lin_row_ind;
	
	// Collect the linear Jacobian elements for each row. This information is used
    // for the evaluation of the Jacobian (in Aimms_Calculate_Jacobian).
    
    nrows = IPOPT_handle.nrows;           // Number of rows
    nelem = IPOPT_handle.nonzeros;        // Number of matrix elements
	
	jac_start   = (int  *) IPOPT_handle.jac_area;
    jac_map     = (int  *) ( jac_start + (nrows+1) );
    lin_row_ind = (bool *) ( jac_map   + nelem     );
    
    for ( i=0; i<nrows; i++ ) {
    	jac_start[i] = -1;
    }
    
    for ( i=0; i<nelem; i++ ) {
    	if ( jac_start[rows[i]] == -1 ) {
    	    jac_start[rows[i]] = i;
    	}
    	jac_map[i] = cols[i];
    }
    jac_start[nrows] = nelem;
    
    // Determine the linear rows.
   	
    for ( i=0; i<nrows; i++ ) {
    	if ( ( row_flag[i] & AOSI_FLAGS_IS_NONLINEAR ) ||
    	     ( row_flag[i] & AOSI_FLAGS_IS_QUADRATIC ) ) {
    		lin_row_ind[i] = false;
    	} else {
    		lin_row_ind[i] = true;
    	}
    }
}


//@< The function |IPOPT_Progress_Info()| @> @;
void IpoptMathProgramInstance::IPOPT_Progress_Info( bool end )
{

	size_t   prog_int[ AOSI_PROG_IMAX ];
	double   prog_dbl[ AOSI_PROG_DMAX ];
	
	// Handle progress differently if LP/QP rather than NLP.
	
	// If the constraint violation is small enough the model is considered as
	// being feasible.
	// The initial constraint violation is set to INF and therefore the first
	// progress update before the actual solve starts will start with infeasibility.
	
	if ( IPOPT_handle.constr_viol >= ipopt_opt_constr_viol_tol ) {
		if ( IPOPT_handle.is_linear_model || IPOPT_handle.is_quadratic_model ) {
			// Linear model
			prog_int[ AOSI_PROG_TYPE ] = AOSI_SOL_TYPE_LP_INFEAS;
		} else {
			// NLP
			prog_int[ AOSI_PROG_TYPE ] = AOSI_SOL_TYPE_NLP_INFEAS;
		}
	} else {
		if ( IPOPT_handle.is_linear_model || IPOPT_handle.is_quadratic_model ) {
			// Linear model
			prog_int[ AOSI_PROG_TYPE ] = AOSI_SOL_TYPE_LP_FEAS;
		} else {
			// NLP
			prog_int[ AOSI_PROG_TYPE ] = AOSI_SOL_TYPE_NLP_FEAS;
		}
	}
	
	// Iteration count.
	prog_int[ AOSI_PROG_ITER ]			= IPOPT_handle.iter;

	// Flag indicating whether this is the last call to AIMMS.
	prog_int[ AOSI_PROG_END ]			= end;

	// Memory used.
	prog_int[ AOSI_PROG_MEM_USED ]		= IPOPT_handle.mem_in_use;
	
	// Like most solvers, IPOPT does not take care of the constant in the AIMMS
    // objective function. Therefore we have to add the objective constant here.
	
	if ( IPOPT_handle.obj == AOSI_NA_REP ) {
		prog_dbl[ AOSI_PROG_OBJ ]		= IPOPT_handle.obj;
		prog_dbl[ AOSI_PROG_OBJ_BEST ]	= IPOPT_handle.obj;
	} else {
		prog_dbl[ AOSI_PROG_OBJ ]		= IPOPT_handle.obj + IPOPT_handle.obj_constant;
		prog_dbl[ AOSI_PROG_OBJ_BEST ]	= IPOPT_handle.obj + IPOPT_handle.obj_constant;
	}
	
	// Sum of (primal) infeasibilities.
	prog_dbl[ AOSI_PROG_SUM_INFEAS ]	= IPOPT_handle.sum_inf;
	
	// Update the AIMMS progress window.
	
	m_callback->PassProgressInfo( prog_int, prog_dbl );
}


//@< The function |IPOPT_Get_Solution_Status()| @> @;
void IpoptMathProgramInstance::IPOPT_Get_Solution_Status( int status, double *x_sol, double *g_sol )
{
	int   is_feas;
	
	// Get model and solver status.
	
	// First check whether the solve was stopped due to hitting the limit for evaluation
	// errors.
	
	if ( IPOPT_handle.dom_err_cnt > ipopt_opt_dom_limit ) {
		is_feas = IPOPT_Is_Feasible_Solution( x_sol, g_sol );
		if ( is_feas ) {
        	IPOPT_handle.model_status  = AOSI_MODELSTAT_INTER_NON_OPT;
        } else {
        	IPOPT_handle.model_status  = AOSI_MODELSTAT_INTER_INFEAS;
        	IPOPT_handle.obj = AOSI_NA_REP;
        }
        IPOPT_handle.solver_status = AOSI_SOLVERSTAT_EVAL_ERROR;
        
        // Pass message back to AIMMS
        sprintf( IPOPT_msg, "Too many function evaluation errors." );
        m_gen->PassMessage( AOSI_PRIO_WARNING, IPOPT_msg );
        return;
    }
	
    switch ( status ) {
	  // EXIT: Optimal solution found.
	  case SUCCESS:
	  	IPOPT_handle.model_status  = AOSI_MODELSTAT_LOCAL_OPT;
	  	IPOPT_handle.solver_status = AOSI_SOLVERSTAT_NORMAL_END;
	  	break;
      
	  // EXIT: Maximum number of iterations exceeded.
	  // Here the number of iterations has exceeded the number
	  // specified by the option 'max_iter'.
	  case MAXITER_EXCEEDED:
	  	is_feas = IPOPT_Is_Feasible_Solution( x_sol, g_sol );
	  	if ( is_feas ) {
	  		IPOPT_handle.model_status  = AOSI_MODELSTAT_INTER_NON_OPT;
	  	} else {
	  		IPOPT_handle.model_status  = AOSI_MODELSTAT_INTER_INFEAS;
	  		IPOPT_handle.obj = AOSI_NA_REP;
	  	}
	  	IPOPT_handle.solver_status = AOSI_SOLVERSTAT_ITERATION_INT;
      
	  	// Pass message back to AIMMS
	  	sprintf( IPOPT_msg, "Solve terminated because maximum number of iterations were exceeded." );
	  	m_gen->PassMessage( AOSI_PRIO_WARNING, IPOPT_msg );
	  	break;
	  
	  // EXIT: CPU time exceeded.
	  case CPUTIME_EXCEEDED:
	  	is_feas = IPOPT_Is_Feasible_Solution( x_sol, g_sol );
	  	if ( is_feas ) {
	  		IPOPT_handle.model_status  = AOSI_MODELSTAT_INTER_NON_OPT;
	  	} else {
	  		IPOPT_handle.model_status  = AOSI_MODELSTAT_INTER_INFEAS;
	  		IPOPT_handle.obj = AOSI_NA_REP;
	  	}
	  	IPOPT_handle.solver_status = AOSI_SOLVERSTAT_RESOURCE_INT;
      
	  	// Pass message back to AIMMS
	  	sprintf( IPOPT_msg, "Solve terminated because maximum CPU time was exceeded." );
	  	m_gen->PassMessage( AOSI_PRIO_WARNING, IPOPT_msg );
	  	break;
      
	  // EXIT: Search Direction is becoming Too Small.
	  // This indicates that Ipopt is calculating very small step sizes and
	  // making very little progress. This could happen if the problem has been
	  // solved to the best numerical accuracy possible given the current scaling.
	  case STOP_AT_TINY_STEP:
	  	is_feas = IPOPT_Is_Feasible_Solution( x_sol, g_sol );
	  	if ( is_feas ) {
	  		IPOPT_handle.model_status  = AOSI_MODELSTAT_INTER_NON_OPT;
	  		IPOPT_handle.solver_status = AOSI_SOLVERSTAT_NORMAL_END;
	  	} else {
	  		IPOPT_handle.model_status  = AOSI_MODELSTAT_INTER_INFEAS;
	  		IPOPT_handle.solver_status = AOSI_SOLVERSTAT_TERMINATED;
	  		IPOPT_handle.obj = AOSI_NA_REP;
     		
	  		// Pass message back to AIMMS
	  		sprintf( IPOPT_msg, "Solve terminated because of very little progress." );
	  		m_gen->PassMessage( AOSI_PRIO_WARNING, IPOPT_msg );
	  	}
	  	break;
      
	  // EXIT: Solved To Acceptable Level.
	  // This indicates that the algorithm did not converge to the "desired" tolerances,
	  // but that it was able to obtain a point satisfying the "acceptable" tolerance level
	  // as specified by 'acceptable-*' options. This may happen if the desired tolerances
	  // are too small for the current problem.
	  case STOP_AT_ACCEPTABLE_POINT:
	  	IPOPT_handle.model_status  = AOSI_MODELSTAT_INTER_NON_OPT;
	  	IPOPT_handle.solver_status = AOSI_SOLVERSTAT_NORMAL_END;
     	
	  	// Pass message back to AIMMS
	  	sprintf( IPOPT_msg, "Acceptable solution fro which the requested accuracy could not be achieved." );
	  	m_gen->PassMessage( AOSI_PRIO_WARNING, IPOPT_msg );      
	  	break;
      
	  // EXIT: Converged to a point of local infeasibility. Problem may be infeasible.
	  case LOCAL_INFEASIBILITY:
	  	IPOPT_handle.model_status  = AOSI_MODELSTAT_INFEASIBLE;
	  	IPOPT_handle.solver_status = AOSI_SOLVERSTAT_NORMAL_END;
	  	IPOPT_handle.obj = AOSI_NA_REP;
	  	break;
      
	  // EXIT: Stopping optimization at current point as requested by user.
	  // This happens when the call-back method Intermediate_callback() returns
	  // 'false'.
	  case USER_REQUESTED_STOP:
		is_feas = IPOPT_Is_Feasible_Solution( x_sol, g_sol );
	  	if ( is_feas ) {
	  		IPOPT_handle.model_status  = AOSI_MODELSTAT_INTER_NON_OPT;
	  		IPOPT_handle.solver_status = AOSI_SOLVERSTAT_USER_INT;
	  	} else {
	  		IPOPT_handle.model_status  = AOSI_MODELSTAT_INTER_INFEAS;
	  		IPOPT_handle.solver_status = AOSI_SOLVERSTAT_USER_INT;
	  		IPOPT_handle.obj = AOSI_NA_REP;
	  	}
	  	break;
      
	  // EXIT: Feasible point for square problem found.
	  // This message is received if the problem is square (NLS) and Ipopt has
	  // found a feasible point.
	  case FEASIBLE_POINT_FOUND:
	  	IPOPT_handle.model_status  = AOSI_MODELSTAT_LOCAL_OPT;
	  	IPOPT_handle.solver_status = AOSI_SOLVERSTAT_NORMAL_END;
	  	break;
      
	  // EXIT: Iterates diverging; problem might be unbounded.
	  // This message is received if the max-norm of the iterates becomes larger
	  // than the value of the option 'diverging_iterates_tol'. This can
	  // happen if the problem is unbounded below and the iterates are diverging.
	  case DIVERGING_ITERATES:
	  	IPOPT_handle.model_status  = AOSI_MODELSTAT_NO_SOLUTION;
	  	IPOPT_handle.solver_status = AOSI_SOLVERSTAT_FAILURE;
	  	IPOPT_handle.obj = AOSI_NA_REP;
      
	  	// Pass message back to AIMMS
	  	sprintf( IPOPT_msg, "Solver terminated due to diverging iterates." );
	  	m_gen->PassMessage( AOSI_PRIO_WARNING, IPOPT_msg );
	  	break;
      
	  // EXIT: Restoration Failed!
	  // This indicates that the restoration phase failed to find a feasible point that
	  // was acceptable to the filter line search for the original problem. This can happen
	  // if the problem is highly degenerate, does not satisfy the constraint qualification,
	  // or if the NLP provides incorrect derivative information.
	  case RESTORATION_FAILURE:
	  	is_feas = IPOPT_Is_Feasible_Solution( x_sol, g_sol );
	  	if ( is_feas ) {
	  		IPOPT_handle.model_status  = AOSI_MODELSTAT_INTER_NON_OPT;
	  		sprintf( IPOPT_msg, "The model is highly degenerate, but IPOPT found a feasible solution." );
	  	} else {
	  		IPOPT_handle. model_status  = AOSI_MODELSTAT_INTER_INFEAS;
	  		IPOPT_handle.obj = AOSI_NA_REP;
	  		sprintf( IPOPT_msg, "The model is highly degenerate and IPOPT could not find a feasible solution." );
	  	}
	  	IPOPT_handle.solver_status = AOSI_SOLVERSTAT_TERMINATED;
      
	  	// Send message back to AIMMS
	  	m_gen->PassMessage( AOSI_PRIO_WARNING, IPOPT_msg );
	  	break;
      
	  // EXIT: Error in step computation (regularization becomes too large?)!
	  // This message is received if Ipopt is unable to compute a search direction,
	  // despite several attempts to modify the iteration matrix. Usually, the value
	  // of the regularization parameter becomes too large. This may happen if
	  // values in the Hessian are invalid (NaN or Inf). You can check whether this
	  // is true by using the 'check_derivatives_for_naninf' option.
	  case ERROR_IN_STEP_COMPUTATION:
	  	IPOPT_handle.model_status  = AOSI_MODELSTAT_NO_SOLUTION;
	  	IPOPT_handle.solver_status = AOSI_SOLVERSTAT_FAILURE;
	  	IPOPT_handle.obj = AOSI_NA_REP;
		
	  	// Pass message back to AIMMS
	  	sprintf( IPOPT_msg, "Solver terminated due to error in step computation." );
	  	m_gen->PassMessage( AOSI_PRIO_WARNING, IPOPT_msg );
	  	break;
      
	  // EXIT: Problem has too few degrees of freedom.
	  // This indicates that the problem, as specified, contains too few degrees of freedom.
	  // This can happen if you have too many equality constraints, or if you fix too many variables.
	  case TOO_FEW_DEGREES_OF_FREEDOM:
	  	IPOPT_handle.model_status  = AOSI_MODELSTAT_NO_SOLUTION;
	  	IPOPT_handle.solver_status = AOSI_SOLVERSTAT_SETUP_ERROR;
	  	IPOPT_handle.obj = AOSI_NA_REP;
      
	  	// Send message back to AIMMS
	  	sprintf( IPOPT_msg, "The model does not have enough degrees of freedom." );
	  	m_gen->PassMessage( AOSI_PRIO_WARNING, IPOPT_msg );
	  	break;
      
	  // EXIT: Not enough memory.
	  // An error occurred while trying to allocate memory.
	  case OUT_OF_MEMORY:
	  	IPOPT_handle.model_status  = AOSI_MODELSTAT_NO_SOLUTION;
	  	IPOPT_handle.solver_status = AOSI_SOLVERSTAT_RESOURCE_INT;
	  	IPOPT_handle.obj = AOSI_NA_REP;
      
	  	// Send message back to AIMMS
	  	sprintf( IPOPT_msg, "Not enough memory for solving the model." );
	  	m_gen->PassMessage( AOSI_PRIO_WARNING, IPOPT_msg );
	  	break;
      
	  // EXIT: INTERNAL ERROR: Unknown SolverReturn value - Notify IPOPT Authors.
	  case INTERNAL_ERROR:                
	  default:
	  	IPOPT_handle.model_status  = AOSI_MODELSTAT_NO_SOLUTION;
	  	IPOPT_handle.solver_status = AOSI_SOLVERSTAT_FAILURE;
	  	IPOPT_handle.obj = AOSI_NA_REP;
		
	  	// Send message back to AIMMS
	  	sprintf( IPOPT_msg, "An internal IPOPT error occurred - notify IPOPT authors." );
	  	m_gen->PassMessage( AOSI_PRIO_WARNING, IPOPT_msg );
	  	break;
    }
}


//@< The function |IPOPT_Is_Feasible_Solution()| @> @;
int IpoptMathProgramInstance::IPOPT_Is_Feasible_Solution( double* x, double* rowlev )
{
    int        i, ncols, nrows, nelem;
    double    *lb, *ub, *lev, *row_lb, *row_ub, *row_lev, *fv, *coef, col_feas_tol,
               row_feas_tol;
    _LONG_T   *col_flag, *row_flag;
#ifdef RLS_TRACE
    char       colname[64], rowname[64];
    _LONG_T    len;
#endif

	// This procedure determines whether obtained solution is feasible.
    
    ncols = IPOPT_handle.ncols;             // Number of columns
    nrows = IPOPT_handle.nrows;             // Number of rows
    nelem = IPOPT_handle.nonzeros;          // Number of matrix elements
    
    lb       = (double  *) IPOPT_handle.model_area;       // Cols
    ub       = (double  *) ( lb       + ncols );
    lev      = (double  *) ( ub       + ncols );
    row_lb   = (double  *) ( lev      + ncols );          // Rows
    row_ub   = (double  *) ( row_lb   + nrows );
    row_lev  = (double  *) ( row_ub   + nrows );
    fv       = (double  *) ( row_lev  + nrows );
    coef     = (double  *) ( fv       + nrows );          // Matrix coefs
    col_flag = (_LONG_T *) ( coef     + nelem );          // Cols
    row_flag = (_LONG_T *) ( col_flag + ncols );          // Rows
    
    // Feasibility tolerance for the columns.
    col_feas_tol = ipopt_opt_constr_viol_tol;
    
    // Feasibility tolerance for rows.
    row_feas_tol = ipopt_opt_constr_viol_tol;
    
    // Check column level values.
    
    for ( i=0; i<ncols; i++ ) {
        if ( ( double ) x[ i ] < lb[ i ] - col_feas_tol )  {
#ifdef RLS_TRACE
            len = sizeof( colname );
            Solver2AimmsGetRowColumnName( AOSI_NAME_COLUMN, i, colname, &len );
            sprintf( IPOPT_msg, "Lower bound violated for column %s: x = %-14.7g, "
                                "lb = %-14.7g, col_feas_tol = %-14.7g\n",
                                colname, x[ i ], lb[ i ], col_feas_tol );

            m_gen->PassMessage( AOSI_WARNING, IPOPT_msg );
#endif

            return 0;   // Column level value outside bounds
        }
        
        if ( ( double ) x[ i ] > ub[ i ] + col_feas_tol ) {
#ifdef RLS_TRACE
            len = sizeof( colname );
            Solver2AimmsGetRowColumnName( AOSI_NAME_COLUMN, i, colname, &len );
            sprintf( IPOPT_msg, "Upper bound violated for column %s: x = %-14.7g, "
                                "ub = %-14.7g, col_feas_tol = %-14.7g\n",
                                colname, x[i], ub[i], col_feas_tol );

            m_gen->PassMessage( AOSI_WARNING, IPOPT_msg );
#endif

            return 0;   // Column level value outside bounds
        }
    }
    
    for ( i=0; i<nrows; i++ ) {
        if ( i != IPOPT_handle.obj_rowno ) {
            if ( ( double ) rowlev[ i ] < row_lb[ i ] - row_feas_tol ) {
#ifdef RLS_TRACE
                len = sizeof( rowname );
                Solver2AimmsGetRowColumnName( AOSI_NAME_ROW, i, rowname, &len );
                sprintf( IPOPT_msg, "Lower bound violated for row %s: lev = %-14.7g, "
                                    "lb = %-14.7g, row_feas_tol = %-14.7g\n",
                                    rowname, rowlev[i], row_lb[i], row_feas_tol );

                m_gen->PassMessage( AOSI_WARNING, IPOPT_msg );
#endif
                return 0;          // Level value of linear row outside bounds
            }
            
            if ( ( double ) rowlev[ i ] > row_ub[ i ] + row_feas_tol ) {
#ifdef RLS_TRACE
                len = sizeof( rowname );
                Solver2AimmsGetRowColumnName( AOSI_NAME_ROW, i, rowname, &len );
                sprintf( IPOPT_msg, "Upper bound violated for row %s: lev = %-14.7g, "
                                    "ub = %-14.7g, row_feas_tol = %-14.7g\n",
                                    rowname, rowlev[i], row_ub[i], row_feas_tol );

                m_gen->PassMessage( AOSI_WARNING, IPOPT_msg );
#endif
                return 0;          // Level value of linear row outside bounds
            }
        }
    }
    
    return 1;
}


#ifdef DEBUG
//@< The function |print_parameters()| @> @;
void IpoptMathProgramInstance::print_parameters( FILE *f, _LONG_T options, _LONG_T *int_param, double *dbl_param )
{
    fprintf( f, "Options                                   : %ld\n",
             options );
    fprintf( f, "Optimization direction                    : %s\n",
             ( int_param[ IPARAM_DIRECTION ] == DIRECTION_MIN ) ? "Minimize" : "Maximize" );
    fprintf( f, "Objective constant                        : %12.5e\n",
             dbl_param[ DPARAM_OBJECTIVE_CONST ] );
    fprintf( f, "Objective column number                   : %ld\n",
             int_param[ IPARAM_OBJ_COL_NO ] );
    fprintf( f, "Number of rows                            : %ld\n",
             int_param[ IPARAM_ROWS ] );
    fprintf( f, "Number of nonlinear rows                  : %ld\n",
             int_param[ IPARAM_NL_ROWS ] );
    fprintf( f, "Number of columns                         : %ld\n",
             int_param[ IPARAM_COLS ] );
    fprintf( f, "Number of nonlinear columns               : %ld\n",
             int_param[ IPARAM_NL_COLS ] );
    fprintf( f, "Number of non-zeros                       : %ld\n",
             int_param[ IPARAM_NONZEROS ] );
    fprintf( f, "Number of nonlinear non-zeros             : %ld\n",
             int_param[ IPARAM_NL_NONZEROS ] );
    fprintf( f, "Number of non-zeros of obj.               : %ld\n",
             int_param[ IPARAM_OBJ_NZ ] );
    fprintf( f, "Basis provided                            : %ld\n",
             int_param[ IPARAM_BASIS ] );
    fprintf( f, "Number of equality rows                   : %ld\n",
             int_param[ IPARAM_EQUAL_R ] );
    fprintf( f, "Maximum elements in a row                 : %ld\n",
             int_param[ IPARAM_MAX_ELEM_R ] );
    fprintf( f, "Maximum elements in a column              : %ld\n",
             int_param[ IPARAM_MAX_ELEM_C ] );
    fprintf( f, "Solve number                              : %ld\n",
             ipopt_solve_no );

    fflush( f );
}


//@< The function |print_columns()| @> @;
void IpoptMathProgramInstance::print_columns( int ncols, _LONG_T number, double *lb, double *lev, double *ub )
{
    _LONG_T   i, len;
    char      colname[64];

    // Print retrieved columns.

    if ( ipopt_opt_tracing ) {
        for ( i=0; i<ncols; i++ ) {
            len = sizeof( colname );
            m_mp->GetRowColumnName( AOSI_NAME_COLUMN, number + i, colname, &len );        

            fprintf( IPOPT_logfile, "colno %ld ",  number + i );
            if ( lb[i] <= -IPOPT_INF ) {
                fprintf( IPOPT_logfile, "lb %16.8e ",  -1e80 );
            } else {
                fprintf( IPOPT_logfile, "lb %16.8e ",  lb[i] );
            }
            if ( ub[i] >= IPOPT_INF ) {
                fprintf( IPOPT_logfile, "ub %16.8e ",  1e80 );
            } else {
                fprintf( IPOPT_logfile, "ub %16.8e ",  ub[i] );
            }
            fprintf( IPOPT_logfile, "lev %16.8e ", lev[i] );
            fprintf( IPOPT_logfile, "   (%s)", colname );
            fprintf( IPOPT_logfile, "\n" );
        }

        fflush( IPOPT_logfile );
    }
}


//@< The function |print_objective()| @> @;
void IpoptMathProgramInstance::print_objective( int ncols, double *obj_coef, int *obj_flag )
{
    _LONG_T   i, len;
    char      colname[64];

    // Print retrieved objective coefficients.

    if ( ipopt_opt_tracing ) {
        for ( i=0; i<ncols; i++ ) {
            len = sizeof( colname );
            m_mp->GetRowColumnName( AOSI_NAME_COLUMN, i, colname, &len );        

            fprintf( IPOPT_logfile, "colno %ld ",  i );
            fprintf( IPOPT_logfile, "obj_coeff %16.8e (flag %ld)", obj_coef[i], obj_flag[i] );
            fprintf( IPOPT_logfile, "   (%s)", colname );
            fputs( "\n", IPOPT_logfile );
        }
        fflush( IPOPT_logfile );
    }
}


//@< The function |print_rows()| @> @;
void IpoptMathProgramInstance::print_rows( int nrows, _LONG_T number, double *row_lb, double *row_ub, double *fv )
{
    _LONG_T   i, len;
    char      rowname[64];

    // Print retrieved rows.
    
    if ( ipopt_opt_tracing ) {
        for ( i=0; i<nrows; i++ ) {
            len = sizeof( rowname );
            m_mp->GetRowColumnName( AOSI_NAME_ROW, number + i, rowname, &len );
            
            fprintf( IPOPT_logfile, "rowno %ld ",  number + i );
            if ( row_lb[i] <= -IPOPT_INF ) {
                fprintf( IPOPT_logfile, "lb %16.8e ",  -1e80 );
            } else {
                fprintf( IPOPT_logfile, "lb %16.8e ",  row_lb[i] );
            }
            if ( row_ub[i] >= IPOPT_INF ) {
                fprintf( IPOPT_logfile, "ub %16.8e ",  1e80 );
            } else {
                fprintf( IPOPT_logfile, "ub %16.8e ",  row_ub[i] );
            }
            fprintf( IPOPT_logfile, "   (%s)", rowname );
            fprintf( IPOPT_logfile, "\n" );
        }

        fflush( IPOPT_logfile );
    }
}


//@< The function |print_elem()| @> @;
void IpoptMathProgramInstance::print_elem( int nelem, _LONG_T *cols, _LONG_T *rows, double *coef, _LONG_T *nlflag )
{
    _LONG_T   i, len;
    char      colname[64], rowname[64];

    // Print retrieved matrix nonzeros.

    if ( ipopt_opt_tracing ) {
        for ( i=0; i<nelem; i++ ) {
            len = sizeof( colname );
            m_mp->GetRowColumnName( AOSI_NAME_COLUMN, cols[i], colname, &len );
            len = sizeof( rowname );
            m_mp->GetRowColumnName( AOSI_NAME_ROW, rows[i], rowname, &len );        
     
            fprintf( IPOPT_logfile, "matrix element (%ld,%ld) = %16.8e (flag %ld)",
                                    rows[i], cols[i], coef[i], nlflag[i] );
            fprintf( IPOPT_logfile, "   (%s, %s)\n", rowname, colname );
        }
         
        fflush( IPOPT_logfile );
    }
}


//@< The function |print_solution_stats()| @> @;
void IpoptMathProgramInstance::print_solution_stats()
{
    if ( ipopt_opt_tracing ) {
        fprintf( IPOPT_logfile, "\n" );
        fprintf( IPOPT_logfile, "Model status    : %ld\n",  IPOPT_handle . model_status  );
        fprintf( IPOPT_logfile, "Solver status   : %ld\n",  IPOPT_handle . solver_status );
        fprintf( IPOPT_logfile, "Objective value : %.5f\n", IPOPT_handle . obj           );
        fprintf( IPOPT_logfile, "# Iterations    : %d\n",   IPOPT_handle . iter          );
        fprintf( IPOPT_logfile, "\n" );
        fflush( IPOPT_logfile );
    }
}
#endif // DEBUG

