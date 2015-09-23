// Copyright (C) 2009 AIMMS B.V. and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// $Id$
//
// Author: Marcel Hunting

#include "cbc_interface.h"
#include "cbc_options.h"
#include "cbc_event.h"

  


// The function GetSolverInfo()
// The function CbcMathProgramInstance()
// The function CbcMathProgramInstance::Delete()
// The function CbcSolverInfo()
// The function CbcSolverInfo::Delete()

// The function cbc_error()
// The function cbc_init_handle()
// The function cbc_get_memory_used()
// The function cbc_progress_info()

//******** Callbacks ********
// The function cbc_install_callback()
// The function cbc_aimms_callback()

//******** Tracing ********
// The function cbc_open_log_file_and_print_parameters()
// The function cbc_print_parameters()
// The function cbc_print_retrieved_columns()
// The function cbc_print_column_updates()
// The function cbc_print_deleted_columns()
// The function cbc_print_retrieved_rows()
// The function cbc_print_row_updates()
// The function cbc_print_deleted_rows()
// The function cbc_print_retrieved_matrix_elements()
// The function cbc_check_col_bounds()
// The function cbc_check_row_bounds()
// The function cbc_check_matrix_elements()
// The function cbc_is_feasible_solution()

// The function cbc_init_postsolve()
// The function cbc_end_postsolve()
// The function cbc_write_mps()

//******** Passing model data ********
// The function cbc_load_branching_priorities()
// The function cbc_load_SOS()
// The function cbc_load_model()
// The function cbc_update_model()
// The function cbc_mm_load_deleted_rows()
// The function cbc_mm_load_new_rows()
// The function cbc_mm_load_changed_row_bounds()
// The function cbc_mm_load_deleted_columns()
// The function cbc_mm_load_new_columns()
// The function cbc_mm_load_changed_column()
// The function cbc_mm_load_changed_obj_coef()
// The function cbc_mm_load_changed_matrix_elements()
// The function cbc_mm_change_model_direction()
// The function cbc_mm_change_model_type()
// The function cbc_update_mm_model()

//******** Solve model ********
// The function cbc_create_optimization_task()
// The function cbc_init_solve()
// The function cbc_get_model_type()
// The function cbc_get_solution_info_lp()
// The function cbc_get_solution_info_mip()
// The function cbc_actually_call_solver()
// The function cbc_solve_model()
// The function cbc_call_solver()

// The function RegisterAsSolver)
// The function GetInitialInfo()
// The function GetOptionsInfo()
// The function GetVersion()
// The function GetHelpFile()
// The function GetLogFile()
// The function Shutdown()

// The function Reset()
// The function DoSolve()
// The function cbc_get_col_sol_during_callback()
// The function GetColumnSolution()
// The function GetRowSolution()
// The function FindIIS()
// The function GetIIS()
// The function GetSensitivityData()
// The function SetCallbackMoments()
// The function DoTuning()
// The function cbc_load_mps_file()
// The function cbc_solve_mps()
// The function DoMpsSolve()
// The function CalculateSubgradient()


// Global variables.

int     cbc_seq_number;                  // Sequence number of output files
char    cbc_project_dir[512];            // Buffer for AIMMS project directory

// Debug related and logging variables.

#ifdef DEBUG
FILE   *cbc_logfile = NULL;              // Logfile associated with this API
int     cbc_solve_no = 0;                // Solving number
int     cbc_last_opt_tracing = 0;        // Last value of a cbc_opt_tracing
#endif



// The function GetSolverInfo()
extern "C" AimmsOSI_API(ISolverInfo *) GetSolverInfo(IAimmsSolverConnection *gen)
{
    return new CbcSolverInfo(gen);
}


// The function CbcMathProgramInstance()
CbcMathProgramInstance::CbcMathProgramInstance(IAimmsMathProgramInfo *mp, IAimmsSolverConnection *gen)
        : m_gen(gen)
        , m_mp(mp)
        , m_manipulation(mp->GetMatrixManipulationInfo())
        , m_callback(mp->GetSolverCallback())
        , cbc_model(0)
        , clp_solver(0)
        , cbc_mp_init(false)
        , cbc_args(0)
{
    memset( &cbc_handle, 0, sizeof(cbc_handle) );
    
    cbc_current = &cbc_handle;
    
    cbc_user_interrupt   = false;
    cbc_opt_priority     = AOSI_PRIO_ERROR;
    cbc_inside_postsolve = false;
}


// The function CbcMathProgramInstance::Delete()
void CbcMathProgramInstance::Delete(void)
{
    // Delete optimization task.
    
    if ( cbc_model ) {
        delete cbc_model;
        cbc_model = NULL;
    }
    
    if ( clp_solver ) {
    	delete clp_solver;
    	clp_solver = NULL;
    }
    
    if ( cbc_args ) {
    	delete[] cbc_args;
    	cbc_args = NULL;
    }
	   
    delete this;
}


// The function CbcSolverInfo()
ISolverMathProgramInstance *CbcSolverInfo::CreateMathProgramInstance(IAimmsMathProgramInfo *mp)
{
	return new CbcMathProgramInstance(mp, m_gen);
}


// The function CbcSolverInfo::Delete()
void CbcSolverInfo::Delete(void)
{       
    delete this;
}


// The function cbc_error()
void CbcMathProgramInstance::cbc_error( char *errmsg )
{
    char      buffer[256];
    _LONG_T   prior;
	
    prior = AOSI_PRIO_ERROR;
	
    if ( cbc_opt_priority >= prior ) {
        sprintf( buffer, "CBC error: %s\n", errmsg );
        m_gen->PassMessage( prior, buffer );

#ifdef DEBUG
    	if ( cbc_opt_tracing ) {
        	fprintf( cbc_logfile, "%s", buffer );
    	}
#endif
    }

    cbc_handle . model_status  = MODELSTAT_NO_SOLUTION;
    cbc_handle . solver_status = SOLVERSTAT_SETUP_ERROR;
}


// The function cbc_init_handle()
void CbcMathProgramInstance::cbc_init_handle()
{
    cbc_handle . ncols            = 0;
    cbc_handle . nrows            = 0;
    cbc_handle . nonzeros         = 0;
    cbc_handle . ncols_upd        = 0;
    cbc_handle . nrows_upd        = 0;
    cbc_handle . nonzeros_upd     = 0;
    cbc_handle . model_status     = MODELSTAT_NOT_SOLVED;
    cbc_handle . solver_status    = SOLVERSTAT_NOT_CALLED;
    cbc_handle . direction        = 0;
#if CBC_VERSION_NO >= 280
    cbc_handle . prev_direction   = 0;
#endif
    cbc_handle . obj_col_no       = -1;
    cbc_handle . basis            = 0;
    cbc_handle . update           = 0;
    cbc_handle . solved           = false;
    cbc_handle . obj_constant     = 0.0;
    cbc_handle . objval           = AIMMS_INF;
    cbc_handle . obj_best         = AIMMS_INF;
    cbc_handle . mip_best_poss    = AIMMS_INF;
    cbc_handle . obj_multiplier   = 1.0;
    cbc_handle . iter             = 0;
    cbc_handle . nodes            = 0;
    cbc_handle . nodes_left       = 0;
    cbc_handle . phase            = -1;
    cbc_handle . is_feasible      = false;
    cbc_handle . mip_is_feasible  = false;
    cbc_handle . start_mem_avail  = 0;
    cbc_handle . finished         = false;
    cbc_handle . mip_solve_as_lp  = false;
    cbc_handle . cb_flags         = 0;
    cbc_handle . cb_iter          = 0;
    cbc_handle . cb_lp_var_error  = false;
    cbc_handle . mps_type         = 0;
#ifdef CBC_PARALLEL
    cbc_handle . m_Mutex          = NULL;
#endif
}


// The function cbc_get_memory_used()
size_t CbcMathProgramInstance::cbc_get_memory_used( void )
{
#ifdef WIN32
	MEMORYSTATUS   stat;
	size_t         virtual_mem;

    GlobalMemoryStatus( &stat );
    
    if ( cbc_handle . start_mem_avail > stat.dwAvailVirtual ) {
		virtual_mem = (cbc_handle . start_mem_avail - stat.dwAvailVirtual);
	} else {
		virtual_mem = 0;
	}
	
	return virtual_mem;
#else
	return 0;
#endif
}


// The function cbc_progress_info()
void CbcMathProgramInstance::cbc_progress_info( bool end )
{
    double    prog_dbl[ AOSI_PROG_DMAX ];
    double    cbc_obj_val, obj_best, lp_best;
    size_t    prog_int[ AOSI_PROG_IMAX ];
    _LONG_T   type;
    int       is_integer_model;
    
    if ( cbc_handle . model_type == CBC_MIP_MODEL ) {
        is_integer_model = 1;
    } else {
        is_integer_model = 0;
    }
    
    if ( end                          &&
         cbc_handle . mip_solve_as_lp ) {
        // MIP/MIQP problem is solved as RMIP/QP. But progress info should be
        // for MIP/MIQP.

        is_integer_model = 1;
        cbc_handle . mip_is_feasible = cbc_handle . is_feasible;
        cbc_handle . phase           = AOSI_PHASE_MIP;
        cbc_handle . obj_best        = cbc_handle . objval;
    }
    
    cbc_obj_val = cbc_handle . objval;
    
    if ( cbc_handle . phase == AOSI_PHASE_MIP ) {
        if ( cbc_handle . mip_is_feasible ) {
            type = AOSI_SOL_TYPE_MIP_FEAS;
            lp_best = cbc_handle . mip_best_poss;
        } else {
            type = AOSI_SOL_TYPE_MIP_INFEAS;
            lp_best = AOSI_NA_REP;
        }
        
        obj_best = cbc_handle . obj_best;
    } else {
        if ( cbc_handle . is_feasible ) {
            type     = AOSI_SOL_TYPE_LP_FEAS;
            obj_best = cbc_obj_val;
        } else {     // LP still infeasible
            type = AOSI_SOL_TYPE_LP_INFEAS;
    
            if ( cbc_handle . direction == DIRECTION_MIN )
                obj_best = -CBC_INF;
            else if ( cbc_handle . direction == DIRECTION_MAX )
                obj_best =  CBC_INF;
            else
                obj_best = cbc_obj_val;
        }
        
        lp_best = obj_best;
    }

    if ( end && cbc_handle . model_status == MODELSTAT_UNBOUNDED ) {
    	if ( is_integer_model ) {
    		type = AOSI_SOL_TYPE_MIP_UNB;
    	} else {
    		type = AOSI_SOL_TYPE_LP_UNB;
    	}
    }
	
    prog_int[ AOSI_PROG_TYPE          ] = type;
    prog_int[ AOSI_PROG_ITER          ] = cbc_handle . iter;
    prog_int[ AOSI_PROG_NODES_SOLVED  ] = ( is_integer_model || cbc_inside_postsolve ) ?
                                          cbc_handle . nodes : 0;
    prog_int[ AOSI_PROG_NODES_LEFT    ] = -1;   // Not supported by CBC
    prog_int[ AOSI_PROG_END           ] = end;
    prog_int[ AOSI_PROG_MEM_USED      ] = cbc_get_memory_used();
    prog_int[ AOSI_PROG_IP_CROSS_ITER ] = 0;
    prog_int[ AOSI_PROG_THREADS       ] = 1;
    prog_int[ AOSI_PROG_MPS_TYPE      ] = cbc_handle . mps_type;
                                        
    // Like most solvers, CBC does not take care of the constant in the AIMMS objective
    // function. Therefore we have to add the objective constant here.
    
    if ( cbc_obj_val > -AIMMS_INF && cbc_obj_val < AIMMS_INF ) {
        prog_dbl[ AOSI_PROG_OBJ       ] = cbc_obj_val + cbc_handle . obj_constant;
    } else {
        prog_dbl[ AOSI_PROG_OBJ       ] = cbc_obj_val;
    }
    
    if ( obj_best > -AIMMS_INF && obj_best < AIMMS_INF ) {
        prog_dbl[ AOSI_PROG_OBJ_BEST  ] = obj_best + cbc_handle . obj_constant;
    } else {
        prog_dbl[ AOSI_PROG_OBJ_BEST  ] = obj_best;
    }
    
    if ( lp_best > -AIMMS_INF && lp_best < AIMMS_INF ) {
        prog_dbl[ AOSI_PROG_LP_BEST   ] = lp_best + cbc_handle . obj_constant;
    } else {
        prog_dbl[ AOSI_PROG_LP_BEST   ] = lp_best;
    }
    
    // CBC cannot give the sum of the infeasibilities (and therefore the capability
    // flag AOSI_CAPAB_PROGRESS_INF is not set in GetInitialInfo).
    
    prog_dbl[ AOSI_PROG_SUM_INFEAS    ] = 0.0;
    
    m_callback->PassProgressInfo( prog_int, prog_dbl );
}


// The function cbc_install_callback()
void CbcMathProgramInstance::cbc_install_callback()
{
	// Install callback for displaying progress information, and for
	// passing incumbent solutions if callback is installed in AIMMS
	// project (MIP only).
    
    if ( ( cbc_progress > 0          ) ||
         ( cbc_progress_interval > 0 ) ) {
        OsiSolverInterface * solver = cbc_model->solver();
    	OsiClpSolverInterface * clpSolver = dynamic_cast<OsiClpSolverInterface *> (solver);
    	ClpSimplex * clpSimplex = clpSolver->getModelPtr();
    	
    	MyEventHandler eventHandler( this );
    	
    	clpSimplex->passInEventHandler( &eventHandler );
        
    	if ( cbc_handle . model_type == CBC_MIP_MODEL ) {
    		MyEventHandler3 eventHandler3( cbc_model, this );
    		
    		cbc_model->passInEventHandler( &eventHandler3 );
    	}
	}
}


// The function cbc_aimms_callback()
int CbcMathProgramInstance::cbc_aimms_callback( int callback_type )
{
    double    cb_dbl[ AOSI_CB_DMAX ];
    double    cbc_obj_val, obj_best, lp_best;
    _LONG_T   cb_int[ AOSI_CB_IMAX ];
    _LONG_T   mod_status, res;
    
    if ( cbc_handle . phase == AOSI_PHASE_MIP ) {
        if ( cbc_handle . mip_is_feasible ) {
            mod_status = MODELSTAT_INT_SOLUTION;
            lp_best = cbc_handle . mip_best_poss;
        } else {
            mod_status = MODELSTAT_INTER_NON_INT;
            lp_best = AOSI_NA_REP;
        }
        
        obj_best = cbc_handle . obj_best;
        cbc_obj_val = obj_best;
    } else {                                        
        cbc_obj_val = cbc_handle . objval;    

        if ( cbc_handle . is_feasible ) {
            if ( cbc_handle . model_type == CBC_MIP_MODEL ) {
                mod_status = MODELSTAT_INTER_NON_INT;
            } else {
                mod_status = MODELSTAT_INTER_NON_OPT;
            }
            obj_best = cbc_obj_val;
        } else {   // LP still infeasible
            if ( cbc_handle . direction == DIRECTION_MIN )
                obj_best = -CBC_INF;
            else if ( cbc_handle . direction == DIRECTION_MAX )
                obj_best =  CBC_INF;
            else
                obj_best = cbc_obj_val;
                
            mod_status = MODELSTAT_INTER_INFEAS;
        }
        
        lp_best = obj_best;
    }
 
    cb_int[ AOSI_CB_MODEL_STAT    ] = mod_status;
    cb_int[ AOSI_CB_SOLVER_STAT   ] = SOLVERSTAT_UNKNOWN;
    cb_int[ AOSI_CB_ITER          ] = cbc_handle . iter;
    cb_int[ AOSI_CB_SOLVE_TIME    ] = m_mp->GetTime() - cbc_start_time;  // 1/100'th of a sec
    cb_int[ AOSI_CB_NODE          ] = -1;
    
    if ( cbc_handle . model_type == CBC_MIP_MODEL ) {
    	cb_int[ AOSI_CB_NR_NODES  ] = cbc_handle . nodes;
    } else {
    	cb_int[ AOSI_CB_NR_NODES  ] = 0;
    }
    
    cb_int[ AOSI_CB_NR_BR_NODES   ] = -1;
    cb_int[ AOSI_CB_NR_NODES_LEFT ] = -1;
 
    cb_dbl[ AOSI_CB_OBJ           ] = cbc_obj_val;
    cb_dbl[ AOSI_CB_OBJ_BEST      ] = obj_best;
    cb_dbl[ AOSI_CB_LP_BEST       ] = lp_best;
    cb_dbl[ AOSI_CB_NODE_OBJ      ] = AOSI_NA_REP;
	
    res = m_callback->RunCallback( callback_type, cb_int, cb_dbl );
    
    return res;
}


#ifdef DEBUG
// The function cbc_open_log_file_and_print_parameters()
int CbcMathProgramInstance::cbc_open_log_file_and_print_parameters( _LONG_T type, _LONG_T options,
                                                                    _LONG_T *int_param, double *dbl_param )
{
    char   log_file_name[512];
	
    if ( cbc_opt_tracing ) {
        if ( ! cbc_logfile ) {
            sprintf( log_file_name, "%s%s\0", cbc_project_dir, CBC_LOG_FILE_NAME );
            cbc_logfile = fopen( log_file_name, "w" );
            if ( cbc_logfile == NULL ) {
                cbc_error( "Unable to open log file for tracing" );
                return 1;
            } else {
                fprintf( cbc_logfile, "CBC version: %s\n", CBC_VERSION );
                fflush( cbc_logfile );
            }
        }
     
        cbc_print_parameters( options, int_param, dbl_param );
     
        fprintf( cbc_logfile, "Solve number                              : %d\n",
                 cbc_solve_no );
        fflush( cbc_logfile );
    }
    
    return 0;
}


// The function cbc_print_parameters()
void CbcMathProgramInstance::cbc_print_parameters( _LONG_T options, _LONG_T *int_param, double *dbl_param )
{
	FILE   *f = cbc_logfile;
	
    fprintf( f, "Options                                   : %d\n",
             options );
    fprintf( f, "Number of rows                            : %d\n",
             int_param[ IPARAM_ROWS ] );
    fprintf( f, "Number of update rows                     : %d\n",
             int_param[ IPARAM_UPD_R ] );
    fprintf( f, "Number of SOS 1 rows                      : %d\n",
             int_param[ IPARAM_SOS1_R ] );
    fprintf( f, "Number of SOS 2 rows                      : %d\n",
             int_param[ IPARAM_SOS2_R ] );
    fprintf( f, "Number of columns                         : %d\n",
             int_param[ IPARAM_COLS ] );
    fprintf( f, "Number of updated columns                 : %d\n",
             int_param[ IPARAM_UPD_C ] );
    fprintf( f, "Number of integer columns                 : %d\n",
             int_param[ IPARAM_INT_C ] );
    fprintf( f, "Number of semi-continuous columns         : %d\n",
             int_param[ IPARAM_SEMI_C ] );
    fprintf( f, "Number of semi-continuous integer columns : %d\n",
             int_param[ IPARAM_SEMI_INT_C ] );
    fprintf( f, "Number of non-zeros                       : %d\n",
             int_param[ IPARAM_NONZEROS ] );
    fprintf( f, "Number of non-zeros of obj.               : %d\n",
             int_param[ IPARAM_OBJ_NZ ] );
    fprintf( f, "Number of updated non-zeros               : %d\n",
             int_param[ IPARAM_UPD_NZ ] );
    fprintf( f, "Number of SOS 1 non-zeros                 : %d\n",
             int_param[ IPARAM_SOS1_NZ ] );
    fprintf( f, "Number of SOS 2 non-zeros                 : %d\n",
             int_param[ IPARAM_SOS2_NZ ] );
    fprintf( f, "Optimization direction                    : %s\n",
             ( int_param[ IPARAM_DIRECTION ] == DIRECTION_MIN ) ? "Minimize" : "Maximize" );
    fprintf( f, "Objective constant                        : %12.5e\n",
             dbl_param[ DPARAM_OBJECTIVE_CONST ] );
    fprintf( f, "Basis provided                            : %d\n",
             int_param[ IPARAM_BASIS ] );
    fprintf( f, "Number of ranged rows                     : %d\n",
             int_param[ IPARAM_RANGED_R ] );
    fprintf( f, "Number of equality rows                   : %d\n",
             int_param[ IPARAM_EQUAL_R ] );
    fprintf( f, "Maximum elements in a row                 : %d\n",
             int_param[ IPARAM_MAX_ELEM_R ] );
    fprintf( f, "Maximum elements in a column              : %d\n",
             int_param[ IPARAM_MAX_ELEM_C ] );
    fprintf( f, "Number of quadratic non-zeros             : %d\n",
             int_param[ IPARAM_QUAD_NZ ] );
    fprintf( f, "Number of updated quadratic non-zeros     : %d\n",
             int_param[ IPARAM_QUAD_UPD_NZ ] );
    fprintf( f, "Pure diagonal quadratic matrix            : %d\n",
             int_param[ IPARAM_QUAD_DIAGONAL ] );
    fprintf( f, "Marginal value of zero                    : %12.5e\n",
             dbl_param[ DPARAM_MARGINAL_ZERO ] );
    fprintf( f, "Lowest integer priority                   : %12.5e\n",
             dbl_param[ DPARAM_LOWEST_PRIORITY ] );
    fprintf( f, "Objective zero coefficient                : %12.5e\n",
             dbl_param[ DPARAM_OBJECTIVE_ZERO ] );

    fflush( f );
}


// The function cbc_print_retrieved_columns()
void CbcMathProgramInstance::cbc_print_retrieved_columns( double *lower, double *upper, double *lev,
                                                          double *obj_coef, _LONG_T number,
                                                          int *col_flag )
{
    _LONG_T   j, len;
    char      colname[64];

    if ( cbc_opt_tracing ) {
        for ( j=0; j<cbc_handle . ncols; j++ ) {
        	len = sizeof( colname );
            m_mp->GetRowColumnName( AOSI_NAME_COLUMN, number + j, colname, &len );
        	
            fprintf( cbc_logfile, "colno %d ", number + j );
            fprintf( cbc_logfile, "lb %16.8e ", lower[j] );
            fprintf( cbc_logfile, "ub %16.8e ", upper[j] );
            if ( lev ) {
                fprintf( cbc_logfile, "lev %16.8e ", lev[j] );
            }
            fprintf( cbc_logfile, "obj_coef %16.8e ", obj_coef[j] );
            if ( col_flag ) {
                fprintf( cbc_logfile, "flag %d ", col_flag[j] );
            }
            fprintf( cbc_logfile, "   (%s)", colname );
            fputs( "\n", cbc_logfile );
        }
        
        fflush( cbc_logfile );
    }
}


// The function cbc_print_column_updates()
void CbcMathProgramInstance::cbc_print_column_updates( int *cols, double *lower, double *upper, double *obj_coef,
                                                       int ncols, _LONG_T *col_flag )
{
    int   j;

    if ( cbc_opt_tracing ) {
        for ( j=0; j<ncols; j++ ) {
            fprintf( cbc_logfile, "(upd) colno %d ", cols[j] );
            fprintf( cbc_logfile, "lb %.16E ", lower[j] );
            fprintf( cbc_logfile, "ub %.16E ", upper[j] );
            if ( obj_coef ) {
                fprintf( cbc_logfile, "obj_coef %.16E ", obj_coef[j] );
            }
            if ( col_flag ) {
                if ( col_flag[j] & AOSI_FLAGS_IS_INTEGER ) {
                    fprintf( cbc_logfile, "int " );
                } else {
                    fprintf( cbc_logfile, "cont " );
                }
            }
            fputs( "\n", cbc_logfile );
        }
        
        fflush( cbc_logfile );
    }
}


// The function cbc_print_deleted_columns()
void CbcMathProgramInstance::cbc_print_deleted_columns( int ncols, int *cols )
{
    int   j;

    if ( cbc_opt_tracing ) {
        for ( j=0; j<ncols; j++ ) {
            fprintf( cbc_logfile, "(delete) colno %d\n", cols[j] );
        }
        
        fflush( cbc_logfile );
    }
}


// The function cbc_print_retrieved_rows()
void CbcMathProgramInstance::cbc_print_retrieved_rows( double *row_lo, double *row_up, double *row_lev,
                                                       _LONG_T number, int *row_flag )
{
    _LONG_T   j, len;
    char      rowname[64];

    if ( cbc_opt_tracing ) {
        for ( j=0; j<cbc_handle . nrows; j++ ) {
        	len = sizeof( rowname );
            m_mp->GetRowColumnName( AOSI_NAME_ROW, number + j, rowname, &len );
        	
            fprintf( cbc_logfile, "rowno %d ", number + j );
            fprintf( cbc_logfile, "lb %16.8e ", row_lo[j] );
            fprintf( cbc_logfile, "ub %16.8e ", row_up[j] );
            if ( row_lev ) {
                fprintf( cbc_logfile, "lev %16.8e ", row_lev[j] );
            }
            if ( row_flag ) {
                fprintf( cbc_logfile, "flag %d ", row_flag[j] );
            }
            fprintf( cbc_logfile, "   (%s)", rowname );
            fputs( "\n", cbc_logfile );
        }
        
        fflush( cbc_logfile );
    }
}


// The function cbc_print_row_updates()
void CbcMathProgramInstance::cbc_print_row_updates( int *rows, double *row_lo, double *row_up,
                                                    int nrows )
{
    int   j;

    if ( cbc_opt_tracing ) {
        for ( j=0; j<nrows; j++ ) {
            fprintf( cbc_logfile, "(upd) rowno %d ", rows[j] );
            fprintf( cbc_logfile, "lb %.16E ", row_lo[j] );
            fprintf( cbc_logfile, "ub %.16E ", row_up[j] );
            fputs( "\n", cbc_logfile );
        }
        
        fflush( cbc_logfile );
    }
}


// The function cbc_print_deleted_rows()
void CbcMathProgramInstance::cbc_print_deleted_rows( int nrows, int *rows )
{
    int   j;

    if ( cbc_opt_tracing ) {
        for ( j=0; j<nrows; j++ ) {
            fprintf( cbc_logfile, "(delete) rowno %d\n", rows[j] );
        }
        
        fflush( cbc_logfile );
    }
}


// The function cbc_print_retrieved_matrix_elements()
void CbcMathProgramInstance::cbc_print_retrieved_matrix_elements( int *row, int *col, double *value,
                                                                  int upd, int nelem )
{
    _LONG_T   i, len;
    char      colname[64], rowname[64];

    if ( cbc_opt_tracing ) {
        for ( i=0; i<nelem; i++ ) {
        	len = sizeof( colname );
            m_mp->GetRowColumnName( AOSI_NAME_COLUMN, col[i], colname, &len );
            len = sizeof( rowname );
            m_mp->GetRowColumnName( AOSI_NAME_ROW   , row[i], rowname, &len );        

          	fprintf( cbc_logfile, "%smatrix element (%d,%d) = %16.8e",
            	                  ( upd ) ? "(upd) " : "", row[i], col[i], value[i] );
            fprintf( cbc_logfile, "   (%s, %s)\n", rowname, colname );
        }
        
        fflush( cbc_logfile );
    }
}


// The function cbc_print_retrieved_SOS()
void CbcMathProgramInstance::cbc_print_retrieved_SOS( int nsos, char *sostype, int *soslen,
                                                      double *sospri, int *sosind, double *sosref )
{
    char      buf[256];
    _LONG_T   len;
    int       i, j, pos;
    
    if ( cbc_opt_tracing ) {
    	pos = 0;
    	
        for ( i=0; i<nsos; i++ ) {
             fprintf( cbc_logfile,
                      "\nsos_set %d  sos_type %c  sos_len %d sospri %d\n",
                      i, sostype[i], soslen[i], (int) sospri[i] );
             
             for ( j=0; j<soslen[i]; j++ ) {
                  len = sizeof( buf );
                  m_mp->GetRowColumnName( AOSI_NAME_COLUMN, sosind[j], buf, &len );
                  
                  fprintf( cbc_logfile,
                           "sosind[%d] = %d  sosref[%d] = %g  (col: %s)\n",
                           j, sosind[pos+j], j, sosref[pos+j], buf );
             }
             
             pos += soslen[i];
        }
        
        fflush( cbc_logfile );
    }
}


// The function cbc_check_col_bounds()
int CbcMathProgramInstance::cbc_check_col_bounds( double *lower, double *upper, int ncols )
{
    int   i;

    for ( i=0; i<ncols; i++ ) {
        if ( lower[i] > upper[i] ) {
            cbc_error( "Encountered variable with lower bound > upper bound" );
            return 1;
        }
        if ( lower[i] >= AIMMS_INF ) {
            cbc_error( "Encountered variable with lower bound >= INF" );
            return 1;
        }
        if ( upper[i] <= -AIMMS_INF ) {
            cbc_error( "Encountered variable with upper bound <= -INF" );
            return 1;
        }
    }
    
    return 0;
}


// The function cbc_check_row_bounds()
int CbcMathProgramInstance::cbc_check_row_bounds( double *row_lo, double *row_up, int nrows )
{
    int   i;

    for ( i=0; i<nrows; i++ ) {
        if ( row_lo[i] > row_up[i] ) {
            cbc_error( "Encountered constraint with lower bound > upper bound" );
            return 1;
        }
        if ( ( row_lo[i] == row_up[i]                            ) &&
             ( row_lo[i] <= -AIMMS_INF || row_up[i] >= AIMMS_INF ) ) {
            cbc_error( "Encountered unbounded equality constraint" );
            return 1;
        }
        if ( row_lo[i] >= AIMMS_INF ) {
            cbc_error( "Encountered constraint with lower bound >= INF" );
            return 1;
        }
        if ( row_up[i] <= -AIMMS_INF ) {
            cbc_error( "Encountered constraint with upper bound <= -INF" );
            return 1;
        }
    }
    
    return 0;
}


// The function cbc_check_matrix_elements()
int CbcMathProgramInstance::cbc_check_matrix_elements( double *value, int nelem )
{
    int   i;

    for ( i=0; i<nelem; i++ ) {
        if ( value[i] <= -AIMMS_INF || value[i] >= AIMMS_INF ) {
            cbc_error( "Unbounded matrix element encountered" );
            return 1;
        }
    }
    
    return 0;
}


// The function cbc_is_feasible_solution()
int CbcMathProgramInstance::cbc_is_feasible_solution( void )
{
	int        i, nrows, ncols, nelem;
    int       *col_flag, *row_flag, *col, *row;
    _LONG_T    mem_size, number;
    double    *model_area, *lb, *ub, *x, *row_lo, *row_up, *row_lev, *coef,
               fp, ival, eps = 1e-6;
    
    nrows = cbc_handle . nrows;
    ncols = cbc_handle . ncols;
    nelem = cbc_handle . nonzeros;
    
    mem_size = ncols * sizeof( double )   // lb
             + ncols * sizeof( double )   // ub
             + ncols * sizeof( double )   // x
             + ncols * sizeof( int    )   // col_flag
             + nrows * sizeof( double )   // row_lo
             + nrows * sizeof( double )   // row_up
             + nrows * sizeof( double )   // row_lev
             + nrows * sizeof( int    )   // row_flag
             + nelem * sizeof( int    )   // row
             + nelem * sizeof( int    )   // col
             + nelem * sizeof( double );  // coef
    
    model_area = (double *) m_mp->AllocateMemory( mem_size );
    if ( model_area == NULL ) {
        sprintf( cbc_msg, "Not enough memory for loading model\n"
                 "Required amount of memory: %d bytes", mem_size );
        cbc_error( cbc_msg );
        return 2;
    }
    
    lb       = (double *) model_area;
    ub       = (double *) ( lb       + ncols );
    x        = (double *) ( ub       + ncols );
    row_lo   = (double *) ( x        + ncols );
    row_up   = (double *) ( row_lo   + nrows );
    row_lev  = (double *) ( row_up   + nrows );
    coef     = (double *) ( row_lev  + nrows );
    row      = (int    *) ( coef     + nelem );
    col      = (int    *) ( row      + nelem );
    col_flag = (int    *) ( col      + nelem );
	row_flag = (int    *) ( col_flag + ncols );
	
	memset( x, 0, ncols * sizeof(double) );
	
	if ( cbc_original_cols ) {
		for ( i=0; i<cbc_nr_preproc_cols; i++ ) {
			x[cbc_original_cols[i]] = cbc_incumbent[i];
		}
	} else if ( cbc_nr_preproc_cols == ncols ) {
		for ( i=0; i<ncols; i++ ) {
			x[i] = cbc_incumbent[i];
		}
	} else {
    	return 0;
    }
	
	// Read and pass matrix column wise.
    
    m_mp->SetMatrixOrder( AOSI_REREAD );
    
    // Get column data from AIMMS.

    if ( ncols != m_mp->GetColumnData( /* is_update */ 0, ncols, &number, 
                                       lb, NULL, ub, 
                                       /* obj_coef  */ NULL,
                                       /* prior     */ NULL,
                                       /* no_elm    */ NULL,
                                       /* no_q_elm  */ NULL,
                                       (_LONG_T *) col_flag ) ) {
        cbc_error( "Retrieving column values failed" );
        m_mp->FreeMemory( model_area );
        return 2;
    }
    
    // Check column level values.
    
    for ( i=0; i<ncols; i++ ) {
        if ( ( x[i] < lb[i] - eps ) ||
             ( x[i] > ub[i] + eps ) ) {
            m_mp->FreeMemory( model_area );
            return 0;          // Column level value outside bounds
        }
        
        if ( col_flag[i] & AOSI_FLAGS_IS_INTEGER ) {
        	fp = modf( x[i], &ival ) ;
    		if ( fp >= 0.5 ) {
    		    ival += 1.0 ;
    		}
    		else if ( fp <= -0.5 ) {
    		    ival -= 1.0 ;
    		}
        	
        	if ( ( x[i] < ival - eps ) ||
        	     ( x[i] > ival + eps ) ) {
        	    m_mp->FreeMemory( model_area );
        	    return 0;      // Column level not integer
        	}
        }
    }
    
    // Get row data from AIMMS.
    
    if ( nrows != m_mp->GetRowData( /* is_update */ 0, nrows, &number,
                                    row_lo, NULL, row_up,
                                    /* no_elm    */ NULL,
                                    /* fv        */ NULL,
                                    (_LONG_T *) row_flag ) ) {
        cbc_error( "Retrieving row values failed" );
        m_mp->FreeMemory( model_area );
        return 2;
    }
    
    // Get matrix elements from AIMMS.

    if ( nelem != m_mp->GetMatrixData( /* is_update */ 0, nelem, (_LONG_T *) row,
                                       (_LONG_T *) col, coef, /* nlflag */ NULL ) ) {
        cbc_error( "Retrieving matrix non-zeros failed" );
        m_mp->FreeMemory( model_area );
        return 2;
    }
    
    memset( row_lev, 0, nrows * sizeof(double) );
    
    for ( i=0; i<nelem; i++ ) {
    	assert( 0 <= col[i] && col[i] < ncols );
    	assert( 0 <= row[i] && row[i] < nrows );
    	
    	row_lev[row[i]] += coef[i] * x[col[i]];
    }
    
    // Check row level values.
    
    for ( i=0; i<nrows; i++ ) {
        if ( ( row_lev[i] < row_lo[i] - eps ) ||
             ( row_lev[i] > row_up[i] + eps ) ) {
            m_mp->FreeMemory( model_area );
            return 0;          // Row level value outside bounds
        }
    }
    
    m_mp->FreeMemory( model_area );
    
    return 1;
}
#endif


// The function cbc_init_postsolve()
void CbcMathProgramInstance::cbc_init_postsolve( void )
{
    cbc_inside_postsolve = true;
    
    // Set iteration limit and time limit to inf, and set option for writing mps
    // equal to off.
    
    cbc_opt_iter_limit_pre_post = cbc_opt_iter_limit;
    cbc_opt_time_limit_pre_post = cbc_opt_time_limit;
    cbc_opt_mps_pre_post        = cbc_int_opt_val[ CBC_OPT_MPS ];
    
    cbc_opt_iter_limit             = CBC_INT_MAX;
    cbc_opt_time_limit             = CBC_INT_MAX;
#ifndef CBC_PRINT_POSTSOLVE_MPS
    cbc_int_opt_val[ CBC_OPT_MPS ] = 0;
#endif
}


// The function cbc_end_postsolve()
void CbcMathProgramInstance::cbc_end_postsolve( void )
{
    int      i, flag;
    double   lb, ub;

    cbc_inside_postsolve = false;
    
    cbc_opt_iter_limit             = cbc_opt_iter_limit_pre_post;
    cbc_opt_time_limit             = cbc_opt_time_limit_pre_post;
    cbc_int_opt_val[ CBC_OPT_MPS ] = cbc_opt_mps_pre_post;
    
    // Bring model back into same state as before postsolve.
    
    for ( i=0; i<cbc_handle . ncols; i++ ) {
        // Make integer columns integer.    

        m_manipulation->GetColumnType( i, &flag );
        
        if ( flag & AOSI_FLAGS_IS_INTEGER ) {
            clp_solver->setInteger( i );
        }
        
        // Pass column bounds again to unfix fixed variables.
        
        m_manipulation->GetColumnBounds( i, &lb, &ub );
        
        if ( ub >= AIMMS_INF ) {
        	ub = CBC_INF;
		}
		if ( lb <= -AIMMS_INF ) {
        	lb = -CBC_INF;
		}
        
        clp_solver->setColBounds( i, lb, ub );
    }
}


// The function cbc_write_mps()
int CbcMathProgramInstance::cbc_write_mps( void )
{
    bool     do_mps, do_lp;
    char     mps_filename[512];
    double   obj_sense;

#ifndef CBC_PRINT_POSTSOLVE_MPS
    if ( cbc_inside_postsolve ) {
        return 0;
    }
#endif
    
    do_mps = false;
    do_lp  = false;

    switch ( cbc_int_opt_val[ CBC_OPT_MPS ] ) {
      case 1:
        if ( cbc_seq_number == 0 ) {
            do_mps = true;
        }
        break;
      case 2:
        do_mps = true;
        break;
      case 3:
        if ( cbc_seq_number == 0 ) {
            do_lp = true;
        }
        break;
      case 4:
        do_lp = true;
        break;
      default:
        break;
    }
    
    if ( ( do_mps || do_lp          ) &&
         ( cbc_seq_number < 1000000 ) ) {
        sprintf( mps_filename, "%scbc%05d", cbc_project_dir, cbc_seq_number );
        
        if ( cbc_handle . direction == DIRECTION_MAX ) {
#if CBC_VERSION_NO >= 280
        	obj_sense = 1.0;
#else
			obj_sense = -1.0;
#endif
        } else if ( cbc_handle . direction == DIRECTION_MIN ) {
        	obj_sense = 1.0;
        } else {
        	obj_sense = 0.0;
        }
        
        if ( do_mps ) {
        	clp_solver->writeMps( mps_filename, "mps", obj_sense );
        } else {
        	clp_solver->writeLp( mps_filename, "lp", /* epsilon */ 1e-5, /* numberAcross */ 10,
        	                     /* decimals */ 8, obj_sense, /* useRowNames */ true );
        }
    }

#ifdef CBC_PRINT_POSTSOLVE_MPS
    cbc_seq_number ++;
#else
    if ( ! cbc_inside_postsolve ) {
        cbc_seq_number ++;
    }
#endif

    return 0;
}


// The function cbc_load_branching_priorities()
int CbcMathProgramInstance::cbc_load_branching_priorities( void )
{
	int       *col_flag, *priority, i, ncols, prior, numberIntegers = 0;
	double    *col_prior, minprior, maxprior;
	_LONG_T    number;
  	
  	ncols = cbc_handle . ncols;
  	
  	col_prior = new double [ncols];
  	col_flag  = new int [ncols];
  	
  	m_mp->SetMatrixOrder( AOSI_REREAD );
  	
  	if ( ncols != m_mp->GetColumnData( /*is_update*/ 0,
                                       ncols, &number,
                                       (double  *) NULL, /* col_lo    */
                                       (double  *) NULL, /* col_level */
                                       (double  *) NULL, /* col_up    */
                                       (double  *) NULL, /* obj_coeff */
                                       col_prior,
                                       (_LONG_T *) NULL, /* no_elm    */
                                       (_LONG_T *) NULL, /* no_q_elm  */
                                       (_LONG_T *) col_flag ) ) {
        cbc_error( "Retrieving branching priorities failed" );
        delete [] col_prior;
        delete [] col_flag;
        return 1;
    }
    
    // We map AIMMS priorities into the range {1,..,1000}. In AIMMS the priorities
  	// are nonnegative. All variables with priority 0 will be considered last by
  	// the branch-and-bound process of the solver. For variables with a positive
  	// priority value, those with the smallest priority value will be considered
  	// first. In CBC 1 is the highest priority and 1000 the standard priority.
    
    // Get minimum and maximum priority.
    
    minprior =  AIMMS_INF;
	maxprior = -AIMMS_INF;
    
    for ( i=0; i<ncols; i++ ) {
    	if ( ( col_flag[i] & AOSI_FLAGS_IS_INTEGER ) &&
    	     ( col_prior[i] > 0.0                  ) ) {
      		if ( col_prior[i] < minprior ) {
      			minprior = col_prior[i];
      		}
      		if ( col_prior[i] > maxprior ) {
      			maxprior = col_prior[i];
      		}
    	}
  	}
  	
  	if ( maxprior > 0.0 ) {
  		// Pass priorities to CBC.
    	
    	priority = new int [ncols];
  		
  		for ( i=0; i<ncols; i++ ) {
    		if ( col_flag[i] & AOSI_FLAGS_IS_INTEGER ) {
    			if ( col_prior[i] > 0.0 ) {
    	  			prior = 1 + (int) ( 999*(col_prior[i]-minprior)/(maxprior-minprior) );
    	  		} else {
    	  			prior = 1000;
    	  		}
    	  		
    	  		priority[numberIntegers++] = prior;
    		}
  		}
  	
  		cbc_model->passInPriorities( priority, false );
  	
  		delete [] priority;
  	}
  	
  	delete [] col_prior;
  	delete [] col_flag;
  	
  	return 0;
}


// The function cbc_load_SOS()
int CbcMathProgramInstance::cbc_load_SOS( _LONG_T *int_param )
{
#ifdef CBC_SUPPORTS_SOS
	// SOS has been disabled because it does not seem to work well. There
	// might be something wrong with the AIMMS-CBC interface... For some
	// models it seems to work though.

    int      *soslen, *sosind, sostype, i, nsos, nsosnz, mem_size, pos, prior;
    double   *sos_area, *sosref, *sosprior, minprior, maxprior;
    char     *sosctype;
    
    if ( cbc_handle . model_type != CBC_MIP_MODEL ) return 0;
	
    nsos = int_param[IPARAM_SOS1_R] + int_param[IPARAM_SOS2_R];
    if ( nsos <= 0 ) return 0;
	
	nsosnz = int_param[IPARAM_SOS1_NZ] + int_param[IPARAM_SOS2_NZ];
	if ( nsosnz <= 0 ) return 0;
	
	mem_size = nsos   * sizeof( char   )   // sosctype
             + nsos   * sizeof( int    )   // soslen
             + nsos   * sizeof( double )   // sosprior
             + nsosnz * sizeof( double )   // sosref
             + nsosnz * sizeof( int    );  // sosind

    sos_area = (double *) m_mp->AllocateMemory( mem_size );
    if ( sos_area == NULL ) {
        sprintf( cbc_msg, "Not enough memory for loading SOS info\n"
                 "Required amount of memory: %d bytes", mem_size );
        cbc_error( cbc_msg );
        return 1;
    }

    sosref   = (double *) sos_area;
    sosprior = (double *) ( sosref   + nsosnz );
    soslen   = (int    *) ( sosprior + nsos   );
    sosind   = (int    *) ( soslen   + nsos   );
    sosctype = (char   *) ( sosind   + nsosnz );
	
	if ( nsos != m_mp->GetSOSData( nsosnz, sosctype, NULL, (_LONG_T *) sosind,
                                   (_LONG_T *) soslen, sosref, sosprior ) ) {
        cbc_error( "Retrieving SOS info failed" );
        m_mp->FreeMemory( sos_area );
        return 1;
    }
	
#ifdef DEBUG
    cbc_print_retrieved_SOS( nsos, sosctype, soslen, sosprior, sosind, sosref );
#endif

	// We map AIMMS priorities into the range {1,..,1000}. In AIMMS the priorities
  	// must be nonnegative. All SOS sets with priority 0 will be considered last by
  	// the branch-and-bound process of the solver. For SOS sets with a positive
  	// priority value, those with the smallest priority value will be considered first
  	// by the solver. In CBC 1 is the highest priority and 1000 the standard priority.
	
	// Get minimum and maximum priority.
    
    minprior =  AIMMS_INF;
	maxprior = -AIMMS_INF;
    
    for ( i=0; i<nsos; i++ ) {
    	if ( sosprior[i] > 0.0 ) {
      		if ( sosprior[i] < minprior ) {
      			minprior = sosprior[i];
      		}
      		if ( sosprior[i] > maxprior ) {
      			maxprior = sosprior[i];
      		}
    	}
  	}
    
    // Pass SOS sets to CBC.
    
    OsiObject ** objects = new OsiObject * [nsos];
    
    pos = 0;
    
    for ( i=0; i<nsos; i++ ) {
    	if ( sosctype[i] == '1' ) {
    		sostype = 1;
    	} else {
    		sostype = 2;
    	}
		
		objects[i] = new CbcSOS( cbc_model, soslen[i], sosind+pos, sosref+pos,
						         i, sostype );
		
		if ( sosprior[i] > 0.0 ) {
			prior = 1 + (int) ( 999*(sosprior[i]-minprior)/(maxprior-minprior) );
		} else {
			prior = 1000;
		}
		
		objects[i]->setPriority( prior );
		
		pos += soslen[i];
    }
    
    cbc_model->addObjects( nsos, objects );
	
	for ( i=0; i<nsos; i++ ) {
		delete objects[i];
	}
	delete [] objects;
    
    m_mp->FreeMemory( sos_area );
#endif // CBC_SUPPORTS_SOS
    
    return 0;
}


// The function cbc_load_model()
int CbcMathProgramInstance::cbc_load_model( _LONG_T *int_param )
{
    int        i, res, nrows, ncols, nelem, basis;
    int       *matbeg, *col_flag = NULL, *row_flag = NULL, *col, *row, *col_no_elm;
    _LONG_T    mem_size, number;
    double    *model_area = NULL, *lb, *ub, *col_lev, *obj_coef, *row_lo, *row_up,
              *row_lev, *coef, objval;
    char      *row_sense;
    
    if ( ( cbc_handle . model_type == CBC_MIP_MODEL  ) && 
         ( cbc_int_opt_val[ CBC_OPT_MIP_BASIS ] == 0 ) ) {
        basis = 0;
    } else {
    	basis = int_param[ IPARAM_BASIS ];
    }
    
    cbc_handle . nrows = int_param[ IPARAM_ROWS ];
    cbc_handle . ncols = int_param[ IPARAM_COLS ];
    
    nrows = cbc_handle . nrows;
    ncols = cbc_handle . ncols;
    nelem = cbc_handle . nonzeros;
    
    mem_size = ncols     * sizeof( double )   // lb
             + ncols     * sizeof( double )   // ub
             + ncols     * sizeof( double )   // col_lev
             + ncols     * sizeof( double )   // obj_coef
             + ncols     * sizeof( int    )   // col_no_elm
             + ncols     * sizeof( int    )   // col_flag
             + (ncols+1) * sizeof( int    )   // matbeg
             + nrows     * sizeof( double )   // row_lo
             + nrows     * sizeof( double )   // row_up
             + nrows     * sizeof( double )   // row_lev
             + nrows     * sizeof( int    )   // row_flag
             + nrows     * sizeof( char   )   // row_sense
             + nelem     * sizeof( int    )   // row
             + nelem     * sizeof( int    )   // col
             + nelem     * sizeof( double );  // coef
    
    model_area = (double *) m_mp->AllocateMemory( mem_size );
    if ( model_area == NULL ) {
        sprintf( cbc_msg, "Not enough memory for loading model\n"
                 "Required amount of memory: %d bytes", mem_size );
        cbc_error( cbc_msg );
        return 1;
    }
    
    lb         = (double *) model_area;
    ub         = (double *) ( lb         + ncols     );
    col_lev    = (double *) ( ub         + ncols     );
    obj_coef   = (double *) ( col_lev    + ncols     );
    row_lo     = (double *) ( obj_coef   + ncols     );
    row_up     = (double *) ( row_lo     + nrows     );
    row_lev    = (double *) ( row_up     + nrows     );
    coef       = (double *) ( row_lev    + nrows     );
    row        = (int    *) ( coef       + nelem     );
    col        = (int    *) ( row        + nelem     );
    col_no_elm = (int    *) ( col        + nelem     );
    col_flag   = (int    *) ( col_no_elm + ncols     );
    matbeg     = (int    *) ( col_flag   + ncols     );
	row_flag   = (int    *) ( matbeg     + (ncols+1) );
	row_sense  = (char   *) ( row_flag   + nrows     );
	
	// Read and pass matrix column wise.
    
    m_mp->SetMatrixOrder( AOSI_COL_WISE );
    
    // Get column data from AIMMS.

    res = m_mp->GetColumnData( /* is_update */ 0, ncols, &number, 
                               lb, col_lev, ub, obj_coef,
                               /* prior     */ NULL,
                               /* no_elm    */ (_LONG_T *) col_no_elm,
                               /* no_q_elm  */ NULL,
                               (_LONG_T *) col_flag );
    
#ifdef DEBUG
    cbc_print_retrieved_columns( lb, ub, /* col_lev */ NULL, obj_coef, number, col_flag );
    
    if ( cbc_check_col_bounds( lb, ub, ncols ) ) {
        m_mp->FreeMemory( model_area );
        return 1;
    }
#endif   
    
    if ( res != ncols ) {
        cbc_error( "Retrieving column values failed" );
        m_mp->FreeMemory( model_area );
        return 1;
    }
    
    // Get row data from AIMMS.
    
    res = m_mp->GetRowData( /* is_update */ 0, nrows, &number,
                            row_lo, row_lev, row_up,
                            /* no_elm    */ NULL,
                            /* fv        */ NULL,
                            (_LONG_T *) row_flag );

#ifdef DEBUG
    cbc_print_retrieved_rows( row_lo, row_up, row_lev, number, row_flag );
    
    if ( cbc_check_row_bounds( row_lo, row_up, nrows ) ) {
        m_mp->FreeMemory( model_area );
        return 1;
    }
#endif
    
    if ( res != nrows ) {
        cbc_error( "Retrieving row values failed" );
        m_mp->FreeMemory( model_area );
        return 1;
    }
    
    // Get matrix elements from AIMMS.

    res = m_mp->GetMatrixData( /* is_update */ 0, nelem, (_LONG_T *) row,
                               (_LONG_T *) col, coef, /* nlflag */ NULL );
    
#ifdef DEBUG
    cbc_print_retrieved_matrix_elements( row, col, coef, /* upd */ 0, res );

    if ( cbc_check_matrix_elements( coef, nelem ) ) {
        m_mp->FreeMemory( model_area );
        return 1;
    }
#endif
    
    if ( res != nelem ) {
        cbc_error( "Retrieving matrix non-zeros failed" );
        m_mp->FreeMemory( model_area );
        return 1;
    }

	for ( i=0; i<ncols; i++ ) {
		if ( col_flag[i] & AOSI_FLAGS_IS_INTEGER ) {
        	if ( ub[i] >= (double) COIN_INT_MAX ) {
        		ub[i] = CBC_INF;
        	}
        	if ( lb[i] <= -(double) COIN_INT_MAX ) {
        		lb[i] = -CBC_INF;
        	}
        } else {
        	if ( ub[i] >= AIMMS_INF ) {
        		ub[i] = CBC_INF;
        	}
        	if ( lb[i] <= -AIMMS_INF ) {
        		lb[i] = -CBC_INF;
        	}
        }
        
#if CBC_VERSION_NO >= 280
       	obj_coef[i] *= cbc_handle . obj_multiplier;
#endif
    }

	// Determine row sense. RHS is placed in row_up and range in row_lo.

	for ( i=0; i<nrows; i++ ) {
        if ( row_up[i] >= AIMMS_INF ) {   // row_up is infinite
        	row_up[i] = CBC_INF;
        	
            if ( row_lo[i] <= -AIMMS_INF ) {   // row_lo is -infinite -> Free row
                row_sense[i] = 'L';
                row_lo[i] = -CBC_INF;
            } else {
                row_sense[i] = 'G';
                row_up[i] = row_lo[i];
                row_lo[i] = 0.0;
            }
        } else {
            if ( row_lo[i] <= -AIMMS_INF ) {   // row_lo is -infinite
                row_sense[i] = 'L';
                row_lo[i] = 0.0;
            } else if ( row_lo[i] >= row_up[i] ) {
                row_sense[i] = 'E';
                row_lo[i] = 0.0;
            } else {   // Ranged row
                row_sense[i] = 'R';
                row_lo[i] = row_up[i] - row_lo[i];
            }
        }
    }
    
    // Determine beginning of each column in array of matrix coeffients.
    
    matbeg[0] = 0;
    for ( i=1; i<=ncols; i++ ) {
        matbeg[i] = matbeg[i-1] + col_no_elm[i-1];
    }
    
    // Load problem into CBC.
	
	clp_solver->loadProblem( ncols, nrows, matbeg, row, coef, lb, ub, obj_coef,
                             row_sense, /* rhs */ row_up, /* range */ row_lo );
    
    // Specify integer variables (if any).
	
    if ( cbc_handle . model_type == CBC_MIP_MODEL ) {
    	for ( i=0; i<ncols; i++ ) {
    		if ( col_flag[i] & AOSI_FLAGS_IS_INTEGER ) {
    			clp_solver->setInteger( i );
    		}
    	}
    }
    
    // Load cbc_model with options first.
	
	CbcMain0( *cbc_model );
    
    // Load basis and column levels (if wanted).
    
    if ( basis ) {
        int   *col_bas, *row_bas;

        col_bas = (int *) m_mp->AllocateMemory( ncols * sizeof( int ) );
        if ( col_bas == NULL ) {
            sprintf( cbc_msg, "Not enough memory for setting basis\n"
                     "Required amount of memory: %d bytes",
                     static_cast<int>( (ncols + nrows) * sizeof( int ) ) );
            cbc_error( cbc_msg );
            m_mp->FreeMemory( model_area );
            return 1;
        }
        
        row_bas = (int *) m_mp->AllocateMemory( nrows * sizeof( int ) );
        if ( row_bas == NULL ) {
            sprintf( cbc_msg, "Not enough memory for setting basis\n"
                     "Required amount of memory: %d bytes", 
                     static_cast<int>( (ncols + nrows) * sizeof( int ) ) );
            cbc_error( cbc_msg );
            m_mp->FreeMemory( col_bas );
            m_mp->FreeMemory( model_area );
            return 1;
        }
        
        assert( col_flag );
        for ( i=0; i<ncols; i++ ) {
            if ( col_flag[i] & AOSI_FLAGS_IS_BASIC ) {
                col_bas[i] = CBC_STATUS_BASIC;         // Basic
            } else if ( col_lev[i] <= lb[i] ) {
                col_bas[i] = CBC_STATUS_NONBASIC_LB;   // Non-basic at lower bound
            } else if ( col_lev[i] >= ub[i] ) {
                col_bas[i] = CBC_STATUS_NONBASIC_UB;   // Non-basic at upper bound
            } else {
            	col_bas[i] = CBC_STATUS_SUPERBASIC;    // Superbasic
            }
        }
        
        assert( row_flag );
        for ( i=0; i<nrows; i++ ) {
            if ( row_flag[i] & AOSI_FLAGS_IS_BASIC ) {
                row_bas[i] = CBC_STATUS_BASIC;         // Basic
            } else if ( row_lev[i] <= row_lo[i] ) {
                row_bas[i] = CBC_STATUS_NONBASIC_LB;   // Non-basic at lower bound
            } else if ( row_lev[i] >= row_up[i] ) {
                row_bas[i] = CBC_STATUS_NONBASIC_UB;   // Non-basic at upper bound
            } else {
                row_bas[i] = CBC_STATUS_SUPERBASIC;    // Superbasic (or free)
            }
        }

        res = clp_solver->setBasisStatus( col_bas, row_bas );
        if ( res ) {
            strcpy( cbc_msg, "Failed to set the basis." );   // Just go on without basis
            m_gen->PassMessage( AOSI_PRIO_REMARK, cbc_msg );
        }

        m_mp->FreeMemory( col_bas ); 
        m_mp->FreeMemory( row_bas );
        
        // Load column levels.
        
        clp_solver->setColSolution( col_lev );
    }
    
    // Load MIP start (if wanted).
    
    if ( ( cbc_handle . model_type == CBC_MIP_MODEL ) &&
         ( cbc_int_opt_val[CBC_OPT_MIP_START] == 1  ) ) {
        // Calculate objective value corresponding to MIP start.
        
        objval = 0.0;
        for ( i=0; i<ncols; i++ ) {
            objval += obj_coef[i] * col_lev[i] * cbc_handle . obj_multiplier;
        }
        
        // Solution is not saved if it is not feasible (because check is true).
        
        cbc_model->setBestSolution( col_lev, ncols, objval, /* check */ true );
    }

    m_mp->FreeMemory( model_area );
    
    return 0;
}


// The function cbc_update_model()
int CbcMathProgramInstance::cbc_update_model( void )
{
    int        i, nrows_upd, ncols_upd, nelem_upd;
    int       *cols, *rows, *col, *row;
    _LONG_T   *type_flag, mem_size;
    double    *model_area = NULL, *lb, *ub, *obj_coef, *row_lo, *row_up, *coef;
    bool       is_int;
    
    nrows_upd = cbc_handle . nrows_upd;
    ncols_upd = cbc_handle . ncols_upd;
    nelem_upd = cbc_handle . nonzeros_upd;

    mem_size = ncols_upd * sizeof( int     )   // cols
             + ncols_upd * sizeof( double  )   // lb
             + ncols_upd * sizeof( double  )   // ub
             + ncols_upd * sizeof( double  )   // obj_coef
             + ncols_upd * sizeof( _LONG_T )   // type_flag
             + nrows_upd * sizeof( int     )   // rows
             + nrows_upd * sizeof( double  )   // row_lo
             + nrows_upd * sizeof( double  )   // row_up
             + nelem_upd * sizeof( int     )   // row
             + nelem_upd * sizeof( int     )   // col
             + nelem_upd * sizeof( double  );  // coef

    model_area = (double *) m_mp->AllocateMemory( mem_size );
    if ( model_area == NULL ) {
        sprintf( cbc_msg, "Not enough memory for loading model\n"
                 "Required amount of memory: %d bytes", mem_size );
        cbc_error( cbc_msg );
        return 1;
    }
    
    lb        = (double  *) model_area;
    ub        = (double  *) ( lb        + ncols_upd );
    obj_coef  = (double  *) ( ub        + ncols_upd );
    row_lo    = (double  *) ( obj_coef  + ncols_upd );
    row_up    = (double  *) ( row_lo    + nrows_upd );
    coef      = (double  *) ( row_up    + nrows_upd );
    type_flag = (_LONG_T *) ( coef      + nelem_upd );
    cols      = (int     *) ( type_flag + ncols_upd );
    rows      = (int     *) ( cols      + ncols_upd );
    row       = (int     *) ( rows      + nrows_upd );
    col       = (int     *) ( row       + nelem_upd );
    
    m_mp->SetMatrixOrder( AOSI_COL_WISE );
    
    // Get column data from AIMMS.

    if ( ncols_upd != m_mp->GetColumnData( /* is_update */ 1, ncols_upd, 
                                           (_LONG_T *) cols, lb, 
                                           /* lev       */ NULL,
                                           ub, obj_coef,
                                           /* prior     */ NULL,
                                           /* no_elm    */ NULL,
                                           /* no_q_elm  */ NULL,
                                           type_flag ) ) {
        cbc_error( "Retrieving column updates failed" );
        m_mp->FreeMemory( model_area );
        return 1;
    }

#ifdef DEBUG
    cbc_print_column_updates( cols, lb, ub, obj_coef, ncols_upd, /* col_flag */ NULL );
    
    if ( cbc_check_col_bounds( lb, ub, ncols_upd ) ) {
        m_mp->FreeMemory( model_area );
        return 1;
    }
#endif
    
    for ( i=0; i<ncols_upd; i++ ) {
    	// Change column type (if necessary).
    	
        is_int = clp_solver->isInteger( cols[i] );
        
        if ( ( is_int                                  ) &&
             ( type_flag[i] & AOSI_FLAGS_IS_CONTINUOUS ) ) {
            clp_solver->setContinuous( cols[i] );
        } else if ( ( ! is_int                             ) &&
                    ( type_flag[i] & AOSI_FLAGS_IS_INTEGER ) ) {
            clp_solver->setInteger( cols[i] );
        }
        
        // Pass column bound updates to CBC.
        
        if ( type_flag[i] & AOSI_FLAGS_IS_INTEGER ) {
        	if ( ub[i] >= (double) COIN_INT_MAX ) {
        		ub[i] = CBC_INF;
        	}
        	if ( lb[i] <= -(double) COIN_INT_MAX ) {
        		lb[i] = -CBC_INF;
        	}
        } else {
        	if ( ub[i] >= AIMMS_INF ) {
        		ub[i] = CBC_INF;
        	}
        	if ( lb[i] <= -AIMMS_INF ) {
        		lb[i] = -CBC_INF;
        	}
        }
        
        clp_solver->setColBounds( cols[i], lb[i], ub[i] );
        
        // Pass objective coefficient updates to CBC.
        
        clp_solver->setObjCoeff( cols[i], obj_coef[i] * cbc_handle . obj_multiplier );
    }
    
    // Get row data from AIMMS.
    
    if ( nrows_upd != m_mp->GetRowData( /* is_update */ 1, nrows_upd,
                                        (_LONG_T *) rows,
                                        row_lo,
                                        /* row_lev   */ NULL,
                                        row_up,
                                        /* no_elm    */ NULL,
                                        /* fv        */ NULL,
                                        /* type_flag */ NULL ) ) {
        cbc_error( "Retrieving row updates failed" );
        m_mp->FreeMemory( model_area );
        return 1;
    }
    
#ifdef DEBUG
    cbc_print_row_updates( rows, row_lo, row_up, nrows_upd );
    
    if ( cbc_check_row_bounds( row_lo, row_up, nrows_upd ) ) {
        m_mp->FreeMemory( model_area );
        return 1;
    }    
#endif
    
    // Pass row bound updates to CBC.
    
    for ( i=0; i<nrows_upd; i++ ) {
    	if ( row_up[i] >= AIMMS_INF ) {
        	row_up[i] = CBC_INF;
		}
		if ( row_lo[i] <= -AIMMS_INF ) {
        	row_lo[i] = -CBC_INF;
		}
        
        clp_solver->setRowBounds( rows[i], row_lo[i], row_up[i] );
    }
    
    // Get matrix elements from AIMMS.

    if ( nelem_upd != m_mp->GetMatrixData( /* is_update */ 1, nelem_upd, 
                                           (_LONG_T *) row, (_LONG_T *) col, coef,
                                           /* nlflag */ NULL ) ) {
        cbc_error( "Retrieving matrix non-zeros failed" );
        m_mp->FreeMemory( model_area );
        return 1;
    }
    
#ifdef DEBUG
    cbc_print_retrieved_matrix_elements( row, col, coef, /* upd */ 1, nelem_upd );

    if ( cbc_check_matrix_elements( coef, nelem_upd ) ) {
        m_mp->FreeMemory( model_area );
        return 1;
    }
#endif

	// Input matrix updates into CBC.
        
    for ( i=0; i<nelem_upd; i++ ) {
    	clp_solver->modifyCoefficient( rows[i], cols[i], coef[i] );
    }
	                     
    m_mp->FreeMemory( model_area );
    
    return 0;
}


// The function cbc_mm_load_deleted_rows()
int CbcMathProgramInstance::cbc_mm_load_deleted_rows( _LONG_T *int_param )
{
    int       *rows, nrows_del;
    _LONG_T    mem_size;
    
    nrows_del = int_param[ IPARAM_MM_DEL_R ];
    if ( nrows_del <= 0 ) return 0;
    
    mem_size = nrows_del * sizeof( int );   // rows
    
    rows = (int *) m_mp->AllocateMemory( mem_size );
    if ( rows == NULL ) {
        sprintf( cbc_msg, "Not enough memory for deleting rows\n"
                 "Required amount of memory: %d bytes", mem_size );
        cbc_error( cbc_msg );
        return 1;
    }
    
    nrows_del = m_manipulation->GetNextDeletedRows( nrows_del, (_LONG_T *) rows );
    
#ifdef DEBUG
    cbc_print_deleted_rows( nrows_del, rows );
#endif
    
    // Delete a number of constraints from the model. Existing constraints are
    // renumbered. For instance, if constraint 5 is removed then constraint 6
    // will be constraint 5 afterwards.
    
    clp_solver->deleteRows( nrows_del, rows );
    
    m_mp->FreeMemory( rows );
	
    return 0;
}


// The function cbc_mm_load_new_rows()
int CbcMathProgramInstance::cbc_mm_load_new_rows( void )
{
    int       *rows, i, nrows_add, count, col;
    _LONG_T   *flag, mem_size;
    double    *model_area = NULL, *row_lo, *row_up, coef;
    
    nrows_add = count = CBC_MM_MAX_ROWS;
    
    mem_size = nrows_add * sizeof( double  )   // row_lo
             + nrows_add * sizeof( double  )   // row_up
             + nrows_add * sizeof( int     )   // rows
             + nrows_add * sizeof( _LONG_T );  // flag
             
    model_area = (double *) m_mp->AllocateMemory( mem_size );
    if ( model_area == NULL ) {
        sprintf( cbc_msg, "Not enough memory for adding rows\n"
                 "Required amount of memory: %d bytes", mem_size );
        cbc_error( cbc_msg );
        return 1;
    }
    
    row_lo = (double  *) model_area;
    row_up = (double  *) ( row_lo + nrows_add );
    rows   = (int     *) ( row_up + nrows_add );
    flag   = (_LONG_T *) ( rows   + nrows_add );

    while ( nrows_add == count ) {
        nrows_add = m_manipulation->GetNextAddedRows( count, (_LONG_T *) rows, row_lo, row_up,
                                                      flag );       // row_lo before row_up!
        if ( ! nrows_add )
            break;

#ifdef DEBUG
        cbc_print_row_updates( rows, row_lo, row_up, nrows_add );
    
        if ( cbc_check_row_bounds( row_lo, row_up, nrows_add ) ) {
            m_mp->FreeMemory( model_area );
            return 1;
        }    
#endif

        // Append a number of constraints to the model. The appended constraints will
        // be declared free. Note: CBC will automatically expand the problem dimension
        // to accommodate the additional constraints.
        
        // Add rows to CBC.
    
        for ( i=0; i<nrows_add; i++ ) {
            if ( row_up[i] >= AIMMS_INF ) {
            	row_up[i] = CBC_INF;
            }
            if ( row_lo[i] <= -AIMMS_INF ) {
            	row_lo[i] = -CBC_INF;
            }
            
			clp_solver->addRow( 0, &col, &coef, row_lo[i], row_up[i] ) ;
        }
    }
    
    m_mp->FreeMemory( model_area );
	
    return 0;
}


// The function cbc_mm_load_changed_row_bounds()
int CbcMathProgramInstance::cbc_mm_load_changed_row_bounds( void )
{
    int       *rows, i, nrows_upd, count, rowno;
    _LONG_T   *flag, mem_size;
    double    *model_area = NULL, *row_lo, *row_up, lb, ub;
    
    nrows_upd = count = CBC_MM_MAX_ROWS;
    
    mem_size = nrows_upd * sizeof( double  )   // row_lo
             + nrows_upd * sizeof( double  )   // row_up
             + nrows_upd * sizeof( int     )   // rows
             + nrows_upd * sizeof( _LONG_T );  // flag
             
    model_area = (double *) m_mp->AllocateMemory( mem_size );
    if ( model_area == NULL ) {
        sprintf( cbc_msg, "Not enough memory for updating rows\n"
                 "Required amount of memory: %d bytes", mem_size );
        cbc_error( cbc_msg );
        return 1;
    }
    
    row_lo = (double  *) model_area;
    row_up = (double  *) ( row_lo + nrows_upd );
    rows   = (int     *) ( row_up + nrows_upd );
    flag   = (_LONG_T *) ( rows   + nrows_upd );

    while ( nrows_upd == count ) {
        nrows_upd = m_manipulation->GetNextChangedRows( count, (_LONG_T *) rows, row_lo, row_up,
                                                        flag );       // row_lo before row_up!
        if ( ! nrows_upd )
            break;
            
        for ( i=0; i<nrows_upd; i++ ) {
        	rowno = rows[i];
            
            if ( flag[i] & AOSI_MM_ROW_DEACTIVATED ) {
            	// If row is deactivated we transform row into a free row.
            	
                clp_solver->setRowUpper( rows[i],  CBC_INF );
                clp_solver->setRowLower( rows[i], -CBC_INF );
            } else {
                if ( flag[i] & AOSI_MM_UB_CHANGED ) {
                	if ( row_up[i] >= AIMMS_INF ) {
                		ub = CBC_INF;
                	} else {
                		ub = row_up[i];
                	}
                	
                	clp_solver->setRowUpper( rows[i], ub );
                }

                if ( flag[i] & AOSI_MM_LB_CHANGED ) {
                	if ( row_lo[i] <= -AIMMS_INF ) {
                		lb = -CBC_INF;
                	} else {
                		lb = row_lo[i];
                	}
                	
                	clp_solver->setRowLower( rows[i], lb );
                }
            }
        }
		
#ifdef DEBUG
        cbc_print_row_updates( rows, row_lo, row_up, nrows_upd );
    
        if ( cbc_check_row_bounds( row_lo, row_up, nrows_upd ) ) {
            m_mp->FreeMemory( model_area );
            return 1;
        }    
#endif
    }
    
    m_mp->FreeMemory( model_area );
	
    return 0;
}


// The function cbc_mm_load_deleted_columns()
int CbcMathProgramInstance::cbc_mm_load_deleted_columns( _LONG_T *int_param )
{
    int       *cols, ncols_del;
    _LONG_T    mem_size;
    
    ncols_del = int_param[ IPARAM_MM_DEL_C ];
    if ( ncols_del <= 0 ) return 0;
    
    mem_size = ncols_del * sizeof( int );   // cols
    
    cols = (int *) m_mp->AllocateMemory( mem_size );
    if ( cols == NULL ) {
        sprintf( cbc_msg, "Not enough memory for deleting columns\n"
                 "Required amount of memory: %d bytes", mem_size );
        cbc_error( cbc_msg );
        return 1;
    }
    
    ncols_del = m_manipulation->GetNextDeletedColumns( ncols_del, (_LONG_T *) cols );
    
#ifdef DEBUG
    cbc_print_deleted_columns( ncols_del, cols );
#endif
    
    // Delete a number of variables from the model. Existing variables are
    // renumbered. For instance, if variable 5 is removed then variable 6
    // will be variable 5 afterwards.
	
    clp_solver->deleteCols( ncols_del, cols );
    
    m_mp->FreeMemory( cols );
	
    return 0;
}


// The function cbc_mm_load_new_columns()
int CbcMathProgramInstance::cbc_mm_load_new_columns( int model_type_changed )
{
    int       *cols, i, ncols_add, count, row, ncols_cbc;
    _LONG_T   *flag, mem_size;
    double    *model_area = NULL, *lb, *ub, coef;
    
    ncols_add = count = CBC_MM_MAX_COLS;
    
    mem_size = ncols_add * sizeof( double  )   // lb
             + ncols_add * sizeof( double  )   // ub
             + ncols_add * sizeof( int     )   // cols
             + ncols_add * sizeof( _LONG_T );  // flag
             
    model_area = (double *) m_mp->AllocateMemory( mem_size );
    if ( model_area == NULL ) {
        sprintf( cbc_msg, "Not enough memory for adding columns\n"
                 "Required amount of memory: %d bytes", mem_size );
        cbc_error( cbc_msg );
        return 1;
    }
    
    lb   = (double  *) model_area;
    ub   = (double  *) ( lb   + ncols_add );
    cols = (int     *) ( ub   + ncols_add );
    flag = (_LONG_T *) ( cols + ncols_add );

    while ( ncols_add == count ) {
        ncols_add = m_manipulation->GetNextAddedColumns( count, (_LONG_T *) cols, ub, lb, flag );
                                                                                    // ub before lb!
        if ( ! ncols_add )
            break;

#ifdef DEBUG
        cbc_print_column_updates( cols, lb, ub, /* obj_coef */ NULL, ncols_add, flag );
    
        if ( cbc_check_col_bounds( lb, ub, ncols_add ) ) {
            m_mp->FreeMemory( model_area );
            return 1;
        }
#endif

		ncols_cbc = clp_solver->getNumCols();
        
        // Add columns to CBC.
        
        for ( i=0; i<ncols_add; i++ ) {
        	if ( flag[i] & AOSI_FLAGS_IS_INTEGER ) {
        		if ( ub[i] >= (double) COIN_INT_MAX ) {
        			ub[i] = CBC_INF;
        		}
        		if ( lb[i] <= -(double) COIN_INT_MAX ) {
        			lb[i] = -CBC_INF;
        		}
        	} else {
        		if ( ub[i] >= AIMMS_INF ) {
        			ub[i] = CBC_INF;
        		}
        		if ( lb[i] <= -AIMMS_INF ) {
        			lb[i] = -CBC_INF;
        		}
        	}
        	
        	clp_solver->addCol( 0, &row, &coef, lb[i], ub[i], 0.0 );
        	
        	// Only specify column as integer if model is MIP.
        
        	if ( cbc_handle . model_type == CBC_MIP_MODEL ) {
        		if ( flag[i] & AOSI_FLAGS_IS_INTEGER ) {
        		    clp_solver->setInteger( ncols_cbc+i );
        		}
        	}
        }
    }
    
    m_mp->FreeMemory( model_area );
	
    return 0;
}


// The function cbc_mm_load_changed_column()
int CbcMathProgramInstance::cbc_mm_load_changed_column( int model_type_changed )
{
    int       *cols, i, ncols_upd, count, colno;
    _LONG_T   *flag, mem_size;
    double    *model_area = NULL, *lb, *ub, lower, upper;
    
    ncols_upd = count = CBC_MM_MAX_COLS;
    
    mem_size = ncols_upd * sizeof( double  )   // lb
             + ncols_upd * sizeof( double  )   // ub
             + ncols_upd * sizeof( int     )   // cols
             + ncols_upd * sizeof( _LONG_T );  // flag
    
    model_area = (double *) m_mp->AllocateMemory( mem_size );
    if ( model_area == NULL ) {
        sprintf( cbc_msg, "Not enough memory for updating columns\n"
                 "Required amount of memory: %d bytes", mem_size );
        cbc_error( cbc_msg );
        return 1;
    }
    
    lb   = (double  *) model_area;
    ub   = (double  *) ( lb   + ncols_upd );
    cols = (int     *) ( ub   + ncols_upd );
    flag = (_LONG_T *) ( cols + ncols_upd );

    while ( ncols_upd == count ) {
        ncols_upd = m_manipulation->GetNextChangedColumns( count, (_LONG_T *) cols, ub, lb, flag );
                                                                                      // ub before lb!
        if ( ! ncols_upd )
            break;
            
        for ( i=0; i<ncols_upd; i++ ) {
        	colno = cols[i];
        	
            // Change column type if necessary.

            if ( flag[i] & AOSI_FLAGS_MM_MODIFIED_TYPE ) {
                if ( flag[i] & AOSI_FLAGS_IS_INTEGER ) {
                	// Only specify column as integer if model is MIP.
                	
                	if ( cbc_handle . model_type == CBC_MIP_MODEL ) {
                    	clp_solver->setInteger( colno );
             		} else {
             			clp_solver->setContinuous( colno );
             		}
                } else if ( flag[i] & AOSI_FLAGS_IS_CONTINUOUS ) {
                    clp_solver->setContinuous( colno );
                } else {
                    cbc_error( "Encountered invalid column type" );
                    m_mp->FreeMemory( model_area );
                    return 1;
                }
            }
            
        	if ( flag[i] & AOSI_FLAGS_MM_MODIFIED_UB ) {
        		if ( ub[i] >= AIMMS_INF ) {
        			upper = CBC_INF;
        		} else {
            		upper = ub[i];
            	}
            	
            	clp_solver->setColUpper( colno, upper );
            }
            
            if ( flag[i] & AOSI_FLAGS_MM_MODIFIED_LB ) {
            	if ( lb[i] <= -AIMMS_INF ) {
        			lower = -CBC_INF;
        		} else {
            		lower = lb[i];
            	}
            	
            	clp_solver->setColLower( colno, lower );
            }
        }

#ifdef DEBUG
        cbc_print_column_updates( cols, lb, ub, /* obj_coef */ NULL, ncols_upd, flag );
#endif
    }
    
    m_mp->FreeMemory( model_area );
	
    return 0;
}


// The function cbc_mm_load_changed_obj_coef()
int CbcMathProgramInstance::cbc_mm_load_changed_obj_coef( void )
{
    int       *cols, i, ncols_upd, count;
    _LONG_T    mem_size;
    double    *model_area = NULL, *obj_coef;
    
    ncols_upd = count = CBC_MM_MAX_COLS;
    
    mem_size = ncols_upd * sizeof( double )   // obj_coef
             + ncols_upd * sizeof( int    );  // cols
             
    model_area = (double *) m_mp->AllocateMemory( mem_size );
    if ( model_area == NULL ) {
        sprintf( cbc_msg, "Not enough memory for updating columns\n"
                 "Required amount of memory: %d bytes", mem_size );
        cbc_error( cbc_msg );
        return 1;
    }
    
    obj_coef = (double *) model_area;
    cols     = (int    *) ( obj_coef + ncols_upd );

    while ( ncols_upd == count ) {
        // Use -count to get objective coefficients.
        
        ncols_upd = m_manipulation->GetNextChangedMatrixElements( -count, NULL, (_LONG_T *) cols, obj_coef );
        if ( ! ncols_upd )
            break;
            
#ifdef DEBUG
        for ( i=0; i<ncols_upd; i++ ) {
            if ( obj_coef[i] <= -AIMMS_INF || obj_coef[i] >= AIMMS_INF ) {
                cbc_error( "Encountered unbounded objective coefficient" );
                m_mp->FreeMemory( model_area );
                return 1;
            }

            if ( cbc_opt_tracing ) {
                 fprintf( cbc_logfile, "Changing obj coef of col %d to %.16E\n",
                          cols[i], obj_coef[i] );
            }
        }
#endif
    
        // Input column updates into CBC.
        
        for ( i=0; i<ncols_upd; i++ ) {
        	clp_solver->setObjCoeff( cols[i], obj_coef[i] * cbc_handle . obj_multiplier );
        }
    }
    
    m_mp->FreeMemory( model_area );
	
    return 0;
}


// The function cbc_mm_load_changed_matrix_elements()
int CbcMathProgramInstance::cbc_mm_load_changed_matrix_elements( void )
{
    int       *cols, *rows, i, nelem_upd, count;
    _LONG_T    mem_size;
    double    *model_area, *coef;
    
    nelem_upd = count = CBC_MM_MAX_ELEM;
    
    mem_size = nelem_upd * sizeof( double )   // coef
             + nelem_upd * sizeof( int    )   // cols
             + nelem_upd * sizeof( int    );  // rows
             
    model_area = (double *) m_mp->AllocateMemory( mem_size );
    if ( model_area == NULL ) {
        sprintf( cbc_msg, "Not enough memory for updating matrix elements\n"
                 "Required amount of memory: %d bytes", mem_size );
        cbc_error( cbc_msg );
        return 1;
    }
    
    coef = (double *) model_area;
    cols = (int    *) ( coef + nelem_upd );
    rows = (int    *) ( cols + nelem_upd );

    while ( nelem_upd == count ) {
        // Use + count to get matrix coefficients.
        
        nelem_upd = m_manipulation->GetNextChangedMatrixElements( count, (_LONG_T *) rows, (_LONG_T *) cols,
                                                                  coef );
        if ( ! nelem_upd )
            break;
            
#ifdef DEBUG
        for ( i=0; i<nelem_upd; i++ ) {
            if ( coef[i] <= -AIMMS_INF || coef[i] >= AIMMS_INF ) {
                cbc_error( "Encountered unbounded matrix coefficient" );
                m_mp->FreeMemory( model_area );
                return 1;
            }

            if ( cbc_opt_tracing ) {
                 fprintf( cbc_logfile, "Changing matrix element (%d,%d) to %.16E\n",
                          rows[i], cols[i], coef[i] );
            }
        }
#endif
    
        // Input matrix updates into CBC.
        
        for ( i=0; i<nelem_upd; i++ ) {
        	clp_solver->modifyCoefficient( rows[i], cols[i], coef[i] );
        }
    }
    
    m_mp->FreeMemory( model_area );
	
    return 0;
}


// The function cbc_mm_change_model_direction()
int CbcMathProgramInstance::cbc_mm_change_model_direction( _LONG_T *int_param )
{
    int            *cols, i, ncols;
    _LONG_T         mem_size, number;
    double         *model_area = NULL, *obj_coef;
    const double   *obj_coeffs;
    
    ncols = cbc_handle . ncols;
    
    if ( cbc_handle . direction == DIRECTION_MIN ||
         cbc_handle . direction == DIRECTION_MAX ) {
        if ( int_param[ IPARAM_MM_FLAGS ] & AOSI_MM_DIRECTION_WAS_NONE ) {
            // Change direction from none into maximizing or minimizing. Because
            // the direction was none all objective coefficients are 0.0. Now we
            // have to restore the objective coefficients.
             
            mem_size = ncols * sizeof( double )   // obj_coef
                     + ncols * sizeof( int    );  // cols
                      
            model_area = (double *) m_mp->AllocateMemory( mem_size );
            if ( model_area == NULL ) {
                sprintf( cbc_msg, "Not enough memory for updating columns\n"
                         "Required amount of memory: %d bytes", mem_size );
                cbc_error( cbc_msg );
                return 1;
            }
             
            obj_coef = (double *) model_area;
            cols     = (int    *) ( obj_coef + ncols );

            // If we have solved a different model previous we have to reset
            // start_sending_col.
            
            m_mp->SetMatrixOrder( AOSI_REREAD );
         
            if ( ncols != m_mp->GetColumnData( /*is_update*/ 0, ncols, &number,
                                               NULL, NULL, NULL, obj_coef, NULL,
                                               NULL, NULL, NULL ) ) {
                cbc_error( "Retrieving columns failed" );
                m_mp->FreeMemory( model_area );
                return 1;
            }
            
            // Pass nonzero objective coefficients to CBC.
         
            for ( i=0; i<ncols; i++ ) {
                if ( obj_coef[i] < -EPS_REP || obj_coef[i] > EPS_REP ) {
                    clp_solver->setObjCoeff( i, obj_coef[i] * cbc_handle . obj_multiplier );
                }
            }
            
            m_mp->FreeMemory( model_area );
#if CBC_VERSION_NO >= 280
        } else if ( ( cbc_handle . model_type == CBC_MIP_MODEL                  ) &&
                    ( ! ( int_param[ IPARAM_MM_FLAGS ] & AOSI_MM_RELOAD_MODEL ) ) &&
                    ( cbc_handle . prev_direction != cbc_handle . direction     ) ) {
        	obj_coeffs = clp_solver->getObjCoefficients();
        	
        	if ( obj_coeffs ) {
        		for ( i=0; i<ncols; i++ ) {
        	        if ( obj_coeffs[i] < -EPS_REP || obj_coeffs[i] > EPS_REP ) {
        	            clp_solver->setObjCoeff( i, - obj_coeffs[i] );
        	    	}
        	    }
        	}
#endif
        }
    } else if ( ! ( int_param[ IPARAM_MM_FLAGS ] & AOSI_MM_DIRECTION_WAS_NONE ) ) {
        // Change direction to none. We change all objective coefficients unequal
        // to 0.0 into 0.0. Hence, we end up with an objective function equal to 0.0.
        
        obj_coeffs = clp_solver->getObjCoefficients();
        
        if ( obj_coeffs ) {
        	for ( i=0; i<ncols; i++ ) {
                if ( obj_coeffs[i] < -EPS_REP || obj_coeffs[i] > EPS_REP ) {
                    clp_solver->setObjCoeff( i, 0.0 );
                }
            }
        }
    }
	
    return 0;
}


// The function cbc_mm_change_model_type()
int CbcMathProgramInstance::cbc_mm_change_model_type( int mm_type )
{
    int   i, ncols, flag;
    
    ncols = cbc_handle . ncols;
    
	if ( mm_type == AOSI_MM_TYPE_MIP ) {         // MIP
        cbc_handle . model_type = CBC_MIP_MODEL;
        
        // Make all integer variables integer in CBC. Because model was RMIP
        // before all variables in CBC are continuous.
        
        for ( i=0; i<ncols; i++ ) {
            m_manipulation->GetColumnType( i, &flag );
            
            if ( flag & AOSI_FLAGS_IS_INTEGER ) {
                clp_solver->setInteger( i );
            }
        }
    } else {                                     // RMIP
        assert( mm_type == AOSI_MM_TYPE_RMIP );
        
        cbc_handle . model_type = CBC_RMIP_MODEL;
        
        // RMIP: Make all integer variables continuous in CBC. 
        
        for ( i=0; i<ncols; i++ ) {
            m_manipulation->GetColumnType( i, &flag );
            
            if ( flag & AOSI_FLAGS_IS_INTEGER ) {
                clp_solver->setContinuous( i );
            }
        }
    }
	
    return 0;
}


// The function cbc_update_mm_model()
int CbcMathProgramInstance::cbc_update_mm_model( _LONG_T *int_param )
{
    int   model_type_changed;
	
    // If the problem(-name) loaded in the solver has changed (i.e., bit AOSI_MM_RELOAD_MODEL
    // is set) then we have loaded the model from scratch again by using cbc_load_model.
    // In cbc_load_model we load the new rows and columns, but not the bounds of the
    // new columns (which we do in cbc_mm_load_changed_column()). Note: In 
    // cbc_load_model we load also the coefficients of the matrix for new rows and
    // columns; that we do here once again (overhead).
    
    // Change model type (MIP/RMIP).
    
    if ( int_param[ IPARAM_MM_FLAGS ] & AOSI_MM_TYPE_CHANGED ) {
		if ( int_param[ IPARAM_MM_TYPE ] != AOSI_MM_TYPE_MIP  &&
             int_param[ IPARAM_MM_TYPE ] != AOSI_MM_TYPE_RMIP ) {
            cbc_error( "Encountered invalid model type change" );
            return 1;
        }
        model_type_changed = 1;
    } else {
        model_type_changed = 0;
    }
    
    if ( ! ( int_param[ IPARAM_MM_FLAGS ] & AOSI_MM_RELOAD_MODEL ) ) {
    	assert( ! ( int_param[ IPARAM_MM_FLAGS ] & AOSI_MM_OUTER_APPROX ) );
            
		cbc_handle . nrows += ( int_param[ IPARAM_MM_ADD_R ] - 
		                        int_param[ IPARAM_MM_DEL_R ] );
		cbc_handle . ncols += ( int_param[ IPARAM_MM_ADD_C ] -
		                        int_param[ IPARAM_MM_DEL_C ] );
    	
    	if ( int_param[ IPARAM_MM_DEL_R ] ) {
            if ( cbc_mm_load_deleted_rows( int_param ) )
                return 1;
        }
    	
        // Adding rows should be done before changing them.
        
        if ( ! ( int_param[ IPARAM_MM_FLAGS ] & AOSI_MM_TYPE_CHANGED ) &&
               ( int_param[ IPARAM_MM_ADD_R ]                        ) ) {
            if ( cbc_mm_load_new_rows() )
                return 1;
        }
            
        if ( int_param[ IPARAM_MM_CHG_R ] ) {
            if ( cbc_mm_load_changed_row_bounds() )
                return 1;
        }
        
        if ( int_param[ IPARAM_MM_ADD_C ] ) {
            if ( cbc_mm_load_new_columns( model_type_changed ) )
                return 1;
        }
        
        // Adding columns must be done before changing them.

        if ( int_param[ IPARAM_MM_DEL_C ] ) {
            if ( cbc_mm_load_deleted_columns( int_param ) )
                return 1;
        }
    }

    if ( int_param[ IPARAM_MM_CHG_C ] ) {
        if ( cbc_mm_load_changed_column( model_type_changed ) )
            return 1;
    }
 
    if ( int_param[ IPARAM_MM_CHG_OBJ_C ] ) {
        if ( cbc_mm_load_changed_obj_coef() )
            return 1;
    }

    if ( int_param[ IPARAM_MM_CHG_E ] ) {
        if ( cbc_mm_load_changed_matrix_elements() )
            return 1;
    }
    
    if ( cbc_mm_change_model_direction( int_param ) )
        return 1;
        
    if ( model_type_changed ) {
        if ( cbc_mm_change_model_type( int_param[ IPARAM_MM_TYPE ] ) )
            return 1;
    }
	
    return 0;
}


// The function cbc_create_optimization_task()
int CbcMathProgramInstance::cbc_create_optimization_task( void )
{
    if ( clp_solver ) {
    	delete clp_solver;
    }
    
    clp_solver = new OsiClpSolverInterface;
    if ( ! clp_solver ) {
        cbc_error( "Failed to create optimization task" );
        return 1;
    }
    
    return 0;
}


// The function cbc_init_solve()
void CbcMathProgramInstance::cbc_init_solve( _LONG_T *int_param, double *dbl_param, int update,
                                             int type )
{
    cbc_handle . model_status    = MODELSTAT_NOT_SOLVED;
    cbc_handle . solver_status   = SOLVERSTAT_NOT_CALLED;

    cbc_handle . nonzeros        = int_param[ IPARAM_NONZEROS   ];
    cbc_handle . nrows_upd       = int_param[ IPARAM_UPD_R      ];
    cbc_handle . ncols_upd       = int_param[ IPARAM_UPD_C      ];
    cbc_handle . nonzeros_upd    = int_param[ IPARAM_UPD_NZ     ];
#if CBC_VERSION_NO >= 280
    cbc_handle . prev_direction  = cbc_handle . direction;
#endif
    cbc_handle . direction       = int_param[ IPARAM_DIRECTION  ];
    cbc_handle . obj_col_no      = int_param[ IPARAM_OBJ_COL_NO ];
    
    if ( ( cbc_handle . model_type == CBC_MIP_MODEL  ) && 
         ( cbc_int_opt_val[ CBC_OPT_MIP_BASIS ] == 0 ) ) {
        cbc_handle . basis       = 0;
    } else {
    	cbc_handle . basis       = int_param[ IPARAM_BASIS     ];
    }
    
    cbc_handle . update          = update;
    cbc_handle . solved          = false;
    cbc_handle . marginal_zero   = dbl_param[ DPARAM_MARGINAL_ZERO   ];
    cbc_handle . obj_constant    = dbl_param[ DPARAM_OBJECTIVE_CONST ];
    cbc_handle . objval          = ( cbc_handle . direction == DIRECTION_MIN ) ?
                                   AIMMS_INF : -AIMMS_INF;
    cbc_handle . obj_best        = cbc_handle . objval;
    cbc_handle . mip_best_poss   = cbc_handle . objval;
    cbc_handle . iter            = 0;
    if ( ! cbc_inside_postsolve ) {
    	cbc_handle . nodes          = 0;
    	cbc_handle . obj_multiplier = 1.0;
    }
    cbc_handle . nodes_left      = 0;
    cbc_handle . phase           = -1;
    cbc_handle . is_feasible     = false;
    cbc_handle . mip_is_feasible = false;
    cbc_handle . finished        = false;
    
    cbc_incumbent                = NULL;
    cbc_nr_preproc_cols          = 0;
	cbc_original_cols            = NULL;
    cbc_next_to_print_iter       = cbc_progress;
    cbc_next_to_print_time       = cbc_start_time;
	
#ifdef WIN32
    MEMORYSTATUS   stat;
    
    GlobalMemoryStatus( &stat );

    cbc_handle . start_mem_avail = stat.dwAvailVirtual;
#else
    cbc_handle . start_mem_avail = 0;
#endif
}


// The function cbc_get_model_type()
int CbcMathProgramInstance::cbc_get_model_type( _LONG_T *int_param )
{
    int   sum;

    sum = int_param[ IPARAM_SOS1_R ] +
          int_param[ IPARAM_SOS2_R ];
    
#ifndef CBC_SUPPORTS_SOS
    if ( sum ) {
        cbc_error( "SOS not supported" );
        return 1;
    }
#endif
	
    if ( sum + int_param[ IPARAM_INT_C ] ) {
        // If model is a LS (i.e. direction is feasible) then we search for the
        // first feasible LP solution.
           
        if ( int_param[ IPARAM_RMIP ] ) {
            cbc_handle . model_type = CBC_RMIP_MODEL;
        } else {
            cbc_handle . model_type = CBC_MIP_MODEL;
        }
    } else {
        if ( ! sum ) {
            cbc_handle . model_type = CBC_LP_MODEL;
        } else {
            cbc_error( "SOS without integer variables not supported" );
            return 1;
        }
    }
    
    return 0;
}


// The function cbc_calculate_objective()
double CbcMathProgramInstance::cbc_calculate_objective( void )
{
	int             i, ncols;
	double          obj;
	const double   *values, *obj_coef;
	
	if ( cbc_handle . obj_col_no < 0 ) return 0.0;
	
	values   = cbc_model->getColSolution();
	obj_coef = cbc_model->getObjCoefficients();
	
	ncols = cbc_handle . ncols;
	
	obj = 0.0;
	for ( i=0; i<ncols; i++ ) {
		obj += obj_coef[i] * values[i] * cbc_handle . obj_multiplier;
	}
	
    return obj;
}


// The function cbc_get_solution_info_lp()
int CbcMathProgramInstance::cbc_get_solution_info_lp( void )
{
    bool   user_interupt, iter_interupt, time_interupt;
    char   cbc_msg[256];                 // Buffer for building (error) messages

	OsiSolverInterface * solver = cbc_model->solver();
	
    // Get solution information (objective, solver status, etc).
    
    cbc_handle . objval = cbc_handle . obj_multiplier * solver->getObjValue();
    
    if ( ! cbc_inside_postsolve ) {
    	cbc_handle . nodes  = 0;
    }
    
    // Getting number of iterations using getIterationCount() does not work if
    // a postsolve is done (by CBC/CLP). The callback using the ClpEventHandler
    // seems to update the number of iterations correctly so we trust and use
    // that.
    
#ifdef NEVER    
	cbc_handle . iter   = solver->getIterationCount();
#endif
    
    user_interupt  = cbc_user_interrupt;
    iter_interupt  = ( cbc_handle . iter >= cbc_opt_iter_limit );
    time_interupt  = cbc_model->isSecondsLimitReached();
    
    if ( user_interupt || iter_interupt || time_interupt ) {
    	if ( cbc_model->bestSolution() ) {
    		cbc_handle . model_status = MODELSTAT_INTER_NON_OPT;
        	cbc_handle . is_feasible = true;
    	} else {
    		cbc_handle . model_status = MODELSTAT_INTER_INFEAS;
    		cbc_handle . objval = AOSI_NA_REP;
    	}
    	
    	if ( user_interupt ) {
    		cbc_handle . solver_status = SOLVERSTAT_USER_INT;
    	} else if ( iter_interupt ) {
    		cbc_handle . solver_status = SOLVERSTAT_ITERATION_INT;
    	} else {
    		cbc_handle . solver_status = SOLVERSTAT_RESOURCE_INT;
    	}
    } else if ( solver->isProvenOptimal() ) {
    	cbc_handle . model_status  = MODELSTAT_OPTIMAL;
    	cbc_handle . solver_status = SOLVERSTAT_NORMAL_END;
    	cbc_handle . is_feasible = true;
    } else if ( solver->isProvenPrimalInfeasible() ) {
    	cbc_handle . model_status  = MODELSTAT_INFEASIBLE;
    	cbc_handle . solver_status = SOLVERSTAT_NORMAL_END;
    	cbc_handle . objval = AOSI_NA_REP;
    } else if ( solver->isProvenDualInfeasible() ) {
    	cbc_handle . model_status  = MODELSTAT_UNBOUNDED;
    	cbc_handle . solver_status = SOLVERSTAT_NORMAL_END;
    	cbc_handle . objval = ( cbc_handle . direction == DIRECTION_MAX ) ? CBC_INF : -CBC_INF;
    } else if ( solver->isAbandoned() ) {
    	cbc_handle . model_status  = MODELSTAT_NO_SOLUTION;
        cbc_handle . solver_status = SOLVERSTAT_UNKNOWN;
        cbc_handle . objval = AOSI_NA_REP;
        sprintf( cbc_msg, "The solve is abandoned due to numerical difficulties." );
        m_gen->PassMessage( AOSI_PRIO_REMARK, cbc_msg );
    } else if ( solver->isPrimalObjectiveLimitReached() ) {
    	cbc_handle . model_status  = MODELSTAT_NO_SOLUTION;
        cbc_handle . solver_status = SOLVERSTAT_UNKNOWN;
        cbc_handle . objval = AOSI_NA_REP;
        sprintf( cbc_msg, "Primal objective limit reached." );
        m_gen->PassMessage( AOSI_PRIO_REMARK, cbc_msg );
    } else if ( solver->isDualObjectiveLimitReached() ) {
    	cbc_handle . model_status  = MODELSTAT_NO_SOLUTION;
        cbc_handle . solver_status = SOLVERSTAT_UNKNOWN;
        cbc_handle . objval = AOSI_NA_REP;
        sprintf( cbc_msg, "Dual objective limit reached." );
        m_gen->PassMessage( AOSI_PRIO_REMARK, cbc_msg );
   	} else {
   		cbc_handle . model_status  = MODELSTAT_NO_SOLUTION;
        cbc_handle . solver_status = SOLVERSTAT_UNKNOWN;
        cbc_handle . objval = AOSI_NA_REP;
   		BUG;
   	}
    
#ifdef DEBUG
    if ( cbc_opt_tracing ) {
    	fprintf( cbc_logfile, "Solve number    : %d\n",  cbc_solve_no                );
        fprintf( cbc_logfile, "Model status    : %d\n",  cbc_handle . model_status  );
        fprintf( cbc_logfile, "Solver status   : %d\n",  cbc_handle . solver_status );
        fprintf( cbc_logfile, "Objective value : %.5f\n", cbc_handle . objval        );
        fprintf( cbc_logfile, "# Iterations    : %d\n",   cbc_handle . iter          );
        fprintf( cbc_logfile, "# Nodes         : %d\n",   cbc_handle . nodes         );
        fflush( cbc_logfile );
    }
#endif
	
    return 0;
}


// The function cbc_get_solution_info_mip()
int CbcMathProgramInstance::cbc_get_solution_info_mip( void )
{
    int    status, secondary_status;
    bool   user_interupt, iter_interupt, time_interupt, solut_interupt, nodes_interupt;
    char   cbc_msg[256];                 // Buffer for building (error) messages
    
    // Pass phase if not yet MIP phase.
    
    if ( cbc_current->phase != AOSI_PHASE_MIP ) {
		cbc_current->phase = AOSI_PHASE_MIP;
		
		m_callback->PassPhaseInfo( cbc_current->phase );
	}

    // Get solution information (objective, solver status, etc).
    
    // The value returned by cbc_model->getObjValue() is not always correct and
    // therefore we calculate the objective ourselves (unless we are solving a MPS
    // file in which case we cannot calculate the objective).
    
    if ( cbc_handle . mps_type > 0 ) {   // MPS solve
    	cbc_handle . objval    = cbc_model->getObjValue();
    } else {
    	cbc_handle . objval    = cbc_calculate_objective();
    }
    cbc_handle . iter          = cbc_model->getIterationCount();
    cbc_handle . nodes         = cbc_model->getNodeCount();
    cbc_handle . mip_best_poss = cbc_handle . obj_multiplier * cbc_model->getBestPossibleObjValue();
    
    user_interupt  = cbc_user_interrupt;
    iter_interupt  = ( cbc_handle . iter >= cbc_opt_iter_limit );
    time_interupt  = cbc_model->isSecondsLimitReached();
    solut_interupt = cbc_model->isSolutionLimitReached();
    nodes_interupt = cbc_model->isNodeLimitReached();
    
    status           = cbc_model->status();
    secondary_status = cbc_model->secondaryStatus();
    
    if ( user_interupt || iter_interupt || time_interupt || solut_interupt || nodes_interupt ) {
    	if ( cbc_model->bestSolution() ) {
        	cbc_handle . model_status = MODELSTAT_INT_SOLUTION;
        	cbc_handle . mip_is_feasible = true;
        	cbc_handle . is_feasible     = true;
        	cbc_handle . obj_best = cbc_handle . objval;
    	} else {
    		cbc_handle . model_status = MODELSTAT_INTER_INFEAS;
    		cbc_handle . objval = AOSI_NA_REP;
    	}
    	
    	if ( user_interupt ) {
    		cbc_handle . solver_status = SOLVERSTAT_USER_INT;
    	} else if ( iter_interupt ) {
    		cbc_handle . solver_status = SOLVERSTAT_ITERATION_INT;
    	} else {
    		cbc_handle . solver_status = SOLVERSTAT_RESOURCE_INT;
    	}
    } else if ( secondary_status == 1 ) {
    	// Secondary status 1: linear relaxation not feasible (or worse than cutoff).
    	
    	cbc_handle . model_status  = MODELSTAT_INFEASIBLE;
      	cbc_handle . solver_status = SOLVERSTAT_NORMAL_END;
    } else if ( secondary_status == 2 ) {
    	// Secondary status 2: stopped on gap. Treat as optimal.
    	
    	cbc_handle . model_status  = MODELSTAT_OPTIMAL;
      	cbc_handle . solver_status = SOLVERSTAT_NORMAL_END;
      	cbc_handle . mip_is_feasible = true;
      	cbc_handle . is_feasible     = true;
      	cbc_handle . obj_best = cbc_handle . objval;
    } else if ( secondary_status == 7 ) {
    	// Secondary status 7: linear relaxation unbounded.
    	
    	cbc_handle . model_status  = MODELSTAT_OPTIMAL;
      	cbc_handle . solver_status = SOLVERSTAT_NORMAL_END;
      	cbc_handle . objval = ( cbc_handle . direction == DIRECTION_MAX ) ? CBC_INF : -CBC_INF;
    } else if ( status == 2 ) {
    	cbc_handle . model_status  = MODELSTAT_NO_SOLUTION;
        cbc_handle . solver_status = SOLVERSTAT_UNKNOWN;
        cbc_handle . objval = AOSI_NA_REP;
        sprintf( cbc_msg, "The solve is abandoned due to numerical difficulties." );
        m_gen->PassMessage( AOSI_PRIO_REMARK, cbc_msg );
    } else if ( status == 0 ) {
      	if ( cbc_model->isProvenOptimal() ) {
      		cbc_handle . model_status  = MODELSTAT_OPTIMAL;
      		cbc_handle . solver_status = SOLVERSTAT_NORMAL_END;
      		cbc_handle . mip_is_feasible = true;
        	cbc_handle . is_feasible     = true;
        	cbc_handle . obj_best = cbc_handle . objval;
      	} else if ( cbc_model->isProvenInfeasible() ) {
      		cbc_handle . model_status  = MODELSTAT_INFEASIBLE;
      		cbc_handle . solver_status = SOLVERSTAT_NORMAL_END;
      		cbc_handle . objval = AOSI_NA_REP;
      	} else if ( cbc_model->isContinuousUnbounded() ) {
      		cbc_handle . model_status  = MODELSTAT_UNBOUNDED;
      		cbc_handle . solver_status = SOLVERSTAT_NORMAL_END;
      		cbc_handle . objval = ( cbc_handle . direction == DIRECTION_MAX ) ? CBC_INF : -CBC_INF;
      	}
   	} else {
   		cbc_handle . model_status  = MODELSTAT_NO_SOLUTION;
        cbc_handle . solver_status = SOLVERSTAT_UNKNOWN;
        cbc_handle . objval = AOSI_NA_REP;
   		BUG;
   	}
    
#ifdef DEBUG
    if ( cbc_opt_tracing ) {
    	fprintf( cbc_logfile, "Solve number    : %d\n",   cbc_solve_no               );
        fprintf( cbc_logfile, "Model status    : %d\n",  cbc_handle . model_status  );
        fprintf( cbc_logfile, "Solver status   : %d\n",  cbc_handle . solver_status );
        fprintf( cbc_logfile, "Objective value : %.5f   (CBC: %.5f)\n",
                 cbc_handle . objval, cbc_model->getObjValue() );
        fprintf( cbc_logfile, "# Iterations    : %d\n",   cbc_handle . iter          );
        fprintf( cbc_logfile, "# Nodes         : %d\n",   cbc_handle . nodes         );
        fflush( cbc_logfile );
    }
#endif
	
    return 0;
}


// The function cbc_actually_call_solver()
int CbcMathProgramInstance::cbc_actually_call_solver( std::list<std::string>& opt_list )
{
	char                  dir[MAX_DIR_LEN], file_name[1024], buf[256];
    _LONG_T               len;
    int                   i, opt_list_length, res = 0;
#ifdef WIN32
	CoinMessageHandler   *handler = NULL;
    FILE                 *sta_file = NULL;
#else // LINUX
    int                   fd = -1;
    bool                  redirect_stdout = false;
#endif
    
    assert( cbc_args );
    
    // Open CBC output file (if wanted).
	
	if ( cbc_opt_status_file == 1 ) {
		len = MAX_DIR_LEN;
        m_gen->GetLogDirName( dir, &len );
        
#ifdef WIN32
        if ( len >= 0 ) {
            sprintf( file_name, "%s/%s", dir, CBC_STATUS_FILE_NAME_ASC );
    
            sta_file = fopen( file_name, "a" );
            
    		if ( sta_file ) {
    			handler = new CoinMessageHandler( sta_file );
    			
    			cbc_model->passInMessageHandler( handler );
    			
#ifdef DEBUG
				OsiSolverInterface * solver = cbc_model->solver();
    			OsiClpSolverInterface * clpSolver = dynamic_cast<OsiClpSolverInterface *> (solver);
  				clp_solver->messageHandler()->setPrefix( true );
  				clp_solver->messageHandler()->setLogLevel( 1 );
#endif
			}
    	}
#else // LINUX
		if ( len >= 0 ) {
			sprintf( file_name, "%s/%s", dir, CBC_STATUS_FILE_NAME_ASC );
			
			fd = dup( fileno(stdout) );
			freopen( file_name, "a", stdout );
			
			redirect_stdout = true;
		}
	} else if ( cbc_opt_status_file == 0 ) {   // No output
		fd = dup( fileno(stdout) );
		freopen( "/dev/null", "w", stdout );
		
		redirect_stdout = true;
#endif
    }
    
    // Install callback for displaying progress information and for passing incumbents.
    
    cbc_install_callback();
	
	// Collect options and add them to list of arguments for CbcMain1.
	
	opt_list_length = (int) opt_list.size();
	
	cbc_args[0] = "CBC";
	i = 1;
	for ( std::list<std::string>::iterator it(opt_list.begin()); it!=opt_list.end(); ++it, ++i ) {
		cbc_args[i] = it->c_str();
	}
	cbc_args[i++]="-solve";
	cbc_args[i++]="-quit";
	
	// Finally call the main solver. Try to catch exceptions like CBC getting out of memory during
	// (MIP) solve.
	
	try {
		CbcMain1( opt_list_length+3, cbc_args, *cbc_model );
	} catch ( std::exception &e ) {
		res = 1;   // Indicating error
		
		cbc_handle . model_status  = MODELSTAT_NO_SOLUTION;
		cbc_handle . solver_status = SOLVERSTAT_TERMINATED;
		cbc_handle . objval = AOSI_NA_REP;
		
		if ( strcmp( e.what(), "bad allocation" ) ) {
			sprintf( buf, "CBC error: %s\0", e.what() );
		} else {
			sprintf( buf, "CBC error: Out of memory during solve\0" );
		}
		m_gen->PassMessage( AOSI_PRIO_ALWAYS, buf );
	}
	
#ifdef WIN32
	if ( sta_file ) {
		fclose( sta_file );
	}
	
	if ( handler ) {
		delete handler;
	}
#else // LINUX
	if ( redirect_stdout ) {
		fflush( stdout );
		dup2( fd, fileno(stdout) );
		close( fd );
	}
#endif
	
	return res;
}


// The function cbc_solve_model()
int CbcMathProgramInstance::cbc_solve_model( std::list<std::string>& opt_list )
{
    int   res;
    
    // Set optimization direction.
    
    if ( cbc_handle . direction == DIRECTION_MAX ) {
#if CBC_VERSION_NO >= 280
    	if ( cbc_handle . obj_multiplier < 0.0 ) {
        	cbc_model->setObjSense( CBC_OBJECTIVE_SENSE_MINIMIZE );
        } else {
        	cbc_model->setObjSense( CBC_OBJECTIVE_SENSE_MAXIMIZE );
        }
#else
        cbc_model->setObjSense( CBC_OBJECTIVE_SENSE_MAXIMIZE );
#endif
    } else if ( cbc_handle . direction == DIRECTION_MIN ) {
    	cbc_model->setObjSense( CBC_OBJECTIVE_SENSE_MINIMIZE );
    } else {
        cbc_model->setObjSense( CBC_OBJECTIVE_SENSE_NONE );
    }
    
    res = cbc_write_mps();
    if ( res ) return 1;
    
    // Optimize.
	
	res = cbc_actually_call_solver( opt_list );
    if ( res ) return 1;
    
    // Get solution information (objective, solver status, etc).
    
    if ( cbc_handle . model_type == CBC_MIP_MODEL ) {
    	res = cbc_get_solution_info_mip();
    } else {
    	res = cbc_get_solution_info_lp();
    }
    if ( res ) return 1;
    
    cbc_handle . finished = true;
    
    return 0;
}


// The function cbc_call_solver()
int CbcMathProgramInstance::cbc_call_solver( _LONG_T *int_param, double *dbl_param, int update, int type )
{
    int    res, nint_cols;
    bool   make_task = false, load_model = false;
    
    // Make optimization task if this is not an update solve.
    
    if ( ! update ) {
        make_task = true;
    } else if ( int_param[ IPARAM_USE_MM ] != 0 ) {
        if ( int_param[ IPARAM_MM_FLAGS ] & AOSI_MM_RELOAD_MODEL ) {
            make_task = true;
        }
    }
    
    if ( make_task ) {
        res = cbc_create_optimization_task();
        if ( res ) return 1;
    }
	
    // Initialize solve routine.
    
    cbc_init_solve( int_param, dbl_param, update, type );
    
    // Determine model type.
    
    res = cbc_get_model_type( int_param );
    if ( res ) return 1;
    
    // CBC 2.8.0 contains a bug when solving maximization problems. Therefore
    // we transform the problem into a mimization problem but we want to report
    // objective values (etc) as if it was a maximization problem.
	
#if CBC_VERSION_NO >= 280
    if ( ( cbc_handle . direction == DIRECTION_MAX  ) &&
         ( cbc_handle . model_type == CBC_MIP_MODEL ) ) {
    	cbc_handle . obj_multiplier = -1.0;
    }
#endif
    
    // Get model data from AIMMS and pass it to the solver.
    
    if ( cbc_model ) {
    	delete cbc_model;
    }
    
    cbc_model = new CbcModel( *clp_solver );
    if ( ! cbc_model ) {
        cbc_error( "Failed to create optimization task" );
        return 1;
    }
	
    if ( ! update ) {
        if ( cbc_load_model( int_param ) ) {
            return 1;
        }
        load_model = true;
    } else {
        if ( int_param[ IPARAM_USE_MM ] == 0 ) {
            if ( cbc_update_model() ) {
                return 1;
            }
        } else {    // Use matrix manipulation/GMP modifications
            if ( int_param[ IPARAM_MM_FLAGS ] & AOSI_MM_RELOAD_MODEL ) {
                // If the problem(-name) in the solver changes, we have to load the
                // model into the solver first. After that we can update the model.
                
                if ( cbc_load_model( int_param ) ) {
                    return 1;
                }
                load_model = true;
            }
			
            if ( int_param[ IPARAM_QUAD_ROWS ] == 0 ) {
                if ( cbc_update_mm_model( int_param ) ) {
                    return 1;
                }
            }
        }
    }
    
    // Now model has too much information e.g. best solution. Simplest is to
    // start again. Seems a bit strange that this has to be done after we have
    // for example just loaded a basis but it seems to be necessary.
    	
    delete cbc_model;
    	
    // Pass data and solver to CbcModel.
    	
    cbc_model = new CbcModel( *clp_solver );
    
    if ( ( cbc_handle . model_type == CBC_MIP_MODEL ) &&
         ( load_model                               ) ) {
    	// Load integer variable branching priorities.
    	
    	res = cbc_load_branching_priorities();
    	if ( res ) return 1;
    	
#ifdef CBC_SUPPORTS_SOS
    	// Load SOS sets.
    	
    	res = cbc_load_SOS( int_param );
    	if ( res ) return 1;
#endif
    }
    
    // A MIP problem without integers becomes a RMIP.
    
    if ( cbc_handle . model_type == CBC_MIP_MODEL ) {
        nint_cols = cbc_model->numberIntegers();
        
        if ( nint_cols == 0 ) {
            cbc_handle . model_type = CBC_RMIP_MODEL;
            cbc_handle . mip_solve_as_lp = true;
        } else {
            cbc_handle . mip_solve_as_lp = false;
        }
    } else {
        cbc_handle . mip_solve_as_lp = false;
    }
    
    // Set and activate CBC options.
    
    std::list<std::string> opt_list;

    res = cbc_set_options( opt_list );
    if ( res ) return 1;
	
    // Pass progress information to AIMMS just before solve starts.

    cbc_progress_info( /* end */ false );

    // Actually solve problem.

    res = cbc_solve_model( opt_list );
    if ( res ) return 1;
    
    // Pass progress information to AIMMS just after solve.

    cbc_progress_info( /* end */ true );
	
    return 0;
}


// The function RegisterAsSolver()
int CbcSolverInfo::RegisterAsSolver( char *name, _LONG_T *model_flags )
{
    /* This function returns the name of the solver and the model types that
       the solver can handle. The latter is done by setting the properiate
       model flags.

       This function is (only) called when the solver is added to the 'Solver
       Configuration' of AIMMS. */
	
    sprintf( name, "CBC %s", CBC_VERSION );
	
    *model_flags = AOSI_MODEL_LP  |
                   AOSI_MODEL_MIP ;
	
    return AOSI_SUCCESS;
}


// The function GetInitialInfo()
_LONG_T CbcSolverInfo::GetInitialInfo( _LONG_T *info_int, double *info_dbl, char *cbc_msg )
{
    /* This function provides information to AIMMS about the solver capabilities
       and about the solver options.

       This function is called when this solver is made default for some model type
       (possible when AIMMS is started up). It is also called when the solver is
       added to the 'Solver Configuration' of AIMMS. */
	
    // On input, info_int[ AOSI_IINFO_VERSION ] and info_int[ AOSI_IINFO_VERSION + 1 ]
    // contain the current AOSI_VERSION number. If the check belows fails then this
    // interface version does not match the current AIMMS version.
	
    if ( info_int[ AOSI_IINFO_VERSION ] != info_int[ AOSI_IINFO_VERSION + 1 ] ||
         info_int[ AOSI_IINFO_VERSION ] != AOSI_VERSION                       ) {
        sprintf( cbc_msg, "Solver version (%d) does not match AIMMS version (%d)",
                 AOSI_VERSION, info_int[ AOSI_IINFO_VERSION ] );
        m_gen->PassMessage( AOSI_PRIO_ALWAYS, cbc_msg );
        info_int[ AOSI_IINFO_VERSION ] = AOSI_VERSION;
        return AOSI_SUCCESS;
    }
    
    info_int[ AOSI_IINFO_VERSION       ] = AOSI_VERSION;
    
    // Specify the solver capability flags:
    // - CBC changes numbering of cols/rows immediately after deleting a col/row,
    //   and therefore the capability flag AOSI_CAPAB_RENUMBER_DEL is set.
    // - CBC has no support for calculating the IIS and therefore the capability
    //   flag AOSI_CAPAB_IIS_FIND is not set.
    // - CBC cannot calculate the smallest and largest values for the RHS of a
	//   constraint so that the optimal basis remains constant, and therefore the
    //   capability flag AOSI_CAPAB_RHS_SENSE is not set.
    // - CBC cannot calculate the smallest and largest values for the objective
	//   coefficient of a variable so that the optimal basis remains constant,
    //   and therefore the capability flag AOSI_CAPAB_OBJ_SENSE is not set.
    // - CBC cannot give the sum of the infeasibilities and therefore the capability
    //   flag AOSI_CAPAB_PROGRESS_INF is not set.
    
    info_int[ AOSI_IINFO_CAPAB_FLAGS   ] = AOSI_CAPAB_UPDATES       |
                                           AOSI_CAPAB_OBJ_ROW_REDUC |
                                           AOSI_CAPAB_MATRIX_MANIP  |
                                           AOSI_CAPAB_RANGED_ROWS   |
                                           AOSI_CAPAB_SOLUTION      |
                                           AOSI_CAPAB_POSTSOLVE     |
                                           AOSI_CAPAB_RENUMBER_DEL  ;
    
    info_int[ AOSI_IINFO_CAPAB2_FLAGS  ] = AOSI_CAPAB2_NEW_OSI      |
                                           AOSI_CAPAB2_NO_ASYNCHR   |
                                           AOSI_CAPAB2_SOLVE_MPS    ;
    
    info_int[ AOSI_IINFO_NOPT_CAT      ] = CBC_CAT_MAX;      // Must be >= 1
    info_int[ AOSI_IINFO_NOPT_INT      ] = CBC_OPT_INT_MAX;
    info_int[ AOSI_IINFO_NOPT_DBL      ] = CBC_OPT_DBL_MAX;
    info_int[ AOSI_IINFO_PROGRESS      ] = -1;
    
    if ( ! cbc_options_ordered ) {
    	cbc_order_options();
    }
    
    // Some solvers (not CBC) report messages about certain constraints and
    // variables. For those solvers a column row mapping is useful. To construct such
    // a mapping info_int[ AOSI_IINFO_START_COL ] and info_int[ AOSI_IINFO_START_ROW ]
    // are used. (See: the Solvers General option 'Solver Column Row Mapping'.)
	
    info_int[ AOSI_IINFO_START_COL     ] = 0;
    info_int[ AOSI_IINFO_START_ROW     ] = 0;
    
    info_dbl[ AOSI_DINFO_ZERO_TOL      ] = 1.0e-12;
    info_dbl[ AOSI_DINFO_INF           ] =  CBC_INF;
    info_dbl[ AOSI_DINFO_MIN_INF       ] = -CBC_INF;
    info_dbl[ AOSI_DINFO_INT_INF       ] =  CBC_INF;
    info_dbl[ AOSI_DINFO_INT_MIN_INF   ] = -CBC_INF;
    info_dbl[ AOSI_DINFO_SEMI_CONT_INF ] =  CBC_INF;    // Not used
    
    // Initialize sequence number of output files.
    
    cbc_seq_number = 0;
    
    // Get name of AIMMS project directory.
    
    _LONG_T   len;
	
    len = sizeof(cbc_project_dir) - 1;
    m_gen->GetEnvironment( "AIMMSPROJECT", cbc_project_dir, len );
    if ( cbc_project_dir[0] != '\0' ) {
        len = (_LONG_T) strlen( cbc_project_dir );
        cbc_project_dir[len]   = '/';
        cbc_project_dir[len+1] = '\0';
    }
    
    return AOSI_SUCCESS;
}


// The function GetOptionsInfo()
_LONG_T CbcSolverInfo::GetOptionsInfo( _LONG_T *info_int, double *info_dbl )
{
    /* This initialization routine is called by AIMMS to obtain information about
       the solver options.

       This function is called when the 'Project Options' tree is opened. */
    
    info_int[ AOSI_IINFO_VERSION  ] = AOSI_VERSION;
    info_int[ AOSI_IINFO_NOPT_CAT ] = CBC_CAT_MAX;      // Must be >= 1
    info_int[ AOSI_IINFO_NOPT_INT ] = CBC_OPT_INT_MAX;
    info_int[ AOSI_IINFO_NOPT_DBL ] = CBC_OPT_DBL_MAX;
    
    if ( ! cbc_options_ordered ) {
    	cbc_order_options();
    }
       
    return AOSI_SUCCESS;
}


// The function GetVersion()
void CbcSolverInfo::GetVersion(
    char    *version,           // version must be of length < 256
    _LONG_T  size )             // size of the buffer including '\0'
{
    /* This function places the version number of the solver (or the solver
       interface), if any, in a character buffer provided by AIMMS. The
       returned string will be used in the 'About AIMMS' menu option. */
       
    char   buf[256];

    sprintf( buf, "%s (DLL)", CBC_VERSION );

    strncpy( version, buf, size );
    version[ size - 1 ] = '\0';
}


// The function GetHelpFile()
void CbcSolverInfo::GetHelpFile(
    char    *filename,         // filename must be of length < 512
    _LONG_T  size )            // size of the buffer including '\0'
{
	strncpy( filename, CBC_HELP_FILE_NAME, size );
	filename[ size - 1 ] = '\0';
}


// The function GetLogFile()
_LONG_T CbcSolverInfo::GetLogFile( _TCHAR *file_name )
{
    _TCHAR    dir[MAX_DIR_LEN];
    _LONG_T   len;
    
    len = MAX_DIR_LEN;
    m_gen->GetLogDirNameWchar( dir, &len );
    assert( len != -1 );
    
    SPRINTF( file_name, _T("%s/%s"), dir, CBC_STATUS_FILE_NAME_UNI );
        
    return AOSI_SUCCESS;
}


// The function Shutdown()
void CbcSolverInfo::Shutdown( void )
{
#ifdef DEBUG
    if ( cbc_logfile != NULL ) {
        fclose( cbc_logfile );
        cbc_logfile = NULL;
    }
#endif   
}


// The function Reset()
void CbcMathProgramInstance::Reset( void )
{
    /* This function removes the optimization task that uniquely corresponds to the
       math program instance from the solver environment and frees the associated
       dynamically allocated memory chunks. This function is called when AIMMS has
       no need for further access to the math program instance. */
    
    // Delete optimization task.
    
    if ( cbc_model ) {
        delete cbc_model;
        cbc_model = NULL;
    }
    
    if ( clp_solver ) {
    	delete clp_solver;
    	clp_solver = NULL;
    }
    
    // Initialize handle.
    
    cbc_init_handle();
    
    // Initialize the solver options. When a new problem is solved, a new 
    // cbc_task is created and the options in CBC are initialized on
    // their default value. Therefore we have to reset the current option
    // values stored.
    
    cbc_init_options();
    
    cbc_mp_init        = false;
    cbc_user_interrupt = false;
}


// The function DoSolve()
void CbcMathProgramInstance::DoSolve(
    _LONG_T  type,            // NEW or UPDATE
    _LONG_T  options,         // 1 if options have to be adapted
    _LONG_T *int_param,       // Integer parameter array of size IPARAM_MAX
    double  *dbl_param,       // Double parameter array of size DPARAM_MAX
    size_t  *int_stat,        // Integer statistics array of size AOSI_ISTAT_MAX
    double  *dbl_stat )       // Double statistics array of size AOSI_DSTAT_MAX
{
    /* The main solve routine. This function should pass the model from AIMMS to
       the solver, call the solver to solve the problem, and retrieve and pass
       problem statistics. The arrays int_param and dbl_param (input) contain
       information about the problem; the arrays int_stat and dbl_stat (output)
       contain the problem statistics passed to AIMMS. */
       
    int   res;
	
	// Initialize the solver options.
	
	if ( ! cbc_mp_init ) {
		cbc_init_options();
    	
    	cbc_mp_init = true;
    }
       
    // Initialize statistics.

    int_stat[ AOSI_ISTAT_MODEL_STAT  ] = MODELSTAT_NOT_SOLVED;
    int_stat[ AOSI_ISTAT_SOLVER_STAT ] = SOLVERSTAT_NOT_CALLED;
    int_stat[ AOSI_ISTAT_TOTAL_ITER  ] = 0;
    int_stat[ AOSI_ISTAT_NODES       ] = 0;
    int_stat[ AOSI_ISTAT_MEM_USED    ] = 0;
    int_stat[ AOSI_ISTAT_RESTART_OFF ] = 0;
    dbl_stat[ AOSI_DSTAT_OBJ         ] = AOSI_NA_REP;
    
    cbc_start_time = m_mp->GetTime();
    
    cbc_user_interrupt = false;
    
    // Get and store modified global solver and CBC specific options.

    if ( options ) {
        cbc_get_options();
    }

#ifdef DEBUG
	if ( type == AOSI_DUAL_MODEL_NEW    ||
	     type == AOSI_VAR_MODEL_NEW     ||
	     type == AOSI_DUAL_MODEL_UPDATE ||
	     type == AOSI_VAR_MODEL_UPDATE  ) {
 		cbc_opt_tracing = cbc_last_opt_tracing;
 	}

    cbc_solve_no ++;
    
    if ( cbc_open_log_file_and_print_parameters( type, options, int_param, dbl_param ) ) {
        return;
    }
#endif
    
#ifdef CBC_PARALLEL
    // Create mutex if doing parallel solve. Used to synchronize callbacks.
	
	if ( cbc_int_opt_val[CBC_OPT_THREAD_LIMIT] > 1 ) {
        cbc_handle . m_Mutex = new PDTThread::MutexControl;
    } else {
    	cbc_handle . m_Mutex = NULL;
    }
#endif
    
    res = 0;
    
    switch ( type ) {
      case AOSI_MODEL_NEW:
      case AOSI_DUAL_MODEL_NEW:
      case AOSI_VAR_MODEL_NEW:
        res = cbc_call_solver( int_param, dbl_param, /* update */ 0, type );
        break;
      case AOSI_MODEL_UPDATE:
      case AOSI_DUAL_MODEL_UPDATE:
      case AOSI_VAR_MODEL_UPDATE:
        res = cbc_call_solver( int_param, dbl_param, /* update */ 1, type );
        break;
      case AOSI_POSTSOLVE_MODEL_UPDATE:
        cbc_init_postsolve();
        res = cbc_call_solver( int_param, dbl_param, /* update */ 1, type );
        break;
      case AOSI_DUAL_MODEL_END:
      case AOSI_VAR_MODEL_END:
        break;
      case AOSI_POSTSOLVE_MODEL_END:
        cbc_end_postsolve();
        break;
      default:
        cbc_error( "Illegal model type supplied" );
        cbc_handle . finished = true;
        res = 1;
        break;
    }
    
#ifdef CBC_PARALLEL
    // Delete mutex.
    
    if ( cbc_handle . m_Mutex ) {
        delete cbc_handle . m_Mutex;
        cbc_handle . m_Mutex = NULL;
    }
#endif
    
    if ( res ) {
    	int_stat[ AOSI_ISTAT_MODEL_STAT  ] = cbc_handle . model_status;
    	int_stat[ AOSI_ISTAT_SOLVER_STAT ] = cbc_handle . solver_status;
    	return;
    }
    
    // Compute statistics.
    
    int_stat[ AOSI_ISTAT_MODEL_STAT     ] = cbc_handle . model_status;
    int_stat[ AOSI_ISTAT_SOLVER_STAT    ] = cbc_handle . solver_status;
    int_stat[ AOSI_ISTAT_TOTAL_ITER     ] = cbc_handle . iter;
    int_stat[ AOSI_ISTAT_NODES          ] = cbc_handle . nodes;
    int_stat[ AOSI_ISTAT_MEM_USED       ] = cbc_get_memory_used();
    int_stat[ AOSI_ISTAT_RESTART_OFF    ] = 0;
    
    // Current settings of postsolve options for CBC.
    
    int_stat[ AOSI_ISTAT_POSTSOLVE_CONT ] = 1;
    int_stat[ AOSI_ISTAT_POSTSOLVE_INT  ] = 1;
    int_stat[ AOSI_ISTAT_MIP_CALC_SENS  ] = 1;
    
    // Like most solvers, CBC does not take care of the constant in the AIMMS
    // objective function. Value AOSI_DSTAT_OBJ is handled by AIMMS but value
    // AOSI_DSTAT_LP_BEST not (which is a bit silly).
	
    dbl_stat[ AOSI_DSTAT_OBJ         ] = cbc_handle . objval;

	if ( cbc_handle . mip_best_poss > -AIMMS_INF &&
         cbc_handle . mip_best_poss <  AIMMS_INF ) {
        dbl_stat[ AOSI_DSTAT_LP_BEST ] = cbc_handle . mip_best_poss + 
                                         cbc_handle . obj_constant;
    } else {
        dbl_stat[ AOSI_DSTAT_LP_BEST ] = cbc_handle . mip_best_poss;
    }
	
    if ( type == AOSI_MODEL_NEW ) {
        sprintf( cbc_msg, "Memory in use by CBC %s: " portingIntFmtStr " bytes.",
                 CBC_VERSION, int_stat[ AOSI_ISTAT_MEM_USED ] );
        m_gen->PassMessage( AOSI_PRIO_REMARK, cbc_msg );
    }
}


// The function cbc_get_col_sol_during_callback()
void CbcMathProgramInstance::cbc_get_col_sol_during_callback( _LONG_T first_col, _LONG_T last_col,
                                                              _LONG_T type_flag, double *lev,
                                                              double *mar, _LONG_T *basic )
{
    int   i, ncols, colno = -1;

    if ( type_flag & AOSI_RETURN_SOLUTION_LEV ) {
        if ( cbc_handle . model_type != CBC_MIP_MODEL ) {
            // Print next message only once.
            
            if ( ! cbc_handle . cb_lp_var_error ) {
                sprintf( cbc_msg, "CBC warning: Retrieving current variable values "
                                  "is only possible for problems with integer variables" );
                m_gen->PassMessage( AOSI_PRIO_ALWAYS, cbc_msg );
                cbc_handle . cb_lp_var_error = true;
            }
            return;
        }
        
        ncols = last_col - first_col + 1;
        
        memset( lev, 0, ncols * sizeof( double ) );
        
        if ( cbc_incumbent ) {
        	if ( cbc_original_cols ) {
        		for ( i=0; i<cbc_nr_preproc_cols; i++ ) {
        			colno = cbc_original_cols[i];
        			if ( colno >= first_col ) {
        				if ( colno <= last_col ) {
        					lev[colno-first_col] = cbc_incumbent[i];
        				} else {
        					break;
        				}
        			}
        		}
        	} else if ( cbc_nr_preproc_cols == cbc_handle . ncols ) {
        		for ( i=0; i<ncols; i++ ) {
        			lev[i] = cbc_incumbent[first_col + i];
        		}
        	}
        }
        
#ifdef DEBUG
    	char      colname[64];
    	_LONG_T   len;
    	
    	if ( cbc_opt_tracing && cbc_incumbent ) {
    		fprintf( cbc_logfile, "Incumbent: Objective value = %.5f\n", cbc_handle . obj_best );
    		
    	    for ( i=0; i<ncols; i++ ) {
    	        len = sizeof( colname );
    	        m_mp->GetRowColumnName( AOSI_NAME_COLUMN, i, colname, &len );
    	
    	        fprintf( cbc_logfile, "colno %d ", first_col + i );
    	        fprintf( cbc_logfile, "lev %.16E ", lev[i] );
    	        fprintf( cbc_logfile, "   (%s)\n", colname );
    	    }
    	    
    	    fflush( cbc_logfile );
    	}
#endif
    }
    
    // It is not possible to get information about marginals or the basis from a
    // CBC callback.
    
    if ( type_flag & AOSI_RETURN_SOLUTION_MAR   ) {
        memset( mar,   0, ncols * sizeof( double  ) );
    }
    if ( type_flag & AOSI_RETURN_SOLUTION_BASIC ) {
        memset( basic, 0, ncols * sizeof( _LONG_T ) );
    }
}


// The function GetColumnSolution()
void CbcMathProgramInstance::GetColumnSolution(
    _LONG_T  first_col,       // retrieve columns first_col - last_col inclusive
    _LONG_T  last_col,
    _LONG_T  type_flag,       // indicates which info should be returned
    double  *lev,             // level values
    double  *mar,             // marginal values
    _LONG_T *basic )          // 1 if basic
{
    _LONG_T         i, j, ncols, level, marginals, base;
    int            *col_bas = NULL, *row_bas = NULL, tot_cols, tot_rows, mem_size;
    const double   *values;
    
    if ( ! cbc_handle . finished ) {
        cbc_get_col_sol_during_callback( first_col, last_col, type_flag, lev, mar, basic );
        return;
    }
    
    OsiSolverInterface * solver = cbc_model->solver();
	
    ncols = last_col - first_col + 1;

    level     = ( type_flag & AOSI_RETURN_SOLUTION_LEV   );
    marginals = ( type_flag & AOSI_RETURN_SOLUTION_MAR   );
    base      = ( type_flag & AOSI_RETURN_SOLUTION_BASIC );
    
    if ( level )
        memset( lev  , 0, ncols * sizeof( double  ) );
    if ( marginals )
        memset( mar  , 0, ncols * sizeof( double  ) );
    if ( base )
        memset( basic, 0, ncols * sizeof( _LONG_T ) );
        
    if ( first_col >= cbc_handle . ncols )
        return;     // Columns out of range
        
    if ( cbc_handle . model_type == CBC_MIP_MODEL ) {
        base      = 0;
        marginals = 0;
    }
	
    if ( level ) {
    	if ( cbc_handle . model_type == CBC_MIP_MODEL ) {
    		values = cbc_model->getColSolution();
    	} else {
    		values = solver->getColSolution();
    	}
        
        if ( values ) {
        	for ( i=0; i<ncols; i++ ) {
            	lev[i] = values[first_col+i];
            }
        }
    }
    
    // Basic information from CBC is also needed for marginals.
	
    if ( base || marginals ) {
    	tot_cols = clp_solver->getNumCols();
    	tot_rows = clp_solver->getNumRows();
    	
    	mem_size = tot_cols * sizeof( int )   // col_bas
                 + tot_rows * sizeof( int );  // row_bas
        
        col_bas = (int *) m_mp->AllocateMemory( mem_size );
    	if ( ! col_bas ) {
    	    cbc_error( "Failed to retrieve column basis" );
            return;
    	}
    	
    	row_bas = (int *) ( col_bas + tot_cols );
        
        solver->getBasisStatus( col_bas, row_bas );
        
        if ( base ) {
            for ( i=first_col, j=0; i<=last_col; i++, j++ ) {
                if ( col_bas[i] == CBC_STATUS_BASIC ) {
                    basic[j] = AOSI_RETURN_BASIC_STATUS_BASIC;
                } else if ( col_bas[i] == CBC_STATUS_SUPERBASIC ) {
                    basic[j] = AOSI_RETURN_BASIC_STATUS_SUPERBASIC;
                } else {
                    basic[j] = AOSI_RETURN_BASIC_STATUS_NON_BASIC;
                }
            }
        }
    }
    
    if ( marginals ) {
        values = solver->getReducedCost();
        
        if ( values ) {
        	for ( i=first_col, j=0; i<=last_col; i++, j++ ) {
                if ( col_bas[i] == CBC_STATUS_BASIC ) {
                    mar[j] = 0.0;
                } else {
                	mar[j] = values[i];
                	
                    if ( mar[j] == 0.0 ) {
                        mar[j] = cbc_handle . marginal_zero;
                    }
                }
            }
        }
    }
	
#ifdef DEBUG
    char      colname[64];
    _LONG_T   len;

    if ( cbc_opt_tracing ) {
        for ( i=0; i<ncols; i++ ) {
            len = sizeof( colname );
            m_mp->GetRowColumnName( AOSI_NAME_COLUMN, i, colname, &len );

            fprintf( cbc_logfile, "colno %d ", first_col + i );
            if ( level )
                fprintf( cbc_logfile, "lev %.16E ", lev[i] );
            if ( marginals )
                fprintf( cbc_logfile, "mar %.16E ", mar[i] );
            if ( base )
                fprintf( cbc_logfile, "col_bas %d", col_bas[i+first_col] );
            fprintf( cbc_logfile, "   (%s)\n", colname );
        }
        
        fflush( cbc_logfile );
    }
#endif
	
	if ( col_bas ) {
		m_mp->FreeMemory( col_bas );
	}
}


// The function GetRowSolution()
void CbcMathProgramInstance::GetRowSolution(
    _LONG_T  first_row,       // retrieve rows first_row - last_row inclusive
    _LONG_T  last_row,
    _LONG_T  type_flag,       // indicates which info should be returned
    double  *lev,             // level values
    double  *mar,             // marginal values
    _LONG_T *basic )          // 1 if basic
{
    _LONG_T         i, j, nrows, level, marginals, base;
    int            *col_bas = NULL, *row_bas = NULL, tot_cols, tot_rows, mem_size;
    const double   *values;
    
    OsiSolverInterface * solver = cbc_model->solver();

    nrows = last_row - first_row + 1;

    level     = ( type_flag & AOSI_RETURN_SOLUTION_LEV   );
    marginals = ( type_flag & AOSI_RETURN_SOLUTION_MAR   );
    base      = ( type_flag & AOSI_RETURN_SOLUTION_BASIC );
    
    if ( level )
        memset( lev  , 0, nrows * sizeof( double  ) );
    if ( marginals )
        memset( mar  , 0, nrows * sizeof( double  ) );
    if ( base )
        memset( basic, 0, nrows * sizeof( _LONG_T ) );
        
    if ( first_row >= cbc_handle . nrows )
        return;     // Rows out of range
        
    if ( cbc_handle . model_type == CBC_MIP_MODEL ) {
        base      = 0;
        marginals = 0;
    }
    
    if ( level ) {
        if ( cbc_handle . model_type == CBC_MIP_MODEL ) {
    		values = cbc_model->getRowActivity();
    	} else {
    		values = solver->getRowActivity();
    	}
        
        if ( values ) {
        	for ( i=0; i<nrows; i++ ) {
            	lev[i] = values[first_row+i];
            }
        }
    }
    
    if ( base || marginals ) {
        tot_cols = clp_solver->getNumCols();
    	tot_rows = clp_solver->getNumRows();
    	
    	mem_size = tot_cols * sizeof( int )   // col_bas
                 + tot_rows * sizeof( int );  // row_bas
        
        col_bas = (int *) m_mp->AllocateMemory( mem_size );
    	if ( ! col_bas ) {
    	    cbc_error( "Failed to retrieve row basis" );
            return;
    	}
    	
    	row_bas = (int *) ( col_bas + tot_cols );
        
        solver->getBasisStatus( col_bas, row_bas );
        
        if ( base ) {
            for ( i=first_row, j=0; i<=last_row; i++, j++ ) {
                if ( row_bas[i] == CBC_STATUS_BASIC ) {
                    basic[j] = AOSI_RETURN_BASIC_STATUS_BASIC;
                } else if ( row_bas[i] == CBC_STATUS_SUPERBASIC ) {
                    basic[j] = AOSI_RETURN_BASIC_STATUS_SUPERBASIC;
                } else {
                    basic[j] = AOSI_RETURN_BASIC_STATUS_NON_BASIC;
                }
            }
        }
    }

    if ( marginals ) {
    	values = solver->getRowPrice();
        
        if ( values ) {
        	for ( i=first_row, j=0; i<=last_row; i++, j++ ) {
                if ( row_bas[i] == CBC_STATUS_BASIC ) {
                    mar[j] = 0.0;
                } else {
                	mar[j] = values[i];
                	
                    if ( mar[j] == 0.0 ) {
                        mar[j] = cbc_handle . marginal_zero;
                    }
                }
            }
        }
    }
        
#ifdef DEBUG
    char      rowname[64];
    _LONG_T   len;

    if ( cbc_opt_tracing ) {
        for ( i=0; i<nrows; i++ ) {
            len = sizeof( rowname );
            m_mp->GetRowColumnName( AOSI_NAME_ROW, i, rowname, &len );        

            fprintf( cbc_logfile, "rowno %d ", first_row + i );
            if ( level )
                fprintf( cbc_logfile, "lev %.16E ", lev[i] );
            if ( marginals )
                fprintf( cbc_logfile, "mar %.16E ", mar[i] );
            if ( base )
                fprintf( cbc_logfile, "row_bas %d", row_bas[i+first_row] );
            fprintf( cbc_logfile, "   (%s)\n", rowname );
        }
        
        fflush( cbc_logfile );
    }
#endif

    if ( col_bas ) {
		m_mp->FreeMemory( col_bas );
	}
}


// The function FindIIS()
void CbcMathProgramInstance::FindIIS( _LONG_T *no_rows, _LONG_T *no_cols )
{
	// CBC has no support for calculating the IIS.
	
	*no_rows = 0;
    *no_cols = 0;
}


// The function GetIIS()
_LONG_T CbcMathProgramInstance::GetIIS( _LONG_T no_rows, _LONG_T *rownos, _LONG_T no_cols,
                                        _LONG_T *colnos )
{
	// CBC has no support for calculating the IIS.
	
    return AOSI_FAILURE;
}


// The function GetSensitivityData()
_LONG_T CbcMathProgramInstance::GetSensitivityData( _LONG_T first, _LONG_T last, _LONG_T type,
                                                    double *lower, double *upper )
{
	// CBC cannot calculate the smallest and largest values for the RHS of a
	// constraint so that the optimal basis remains constant, and therefore the
    // capability flag AOSI_CAPAB_RHS_SENSE is not set.
    
    // CBC cannot calculate the smallest and largest values for the objective
	// coefficient of a variable so that the optimal basis remains constant,
    // and therefore the capability flag AOSI_CAPAB_OBJ_SENSE is not set.
    
    // Note that shadow price ranges and variables ranges are calculated by AIMMS
    // (using the solver).
       
    return AOSI_FAILURE;
}


// The function SetCallbackMoments()
_LONG_T CbcMathProgramInstance::SetCallbackMoments( _LONG_T flags, _LONG_T *info )
{
    cbc_handle . cb_flags = flags;
    cbc_handle . cb_iter  = info[ AOSI_CB_INFO_ITER ];
	
    return AOSI_SUCCESS;
}


// The function DoTuning()
void CbcMathProgramInstance::DoTuning(
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
	// Not supported.
}



// The function cbc_load_mps_file()
int CbcMathProgramInstance::cbc_load_mps_file( char *filename )
{
	int       res, nint_cols;
	double    dir;
	char     *isLp;
	
	res = cbc_create_optimization_task();
    if ( res ) return 1;
    
    isLp = strstr( filename, ".lp" );
    
    if ( isLp ) {
    	res = clp_solver->readLp( filename, 1e-8 );
    	if ( res ) {
    	    cbc_error( "Reading LP file failed" );
    	    return 1;
    	}
    } else {
    	res = clp_solver->readMps( filename, "" );
    	if ( res ) {
    	    cbc_error( "Reading MPS file failed" );
    	    return 1;
    	}
    }
    
    if ( cbc_model ) {
    	delete cbc_model;
    }
    
    cbc_model = new CbcModel( *clp_solver );
    if ( ! cbc_model ) {
        cbc_error( "Failed to create optimization task" );
        return 1;
    }
    
    // Load cbc_model with options first.
	
	CbcMain0( *cbc_model );
    
    // Determine model type.
    
    nint_cols = cbc_model->numberIntegers();
    
    if ( nint_cols > 0 ) {
    	cbc_handle . model_type = CBC_MIP_MODEL;
    	cbc_handle . mps_type   = AOSI_MODEL_MIP;
    } else {
    	cbc_handle . model_type = CBC_LP_MODEL;
    	cbc_handle . mps_type   = AOSI_MODEL_LP;
    }
    
    // Get optimization direction.
    
    dir = cbc_model->getObjSense();
	
	if ( dir == CBC_OBJECTIVE_SENSE_MAXIMIZE ) {
		cbc_handle . direction = DIRECTION_MAX;
	} else if ( dir == CBC_OBJECTIVE_SENSE_MINIMIZE ) {
		cbc_handle . direction = DIRECTION_MIN;
	} else {
		cbc_handle . direction = DIRECTION_FEAS;
	}
	
	return 0;
}


// The function cbc_solve_mps()
int CbcMathProgramInstance::cbc_solve_mps( _LONG_T int_param[], double dbl_param[],
                                           char *filename, _LONG_T create_sol_file )
{
    int     res;
    char   *str1, *str2, sol_file[1024];

    cbc_init_solve( int_param, dbl_param, /* update */ 0, AOSI_MODEL_NEW );
    
    // Initialize handle for callbacks etc.
    
    cbc_handle . basis    = 0;
    cbc_handle . cb_flags = 0;
    
    // It seems that CBC always solves a mps file as a minimization problem.
    
    res = cbc_load_mps_file( filename );
    if ( res ) return 1;
    
    // Set CBC options.
    
    std::list<std::string> opt_list;
    
    res = cbc_set_options( opt_list );
    if ( res ) return 1;

	// If presolve is switched on then phase is presolve unless proven otherwise.

    if ( cbc_int_opt_val[CBC_OPT_PRESOLVE] != 0 ) {
        m_callback->PassPhaseInfo( AOSI_PHASE_PRESOLVE );
    }
    
#ifdef CBC_PARALLEL
    // Create mutex if doing parallel solve. Used to synchronize callbacks.
    
    if ( cbc_int_opt_val[CBC_OPT_THREAD_LIMIT] > 1 ) {
        cbc_handle . m_Mutex = new PDTThread::MutexControl;
    } else {
    	cbc_handle . m_Mutex = NULL;
    }
#endif
    
    // Pass progress information to AIMMS just before solve starts.
    
    cbc_progress_info( /* end */ false );

    // Actually solve problem.

    res = cbc_solve_model( opt_list );
    
#ifdef CBC_PARALLEL
    // Delete mutex.
    
    if ( cbc_handle . m_Mutex ) {
        delete cbc_handle . m_Mutex;
        cbc_handle . m_Mutex = NULL;
    }
#endif
    
    if ( res ) return 1;
    
    // Pass progress information to AIMMS just after solve.
	
    cbc_progress_info( /* end */ true );
    
    // Save binary solution file.
    
    if ( create_sol_file ) {
    	strcpy( sol_file, filename );
    	
    	str1 = strstr( sol_file, ".mps" );
    	str2 = strstr( sol_file, ".lp"  );
    	if ( str1 || str2 ) {
    		if ( str1 ) {
    			strcpy( str1, ".sol\0" );
    		} else {
    			strcpy( str2, ".sol\0" );
    		}
    		
    		OsiSolverInterface * solver = cbc_model->solver();
    		OsiClpSolverInterface * clpSolver = dynamic_cast<OsiClpSolverInterface *> (solver);
    		ClpSimplex * clpSimplex = clpSolver->getModelPtr();
    
    		saveSolution( clpSimplex, sol_file );
    	}
	}
    
    cbc_handle . finished = true;

    return 0;
}


// The function DoMpsSolve()
void CbcMathProgramInstance::DoMpsSolve(
	_LONG_T  options,
	_LONG_T  int_param[],
	double   dbl_param[],
	size_t   int_stat[],
	double   dbl_stat[],
	_LONG_T  filename_len,
	_TCHAR  *filenameW,
	_LONG_T  create_sol_file )
{
	char   filename[1024];
	
	// Initialize the solver options.

	if ( ! cbc_mp_init ) {
		cbc_init_options();
    	
    	cbc_mp_init = true;
    }
       
    // Initialize statistics.

    int_stat[ AOSI_ISTAT_MODEL_STAT  ] = MODELSTAT_NOT_SOLVED;
    int_stat[ AOSI_ISTAT_SOLVER_STAT ] = SOLVERSTAT_NOT_CALLED;
    int_stat[ AOSI_ISTAT_TOTAL_ITER  ] = 0;
    int_stat[ AOSI_ISTAT_NODES       ] = 0;
    int_stat[ AOSI_ISTAT_MEM_USED    ] = 0;
    int_stat[ AOSI_ISTAT_RESTART_OFF ] = 0;
    dbl_stat[ AOSI_DSTAT_OBJ         ] = AOSI_NA_REP;
    
    cbc_start_time = m_mp->GetTime();
    
    cbc_user_interrupt = false;
    
    // Get and store modified global solver and CBC specific options.

    if ( options ) {
        cbc_get_options();
    }

#ifdef DEBUG
    cbc_solve_no ++;
    
    if ( cbc_open_log_file_and_print_parameters( AOSI_MODEL_NEW, options, int_param, dbl_param ) ) {
        return;
    }
#endif

	STRCPY_UNI_TO_ASC( filename, filenameW );

    cbc_solve_mps( int_param, dbl_param, filename, create_sol_file );
	
    // Compute statistics.
    
    int_stat[ AOSI_ISTAT_MODEL_STAT  ] = cbc_handle . model_status;
    int_stat[ AOSI_ISTAT_SOLVER_STAT ] = cbc_handle . solver_status;
    int_stat[ AOSI_ISTAT_TOTAL_ITER  ] = cbc_handle . iter;
    int_stat[ AOSI_ISTAT_NODES       ] = cbc_handle . nodes;
    int_stat[ AOSI_ISTAT_MEM_USED    ] = cbc_get_memory_used();
    int_stat[ AOSI_ISTAT_RESTART_OFF ] = 0;
    
    // Like most solvers, CBC does not take care of the constant in the AIMMS
    // objective function. Value AOSI_DSTAT_OBJ is handled by AIMMS but value
    // AOSI_DSTAT_LP_BEST not (which is a bit silly).
	
	dbl_stat[ AOSI_DSTAT_OBJ         ] = cbc_handle . objval;

	if ( cbc_handle . mip_best_poss > -AIMMS_INF &&
         cbc_handle . mip_best_poss <  AIMMS_INF ) {
        dbl_stat[ AOSI_DSTAT_LP_BEST ] = cbc_handle . mip_best_poss + 
                                         cbc_handle . obj_constant;
    } else {
        dbl_stat[ AOSI_DSTAT_LP_BEST ] = cbc_handle . mip_best_poss;
    }
	
    sprintf( cbc_msg, "Memory in use by CBC %s: " portingIntFmtStr " bytes.",
             CBC_VERSION, int_stat[ AOSI_ISTAT_MEM_USED ] );
    m_gen->PassMessage( AOSI_PRIO_REMARK, cbc_msg );
}


// The function CalculateSubgradient()
_LONG_T CbcMathProgramInstance::CalculateSubgradient(
	double   delta_rhs[],
	_LONG_T  delta_rhs_len,
	double   subgr[],
	_LONG_T  subgr_len )
{
	// Not supported.
	
	return AOSI_FAILURE;
}



