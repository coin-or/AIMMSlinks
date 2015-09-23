// aimmsosi_compat.h
//
// Copyright (C) 2001-2015  AIMMS B.V.
// All Rights Reserved.
//
// This code is published under the Eclipse Public License.
//
// Last Modified : March 10, 2015
//
// $Id$

#include "IAimmsOSI.h"

#define AOSI_PRIO_ALWAYS    IAimmsSolverConnection::PRIO_ALWAYS 
#define AOSI_PRIO_ERROR     IAimmsSolverConnection::PRIO_ERROR  
#define AOSI_PRIO_WARNING   IAimmsSolverConnection::PRIO_WARNING
#define AOSI_PRIO_REMARK    IAimmsSolverConnection::PRIO_REMARK 
#define AOSI_PRIO_DETAIL    IAimmsSolverConnection::PRIO_DETAIL 

#define AOSI_CAPAB_SOS1_INT        ISolverInfo::CAPAB_SOS1_INT     
#define AOSI_CAPAB_SOS1_CONT       ISolverInfo::CAPAB_SOS1_CONT        
#define AOSI_CAPAB_SOS2_INT        ISolverInfo::CAPAB_SOS2_INT     
#define AOSI_CAPAB_SOS2_CONT       ISolverInfo::CAPAB_SOS2_CONT     
#define AOSI_CAPAB_FORT_CONV       ISolverInfo::CAPAB_FORT_CONV    
#define AOSI_CAPAB_IIS_FIND        ISolverInfo::CAPAB_IIS_FIND     
#define AOSI_CAPAB_RANGED_ROWS     ISolverInfo::CAPAB_RANGED_ROWS  
#define AOSI_CAPAB_OBJ_SENSE       ISolverInfo::CAPAB_OBJ_SENSE    
#define AOSI_CAPAB_RHS_SENSE       ISolverInfo::CAPAB_RHS_SENSE    
#define AOSI_CAPAB_UPDATES         ISolverInfo::CAPAB_UPDATES      
#define AOSI_CAPAB_OBJ_SPARSE      ISolverInfo::CAPAB_OBJ_SPARSE   
#define AOSI_CAPAB_SOLUTION        ISolverInfo::CAPAB_SOLUTION     
#define AOSI_CAPAB_SEMI_CONT       ISolverInfo::CAPAB_SEMI_CONT    
#define AOSI_CAPAB_SEMI_CONT_INT   ISolverInfo::CAPAB_SEMI_CONT_INT
#define AOSI_CAPAB_OBJ_ROW_REDUC   ISolverInfo::CAPAB_OBJ_ROW_REDUC
#define AOSI_CAPAB_MATRIX_MANIP    ISolverInfo::CAPAB_MATRIX_MANIP 
#define AOSI_CAPAB_THREAD_SAFE     ISolverInfo::CAPAB_THREAD_SAFE  
#define AOSI_CAPAB_QP_NOT_SPEC     ISolverInfo::CAPAB_QP_NOT_SPEC  
#define AOSI_CAPAB_COMPL_BOUNDS    ISolverInfo::CAPAB_COMPL_BOUNDS 
#define AOSI_CAPAB_NO_EMPTY_JAC    ISolverInfo::CAPAB_NO_EMPTY_JAC 
#define AOSI_CAPAB_PROGRESS_INF    ISolverInfo::CAPAB_PROGRESS_INF 
#define AOSI_CAPAB_EVAL_LIN_PART   ISolverInfo::CAPAB_EVAL_LIN_PART
#define AOSI_CAPAB_IP_CROSS_ITER   ISolverInfo::CAPAB_IP_CROSS_ITER
#define AOSI_CAPAB_USE_HESSIAN     ISolverInfo::CAPAB_USE_HESSIAN  
#define AOSI_CAPAB_CHG_QUAD_ROW    ISolverInfo::CAPAB_CHG_QUAD_ROW 
#define AOSI_CAPAB_CALLBACK_CUTS   ISolverInfo::CAPAB_CALLBACK_CUTS
#define AOSI_CAPAB_POSTSOLVE       ISolverInfo::CAPAB_POSTSOLVE    
#define AOSI_CAPAB_GLOBAL_OPT      ISolverInfo::CAPAB_GLOBAL_OPT   
#define AOSI_CAPAB_MULTI_PROB      ISolverInfo::CAPAB_MULTI_PROB   
#define AOSI_CAPAB_RENUMBER_DEL    ISolverInfo::CAPAB_RENUMBER_DEL 

#define AOSI_CAPAB2_NEW_OSI        ISolverInfo::CAPAB2_NEW_OSI     
#define AOSI_CAPAB2_INDICATOR      ISolverInfo::CAPAB2_INDICATOR   
#define AOSI_CAPAB2_DIF_MIP_GAP    ISolverInfo::CAPAB2_DIF_MIP_GAP 
#define AOSI_CAPAB2_PARALLEL       ISolverInfo::CAPAB2_PARALLEL    
#define AOSI_CAPAB2_RELOAD_QP      ISolverInfo::CAPAB2_RELOAD_QP   
#define AOSI_CAPAB2_NO_ASYNCHR     ISolverInfo::CAPAB2_NO_ASYNCHR  
#define AOSI_CAPAB2_PRESLV_INFO    ISolverInfo::CAPAB2_PRESLV_INFO 
#define AOSI_CAPAB2_LAZY_ROWS      ISolverInfo::CAPAB2_LAZY_ROWS   
#define AOSI_CAPAB2_CUT_POOL       ISolverInfo::CAPAB2_CUT_POOL    
#define AOSI_CAPAB2_FILTERS        ISolverInfo::CAPAB2_FILTERS     
#define AOSI_CAPAB2_EMPTY_FILE     ISolverInfo::CAPAB2_EMPTY_FILE  
#define AOSI_CAPAB2_TUNING         ISolverInfo::CAPAB2_TUNING      
#define AOSI_CAPAB2_SOLVE_MPS      ISolverInfo::CAPAB2_SOLVE_MPS   
#define AOSI_CAPAB2_CALC_SUBGR     ISolverInfo::CAPAB2_CALC_SUBGR  
#define AOSI_CAPAB2_BRANCH_CB      ISolverInfo::CAPAB2_BRANCH_CB   
#define AOSI_CAPAB2_RELOAD_MM      ISolverInfo::CAPAB2_RELOAD_MM   
#define AOSI_CAPAB2_SEP_SPEC_ROW   ISolverInfo::CAPAB2_SEP_SPEC_ROW
#define AOSI_CAPAB2_PASS_THROUGH   ISolverInfo::CAPAB2_PASS_THROUGH
#define AOSI_CAPAB2_CALLBACK_LAZY  ISolverInfo::CAPAB2_CALLBACK_LAZY
#define AOSI_CAPAB2_RUN_SEP_THR    ISolverInfo::CAPAB2_RUN_SEP_THR
#define AOSI_CAPAB2_CP_FLOAT_EXP   ISolverInfo::CAPAB2_CP_FLOAT_EXP
#define AOSI_CAPAB2_CP_ACTIVITY    ISolverInfo::CAPAB2_CP_ACTIVITY
#define AOSI_CAPAB2_CP_FORB_ASNG   ISolverInfo::CAPAB2_CP_FORB_ASNG
#define AOSI_CAPAB2_CON_VAR_PRIO   ISolverInfo::CAPAB2_CON_VAR_PRIO
#define AOSI_CAPAB2_QP_AS_MIQP     ISolverInfo::CAPAB2_QP_AS_MIQP

#define AOSI_COMPL_PATH            ISolverInfo::COMPL_PATH
#define AOSI_COMPL_KNITRO          ISolverInfo::COMPL_KNITRO

#define    AOSI_IINFO_SIZE                      ISolverInfo::IINFO_SIZE          
#define    AOSI_IINFO_VERSION                   ISolverInfo::IINFO_VERSION       
#define    AOSI_IINFO_CAPAB_FLAGS               ISolverInfo::IINFO_CAPAB_FLAGS
#define    AOSI_IINFO_NOPT_CAT                  ISolverInfo::IINFO_NOPT_CAT      
#define    AOSI_IINFO_NOPT_INT                  ISolverInfo::IINFO_NOPT_INT      
#define    AOSI_IINFO_NOPT_DBL                  ISolverInfo::IINFO_NOPT_DBL      
#define    AOSI_IINFO_START_COL                 ISolverInfo::IINFO_START_COL     
#define    AOSI_IINFO_START_ROW                 ISolverInfo::IINFO_START_ROW          
#define    AOSI_IINFO_PROGRESS                  ISolverInfo::IINFO_PROGRESS
#define    AOSI_IINFO_COMPLEMENT                ISolverInfo::IINFO_COMPLEMENT  
#define    AOSI_IINFO_CAPAB2_FLAGS              ISolverInfo::IINFO_CAPAB2_FLAGS
#define    AOSI_IINFO_MAX                       ISolverInfo::IINFO_MAX           
   
#define    AOSI_DINFO_SIZE                      ISolverInfo::DINFO_SIZE          
#define    AOSI_DINFO_ZERO_TOL                  ISolverInfo::DINFO_ZERO_TOL      
#define    AOSI_DINFO_INF                       ISolverInfo::DINFO_INF           
#define    AOSI_DINFO_MIN_INF                   ISolverInfo::DINFO_MIN_INF       
#define    AOSI_DINFO_INT_INF                   ISolverInfo::DINFO_INT_INF       
#define    AOSI_DINFO_INT_MIN_INF               ISolverInfo::DINFO_INT_MIN_INF   
#define    AOSI_DINFO_SEMI_CONT_INF             ISolverInfo::DINFO_SEMI_CONT_INF 
#define    AOSI_DINFO_MAX                       ISolverInfo::DINFO_MAX           
   
#define    AOSI_OPTION_LO                       ISolverInfo::OPTION_LO       
#define    AOSI_OPTION_DEFAULT                  ISolverInfo::OPTION_DEFAULT  
#define    AOSI_OPTION_UP                       ISolverInfo::OPTION_UP
#define    AOSI_OPTION_CURRENT                  ISolverInfo::OPTION_CURRENT
#define    AOSI_OPTION_MAX                      ISolverInfo::OPTION_MAX      
   
#define    AOSI_OPTINFO_CAT                     ISolverInfo::OPTINFO_CAT     
#define    AOSI_OPTINFO_INDEX                   ISolverInfo::OPTINFO_INDEX   
#define    AOSI_OPTINFO_CHANGE                  ISolverInfo::OPTINFO_CHANGE  
#define    AOSI_OPTINFO_KEYWORDS                ISolverInfo::OPTINFO_KEYWORDS
#define    AOSI_OPTINFO_MAX                     ISolverInfo::OPTINFO_MAX     

#define AOSI_MODEL_LP                   ISolverMathProgramInstance::MODEL_LP              
#define AOSI_MODEL_MIP                  ISolverMathProgramInstance::MODEL_MIP             
#define AOSI_MODEL_NLP                  ISolverMathProgramInstance::MODEL_NLP             
#define AOSI_MODEL_QUADRATIC            ISolverMathProgramInstance::MODEL_QUADRATIC       
#define AOSI_MODEL_QUAD_INT             ISolverMathProgramInstance::MODEL_QUAD_INT        
#define AOSI_MODEL_BILIN_INT            ISolverMathProgramInstance::MODEL_BILIN_INT       
#define AOSI_MODEL_TRILINEAR            ISolverMathProgramInstance::MODEL_TRILINEAR       
#define AOSI_MODEL_MCP                  ISolverMathProgramInstance::MODEL_MCP             
#define AOSI_MODEL_MINLP                ISolverMathProgramInstance::MODEL_MINLP           
#define AOSI_MODEL_CONSTR_QUADRATIC     ISolverMathProgramInstance::MODEL_CONSTR_QUADRATIC
#define AOSI_MODEL_CONSTR_QUAD_INT      ISolverMathProgramInstance::MODEL_CONSTR_QUAD_INT 
#define AOSI_MODEL_MPCC                 ISolverMathProgramInstance::MODEL_MPCC            
#define AOSI_MODEL_COP                  ISolverMathProgramInstance::MODEL_COP

#define AOSI_MODEL_NEW                          ISolverMathProgramInstance::MODEL_NEW                
#define AOSI_MODEL_UPDATE                       ISolverMathProgramInstance::MODEL_UPDATE             
#define AOSI_DUAL_MODEL_NEW                     ISolverMathProgramInstance::DUAL_MODEL_NEW           
#define AOSI_DUAL_MODEL_UPDATE                  ISolverMathProgramInstance::DUAL_MODEL_UPDATE        
#define AOSI_DUAL_MODEL_END                     ISolverMathProgramInstance::DUAL_MODEL_END           
#define AOSI_VAR_MODEL_NEW                      ISolverMathProgramInstance::VAR_MODEL_NEW            
#define AOSI_VAR_MODEL_UPDATE                   ISolverMathProgramInstance::VAR_MODEL_UPDATE         
#define AOSI_VAR_MODEL_END                      ISolverMathProgramInstance::VAR_MODEL_END            
#define AOSI_POSTSOLVE_MODEL_NEW                ISolverMathProgramInstance::POSTSOLVE_MODEL_NEW      
#define AOSI_POSTSOLVE_MODEL_UPDATE             ISolverMathProgramInstance::POSTSOLVE_MODEL_UPDATE   
#define AOSI_POSTSOLVE_MODEL_END                ISolverMathProgramInstance::POSTSOLVE_MODEL_END      

#define AOSI_DIRECTION_MIN                      ISolverMathProgramInstance::DIRECTION_MIN 
#define AOSI_DIRECTION_FEAS                     ISolverMathProgramInstance::DIRECTION_FEAS
#define AOSI_DIRECTION_MAX                      ISolverMathProgramInstance::DIRECTION_MAX 

#define AOSI_MODELSTAT_NOT_SOLVED               ISolverMathProgramInstance::MODELSTAT_NOT_SOLVED     
#define AOSI_MODELSTAT_OPTIMAL                  ISolverMathProgramInstance::MODELSTAT_OPTIMAL        
#define AOSI_MODELSTAT_LOCAL_OPT                ISolverMathProgramInstance::MODELSTAT_LOCAL_OPT      
#define AOSI_MODELSTAT_UNBOUNDED                ISolverMathProgramInstance::MODELSTAT_UNBOUNDED      
#define AOSI_MODELSTAT_INFEASIBLE               ISolverMathProgramInstance::MODELSTAT_INFEASIBLE     
#define AOSI_MODELSTAT_LOCAL_INFEAS             ISolverMathProgramInstance::MODELSTAT_LOCAL_INFEAS   
#define AOSI_MODELSTAT_INTER_INFEAS             ISolverMathProgramInstance::MODELSTAT_INTER_INFEAS   
#define AOSI_MODELSTAT_INTER_NON_OPT            ISolverMathProgramInstance::MODELSTAT_INTER_NON_OPT  
#define AOSI_MODELSTAT_INT_SOLUTION             ISolverMathProgramInstance::MODELSTAT_INT_SOLUTION   
#define AOSI_MODELSTAT_INTER_NON_INT            ISolverMathProgramInstance::MODELSTAT_INTER_NON_INT  
#define AOSI_MODELSTAT_INT_INFEASIBLE           ISolverMathProgramInstance::MODELSTAT_INT_INFEASIBLE 
#define AOSI_MODELSTAT_INF_OR_UNB               ISolverMathProgramInstance::MODELSTAT_INF_OR_UNB
#define AOSI_MODELSTAT_UNKNOWN_ERROR            ISolverMathProgramInstance::MODELSTAT_UNKNOWN_ERROR  
#define AOSI_MODELSTAT_NO_SOLUTION              ISolverMathProgramInstance::MODELSTAT_NO_SOLUTION    

#define AOSI_SOLVERSTAT_NOT_CALLED              ISolverMathProgramInstance::SOLVERSTAT_NOT_CALLED    
#define AOSI_SOLVERSTAT_NORMAL_END              ISolverMathProgramInstance::SOLVERSTAT_NORMAL_END    
#define AOSI_SOLVERSTAT_ITERATION_INT           ISolverMathProgramInstance::SOLVERSTAT_ITERATION_INT 
#define AOSI_SOLVERSTAT_RESOURCE_INT            ISolverMathProgramInstance::SOLVERSTAT_RESOURCE_INT  
#define AOSI_SOLVERSTAT_TERMINATED              ISolverMathProgramInstance::SOLVERSTAT_TERMINATED    
#define AOSI_SOLVERSTAT_EVAL_ERROR              ISolverMathProgramInstance::SOLVERSTAT_EVAL_ERROR    
#define AOSI_SOLVERSTAT_UNKNOWN                 ISolverMathProgramInstance::SOLVERSTAT_UNKNOWN       
#define AOSI_SOLVERSTAT_USER_INT                ISolverMathProgramInstance::SOLVERSTAT_USER_INT      
#define AOSI_SOLVERSTAT_PREPROC_ERROR           ISolverMathProgramInstance::SOLVERSTAT_PREPROC_ERROR 
#define AOSI_SOLVERSTAT_SETUP_ERROR             ISolverMathProgramInstance::SOLVERSTAT_SETUP_ERROR   
#define AOSI_SOLVERSTAT_FAILURE                 ISolverMathProgramInstance::SOLVERSTAT_FAILURE       
#define AOSI_SOLVERSTAT_INTERNAL_ERROR          ISolverMathProgramInstance::SOLVERSTAT_INTERNAL_ERROR
#define AOSI_SOLVERSTAT_POSTPROC_ERROR          ISolverMathProgramInstance::SOLVERSTAT_POSTPROC_ERROR
#define AOSI_SOLVERSTAT_SYSTEM_FAILURE          ISolverMathProgramInstance::SOLVERSTAT_SYSTEM_FAILURE

#define    AOSI_IPARAM_SIZE                     ISolverMathProgramInstance::IPARAM_SIZE            
#define    AOSI_IPARAM_ROWS                     ISolverMathProgramInstance::IPARAM_ROWS            
#define    AOSI_IPARAM_NL_ROWS                  ISolverMathProgramInstance::IPARAM_NL_ROWS         
#define    AOSI_IPARAM_UPD_R                    ISolverMathProgramInstance::IPARAM_UPD_R           
#define    AOSI_IPARAM_SOS1_R                   ISolverMathProgramInstance::IPARAM_SOS1_R          
#define    AOSI_IPARAM_SOS2_R                   ISolverMathProgramInstance::IPARAM_SOS2_R          
#define    AOSI_IPARAM_COLS                     ISolverMathProgramInstance::IPARAM_COLS            
#define    AOSI_IPARAM_NL_COLS                  ISolverMathProgramInstance::IPARAM_NL_COLS         
#define    AOSI_IPARAM_UPD_C                    ISolverMathProgramInstance::IPARAM_UPD_C           
#define    AOSI_IPARAM_INT_C                    ISolverMathProgramInstance::IPARAM_INT_C           
#define    AOSI_IPARAM_SEMI_C                   ISolverMathProgramInstance::IPARAM_SEMI_C          
#define    AOSI_IPARAM_SEMI_INT_C               ISolverMathProgramInstance::IPARAM_SEMI_INT_C      
#define    AOSI_IPARAM_NONZEROS                 ISolverMathProgramInstance::IPARAM_NONZEROS        
#define    AOSI_IPARAM_L_NONZEROS               ISolverMathProgramInstance::IPARAM_L_NONZEROS      
#define    AOSI_IPARAM_NL_NONZEROS              ISolverMathProgramInstance::IPARAM_NL_NONZEROS     
#define    AOSI_IPARAM_OBJ_NZ                   ISolverMathProgramInstance::IPARAM_OBJ_NZ          
#define    AOSI_IPARAM_UPD_NZ                   ISolverMathProgramInstance::IPARAM_UPD_NZ          
#define    AOSI_IPARAM_UPD_L_NZ                 ISolverMathProgramInstance::IPARAM_UPD_L_NZ        
#define    AOSI_IPARAM_UPD_NL_NZ                ISolverMathProgramInstance::IPARAM_UPD_NL_NZ       
#define    AOSI_IPARAM_SOS1_NZ                  ISolverMathProgramInstance::IPARAM_SOS1_NZ         
#define    AOSI_IPARAM_SOS2_NZ                  ISolverMathProgramInstance::IPARAM_SOS2_NZ         
#define    AOSI_IPARAM_DIRECTION                ISolverMathProgramInstance::IPARAM_DIRECTION       
#define    AOSI_IPARAM_BASIS                    ISolverMathProgramInstance::IPARAM_BASIS           
#define    AOSI_IPARAM_RANGED_R                 ISolverMathProgramInstance::IPARAM_RANGED_R        
#define    AOSI_IPARAM_EQUAL_R                  ISolverMathProgramInstance::IPARAM_EQUAL_R         
#define    AOSI_IPARAM_MAX_ELEM_R               ISolverMathProgramInstance::IPARAM_MAX_ELEM_R      
#define    AOSI_IPARAM_MAX_ELEM_C               ISolverMathProgramInstance::IPARAM_MAX_ELEM_C      
#define    AOSI_IPARAM_QUAD_NZ                  ISolverMathProgramInstance::IPARAM_QUAD_NZ         
#define    AOSI_IPARAM_QUAD_UPD_NZ              ISolverMathProgramInstance::IPARAM_QUAD_UPD_NZ     
#define    AOSI_IPARAM_QUAD_DIAGONAL            ISolverMathProgramInstance::IPARAM_QUAD_DIAGONAL   
#define    AOSI_IPARAM_OBJ_COL_NO               ISolverMathProgramInstance::IPARAM_OBJ_COL_NO      
#define    AOSI_IPARAM_RMIP                     ISolverMathProgramInstance::IPARAM_RMIP            
#define    AOSI_IPARAM_USE_MM                   ISolverMathProgramInstance::IPARAM_USE_MM          
#define    AOSI_IPARAM_MM_ADD_C                 ISolverMathProgramInstance::IPARAM_MM_ADD_C        
#define    AOSI_IPARAM_MM_CHG_C                 ISolverMathProgramInstance::IPARAM_MM_CHG_C        
#define    AOSI_IPARAM_MM_CHG_TYP_C             ISolverMathProgramInstance::IPARAM_MM_CHG_TYP_C    
#define    AOSI_IPARAM_MM_CHG_OBJ_C             ISolverMathProgramInstance::IPARAM_MM_CHG_OBJ_C    
#define    AOSI_IPARAM_MM_DEL_C                 ISolverMathProgramInstance::IPARAM_MM_DEL_C
#define    AOSI_IPARAM_MM_ADD_R                 ISolverMathProgramInstance::IPARAM_MM_ADD_R        
#define    AOSI_IPARAM_MM_CHG_R                 ISolverMathProgramInstance::IPARAM_MM_CHG_R        
#define    AOSI_IPARAM_MM_DEL_R                 ISolverMathProgramInstance::IPARAM_MM_DEL_R    
#define    AOSI_IPARAM_MM_CHG_E                 ISolverMathProgramInstance::IPARAM_MM_CHG_E        
#define    AOSI_IPARAM_MM_CHG_QE                ISolverMathProgramInstance::IPARAM_MM_CHG_QE       
#define    AOSI_IPARAM_MM_FLAGS                 ISolverMathProgramInstance::IPARAM_MM_FLAGS        
#define    AOSI_IPARAM_MM_TYPE                  ISolverMathProgramInstance::IPARAM_MM_TYPE         
#define    AOSI_IPARAM_NO_SOLVES                ISolverMathProgramInstance::IPARAM_NO_SOLVES       
#define    AOSI_IPARAM_CALLBACK                 ISolverMathProgramInstance::IPARAM_CALLBACK        
#define    AOSI_IPARAM_UNAS_COMP_COL            ISolverMathProgramInstance::IPARAM_UNAS_COMP_COL   
#define    AOSI_IPARAM_NEW_COMP_COL             ISolverMathProgramInstance::IPARAM_NEW_COMP_COL    
#define    AOSI_IPARAM_2D_DIR_DER               ISolverMathProgramInstance::IPARAM_2D_DIR_DER      
#define    AOSI_IPARAM_HESSIAN                  ISolverMathProgramInstance::IPARAM_HESSIAN         
#define    AOSI_IPARAM_HESSIAN_ELEM             ISolverMathProgramInstance::IPARAM_HESSIAN_ELEM    
#define    AOSI_IPARAM_QUAD_ROWS                ISolverMathProgramInstance::IPARAM_QUAD_ROWS       
#define    AOSI_IPARAM_MAX_ELEM_Q_R             ISolverMathProgramInstance::IPARAM_MAX_ELEM_Q_R    
#define    AOSI_IPARAM_BRANCH_CALLBACK          ISolverMathProgramInstance::IPARAM_BRANCH_CALLBACK  
#define    AOSI_IPARAM_CUTS_CALLBACK            ISolverMathProgramInstance::IPARAM_CUTS_CALLBACK   
#define    AOSI_IPARAM_HEUR_CALLBACK            ISolverMathProgramInstance::IPARAM_HEUR_CALLBACK   
#define    AOSI_IPARAM_INCUMB_CALLBACK          ISolverMathProgramInstance::IPARAM_INCUMB_CALLBACK 
#define    AOSI_IPARAM_OBJ_TYPE                 ISolverMathProgramInstance::IPARAM_OBJ_TYPE
#define    AOSI_IPARAM_COMPL_COUPLES            ISolverMathProgramInstance::IPARAM_COMPL_COUPLES
#define    AOSI_IPARAM_INDICATOR_ROWS           ISolverMathProgramInstance::IPARAM_INDICATOR_ROWS
#define    AOSI_IPARAM_LAZY_ROWS                ISolverMathProgramInstance::IPARAM_LAZY_ROWS
#define    AOSI_IPARAM_CUTPOOL_ROWS             ISolverMathProgramInstance::IPARAM_CUTPOOL_ROWS
#define    AOSI_IPARAM_FILTERS                  ISolverMathProgramInstance::IPARAM_FILTERS
#define    AOSI_IPARAM_MAX_ELEM_LAZY            ISolverMathProgramInstance::IPARAM_MAX_ELEM_LAZY
#define    AOSI_IPARAM_MAX_ELEM_CUTPOOL         ISolverMathProgramInstance::IPARAM_MAX_ELEM_CUTPOOL
#define    AOSI_IPARAM_MAX_ELEM_FILTER          ISolverMathProgramInstance::IPARAM_MAX_ELEM_FILTER
#define    AOSI_IPARAM_IS_ASYN_EXE              ISolverMathProgramInstance::IPARAM_IS_ASYN_EXE
#define    AOSI_IPARAM_IS_MPI_SOLVE             ISolverMathProgramInstance::IPARAM_IS_MPI_SOLVE
#define    AOSI_IPARAM_IS_ENCRYPTED             ISolverMathProgramInstance::IPARAM_IS_ENCRYPTED
#define    AOSI_IPARAM_IS_QUADRATIC             ISolverMathProgramInstance::IPARAM_IS_QUADRATIC
#define    AOSI_IPARAM_NR_MIP_STARTS            ISolverMathProgramInstance::IPARAM_NR_MIP_STARTS
#define    AOSI_IPARAM_IS_SOCP                  ISolverMathProgramInstance::IPARAM_IS_SOCP
#define    AOSI_IPARAM_LAZY_CALLBACK            ISolverMathProgramInstance::IPARAM_LAZY_CALLBACK  
#define    AOSI_IPARAM_MAX_INSTRUCT             ISolverMathProgramInstance::IPARAM_MAX_INSTRUCT
#define    AOSI_IPARAM_MAX_CONSTPOOL            ISolverMathProgramInstance::IPARAM_MAX_CONSTPOOL
#define    AOSI_IPARAM_MAX_INT_CONSTP           ISolverMathProgramInstance::IPARAM_MAX_INT_CONSTP
#define    AOSI_IPARAM_MAX                      ISolverMathProgramInstance::IPARAM_MAX             

#define    AOSI_DPARAM_SIZE                     ISolverMathProgramInstance::DPARAM_SIZE            
#define    AOSI_DPARAM_MARGINAL_ZERO            ISolverMathProgramInstance::DPARAM_MARGINAL_ZERO   
#define    AOSI_DPARAM_LOWEST_PRIORITY          ISolverMathProgramInstance::DPARAM_LOWEST_PRIORITY 
#define    AOSI_DPARAM_OBJECTIVE_ZERO           ISolverMathProgramInstance::DPARAM_OBJECTIVE_ZERO  
#define    AOSI_DPARAM_OBJECTIVE_CONST          ISolverMathProgramInstance::DPARAM_OBJECTIVE_CONST 
#define    AOSI_DPARAM_MAX                      ISolverMathProgramInstance::DPARAM_MAX             

#define    AOSI_ISTAT_MODEL_STAT                ISolverMathProgramInstance::ISTAT_MODEL_STAT    
#define    AOSI_ISTAT_SOLVER_STAT               ISolverMathProgramInstance::ISTAT_SOLVER_STAT   
#define    AOSI_ISTAT_TOTAL_ITER                ISolverMathProgramInstance::ISTAT_TOTAL_ITER    
#define    AOSI_ISTAT_NODES                     ISolverMathProgramInstance::ISTAT_NODES         
#define    AOSI_ISTAT_MEM_USED                  ISolverMathProgramInstance::ISTAT_MEM_USED      
#define    AOSI_ISTAT_RESTART_OFF               ISolverMathProgramInstance::ISTAT_RESTART_OFF   
#define    AOSI_ISTAT_POSTSOLVE_WARN            ISolverMathProgramInstance::ISTAT_POSTSOLVE_WARN
#define    AOSI_ISTAT_PRESOLVE_STAT             ISolverMathProgramInstance::ISTAT_PRESOLVE_STAT
#define    AOSI_ISTAT_POSTSOLVE_CONT            ISolverMathProgramInstance::ISTAT_POSTSOLVE_CONT
#define    AOSI_ISTAT_POSTSOLVE_INT             ISolverMathProgramInstance::ISTAT_POSTSOLVE_INT
#define    AOSI_ISTAT_MIP_CALC_SENS             ISolverMathProgramInstance::ISTAT_MIP_CALC_SENS
#define    AOSI_ISTAT_BRANCHES                  ISolverMathProgramInstance::ISTAT_BRANCHES
#define    AOSI_ISTAT_FAILURES                  ISolverMathProgramInstance::ISTAT_FAILURES
#define    AOSI_ISTAT_CALC_FARKAS               ISolverMathProgramInstance::ISTAT_CALC_FARKAS
#define    AOSI_ISTAT_MAX                       ISolverMathProgramInstance::ISTAT_MAX

#define    AOSI_DSTAT_OBJ                       ISolverMathProgramInstance::DSTAT_OBJ           
#define    AOSI_DSTAT_LP_BEST                   ISolverMathProgramInstance::DSTAT_LP_BEST
#define    AOSI_DSTAT_FARKAS_PROOF              ISolverMathProgramInstance::DSTAT_FARKAS_PROOF
#define    AOSI_DSTAT_MAX                       ISolverMathProgramInstance::DSTAT_MAX           

#define    AOSI_PROG_TYPE               IAimmsMathProgramCallback::PROG_TYPE            
#define    AOSI_PROG_ITER               IAimmsMathProgramCallback::PROG_ITER          
#define    AOSI_PROG_NODES_SOLVED       IAimmsMathProgramCallback::PROG_NODES_SOLVED  
#define    AOSI_PROG_NODES_LEFT         IAimmsMathProgramCallback::PROG_NODES_LEFT    
#define    AOSI_PROG_END                IAimmsMathProgramCallback::PROG_END           
#define    AOSI_PROG_MEM_USED           IAimmsMathProgramCallback::PROG_MEM_USED      
#define    AOSI_PROG_IP_CROSS_ITER      IAimmsMathProgramCallback::PROG_IP_CROSS_ITER    
#define    AOSI_PROG_THREADS            IAimmsMathProgramCallback::PROG_THREADS
#define    AOSI_PROG_MPS_TYPE           IAimmsMathProgramCallback::PROG_MPS_TYPE  
#define    AOSI_PROG_QP_AS_MIQP         IAimmsMathProgramCallback::PROG_QP_AS_MIQP  
#define    AOSI_PROG_IMAX               IAimmsMathProgramCallback::PROG_IMAX

#define    AOSI_PROG_OBJ                IAimmsMathProgramCallback::PROG_OBJ           
#define    AOSI_PROG_OBJ_BEST           IAimmsMathProgramCallback::PROG_OBJ_BEST      
#define    AOSI_PROG_LP_BEST            IAimmsMathProgramCallback::PROG_LP_BEST       
#define    AOSI_PROG_SUM_INFEAS         IAimmsMathProgramCallback::PROG_SUM_INFEAS
#define    AOSI_PROG_TUNING             IAimmsMathProgramCallback::PROG_TUNING
#define    AOSI_PROG_DMAX               IAimmsMathProgramCallback::PROG_DMAX

#define   AOSI_CB_INFO_ITER             ISolverMathProgramInstance::INFO_ITER  
#define   AOSI_CB_INFO_MAX              ISolverMathProgramInstance::INFO_MAX   

#define AOSI_PHASE_UNKNOWN              IAimmsMathProgramCallback::PHASE_UNKNOWN          
#define AOSI_PHASE_PRESOLVE             IAimmsMathProgramCallback::PHASE_PRESOLVE         
#define AOSI_PHASE_SIMPLEX              IAimmsMathProgramCallback::PHASE_SIMPLEX          
#define AOSI_PHASE_BARRIER              IAimmsMathProgramCallback::PHASE_BARRIER          
#define AOSI_PHASE_NETWORK              IAimmsMathProgramCallback::PHASE_NETWORK          
#define AOSI_PHASE_BARRIER_CROSSOVER    IAimmsMathProgramCallback::PHASE_BARRIER_CROSSOVER
#define AOSI_PHASE_NETWORK_CROSSOVER    IAimmsMathProgramCallback::PHASE_NETWORK_CROSSOVER
#define AOSI_PHASE_MIP                  IAimmsMathProgramCallback::PHASE_MIP              
#define AOSI_PHASE_POSTSOLVE            IAimmsMathProgramCallback::PHASE_POSTSOLVE        
#define AOSI_PHASE_MIP_POSTSOLVE        IAimmsMathProgramCallback::PHASE_MIP_POSTSOLVE    
#define AOSI_PHASE_SIFTING              IAimmsMathProgramCallback::PHASE_SIFTING          
#define AOSI_PHASE_PRIMAL_SIMPLEX       IAimmsMathProgramCallback::PHASE_PRIMAL_SIMPLEX   
#define AOSI_PHASE_DUAL_SIMPLEX         IAimmsMathProgramCallback::PHASE_DUAL_SIMPLEX     
#define AOSI_PHASE_MIP_POPULATE         IAimmsMathProgramCallback::PHASE_MIP_POPULATE     
#define AOSI_PHASE_TUNING               IAimmsMathProgramCallback::PHASE_TUNING           
#define AOSI_PHASE_IIS                  IAimmsMathProgramCallback::PHASE_IIS              

#define AOSI_SOL_TYPE_LP_INFEAS         IAimmsMathProgramCallback::SOL_TYPE_LP_INFEAS 
#define AOSI_SOL_TYPE_LP_FEAS           IAimmsMathProgramCallback::SOL_TYPE_LP_FEAS   
#define AOSI_SOL_TYPE_LP_UNB            IAimmsMathProgramCallback::SOL_TYPE_LP_UNB    
#define AOSI_SOL_TYPE_MIP_INFEAS        IAimmsMathProgramCallback::SOL_TYPE_MIP_INFEAS
#define AOSI_SOL_TYPE_MIP_FEAS          IAimmsMathProgramCallback::SOL_TYPE_MIP_FEAS  
#define AOSI_SOL_TYPE_MIP_UNB           IAimmsMathProgramCallback::SOL_TYPE_MIP_UNB   
#define AOSI_SOL_TYPE_NLP_INFEAS        IAimmsMathProgramCallback::SOL_TYPE_NLP_INFEAS
#define AOSI_SOL_TYPE_NLP_FEAS          IAimmsMathProgramCallback::SOL_TYPE_NLP_FEAS  
#define AOSI_SOL_TYPE_NLP_UNB           IAimmsMathProgramCallback::SOL_TYPE_NLP_UNB   

#define AOSI_CB_ITERATION               IAimmsMathProgramCallback::CB_ITERATION   
#define AOSI_CB_STATE_CHANGE            IAimmsMathProgramCallback::CB_STATE_CHANGE
#define AOSI_CB_INT_SOLUTION            IAimmsMathProgramCallback::CB_INT_SOLUTION
#define AOSI_CB_ADD_CUT                 IAimmsMathProgramCallback::CB_ADD_CUT     
#define AOSI_CB_HEURISTIC               IAimmsMathProgramCallback::CB_HEURISTIC   
#define AOSI_CB_INCUMBENT               IAimmsMathProgramCallback::CB_INCUMBENT   
#define AOSI_CB_BRANCH                  IAimmsMathProgramCallback::CB_BRANCH      
#define AOSI_CB_ADD_LAZY_CON            IAimmsMathProgramCallback::CB_ADD_LAZY_CON
                                                      
#define   AOSI_CB_MODEL_STAT            IAimmsMathProgramCallback::CB_MODEL_STAT 
#define   AOSI_CB_SOLVER_STAT           IAimmsMathProgramCallback::CB_SOLVER_STAT
#define   AOSI_CB_ITER                  IAimmsMathProgramCallback::CB_ITER       
#define   AOSI_CB_SOLVE_TIME            IAimmsMathProgramCallback::CB_SOLVE_TIME 
#define   AOSI_CB_NODE                  IAimmsMathProgramCallback::CB_NODE       
#define   AOSI_CB_NR_NODES              IAimmsMathProgramCallback::CB_NR_NODES
#define   AOSI_CB_NR_BR_NODES           IAimmsMathProgramCallback::CB_NR_BR_NODES
#define   AOSI_CB_NR_NODES_LEFT         IAimmsMathProgramCallback::CB_NR_NODES_LEFT
#define   AOSI_CB_NR_BRANCHES           IAimmsMathProgramCallback::CB_NR_BRANCHES
#define   AOSI_CB_NR_FAILURES           IAimmsMathProgramCallback::CB_NR_FAILURES
#define   AOSI_CB_IMAX                  IAimmsMathProgramCallback::CB_IMAX       

#define   AOSI_CB_OBJ                   IAimmsMathProgramCallback::CB_OBJ        
#define   AOSI_CB_OBJ_BEST              IAimmsMathProgramCallback::CB_OBJ_BEST   
#define   AOSI_CB_LP_BEST               IAimmsMathProgramCallback::CB_LP_BEST    
#define   AOSI_CB_NODE_OBJ              IAimmsMathProgramCallback::CB_NODE_OBJ
#define   AOSI_CB_DMAX                  IAimmsMathProgramCallback::CB_DMAX       

#define AOSI_CB_RET_OK                  IAimmsMathProgramCallback::RET_OK        
#define AOSI_CB_RET_INTR                IAimmsMathProgramCallback::RET_INTR      
#define AOSI_CB_RET_HEUR_SOL            IAimmsMathProgramCallback::RET_HEUR_SOL  
#define AOSI_CB_RET_REJECT_SOL          IAimmsMathProgramCallback::RET_REJECT_SOL

#define AOSI_PROG_RET_OK                IAimmsMathProgramCallback::INTR_RET_OK  
#define AOSI_PROG_RET_INTR              IAimmsMathProgramCallback::INTR_RET_INTR

#define AOSI_RETURN_BASIC_STATUS_NON_BASIC    ISolverMathProgramInstance::BASIC_STATUS_NON_BASIC 
#define AOSI_RETURN_BASIC_STATUS_BASIC        ISolverMathProgramInstance::BASIC_STATUS_BASIC     
#define AOSI_RETURN_BASIC_STATUS_SUPERBASIC   ISolverMathProgramInstance::BASIC_STATUS_SUPERBASIC
#define AOSI_RETURN_PRE_STATUS_IN_PRESOLVED   ISolverMathProgramInstance::PRE_STATUS_IN_PRESOLVED
#define AOSI_RETURN_PRE_STATUS_FIXED_LB       ISolverMathProgramInstance::PRE_STATUS_FIXED_LB
#define AOSI_RETURN_PRE_STATUS_FIXED_UB       ISolverMathProgramInstance::PRE_STATUS_FIXED_UB
#define AOSI_RETURN_PRE_STATUS_FIXED_OTHER    ISolverMathProgramInstance::PRE_STATUS_FIXED_OTHER
#define AOSI_RETURN_PRE_STATUS_AGGREGATED     ISolverMathProgramInstance::PRE_STATUS_AGGREGATED
#define AOSI_RETURN_PRE_STATUS_DELETED_OTHER  ISolverMathProgramInstance::PRE_STATUS_DELETED_OTHER
#define AOSI_RETURN_PRE_STATUS_REDUNDANT      ISolverMathProgramInstance::PRE_STATUS_REDUNDANT

#define AOSI_IOPT_progress_solution     IAimmsMathProgramInfo::IOPT_progress_solution 
#define AOSI_IOPT_progress_interval     IAimmsMathProgramInfo::IOPT_progress_interval
#define AOSI_IOPT_iteration_limit       IAimmsMathProgramInfo::IOPT_iteration_limit   
#define AOSI_IOPT_time_limit            IAimmsMathProgramInfo::IOPT_time_limit        
#define AOSI_IOPT_after_integer_sol     IAimmsMathProgramInfo::IOPT_after_integer_sol 
#define AOSI_IOPT_solver_messages       IAimmsMathProgramInfo::IOPT_solver_messages   
#define AOSI_IOPT_max_domain_errors     IAimmsMathProgramInfo::IOPT_max_domain_errors 
#define AOSI_IOPT_use_2D_order_der      IAimmsMathProgramInfo::IOPT_use_2D_order_der  
#ifdef DEBUG
#define AOSI_IOPT_solver_tracing        IAimmsMathProgramInfo::IOPT_solver_tracing  
#endif
#define AOSI_IOPT_MAX                   IAimmsMathProgramInfo::IOPT_MAX

#define AOSI_DOPT_solver_workspace      IAimmsMathProgramInfo::DOPT_solver_workspace 
#define AOSI_DOPT_mip_relative_opt      IAimmsMathProgramInfo::DOPT_mip_relative_opt 
#define AOSI_DOPT_mip_absolute_opt      IAimmsMathProgramInfo::DOPT_mip_absolute_opt 
#define AOSI_DOPT_mip_cutoff            IAimmsMathProgramInfo::DOPT_mip_cutoff       
#define AOSI_DOPT_MAX                   IAimmsMathProgramInfo::DOPT_MAX       

#define AOSI_ROW_WISE                   IAimmsMathProgramInfo::ROW_WISE
#define AOSI_COL_WISE                   IAimmsMathProgramInfo::COL_WISE
#define AOSI_REREAD                     IAimmsMathProgramInfo::REREAD  

#define AOSI_FLAGS_IS_BASIC             IAimmsMathProgramInfo::FLAGS_IS_BASIC        
#define AOSI_FLAGS_IS_INTEGER           IAimmsMathProgramInfo::FLAGS_IS_INTEGER      
#define AOSI_FLAGS_IS_CONTINUOUS        IAimmsMathProgramInfo::FLAGS_IS_CONTINUOUS   
#define AOSI_FLAGS_IS_SOS_TYPE_1        IAimmsMathProgramInfo::FLAGS_IS_SOS_TYPE_1   
#define AOSI_FLAGS_IS_SOS_TYPE_2        IAimmsMathProgramInfo::FLAGS_IS_SOS_TYPE_2   
#define AOSI_FLAGS_IS_SEMI_CONT         IAimmsMathProgramInfo::FLAGS_IS_SEMI_CONT    
#define AOSI_FLAGS_FIXED_COLUMN         IAimmsMathProgramInfo::FLAGS_FIXED_COLUMN    
#define AOSI_FLAGS_IS_NONLINEAR         IAimmsMathProgramInfo::FLAGS_IS_NONLINEAR    
#define AOSI_FLAGS_IS_QUADRATIC         IAimmsMathProgramInfo::FLAGS_IS_QUADRATIC    
#define AOSI_FLAGS_IS_CONVEX            IAimmsMathProgramInfo::FLAGS_IS_CONVEX       
#define AOSI_FLAGS_IS_RELAX_ONLY        IAimmsMathProgramInfo::FLAGS_IS_RELAX_ONLY   
#define AOSI_FLAGS_IS_INDICATOR         IAimmsMathProgramInfo::FLAGS_IS_INDICATOR    
#define AOSI_FLAGS_IS_LAZY              IAimmsMathProgramInfo::FLAGS_IS_LAZY         
#define AOSI_FLAGS_IS_CUT_POOL          IAimmsMathProgramInfo::FLAGS_IS_CUT_POOL     
#define AOSI_FLAGS_IS_FILTER            IAimmsMathProgramInfo::FLAGS_IS_FILTER       
#define AOSI_FLAGS_MM_MODIFIED_UB       IAimmsMathProgramInfo::FLAGS_MM_MODIFIED_UB  
#define AOSI_FLAGS_MM_MODIFIED_LB       IAimmsMathProgramInfo::FLAGS_MM_MODIFIED_LB  
#define AOSI_FLAGS_MM_MODIFIED_TYPE     IAimmsMathProgramInfo::FLAGS_MM_MODIFIED_TYPE
#define AOSI_FLAGS_IS_ACTIVITY          IAimmsMathProgramInfo::FLAGS_IS_ACTIVITY        
#define AOSI_FLAGS_IS_DUPL_SIZE_ROW     IAimmsMathProgramInfo::FLAGS_IS_DUPL_SIZE_ROW
#define AOSI_FLAGS_IN_START_POINT       IAimmsMathProgramInfo::FLAGS_IN_START_POINT

#define AOSI_NAME_COLUMN                IAimmsMathProgramInfo::NAME_COLUMN
#define AOSI_NAME_ROW                   IAimmsMathProgramInfo::NAME_ROW   
#define AOSI_NAME_MODEL                 IAimmsMathProgramInfo::NAME_MODEL 

#define AOSI_RETURN_SOLUTION_LEV        ISolverMathProgramInstance::RETURN_SOLUTION_LEV     
#define AOSI_RETURN_SOLUTION_MAR        ISolverMathProgramInstance::RETURN_SOLUTION_MAR     
#define AOSI_RETURN_SOLUTION_BASIC      ISolverMathProgramInstance::RETURN_SOLUTION_BASIC   
#define AOSI_RETURN_SOLUTION_PRESOLVE   ISolverMathProgramInstance::RETURN_SOLUTION_PRESOLVE
  
#define AOSI_RANGE_OBJ_COEF             ISolverMathProgramInstance::RANGE_OBJ_COEF
#define AOSI_RANGE_RHS                  ISolverMathProgramInstance::RANGE_RHS     

#define AOSI_TUNING_NONE                ISolverMathProgramInstance::TUNING_NONE    
#define AOSI_TUNING_FIXED               ISolverMathProgramInstance::TUNING_FIXED
#define AOSI_TUNING_TUNED               ISolverMathProgramInstance::TUNING_TUNED 

#define AOSI_DEF_IS_GONOMETRIC          IAimmsMathProgramNonLinearEvaluator::DEF_IS_GONOMETRIC
#define AOSI_DEF_HAS_ERRORF             IAimmsMathProgramNonLinearEvaluator::DEF_HAS_ERRORF

#define AOSI_ACT_SIZE_BINDING_LB        IAimmsMathProgramNonLinearEvaluator::ACT_SIZE_BINDING_LB
#define AOSI_ACT_SIZE_BINDING_UB        IAimmsMathProgramNonLinearEvaluator::ACT_SIZE_BINDING_UB
#define AOSI_ACT_LEN_BINDING_LB         IAimmsMathProgramNonLinearEvaluator::ACT_LEN_BINDING_LB
#define AOSI_ACT_LEN_BINDING_UB         IAimmsMathProgramNonLinearEvaluator::ACT_LEN_BINDING_UB
#define AOSI_ACT_START_BINDING_LB       IAimmsMathProgramNonLinearEvaluator::ACT_START_BINDING_LB
#define AOSI_ACT_START_BINDING_UB       IAimmsMathProgramNonLinearEvaluator::ACT_START_BINDING_UB
#define AOSI_ACT_END_BINDING_LB         IAimmsMathProgramNonLinearEvaluator::ACT_END_BINDING_LB
#define AOSI_ACT_END_BINDING_UB         IAimmsMathProgramNonLinearEvaluator::ACT_END_BINDING_UB

#define AOSI_MM_UB_CHANGED              IAimmsMathProgramMatrixManipulationInfo::MM_UB_CHANGED        
#define AOSI_MM_LB_CHANGED              IAimmsMathProgramMatrixManipulationInfo::MM_LB_CHANGED        
#define AOSI_MM_TYPE_CHANGED            IAimmsMathProgramMatrixManipulationInfo::MM_TYPE_CHANGED      
#define AOSI_MM_DIRECTION_CHANGED       IAimmsMathProgramMatrixManipulationInfo::MM_DIRECTION_CHANGED 
#define AOSI_MM_ROW_DEACTIVATED         IAimmsMathProgramMatrixManipulationInfo::MM_ROW_DEACTIVATED   
#define AOSI_MM_ROW_ACTIVATED           IAimmsMathProgramMatrixManipulationInfo::MM_ROW_ACTIVATED     
#define AOSI_MM_ROW_TYPE_CHANGED        IAimmsMathProgramMatrixManipulationInfo::MM_ROW_TYPE_CHANGED  
#define AOSI_MM_ROW_TYPE_LE             IAimmsMathProgramMatrixManipulationInfo::MM_ROW_TYPE_LE       
#define AOSI_MM_ROW_TYPE_GE             IAimmsMathProgramMatrixManipulationInfo::MM_ROW_TYPE_GE       
#define AOSI_MM_ROW_TYPE_EQ             IAimmsMathProgramMatrixManipulationInfo::MM_ROW_TYPE_EQ       
#define AOSI_MM_ROW_TYPE_RANGED         IAimmsMathProgramMatrixManipulationInfo::MM_ROW_TYPE_RANGED   
#define AOSI_MM_CHANGED_MODEL           IAimmsMathProgramMatrixManipulationInfo::MM_CHANGED_MODEL     
#define AOSI_MM_DIRECTION_WAS_NONE      IAimmsMathProgramMatrixManipulationInfo::MM_DIRECTION_WAS_NONE
#define AOSI_MM_OUTER_APPROX            IAimmsMathProgramMatrixManipulationInfo::MM_OUTER_APPROX      
#define AOSI_MM_RELOAD_MODEL            IAimmsMathProgramMatrixManipulationInfo::MM_RELOAD_MODEL      

#define AOSI_MM_TYPE_LP                 IAimmsMathProgramMatrixManipulationInfo::MM_TYPE_LP    
#define AOSI_MM_TYPE_LS                 IAimmsMathProgramMatrixManipulationInfo::MM_TYPE_LS   
#define AOSI_MM_TYPE_MIP                IAimmsMathProgramMatrixManipulationInfo::MM_TYPE_MIP   
#define AOSI_MM_TYPE_RMIP               IAimmsMathProgramMatrixManipulationInfo::MM_TYPE_RMIP   

