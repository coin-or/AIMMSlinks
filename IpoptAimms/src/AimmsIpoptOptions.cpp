// Copyright (C) 2009 Paragon Decision Technology B.V. and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// $Id$
//
// Authors: Yidong Lang, L.T. Biegler, Chuck Teeter, Marcel Hunting

// Suppress warning messages associated with older CRT library functions such as sprintf() and strcpy()
#define _CRT_SECURE_NO_WARNINGS

// Includes necessary IPOPT header files
#include <stdio.h>
#include "libIpopt.h"
#include "AimmsIpoptOptions.h"
#include <string.h>


//@< The function |IPOPT_order_options()| @> @;
//@< The function |IPOPT_Init_Global_Options()| @> @;
//@< The function |IPOPT_Init_Options()| @> @;
//@< The function |IPOPT_Get_Options()| @> @;
//@< The function |IPOPT_Set_Options()| @> @;

//@< The function |GetIntegerOption()| @> @;
//@< The function |GetIntegerKeyword()| @> @;
//@< The function |GetDoubleOption()| @> @;
//@< The function |GetCategoryName()| @> @;


// Names of the option categories to be displayed in the AIMMS solver option dialog.
static const char* ipopt_category[] = 
{
    "Barrier_parameter",
    "Hessian_perturbation",
    "Initialization",
    "Line_search",	
	"Linear_solver",
	"Multipliers",
	"NLP",
    "NLP_scaling",
	"Output",
	"Quasi-Newton",
	"Restoration_phase",
	"Termination",
	
	"Derivative_test"

// CET 8-23-2009: Warm start not yet supported
//	"Warm_start"
};



// The lower bounds of the integer options
static int  ipopt_int_opt_low[ IPOPT_OPT_INT_MAX ] = 
{
	// CET: Updated integer options for IPOPT 3.7
	
	/* IPOPT_OPT_ACCEPT_EVERY_TRIAL_STEP	   */	  0,

	/* IPOPT_OPT_BOUND_MULT_INIT_METHOD		   */	  0,			// CET: Added 8-24-2009
	/* IPOPT_OPT_WATCHDOG_SHORTENED_ITER_TRIGGER */	  0,			// CET: Added 8-24-2009
	/* IPOPT_OPT_LIMITED_MEMORY_MAX_HISTORY	   */	  0,			// CET: Added 8-24-2009
	/* IPOPT_OPT_LIMITED_MEMORY_MAX_SKIPPING   */	  1,			// CET: Added 8-24-2009

	/* IPOPT_OPT_START_WITH_RESTO			   */	  0,			// CET: Added 8-24-2009
	/* IPOPT_OPT_EVALUATE_ORIG_OBJ_AT_RESTO_TRIAL */  0,			// CET: Added 8-24-2009

#ifdef MC19_IS_AVAILABLE
	/* IPOPT_OPT_LINEAR_SYSTEM_SCALING		   */	  0,			// CET: Added 8-24-2009
#endif

	/* IPOPT_OPT_JAC_C_CONSTANT				   */	  0,
	/* IPOPT_OPT_JAC_D_CONSTANT				   */	  0,
	/* IPOPT_OPT_HESSIAN_CONSTANT			   */     0,
	/* IPOPT_OPT_BARRIER_UPDATE_METHOD		   */     0,
	/* IPOPT_OPT_CHECK_DERIV_NANINF			   */     0,
	/* IPOPT_OPT_CONSTRAINT_MULT_STEP_SIZE	   */	  0,
	/* IPOPT_OPT_CHECK_DERIVATIVES			   */     0,
	/* IPOPT_OPT_BARRIER_MEHROTRA			   */	  0,
	/* IPOPT_OPT_EXPECT_INFEAS_PROBLEM		   */	  0,
	/* IPOPT_OPT_FIXED_VARIABLE_TREATMENT	   */	  0,
	/* IPOPT_OPT_HESSIAN_APPROXIMATION		   */	  0,
	/* IPOPT_OPT_LINEAR_SOLVER				   */	  0,			// CET: Added 4-30-2009

#ifdef MC19_IS_AVAILABLE
	/* IPOPT_OPT_SCALE_ON_DEMAND			   */	  0,
#endif

#ifdef MA57_IS_AVAILABLE
	/* IPOPT_OPT_MA57_PIVOT_ORDER			   */	  0,			// CET: Added 04-23-2010
	/* IPOPT_OPT_MA57_BLOCK_SIZE			   */	  1,			// CET: Added 04-28-2010
	/* IPOPT_OPT_MA57_NODE_AMALGAMATION		   */	  1,			// CET: Added 04-28-2010
	/* IPOPT_OPT_MA57_SCALING_FLAG			   */	  0,			// CET: Added 04-28-2010
	/* IPOPT_OPT_MA57_SMALL_PIVOT_FLAG		   */	  0,			// CET: Added 04-28-2010
#endif

#ifdef MUMPS_IS_AVAILABLE
	/* IPOPT_OPT_MUMPS_MEM_PERCENT			   */	  0,			// CET: Added 8-22-2009
	/* IPOPT_OPT_MUMPS_PERMUTING_SCALING	   */	  0,			// CET: Added 8-22-2009
	/* IPOPT_OPT_MUMPS_PIVOT_ORDER			   */	  0,			// CET: Added 8-22-2009
	/* IPOPT_OPT_MUMPS_SCALING				   */	 -2,			// CET: Added 8-22-2009
#endif

	/* IPOPT_OPT_MAX_NUM_ITER				   */	  0,
	/* IPOPT_OPT_ACCEPTABLE_ITER			   */	  0,			// CET: Added 8-23-2009
	/* IPOPT_OPT_MAX_NUM_REFINEMENT_STEPS	   */	  0,
	/* IPOPT_OPT_MAX_WATCHDOG_ITER			   */	  0,
	/* IPOPT_OPT_MIN_NUM_REFINEMENT_STEPS	   */	  0,
	/* IPOPT_OPT_SCALING_METHOD                */     0,
	/* IPOPT_OPT_BARRIER_ORACLE_ADAPTIVE	   */	  0,
	/* IPOPT_OPT_BARRIER_FIXED_MU_ORACLE	   */     0,
	/* IPOPT_OPT_PRINT_LEVEL				   */	  0,
	/* IPOPT_OPT_PRINT_OPTIONS_DOCUMENTATION   */     0,
	/* IPOPT_OPT_PRINT_DERIV				   */	  0,
	/* IPOPT_OPT_PRINT_USER_OPTIONS			   */	  0,
	/* IPOPT_OPT_HONOR_ORIGINAL_BOUNDS		   */	  0,
	/* IPOPT_OPT_BARRIER_QUALITY_FN_MAX_STEPS  */	  0,
	/* IPOPT_OPT_RECALC_MULTIPLIERS			   */	  0,
	/* IPOPT_OPT_STATUS_FILE                   */     0,
	/* IPOPT_OPT_MAX_2ND_ORDER_STEPS		   */	  0,
	/* IPOPT_OPT_CORRECTOR_STEP_TYPE		   */	  0
//,	/* IPOPT_OPT_WARM_START					   */	  0				// CET: Not currently used
};


// The default values of the integer options
static int  ipopt_int_opt_def[ IPOPT_OPT_INT_MAX ] = 
{
	// CET: Updated integer options for IPOPT 3.7
	
	/* IPOPT_OPT_ACCEPT_EVERY_TRIAL_STEP	   */	  0,

	/* IPOPT_OPT_BOUND_MULT_INIT_METHOD		   */	  0,			// CET: Added 8-24-2009
	/* IPOPT_OPT_WATCHDOG_SHORTENED_ITER_TRIGGER */	 10,			// CET: Added 8-24-2009
	/* IPOPT_OPT_LIMITED_MEMORY_MAX_HISTORY	   */	  6,			// CET: Added 8-24-2009
	/* IPOPT_OPT_LIMITED_MEMORY_MAX_SKIPPING   */	  2,			// CET: Added 8-24-2009

	/* IPOPT_OPT_START_WITH_RESTO			   */	  0,			// CET: Added 8-24-2009
	/* IPOPT_OPT_EVALUATE_ORIG_OBJ_AT_RESTO_TRIAL */  1,			// CET: Added 8-24-2009

#ifdef MC19_IS_AVAILABLE
	/* IPOPT_OPT_LINEAR_SYSTEM_SCALING		   */	  1,			// CET: Added 8-24-2009
#endif

	/* IPOPT_OPT_JAC_C_CONSTANT				   */	  0,
	/* IPOPT_OPT_JAC_D_CONSTANT				   */	  0,
	/* IPOPT_OPT_HESSIAN_CONSTANT			   */     0,
	/* IPOPT_OPT_BARRIER_UPDATE_METHOD		   */     0,
	/* IPOPT_OPT_CHECK_DERIV_NANINF			   */     0,
	/* IPOPT_OPT_CONSTRAINT_MULT_STEP_SIZE	   */	  0,
	/* IPOPT_OPT_CHECK_DERIVATIVES			   */     0,
	/* IPOPT_OPT_BARRIER_MEHROTRA			   */	  0,
	/* IPOPT_OPT_EXPECT_INFEAS_PROBLEM		   */	  0,
	/* IPOPT_OPT_FIXED_VARIABLE_TREATMENT	   */	  0,
	/* IPOPT_OPT_HESSIAN_APPROXIMATION		   */	  0,
	/* IPOPT_OPT_LINEAR_SOLVER				   */	  0,			// CET: Added 4-30-2009

#ifdef MC19_IS_AVAILABLE
	/* IPOPT_OPT_SCALE_ON_DEMAND			   */	  1,
#endif

#ifdef MA57_IS_AVAILABLE
	/* IPOPT_OPT_MA57_PIVOT_ORDER			   */	  5,			// CET: Added 04-23-2010
	/* IPOPT_OPT_MA57_BLOCK_SIZE			   */	 16,			// CET: Added 04-28-2010
	/* IPOPT_OPT_MA57_NODE_AMALGAMATION		   */	 16,			// CET: Added 04-28-2010
	/* IPOPT_OPT_MA57_SCALING_FLAG			   */	  1,			// CET: Added 04-28-2010
	/* IPOPT_OPT_MA57_SMALL_PIVOT_FLAG		   */	  0,			// CET: Added 04-28-2010
#endif

#ifdef MUMPS_IS_AVAILABLE
	/* IPOPT_OPT_MUMPS_MEM_PERCENT			   */  1000,			// CET: Added 8-22-2009
	/* IPOPT_OPT_MUMPS_PERMUTING_SCALING	   */	  7,			// CET: Added 8-22-2009
	/* IPOPT_OPT_MUMPS_PIVOT_ORDER			   */	  7,			// CET: Added 8-22-2009
	/* IPOPT_OPT_MUMPS_SCALING				   */	  7,			// CET: Added 8-22-2009
#endif

	/* IPOPT_OPT_MAX_NUM_ITER				   */  3000,
	/* IPOPT_OPT_ACCEPTABLE_ITER			   */	 15,			// CET: Added 8-23-2009
	/* IPOPT_OPT_MAX_NUM_REFINEMENT_STEPS	   */	 10,
	/* IPOPT_OPT_MAX_WATCHDOG_ITER			   */	  3,
	/* IPOPT_OPT_MIN_NUM_REFINEMENT_STEPS	   */	  1,
	/* IPOPT_OPT_SCALING_METHOD                */     1,			// CET: The default is now 1 instead of 2, since user scaling is not supported
	/* IPOPT_OPT_BARRIER_ORACLE_ADAPTIVE	   */	  2,
	/* IPOPT_OPT_BARRIER_FIXED_MU_ORACLE	   */     3,
	/* IPOPT_OPT_PRINT_LEVEL				   */	  5,
	/* IPOPT_OPT_PRINT_OPTIONS_DOCUMENTATION   */     0,
	/* IPOPT_OPT_PRINT_DERIV				   */	  0,
	/* IPOPT_OPT_PRINT_USER_OPTIONS			   */	  0,
	/* IPOPT_OPT_HONOR_ORIGINAL_BOUNDS		   */	  1,
	/* IPOPT_OPT_BARRIER_QUALITY_FN_MAX_STEPS  */	  8,
	/* IPOPT_OPT_RECALC_MULTIPLIERS			   */	  0,
	/* IPOPT_OPT_STATUS_FILE                   */     0,
	/* IPOPT_OPT_MAX_2ND_ORDER_STEPS		   */	  4,
	/* IPOPT_OPT_CORRECTOR_STEP_TYPE		   */	  0
//,	/* IPOPT_OPT_WARM_START					   */	  0				// CET: Not currently used
};


// The upper bounds of the integer options
static int  ipopt_int_opt_up[ IPOPT_OPT_INT_MAX ] = 
{
	// CET: Updated integer options for IPOPT 3.7
	
	/* IPOPT_OPT_ACCEPT_EVERY_TRIAL_STEP	   */	  1,

	/* IPOPT_OPT_BOUND_MULT_INIT_METHOD		   */	  1,			// CET: Added 8-24-2009
	/* IPOPT_OPT_WATCHDOG_SHORTENED_ITER_TRIGGER */	  IPOPT_MAXINT,	// CET: Added 8-24-2009
	/* IPOPT_OPT_LIMITED_MEMORY_MAX_HISTORY	   */	  IPOPT_MAXINT,	// CET: Added 8-24-2009
	/* IPOPT_OPT_LIMITED_MEMORY_MAX_SKIPPING   */	  IPOPT_MAXINT,	// CET: Added 8-24-2009

	/* IPOPT_OPT_START_WITH_RESTO			   */	  1,			// CET: Added 8-24-2009
	/* IPOPT_OPT_EVALUATE_ORIG_OBJ_AT_RESTO_TRIAL */  1,			// CET: Added 8-24-2009

#ifdef MC19_IS_AVAILABLE
	/* IPOPT_OPT_LINEAR_SYSTEM_SCALING		   */	  1,			// CET: Added 8-24-2009
#endif

	/* IPOPT_OPT_JAC_C_CONSTANT				   */	  1,
	/* IPOPT_OPT_JAC_D_CONSTANT				   */	  1,
	/* IPOPT_OPT_HESSIAN_CONSTANT			   */     1,
	/* IPOPT_OPT_BARRIER_UPDATE_METHOD		   */     1,
	/* IPOPT_OPT_CHECK_DERIV_NANINF			   */     1,
	/* IPOPT_OPT_CONSTRAINT_MULT_STEP_SIZE	   */	  9,
	/* IPOPT_OPT_CHECK_DERIVATIVES			   */     2,
	/* IPOPT_OPT_BARRIER_MEHROTRA			   */	  1,
	/* IPOPT_OPT_EXPECT_INFEAS_PROBLEM		   */	  1,
	/* IPOPT_OPT_FIXED_VARIABLE_TREATMENT	   */	  2,
	/* IPOPT_OPT_HESSIAN_APPROXIMATION		   */	  2,
	/* IPOPT_OPT_LINEAR_SOLVER				   */	  NR_LINEAR_SOLVERS - 1, // CET: Added 4-30-2009.  Note that use of "custom" linear solver is not supported.

#ifdef MC19_IS_AVAILABLE
	/* IPOPT_OPT_SCALE_ON_DEMAND			   */	  1,
#endif

#ifdef MA57_IS_AVAILABLE
	/* IPOPT_OPT_MA57_PIVOT_ORDER			   */	  5,			// CET: Added 04-23-2010
	/* IPOPT_OPT_MA57_BLOCK_SIZE			   */	  IPOPT_MAXINT,	// CET: Added 04-28-2010
	/* IPOPT_OPT_MA57_NODE_AMALGAMATION		   */	  IPOPT_MAXINT,	// CET: Added 04-28-2010
	/* IPOPT_OPT_MA57_SCALING_FLAG			   */	  1,			// CET: Added 04-28-2010
	/* IPOPT_OPT_MA57_SMALL_PIVOT_FLAG		   */	  1,			// CET: Added 04-28-2010
#endif

#ifdef MUMPS_IS_AVAILABLE
	/* IPOPT_OPT_MUMPS_MEM_PERCENT			   */	  IPOPT_MAXINT,	// CET: Added 8-22-2009
	/* IPOPT_OPT_MUMPS_PERMUTING_SCALING	   */	  7,			// CET: Added 8-22-2009
	/* IPOPT_OPT_MUMPS_PIVOT_ORDER			   */	  7,			// CET: Added 8-22-2009
	/* IPOPT_OPT_MUMPS_SCALING				   */	  7,			// CET: Added 8-22-2009
#endif

	/* IPOPT_OPT_MAX_NUM_ITER				   */	  IPOPT_MAXINT,
	/* IPOPT_OPT_ACCEPTABLE_ITER			   */	  IPOPT_MAXINT,	// CET: Added 8-23-2009
	/* IPOPT_OPT_MAX_NUM_REFINEMENT_STEPS	   */	  IPOPT_MAXINT,
	/* IPOPT_OPT_MAX_WATCHDOG_ITER			   */	  IPOPT_MAXINT,
	/* IPOPT_OPT_MIN_NUM_REFINEMENT_STEPS	   */	  IPOPT_MAXINT,
	/* IPOPT_OPT_SCALING_METHOD                */     2,			// CET: The upper bound is now 2 instead of 3, since user-scaling is not supported
	/* IPOPT_OPT_BARRIER_ORACLE_ADAPTIVE	   */	  2,
	/* IPOPT_OPT_BARRIER_FIXED_MU_ORACLE	   */     3,
	/* IPOPT_OPT_PRINT_LEVEL				   */	 12,
	/* IPOPT_OPT_PRINT_OPTIONS_DOCUMENTATION   */     1,
	/* IPOPT_OPT_PRINT_DERIV				   */	  1,
	/* IPOPT_OPT_PRINT_USER_OPTIONS			   */	  1,
	/* IPOPT_OPT_HONOR_ORIGINAL_BOUNDS		   */	  1,
	/* IPOPT_OPT_BARRIER_QUALITY_FN_MAX_STEPS  */	  IPOPT_MAXINT,
	/* IPOPT_OPT_RECALC_MULTIPLIERS			   */	  1,
	/* IPOPT_OPT_STATUS_FILE                   */     1,
	/* IPOPT_OPT_MAX_2ND_ORDER_STEPS		   */	  IPOPT_MAXINT,
	/* IPOPT_OPT_CORRECTOR_STEP_TYPE		   */	  2
//,	/* IPOPT_OPT_WARM_START					   */	  1				// CET: Not currently implemented
};



// The lower bounds of the double options
static double  ipopt_dbl_opt_low[ IPOPT_OPT_DBL_MAX ] = 
{
	/* IPOPT_OPT_NLP_LOWER_BOUND_INF		   */	-IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_NLP_UPPER_BOUND_INF		   */	-IPOPT_INF,			// CET: Added 8-24-2009

	/* IPOPT_OPT_SLACK_BOUND_FRAC			   */ 	0.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_SLACK_BOUND_PUSH 			   */	0.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_CONSTR_MULT_INIT_MAX		   */	0.0,				// CET: Added 8-24-2009

	/* IPOPT_OPT_MU_MAX_FACT				   */	0.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_MU_MAX						   */	0.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_MU_MIN						   */	0.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_BARRIER_TOL_FACTOR			   */	0.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_MU_LINEAR_DECREASE_FACTOR	   */	0.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_MU_SUPERLINEAR_DECREASE_POWER */	1.0,				// CET: Added 8-24-2009

	/* IPOPT_OPT_RECALC_Y_FEAS_TOL			   */	1.0e-10,				// CET: Added 8-24-2009

	/* IPOPT_OPT_EXPECT_INFEASIBLE_PROBLEM_CTOL			*/	0.0,		// CET: Added 8-24-2009
	/* IPOPT_OPT_EXPECT_INFEASIBLE_PROBLEM_YTOL			*/	1.0e-10,
	/* IPOPT_OPT_SOFT_RESTO_PDERROR_REDUCTION_FACTOR	*/	0.0,		// CET: Added 8-24-2009
	/* IPOPT_OPT_REQUIRED_INFEASIBILITY_REDUCTION		*/	0.0,		// CET: Added 8-24-2009
	/* IPOPT_OPT_BOUND_MULT_RESET_THRESHOLD				*/	0.0,		// CET: Added 8-24-2009
	/* IPOPT_OPT_CONSTR_MULT_RESET_THRESHOLD			*/	0.0,		// CET: Added 8-24-2009

	/* IPOPT_OPT_MAX_HESSIAN_PERTURBATION	   */	0.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_MIN_HESSIAN_PERTURBATION	   */	0.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_FIRST_HESSIAN_PERTURBATION	   */	0.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_PERTURB_INC_FACT_FIRST		   */	1.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_PERTURB_INC_FACT			   */	1.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_PERTURB_DEC_FACT			   */	0.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_JACOBIAN_REGULARIZATION_VALUE */	0.0,				// CET: Added 8-24-2009

	/* IPOPT_OPT_DERIVATIVE_TEST_PERTURBATION  */	1.0e-10,				// CET: Added 8-24-2009
	/* IPOPT_OPT_DERIVATIVE_TEST_TOL		   */	1.0e-10,				// CET: Added 8-24-2009
	/* IPOPT_OPT_POINT_PERTURBATION_RADIUS	   */	0.0,				// CET: Added 8-24-2009

	/* IPOPT_OPT_ABSOLUTE_BOUND_DISTANCE       */   0.0 ,				// CET: Updated 4-28-2009
	/* IPOPT_OPT_BARRIER_INITIAL_VALUE         */   0.0 ,				// CET: Updated 4-28-2009
	/* IPOPT_OPT_BOUND_MULTIPLIERS_INIT_VAL    */   0.0 ,				// CET: Updated 4-28-2009
	/* IPOPT_OPT_BOUNDS_RELAXATION_FACTOR      */   0.0 ,				// CET: Checked 4-28-2009
	/* IPOPT_OPT_COMPLEMENTARITY_ACCEP_TOL     */   1.0e-10 ,				// CET: Updated 4-28-2009
	/* IPOPT_OPT_COMPLEMENTARITY_TOLERANCE     */   1.0e-10 ,				// CET: Updated 4-28-2009
	/* IPOPT_OPT_CONSTRAINT_VIOL_ACCEP_TOL     */   1.0e-10 ,				// CET: Updated 4-28-2009
	/* IPOPT_OPT_CONSTRAINT_VIOL_TOLERANCE     */   1.0e-10 ,				// CET: Updated 4-28-2009
	/* IPOPT_OPT_CONVERGENCE_ACCEP_TOL         */   1.0e-10 ,				// CET: Updated 4-28-2009
	/* IPOPT_OPT_CONVERGENCE_TOLERANCE         */   1.0e-10 ,				// CET: Updated 4-28-2009
	/* IPOPT_OPT_DIVERGING_ITERATES_TOL		   */	1.0e-10 ,				// CET: Added 4-28-2009
	/* IPOPT_OPT_DUAL_INF_ACCEP_TOL            */   1.0e-10 ,				// CET: Updated 4-28-2009
	/* IPOPT_OPT_DUAL_INF_TOLERANCE            */   1.0e-10 ,				// CET: Updated 4-28-2009

#ifdef MA27_IS_AVAILABLE
	/* IPOPT_OPT_MA27_MAXIMUM_PIVOT_TOLERANCE  */   0.0 ,				// CET: Updated 4-30-2009
	/* IPOPT_OPT_MA27_PIVOT_TOLERANCE          */   0.0 ,				// CET: Checked 4-30-2009
	/* IPOPT_OPT_MA27_LIW_INIT_FACTOR		   */   1.0 ,				// CET: Added 8-22-2009
	/* IPOPT_OPT_MA27_LA_INIT_FACTOR		   */   1.0 ,				// CET: Added 8-22-2009
	/* IPOPT_OPT_MA27_MEMINC_FACTOR			   */	1.0 ,				// CET: Added 8-22-2009
#endif

#ifdef MA57_IS_AVAILABLE
	/* IPOPT_OPT_MA57_MAXIMUM_PIVOT_TOLERANCE  */   0.0 ,				// CET: Updated 4-30-2009
	/* IPOPT_OPT_MA57_PIVOT_TOLERANCE		   */   0.0 ,				// CET: Updated 4-30-2009
	/* IPOPT_OPT_MA57_PRE_ALLOC				   */	1.0 ,				// CET: Added 04-23-2010
#endif

#ifdef MUMPS_IS_AVAILABLE
	/* IPOPT_OPT_MUMPS_MAXIMUM_PIVOT_TOLERANCE */   0.0 ,				// CET: Updated 4-30-2009
	/* IPOPT_OPT_MUMPS_PIVOT_TOLERANCE		   */   0.0 ,				// CET: Updated 4-30-2009
#endif
	
	/* IPOPT_OPT_NLP_SCALING_MAXIMUM_GRAD      */   1.0e-10 ,				// CET: Updated 4-28-2009
	/* IPOPT_OPT_OBJ_CHANGE_ACCEP_TOL		   */   0.0 ,				// CET: Added 4-30-2009
	/* IPOPT_OPT_OBJECTIVE_SCALING_FACTOR      */   -IPOPT_INF ,		// CET: Checked 4-28-2009
	/* IPOPT_OPT_RELATIVE_BOUND_DISTANCE       */   0.0	,				// CET: Updated 4-28-2009
	/* IPOPT_OPT_ALPHA_FOR_Y_TOL			   */   0.0					// CET: Added 4-30-2009

#ifdef WSMP_IS_AVAILABLE
,	/* IPOPT_OPT_WSMP_MAXIMUM_PIVOT_TOLERANCE  */   0.0 ,				// CET: Updated 4-30-2009
	/* IPOPT_OPT_WSMP_PIVOT_TOLERANCE		   */   0.0					// CET: Updated 4-30-2009
#endif
};


// The default values of the double options
static double  ipopt_dbl_opt_def[ IPOPT_OPT_DBL_MAX ] = 
{
	/* IPOPT_OPT_NLP_LOWER_BOUND_INF		   */	-1.0e19,			// CET: Added 8-24-2009
	/* IPOPT_OPT_NLP_UPPER_BOUND_INF		   */	 1.0e19,			// CET: Added 8-24-2009

	/* IPOPT_OPT_SLACK_BOUND_FRAC			   */ 	0.01,				// CET: Added 8-24-2009
	/* IPOPT_OPT_SLACK_BOUND_PUSH,			   */	0.01,				// CET: Added 8-24-2009
	/* IPOPT_OPT_CONSTR_MULT_INIT_MAX,		   */	1000.0,				// CET: Added 8-24-2009

	/* IPOPT_OPT_MU_MAX_FACT				   */	1000.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_MU_MAX						   */	100000.0,			// CET: Added 8-24-2009
	/* IPOPT_OPT_MU_MIN						   */	1.0e-11,			// CET: Added 8-24-2009
	/* IPOPT_OPT_BARRIER_TOL_FACTOR			   */	10.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_MU_LINEAR_DECREASE_FACTOR	   */	0.2,				// CET: Added 8-24-2009
	/* IPOPT_OPT_MU_SUPERLINEAR_DECREASE_POWER */	1.5,				// CET: Added 8-24-2009

	/* IPOPT_OPT_RECALC_Y_FEAS_TOL			   */	1.0e-6,				// CET: Added 8-24-2009

	/* IPOPT_OPT_EXPECT_INFEASIBLE_PROBLEM_CTOL			*/	0.001,		// CET: Added 8-24-2009
	/* IPOPT_OPT_EXPECT_INFEASIBLE_PROBLEM_YTOL			*/	1.0e8,
	/* IPOPT_OPT_SOFT_RESTO_PDERROR_REDUCTION_FACTOR	*/	0.9999,		// CET: Added 8-24-2009
	/* IPOPT_OPT_REQUIRED_INFEASIBILITY_REDUCTION		*/	0.9,		// CET: Added 8-24-2009
	/* IPOPT_OPT_BOUND_MULT_RESET_THRESHOLD				*/	1000.0,		// CET: Added 8-24-2009
	/* IPOPT_OPT_CONSTR_MULT_RESET_THRESHOLD			*/	0.0,		// CET: Added 8-24-2009

	/* IPOPT_OPT_MAX_HESSIAN_PERTURBATION	   */	IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_MIN_HESSIAN_PERTURBATION	   */	1.0e-20,			// CET: Added 8-24-2009
	/* IPOPT_OPT_FIRST_HESSIAN_PERTURBATION	   */	0.0001,				// CET: Added 8-24-2009
	/* IPOPT_OPT_PERTURB_INC_FACT_FIRST		   */	100.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_PERTURB_INC_FACT			   */	8.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_PERTURB_DEC_FACT			   */	0.333333,			// CET: Added 8-24-2009
	/* IPOPT_OPT_JACOBIAN_REGULARIZATION_VALUE */	1.0e-8,				// CET: Added 8-24-2009

	/* IPOPT_OPT_DERIVATIVE_TEST_PERTURBATION  */	1.0e-8,				// CET: Added 8-24-2009
	/* IPOPT_OPT_DERIVATIVE_TEST_TOL		   */	0.0001,				// CET: Added 8-24-2009
	/* IPOPT_OPT_POINT_PERTURBATION_RADIUS	   */	10.0,				// CET: Added 8-24-2009

	/* IPOPT_OPT_ABSOLUTE_BOUND_DISTANCE       */   0.01 ,				// CET: Checked 4-28-2009
	/* IPOPT_OPT_BARRIER_INITIAL_VALUE         */   0.1 ,				// CET: Checked 4-28-2009
	/* IPOPT_OPT_BOUND_MULTIPLIERS_INIT_VAL    */   1.0 ,				// CET: Checked 4-28-2009
	/* IPOPT_OPT_BOUNDS_RELAXATION_FACTOR      */   1.0e-8 ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_COMPLEMENTARITY_ACCEP_TOL     */   0.01 ,				// CET: Checked 4-28-2009
	/* IPOPT_OPT_COMPLEMENTARITY_TOLERANCE     */   0.0001 ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_CONSTRAINT_VIOL_ACCEP_TOL     */   0.01 ,				// CET: Checked 4-28-2009
	/* IPOPT_OPT_CONSTRAINT_VIOL_TOLERANCE     */   0.0001 ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_CONVERGENCE_ACCEP_TOL         */   1.0e-6 ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_CONVERGENCE_TOLERANCE         */   1.0e-8 ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_DIVERGING_ITERATES_TOL		   */	IPOPT_INF,			// CET: Added 4-28-2009
	/* IPOPT_OPT_DUAL_INF_ACCEP_TOL            */   1.0e10,				// CET: Updated 4-28-2009
	/* IPOPT_OPT_DUAL_INF_TOLERANCE            */   1.0 ,				// CET: Updated 4-28-2009

#ifdef MA27_IS_AVAILABLE
	/* IPOPT_OPT_MA27_MAXIMUM_PIVOT_TOLERANCE  */   0.0001 ,			// CET: Checked 4-30-2009
	/* IPOPT_OPT_MA27_PIVOT_TOLERANCE          */   1.0e-8 ,			// CET: Checked 4-30-2009
	/* IPOPT_OPT_MA27_LIW_INIT_FACTOR		   */   5.0 ,				// CET: Added 8-22-2009
	/* IPOPT_OPT_MA27_LA_INIT_FACTOR		   */   5.0 ,				// CET: Added 8-22-2009
	/* IPOPT_OPT_MA27_MEMINC_FACTOR			   */	10.0 ,				// CET: Added 8-22-2009
#endif

#ifdef MA57_IS_AVAILABLE
	/* IPOPT_OPT_MA57_MAXIMUM_PIVOT_TOLERANCE  */   0.0001 ,			// CET: Updated 4-30-2009
	/* IPOPT_OPT_MA57_PIVOT_TOLERANCE		   */   1.0e-8 ,			// CET: Updated 4-30-2009
	/* IPOPT_OPT_MA57_PRE_ALLOC				   */	3.0 ,				// CET: Added 04-23-2010
#endif

#ifdef MUMPS_IS_AVAILABLE
	/* IPOPT_OPT_MUMPS_MAXIMUM_PIVOT_TOLERANCE */   0.1 ,				// CET: Updated 4-30-2009
	/* IPOPT_OPT_MUMPS_PIVOT_TOLERANCE		   */   1.0e-6 ,			// CET: Updated 4-30-2009
#endif
	
	/* IPOPT_OPT_NLP_SCALING_MAXIMUM_GRAD      */   100.0 ,				// CET: Checked 4-28-2009
	/* IPOPT_OPT_OBJ_CHANGE_ACCEP_TOL		   */   IPOPT_INF ,			// CET: Added 4-30-2009
	/* IPOPT_OPT_OBJECTIVE_SCALING_FACTOR      */   1.0 ,				// CET: Checked 4-28-2009
	/* IPOPT_OPT_RELATIVE_BOUND_DISTANCE       */   0.01 ,				// CET: Checked 4-28-2009
	/* IPOPT_OPT_ALPHA_FOR_Y_TOL			   */   10.0				// CET: Added 4-30-2009

#ifdef WSMP_IS_AVAILABLE
,	/* IPOPT_OPT_WSMP_MAXIMUM_PIVOT_TOLERANCE  */   0.1 ,				// CET: Updated 4-30-2009
	/* IPOPT_OPT_WSMP_PIVOT_TOLERANCE		   */   0.0001 				// CET: Updated 4-30-2009
#endif
};


// The upper bounds of the double options
static double  ipopt_dbl_opt_up[ IPOPT_OPT_DBL_MAX ] = 
{
	/* IPOPT_OPT_NLP_LOWER_BOUND_INF		   */	IPOPT_INF ,			// CET: Added 8-24-2009
	/* IPOPT_OPT_NLP_UPPER_BOUND_INF		   */	IPOPT_INF ,			// CET: Added 8-24-2009

	/* IPOPT_OPT_SLACK_BOUND_FRAC			   */ 	0.5,				// CET: Added 8-24-2009
	/* IPOPT_OPT_SLACK_BOUND_PUSH,			   */	IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_CONSTR_MULT_INIT_MAX,		   */	IPOPT_INF,			// CET: Added 8-24-2009

	/* IPOPT_OPT_MU_MAX_FACT				   */	IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_MU_MAX						   */	IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_MU_MIN						   */	IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_BARRIER_TOL_FACTOR			   */	IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_MU_LINEAR_DECREASE_FACTOR	   */	1.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_MU_SUPERLINEAR_DECREASE_POWER */	2.0,				// CET: Added 8-24-2009

	/* IPOPT_OPT_RECALC_Y_FEAS_TOL			   */	IPOPT_INF,			// CET: Added 8-24-2009

	/* IPOPT_OPT_EXPECT_INFEASIBLE_PROBLEM_CTOL			*/	IPOPT_INF,	// CET: Added 8-24-2009
	/* IPOPT_OPT_EXPECT_INFEASIBLE_PROBLEM_YTOL			*/	IPOPT_INF,
	/* IPOPT_OPT_SOFT_RESTO_PDERROR_REDUCTION_FACTOR	*/	IPOPT_INF,	// CET: Added 8-24-2009
	/* IPOPT_OPT_REQUIRED_INFEASIBILITY_REDUCTION		*/	1.0,		// CET: Added 8-24-2009
	/* IPOPT_OPT_BOUND_MULT_RESET_THRESHOLD				*/	IPOPT_INF,	// CET: Added 8-24-2009
	/* IPOPT_OPT_CONSTR_MULT_RESET_THRESHOLD			*/	IPOPT_INF,	// CET: Added 8-24-2009

	/* IPOPT_OPT_MAX_HESSIAN_PERTURBATION	   */	IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_MIN_HESSIAN_PERTURBATION	   */	IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_FIRST_HESSIAN_PERTURBATION	   */	IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_PERTURB_INC_FACT_FIRST		   */	IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_PERTURB_INC_FACT			   */	IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_PERTURB_DEC_FACT			   */	1.0,				// CET: Added 8-24-2009
	/* IPOPT_OPT_JACOBIAN_REGULARIZATION_VALUE */	IPOPT_INF,			// CET: Added 8-24-2009

	/* IPOPT_OPT_DERIVATIVE_TEST_PERTURBATION  */	IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_DERIVATIVE_TEST_TOL		   */	IPOPT_INF,			// CET: Added 8-24-2009
	/* IPOPT_OPT_POINT_PERTURBATION_RADIUS	   */	IPOPT_INF,			// CET: Added 8-24-2009

	/* IPOPT_OPT_ABSOLUTE_BOUND_DISTANCE       */   IPOPT_INF ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_BARRIER_INITIAL_VALUE         */   IPOPT_INF ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_BOUND_MULTIPLIERS_INIT_VAL    */   IPOPT_INF ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_BOUNDS_RELAXATION_FACTOR      */   IPOPT_INF ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_COMPLEMENTARITY_ACCEP_TOL     */   IPOPT_INF ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_COMPLEMENTARITY_TOLERANCE     */   IPOPT_INF ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_CONSTRAINT_VIOL_ACCEP_TOL     */   IPOPT_INF ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_CONSTRAINT_VIOL_TOLERANCE     */   IPOPT_INF ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_CONVERGENCE_ACCEP_TOL         */   IPOPT_INF ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_CONVERGENCE_TOLERANCE         */   IPOPT_INF ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_DIVERGING_ITERATES_TOL		   */	IPOPT_INF ,			// CET: Added 4-28-2009
	/* IPOPT_OPT_DUAL_INF_ACCEP_TOL            */   IPOPT_INF ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_DUAL_INF_TOLERANCE            */   IPOPT_INF ,			// CET: Checked 4-28-2009
	
#ifdef MA27_IS_AVAILABLE
	/* IPOPT_OPT_MA27_MAXIMUM_PIVOT_TOLERANCE  */   1.0 ,				// CET: Updated 4-30-2009
	/* IPOPT_OPT_MA27_PIVOT_TOLERANCE          */   1.0 ,				// CET: Updated 4-30-2009
	/* IPOPT_OPT_MA27_LIW_INIT_FACTOR		   */   IPOPT_INF ,			// CET: Added 8-22-2009
	/* IPOPT_OPT_MA27_LA_INIT_FACTOR		   */   IPOPT_INF ,			// CET: Added 8-22-2009
	/* IPOPT_OPT_MA27_MEMINC_FACTOR			   */	IPOPT_INF ,			// CET: Added 8-22-2009

#endif

#ifdef MA57_IS_AVAILABLE
	/* IPOPT_OPT_MA57_MAXIMUM_PIVOT_TOLERANCE  */   1.0 ,				// CET: Updated 4-30-2009
	/* IPOPT_OPT_MA57_PIVOT_TOLERANCE		   */   1.0 ,				// CET: Updated 4-30-2009
	/* IPOPT_OPT_MA57_PRE_ALLOC				   */	IPOPT_INF ,			// CET: Added 04-23-2010
#endif

#ifdef MUMPS_IS_AVAILABLE
	/* IPOPT_OPT_MUMPS_MAXIMUM_PIVOT_TOLERANCE */   1.0 ,				// CET: Updated 4-30-2009
	/* IPOPT_OPT_MUMPS_PIVOT_TOLERANCE		   */   1.0 ,				// CET: Updated 4-30-2009
#endif
	
	/* IPOPT_OPT_NLP_SCALING_MAXIMUM_GRAD      */   IPOPT_INF ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_OBJ_CHANGE_ACCEP_TOL		   */   IPOPT_INF ,			// CET: Added 4-30-2009
	/* IPOPT_OPT_OBJECTIVE_SCALING_FACTOR      */   IPOPT_INF ,			// CET: Checked 4-28-2009
	/* IPOPT_OPT_RELATIVE_BOUND_DISTANCE       */   0.5 ,				// CET: Checked 4-28-2009
	/* IPOPT_OPT_ALPHA_FOR_Y_TOL			   */   IPOPT_INF			// CET: Added 4-30-2009

#ifdef WSMP_IS_AVAILABLE
,	/* IPOPT_OPT_WSMP_MAXIMUM_PIVOT_TOLERANCE  */   1.0 ,				// CET: Updated 4-30-2009
	/* IPOPT_OPT_WSMP_PIVOT_TOLERANCE		   */   1.0 				// CET: Updated 4-30-2009
#endif
};



// Arrays ipopt_int_opt_ord and ipopt_dbl_opt_ord determine the alphabetical ordering
// of the options. (These arrays can be generated automatically by IPOPT_order_options.)

static int ipopt_int_opt_ord[ IPOPT_OPT_INT_MAX ];

static int ipopt_dbl_opt_ord[ IPOPT_OPT_DBL_MAX ];



// The IPOPT keywords definitions as they will be shown in AIMMS.
// Note: Keyword doesn't look nice in AIMMS if length of keyword > 37...
// Note: The underscore character "_" in the following strings is interpreted by AIMMS as a space " ".

// AIMMS keywords for Ipopt option "mu_strategy" - update strategy for Barrier parameter
static const char* aimms_keyw_bar_upd[] = 
{
    "Monotone",					// Default
    "Adaptive"
};

// The equivalent Ipopt options
static const char* ipopt_keyw_bar_upd[] = 
{
    "monotone",					// Default
    "adaptive"
};



// AIMMS keywords for Ipopt option "mu_oracle" - Oracle for a new barrier parameter in the "Adaptive" strategy.
// This option is not used when "Monotone" is selected for the update strategy.
static const char* aimms_keyw_bar_ora[] = 
{
    "Probing",
    "Loqo",
    "Quality_function"			// Default
};

// The corresponding Ipopt keywords:
static const char* ipopt_keyw_bar_ora[] = 
{
    "probing",
    "loqo",
    "quality-function"			// Default
};



// AIMMS keywords for Ipopt option "alpha_for_y" - method to determine step size for constraint multipliers
static const char* aimms_keyw_con_step[] = 
{
    "Primal",					// Default
    "Bound_multipliers",
    "Minimum_of_primal_and_bound_multipliers",
    "Maximum_of_primal_and_bound_multipliers",
    "Full",
    "Min_dual_infeasibility",
    "Safer_min_dual_infeasibility",
	"Primal_and_full",
	"Dual_and_full",
	"Acceptor"
};

// The equivalent Ipopt options
static const char* ipopt_keyw_con_step[] = 
{
    "primal",					// Default
    "bound-mult",
    "min",
    "max",
    "full",
    "min-dual-infeas",
    "safer-min-dual-infeas",
	"primal-and-full",
	"dual-and-full",
	"acceptor"
};



// AIMMS keywords for Ipopt option "corrector_type" - the type of corrector steps that should be taken.
// This is only used when "mu_strategy" is "Adaptive".
static const char* aimms_keyw_correc[] = 
{
    "None",						// Default
    "Affine",
    "Primal-dual"
};

// The corresponding Ipopt options
static const char* ipopt_keyw_correc[] = 
{
    "none",						// Default
    "affine",
    "primal-dual"
};



// AIMMS keywords for Ipopt options requiring a simple "Yes" or "No" selection by the user.
static const char* aimms_keyw_no_yes[] = 
{
    "No",
    "Yes"
};

// The corresponding Ipopt options
static const char* ipopt_keyw_no_yes[] = 
{
    "no",
    "yes"
};



// AIMMS keywords for Ipopt option "derivative_test_print_all" - indicates whether information for all estimated
// derivatives should be printed.
static const char* aimms_keyw_print_deriv[] = 
{
	"Print_only_suspect_derivatives",	// Default
	"Print_all_derivatives"
};

// The equivalent Ipopt options
static const char* ipopt_keyw_print_deriv[] = 
{
	"no",								// Default
	"yes"
};



// AIMMS keywords for Ipopt option "nlp_scaling_method" - sets the method used for scaling
static const char* aimms_keyw_scal_m[] = 
{
    "None",
//    "User_scaling",					// CET:  This option has been disabled for AIMMS, since it requires scaling factors to be 
										//		 provided by the user.

    "Gradient_based",					// Default
	"Equilibration_based"
};

// The equivalent Ipopt options
static const char* ipopt_keyw_scal_m[] = 
{
    "none",
//    "user-scaling",					// CET: This option has been disabled for AIMMS, since it requires scaling factors to be
										//		provided by the user.
    "gradient-based",					// Default
	"equilibration-based"
};



// AIMMS keywords for Ipopt option "derivative_test" - determines whether the derivative checker should be enabled
static const char* aimms_keyw_deriv_check[] =
{
	"Do_not_perform_derivative_tests",			// Default
	"Test_first_derivatives_at_starting_point",
	"Test_first_and_second_derivatives_at_starting_point"
};

// The corresponding Ipopt options
static const char* ipopt_keyw_deriv_check[] =
{
	"none",										// Default
	"first-order",
	"second-order"
};



// AIMMS keywords for Ipopt option "fixed_variable_treatment" - determines how fixed variables should be handled
static const char* aimms_keyw_fixed_var[] =
{
	"Convert_to_parameter",						// Default
	"Add_equality_constraints",
	"Relax_fixing_bound_constraints"
};

// The corresponding Ipopt options
static const char* ipopt_keyw_fixed_var[] =
{
	"make_parameter",							// Default
	"make_constraint",
	"relax_bounds"
};



// AIMMS keywords for Ipopt option "fixed_mu_oracle" - Oracle for the Barrier parameter when switching to fixed mode.
// Only considered if "adaptive" is selected for option "mu_strategy", and the "monotone" option is also selected.
static const char* aimms_keyw_bar_ora_fix[] = 
{
    "Probing",
    "Loqo",
    "Quality_function",
	"Average_complementarity"					// Default
};

// The corresponding Ipopt options
static const char* ipopt_keyw_bar_ora_fix[] = 
{
    "probing",
    "loqo",
    "quality_function",
	"average_compl"								// Default
};



// AIMMS keywords for Ipopt option "hessian_approximation" - Determines whether exact 2nd derivatives are used
// or a quasi-Newton approximation is used.
static const char* aimms_keyw_hess[] = 
{
	"Automatic",                                // Default
    "Exact",
    "Quasi-Newton"
};

// The corresponding Ipopt options
static const char* ipopt_keyw_hess[] = 
{
	"dummy",                                    // Not used
    "exact",									// Default
    "limited-memory"
};



// CET: Added 8-24-2009
// AIMMS keywords for Ipopt option "bound_mult_init_method" - initialization method for bound multipliers
static const char* aimms_keyw_bound_mult_init_method[] = 
{
    "Constant",					// Default
    "Mu-based"
};

// The equivalent Ipopt options
static const char* ipopt_keyw_bound_mult_init_method[] = 
{
    "constant",					// Default
    "mu-based"
};



// CET: Added 8-24-2009
// AIMMS keywords for Ipopt option "linear_system_scaling" - method for scaling the linear system
static const char* aimms_keyw_linear_scaling_method[] = 
{
    "None",						
    "MC19"						// Default
};

// The equivalent Ipopt options
static const char* ipopt_keyw_linear_scaling_method[] = 
{
    "none",					
    "mc19"						// Default
};



// AIMMS keywords for Ipopt option "linear_scaling_on_demand"
static const char* aimms_keyw_linear_scaling[] = 
{
    "Always",					// Default	
    "If solutions seem not good"
};



// AIMMS keywords for Ipopt option "linear_solver" - Determines which linear solver is to be used 
static const char* aimms_keyw_linear_solver[] = 
{
#ifdef MUMPS_IS_AVAILABLE
	#if defined (MA27_IS_AVAILABLE) || defined (MA57_IS_AVAILABLE) || defined (PARDISO_IS_AVAILABLE) || defined (WSMP_IS_AVAILABLE)
	"MUMPS_solver",
	#else
	"MUMPS_solver"
	#endif
#endif
	
#ifdef MA27_IS_AVAILABLE 
	#if defined (MA57_IS_AVAILABLE) || defined (PARDISO_IS_AVAILABLE) || defined (WSMP_IS_AVAILABLE)
    "HSL_routine_MA27",			// CET: 04-23-2010.  Modified from "Harwell_routine_MA27".
	#else
	"HSL_routine_MA27"			// CET: 04-23-2010.  Modified from "Harwell_routine_MA27".
	#endif
#endif

#ifdef MA57_IS_AVAILABLE
	#if defined (PARDISO_IS_AVAILABLE) || defined (WSMP_IS_AVAILABLE)
    "HSL_routine_MA57",			// CET:  04-23-2010.  Modified from "Harwell_routine_MA57".
	#else
    "HSL_routine_MA57"			// CET: 04-23-2010.  Modified from "Harwell_routine_MA57".
	#endif
#endif

#ifdef PARDISO_IS_AVAILABLE
	#if defined (WSMP_IS_AVAILABLE)
	"Pardiso_solver",
	#else
	"Pardiso_solver"
	#endif
#endif

#ifdef WSMP_IS_AVAILABLE
	"WSMP_solver"
#endif
};

// The corresponding Ipopt options
static const char* ipopt_keyw_linear_solver[] = 
{
#ifdef MUMPS_IS_AVAILABLE
	#if defined (MA27_IS_AVAILABLE) || defined (MA57_IS_AVAILABLE) || defined (PARDISO_IS_AVAILABLE) || defined (WSMP_IS_AVAILABLE)
	"mumps",
	#else
	"mumps"
	#endif
#endif

#ifdef MA27_IS_AVAILABLE 
	#if defined (MA57_IS_AVAILABLE) || defined (PARDISO_IS_AVAILABLE) || defined (WSMP_IS_AVAILABLE)
    "ma27",
	#else
	"ma27"
	#endif
#endif

#ifdef MA57_IS_AVAILABLE
	#if defined (PARDISO_IS_AVAILABLE) || defined (WSMP_IS_AVAILABLE)
    "ma57",
	#else
    "ma57"
	#endif
#endif

#ifdef PARDISO_IS_AVAILABLE
	#if defined (WSMP_IS_AVAILABLE)
	"pardiso",
	#else
	"pardiso"
	#endif
#endif

#ifdef WSMP_IS_AVAILABLE
	"wsmp"
#endif
};




// The tables of all the IPOPT options.

// Definition of the integer options. Note that the order here must be identical
// to the order of the integer options enumeration defined above in order to properly
// get and set the IPOPT options correctly.
static ipopt_option_rec  ipopt_int_options[] = 
{
	//CET: Updated integer options for Ipopt 3.7.
	
	{ IPOPT_OPT_ACCEPT_EVERY_TRIAL_STEP           , IPOPT_CAT_LINE_SEARCH    ,  0,
	  "Always_accept_full_trial_step"		      , "accept_every_trial_step"    ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            },

	{ IPOPT_OPT_BOUND_MULT_INIT_METHOD            , IPOPT_CAT_INITIALIZATION ,  0,				// CET: Added 8-24-2009
	  "Bound_multipliers_initialization_method"   , "bound_mult_init_method"     ,
	  1 , aimms_keyw_bound_mult_init_method       , ipopt_keyw_bound_mult_init_method },

	{ IPOPT_OPT_WATCHDOG_SHORTENED_ITER_TRIGGER   , IPOPT_CAT_LINE_SEARCH     , 0,				// CET: Added 8-24-2009
	  "Watchdog_shortened_iteration_trigger"      , "watchdog_shortened_iter_trigger" ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_LIMITED_MEMORY_MAX_HISTORY        , IPOPT_CAT_QUASI_NEWTON    , 0,				// CET: Added 8-24-2009
	  "Hessian_approximation_history_memory_limit", "limited_memory_max_history" ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_LIMITED_MEMORY_MAX_SKIPPING       , IPOPT_CAT_QUASI_NEWTON   ,  0,				// CET: Added 8-24-2009
	  "Hessian_approximation_successive_iterations_limit", "limited_memory_max_skipping" ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_START_WITH_RESTO                  , IPOPT_CAT_RESTORATION_PHASE, 0,				// CET: Added 8-24-2009
	  "Force_start_in_restoration_phase"          , "start_with_resto"            ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            },

	{ IPOPT_OPT_EVALUATE_ORIG_OBJ_AT_RESTO_TRIAL  , IPOPT_CAT_RESTORATION_PHASE, 0,				// CET: Added 8-24-2009
	  "Use_original_objective_function_in_restoration_phase"    , "evaluate_orig_obj_at_resto_trial"  ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            },

#ifdef MC19_IS_AVAILABLE
	{ IPOPT_OPT_LINEAR_SYSTEM_SCALING             , IPOPT_CAT_LINEAR_SOLVER    , 0,				// CET: Added 8-24-2009
	  "Linear_system_scaling_method"              , "linear_system_scaling"       ,
	  1 , aimms_keyw_linear_scaling_method        , ipopt_keyw_linear_scaling_method  },
#endif

	{ IPOPT_OPT_JAC_C_CONSTANT                    , IPOPT_CAT_NLP             ,  0,
	  "Assume_equality_constraints_are_linear"    , "jac_c_constant"              ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            },

	{ IPOPT_OPT_JAC_D_CONSTANT                    , IPOPT_CAT_NLP             ,  0,
	  "Assume_inequality_constraints_are_linear"  , "jac_d_constant"              ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            },

	{ IPOPT_OPT_HESSIAN_CONSTANT                  , IPOPT_CAT_NLP             ,  0,
	  "Assume_quadratic_problem"				  , "hessian_constant"            ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            },

	{ IPOPT_OPT_BARRIER_UPDATE_METHOD             , IPOPT_CAT_BARRIER_PARAMETER,  0,
	  "Barrier_parameter_update_strategy"         , "mu_strategy"                 ,
	  1 , aimms_keyw_bar_upd                      , ipopt_keyw_bar_upd           },

	{ IPOPT_OPT_CHECK_DERIV_NANINF                , IPOPT_CAT_NLP  			  ,  0,
	  "Check_derivatives_for_invalid_numbers"     , "check_derivatives_for_naninf" ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes             },
	
	{ IPOPT_OPT_CONSTRAINT_MULT_STEP_SIZE         , IPOPT_CAT_MULTIPLIER_UPDATES ,  0,
	  "Constraint_multipliers_step_size_method"   , "alpha_for_y"                ,
	  1 , aimms_keyw_con_step                     , ipopt_keyw_con_step          },
	
	{ IPOPT_OPT_CHECK_DERIVATIVES                 , IPOPT_CAT_DERIVATIVE_TEST ,  0,
	  "Derivative_testing"						  , "derivative_test"             ,
	  1 , aimms_keyw_deriv_check                  , ipopt_keyw_deriv_check       },
	
	{ IPOPT_OPT_BARRIER_MEHROTRA                  , IPOPT_CAT_BARRIER_PARAMETER, 0,
	  "Execute_Mehrotra_algorithm"                , "mehrotra_algorithm"          ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            },
	
	{ IPOPT_OPT_EXPECT_INFEAS_PROBLEM             , IPOPT_CAT_RESTORATION_PHASE, 0,
	  "Quickly_detect_infeasible_problem"         , "expect_infeasible_problem"   ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            },
	
	{ IPOPT_OPT_FIXED_VARIABLE_TREATMENT          , IPOPT_CAT_NLP             ,  0,
	  "Fixed_variable_handling"                   , "fixed_variable_treatment"    ,
	  1 , aimms_keyw_fixed_var                    , ipopt_keyw_fixed_var         },
	
	{ IPOPT_OPT_HESSIAN_APPROXIMATION             , IPOPT_CAT_QUASI_NEWTON    ,  0,
	  "Method_for_Hessian_computation"		      , "hessian_approximation"       ,
	  1 , aimms_keyw_hess                         , ipopt_keyw_hess              },

	{ IPOPT_OPT_LINEAR_SOLVER	                  , IPOPT_CAT_LINEAR_SOLVER   ,  0,
	  "Linear_solver_selection"					  , "linear_solver"			  	  ,
	  1 , aimms_keyw_linear_solver                , ipopt_keyw_linear_solver     },

#ifdef MC19_IS_AVAILABLE
	{ IPOPT_OPT_SCALE_ON_DEMAND	                  , IPOPT_CAT_LINEAR_SOLVER  ,   0,
	  "Linear_system_scaling"                     , "linear_scaling_on_demand"    ,
	  1 , aimms_keyw_linear_scaling               , ipopt_keyw_no_yes            },
#endif

#ifdef MA57_IS_AVAILABLE
	{ IPOPT_OPT_MA57_PIVOT_ORDER                  , IPOPT_CAT_LINEAR_SOLVER  ,	 0,		// CET: Added 04-23-2010
	  "MA57_pivot_order"					      , "ma57_pivot_order"           ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_MA57_BLOCK_SIZE                   , IPOPT_CAT_LINEAR_SOLVER  ,	 0,		// CET: Added 04-28-2010
	  "MA57_block_size"					          , "ma57_block_size"            ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_MA57_NODE_AMALGAMATION            , IPOPT_CAT_LINEAR_SOLVER  ,	 0,		// CET: Added 04-28-2010
	  "MA57_node_amalgamation_parameter"		  , "ma57_node_amalgamation" ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_MA57_SCALING_FLAG                 , IPOPT_CAT_LINEAR_SOLVER  ,	 0,		// CET: Added 04-28-2010
	  "MA57_scaling"							  , "ma57_automatic_scaling"      ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            },

	{ IPOPT_OPT_MA57_SMALL_PIVOT_FLAG             , IPOPT_CAT_LINEAR_SOLVER  ,	 0,		// CET: Added 04-28-2010
	  "MA57_small_pivot_parameter"				  , "ma57_small_pivot_flag"       ,
	  0 , NULL                                    , NULL                         },
#endif

#ifdef MUMPS_IS_AVAILABLE
	{ IPOPT_OPT_MUMPS_MEM_PERCENT                 , IPOPT_CAT_LINEAR_SOLVER  ,	 0,		// CET: Added 8-22-2009
	  "MUMPS_working_space_percentage_increase"   , "mumps_mem_percent"           ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_MUMPS_PERMUTING_SCALING           , IPOPT_CAT_LINEAR_SOLVER  ,	 0,		// CET: Added 8-22-2009
	  "MUMPS_permuting_and_scaling"               , "mumps_permuting_scaling"     ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_MUMPS_PIVOT_ORDER                 , IPOPT_CAT_LINEAR_SOLVER  ,	 0,		// CET: Added 8-22-2009
	  "MUMPS_pivot_order"					      , "mumps_pivot_order"           ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_MUMPS_SCALING                     , IPOPT_CAT_LINEAR_SOLVER  ,	 0,		// CET: Added 8-22-2009
	  "MUMPS_scaling"						      , "mumps_scaling"               ,
	  0 , NULL                                    , NULL                         },
#endif

	{ IPOPT_OPT_MAX_NUM_ITER                      , IPOPT_CAT_TERMINATION     ,  0,
	  "Maximum_number_of_iterations"              , "max_iter"                    ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_ACCEPTABLE_ITER                   , IPOPT_CAT_TERMINATION     ,  0,		// CET: Added 8-23-2009
	  "Maximum_number_of_acceptable_iterations"   , "acceptable_iter"             ,
	  0 , NULL                                    , NULL                         },
	
	{ IPOPT_OPT_MAX_NUM_REFINEMENT_STEPS          , IPOPT_CAT_LINEAR_SOLVER   ,  0,
	  "Maximum_number_of_refinement_steps"        , "max_refinement_steps"        ,
	  0 , NULL                                    , NULL                         },
	  
	{ IPOPT_OPT_MAX_WATCHDOG_ITER	              , IPOPT_CAT_LINE_SEARCH    ,   0,
	  "Maximum_number_of_watchdog_iterations"	  , "watchdog_trial_iter_max"     ,
	  0 , NULL                                    , NULL                         },
	
	{ IPOPT_OPT_MIN_NUM_REFINEMENT_STEPS          , IPOPT_CAT_LINEAR_SOLVER  ,   0,
	  "Minimum_number_of_refinement_steps"        , "min_refinement_steps"        ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_SCALING_METHOD                    , IPOPT_CAT_NLP_SCALING    ,   0,
	  "NLP_scaling_method"                        , "nlp_scaling_method"          ,
	  1 , aimms_keyw_scal_m                       , ipopt_keyw_scal_m            },
		  
	{ IPOPT_OPT_BARRIER_ORACLE_ADAPTIVE           , IPOPT_CAT_BARRIER_PARAMETER, 0,
	  "Adaptive_strategy_oracle"                  , "mu_oracle"                   ,
	  1 , aimms_keyw_bar_ora                      , ipopt_keyw_bar_ora           },
	
	{ IPOPT_OPT_BARRIER_FIXED_MU_ORACLE           , IPOPT_CAT_BARRIER_PARAMETER, 0,
	  "Fixed_mode_oracle"                         , "fixed_mu_oracle"             ,
	  1 , aimms_keyw_bar_ora_fix                  , ipopt_keyw_bar_ora_fix       },
	  
	{ IPOPT_OPT_PRINT_LEVEL                       , IPOPT_CAT_OUTPUT         ,   0,
	  "Output_verbosity_level"                    , "print_level"                 ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_PRINT_OPTIONS_DOCUMENTATION       , IPOPT_CAT_OUTPUT         ,   0,
	  "Print_all_available_algorithmic_options"   , "print_options_documentation" ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            },
	
	{ IPOPT_OPT_PRINT_DERIV						  , IPOPT_CAT_DERIVATIVE_TEST,   0,
	  "Derivative_checker_verbosity"	          , "derivative_test_print_all"   ,
	  1 , aimms_keyw_print_deriv                  , ipopt_keyw_print_deriv       },
	  
	{ IPOPT_OPT_PRINT_USER_OPTIONS                , IPOPT_CAT_OUTPUT         ,   0,
	  "Print_all_user_selected_options"           , "print_user_options"          ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            },
	
	{ IPOPT_OPT_HONOR_ORIGINAL_BOUNDS             , IPOPT_CAT_NLP            ,   0,
	  "Honor_original_bounds"                     , "honor_original_bounds"       ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            },

	{ IPOPT_OPT_BARRIER_QUALITY_FN_MAX_STEPS      , IPOPT_CAT_BARRIER_PARAMETER, 0,
	  "Quality_function_section_steps_limit"      , "quality_function_max_section_steps" ,
	  0 , NULL                                    , NULL						 },

	{ IPOPT_OPT_RECALC_MULTIPLIERS                , IPOPT_CAT_MULTIPLIER_UPDATES, 0,
	  "Recalculate_constraint_multipliers"        , "recalc_y"                    ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            },
	
	{ IPOPT_OPT_STATUS_FILE                       , IPOPT_CAT_OUTPUT         ,   0,
	  "Status_file"                               , NULL                          ,
	  1 , aimms_keyw_no_yes                       , NULL                         },

	{ IPOPT_OPT_MAX_2ND_ORDER_STEPS               , IPOPT_CAT_LINE_SEARCH    ,   0,
	  "Second_order_correction_trial_steps_limit" , "max_soc"                     ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_CORRECTOR_STEP_TYPE               , IPOPT_CAT_LINE_SEARCH    ,   0,
	  "Corrector_steps_type"                      , "corrector_type"              ,
	  1 , aimms_keyw_correc                       , ipopt_keyw_correc            }

/*	CET: Not used for now
	,
	{ IPOPT_OPT_WARM_START                        , IPOPT_CAT_WARM_START     ,   0,
	  "Use_warm_start_for_initial_point"		  , "warm_start_init_point"       ,
	  1 , aimms_keyw_no_yes                       , ipopt_keyw_no_yes            }
*/
};


// Definition of double options. Note that the order here must be identical
// to the order of the double options enumeration defined above in order to properly
// get and set the IPOPT options correctly.
static ipopt_option_rec  ipopt_dbl_options[] = 
{
	//CET: Updated double options for Ipopt 3.7.

	{ IPOPT_OPT_NLP_LOWER_BOUND_INF               , IPOPT_CAT_NLP            ,  0,				// CET: Added 8-24-2009		
	  "Minus_infinity_lower_bound"                , "nlp_lower_bound_inf"        ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_NLP_UPPER_BOUND_INF               , IPOPT_CAT_NLP            ,  0,				// CET: Added 8-24-2009		
	  "Infinity_upper_bound"                      , "nlp_upper_bound_inf"        ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_SLACK_BOUND_FRAC                  , IPOPT_CAT_INITIALIZATION ,  0,				// CET: Added 8-24-2009		
	  "Slack_to_bound_relative_distance"          , "slack_bound_frac"           ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_SLACK_BOUND_PUSH                  , IPOPT_CAT_INITIALIZATION ,  0,				// CET: Added 8-24-2009		
	  "Slack_to_bound_absolute_distance"          , "slack_bound_push"           ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_CONSTR_MULT_INIT_MAX              , IPOPT_CAT_INITIALIZATION ,  0,				// CET: Added 8-24-2009		
	  "Constraint_multipliers_initial_guess_limit", "constr_mult_init_max"       ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_MU_MAX_FACT                       , IPOPT_CAT_BARRIER_PARAMETER , 0,			// CET: Added 8-24-2009		
	  "Adaptive_strategy_factor_limit"            , "mu_max_fact"                  ,
	  0 , NULL                                    , NULL                          },

	{ IPOPT_OPT_MU_MAX                            , IPOPT_CAT_BARRIER_PARAMETER , 0,			// CET: Added 8-24-2009		
	  "Maximum_value_for_barrier_parameter"       , "mu_max"                      ,
	  0 , NULL                                    , NULL                          },

	{ IPOPT_OPT_MU_MIN                            , IPOPT_CAT_BARRIER_PARAMETER , 0,			// CET: Added 8-24-2009		
	  "Minimum_value_for_barrier_parameter"       , "mu_min"                      ,
	  0 , NULL                                    , NULL                          },

	{ IPOPT_OPT_BARRIER_TOL_FACTOR                , IPOPT_CAT_BARRIER_PARAMETER , 0,			// CET: Added 8-24-2009		
	  "Barrier_convergence_tolerance_factor"      , "barrier_tol_factor"          ,
	  0 , NULL                                    , NULL                          },

	{ IPOPT_OPT_MU_LINEAR_DECREASE_FACTOR         , IPOPT_CAT_BARRIER_PARAMETER , 0,			// CET: Added 8-24-2009		
	  "Linear_decrease_factor_barrier_parameter"  , "mu_linear_decrease_factor"    ,
	  0 , NULL                                    , NULL                           },

	{ IPOPT_OPT_MU_SUPERLINEAR_DECREASE_POWER     , IPOPT_CAT_BARRIER_PARAMETER , 0,			// CET: Added 8-24-2009		
	  "Superlinear_decrease_rate_barrier_parameter", "mu_superlinear_decrease_power" ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_RECALC_Y_FEAS_TOL                 , IPOPT_CAT_MULTIPLIER_UPDATES , 0,			// CET: Added 8-24-2009
	  "Recalculate_constraint_multipliers_tolerance" , "recalc_y_feas_tol"          ,
	  0 , NULL									  , NULL                         },

	{ IPOPT_OPT_EXPECT_INFEASIBLE_PROBLEM_CTOL    , IPOPT_CAT_RESTORATION_PHASE , 0,			// CET: Added 8-24-2009
	  "Minimum_violation_infeasible_problem"      , "expect_infeasible_problem_ctol" ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_EXPECT_INFEASIBLE_PROBLEM_YTOL    , IPOPT_CAT_RESTORATION_PHASE , 0,
	  "Maximum_multipliers_infeasible_problem"    , "expect_infeasible_problem_ytol" ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_SOFT_RESTO_PDERROR_REDUCTION_FACTOR, IPOPT_CAT_RESTORATION_PHASE , 0,			// CET: Added 8-24-2009
	  "Reduction_factor_primal_dual_error"        , "soft_resto_pderror_reduction_factor" ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_REQUIRED_INFEASIBILITY_REDUCTION  , IPOPT_CAT_RESTORATION_PHASE ,  0,			// CET: Added 8-24-2009
	  "Required_infeasibility_reduction"          , "required_infeasibility_reduction"  ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_BOUND_MULT_RESET_THRESHOLD        , IPOPT_CAT_RESTORATION_PHASE ,  0,			// CET: Added 8-24-2009
	  "Bound_multipliers_reset_threshold"         , "bound_mult_reset_threshold"    ,
	  0 , NULL                                    , NULL                            },

	{ IPOPT_OPT_CONSTR_MULT_RESET_THRESHOLD       , IPOPT_CAT_RESTORATION_PHASE ,  0,			// CET: Added 8-24-2009
	  "Constraint_multipliers_reset_threshold"    , "constr_mult_reset_threshold"   ,
	  0 , NULL                                    , NULL                            },

	{ IPOPT_OPT_MAX_HESSIAN_PERTURBATION          , IPOPT_CAT_HESSIAN_PERTURBATION ,  0,			// CET: Added 8-24-2009
	  "Maximum_Hessian_perturbation"              , "max_hessian_perturbation"    ,
	  0 , NULL                                    , NULL                          },

	{ IPOPT_OPT_MIN_HESSIAN_PERTURBATION          , IPOPT_CAT_HESSIAN_PERTURBATION ,  0,			// CET: Added 8-24-2009
	  "Minimum_Hessian_perturbation"              , "min_hessian_perturbation"    ,
	  0 , NULL                                    , NULL                          },

	{ IPOPT_OPT_FIRST_HESSIAN_PERTURBATION        , IPOPT_CAT_HESSIAN_PERTURBATION ,  0,			// CET: Added 8-24-2009
	  "First_Hessian_perturbation_size"           , "first_hessian_perturbation"  ,
	  0 , NULL                                    , NULL                          },

	{ IPOPT_OPT_PERTURB_INC_FACT_FIRST            , IPOPT_CAT_HESSIAN_PERTURBATION ,  0,			// CET: Added 8-24-2009
	  "First_Hessian_perturbation_increase_factor", "perturb_inc_fact_first"      ,
	  0 , NULL                                    , NULL                          },

	{ IPOPT_OPT_PERTURB_INC_FACT                  , IPOPT_CAT_HESSIAN_PERTURBATION ,  0,			// CET: Added 8-24-2009
	  "Hessian_perturbation_increase_factor"      , "perturb_inc_fact"            ,
	  0 , NULL                                    , NULL                          },

	{ IPOPT_OPT_PERTURB_DEC_FACT                  , IPOPT_CAT_HESSIAN_PERTURBATION ,  0,			// CET: Added 8-24-2009
	  "Hessian_perturbation_decrease_factor"      , "perturb_dec_fact"            ,
	  0 , NULL                                    , NULL                          },

	{ IPOPT_OPT_JACOBIAN_REGULARIZATION_VALUE     , IPOPT_CAT_HESSIAN_PERTURBATION ,  0,			// CET: Added 8-24-2009
	  "Jacobian_regularization_value"             , "jacobian_regularization_value"  ,
	  0 , NULL                                    , NULL                            },

	{ IPOPT_OPT_DERIVATIVE_TEST_PERTURBATION      , IPOPT_CAT_DERIVATIVE_TEST ,  0,					// CET: Added 8-24-2009
	  "Derivative_test_perturbation_size"         , "derivative_test_perturbation"  ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_DERIVATIVE_TEST_TOL               , IPOPT_CAT_DERIVATIVE_TEST ,  0,					// CET: Added 8-24-2009
	  "Derivative_test_tolerance"                 , "derivative_test_tol"         ,
	  0 , NULL                                    , NULL                          },

	{ IPOPT_OPT_POINT_PERTURBATION_RADIUS         , IPOPT_CAT_DERIVATIVE_TEST ,  0,					// CET: Added 8-24-2009
	  "Maximum_perturbation_of_evaluation_point"  , "point_perturbation_radius"   ,
	  0 , NULL                                    , NULL                          },

	{ IPOPT_OPT_ABSOLUTE_BOUND_DISTANCE           , IPOPT_CAT_INITIALIZATION ,  0,		
	  "Point_to_bound_absolute_distance"          , "bound_push"                 ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_BARRIER_INITIAL_VALUE             , IPOPT_CAT_BARRIER_PARAMETER ,  0,		
	  "Barrier_parameter_initial_value"           , "mu_init"                    ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_BOUND_MULTIPLIERS_INIT_VAL        , IPOPT_CAT_INITIALIZATION ,  0,		
	  "Initial_value_for_bound_multipliers"       , "bound_mult_init_val"        ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_BOUNDS_RELAXATION_FACTOR          , IPOPT_CAT_NLP            ,  0,		
	  "Factor_for_initial_bounds_relaxation"      , "bound_relax_factor"         ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_COMPLEMENTARITY_ACCEP_TOL         , IPOPT_CAT_TERMINATION    ,  0,		
	  "Acceptable_complementarity_tolerance"      , "acceptable_compl_inf_tol"   ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_COMPLEMENTARITY_TOLERANCE         , IPOPT_CAT_TERMINATION    ,  0,		
	  "Complementarity_tolerance"                 , "compl_inf_tol"       ,
	  0 , NULL                                    , NULL                         },
	
	{ IPOPT_OPT_CONSTRAINT_VIOL_ACCEP_TOL         , IPOPT_CAT_TERMINATION    ,  0,		
	  "Acceptable_constraint_violation_tolerance" , "acceptable_constr_viol_tol" ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_CONSTRAINT_VIOL_TOLERANCE         , IPOPT_CAT_TERMINATION    ,  0,		
	  "Constraint_violation_tolerance"            , "constr_viol_tol"            ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_CONVERGENCE_ACCEP_TOL             , IPOPT_CAT_TERMINATION    ,  0,		
	  "Acceptable_relative_convergence_tolerance" , "acceptable_tol"             ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_CONVERGENCE_TOLERANCE             , IPOPT_CAT_TERMINATION    ,  0,		
	  "Relative_convergence_tolerance"            , "tol"                        ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_DIVERGING_ITERATES_TOL            , IPOPT_CAT_TERMINATION    ,  0,		
	  "Diverging_iterates_tolerance"	          , "diverging_iterates_tol"     ,
	  0 , NULL                                    , NULL                         },
	
	{ IPOPT_OPT_DUAL_INF_ACCEP_TOL                , IPOPT_CAT_TERMINATION    ,  0,		
	  "Acceptable_dual_infeasibility_tolerance"   , "acceptable_dual_inf_tol"    ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_DUAL_INF_TOLERANCE                , IPOPT_CAT_TERMINATION    ,  0,		
	  "Dual_infeasibility_tolerance"              , "dual_inf_tol"               ,
	  0 , NULL                                    , NULL                         },

#ifdef MA27_IS_AVAILABLE
	{ IPOPT_OPT_MA27_MAXIMUM_PIVOT_TOLERANCE      , IPOPT_CAT_LINEAR_SOLVER  ,  0,		
	  "MA27_maximum_pivot_tolerance"              , "ma27_pivtolmax"             ,
	  0 , NULL                                    , NULL                         },
	
	{ IPOPT_OPT_MA27_PIVOT_TOLERANCE              , IPOPT_CAT_LINEAR_SOLVER  ,  0,		
	  "MA27_pivot_tolerance"                      , "ma27_pivtol"                ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_MA27_LIW_INIT_FACTOR			  , IPOPT_CAT_LINEAR_SOLVER   ,  0,		// CET: Added 8-22-2009		
	  "MA27_integer_workspace_memory"             , "ma27_liw_init_factor"       ,
	  0 , NULL                                    , NULL                         },
	
	{ IPOPT_OPT_MA27_LA_INIT_FACTOR               , IPOPT_CAT_LINEAR_SOLVER   ,  0,		// CET: Added 8-22-2009		
	  "MA27_real_workspace_memory"                , "ma27_la_init_factor"        ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_MA27_MEMINC_FACTOR                , IPOPT_CAT_LINEAR_SOLVER   ,  0,		// CET: Added 8-22-2009		
	  "MA27_increment_factor_for_workspace_size"  , "ma27_meminc_factor"         ,
	  0 , NULL                                    , NULL                         },
#endif

#ifdef MA57_IS_AVAILABLE
	{ IPOPT_OPT_MA57_MAXIMUM_PIVOT_TOLERANCE      , IPOPT_CAT_LINEAR_SOLVER  ,  0,		
	  "MA57_maximum_pivot_tolerance"              , "ma57_pivtolmax"             ,
	  0 , NULL                                    , NULL                         },
	
	{ IPOPT_OPT_MA57_PIVOT_TOLERANCE			  , IPOPT_CAT_LINEAR_SOLVER  ,  0,		
	  "MA57_pivot_tolerance"					  , "ma57_pivtol"                ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_MA57_PRE_ALLOC					  , IPOPT_CAT_LINEAR_SOLVER  ,  0,		// CET: Added 04-23-2010
	  "MA57_work_space_memory_safety_factor"	  , "ma57_pre_alloc"             ,
	  0 , NULL                                    , NULL                         },
#endif

#ifdef MUMPS_IS_AVAILABLE
	{ IPOPT_OPT_MUMPS_MAXIMUM_PIVOT_TOLERANCE     , IPOPT_CAT_LINEAR_SOLVER  ,  0,		
	  "MUMPS_maximum_pivot_tolerance"             , "mumps_pivtolmax"            ,
	  0 , NULL                                    , NULL                         },
	
	{ IPOPT_OPT_MUMPS_PIVOT_TOLERANCE			  , IPOPT_CAT_LINEAR_SOLVER  ,  0,		
	  "MUMPS_pivot_tolerance"					  , "mumps_pivtol"               ,
	  0 , NULL                                    , NULL                         },
#endif
	
	{ IPOPT_OPT_NLP_SCALING_MAXIMUM_GRAD          , IPOPT_CAT_NLP_SCALING    ,  0,		
	  "Maximum_gradient_after_NLP_scaling"        , "nlp_scaling_max_gradient"   ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_OBJ_CHANGE_ACCEP_TOL			  , IPOPT_CAT_TERMINATION    ,  0,		
	  "Acceptable_objective_change_tolerance"     , "acceptable_obj_change_tol"  ,
	  0 , NULL                                    , NULL                         },
	
	{ IPOPT_OPT_OBJECTIVE_SCALING_FACTOR          , IPOPT_CAT_NLP_SCALING    ,  0,		
	  "Objective_function_scaling_factor"         , "obj_scaling_factor"         ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_RELATIVE_BOUND_DISTANCE           , IPOPT_CAT_INITIALIZATION ,  0,		
	  "Point_to_bound_relative_distance"          , "bound_frac"                 ,
	  0 , NULL                                    , NULL                         },

	{ IPOPT_OPT_ALPHA_FOR_Y_TOL			          , IPOPT_CAT_MULTIPLIER_UPDATES ,  0,		
	  "Equality_multipliers_switch_tolerance"     , "alpha_for_y_tol"            ,
	  0 , NULL                                    , NULL                         }

#ifdef WSMP_IS_AVAILABLE
	,
	{ IPOPT_OPT_WSMP_MAXIMUM_PIVOT_TOLERANCE      , IPOPT_CAT_LINEAR_SOLVER  ,  0,		
	  "WSMP_maximum_pivot_tolerance"              , "wsmp_pivtolmax"             ,
	  0 , NULL                                    , NULL                         },
	
	{ IPOPT_OPT_WSMP_PIVOT_TOLERANCE			  , IPOPT_CAT_LINEAR_SOLVER  ,  0,		
	  "WSMP_pivot_tolerance"					  , "wsmp_pivtol"                ,
	  0 , NULL                                    , NULL                         }
#endif
};



//@< The function |IPOPT_order_options()| @> @;
void IpoptSolverInfo::IPOPT_order_options( void )
{
	int   i;
	
	// Determine the alphabetical ordering of the options and store it in the arrays
	// ipopt_int_opt_ord and ipopt_dbl_opt_ord. The algorithm used is not the most
	// efficient one but will not take much execution time anyway.
	
	// If the arrays ipopt_int_opt_ord and ipopt_dbl_opt_ord are "known" then you
	// can initialize the arrays and skip the ordering algorithm by undefining
	// RUN_ALPHABETICAL_ORDERING_ALG. The arrays can be generated automatically once
	// by temporary setting
	//    #define PRINT_OPTION_ORDERING
	// and building libAimmsIpopt.dll. When opening AIMMS the two arrays will be
	// written to file 'ipopt_options_ordering.log'. The contents of that file should
	// be copied into this file, namely at the location were ipopt_int_opt_ord and
	// ipopt_dbl_opt_ord are defined. After that, undefine PRINT_OPTION_ORDERING
	// and RUN_ALPHABETICAL_ORDERING_ALG and build libAimmsIpopt.dll again.

#define RUN_ALPHABETICAL_ORDERING_ALG
#ifdef RUN_ALPHABETICAL_ORDERING_ALG
	int     j, best_opt_ind;
	bool    best_opt_is_int, handled_int[IPOPT_OPT_INT_MAX], handled_dbl[IPOPT_OPT_DBL_MAX];
	const char *best_opt;

	memset( handled_int, 0, IPOPT_OPT_INT_MAX * sizeof(bool) );
	memset( handled_dbl, 0, IPOPT_OPT_DBL_MAX * sizeof(bool) );

	for ( i=0; i<IPOPT_OPT_INT_MAX + IPOPT_OPT_DBL_MAX; i++ ) {
		best_opt_ind = -1;
		
		for ( j=0; j<IPOPT_OPT_INT_MAX; j++ ) {
			if ( handled_int[ j ] == 0 ) {
			    if ( best_opt_ind == -1                                         ||
			         STRICMP( ipopt_int_options[ j ].aimms_name, best_opt ) < 0 ) {
			        best_opt_ind = j;
			        best_opt_is_int = true;
			        best_opt = ipopt_int_options[ j ].aimms_name;
			    }
			}
		}
		
		for ( j=0; j<IPOPT_OPT_DBL_MAX; j++ ) {
			if ( handled_dbl[ j ] == 0 ) {
			    if ( best_opt_ind == -1                                         ||
			         STRICMP( ipopt_dbl_options[ j ].aimms_name, best_opt ) < 0 ) {
			        best_opt_ind = j;
			        best_opt_is_int = false;
			        best_opt = ipopt_dbl_options[ j ].aimms_name;
			    }
			}
		}
		
		assert( best_opt_ind >= 0 );
		
		if ( best_opt_is_int ) {
			ipopt_int_opt_ord[ best_opt_ind ] = i;
			handled_int[ best_opt_ind ] = 1;
		} else {
			ipopt_dbl_opt_ord[ best_opt_ind ] = i;
			handled_dbl[ best_opt_ind ] = 1;
		}
	}
#endif // RUN_ALPHABETICAL_ORDERING_ALG
	
#ifdef PRINT_OPTION_ORDERING
	int   cnt;

	FILE *f = fopen( "ipopt_options_ordering.log", "w" );
	if ( f ) {
		fprintf( f, "static int ipopt_int_opt_ord[ IPOPT_OPT_INT_MAX ] =\n{" );
		
		cnt = 0;
		for ( j=0; j<IPOPT_OPT_INT_MAX; j++ ) {
			fprintf( f, " %2d", ipopt_int_opt_ord[j] );
			
			if ( j == IPOPT_OPT_INT_MAX - 1 ) {
				fprintf( f, " };\n" );
			} else if ( cnt == 9 ) {
				fprintf( f, ",\n " );
				cnt = 0;
			} else {
				fprintf( f, ", " );
				cnt ++;
			}
		}
		
		fprintf( f, "\nstatic int ipopt_dbl_opt_ord[ IPOPT_OPT_DBL_MAX ] =\n{" );
		
		cnt = 0;
		for ( j=0; j<IPOPT_OPT_DBL_MAX; j++ ) {
			fprintf( f, " %2d", ipopt_dbl_opt_ord[j] );
			
			if ( j == IPOPT_OPT_DBL_MAX - 1 ) {
				fprintf( f, " };\n" );
			} else if ( cnt == 9 ) {
				fprintf( f, ",\n " );
				cnt = 0;
			} else {
				fprintf( f, ", " );
				cnt ++;
			}
		}
		
		fclose( f );
	}
#endif // PRINT_OPTION_ORDERING

	for ( i=0; i<IPOPT_OPT_INT_MAX; i++ ) {
		ipopt_int_options[ i ] . order = ipopt_int_opt_ord[ i ];
	}
	
	for ( i=0; i<IPOPT_OPT_DBL_MAX; i++ ) {
		ipopt_dbl_options[ i ] . order = ipopt_dbl_opt_ord[ i ];
	}
	
	ipopt_options_ordered = true;
}


//@< The function |IPOPT_Init_Global_Options()| @> @;
void IpoptMathProgramInstance::IPOPT_Init_Global_Options( void )
{
	// Set global option variables equal to their corresponding AIMMS default value.
	
	ipopt_opt_sol_progress = 100;
    ipopt_opt_time_limit   = IPOPT_MAXINT;
    ipopt_opt_priority     = AOSI_PRIO_ALWAYS;
    ipopt_opt_dom_limit    = 0;
#ifdef DEBUG
    ipopt_opt_tracing      = 0;
#endif
}


//@< The function |IPOPT_Init_Options()| @> @;
void IpoptMathProgramInstance::IPOPT_Init_Options( void )
{
	// Set options to default values as defined above
    memmove( ipopt_int_opt_val, ipopt_int_opt_def, IPOPT_OPT_INT_MAX* sizeof( int ) );
    memmove( ipopt_dbl_opt_val, ipopt_dbl_opt_def, IPOPT_OPT_DBL_MAX* sizeof( double ) );
    
    // Set global option variables.

	IPOPT_Init_Global_Options();
}


//@< The function |IPOPT_Get_Options()| @> @;
void IpoptMathProgramInstance::IPOPT_Get_Options( void )
{
    _LONG_T  i, index, no_int, no_dbl,
             ind_int[ AOSI_IOPT_MAX + IPOPT_OPT_INT_MAX ],
             ind_dbl[ AOSI_DOPT_MAX + IPOPT_OPT_DBL_MAX ],
             opt_int[ AOSI_IOPT_MAX + IPOPT_OPT_INT_MAX ];

    double   opt_dbl[ AOSI_DOPT_MAX + IPOPT_OPT_DBL_MAX ];
        
    // Get modified global solver and IPOPT specific options. Global options have a negative
	// index and will be processed before IPOPT specific options.
	
    m_mp->GetChangedOptions( &no_int, ind_int, opt_int, &no_dbl, ind_dbl, opt_dbl );
    
    // Variable no_int contains the actual number of modified integer options.
    
    for ( i=0; i<no_int; i++ ) {
        index = ind_int[ i ];

        if ( index >= 0 ) {
			// Store IPOPT specific integer option.
			
            ipopt_int_opt_val[ index ] = opt_int[ i ];
        } else {
			// Store global integer AIMMS option.
			
            switch ( index ) {
              case AOSI_IOPT_iteration_limit:
              	// This value represents the iteration limit. It is NOT mapped to an IPOPT-specific
			    // option (e.g., option 'max_iter') because the AIMMS default of MAXINT is too
			    // large for IPOPT.
                break;
              
			  case AOSI_IOPT_time_limit:
			  	// This value represents the time limit. It is mapped to the IPOPT-specific
			    // option 'max_cpu_time'.
			    ipopt_opt_time_limit = opt_int[ i ];
			    if ( ipopt_opt_time_limit > IPOPT_MAXINT ) {
                    ipopt_opt_time_limit = IPOPT_MAXINT;
                }
                break;

              case AOSI_IOPT_progress_solution:
				// This value represents the number of iterations that must be completed
			    // before the progress window is updated in AIMMS.
                ipopt_opt_sol_progress = opt_int[ i ];
                break;

              case AOSI_IOPT_max_domain_errors:
			    // CET: We set this, but it is not currently implemented as an option in IPOPT.
                ipopt_opt_dom_limit = opt_int[ i ];
                break;

              case AOSI_IOPT_solver_messages:
                ipopt_opt_priority = opt_int[ i ];
                if ( ipopt_opt_priority == AOSI_PRIO_ALWAYS ) {
                    ipopt_opt_priority = 100;        // Pass all messages to AIMMS.
				}
                break;
#ifdef DEBUG
              case AOSI_IOPT_solver_tracing:
                ipopt_opt_tracing = opt_int[i];
                break;
#endif

              default:
                // Other global integer AIMMS options are not used.
                break;
            }
        }
    }

	// Variable no_dbl contains the actual number of modified double options.

    for ( i=0; i<no_dbl; i++ ) {
        index = ind_dbl[ i ];

        if ( index >= 0 ) {
			// Store IPOPT specific double option.
			
            ipopt_dbl_opt_val[ index ] = opt_dbl[ i ];
        } else {
			// Store global double AIMMS option.
			// No global double AIMMS options are currently used by IPOPT.
        }
    }
}


//@< The function |IPOPT_Set_Options()| @> @;
int IpoptMathProgramInstance::IPOPT_Set_Options( Ipopt::SmartPtr<Ipopt::IpoptApplication>& app )
{
	int       i, nValue;
	double    dValue;
	bool      bReturn = true;
	char      file_name[1024];
#ifndef _AIMMS390_
	char      dir[MAX_DIR_LEN];
	_LONG_T   len;
#endif
	
	// Set value for constraint violation tolerance. This makes the value of global variable 
	// ipopt_opt_constr_viol_tol available to code in IpoptProblemInstance
	ipopt_opt_constr_viol_tol = ipopt_dbl_opt_val[ IPOPT_OPT_CONSTRAINT_VIOL_TOLERANCE ];
	
	
	// The default setting of 'Automatic' for option 'Hessian approximation'
	// is not available in IPOPT. Depending on whether the Hessian is available
	// or not, the value is translated to exact or Quasi-Newton/limited-memory.
	// The Hessian is not available in AIMMS if one of the constraints contains
	// a call to an external function.
	
	if ( ipopt_int_opt_val[IPOPT_OPT_HESSIAN_APPROXIMATION] == 0 ) {   // Automatic
		if ( IPOPT_handle.hess_present ) {
			ipopt_int_opt_val[IPOPT_OPT_HESSIAN_APPROXIMATION] = 1;    // Exact
		} else {
			ipopt_int_opt_val[IPOPT_OPT_HESSIAN_APPROXIMATION] = 2;    // Quasi-Newton/limited-memory
		}
	}
	
	
	// If all variables are fixed then IPOPT might run into all kind of trouble
	// especially during the "finalize solution call" (bug 23219). Changing the
	// way fixed variables are treated seems to help.
	
	if ( IPOPT_handle.ncols_fix >= IPOPT_handle.ncols ) {
		ipopt_int_opt_val[IPOPT_OPT_FIXED_VARIABLE_TREATMENT] = 1;
		//bReturn = app->Options()->SetStringValue( "skip_finalize_solution_call", "yes" );
	}
	
	
	// Loop through the integer options and see which ones have been changed by the user.
	
	for ( i=0; i<IPOPT_OPT_INT_MAX; i++ ) {
		// We skip options for which ipopt_name is NULL since these are no IPOPT options.
		
		if ( ipopt_int_options[i].ipopt_name              &&
             ipopt_int_opt_val[i] != ipopt_int_opt_def[i] ) {
		
			// The option has been changed by the user. Here we need to make a distinction
			// between integer options and string options. An integer option will not have
			// any keywords associated with it.
			nValue = ipopt_int_opt_val[ i ];
			
			// If no keywords are present, this is an integer option
			if ( ipopt_int_options[ i ].keywords == 0 ) {
				// Set the updated integer value of the IPOPT option
				bReturn = app->Options()->SetIntegerValue( ipopt_int_options[ i ].ipopt_name, nValue );
			} else {
				// Otherwise, set the corresponding string option
				bReturn = app->Options()->SetStringValue( ipopt_int_options[ i ].ipopt_name, ipopt_int_options[ i ].ipopt_keyword[ nValue ] );
			}
		}
		
		if ( bReturn == false ) {
			// An error occurred while setting an option
			sprintf( IPOPT_msg, "Error while setting option \'%s\'", ipopt_int_options[ i ].ipopt_name );
			IPOPT_Error( IPOPT_msg );
			return 1;
		}
	}
	
	
	// Loop through the double options and see which ones have been changed by the user.
	
	for ( i=0; i<IPOPT_OPT_DBL_MAX; i++ ) {
		// We skip options for which ipopt_name is NULL since these are no IPOPT options.
    	
        if ( ipopt_dbl_options[i].ipopt_name              &&
             ipopt_dbl_opt_val[i] != ipopt_dbl_opt_def[i] ) {
			// The option has been changed by the user, so retrieve the updated value.  
			dValue = ipopt_dbl_opt_val[ i ];
			
			// Set the updated double value of the IPOPT option
			bReturn = app->Options()->SetNumericValue( ipopt_dbl_options[ i ].ipopt_name, dValue );
		}
		
		if ( bReturn == false ) {
			// An error occurred while setting an option
			sprintf( IPOPT_msg, "Error while setting option \'%s\'", ipopt_dbl_options[ i ].ipopt_name );
			IPOPT_Error( IPOPT_msg );
			return 1;
		}
	}
	
	
	// Set AIMMS general solvers option 'time limit' by mapping it to the IPOPT specific
	// option 'max_cpu_time'.
	
	if ( ipopt_opt_time_limit >= IPOPT_MAXINT ) {
    	dValue = IPOPT_INF;
    } else {
    	dValue = (double) ipopt_opt_time_limit;
    }
    
    bReturn = app->Options()->SetNumericValue( "max_cpu_time", dValue );
    if ( bReturn == false ) {
		// An error occurred while setting the 'max_cpu_time' option
		sprintf( IPOPT_msg, "Error while setting option \'time limit\'" );
		IPOPT_Error( IPOPT_msg );
		return 1;
	}
	
	
	// Let IPOPT generate a log file if the option 'status file' is switched on.
	
	if ( ipopt_int_opt_val[IPOPT_OPT_STATUS_FILE] ) {
#ifdef _AIMMS390_
    	sprintf( file_name, "%s", IPOPT_STATUS_FILE_NAME );
    	
    	bReturn = app->Options()->SetStringValue( "output_file", file_name );
		if ( bReturn == false ) {
			// An error occurred while setting the 'output_file' option
			sprintf( IPOPT_msg, "Error while setting option \'status file\'" );
			IPOPT_Error( IPOPT_msg );
			return 1;
		}
#else
		len = MAX_DIR_LEN;

        m_gen->GetLogDirName( dir, &len );
    	    
    	if ( len >= 0 ) {
    	    sprintf( file_name, "%s/%s", dir, IPOPT_STATUS_FILE_NAME );
    	
			bReturn = app->Options()->SetStringValue( "output_file", file_name );
			if ( bReturn == false ) {
				// An error occurred while setting the 'output_file' option
				sprintf( IPOPT_msg, "Error while setting option \'status file\'" );
				IPOPT_Error( IPOPT_msg );
				return 1;
			}
		}
#endif
	}
	
	return 0;   // No errors
}


//@< The function |GetIntegerOption()| @> @;
void IpoptSolverInfo::GetIntegerOption( _LONG_T int_option_no, char* name, _LONG_T* values, _LONG_T* optinfo )
{
	// Get an integer option. We strongly recommend to use the framework of
    // option definitions as given at the beginning of this file.
    
    strcpy( name, ipopt_int_options[ int_option_no ].aimms_name );
	
    values[ AOSI_OPTION_LO      ]    = ipopt_int_opt_low[ int_option_no ];
    values[ AOSI_OPTION_DEFAULT ]    = ipopt_int_opt_def[ int_option_no ];
    values[ AOSI_OPTION_UP      ]    = ipopt_int_opt_up[  int_option_no ];
    values[ AOSI_OPTION_CURRENT ]    = ipopt_int_opt_def[ int_option_no ];
	
    optinfo[ AOSI_OPTINFO_CAT   ]    = ipopt_int_options[ int_option_no ].category;
    optinfo[ AOSI_OPTINFO_INDEX    ] = ipopt_int_options[ int_option_no ].order;
    optinfo[ AOSI_OPTINFO_CHANGE   ] = 0;
    optinfo[ AOSI_OPTINFO_KEYWORDS ] = ipopt_int_options[ int_option_no ].keywords;
}


//@< The function |GetIntegerKeyword()| @> @;
void IpoptSolverInfo::GetIntegerKeyword(
    _LONG_T  int_option_no,
    _LONG_T  index,
    char    *keyword )          // keyword must be of length <  37
{
    // Get name of keyword to 'index' for integer option with number 'int_option_no'.
    // We strongly recommend to use the framework of option definitions as given at
    // the beginning of this file.
    
    if ( int_option_no >= IPOPT_OPT_INT_MAX ) {
        m_gen->PassMessage( AOSI_PRIO_ERROR,
                            (char *)"Invalid integer option number encountered" );
        return;
    }

    if ( ipopt_int_options[ int_option_no ].keywords ) {
        strcpy( keyword, ipopt_int_options[ int_option_no ].keyword[ index ] );
    } else {
        keyword[ 0 ] = '\0';
    }
}


//@< The function |GetDoubleOption()| @> @;
void IpoptSolverInfo::GetDoubleOption(
    _LONG_T  dbl_option_no,   // dbl_option_no, starting at 0
    char    *name,            // name must be of length < 256
    double  *values,
    _LONG_T *info )
{
    // Get a double option. We strongly recommend to use the framework of
    // option definitions as given at the beginning of this file.
    
    strcpy( name, ipopt_dbl_options[ dbl_option_no ].aimms_name );

    values[ AOSI_OPTION_LO      ] = ipopt_dbl_opt_low[ dbl_option_no ];
    values[ AOSI_OPTION_DEFAULT ] = ipopt_dbl_opt_def[ dbl_option_no ];
    values[ AOSI_OPTION_UP      ] = ipopt_dbl_opt_up[  dbl_option_no ];
    values[ AOSI_OPTION_CURRENT ] = ipopt_dbl_opt_def[ dbl_option_no ];

    info[ AOSI_OPTINFO_CAT      ] = ipopt_dbl_options[ dbl_option_no ].category;
    info[ AOSI_OPTINFO_INDEX    ] = ipopt_dbl_options[ dbl_option_no ].order;
    info[ AOSI_OPTINFO_CHANGE   ] = 0;
}


//@< The function |GetCategoryName()| @> @;
void IpoptSolverInfo::GetCategoryName(
    _LONG_T  category,          // category number, starting at 0
    char    *name )             // name must be of length < 256
{
    // Get a category. We strongly recommend to use the framework of option
    // definitions as given at the beginning of this file.
    
    strcpy( name, ipopt_category[ category ] );	
}

