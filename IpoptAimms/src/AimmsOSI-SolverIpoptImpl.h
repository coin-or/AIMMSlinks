// Copyright (C) 2009 Paragon Decision Technology B.V. and others.
// All Rights Reserved.
// This code is published under the Common Public License.
//
// $Id$
//
// Authors: Yidong Lang, L.T. Biegler, Chuck Teeter, Marcel Hunting

#ifndef _DEFINED_AIMMSOSI_SOLVERIPOPTIMPL_H
#define _DEFINED_AIMMSOSI_SOLVERIPOPTIMPL_H

#include "IAimmsOSI.h"
#include "libIpopt.h"
#include "AimmsIpoptOptions.h"

class IpoptSolverInfo : public ISolverInfo
{
public:
	IpoptSolverInfo(IAimmsSolverConnection *gen) : m_gen(gen)
	          , ipopt_options_ordered(false) {}
protected:
        virtual ~IpoptSolverInfo() {}
public:
        virtual int RegisterAsSolver(
                char    name[],
                _LONG_T   *model_flags);
        virtual _LONG_T GetInitialInfo(
                _LONG_T    info_int[],
                double  info_dbl[],
                char   *msg);
        virtual _LONG_T GetOptionsInfo(
                _LONG_T    info_int[],
                double  info_dbl[]);
        virtual void Shutdown(void);
        virtual void GetIntegerOption(
                _LONG_T    int_option_no,
                char    name[],
                _LONG_T    values[],
                _LONG_T    optinfo[]);
        virtual void GetIntegerKeyword(
                _LONG_T    int_option_no,
                _LONG_T    index,
                char    keyword[]);
        virtual void GetDoubleOption(
                _LONG_T    dbl_option_no,
                char    name[],
                double  values[],
                _LONG_T    optinfo[]);
        virtual void GetCategoryName(
                _LONG_T    category,
                char    name[]);
        virtual void GetVersion(
                char    version[],
                _LONG_T    size);
        virtual void GetHelpFile(
                char    filename[],
                _LONG_T    size);
        virtual _LONG_T GetLogFile(
                _TCHAR  filename[]);
                
        virtual class ISolverMathProgramInstance *CreateMathProgramInstance(
        	class IAimmsMathProgramInfo *mp);
        	
        virtual void Delete(void);
private:
	IAimmsSolverConnection           *m_gen;
	
	bool ipopt_options_ordered;          // If 1, options have been ordered
	
	void IPOPT_order_options( void );
};


class IpoptMathProgramInstance : public ISolverMathProgramInstance
{
public:
	IpoptMathProgramInstance(class IAimmsMathProgramInfo *mp, IAimmsSolverConnection *gen);

protected:	
	virtual ~IpoptMathProgramInstance() {}
public:
        virtual void DoSolve(
                _LONG_T    type,
                _LONG_T    options,
                _LONG_T    int_param[],
                double  dbl_param[],
                size_t  int_stat[],
                double  dbl_stat[]);
        virtual _LONG_T SetCallbackMoments(
                _LONG_T    flags,
                _LONG_T    info[]);
        virtual void GetColumnSolution(
                _LONG_T    first_col,
                _LONG_T    last_col,
                _LONG_T    type_flag,
                double  lev[],
                double  mar[],
                _LONG_T    basic[]);
        virtual void GetRowSolution(
                _LONG_T    first_row,
                _LONG_T    last_row,
                _LONG_T    type_flag,
                double  lev[],
                double  mar[],
                _LONG_T    basic[]);
        virtual void FindIIS(
                _LONG_T   *no_rows,
                _LONG_T   *no_cols);
        virtual _LONG_T GetIIS(
                _LONG_T    no_rows,
                _LONG_T   *rownos,
                _LONG_T    no_cols,
                _LONG_T   *colnos);
        virtual _LONG_T GetSensitivityData(
                _LONG_T    first,
                _LONG_T    last,
                _LONG_T    type,
                double  lower[],
                double  upper[]);
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
	IAimmsSolverConnection                *m_gen;
	IAimmsMathProgramInfo                 *m_mp;
	IAimmsMathProgramNonLinearEvaluator   *m_evaluator;
	IAimmsMathProgramCallback             *m_callback;
	
	// Global variables.
	
public:
	IPOPT_handle_struct   IPOPT_handle;				 // This structure holds IPOPT problem data and is used to
	                                                 // transfer data to/from AIMMS
private:
	char    IPOPT_msg[256];							 // Buffer for building error/warning messages to be sent to
	                                                 // the AIMMS message window
	bool	IPOPT_mp_init;		                     // If 1, the mp has been initialized
	
	// Options related variables.
	
public:
	_LONG_T ipopt_opt_sol_progress;			         // Solution progress passed to AIMMS every n number
	                                                 // of iterations with n the value of this option
	_LONG_T ipopt_opt_dom_limit;					 // Maximum number of domain errors
private:
	_LONG_T ipopt_opt_priority;                      // Minimum priority of message passed through to AIMMS
	int     ipopt_opt_time_limit;                    // Max. duration time of a solve (in seconds)

    int     ipopt_int_opt_val[ IPOPT_OPT_INT_MAX ];  // Array containing the current values of the
                                                     // integer options
    double  ipopt_dbl_opt_val[ IPOPT_OPT_DBL_MAX ];  // Array containing the current values of the
                                                     // double options
    
    // Global variables for IPOPT-specific options.
    
	double  ipopt_opt_constr_viol_tol;				 // Value of IPOPT option 'constraint violation tolerance'
    
    // Debug related variables.
	
#ifdef DEBUG
	int     ipopt_opt_tracing;                       // If 1, write to 'ipoptXX.log'
#endif

public:	
	IAimmsMathProgramInfo * get_mp() { return m_mp; };
	IAimmsMathProgramNonLinearEvaluator * get_evaluator() { return m_evaluator; };
	IAimmsMathProgramCallback * get_callback() { return m_callback; };

	void IPOPT_Error( const char* errmsg );
	void IPOPT_Progress_Info( bool end );
	void IPOPT_Get_Solution_Status( int status, double *x_sol, double *g_sol );
	int  IPOPT_Is_Feasible_Solution( double* x, double* rowlev );

private:
	void IPOPT_Init_Handle( void );
	int  IPOPT_Solve_Model( _LONG_T* int_param, double* dbl_param, int* mem_used );
	int  IPOPT_Initialize_Solve_Parameters( _LONG_T* int_param, double* dbl_param );
	int  IPOPT_Get_Model( void );
	void IPOPT_Get_Objective_Data( _LONG_T *col_flag );
	void IPOPT_Get_Jacobian_Map( _LONG_T *rows, _LONG_T *cols, double *coef, _LONG_T *nlflag,
	                             _LONG_T *row_flag );
	
    void IPOPT_Init_Global_Options( void );
    void IPOPT_Init_Options( void );
    void IPOPT_Get_Options( void );
    int  IPOPT_Set_Options( Ipopt::SmartPtr<Ipopt::IpoptApplication>& app );
    
#ifdef DEBUG
	void print_parameters( FILE *f, _LONG_T options, _LONG_T *int_param, double *dbl_param );
	void print_columns( int ncols, _LONG_T number, double *lb, double *lev, double *ub );
	void print_objective( int ncols, double *obj_coef, int *obj_flag );
	void print_rows( int nrows, _LONG_T number, double *row_lb, double *row_ub, double *fv );
	void print_elem( int nelem, _LONG_T *cols, _LONG_T *rows, double *coef, _LONG_T *nlflag );
	void print_solution_stats( void );
#endif
};

#endif // _DEFINED_AIMMSOSI_SOLVERIPOPTIMPL_H

