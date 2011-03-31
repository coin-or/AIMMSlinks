// Copyright (C) 2009 Paragon Decision Technology B.V. and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// $Id$
//
// Author: Marcel Hunting

#ifndef _DEFINED_AIMMSOSI_SOLVERCBCIMPL_H
#define _DEFINED_AIMMSOSI_SOLVERCBCIMPL_H

#include "IAimmsOSI.h"
#include "cbc_interface.h"
#include "cbc_options.h"

class CbcSolverInfo : public ISolverInfo
{
public:
#ifdef _AIMMS390_
	CbcSolverInfo(IAimmsSolverConnection *gen) : m_gen(gen)
	          , m_si_spec(gen->GetExtendedConnection()), cbc_options_ordered(false) {}
#else
	CbcSolverInfo(IAimmsSolverConnection *gen) : m_gen(gen)
	          , cbc_options_ordered(false) {}
#endif
protected:
	virtual ~CbcSolverInfo() {}
public:
        virtual int RegisterAsSolver(
                char     name[],
                _LONG_T *model_flags);
        virtual _LONG_T GetInitialInfo(
                _LONG_T  info_int[],
                double   info_dbl[],
                char    *msg);
        virtual _LONG_T GetOptionsInfo(
                _LONG_T  info_int[],
                double   info_dbl[]);
        virtual void Shutdown(void);
        virtual void GetIntegerOption(
                _LONG_T  int_option_no,
                char     name[],
                _LONG_T  values[],
                _LONG_T  optinfo[]);
        virtual void GetIntegerKeyword(
                _LONG_T  int_option_no,
                _LONG_T  index,
                char     keyword[]);
        virtual void GetDoubleOption(
                _LONG_T  dbl_option_no,
                char     name[],
                double   values[],
                _LONG_T  optinfo[]);
        virtual void GetCategoryName(
                _LONG_T  category,
                char     name[]);
        virtual void GetVersion(
                char     version[],
                _LONG_T  size);
        virtual void GetHelpFile(
                char     filename[],
                _LONG_T  size);
        virtual _LONG_T GetLogFile(
                _TCHAR   filename[]);
                
        virtual class ISolverMathProgramInstance *CreateMathProgramInstance(
        	class IAimmsMathProgramInfo *mp);
        	
        virtual void Delete(void);
private:
	IAimmsSolverConnection   *m_gen;
#ifdef _AIMMS390_
	IAimmsExtendedSolverConnection   *m_si_spec;
#endif
	
	bool cbc_options_ordered;          // If 1, options have been ordered
	
	void cbc_order_options( void );
};


class CbcMathProgramInstance : public ISolverMathProgramInstance
{
public:
#ifdef _AIMMS390_
	CbcMathProgramInstance(class IAimmsMathProgramInfo *mp, IAimmsSolverConnection *gen,
	                       IAimmsExtendedSolverConnection *si_spec);
#else
	CbcMathProgramInstance(class IAimmsMathProgramInfo *mp, IAimmsSolverConnection *gen);
#endif

protected:	
	virtual ~CbcMathProgramInstance() {}
public:
        virtual void DoSolve(
                _LONG_T  type,
                _LONG_T  options,
                _LONG_T  int_param[],
                double   dbl_param[],
                size_t   int_stat[],
                double   dbl_stat[]);
        virtual _LONG_T SetCallbackMoments(
                _LONG_T  flags,
                _LONG_T  info[]);
        virtual void GetColumnSolution(
                _LONG_T  first_col,
                _LONG_T  last_col,
                _LONG_T  type_flag,
                double   lev[],
                double   mar[],
                _LONG_T  basic[]);
        virtual void GetRowSolution(
                _LONG_T  first_row,
                _LONG_T  last_row,
                _LONG_T  type_flag,
                double   lev[],
                double   mar[],
                _LONG_T  basic[]);
        virtual void FindIIS(
                _LONG_T *no_rows,
                _LONG_T *no_cols);
        virtual _LONG_T GetIIS(
                _LONG_T  no_rows,
                _LONG_T *rownos,
                _LONG_T  no_cols,
                _LONG_T *colnos);
        virtual _LONG_T GetSensitivityData(
                _LONG_T  first,
                _LONG_T  last,
                _LONG_T  type,
                double   lower[],
                double   upper[]);
        virtual void Reset(void);
        virtual void Delete(void);
        virtual void DoTuning(
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
                char    *filenames);
        virtual void DoMpsSolve(
                _LONG_T  options,
                _LONG_T  int_param[],
                double   dbl_param[],
                size_t   int_stat[],
                double   dbl_stat[],
                _LONG_T  filename_len,
                _TCHAR  *filename,
                _LONG_T  create_sol_file);
        virtual _LONG_T CalculateSubgradient(
                double   delta_rhs[],
                _LONG_T  delta_rhs_len,
                double   subgr[],
                _LONG_T  subgr_len);
private:
	IAimmsSolverConnection                    *m_gen;
	IAimmsMathProgramInfo                     *m_mp;
	IAimmsMathProgramMatrixManipulationInfo   *m_manipulation;
	IAimmsMathProgramCallback                 *m_callback;
#ifdef _AIMMS390_
	IAimmsExtendedSolverConnection            *m_si_spec;
#endif
	
	cbc_handle_struct    	cbc_handle;           // Handle
	
	bool    cbc_mp_init;                          // If 1, the mp has been initialized
	
	// Options related variables.
	
	int     cbc_int_opt_val[ CBC_OPT_INT_MAX ];   // Array containing the current 
                                                  // values of the integer options
	double  cbc_dbl_opt_val[ CBC_OPT_DBL_MAX ];   // Array containing the current 
                                                  // values of the double options

public:
	cbc_handle_struct        *cbc_current;    // Pointer to handle

	CbcModel                 *cbc_model;      // CBC optimization task
	OsiClpSolverInterface    *clp_solver;     // CBC uses LP solver CLP

	// Global variables.
	
	char    cbc_msg[256];                 // Buffer for building (error) messages
	bool    cbc_user_interrupt;           // If 1, there was a user interupt
	
	// Progress variables.
	
	int     cbc_next_to_print_iter;       // Next iteration for which a progress should be
	                                      // printed
	int     cbc_start_time;               // Start time of the solve in 1/100th of a
	                                      // second
	
	int     cbc_progress;                 // Number of iterations after which a progress
	                                      // report should be sent to AIMMS. If its value
	                                      // is 0, no progress reports are made
	double *cbc_incumbent;                // Pointer to incumbent solution in CBC; can be
	                                      // NULL
	int     cbc_nr_preproc_cols;          // Number of columns after preprocessing
	int    *cbc_original_cols;            // Pointer to array (of length cbc_nr_preproc_cols)
	                                      // containing the mapping of preprocessed columns
	                                      // to original columns
	
	// Option variables.
	
	int     cbc_opt_priority;             // Minimum priority of message passed to AIMMS
	int     cbc_opt_status_file;          // If 1, let CBC output to status file
	int     cbc_opt_iter_limit;           // Maximal number of iterations
	int     cbc_opt_time_limit;           // Max. duration time of a solve (in seconds)
	int     cbc_opt_integer_sols;         // Max. number of integer solutions to find
	double  cbc_mip_absolute_gap;         // MIP absolute optimality tolerance
	double  cbc_mip_relative_gap;         // MIP relative optimality tolerance
	double  cbc_mip_cutoff;               // MIP cutoff

	const char** cbc_args;                // Arguments passed to CbcMain1
	int     cbc_nr_args;                  // Number of arguments passed to CbcMain1
	
	int     cbc_lp_phase;                 // Phase corresponding to LP method
	
	// Postsolve related variables.
	
	bool    cbc_inside_postsolve;         // If 1, we are inside a postsolve (initiated by
	                                      // AIMMS)
	int     cbc_opt_iter_limit_pre_post;  // Value of option cbc_opt_iter_limit before
	                                      // postsolve
	int     cbc_opt_time_limit_pre_post;  // Value of option cbc_opt_time_limit before
	                                      // postsolve
	int     cbc_opt_mps_pre_post;         // Value of option CBC_OPT_MPS before postsolve
	
	// Debug related variables.
	
#ifdef DEBUG
	int     cbc_opt_tracing;              // If 1, write to 'cbcXX.log'
#endif

public:
	IAimmsMathProgramCallback * get_callback() { return m_callback; };
	
	void cbc_error( char *errmsg );
	size_t cbc_get_memory_used( void );
	void cbc_progress_info( bool end );
	void cbc_install_callback( void );
	int  cbc_aimms_callback( int callback_type );
#ifdef DEBUG
	int  cbc_is_feasible_solution( void );
#endif

private:
	void cbc_init_handle( void );
	
#ifdef DEBUG
	int  cbc_open_log_file_and_print_parameters( _LONG_T type, _LONG_T options,
	                                             _LONG_T *int_param, double *dbl_param );
	void cbc_print_parameters( _LONG_T options, _LONG_T *int_param, double *dbl_param );
	void cbc_print_retrieved_columns( double *lower, double *upper, double *lev,
                                      double *obj_coef, _LONG_T number, int *col_flag );
    void cbc_print_column_updates( int *cols, double *lower, double *upper, double *obj_coef,
                                   int ncols, _LONG_T *col_flag );
    void cbc_print_deleted_columns( int ncols, int *cols );
    void cbc_print_retrieved_rows( double *row_lo, double *row_up, double *row_lev,
                                   _LONG_T number, int *row_flag );
    void cbc_print_row_updates( int *rows, double *row_lo, double *row_up, int nrows );
    void cbc_print_deleted_rows( int nrows, int *rows );
    void cbc_print_retrieved_matrix_elements( int *row, int *col, double *value, int upd, int nelem );
    void cbc_print_retrieved_SOS( int nsos, char *sostype, int *soslen, double *sospri,
                                  int *sosind, double *sosref );
    int  cbc_check_col_bounds( double *lower, double *upper, int ncols );
    int  cbc_check_row_bounds( double *row_lo, double *row_up, int nrows );
    int  cbc_check_matrix_elements( double *value, int nelem );
#endif

	void cbc_init_postsolve( void );
	void cbc_end_postsolve( void );
	int  cbc_write_mps( void );
	int  cbc_load_branching_priorities( void );
	int  cbc_load_SOS( _LONG_T *int_param );
	int  cbc_load_model( _LONG_T *int_param );
	int  cbc_update_model( void );
	int  cbc_mm_load_deleted_rows( _LONG_T *int_param );
	int  cbc_mm_load_new_rows( void );
	int  cbc_mm_load_changed_row_bounds( void );
	int  cbc_mm_load_deleted_columns( _LONG_T *int_param );
	int  cbc_mm_load_new_columns( int model_type_changed );
	int  cbc_mm_load_changed_column( int model_type_changed );
	int  cbc_mm_load_changed_obj_coef( void );
	int  cbc_mm_load_changed_matrix_elements( void );
	int  cbc_mm_change_model_direction( _LONG_T *int_param );
	int  cbc_mm_change_model_type( int mm_type );
	int  cbc_update_mm_model( _LONG_T *int_param );
	int  cbc_create_optimization_task( void );
	void cbc_init_solve( _LONG_T *int_param, double *dbl_param, int update, int type );
	int  cbc_get_model_type( _LONG_T *int_param );
	double cbc_calculate_objective( void );
	int  cbc_get_solution_info_lp( void );
	int  cbc_get_solution_info_mip( void );
	int  cbc_actually_call_solver( std::list<std::string>& opt_list );
	int  cbc_solve_model( std::list<std::string>& opt_list );
	int  cbc_call_solver( _LONG_T *int_param, double *dbl_param, int update, int type );
	void cbc_get_col_sol_during_callback( _LONG_T first_col, _LONG_T last_col, _LONG_T type_flag,
                                          double *lev, double *mar, _LONG_T *basic );

public:
	int  cbc_get_options( void );
    int  cbc_set_options( std::list<std::string>& opt_list );
private:
    void cbc_init_global_options( void );
    void cbc_init_options( void );
    
    int  cbc_load_mps_file( char *filename );
    int  cbc_solve_mps( _LONG_T int_param[], double dbl_param[], char *filename, _LONG_T create_sol_file );
};



#ifdef CBC_PARALLEL
// Mutex definitions.

#ifdef DEBUG
extern FILE *cbc_logfile;
#endif

class MutexAccess
{  
    cbc_handle_struct *cbc_current;
    int m_LineNr;
public:    

    MutexAccess( cbc_handle_struct *h )
    : cbc_current(h)
    {
        if ( cbc_current -> m_Mutex ) {
            cbc_current -> m_Mutex -> GetAccess();
        }
    }
#ifdef DEBUG
    MutexAccess( cbc_handle_struct *h, int lineNr )
    : cbc_current(h)
    , m_LineNr(lineNr)
    {
        if ( cbc_current -> m_Mutex ) {
            cbc_current -> m_Mutex -> GetAccess();
        }
//        fprintf( cbc_logfile, "Got Access on line %d\n", m_LineNr );
        
    }
#endif

    ~MutexAccess()
    {
#ifdef DEBUG
//        fprintf( cbc_logfile, "Releasing access Got on line %d\n", m_LineNr );
#endif
        if ( cbc_current -> m_Mutex ) {
            cbc_current -> m_Mutex -> ReleaseAccess();
        }
    }
};

#ifdef DEBUG
#define MUTEX_ACCESS MutexAccess ma( cbc_current, __LINE__ )
#else
#define MUTEX_ACCESS MutexAccess ma( cbc_current )
#endif
#endif // CBC_PARALLEL

#endif // _DEFINED_AIMMSOSI_SOLVERCBCIMPL_H

