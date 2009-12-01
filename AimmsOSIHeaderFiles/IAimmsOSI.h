// IAimmsOSI.h
//
// Copyright (C) 2001-2009  Paragon Decision Technology B.V.
// All Rights Reserved.
//
// This code is published under the Common Public License.
//
// Last Modified : August 28, 2009
//
// $Id$

#ifndef IAIMMSOSI_H_included
#define IAIMMSOSI_H_included

#ifdef WIN32
#include <tchar.h>
#else // WIN32
#define _TCHAR char
#define _T(x) x
#endif // WIN32

#ifdef LINUX64
    typedef signed int  _LONG_T ;
#else // LINUX64
    typedef signed long _LONG_T ;
#endif // LINUX64


class IAimmsSolverConnection;
class IAimmsMathProgramInfo;
class IAimmsMathProgramNonLinearEvaluator;
class IAimmsMathProgramCallback;
class IAimmsMathProgramMatrixManipulationInfo;
class ISolverMathProgramInstance;

#define AOSI_VERSION         20090414

#define AOSI_SUCCESS         0
#define AOSI_FAILURE         1


class IAimmsSolverConnection
{
public:
        enum MessagePriorityLevel {
        	PRIO_ALWAYS,
        	PRIO_ERROR,
        	PRIO_WARNING,
        	PRIO_REMARK,
        	PRIO_DETAIL
        };

        virtual void PassMessage(
                _LONG_T    priority,
                char       msg[]) = 0;
        virtual void PassMessageW(
                _LONG_T    priority,
                _TCHAR     msg[]) = 0;
        virtual void GetEnvironment(
                char      *name,
                char      *result,
                _LONG_T    len) = 0;
        virtual void GetLogDirName(
                char      *name,
                _LONG_T   *len) = 0;
        virtual void GetLogDirNameWchar(
                _TCHAR    *name,
                _LONG_T   *len) = 0;
                
        virtual class IAimmsExtendedSolverConnection *GetExtendedConnection() = 0;
};


class ISolverInfo
{
public:
        enum SolverCapability {
            CAPAB_SOS1_INT      = 0x00000001,
            CAPAB_SOS1_CONT     = 0x00000002,
            CAPAB_SOS2_INT      = 0x00000008,
            CAPAB_SOS2_CONT     = 0x00000010,
            CAPAB_FORT_CONV     = 0x00000040,
            CAPAB_IIS_FIND      = 0x00000080,
            CAPAB_RANGED_ROWS   = 0x00000100,
            CAPAB_OBJ_SENSE     = 0x00000200,
            CAPAB_RHS_SENSE     = 0x00000400,
            CAPAB_UPDATES       = 0x00000800,
            CAPAB_OBJ_SPARSE    = 0x00001000,
            CAPAB_SOLUTION      = 0x00002000,
            CAPAB_SEMI_CONT     = 0x00004000,
            CAPAB_SEMI_CONT_INT = 0x00008000,
            CAPAB_OBJ_ROW_REDUC = 0x00010000,
            CAPAB_MATRIX_MANIP  = 0x00020000,
            CAPAB_THREAD_SAFE   = 0x00040000,
            CAPAB_QP_NOT_SPEC   = 0x00080000,
            CAPAB_COMPL_BOUNDS  = 0x00100000,
            CAPAB_NO_EMPTY_JAC  = 0x00200000,
            CAPAB_PROGRESS_INF  = 0x00400000,
            CAPAB_EVAL_LIN_PART = 0x00800000,
            CAPAB_IP_CROSS_ITER = 0x01000000,
            CAPAB_USE_HESSIAN   = 0x02000000,
            CAPAB_CHG_QUAD_ROW  = 0x04000000,
            CAPAB_CALLBACK_CUTS = 0x08000000,
            CAPAB_POSTSOLVE     = 0x10000000,
            CAPAB_GLOBAL_OPT    = 0x20000000,
            CAPAB_MULTI_PROB    = 0x40000000,
            CAPAB_RENUMBER_DEL  = 0x80000000
        };
        
        enum SolverCapability2 {
            CAPAB2_NEW_OSI      = 0x00000001,
            CAPAB2_INDICATOR    = 0x00000002,
            CAPAB2_DIF_MIP_GAP  = 0x00000004,
            CAPAB2_PARALLEL     = 0x00000008,
            CAPAB2_RELOAD_QP    = 0x00000010,
            CAPAB2_NO_ASYNCHR   = 0x00000020,
            CAPAB2_PRESLV_INFO  = 0x00000040,
            CAPAB2_LAZY_ROWS    = 0x00000080,
            CAPAB2_CUT_POOL     = 0x00000100,
            CAPAB2_FILTERS      = 0x00000200,
            CAPAB2_EMPTY_FILE   = 0x00000400,
            CAPAB2_TUNING       = 0x00000800,
            CAPAB2_SOLVE_MPS    = 0x00001000,
            CAPAB2_CALC_SUBGR   = 0x00002000,
            CAPAB2_BRANCH_CB    = 0x00004000,
            CAPAB2_RELOAD_MM    = 0x00008000,
            CAPAB2_SEP_SPEC_ROW = 0x00010000,
            CAPAB2_PASS_THROUGH = 0x00020000
        };
        
        enum ComplementarityType {
        	COMPL_PATH,
        	COMPL_KNITRO
        };
        
        enum IntegerSolverInfo {
            IINFO_SIZE,
            IINFO_VERSION,
            IINFO_CAPAB_FLAGS,
            IINFO_NOPT_CAT,
            IINFO_NOPT_INT,
            IINFO_NOPT_DBL,
            IINFO_START_COL,
            IINFO_START_ROW,
            IINFO_PROGRESS,
            IINFO_COMPLEMENT,
            IINFO_CAPAB2_FLAGS,
            IINFO_MAX
        };
        
        enum DoubleSolverInfo {
            DINFO_SIZE,
            DINFO_ZERO_TOL,
            DINFO_INF,
            DINFO_MIN_INF,
            DINFO_INT_INF,
            DINFO_INT_MIN_INF,
            DINFO_SEMI_CONT_INF,
            DINFO_MAX
        };

        enum OptionValues {
            OPTION_LO,
            OPTION_DEFAULT,
            OPTION_UP,
            OPTION_CURRENT,
            OPTION_MAX
        };
        
        enum IntegerOptionInfo {
            OPTINFO_CAT,
            OPTINFO_INDEX,
            OPTINFO_CHANGE,
            OPTINFO_KEYWORDS,
            OPTINFO_MAX
        };

        virtual ~ISolverInfo() {}
        virtual int RegisterAsSolver(
                char       name[],
                _LONG_T   *model_flags) = 0;
        virtual _LONG_T GetInitialInfo(
                _LONG_T    info_int[],
                double     info_dbl[],
                char      *msg) = 0;
        virtual _LONG_T GetOptionsInfo(
                _LONG_T    info_int[],
                double     info_dbl[]) = 0;
        virtual void Shutdown(void) = 0;
        virtual void GetIntegerOption(
                _LONG_T    int_option_no,
                char       name[],
                _LONG_T    values[],
                _LONG_T    optinfo[]) = 0;
        virtual void GetIntegerKeyword(
                _LONG_T    int_option_no,
                _LONG_T    index,
                char       keyword[]) = 0;
        virtual void GetDoubleOption(
                _LONG_T    dbl_option_no,
                char       name[],
                double     values[],
                _LONG_T    optinfo[]) = 0;
        virtual void GetCategoryName(
                _LONG_T    category,
                char       name[]) = 0;
        virtual void GetVersion(
                char       version[],
                _LONG_T    size) = 0;
        virtual void GetHelpFile(
                char       filename[],
                _LONG_T    size) = 0; 
        virtual _LONG_T    GetLogFile(
                _TCHAR     filename[]) = 0;
                
        virtual class ISolverMathProgramInstance *CreateMathProgramInstance(
                class IAimmsMathProgramInfo *mp) = 0;
                
        virtual void Delete(void) = 0;
};

/* Special values */
#define AOSI_BASE_SPECIAL    1.33504431510432e-307

#define AOSI_INF_REP         AOSI_BASE_SPECIAL
#define AOSI_NA_REP          (2*AOSI_BASE_SPECIAL)
#define AOSI_UNDF_REP        (4 *AOSI_BASE_SPECIAL)
#define AOSI_ZERO_REP        (8 *AOSI_BASE_SPECIAL)
#define AOSI_MININF_REP      (16 *AOSI_BASE_SPECIAL)

#define AOSI_MAXINT          2147483647

class ISolverMathProgramInstance
{
public:
        enum ModelType {
            MODEL_LP               = 0x00000001,
            MODEL_MIP              = 0x00000002,
            MODEL_NLP              = 0x00000004,
            MODEL_QUADRATIC        = 0x00000008,
            MODEL_QUAD_INT         = 0x00000010,
            MODEL_BILIN_INT        = 0x00000020,
            MODEL_TRILINEAR        = 0x00000040,
            MODEL_MCP              = 0x00000080,
            MODEL_MINLP            = 0x00000100,
            MODEL_CONSTR_QUADRATIC = 0x00000200,
            MODEL_CONSTR_QUAD_INT  = 0x00000400,
            MODEL_MPCC             = 0x00000800
        };

        enum SolveType {
            MODEL_NEW = 1           ,
            MODEL_UPDATE            ,
            DUAL_MODEL_NEW          ,
            DUAL_MODEL_UPDATE       ,
            DUAL_MODEL_END          ,
            VAR_MODEL_NEW           ,
            VAR_MODEL_UPDATE        ,
            VAR_MODEL_END           ,
            POSTSOLVE_MODEL_NEW     ,
            POSTSOLVE_MODEL_UPDATE  ,
            POSTSOLVE_MODEL_END     
        };
        
        enum SolveDirection {
            DIRECTION_MIN = -1,
            DIRECTION_FEAS    ,
            DIRECTION_MAX
        };

        enum IntegerMathProgramInfo {
            IPARAM_SIZE,
            IPARAM_ROWS,
            IPARAM_NL_ROWS,
            IPARAM_UPD_R,
            IPARAM_SOS1_R,
            IPARAM_SOS2_R,
            IPARAM_COLS,
            IPARAM_NL_COLS,
            IPARAM_UPD_C,
            IPARAM_INT_C,
            IPARAM_SEMI_C,
            IPARAM_SEMI_INT_C,
            IPARAM_NONZEROS,
            IPARAM_L_NONZEROS,
            IPARAM_NL_NONZEROS,
            IPARAM_OBJ_NZ,
            IPARAM_UPD_NZ,
            IPARAM_UPD_L_NZ,
            IPARAM_UPD_NL_NZ,
            IPARAM_SOS1_NZ,
            IPARAM_SOS2_NZ,
            IPARAM_DIRECTION,
            IPARAM_BASIS,
            IPARAM_RANGED_R,
            IPARAM_EQUAL_R,
            IPARAM_MAX_ELEM_R,
            IPARAM_MAX_ELEM_C,
            IPARAM_QUAD_NZ,
            IPARAM_QUAD_UPD_NZ,
            IPARAM_QUAD_DIAGONAL,
            IPARAM_OBJ_COL_NO,
            IPARAM_RMIP,
            IPARAM_USE_MM,
            IPARAM_MM_ADD_C,
            IPARAM_MM_CHG_C,
            IPARAM_MM_CHG_TYP_C,
            IPARAM_MM_CHG_OBJ_C,
            IPARAM_MM_DEL_C,
            IPARAM_MM_ADD_R,
            IPARAM_MM_CHG_R,
            IPARAM_MM_DEL_R,
            IPARAM_MM_CHG_E,
            IPARAM_MM_CHG_QE,
            IPARAM_MM_FLAGS,
            IPARAM_MM_TYPE,
            IPARAM_NO_SOLVES,
            IPARAM_CALLBACK,
            IPARAM_UNAS_COMP_COL,
            IPARAM_NEW_COMP_COL,
            IPARAM_2D_DIR_DER,
            IPARAM_HESSIAN,
            IPARAM_HESSIAN_ELEM,
            IPARAM_QUAD_ROWS,
            IPARAM_MAX_ELEM_Q_R,
            IPARAM_BRANCH_CALLBACK,
            IPARAM_CUTS_CALLBACK,
            IPARAM_HEUR_CALLBACK,
            IPARAM_INCUMB_CALLBACK,
            IPARAM_OBJ_TYPE,
            IPARAM_COMPL_COUPLES,
            IPARAM_INDICATOR_ROWS,
            IPARAM_LAZY_ROWS,
            IPARAM_CUTPOOL_ROWS,
            IPARAM_FILTERS,
            IPARAM_MAX_ELEM_LAZY,
            IPARAM_MAX_ELEM_CUTPOOL,
            IPARAM_MAX_ELEM_FILTER,
            IPARAM_IS_ASYN_EXE,
            IPARAM_IS_MPI_SOLVE,
            IPARAM_IS_ENCRYPTED,
            IPARAM_IS_QUADRATIC,
            IPARAM_NR_MIP_STARTS,
            IPARAM_MAX
        };
        
        enum DoubleMathProgramInfo {
            DPARAM_SIZE,
            DPARAM_MARGINAL_ZERO,
            DPARAM_LOWEST_PRIORITY,
            DPARAM_OBJECTIVE_ZERO,
            DPARAM_OBJECTIVE_CONST,
            DPARAM_MAX
        };

        enum ModelStatus {
            MODELSTAT_NOT_SOLVED       ,
            MODELSTAT_OPTIMAL          ,
            MODELSTAT_LOCAL_OPT        ,
            MODELSTAT_UNBOUNDED        ,
            MODELSTAT_INFEASIBLE       ,
            MODELSTAT_LOCAL_INFEAS     ,
            MODELSTAT_INTER_INFEAS     ,
            MODELSTAT_INTER_NON_OPT    ,
            MODELSTAT_INT_SOLUTION     ,
            MODELSTAT_INTER_NON_INT    ,
            MODELSTAT_INT_INFEASIBLE   ,
            MODELSTAT_INF_OR_UNB       ,
            MODELSTAT_UNKNOWN_ERROR    ,
            MODELSTAT_NO_SOLUTION      
        };

        enum SolverStatus {
            SOLVERSTAT_NOT_CALLED      ,
            SOLVERSTAT_NORMAL_END      ,
            SOLVERSTAT_ITERATION_INT   ,
            SOLVERSTAT_RESOURCE_INT    ,
            SOLVERSTAT_TERMINATED      ,
            SOLVERSTAT_EVAL_ERROR      ,
            SOLVERSTAT_UNKNOWN         ,
            SOLVERSTAT_USER_INT        ,
            SOLVERSTAT_PREPROC_ERROR   ,
            SOLVERSTAT_SETUP_ERROR     ,
            SOLVERSTAT_FAILURE         ,
            SOLVERSTAT_INTERNAL_ERROR  ,
            SOLVERSTAT_POSTPROC_ERROR  ,
            SOLVERSTAT_SYSTEM_FAILURE  
        };
        
        enum IntegerMathProgramStatus {
            ISTAT_MODEL_STAT,
            ISTAT_SOLVER_STAT,
            ISTAT_TOTAL_ITER,
            ISTAT_NODES,
            ISTAT_MEM_USED,
            ISTAT_RESTART_OFF,
            ISTAT_POSTSOLVE_WARN,
            ISTAT_PRESOLVE_STAT,
            ISTAT_POSTSOLVE_CONT,
            ISTAT_POSTSOLVE_INT,
            ISTAT_MIP_CALC_SENS,
            ISTAT_MAX
        };
        
        enum DoubleMathProgramStatus{
            DSTAT_OBJ,
            DSTAT_LP_BEST,
            DSTAT_MAX
        };

        enum CallbackInfo {
            INFO_ITER,
            INFO_MAX
        };
        
        enum SolutionReturnType {
            RETURN_SOLUTION_LEV       = 0x00000001,
            RETURN_SOLUTION_MAR       = 0x00000002,
            RETURN_SOLUTION_BASIC     = 0x00000004,
            RETURN_SOLUTION_PRESOLVE  = 0x00000008
        };

        enum BasicStatus {
            BASIC_STATUS_NON_BASIC    ,
            BASIC_STATUS_BASIC        ,
            BASIC_STATUS_SUPERBASIC   ,
        };
        
        enum PresolveStatus {
            PRE_STATUS_IN_PRESOLVED = 10,
            PRE_STATUS_FIXED_LB         ,
            PRE_STATUS_FIXED_UB         ,
            PRE_STATUS_FIXED_OTHER      ,
            PRE_STATUS_AGGREGATED       ,
            PRE_STATUS_DELETED_OTHER    ,
            PRE_STATUS_REDUNDANT
        };
                
        enum SensitivityType {  
            RANGE_OBJ_COEF = 1,
            RANGE_RHS
        };
        
        enum TuningType {  
            TUNING_NONE      ,
            TUNING_FIXED     ,
            TUNING_TUNED
        };

protected:      
        virtual ~ISolverMathProgramInstance() {}
public:
        virtual void DoSolve(
                _LONG_T    type,
                _LONG_T    options,
                _LONG_T    int_param[],
                double     dbl_param[],
                size_t     int_stat[],
                double     dbl_stat[]) = 0;
        virtual _LONG_T SetCallbackMoments(
                _LONG_T    flags,
                _LONG_T    info[]) = 0;
        virtual void GetColumnSolution(
                _LONG_T    first_col,
                _LONG_T    last_col,
                _LONG_T    type_flag,
                double     lev[],
                double     mar[],
                _LONG_T    basic[]) = 0;
        virtual void GetRowSolution(
                _LONG_T    first_row,
                _LONG_T    last_row,
                _LONG_T    type_flag,
                double     lev[],
                double     mar[],
                _LONG_T    basic[]) = 0;
        virtual void FindIIS(
                _LONG_T   *no_rows,
                _LONG_T   *no_cols) = 0;
        virtual _LONG_T GetIIS(
                _LONG_T    no_rows,
                _LONG_T   *rownos,
                _LONG_T    no_cols,
                _LONG_T   *colnos) = 0;
        virtual _LONG_T GetSensitivityData(
                _LONG_T    first,
                _LONG_T    last,
                _LONG_T    type,
                double     lower[],
                double     upper[]) = 0;
        virtual void Reset(void) = 0;
        virtual void Delete(void) = 0;
        virtual void DoTuning(
                _LONG_T    options,
                _LONG_T    int_param[],
                double     dbl_param[],
                size_t     int_stat[],
                double     dbl_stat[],
                _LONG_T    nr_int_opt,
                _LONG_T    int_opt_stat[],
                _LONG_T    int_opt_tuned[],
                _LONG_T    nr_dbl_opt,
                _LONG_T    dbl_opt_stat[],
                double     dbl_opt_tuned[],
                _LONG_T    nr_files,
                _LONG_T    start_pos[],
                _LONG_T    filenames_len,
                char      *filenames) = 0;
        virtual void DoMpsSolve(
                _LONG_T    options,
                _LONG_T    int_param[],
                double     dbl_param[],
                size_t     int_stat[],
                double     dbl_stat[],
                _LONG_T    filename_len,
                _TCHAR    *filename,
                _LONG_T    create_sol_file) = 0;
        virtual _LONG_T CalculateSubgradient(  
                double     delta_rhs[],
                _LONG_T    delta_rhs_len,
                double     subgr[],
                _LONG_T    subgr_len) = 0;
};


class IAimmsMathProgramInfo
{
public:
        enum IntegerGeneralOption {
            IOPT_progress_solution     = -1,
            IOPT_iteration_limit       = -2,
            IOPT_time_limit            = -3,
            IOPT_after_integer_sol     = -4,
            IOPT_solver_messages       = -5,
            IOPT_max_domain_errors     = -6,
            IOPT_use_2D_order_der      = -7,
#ifdef DEBUG
            IOPT_solver_tracing        = -8,
            IOPT_MAX                   =  8
#else
            IOPT_MAX                   =  7
#endif
        };

        enum DoubleGeneralOption {
            DOPT_solver_workspace      = -1,
            DOPT_mip_relative_opt      = -2,
            DOPT_mip_absolute_opt      = -3,
            DOPT_mip_cutoff            = -4,
            DOPT_MAX                   =  4
        };
        
        enum MatrixOrder {
            ROW_WISE     = 0x00000001,
            COL_WISE     = 0x00000002,
            REREAD       = 0x00000004
        };

        enum ColumnRowFlag {
            FLAGS_IS_BASIC         = 0x00000001,
            FLAGS_IS_INTEGER       = 0x00000002,
            FLAGS_IS_CONTINUOUS    = 0x00000004,
            FLAGS_IS_SOS_TYPE_1    = 0x00000008,
            FLAGS_IS_SOS_TYPE_2    = 0x00000010,
            FLAGS_IS_SEMI_CONT     = 0x00000020,
            FLAGS_FIXED_COLUMN     = 0x00000040,
            FLAGS_IS_NONLINEAR     = 0x00000080,
            FLAGS_IS_QUADRATIC     = 0x00000100,
            FLAGS_IS_CONVEX        = 0x00000200,
            FLAGS_IS_RELAX_ONLY    = 0x00000400,
            FLAGS_IS_INDICATOR     = 0x00000800,
            FLAGS_IS_LAZY          = 0x00001000,
            FLAGS_IS_CUT_POOL      = 0x00002000,
            FLAGS_IS_FILTER        = 0x00004000,
            FLAGS_MM_MODIFIED_UB   = 0x00008000,
            FLAGS_MM_MODIFIED_LB   = 0x00010000,
            FLAGS_MM_MODIFIED_TYPE = 0x00020000
        };

        enum IdentifierName {
            NAME_COLUMN  ,
            NAME_ROW     ,
            NAME_MODEL   
        };
        
        virtual ~IAimmsMathProgramInfo() {}
                
        virtual void GetChangedOptions(
                _LONG_T   *no_int,
                _LONG_T    ind_int[],
                _LONG_T    opt_int[],
                _LONG_T   *no_dbl,
                _LONG_T    ind_dbl[],
                double     opt_dbl[]) = 0;
                
        virtual void SetMatrixOrder(
                _LONG_T    type_flag) = 0;
        virtual _LONG_T GetColumnData(
                _LONG_T    is_update,
                _LONG_T    max_no,
                _LONG_T    col_no[],
                double     lb[],
                double     lev[],
                double     ub[],
                double     obj_coeff[],
                double     prior[],
                _LONG_T    no_elm[],
                _LONG_T    no_q_elm[],
                _LONG_T    type_flag[]) = 0;
        virtual _LONG_T GetRowData(
                _LONG_T    is_update,
                _LONG_T    max_no,
                _LONG_T    row_no[],
                double     lb[],
                double     lev[],
                double     ub[],
                _LONG_T    no_elm[],
                double     sum_nl[],
                _LONG_T    type_flag[]) = 0;
        virtual _LONG_T GetSOSData(
                _LONG_T    nsos,
                char       sos_type[],
                _LONG_T    sos_set_no[],
                _LONG_T    sos_set_elem[],
                _LONG_T    sos_len[],
                double     sos_ref[],
                double     sos_prior[]) = 0;
        virtual _LONG_T GetMatrixData(
                _LONG_T    is_update,
                _LONG_T    max_no,
                _LONG_T    row_no[],
                _LONG_T    col_no[],
                double     val[],
                _LONG_T    nl_flag[]) = 0;
        virtual _LONG_T GetQmatrixData(
                _LONG_T    is_update,
                _LONG_T    max_no,
                _LONG_T    col_no1[],
                _LONG_T    col_no2[],
                double     val[]) = 0;
        virtual _LONG_T GetQuadraticRow(          
                _LONG_T   *row_no,
                double    *lb,
                double    *ub,
                _LONG_T    len,
                _LONG_T   *no_lin,
                _LONG_T    lin_no[],
                double     lin_val[],
                _LONG_T   *no_quad,
                _LONG_T    quad_no1[],
                _LONG_T    quad_no2[],
                double     quad_val[]) = 0;
        virtual _LONG_T GetIndicatorRow(
                _LONG_T   *row_no,
                double    *lb,
                double    *ub,
                _LONG_T    len,
                _LONG_T   *no_lin,
                _LONG_T    lin_no[],  
                double     lin_val[],
                _LONG_T   *ind_col_no,
                _LONG_T   *ind_val) = 0;
        virtual _LONG_T GetLazyRows(
                _LONG_T    maxnzcnt,
                _LONG_T   *nrows,
                double    *lb,
                double    *ub,
                _LONG_T   *rmatbeg,
                _LONG_T   *colind,
                double    *coef) = 0;
        virtual _LONG_T GetCutPoolRows(
                _LONG_T    maxnzcnt,
                _LONG_T   *nrows,
                double    *lb,
                double    *ub,
                _LONG_T   *rmatbeg,
                _LONG_T   *colind,
                double    *coef) = 0;
        virtual _LONG_T GetFilter(
                _LONG_T   *row_no,
                double    *lb,
                double    *ub,
                _LONG_T    len,
                _LONG_T   *nzcnt,
                _LONG_T    col_no[],
                double     val[],  
                double     refval[],
                _LONG_T   *is_range) = 0;
        virtual _LONG_T GetMIPStart(
                int        max_no,
                double     values[],
                int       *level) = 0;
        virtual _LONG_T GetComplementData(
                _LONG_T    max_no,
                _LONG_T    col_no[],
                _LONG_T    row_no[]) = 0;
        virtual void GetRowColumnName(
                _LONG_T    type,
                _LONG_T    number,
                char      *name,
                _LONG_T   *len) = 0;

        virtual void * AllocateMemory(
                _LONG_T    size) = 0;
        virtual void FreeMemory(
                void      *ptr) = 0;

        virtual _LONG_T IsNonlinearRow(
                _LONG_T    row) = 0;
        virtual _LONG_T IsQuadraticRow(
                int        number,
                double    *lb,
                double    *ub) = 0;
        virtual _LONG_T IsIndicatorRow(
                int        number,
                double    *lb,
                double    *ub) = 0;
        virtual _LONG_T IsLazyRow(
                _LONG_T    row) = 0;
        virtual _LONG_T IsCutPoolRow(
                _LONG_T    row) = 0;
        virtual _LONG_T IsFilter(
                _LONG_T    row) = 0;
        virtual _LONG_T GetModelType(void) = 0;
        virtual _LONG_T GetTime(void) = 0;
        virtual char * GetDoubleAsString(
                double     x) = 0;
        virtual int StoreKBestSolution(
                double    *lev,
                int        k) = 0;
        virtual _LONG_T GetNonlinearObjectiveColumns(
                _LONG_T    max_no,
                _LONG_T    col_no[]) = 0;

        virtual class IAimmsMathProgramMatrixManipulationInfo *GetMatrixManipulationInfo() = 0;
        virtual class IAimmsMathProgramNonLinearEvaluator *GetNonLinearEvaluator() = 0;
        virtual class IAimmsMathProgramCallback *GetSolverCallback() = 0;
        virtual class IAimmsExtendedMathProgramInfo *GetExtendedMathProgramInfo() = 0;
};


class IAimmsMathProgramNonLinearEvaluator
{
protected:
        virtual ~IAimmsMathProgramNonLinearEvaluator() {}
public:
        enum RowDefinitionProperty {
        	DEF_IS_GONOMETRIC = 0x00000001,
            DEF_HAS_ERRORF    = 0x00000002
        };
        
        virtual _LONG_T EvaluateRow(
                _LONG_T    row_no,
                double     point[],
                int        new_point,
                double    *val) = 0;
        virtual _LONG_T EvaluateDiffRow(
                _LONG_T    row_no,
                double     point[],
                int        new_point,
                double     jac[],
                double    *val) = 0;
        virtual _LONG_T GetSecondOrderDirectionalDerivatives(
                _LONG_T    row_no,
                int        nnonlin_cols,
                double     point[],
                int        new_point,
                double     dirs[],
                double     der2[]) = 0;
        virtual _LONG_T GetHessianSparsityPattern(
                int        nhess,
                int        rowno[],
                int        colno[]) = 0;
        virtual _LONG_T EvaluateHessian(
                double     point[],
                double     weights[],
                int        nhess,
                double     value[],
                double     obj_weight) = 0;
        virtual _LONG_T GetRowDefinitionString(
                int        rowno,
                char      *constr_def,
                int       *constr_def_length,
                int        obj_row_no,
                int        is_first,
                int        is_last,
                int       *flags) = 0;
};

#define AOSI_MAX_ERROR_LEN         256
#define AOSI_PROGRESS_TITLE_LEN    16
#define AOSI_PROGRESS_VALUE_LEN    64

class IAimmsMathProgramCallback
{
public:
        enum Phase {
            PHASE_UNKNOWN           = 0x00000000,
            PHASE_PRESOLVE          = 0x00000001,
            PHASE_SIMPLEX           = 0x00000002,
            PHASE_BARRIER           = 0x00000004,
            PHASE_NETWORK           = 0x00000008,
            PHASE_BARRIER_CROSSOVER = 0x00000010,
            PHASE_NETWORK_CROSSOVER = 0x00000020,
            PHASE_MIP               = 0x00000040,
            PHASE_POSTSOLVE         = 0x00000080,
            PHASE_MIP_POSTSOLVE     = 0x00000100,
            PHASE_SIFTING           = 0x00000200,
            PHASE_PRIMAL_SIMPLEX    = 0x00000400,
            PHASE_DUAL_SIMPLEX      = 0x00000800,
            PHASE_MIP_POPULATE      = 0x00001000,
            PHASE_TUNING            = 0x00002000,
            PHASE_IIS               = 0x00004000
        };

        enum SolutionType {
            SOL_TYPE_LP_INFEAS   ,
            SOL_TYPE_LP_FEAS     ,
            SOL_TYPE_LP_UNB      ,
            SOL_TYPE_MIP_INFEAS  ,
            SOL_TYPE_MIP_FEAS    ,
            SOL_TYPE_MIP_UNB     ,
            SOL_TYPE_NLP_INFEAS  ,
            SOL_TYPE_NLP_FEAS    ,
            SOL_TYPE_NLP_UNB
        };

        enum CallbackInfoFlag {
            CB_ITERATION    = 0x00000001,
            CB_STATE_CHANGE = 0x00000002,
            CB_INT_SOLUTION = 0x00000004,
            CB_ADD_CUT      = 0x00000008,
            CB_HEURISTIC    = 0x00000010,
            CB_INCUMBENT    = 0x00000020,
            CB_BRANCH       = 0x00000040
        };

        enum IntegerProgressInfo {
            PROG_TYPE,
            PROG_ITER,
            PROG_NODES_SOLVED,
            PROG_NODES_LEFT,
            PROG_END,
            PROG_MEM_USED,
            PROG_IP_CROSS_ITER,
            PROG_THREADS,
            PROG_MPS_TYPE,
            PROG_IMAX
        };
        
        enum DoubleProgressInfo {
            PROG_OBJ,
            PROG_OBJ_BEST,
            PROG_LP_BEST,
            PROG_SUM_INFEAS,
            PROG_TUNING,
            PROG_DMAX
        };

        enum IntegerCallbackInfo {
            CB_MODEL_STAT,
            CB_SOLVER_STAT,
            CB_ITER,
            CB_SOLVE_TIME,
            CB_NODE,
            CB_NR_NODES,
            CB_NR_BR_NODES,
            CB_IMAX
        };
        
        enum DoubleCallbackInfo {
            CB_OBJ,
            CB_OBJ_BEST,
            CB_LP_BEST,
            CB_NODE_OBJ,
            CB_DMAX
        };

        enum CallbackReturnStatus {
            RET_OK           ,
            RET_INTR         ,
            RET_HEUR_SOL     ,
            RET_REJECT_SOL   
        };

        enum InterruptReturnStatus {
            INTR_RET_OK      ,
            INTR_RET_INTR   
        };

protected:
        virtual ~IAimmsMathProgramCallback() {}
public:
        virtual void PassProgressInfo(
                size_t     prog_int[],
                double     prog_dbl[]) = 0;
        virtual void PassProgressLines(
                _LONG_T    no_lines,
                char      *titles,
                char      *values,
                size_t     mem_used) = 0;
        virtual void PassPhaseInfo(
                _LONG_T    phase) = 0;
        virtual _LONG_T ProcessEvents(void) = 0;
        virtual _LONG_T RunCallback(
                _LONG_T    flags,
                _LONG_T    prog_int[],
                double     prog_dbl[]) = 0;
                
        virtual _LONG_T GetMaximumCutElements(void) = 0;
        virtual _LONG_T GetNextCuts(
                int        max_no,
                int        max_elem_cut,
                double     lb[],
                double     ub[],
                int        nnonz[],
                int        col_no[],
                double     val[],
                int        is_local[],
                int        is_purgeable[]) = 0;
        virtual _LONG_T GetHeuristicSolution(
                int        max_no,
                double    *lev,
                double    *objval) = 0;
};

class IAimmsMathProgramMatrixManipulationInfo
{
public:
        enum MMFlag {
            MM_UB_CHANGED         = 0x000000001,
            MM_LB_CHANGED         = 0x000000002,
            MM_TYPE_CHANGED       = 0x000000004,
            MM_DIRECTION_CHANGED  = 0x000000008,
            MM_ROW_DEACTIVATED    = 0x000000010,
            MM_ROW_ACTIVATED      = 0x000000020,
            MM_ROW_TYPE_CHANGED   = 0x000000040,
            MM_ROW_TYPE_LE        = 0x000000080,
            MM_ROW_TYPE_GE        = 0x000000100,
            MM_ROW_TYPE_EQ        = 0x000000200,
            MM_ROW_TYPE_RANGED    = 0x000000400,
            MM_CHANGED_MODEL      = 0x000000800,
            MM_DIRECTION_WAS_NONE = 0x000001000,
            MM_OUTER_APPROX       = 0x000002000,
            MM_RELOAD_MODEL       = 0x000004000
        };
        
        enum MMType {
            MM_TYPE_LP = 1 ,
            MM_TYPE_LS     ,
            MM_TYPE_MIP    ,
            MM_TYPE_RMIP
        };
protected:
        virtual ~IAimmsMathProgramMatrixManipulationInfo() {}
public:
        virtual _LONG_T GetNextChangedRows(
                _LONG_T    max_no,
                _LONG_T    row_no[],
                double     lb[],
                double     ub[],
                _LONG_T    flag[]) = 0;
        virtual _LONG_T GetNextAddedRows(
                _LONG_T    max_no,
                _LONG_T    row_no[],
                double     lb[],
                double     ub[],
                _LONG_T    flag[]) = 0;
        virtual _LONG_T GetNextDeletedRows(
                _LONG_T    max_no,
                _LONG_T    row_no[]) = 0;
        virtual _LONG_T GetNextChangedMatrixElements(
                _LONG_T    max_no,
                _LONG_T    row_no[],
                _LONG_T    col_no[],
                double     val[]) = 0;
        virtual _LONG_T GetNextChangedQMatrixElements(
                _LONG_T    max_no,
                _LONG_T    col_no1[],
                _LONG_T    col_no2[],
                double     val[]) = 0;
        virtual _LONG_T GetNextChangedColumns(
                _LONG_T    max_no,
                _LONG_T    col_no[],
                double     ub[],
                double     lb[],
                _LONG_T    type_flag[]) = 0;
        virtual _LONG_T GetNextAddedColumns(
                _LONG_T    max_no,
                _LONG_T    col_no[],
                double     ub[],
                double     lb[],
                _LONG_T    type_flag[]) = 0;
        virtual _LONG_T GetNextDeletedColumns(
                _LONG_T    max_no, 
                _LONG_T    col_no[]) = 0;
                
        virtual void GetColumnType(
                int        colno,
                int       *flag) = 0;
        virtual void GetColumnBounds(
                int        colno,
                double    *lb,
                double    *ub) = 0;
};


#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif


#ifdef _MSC_VER
  #define __DLLEXPORT__ __declspec(dllexport)
  #define __DLLIMPORT__ __declspec(dllimport)
#else
  #ifdef GCC_HASCLASSVISIBILITY
    #define __DLLEXPORT__ __attribute__ ((visibility("default")))
    #define __DLLIMPORT__ __attribute__ ((visibility("default")))
  #else
    #define __DLLEXPORT__
    #define __DLLIMPORT__
  #endif
  #ifndef WINAPI
    #define WINAPI
  #endif
  #ifndef _stdcall
    #define _stdcall
  #endif
  #ifndef __cdecl
    #define __cdecl
  #endif
#endif

#ifdef __IS_AIMMS__
  #define AimmsOSI_API(x)      __DLLIMPORT__  x WINAPI
#else
  #define AimmsOSI_API(x)      __DLLEXPORT__  x WINAPI
#endif

extern "C" AimmsOSI_API(ISolverInfo *) GetSolverInfo(IAimmsSolverConnection *err);

#endif
