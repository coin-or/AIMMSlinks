// Copyright (C) 2009 Paragon Decision Technology B.V. and others.
// All Rights Reserved.
// This code is published under the Common Public License.
//
// $Id$
//
// Author: Marcel Hunting

#include "cbc_interface.h"
#include "cbc_options.h"


// The function cbc_order_options()
// The function cbc_init_global_options()
// The function cbc_init_options()
// The function cbc_get_options()
// The function cbc_set_options()

// The function GetIntegerOption()
// The function GetIntegerKeyword()
// The function GetDoubleOption()
// The function GetCategoryName()


// Names of the categories.

static char *cbc_category[] = {
    "General",
    "Barrier",
    "MIP",
    "MIP cuts",
    "MIP heuristics",
#ifdef CBC_PARALLEL
    "Parallel",
#endif
    "Presolve",
    "Simplex",
    "Logging"
};


// The lower and upper bound and default value of the integer options.

static int  cbc_int_opt_low[ CBC_OPT_INT_MAX ] = {
/* CBC_OPT_BARRIER_CROSSOVER             */   0,
/* CBC_OPT_BARRIER_SCALING               */   0,
/* CBC_OPT_CLIQUE_CUTS                   */   0,
/* CBC_OPT_COMBINE_SOLUTIONS             */   0,
/* CBC_OPT_CRASH                         */   0,
/* CBC_OPT_CUT_DEPTH                     */  -1,
/* CBC_OPT_CUT_PASSES_ROOT_NODE          */  -999999,
/* CBC_OPT_CUT_PASSES_TREE               */  -999999,
/* CBC_OPT_DUAL_PIVOT                    */  -1,
/* CBC_OPT_FEASIBILITY_PUMP              */   0,
/* CBC_OPT_FEASIBILITY_PUMP_PASSES       */   0,
/* CBC_OPT_FLOW_COVER_CUTS               */   0,
/* CBC_OPT_GLOBAL_CUT_CONTROL            */   0,
/* CBC_OPT_GOMORY_CUTS                   */   0,
/* CBC_OPT_GREEDY_HEURISTIC              */   0,
/* CBC_OPT_HEURISTICS                    */   0,
/* CBC_OPT_IDIOT_CRASH                   */  -1,
/* CBC_OPT_KNAPSACK_CUTS                 */   0,
/* CBC_OPT_LIFT_AND_PROJECT_CUTS         */   0,
/* CBC_OPT_LOCAL_TREE_SEARCH             */   0,
/* CBC_OPT_LP_METHOD                     */   0,
/* CBC_OPT_MIP_BASIS                     */   0,
/* CBC_OPT_MIP_CALCULATE_BASIS_AND_MARG  */   0,
/* CBC_OPT_MIP_PRESOLVE                  */   0,
/* CBC_OPT_MIP_PRINT_FREQ                */   0,
/* CBC_OPT_MIP_START                     */   0,
/* CBC_OPT_MIR_CUTS                      */   0,
/* CBC_OPT_MPS                           */   0,
/* CBC_OPT_NODE_SELECTION                */   0,
/* CBC_OPT_OUTPUT_LEVEL                  */  -1,
/* CBC_OPT_PERTURBATION                  */   0,
/* CBC_OPT_POSTSOLVE_CONT                */   0,
/* CBC_OPT_POSTSOLVE_INT                 */   0,
/* CBC_OPT_PRESOLVE                      */   0,
/* CBC_OPT_PRIMAL_PIVOT                  */  -1,
/* CBC_OPT_PROBING_CUTS                  */   0,
/* CBC_OPT_REDUCE_AND_SPLIT_CUTS         */   0,
/* CBC_OPT_REFACTOR_INTERVAL             */   1,
/* CBC_OPT_RESIDUAL_CAPACITY_CUTS        */   0,
/* CBC_OPT_RINS_HEURISTIC                */   0,
/* CBC_OPT_ROUNDING_HEURISTIC            */   0,
/* CBC_OPT_SCALING                       */  -1,
/* CBC_OPT_SIFTING                       */  -1,
/* CBC_OPT_STATUS_FILE                   */   0,
/* CBC_OPT_STRONG_BRANCHING              */   0,
#ifdef CBC_PARALLEL
/* CBC_OPT_THREAD_LIMIT                  */   1,
#endif
/* CBC_OPT_TRUST_PSEUDO_COSTS            */  -3,
/* CBC_OPT_TWO_MIR_CUTS                  */   0,
/* CBC_OPT_VARIABLE_SELECTION            */   0
};


static int  cbc_int_opt_def[ CBC_OPT_INT_MAX ] = {
/* CBC_OPT_BARRIER_CROSSOVER             */   1,
/* CBC_OPT_BARRIER_SCALING               */   0,
/* CBC_OPT_CLIQUE_CUTS                   */   3,
/* CBC_OPT_COMBINE_SOLUTIONS             */   1,
/* CBC_OPT_CRASH                         */   0,
/* CBC_OPT_CUT_DEPTH                     */  -1,
/* CBC_OPT_CUT_PASSES_ROOT_NODE          */  -1,
/* CBC_OPT_CUT_PASSES_TREE               */   1,
/* CBC_OPT_DUAL_PIVOT                    */  -1,
/* CBC_OPT_FEASIBILITY_PUMP              */   1,
/* CBC_OPT_FEASIBILITY_PUMP_PASSES       */   20,
/* CBC_OPT_FLOW_COVER_CUTS               */   3,
/* CBC_OPT_GLOBAL_CUT_CONTROL            */   1,
/* CBC_OPT_GOMORY_CUTS                   */   3,
/* CBC_OPT_GREEDY_HEURISTIC              */   1,
/* CBC_OPT_HEURISTICS                    */   1,
/* CBC_OPT_IDIOT_CRASH                   */  -1,
/* CBC_OPT_KNAPSACK_CUTS                 */   3,
/* CBC_OPT_LIFT_AND_PROJECT_CUTS         */   0,
/* CBC_OPT_LOCAL_TREE_SEARCH             */   0,
/* CBC_OPT_LP_METHOD                     */   1,
/* CBC_OPT_MIP_BASIS                     */   0,
/* CBC_OPT_MIP_CALCULATE_BASIS_AND_MARG  */   0,
/* CBC_OPT_MIP_PRESOLVE                  */   1,
/* CBC_OPT_MIP_PRINT_FREQ                */   0,
/* CBC_OPT_MIP_START                     */   0,
/* CBC_OPT_MIR_CUTS                      */   3,
/* CBC_OPT_MPS                           */   0,
/* CBC_OPT_NODE_SELECTION                */   1,
/* CBC_OPT_OUTPUT_LEVEL                  */   1,
/* CBC_OPT_PERTURBATION                  */   1,
/* CBC_OPT_POSTSOLVE_CONT                */   0,
/* CBC_OPT_POSTSOLVE_INT                 */   0,
/* CBC_OPT_PRESOLVE                      */   1,
/* CBC_OPT_PRIMAL_PIVOT                  */  -1,
/* CBC_OPT_PROBING_CUTS                  */   3,
/* CBC_OPT_REDUCE_AND_SPLIT_CUTS         */   0,
/* CBC_OPT_REFACTOR_INTERVAL             */   200,
/* CBC_OPT_RESIDUAL_CAPACITY_CUTS        */   0,
/* CBC_OPT_RINS_HEURISTIC                */   0,
/* CBC_OPT_ROUNDING_HEURISTIC            */   1,
/* CBC_OPT_SCALING                       */  -1,
/* CBC_OPT_SIFTING                       */  -1,
/* CBC_OPT_STATUS_FILE                   */   0,
/* CBC_OPT_STRONG_BRANCHING              */   5,
#ifdef CBC_PARALLEL
/* CBC_OPT_THREAD_LIMIT                  */   1,
#endif
/* CBC_OPT_TRUST_PSEUDO_COSTS            */   5,
/* CBC_OPT_TWO_MIR_CUTS                  */   3,
/* CBC_OPT_VARIABLE_SELECTION            */   0
};


static int  cbc_int_opt_up[ CBC_OPT_INT_MAX ] = {
/* CBC_OPT_BARRIER_CROSSOVER             */   1,
/* CBC_OPT_BARRIER_SCALING               */   1,
/* CBC_OPT_CLIQUE_CUTS                   */   4,
/* CBC_OPT_COMBINE_SOLUTIONS             */   3,
/* CBC_OPT_CRASH                         */   3,
/* CBC_OPT_CUT_DEPTH                     */   999999,
/* CBC_OPT_CUT_PASSES_ROOT_NODE          */   999999,
/* CBC_OPT_CUT_PASSES_TREE               */   999999,
/* CBC_OPT_DUAL_PIVOT                    */   1,
/* CBC_OPT_FEASIBILITY_PUMP              */   3,
/* CBC_OPT_FEASIBILITY_PUMP_PASSES       */   10000,
/* CBC_OPT_FLOW_COVER_CUTS               */   4,
/* CBC_OPT_GLOBAL_CUT_CONTROL            */   4,
/* CBC_OPT_GOMORY_CUTS                   */   5,
/* CBC_OPT_GREEDY_HEURISTIC              */   3,
/* CBC_OPT_HEURISTICS                    */   1,
/* CBC_OPT_IDIOT_CRASH                   */   999999,
/* CBC_OPT_KNAPSACK_CUTS                 */   4,
/* CBC_OPT_LIFT_AND_PROJECT_CUTS         */   4,
/* CBC_OPT_LOCAL_TREE_SEARCH             */   1,
/* CBC_OPT_LP_METHOD                     */   2,
/* CBC_OPT_MIP_BASIS                     */   1,
/* CBC_OPT_MIP_CALCULATE_BASIS_AND_MARG  */   1,
/* CBC_OPT_MIP_PRESOLVE                  */   8,
/* CBC_OPT_MIP_PRINT_FREQ                */   999999,
/* CBC_OPT_MIP_START                     */   1,
/* CBC_OPT_MIR_CUTS                      */   4,
/* CBC_OPT_MPS                           */   4,
/* CBC_OPT_NODE_SELECTION                */   6,
/* CBC_OPT_OUTPUT_LEVEL                  */   63,
/* CBC_OPT_PERTURBATION                  */   1,
/* CBC_OPT_POSTSOLVE_CONT                */   2,
/* CBC_OPT_POSTSOLVE_INT                 */   2,
/* CBC_OPT_PRESOLVE                      */   2,
/* CBC_OPT_PRIMAL_PIVOT                  */   4,
/* CBC_OPT_PROBING_CUTS                  */   4,
/* CBC_OPT_REDUCE_AND_SPLIT_CUTS         */   4,
/* CBC_OPT_REFACTOR_INTERVAL             */   999999,
/* CBC_OPT_RESIDUAL_CAPACITY_CUTS        */   4,
/* CBC_OPT_RINS_HEURISTIC                */   2,
/* CBC_OPT_ROUNDING_HEURISTIC            */   3,
/* CBC_OPT_SCALING                       */   2,
/* CBC_OPT_SIFTING                       */   5000000,
/* CBC_OPT_STATUS_FILE                   */   2,
/* CBC_OPT_STRONG_BRANCHING              */   999999,
#ifdef CBC_PARALLEL
/* CBC_OPT_THREAD_LIMIT                  */   1000,
#endif
/* CBC_OPT_TRUST_PSEUDO_COSTS            */   2000000,
/* CBC_OPT_TWO_MIR_CUTS                  */   4,
/* CBC_OPT_VARIABLE_SELECTION            */   2
};


// The lower and upper bound and default value of the double options.

static double  cbc_dbl_opt_low[ CBC_OPT_DBL_MAX ] = {
/* CBC_OPT_DUAL_FEAS_TOL              */   1.0e-20,
/* CBC_OPT_INCREMENT                  */  -1.0e+20,
/* CBC_OPT_INTEGRALITY                */   1.0e-20,
/* CBC_OPT_OBJ_SCALE_FACTOR           */  -1.0e+20,
/* CBC_OPT_PRESOLVE_TOL               */   1.0e-20,
/* CBC_OPT_PRIMAL_FEAS_TOL            */   1.0e-20,
/* CBC_OPT_RHS_SCALE_FACTOR           */  -1.0e+20
};


static double  cbc_dbl_opt_def[ CBC_OPT_DBL_MAX ] = {
/* CBC_OPT_DUAL_FEAS_TOL              */   1.0e-7,
/* CBC_OPT_INCREMENT                  */   1.0e-5,
/* CBC_OPT_INTEGRALITY                */   1.0e-6,
/* CBC_OPT_OBJ_SCALE_FACTOR           */   1.0,
/* CBC_OPT_PRESOLVE_TOL               */   1.0e-8,
/* CBC_OPT_PRIMAL_FEAS_TOL            */   1.0e-7,
/* CBC_OPT_RHS_SCALE_FACTOR           */   1.0
};


static double  cbc_dbl_opt_up[ CBC_OPT_DBL_MAX ] = {
/* CBC_OPT_DUAL_FEAS_TOL              */   1.0e+12,
/* CBC_OPT_INCREMENT                  */   1.0e+20,
/* CBC_OPT_INTEGRALITY                */   0.5,
/* CBC_OPT_OBJ_SCALE_FACTOR           */   1.0e+20,
/* CBC_OPT_PRESOLVE_TOL               */   1.0e+12,
/* CBC_OPT_PRIMAL_FEAS_TOL            */   1.0e+12,
/* CBC_OPT_RHS_SCALE_FACTOR           */   1.0e+20
};


// The option-keyword definitions.
// Preferably keyword should have length < 37, otherwise it looks ugly in AIMMS.

static char *akeyw_off_on[] = {
    "Off",
    "On" };

static char *akeyw_crash[] = {
    "Off",
    "On",
    "Solow-Halim",
    "Modified Solow-Halim" };
static char *ckeyw_crash[] = {
    "off",
    "on",
    "solow_halim",
    "halim_solow" };

static char *akeyw_cuts[] = {
    "Off",
    "On",
    "Root only",
    "If promising",
    "Always" };
static char *ckeyw_cuts[] = {
    "off",
    "on",
    "root",
    "ifmove",
    "forceOn" };

static char *akeyw_dual_pivot[] = {
    "Automatic",
    "Partial",
    "Steepest" };

static char *akeyw_gomory_cuts[] = {
    "Off",
    "On",
    "Root only",
    "If promising",
    "Often",
    "Always" };
static char *ckeyw_gomory_cuts[] = {
    "off",
    "on",
    "root",
    "ifmove",
    "forceOn",
    "forceLongOn" };

static char *akeyw_heuristic[] = {
    "Off",
    "On",
    "Before",
    "Both" };
static char *ckeyw_heuristic[] = {
    "off",
    "on",
    "before",
    "both" };

static char *akeyw_lp_method[] = {
    "Primal simplex",
    "Dual simplex",
    "Barrier" };

static char *akeyw_mip_preslv[] = {
    "Off",
    "On",
    "Clique inequalities",
    "SOS",
    "Try SOS",
    "All inequalities",
    "Strategy",
    "Aggregate",
    "Force SOS" };
static char *ckeyw_mip_preslv[] = {
    "off",
    "on",
    "equal",
    "sos",
    "trysos",
    "equalall",
    "strategy",
    "aggregate",
    "forcesos" };

static char *akeyw_mps[] = {
    "Never",
    "At the first solve (MPS format)",
    "At every solve (MPS format)",
    "At the first solve (LP format)",
    "At every solve (LP format)" };

static char *akeyw_node_select[] = {
    "Hybrid",
    "Fewest infeasibilities",
    "Depth first",
    "Up branch fewest infeasibilities",
    "Down branch fewest infeasibilities",
    "Up branch depth first",
    "Down branch depth first" };
static char *ckeyw_node_select[] = {
    "hybrid",
    "fewest",
    "depth",
    "upfewest",
    "downfewest",
    "updepth",
    "downdepth" };

static char *akeyw_postslv_cont[] = {
    "No rounding",
    "Round to nearest bound",
    "Round to nearest bound, resolve LP" };

static char *akeyw_postslv_int[] = {
    "No rounding",
    "Round to integer",
    "Round integers and resolve LP" };

static char *akeyw_presolve[] = {
    "Off",
    "On",
    "More" };

static char *akeyw_primal_pivot[] = {
    "Automatic",
    "Exact",
    "Partial",
    "Steepest",
    "Change",
    "Sprint" };

static char *akeyw_rins[] = {
    "Off",
    "On",
    "Often" };

static char *akeyw_scaling[] = {
    "Automatic",
    "Off",
    "Equilibrium",
    "Geometric" };

static char *akeyw_status_file[] = {
    "Off",
    "File",
    "Stdout" };

static char *akeyw_var_select[] = {
    "Off",
    "Absolute cost",
    "Column order" };
static char *ckeyw_var_select[] = {
    "off",
    "priorities",
    "columnOrder" };


// The tables of all the CBC options.

// Table of integer options.

static cbc_option_rec  cbc_int_options[] = {
{ CBC_OPT_BARRIER_CROSSOVER           , CBC_CAT_BARRIER  , 0 , 1 , akeyw_off_on      , NULL             ,
  0, "Barrier_crossover"                 , "-crossover"                                                 },
{ CBC_OPT_BARRIER_SCALING             , CBC_CAT_BARRIER  , 0 , 1 , akeyw_off_on      , NULL             ,
  0, "Barrier_scaling"                   , "-bscale"                                                    },
{ CBC_OPT_CLIQUE_CUTS                 , CBC_CAT_MIP_CUTS , 0 , 1 , akeyw_cuts        , ckeyw_cuts       ,
  0, "Clique_cuts"                       , "-cliqueCuts"                                                },
{ CBC_OPT_COMBINE_SOLUTIONS           , CBC_CAT_MIP_HEUR , 0 , 1 , akeyw_heuristic   , ckeyw_heuristic  ,
  0, "Combine_solutions"                 , "-combineSolutions"                                          },
{ CBC_OPT_CRASH                       , CBC_CAT_SIMPLEX  , 0 , 1 , akeyw_crash       , ckeyw_crash      ,
  0, "Crash"                             , "-crash"                                                     },
{ CBC_OPT_CUT_DEPTH                   , CBC_CAT_MIP_CUTS , 0 , 0 , NULL              , NULL             ,
  0, "Cut_depth"                         , "-cutDepth"                                                  },
{ CBC_OPT_CUT_PASSES_ROOT_NODE        , CBC_CAT_MIP_CUTS , 0 , 0 , NULL              , NULL             ,
  0, "Cut_passes_root_node"              , "-passCuts"                                                  },
{ CBC_OPT_CUT_PASSES_TREE             , CBC_CAT_MIP_CUTS , 0 , 0 , NULL              , NULL             ,
  0, "Cut_passes_tree"                   , "-passTree"                                                  },
{ CBC_OPT_DUAL_PIVOT                  , CBC_CAT_SIMPLEX  , 0 , 1 , akeyw_dual_pivot  , NULL             ,
  0, "Dual_pivot"                        , "-dualPivot"                                                 },
{ CBC_OPT_FEASIBILITY_PUMP            , CBC_CAT_MIP_HEUR , 0 , 1 , akeyw_heuristic   , ckeyw_heuristic  ,
  0, "Feasibility_pump"                  , "-feasibilityPump"                                           },
{ CBC_OPT_FEASIBILITY_PUMP_PASSES     , CBC_CAT_MIP_HEUR , 0 , 0 , NULL              , NULL             ,
  0, "Feasibility_pump_passes"           , "-passFeasibilityPump"                                       },
{ CBC_OPT_FLOW_COVER_CUTS             , CBC_CAT_MIP_CUTS , 0 , 1 , akeyw_cuts        , ckeyw_cuts       ,
  0, "Flow_cover_cuts"                   , "-flowCoverCuts"                                             },
{ CBC_OPT_GLOBAL_CUT_CONTROL          , CBC_CAT_MIP_CUTS , 0 , 1 , akeyw_cuts        , ckeyw_cuts       ,
  0, "Global_cut_control"                , "-cuts"                                                      },
{ CBC_OPT_GOMORY_CUTS                 , CBC_CAT_MIP_CUTS , 0 , 1 , akeyw_gomory_cuts , ckeyw_gomory_cuts,
  0, "Gomory_cuts"                       , "-gomoryCuts"                                                },
{ CBC_OPT_GREEDY_HEURISTIC            , CBC_CAT_MIP_HEUR , 0 , 1 , akeyw_heuristic   , ckeyw_heuristic  ,
  0, "Greedy_heuristic"                  , "-greedyHeuristic"                                           },
{ CBC_OPT_HEURISTICS                  , CBC_CAT_MIP_HEUR , 0 , 1 , akeyw_off_on      , NULL             ,
  0, "Heuristics"                        , "-heuristicsOnOff"                                           },
{ CBC_OPT_IDIOT_CRASH                 , CBC_CAT_SIMPLEX  , 0 , 0 , NULL              , NULL             ,
  0, "Idiot_crash"                       , "-idiotCrash"                                                },
{ CBC_OPT_KNAPSACK_CUTS               , CBC_CAT_MIP_CUTS , 0 , 1 , akeyw_cuts        , ckeyw_cuts       ,
  0, "Knapsack_cuts"                     , "-knapsackCuts"                                              },
{ CBC_OPT_LIFT_AND_PROJECT_CUTS       , CBC_CAT_MIP_CUTS , 0 , 1 , akeyw_cuts        , ckeyw_cuts       ,
  0, "Lift_and_project_cuts"             , "-liftAndProjectCuts"                                        },
{ CBC_OPT_LOCAL_TREE_SEARCH           , CBC_CAT_MIP_HEUR , 0 , 1 , akeyw_off_on      , NULL             ,
  0, "Local_tree_search"                 , "-localTreeSearch"                                           },
{ CBC_OPT_LP_METHOD                   , CBC_CAT_GENERAL  , 0 , 1 , akeyw_lp_method   , NULL             ,
  0, "LP_method"                         , NULL                                                         },
{ CBC_OPT_MIP_BASIS                   , CBC_CAT_MIP      , 0 , 1 , akeyw_off_on      , NULL             ,
  0, "MIP_basis"                         , NULL                                                         },
{ CBC_OPT_MIP_CALCULATE_BASIS_AND_MARG, CBC_CAT_MIP      , 0 , 1 , akeyw_off_on      , NULL             ,
  0, "MIP_calculate_basis_and_marginals" , NULL                                                         },
{ CBC_OPT_MIP_PRESOLVE                , CBC_CAT_MIP      , 0 , 1 , akeyw_mip_preslv  , ckeyw_mip_preslv ,
  0, "MIP_presolve"                      , "-preprocess"                                                },
{ CBC_OPT_MIP_PRINT_FREQ              , CBC_CAT_LOGGING  , 0 , 0 , NULL              , NULL             ,
  0, "MIP_print_frequency"               , NULL                                                         },
{ CBC_OPT_MIP_START                   , CBC_CAT_MIP      , 0 , 1 , akeyw_off_on      , NULL             ,
  0, "MIP_start"                         , NULL                                                         },
{ CBC_OPT_MIR_CUTS                    , CBC_CAT_MIP_CUTS , 0 , 1 , akeyw_cuts        , ckeyw_cuts       ,
  0, "MIR_cuts"                          , "-mixedIntegerRoundingCuts"                                  },
{ CBC_OPT_MPS                         , CBC_CAT_GENERAL  , 0 , 1 , akeyw_mps         , NULL             ,
  0, "MPS"                               , NULL                                                         },
{ CBC_OPT_NODE_SELECTION              , CBC_CAT_MIP      , 0 , 1 , akeyw_node_select , ckeyw_node_select,
  0, "Node_selection"                    , "-nodeStrategy"                                              },
{ CBC_OPT_OUTPUT_LEVEL                , CBC_CAT_LOGGING  , 0 , 0 , NULL              , NULL             ,
  0, "Output_level"                      , "-slogLevel"                                                 },
{ CBC_OPT_PERTURBATION                , CBC_CAT_GENERAL  , 0 , 1 , akeyw_off_on      , NULL             ,
  0, "Perturbation"                      , "-perturbation"                                              },
{ CBC_OPT_POSTSOLVE_CONT              , CBC_CAT_GENERAL  , 0 , 1 , akeyw_postslv_cont, NULL             ,
  0, "Postsolve_continuous_variables"    , NULL                                                         },
{ CBC_OPT_POSTSOLVE_INT               , CBC_CAT_MIP      , 0 , 1 , akeyw_postslv_int , NULL             ,
  0, "Postsolve_integer_variables"       , NULL                                                         },
{ CBC_OPT_PRESOLVE                    , CBC_CAT_PRESOLVE , 0 , 1 , akeyw_presolve    , NULL             ,
  0, "Presolve"                          , "-presolve"                                                  },
{ CBC_OPT_PRIMAL_PIVOT                , CBC_CAT_SIMPLEX  , 0 , 1 , akeyw_primal_pivot, NULL             ,
  0, "Primal_pivot"                      , "-primalPivot"                                               },
{ CBC_OPT_PROBING_CUTS                , CBC_CAT_MIP_CUTS , 0 , 1 , akeyw_cuts        , ckeyw_cuts       ,
  0, "Probing_cuts"                      , "-probingCuts"                                               },
{ CBC_OPT_REDUCE_AND_SPLIT_CUTS       , CBC_CAT_MIP_CUTS , 0 , 1 , akeyw_cuts        , ckeyw_cuts       ,
  0, "Reduce_and_split_cuts"             , "-reduceAndSplitCuts"                                        },
{ CBC_OPT_REFACTOR_INTERVAL           , CBC_CAT_GENERAL  , 0 , 0 , NULL              , NULL             ,
  0, "Refactorization_interval"          , "-maxFactor"                                                 },
{ CBC_OPT_RESIDUAL_CAPACITY_CUTS      , CBC_CAT_MIP_CUTS , 0 , 1 , akeyw_cuts        , ckeyw_cuts       ,
  0, "Residual_capacity_cuts"            , "-residualCapacityCuts"                                      },
{ CBC_OPT_RINS_HEURISTIC              , CBC_CAT_MIP_HEUR , 0 , 1 , akeyw_rins        , NULL             ,
  0, "RINS_heuristic"                    , "-Rins"                                                      },
{ CBC_OPT_ROUNDING_HEURISTIC          , CBC_CAT_MIP_HEUR , 0 , 1 , akeyw_heuristic   , ckeyw_heuristic  ,
  0, "Rounding_heuristic"                , "-roundingHeuristic"                                         },
{ CBC_OPT_SCALING                     , CBC_CAT_GENERAL  , 0 , 1 , akeyw_scaling     , NULL             ,
  0, "Scaling"                           , "-scaling"                                                   },
{ CBC_OPT_SIFTING                     , CBC_CAT_GENERAL  , 0 , 0 , NULL              , NULL             ,
  0, "Sifting"                           , "-sprintCrash"                                               },
{ CBC_OPT_STATUS_FILE                 , CBC_CAT_LOGGING  , 0 , 1 , akeyw_status_file , NULL             ,
  0, "Status_file"                       , NULL                                                         },
{ CBC_OPT_STRONG_BRANCHING            , CBC_CAT_MIP      , 0 , 0 , NULL              , NULL             ,
  0, "Strong_branching"                  , "-strongbranching"                                           },
#ifdef CBC_PARALLEL
{ CBC_OPT_THREAD_LIMIT                , CBC_CAT_PARALLEL , 0 , 0 , NULL              , NULL             ,
  0, "Thread_limit"                      , NULL                                                         },
#endif
{ CBC_OPT_TRUST_PSEUDO_COSTS          , CBC_CAT_MIP      , 0 , 0 , NULL              , NULL             ,
  0, "Trust_pseudo_costs"                , "-trustPseudoCosts"                                          },
{ CBC_OPT_TWO_MIR_CUTS                , CBC_CAT_MIP_CUTS , 0 , 1 , akeyw_gomory_cuts , ckeyw_gomory_cuts,
  0, "Two_mir_cuts"                      , "-twoMirCuts"                                                },
{ CBC_OPT_VARIABLE_SELECTION          , CBC_CAT_MIP      , 0 , 1 , akeyw_var_select  , ckeyw_var_select ,
  0, "Variable_selection"                , "-costStrategy"                                              }
};
  

// Table of double options.

static cbc_option_rec  cbc_dbl_options[] = {
{ CBC_OPT_DUAL_FEAS_TOL               , CBC_CAT_GENERAL  , 0 , 0 , NULL, NULL ,
  0, "Dual_feasibility_tolerance"        , "-dualTolerance"                   },
{ CBC_OPT_INCREMENT                   , CBC_CAT_MIP      , 0 , 0 , NULL, NULL ,
  0, "Increment"                         , "-increment"                       },
{ CBC_OPT_INTEGRALITY                 , CBC_CAT_MIP      , 0 , 0 , NULL, NULL ,
  0, "Integrality"                       , "-integerTolerance"                },
{ CBC_OPT_OBJ_SCALE_FACTOR            , CBC_CAT_GENERAL  , 0 , 0 , NULL, NULL ,
  0, "Objective_scale_factor"            , "-objectiveScale"                  },
{ CBC_OPT_PRESOLVE_TOL                , CBC_CAT_PRESOLVE , 0 , 0 , NULL, NULL ,
  0, "Presolve_tolerance"                , "-preTolerance"                    },
{ CBC_OPT_PRIMAL_FEAS_TOL             , CBC_CAT_GENERAL  , 0 , 0 , NULL, NULL ,
  0, "Primal_feasibility_tolerance"      , "-primalTolerance"                 },
{ CBC_OPT_RHS_SCALE_FACTOR            , CBC_CAT_GENERAL  , 0 , 0 , NULL, NULL ,
  0, "RHS_scale_factor"                  , "-rhsScale"                        }
};



// The function cbc_order_options()
void CbcSolverInfo::cbc_order_options( void )
{
	int   cnt = 0, ind_int = 0, ind_dbl = 0;
	
	// The groups of integer and double options are each ordered according to their AIMMS
	// name. Now we combine these orderings and store the combined ordering.
	
#ifdef DEBUG
	// First check whether the groups of integer and double options are correctly ordered.
	
	for ( int i=0; i<CBC_OPT_INT_MAX-1; i++ ) {
		if ( STRICMP( cbc_int_options[i] . aimms_name, cbc_int_options[i+1] . aimms_name ) > 0 ) {
			BUG;
		}
	}
	
	for ( int i=0; i<CBC_OPT_DBL_MAX-1; i++ ) {
		if ( STRICMP( cbc_dbl_options[i] . aimms_name, cbc_dbl_options[i+1] . aimms_name ) > 0 ) {
			BUG;
		}
	}
#endif

	while ( 1 ) {
		if ( ind_int >= CBC_OPT_INT_MAX ) {
		    if ( ind_dbl >= CBC_OPT_DBL_MAX ) {
		    	break;   // Done
		    } else {
		    	cbc_dbl_options[ind_dbl] . order = cnt;
		    	ind_dbl ++;
		    }
		} else if ( ind_dbl >= CBC_OPT_DBL_MAX ) {
			cbc_int_options[ind_int] . order = cnt;
			ind_int ++;
		} else if ( STRICMP( cbc_int_options[ind_int] . aimms_name,
		                     cbc_dbl_options[ind_dbl] . aimms_name ) > 0 ) {
			cbc_dbl_options[ind_dbl] . order = cnt;
		    ind_dbl ++;
		} else {
			cbc_int_options[ind_int] . order = cnt;
			ind_int ++;
		}
		
		cnt ++;
	}
	
	cbc_options_ordered = true;
}


// The function cbc_init_global_options()
void CbcMathProgramInstance::cbc_init_global_options( void )
{
	// Set global option variables equal to their corresponding AIMMS default value.
	
	cbc_progress         = 100;
	cbc_opt_iter_limit   = CBC_INT_MAX;
    cbc_opt_time_limit   = CBC_INT_MAX;
    cbc_opt_integer_sols = -1;
    cbc_opt_priority     = AOSI_PRIO_ALWAYS;
#ifdef DEBUG
    cbc_opt_tracing      = 0;
#endif
    
    cbc_mip_absolute_gap = 1e-13;
    cbc_mip_relative_gap = 0.0;
    cbc_mip_cutoff       = AOSI_NA_REP;
    
    // Set initial solver phase.
    
    cbc_lp_phase         = AOSI_PHASE_DUAL_SIMPLEX;
}


// The function cbc_init_options()
void CbcMathProgramInstance::cbc_init_options( void )
{
    memmove( cbc_int_opt_val, cbc_int_opt_def, CBC_OPT_INT_MAX * sizeof( int    ) );
    memmove( cbc_dbl_opt_val, cbc_dbl_opt_def, CBC_OPT_DBL_MAX * sizeof( double ) );
    
    // Reset values representing CBC options.
    
    cbc_opt_status_file = cbc_int_opt_def[ CBC_OPT_STATUS_FILE ];
    
    // Set global option variables.
    
    cbc_init_global_options();
}


// The function cbc_get_options()
int CbcMathProgramInstance::cbc_get_options( void )
{
    _LONG_T   no_int, no_dbl,
              ind_int[ CBC_OPT_INT_MAX + AOSI_IOPT_MAX ],
              ind_dbl[ CBC_OPT_DBL_MAX + AOSI_DOPT_MAX ],
              opt_int[ CBC_OPT_INT_MAX + AOSI_IOPT_MAX ];
    double    opt_dbl[ CBC_OPT_DBL_MAX + AOSI_DOPT_MAX ];
    int       i, index, res = 0;
 
    m_mp->GetChangedOptions( &no_int, ind_int, opt_int, &no_dbl, ind_dbl, opt_dbl );
    
    // no_int: actual number of modified integer options.

    for ( i=0; i<no_int; i++ ) {
        index = ind_int[i];

        if ( index >= 0 ) {
            // Store CBC specific integer option.
            
            cbc_int_opt_val[index] = opt_int[i];
        } else {
            // Store global integer AIMMS option.
            switch ( index ) {
              case AOSI_IOPT_iteration_limit:
                cbc_opt_iter_limit = opt_int[i];
                if ( cbc_opt_iter_limit >= CBC_INT_MAX ) {
                    cbc_opt_iter_limit = CBC_INT_MAX;
                }
                break;
              case AOSI_IOPT_time_limit:
                cbc_opt_time_limit = opt_int[i];
                if ( cbc_opt_time_limit > CBC_INT_MAX ) {
                    cbc_opt_time_limit = CBC_INT_MAX;
                }
                break;
              case AOSI_IOPT_progress_solution:
                cbc_progress = opt_int[i];
                break;
              case AOSI_IOPT_after_integer_sol:
                cbc_opt_integer_sols = opt_int[i];
                if ( cbc_opt_integer_sols >= 9999999 ) {
                    cbc_opt_integer_sols = -1;
                }
                break;
              case AOSI_IOPT_solver_messages:
                cbc_opt_priority = opt_int[i];
                if ( cbc_opt_priority == AOSI_PRIO_ALWAYS ) {
                    cbc_opt_priority = 100;    // Pass all messages to AIMMS.
                }
                break;
#ifdef DEBUG
              case AOSI_IOPT_solver_tracing:
                cbc_opt_tracing = opt_int[i];
                cbc_last_opt_tracing = opt_int[i];
                break;
#endif               
              default:
                // Other global integer options are not used.
                break;
            }
        }
    }
	
    // no_dbl: actual number of modified double options.

    for ( i=0; i<no_dbl; i++ ) {
        index = ind_dbl[i];

        if ( index >= 0 ) {
            // Store CBC specific double option.
            
            cbc_dbl_opt_val[index] = opt_dbl[i];
        } else {
            // Store global double AIMMS option.
            switch ( index ) {
              case AOSI_DOPT_mip_absolute_opt:
                cbc_mip_absolute_gap = opt_dbl[i];
                break;
              case AOSI_DOPT_mip_relative_opt:
                cbc_mip_relative_gap = opt_dbl[i];
                break;
              case AOSI_DOPT_mip_cutoff:
                cbc_mip_cutoff = opt_dbl[i];
                if ( cbc_mip_cutoff < -1.0e30 ) {
                	cbc_mip_cutoff = -1.0e30;
                } else if ( cbc_mip_cutoff > 1.0e30 ) {
                	cbc_mip_cutoff = 1.0e30;
                }
            	break;
              default:
                // Other global double options are not used.
                break;
            }
        }
    }

    return res;
}


// The function cbc_set_options()
int CbcMathProgramInstance::cbc_set_options( std::list<std::string>& opt_list )
{
    int      i, ival;
    double   dval;
    char     value[128];
    
    // Get value of option for printing to status file.
        
    cbc_opt_status_file = cbc_int_opt_val[ CBC_OPT_STATUS_FILE ];
	
#ifdef WIN32
	if ( cbc_opt_status_file == 2 ) {
		cbc_opt_status_file = 0;
	}
#endif
    
    // Set CBC specific options. This is done by passing the options with nondefault
    // value as arguments of CbcMain1 in cbc_actually_call_solver(). Note: three is
    // added for extra arguments for CbcMain1.
    
    if ( ! cbc_args ) {
    	cbc_args = new const char*[CBC_OPT_INT_MAX+CBC_OPT_DBL_MAX+AOSI_IOPT_MAX+AOSI_DOPT_MAX+3];
    }
    
    for ( i=0; i<CBC_OPT_INT_MAX; i++ ) {
        // We skip options for which cbc_name is NULL since these are no CBC options.
        
		if ( cbc_int_options[i] . cbc_name            &&
             cbc_int_opt_val[i] != cbc_int_opt_def[i] ) {
            opt_list.push_back( cbc_int_options[i] . cbc_name );
            
            ival = cbc_int_opt_val[i];
            
            if ( cbc_int_options[i] . keywords ) {
            	if ( cbc_int_options[i] . cbc_keyword ) {
					opt_list.push_back( cbc_int_options[i] . cbc_keyword[ival] );
				} else {
					opt_list.push_back( cbc_int_options[i] . keyword[ival] );
				}
			} else {
				sprintf( value, "%d\0", ival );
				opt_list.push_back( value );
			}
        }
    }
    
    for ( i=0; i<CBC_OPT_DBL_MAX; i++ ) {
    	// We skip options for which cbc_name is NULL since these are no CBC options.
    	
        if ( cbc_dbl_options[i] . cbc_name            &&
             cbc_dbl_opt_val[i] != cbc_dbl_opt_def[i] ) {
            opt_list.push_back( cbc_int_options[i] . cbc_name );
            
			sprintf( value, "%g\0", cbc_dbl_opt_val[i] );
			opt_list.push_back( value );
        }
    }
    
    // Set LP method option (if not equal to default 'dual simplex').
    
    if ( cbc_int_opt_val[CBC_OPT_LP_METHOD] != 1 ) {
    	switch ( cbc_int_opt_val[CBC_OPT_LP_METHOD] ) {
    	  case 0:
    	  	cbc_lp_phase = AOSI_PHASE_PRIMAL_SIMPLEX;
    		strcpy( value, "-primalSimplex" );
    		break;
    	  case 2:
    	  	cbc_lp_phase = AOSI_PHASE_BARRIER;
    		strcpy( value, "-barrier" );
    		break;
    	}
    	
    	opt_list.push_back( value );
    } else {
    	cbc_lp_phase = AOSI_PHASE_DUAL_SIMPLEX;
    }
    
    // Set CBC specific options that cannot be set by passing them as argument to CbcMain1.
    
    cbc_model->setPrintFrequency( cbc_int_opt_val[CBC_OPT_MIP_PRINT_FREQ] );
    
#ifdef CBC_PARALLEL
    if ( cbc_int_opt_val[CBC_OPT_THREAD_LIMIT ] > 1 ) {
    	cbc_model->setNumberThreads ( cbc_int_opt_val[CBC_OPT_THREAD_LIMIT ] );
    }
#endif
    
    // Set AIMMS general solver options.
    
    // Set option for maximal number of iterations.
    
    if ( cbc_opt_iter_limit < CBC_INT_MAX ) {
    	opt_list.push_back( "maxIter" );
		sprintf( value, "%d\0", cbc_opt_iter_limit );
		opt_list.push_back( value );
	}
    
    // Set option for maximal number of integer solutions.
    
    if ( cbc_opt_integer_sols > 0 ) {
    	opt_list.push_back( "maxSolutions" );
		sprintf( value, "%d\0", cbc_opt_integer_sols );
		opt_list.push_back( value );
	}
    
    // Set time limit.
    
    if ( cbc_opt_time_limit >= CBC_INT_MAX ) {
    	dval = CBC_INF;
    } else {
    	dval = (double) cbc_opt_time_limit;
    }
    cbc_model->setMaximumSeconds( dval );
    
    // Set MIP option for absolute optimality tolerance.
    
    cbc_model->setAllowableGap( cbc_mip_absolute_gap );
    
    // Set MIP option for relative optimality tolerance.
    
    cbc_model->setAllowableFractionGap( cbc_mip_relative_gap );
    
    // Set MIP option for cutoff. CBC treats cutoff as if problem is a minimization problem.
    
    if ( cbc_mip_cutoff == AOSI_NA_REP ) {
    	dval = 1.0e30;
    } else {
    	if ( cbc_handle . direction == DIRECTION_MAX ) {
    		dval = cbc_handle . obj_constant - cbc_mip_cutoff;
    	} else {
    		dval = cbc_mip_cutoff - cbc_handle . obj_constant;
    	}
    }
    cbc_model->setCutoff( dval );
	
    return 0;
}


// The function GetIntegerOption()
void CbcSolverInfo::GetIntegerOption(
    _LONG_T  int_option_no,     // int_option_no, starting at 0
    char    *name,              // name must be of length < 256
    _LONG_T *values,
    _LONG_T *info )
{
	int   index, cat_no;
	
    /* Get an integer option. We strongly recommend to use the framework of
       option definitions as given at the beginning of this file. */
       
    if ( int_option_no >= CBC_OPT_INT_MAX ) {
        m_gen->PassMessage( AOSI_PRIO_ERROR,
                            "Invalid integer option number encountered" );
        return;
    }
	
    strcpy( name, cbc_int_options[int_option_no] . aimms_name );

    index = cbc_int_options[int_option_no].index;

    values[ AOSI_OPTION_LO      ] = cbc_int_opt_low[index];
    values[ AOSI_OPTION_DEFAULT ] = cbc_int_opt_def[index];
    values[ AOSI_OPTION_UP      ] = cbc_int_opt_up[index];
    values[ AOSI_OPTION_CURRENT ] = cbc_int_opt_def[index];
    
    cat_no = cbc_int_options[int_option_no].category;
#ifdef DEBUG
    if ( cat_no < 0 ) {
        m_gen->PassMessage( AOSI_PRIO_ERROR,
                            "Invalid integer option category encountered" );
        return;
    }
#endif

    info[ AOSI_OPTINFO_CAT      ] = cat_no;
    info[ AOSI_OPTINFO_INDEX    ] = cbc_int_options[int_option_no] . order;
    info[ AOSI_OPTINFO_CHANGE   ] = cbc_int_options[int_option_no] . change;
    info[ AOSI_OPTINFO_KEYWORDS ] = cbc_int_options[int_option_no] . keywords;
}


// The function GetIntegerKeyword()
void CbcSolverInfo::GetIntegerKeyword(
    _LONG_T  int_option_no,
    _LONG_T  index,
    char    *keyword )          // keyword must be of length <  37
{
    /* Get name of keyword to 'index' for integer option with number 'int_option_no'.
       We strongly recommend to use the framework of option definitions as given at
       the beginning of this file. */
    
    if ( int_option_no >= CBC_OPT_INT_MAX ) {
        m_gen->PassMessage( AOSI_PRIO_ERROR,
                            "Invalid integer option number encountered" );
        return;
    }
      
    if ( cbc_int_options[int_option_no] . keywords ) {
        strcpy( keyword, cbc_int_options[int_option_no] . keyword[index] );
    } else {
        keyword[0] = '\0';
    }
}


// The function GetDoubleOption()
void CbcSolverInfo::GetDoubleOption(
    _LONG_T  dbl_option_no,   // dbl_option_no, starting at 0
    char    *name,            // name must be of length < 256
    double  *values,
    _LONG_T *info )
{
	int   index, cat_no;
	
    /* Get a double option. We strongly recommend to use the framework of
       option definitions as given at the beginning of this file. */
    
    if ( dbl_option_no >= CBC_OPT_DBL_MAX ) {
        m_gen->PassMessage( AOSI_PRIO_ERROR,
                            "Invalid double option number encountered" );
        return;
    }
	
    strcpy( name, cbc_dbl_options[dbl_option_no] . aimms_name );

    index = cbc_dbl_options[dbl_option_no].index;

    values[ AOSI_OPTION_LO      ] = cbc_dbl_opt_low[index];
    values[ AOSI_OPTION_DEFAULT ] = cbc_dbl_opt_def[index];
    values[ AOSI_OPTION_UP      ] = cbc_dbl_opt_up[index];
    values[ AOSI_OPTION_CURRENT ] = cbc_dbl_opt_def[index];

    cat_no = cbc_dbl_options[dbl_option_no].category;
#ifdef DEBUG
    if ( cat_no < 0 ) {
        m_gen->PassMessage( AOSI_PRIO_ERROR,
                            "Invalid double option category encountered" );
        return;
    }
#endif

    info[ AOSI_OPTINFO_CAT      ] = cat_no;
    info[ AOSI_OPTINFO_INDEX    ] = cbc_dbl_options[dbl_option_no] . order;
    info[ AOSI_OPTINFO_CHANGE   ] = cbc_dbl_options[dbl_option_no] . change;
}


// The function GetCategoryName()
void CbcSolverInfo::GetCategoryName(
    _LONG_T  category,          // category number, starting at 0
    char    *name )             // name must be of length < 256
{
    /* Get a category. We strongly recommend to use the framework of option
       definitions as given at the beginning of this file. */
       
	if ( category >= CBC_CAT_MAX ) {
         m_gen->PassMessage( AOSI_PRIO_ERROR, "Invalid category number encountered" );
         return;
    }
      
    strcpy( name, cbc_category[category] );
}

