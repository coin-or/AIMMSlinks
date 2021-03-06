// Copyright (C) 2009 AIMMS B.V. and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// $Id$
//
// Authors: Yidong Lang, L.T. Biegler, Chuck Teeter, Marcel Hunting

#ifndef _DEFINED_IPOPT_OPTIONS_H
#define _DEFINED_IPOPT_OPTIONS_H

/* Creates category IPOPT in the 'Project Options' with IPOPT_CAT_MAX subcategories:

     - Specific solvers
          - IPOPT
               - Output
                    - Assume equality constraints are linear
                    - Assume inequality constraints are linear
                    - Assume quadratic problem
                    - etc
               - Termination
                    - Complementarity acceptance tolerance
                    - Constraint violation acceptance tolerance
                    - etc
               - NLP scaling
                    - Barrier update strategy
                    - Execute Mehrotra algorithm
                    - etc
               - etc
*/


// The AIMMS option categories; there should be at least 1!

enum {
	IPOPT_CAT_BARRIER_PARAMETER,
	IPOPT_CAT_HESSIAN_PERTURBATION,
	IPOPT_CAT_INITIALIZATION,
	IPOPT_CAT_LINE_SEARCH,
	IPOPT_CAT_LINEAR_SOLVER,
	IPOPT_CAT_MULTIPLIER_UPDATES,
	IPOPT_CAT_NLP,
	IPOPT_CAT_NLP_SCALING,
	IPOPT_CAT_OUTPUT,
	IPOPT_CAT_QUASI_NEWTON,
	IPOPT_CAT_RESTORATION_PHASE,
	IPOPT_CAT_TERMINATION,
	
	IPOPT_CAT_DERIVATIVE_TEST,
	
// CET 8-23-2009: Warm start options not yet supported
//	IPOPT_CAT_WARM_START,

	// This identifier must always be at the end of the list,
	// as it sets the number of categories
	IPOPT_CAT_MAX

};


// The integer options that will be available from within AIMMS

enum {
// CET: Updated integer options for Ipopt 3.7
	IPOPT_OPT_ACCEPT_EVERY_TRIAL_STEP,

	IPOPT_OPT_BOUND_MULT_INIT_METHOD,			// CET: Added 8-24-2009
	IPOPT_OPT_WATCHDOG_SHORTENED_ITER_TRIGGER,	// CET: Added 8-24-2009
	IPOPT_OPT_LIMITED_MEMORY_MAX_HISTORY,		// CET: Added 8-24-2009
	IPOPT_OPT_LIMITED_MEMORY_MAX_SKIPPING,		// CET: Added 8-24-2009
	IPOPT_OPT_START_WITH_RESTO,					// CET: Added 8-24-2009
	IPOPT_OPT_EVALUATE_ORIG_OBJ_AT_RESTO_TRIAL, // CET: Added 8-24-2009

#ifdef MC19_IS_AVAILABLE
	IPOPT_OPT_LINEAR_SYSTEM_SCALING,			// CET: Added 8-24-2009
#endif

	IPOPT_OPT_JAC_C_CONSTANT,
	IPOPT_OPT_JAC_D_CONSTANT,
	IPOPT_OPT_HESSIAN_CONSTANT,
	IPOPT_OPT_BARRIER_UPDATE_METHOD,
	IPOPT_OPT_CHECK_DERIV_NANINF,
	IPOPT_OPT_CONSTRAINT_MULT_STEP_SIZE,
	IPOPT_OPT_CHECK_DERIVATIVES,
	IPOPT_OPT_BARRIER_MEHROTRA,
	IPOPT_OPT_EXPECT_INFEAS_PROBLEM,
	IPOPT_OPT_FIXED_VARIABLE_TREATMENT,
	IPOPT_OPT_HESSIAN_APPROXIMATION,
	IPOPT_OPT_LINEAR_SOLVER,

#ifdef MC19_IS_AVAILABLE
	IPOPT_OPT_SCALE_ON_DEMAND,
#endif


#ifdef MA57_IS_AVAILABLE
	IPOPT_OPT_MA57_PIVOT_ORDER,				// CET: Added 04-23-2010
	IPOPT_OPT_MA57_BLOCK_SIZE,				// CET: Added 04-28-2010
	IPOPT_OPT_MA57_NODE_AMALGAMATION,		// CET: Added 04-28-2010
	IPOPT_OPT_MA57_SCALING_FLAG,			// CET: Added 04-28-2010
	IPOPT_OPT_MA57_SMALL_PIVOT_FLAG,		// CET: Added 04-28-2010
#endif


#ifdef MUMPS_IS_AVAILABLE
	IPOPT_OPT_MUMPS_MEM_PERCENT,			// CET: Added 8-22-2009
	IPOPT_OPT_MUMPS_PERMUTING_SCALING,		// CET: Added 8-22-2009
	IPOPT_OPT_MUMPS_PIVOT_ORDER,			// CET: Added 8-22-2009
	IPOPT_OPT_MUMPS_SCALING,				// CET: Added 8-22-2009
#endif


	IPOPT_OPT_MAX_NUM_ITER,
	IPOPT_OPT_ACCEPTABLE_ITER,				// CET: Added 8-23-2009
	IPOPT_OPT_MAX_NUM_REFINEMENT_STEPS,
	IPOPT_OPT_MAX_WATCHDOG_ITER,
	IPOPT_OPT_MIN_NUM_REFINEMENT_STEPS,
	IPOPT_OPT_SCALING_METHOD,
	IPOPT_OPT_BARRIER_ORACLE_ADAPTIVE,
	IPOPT_OPT_BARRIER_FIXED_MU_ORACLE,
	IPOPT_OPT_PRINT_LEVEL,
	IPOPT_OPT_PRINT_OPTIONS_DOCUMENTATION,
	IPOPT_OPT_PRINT_DERIV,
	IPOPT_OPT_PRINT_USER_OPTIONS,
	IPOPT_OPT_HONOR_ORIGINAL_BOUNDS,
	IPOPT_OPT_BARRIER_QUALITY_FN_MAX_STEPS,
	IPOPT_OPT_RECALC_MULTIPLIERS,
	IPOPT_OPT_STATUS_FILE,
	IPOPT_OPT_MAX_2ND_ORDER_STEPS,
	IPOPT_OPT_CORRECTOR_STEP_TYPE,
//	IPOPT_OPT_WARM_START,				// CET: Not currently used

	// This identifier must always be at the end of the list, as it sets
	// the number of integer options
    IPOPT_OPT_INT_MAX		
};


// The double options that will be available from within AIMMS

enum {
	IPOPT_OPT_NLP_LOWER_BOUND_INF,				// CET: Added 8-24-2009
	IPOPT_OPT_NLP_UPPER_BOUND_INF,				// CET: Added 8-24-2009
	IPOPT_OPT_SLACK_BOUND_FRAC,					// CET: Added 8-24-2009
	IPOPT_OPT_SLACK_BOUND_PUSH,					// CET: Added 8-24-2009
	IPOPT_OPT_CONSTR_MULT_INIT_MAX,				// CET: Added 8-24-2009

	IPOPT_OPT_MU_MAX_FACT,						// CET: Added 8-24-2009
	IPOPT_OPT_MU_MAX,							// CET: Added 8-24-2009
	IPOPT_OPT_MU_MIN,							// CET: Added 8-24-2009
	IPOPT_OPT_BARRIER_TOL_FACTOR,				// CET: Added 8-24-2009
	IPOPT_OPT_MU_LINEAR_DECREASE_FACTOR,		// CET: Added 8-24-2009
	IPOPT_OPT_MU_SUPERLINEAR_DECREASE_POWER,	// CET: Added 8-24-2009

	IPOPT_OPT_RECALC_Y_FEAS_TOL,				// CET: Added 8-24-2009

	IPOPT_OPT_EXPECT_INFEASIBLE_PROBLEM_CTOL,		// CET: Added 8-24-2009
	IPOPT_OPT_EXPECT_INFEASIBLE_PROBLEM_YTOL,
	IPOPT_OPT_SOFT_RESTO_PDERROR_REDUCTION_FACTOR,	// CET: Added 8-24-2009
	IPOPT_OPT_REQUIRED_INFEASIBILITY_REDUCTION,		// CET: Added 8-24-2009
	IPOPT_OPT_BOUND_MULT_RESET_THRESHOLD,			// CET: Added 8-24-2009
	IPOPT_OPT_CONSTR_MULT_RESET_THRESHOLD,			// CET: Added 8-24-2009

	IPOPT_OPT_MAX_HESSIAN_PERTURBATION,				// CET: Added 8-24-2009
	IPOPT_OPT_MIN_HESSIAN_PERTURBATION,				// CET: Added 8-24-2009
	IPOPT_OPT_FIRST_HESSIAN_PERTURBATION,			// CET: Added 8-24-2009
	IPOPT_OPT_PERTURB_INC_FACT_FIRST,				// CET: Added 8-24-2009
	IPOPT_OPT_PERTURB_INC_FACT,						// CET: Added 8-24-2009
	IPOPT_OPT_PERTURB_DEC_FACT,						// CET: Added 8-24-2009
	IPOPT_OPT_JACOBIAN_REGULARIZATION_VALUE,		// CET: Added 8-24-2009

	IPOPT_OPT_DERIVATIVE_TEST_PERTURBATION,			// CET: Added 8-24-2009
	IPOPT_OPT_DERIVATIVE_TEST_TOL,					// CET: Added 8-24-2009
	IPOPT_OPT_POINT_PERTURBATION_RADIUS,			// CET: Added 8-24-2009


    IPOPT_OPT_ABSOLUTE_BOUND_DISTANCE,
    IPOPT_OPT_BARRIER_INITIAL_VALUE,
    IPOPT_OPT_BOUND_MULTIPLIERS_INIT_VAL,
    IPOPT_OPT_BOUNDS_RELAXATION_FACTOR,
    IPOPT_OPT_COMPLEMENTARITY_ACCEP_TOL,
    IPOPT_OPT_COMPLEMENTARITY_TOLERANCE,
    IPOPT_OPT_CONSTRAINT_VIOL_ACCEP_TOL,
    IPOPT_OPT_CONSTRAINT_VIOL_TOLERANCE,
    IPOPT_OPT_CONVERGENCE_ACCEP_TOL,
    IPOPT_OPT_CONVERGENCE_TOLERANCE,
	IPOPT_OPT_DIVERGING_ITERATES_TOL,				// CET: Added 4-28-2009
	IPOPT_OPT_DUAL_INF_ACCEP_TOL,
	IPOPT_OPT_DUAL_INF_TOLERANCE,
	
#ifdef MA27_IS_AVAILABLE
    IPOPT_OPT_MA27_MAXIMUM_PIVOT_TOLERANCE,
    IPOPT_OPT_MA27_PIVOT_TOLERANCE,
	IPOPT_OPT_MA27_LIW_INIT_FACTOR,					// CET: Added 8-22-2009
	IPOPT_OPT_MA27_LA_INIT_FACTOR,					// CET: Added 8-22-2009
	IPOPT_OPT_MA27_MEMINC_FACTOR,					// CET: Added 8-22-2009

#endif

#ifdef MA57_IS_AVAILABLE
    IPOPT_OPT_MA57_MAXIMUM_PIVOT_TOLERANCE,
    IPOPT_OPT_MA57_PIVOT_TOLERANCE,
	IPOPT_OPT_MA57_PRE_ALLOC,						// CET: Added 04-23-2010
#endif
	
#ifdef MUMPS_IS_AVAILABLE
    IPOPT_OPT_MUMPS_MAXIMUM_PIVOT_TOLERANCE,
    IPOPT_OPT_MUMPS_PIVOT_TOLERANCE,

#endif

	IPOPT_OPT_NLP_SCALING_MAXIMUM_GRAD,
	IPOPT_OPT_OBJ_CHANGE_ACCEP_TOL,				// CET: Added 4-30-2009
	IPOPT_OPT_OBJECTIVE_SCALING_FACTOR,
	IPOPT_OPT_RELATIVE_BOUND_DISTANCE,
	IPOPT_OPT_ALPHA_FOR_Y_TOL,

#ifdef WSMP_IS_AVAILABLE
    IPOPT_OPT_WSMP_MAXIMUM_PIVOT_TOLERANCE,
    IPOPT_OPT_WSMP_PIVOT_TOLERANCE,
#endif

	// This identifier must always be at the end of the list, as it sets the
	// number of double options
    IPOPT_OPT_DBL_MAX
};



// The structure defining all the IPOPT options.

typedef struct ipopt_option_tag 
{
    int           index;			// Index of the option
    int           category;			// Category number
    int           order;			// Index within category
    const char   *aimms_name;		// Name of the option displayed in AIMMS
    const char   *ipopt_name;		// Name of the option in IPOPT
    int           keywords;			// 1 if option has keywords, NULL if it does not
    const char  **keyword;			// String array of the keywords to be displayed in AIMMS
    const char  **ipopt_keyword;	// String array of the corresponding keywords that are used as IPOPT options

} ipopt_option_rec;


#ifdef MUMPS_IS_AVAILABLE 
#define NR_MUMPS_AVAILABLE  1
#else
#define NR_MUMPS_AVAILABLE  0
#endif

#ifdef MA27_IS_AVAILABLE 
#define NR_M27_AVAILABLE  1
#else
#define NR_M27_AVAILABLE  0
#endif

#ifdef MA57_IS_AVAILABLE 
#define NR_M57_AVAILABLE  1
#else
#define NR_M57_AVAILABLE  0
#endif

#ifdef PARDISO_IS_AVAILABLE 
#define NR_PARDISO_AVAILABLE  1
#else
#define NR_PARDISO_AVAILABLE  0
#endif

#ifdef WSMP_IS_AVAILABLE 
#define NR_WSMP_AVAILABLE  1
#else
#define NR_WSMP_AVAILABLE  0
#endif

#define NR_LINEAR_SOLVERS  ( NR_MUMPS_AVAILABLE + NR_M27_AVAILABLE + NR_M57_AVAILABLE + NR_PARDISO_AVAILABLE + NR_WSMP_AVAILABLE )

#endif // _DEFINED_IPOPT_OPTIONS_H
