// Copyright (C) 2009 Paragon Decision Technology B.V. and others.
// All Rights Reserved.
// This code is published under the Common Public License.
//
// $Id$
//
// Author: Marcel Hunting

#ifndef _DEFINED_CBC_OPTIONS_H
#define _DEFINED_CBC_OPTIONS_H

/* Creates category CBC in the 'Project Options' with CBC_CAT_MAX subcategories:

     - Specific solvers
          - CBC
               - General
                    - Dual feasibility tolerance
                    - LP method
                    - etc
               - Barrier
                    - Barrier crossover
                    - Barrier scaling
               - MIP
                    - Increment
                    - Integrality
                    - MIP calculate basis and marginals
                    - etc
               - etc
*/

// The CBC categories; there must be at least 1!

enum {
    CBC_CAT_GENERAL,
    CBC_CAT_BARRIER,
    CBC_CAT_MIP,
    CBC_CAT_MIP_CUTS,
    CBC_CAT_MIP_HEUR,
#ifdef CBC_PARALLEL
    CBC_CAT_PARALLEL,
#endif
    CBC_CAT_PRESOLVE,
    CBC_CAT_SIMPLEX,
    CBC_CAT_LOGGING,
    CBC_CAT_MAX
};

// The CBC integer options.

enum {
	CBC_OPT_BARRIER_CROSSOVER,
	CBC_OPT_BARRIER_SCALING,
	CBC_OPT_CLIQUE_CUTS,
	CBC_OPT_COMBINE_SOLUTIONS,
	CBC_OPT_CRASH,
	CBC_OPT_CUT_DEPTH,
	CBC_OPT_CUT_PASSES_ROOT_NODE,
	CBC_OPT_CUT_PASSES_TREE,
	CBC_OPT_DUAL_PIVOT,
	CBC_OPT_FEASIBILITY_PUMP,
	CBC_OPT_FEASIBILITY_PUMP_PASSES,
	CBC_OPT_FLOW_COVER_CUTS,
	CBC_OPT_GLOBAL_CUT_CONTROL,
    CBC_OPT_GOMORY_CUTS,
    CBC_OPT_GREEDY_HEURISTIC,
    CBC_OPT_HEURISTICS,
    CBC_OPT_IDIOT_CRASH,
    CBC_OPT_KNAPSACK_CUTS,
    CBC_OPT_LIFT_AND_PROJECT_CUTS,
    CBC_OPT_LOCAL_TREE_SEARCH,
    CBC_OPT_LP_METHOD,
    CBC_OPT_MIP_BASIS,
    CBC_OPT_MIP_CALCULATE_BASIS_AND_MARG,
    CBC_OPT_MIP_PRESOLVE,
    CBC_OPT_MIP_PRINT_FREQ,
    CBC_OPT_MIP_START,
    CBC_OPT_MIR_CUTS,
    CBC_OPT_MPS,
    CBC_OPT_NODE_SELECTION,
    CBC_OPT_OUTPUT_LEVEL,
    CBC_OPT_PERTURBATION,
    CBC_OPT_POSTSOLVE_CONT,
    CBC_OPT_POSTSOLVE_INT,
    CBC_OPT_PRESOLVE,
    CBC_OPT_PRIMAL_PIVOT,
    CBC_OPT_PROBING_CUTS,
    CBC_OPT_REDUCE_AND_SPLIT_CUTS,
    CBC_OPT_REFACTOR_INTERVAL,
    CBC_OPT_RESIDUAL_CAPACITY_CUTS,
    CBC_OPT_RINS_HEURISTIC,
    CBC_OPT_ROUNDING_HEURISTIC,
    CBC_OPT_SCALING,
    CBC_OPT_SIFTING,
    CBC_OPT_STATUS_FILE,
    CBC_OPT_STRONG_BRANCHING,
#ifdef CBC_PARALLEL
    CBC_OPT_THREAD_LIMIT,
#endif
    CBC_OPT_TRUST_PSEUDO_COSTS,
    CBC_OPT_TWO_MIR_CUTS,
    CBC_OPT_VARIABLE_SELECTION,
    CBC_OPT_INT_MAX
};

// The CBC double options.

enum {
	CBC_OPT_DUAL_FEAS_TOL,
	CBC_OPT_INCREMENT,
    CBC_OPT_INTEGRALITY,
    CBC_OPT_OBJ_SCALE_FACTOR,
    CBC_OPT_PRESOLVE_TOL,
    CBC_OPT_PRIMAL_FEAS_TOL,
    CBC_OPT_RHS_SCALE_FACTOR,
    CBC_OPT_DBL_MAX
};



// The structure defining all the CBC options

typedef struct cbc_option_tag {
    int      index;           // Index
    int      category;        // Category number
    int      order;           // Index within category
    int      keywords;        // 1 if option has keywords
    char   **keyword;         // Description of the keywords in AIMMS
    char   **cbc_keyword;     // Description of the keywords in CBC
    int      change;          // 1 if option can be changed during progress
    char    *aimms_name;      // Name of the option in AIMMS
    char    *cbc_name;        // Name of the option in CBC
} cbc_option_rec;

#endif // _DEFINED_CBC_OPTIONS_H

