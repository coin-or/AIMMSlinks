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
#include "libIpopt.h"


//@< The function |IpoptProblemInstance::Constructor()| @> @;
//@< The function |IpoptProblemInstance::Destructor()| @> @;
//@< The function |get_nlp_info()| @> @;
//@< The function |get_bounds_info()| @> @;
//@< The function |get_starting_point()| @> @;
//@< The function |eval_f()| @> @;
//@< The function |eval_grad_f()| @> @;
//@< The function |eval_g()| @> @;
//@< The function |eval_jac_g()| @> @;
//@< The function |eval_h()| @> @;
//@< The function |finalize_solution()| @> @;
//@< The function |intermediate_callback()| @> @;
//@< The function |get_number_of_nonlinear_variables()| @> @;
//@< The function |get_list_of_nonlinear_variables()| @> @;

//******** AIMMS helper functions ********
//@< The function |Aimms_Calculate_Objective()| @> @;
//@< The function |Aimms_Calculate_Rows()| @> @;
//@< The function |Aimms_Calculate_Jacobian()| @> @;
//@< The function |Aimms_Calculate_Hessian()| @> @;


using namespace Ipopt;



//@< The function |IpoptProblemInstance::Constructor()| @> @;
IpoptProblemInstance::IpoptProblemInstance( IpoptMathProgramInstance *impi )
{
	// Constructor
	
	m_impi = impi;
	
	IPOPT_current = &(m_impi->IPOPT_handle);
	
	is_linear_model  = IPOPT_current->is_linear_model;
	obj_is_nonlinear = IPOPT_current->obj_is_nonlinear;
	
	direction = IPOPT_current->direction;

    ncols     = IPOPT_current->ncols;           // Number of columns
    nrows     = IPOPT_current->nrows;           // Number of rows
    nelem     = IPOPT_current->nonzeros;        // Number of nonzero in Jacobian
    nhess     = IPOPT_current->nhess;
	
	// Initialize the pointers to point to applicable parts of the retrieved model.
	
    lb        = (double  *) IPOPT_current->model_area;	   // Cols
    ub        = (double  *) ( lb       + ncols );
    lev       = (double  *) ( ub       + ncols );
    row_lb    = (double  *) ( lev      + ncols );          // Rows
    row_ub    = (double  *) ( row_lb   + nrows );
    row_lev   = (double  *) ( row_ub   + nrows );
    fv        = (double  *) ( row_lev  + nrows );
    coef      = (double  *) ( fv       + nrows );          // Matrix coefs
    col_flag  = (_LONG_T *) ( coef     + nelem );          // Cols
    row_flag  = (_LONG_T *) ( col_flag + ncols );          // Rows
    cols      = (_LONG_T *) ( row_flag + nrows );          // Matrix coefs
    rows      = (_LONG_T *) ( cols     + nelem );
    nlflag    = (_LONG_T *) ( rows     + nelem );

    // YDL : for Hessian information
    hcols     = (int     *) ( nlflag   + nelem );   
    hrows     = (int     *) ( hcols    + nhess );
    
    // Part containing objective row elements 
    obj_coef = (double *) IPOPT_current->obj_area;
    obj_flag = (int    *) ( obj_coef + ncols );
	
    // Linear part of the Jacobian   
    nlin = IPOPT_current->n_lin_elem;
	
    coef_lin = (double *) IPOPT_current->linear_area;
    rows_lin = (int    *) ( coef_lin + nlin );
    cols_lin = (int    *) ( rows_lin + nlin );
    lin_ind  = (int    *) ( cols_lin + nlin );
    
    // Part containing Jacobian elements
    jac_start   = (int  *) IPOPT_current->jac_area;
    jac_map     = (int  *) ( jac_start + (nrows+1) );
    lin_row_ind = (bool *) ( jac_map   + nelem     );
	
	// CET: Allocate memory for the Jacobian evaluation function.
	// Add one extra for the objective column.
	
	localJac = NULL;
	localJac = (double *) m_impi->get_mp()->AllocateMemory( (ncols + 1) * sizeof( double ) );

	// Initialize the progress iteration counter.
	
	ProgressIterationCount = 0;
}


//@< The function |IpoptProblemInstance::Destructor()| @> @;
IpoptProblemInstance::~IpoptProblemInstance()
{
	// Destructor

	// CET: Release allocated memory for the Jacobian evaluation function.
	
	if ( localJac ) {
		m_impi->get_mp()->FreeMemory( localJac );
	}
}


//@< The function |get_nlp_info()| @> @;
bool IpoptProblemInstance::get_nlp_info( Index& n, Index& m, Index& nnz_jac_g, Index& nnz_h_lag, IndexStyleEnum& index_style )
{
	// This procedure returns the size of the problem.
	
	// Here we can use the newly instantiated IPOPT_handle structure to set the problem size.

	// The number of variables.
	n = ncols;

	// The number of constraints.
	m = nrows;

	// The number of nonzeros in the Jacobian.
	nnz_jac_g = nelem;

	// The number of elements in the lower left corner of the Hessian.
	nnz_h_lag = nhess;

	// Use the C style indexing (0-based).
	index_style = TNLP::C_STYLE;

	return true;
}


//@< The function |get_bounds_info()| @> @;
bool IpoptProblemInstance::get_bounds_info( Index n, Number* x_l, Number* x_u,
											Index m, Number* g_l, Number* g_u )
{
	// This procedure returns the variable and constraint bounds.
	
	// Set variable lower and upper bounds.
	
	for ( i=0; i<ncols; i++ ) {
		// Lower bounds
		x_l[ i ] = lb[ i ];

		// Upper bounds
        x_u[ i ] = ub[ i ];
	}
         
	// Set constraint upper and lower bounds.
	
    for ( i=0; i<nrows; i++ ) {
		// Lower bounds - see note below for upper bounds
		g_l[ i ] = row_lb[ i ];
		
		// Upper bounds
		// The default threshold for IPOPT is 1.0e19; any value above
		// this is interpreted by IPOPT as infinity.
		g_u[ i ] = row_ub[ i ];
    }

	return true;
}


//@< The function |get_starting_point()| @> @;
bool IpoptProblemInstance::get_starting_point( Index n, bool init_x, Number* x,
											   bool init_z, Number* z_L, Number* z_U,
											   Index m, bool init_lambda,
										 	   Number* lambda )
{
	// This procedure returns the initial point for the problem.
	
	assert( init_x == true );
	assert( init_z == false );
	assert( init_lambda == false );
	
	for ( i=0; i<ncols; i++ ) {
		x[ i ] = lev[ i ];
	}
	
	return true;
}


//@< The function |eval_f()| @> @;
bool IpoptProblemInstance::eval_f( Index n, const Number* x, bool new_x, Number& obj_value )
{
	int      errcnt;
	double   funcval;
	
	// This procedure returns the value of the objective function.
	
	// Noth much to do for NLS.
	
	if ( direction == AOSI_DIRECTION_FEAS ) {
		obj_value = 0.0;
		return true;
	}
	
	// Calculate the objective value for IPOPT.
	// If the first argument of EvaluateRow equals -1 then the objective function will be used.
    
    errcnt = Aimms_Calculate_Objective( x, &funcval );
    
    // Multiply by -1 in case of maximization.
    
    if ( direction == AOSI_DIRECTION_MAX ) {
    	obj_value = -funcval;
    } else {
    	obj_value = funcval;
    }
    
    // Check whether the total amount of (domain) evaluation errors is larger than the limit.
    // If yes, stop solve.
    IPOPT_current->dom_err_cnt += errcnt;
	if ( IPOPT_current->dom_err_cnt > m_impi->ipopt_opt_dom_limit ) {
		return false;
	}

	return true;
}


//@< The function |eval_grad_f()| @> @;
bool IpoptProblemInstance::eval_grad_f( Index n, const Number* x, bool new_x, Number* grad_f )
{
	double   funcval;
	int      i, errcnt;
	
	// This procedure returns the gradient of the objective function grad_{x} f(x).
	
    memset( grad_f, 0, n * sizeof( Number ) );
    
    // Nothing to do anymore for NLS.
    
    if ( direction == AOSI_DIRECTION_FEAS ) {
    	return true;
    }
    
    if ( obj_is_nonlinear ) {
    	// Calculate the gradient of the objective.
    	// If the first argument of EvaluateDiffRow equals -1 then the objective function will be used.
		
    	errcnt = m_impi->get_evaluator()->EvaluateDiffRow( -1, (double *) x, /* new_point */ new_x ? 1 : 0,
    	                                                   grad_f, &funcval );
    } else {
    	errcnt = 0;
    }
    
    // Calculate linear part of Jacobian of objective.
    
    for ( i=0; i<ncols; i++ ) {
      	if ( ! obj_flag[i] ) {
      		// Linear objective coefficient
	   	    grad_f[i] = obj_coef[i];
	   	}
	}
	
	// Multiply by -1 in case of maximization.
	
	if ( direction == AOSI_DIRECTION_MAX ) {
		for ( i=0; i<ncols; i++ ) {
      		grad_f[i] *= -1.0;
		}
	}
    
    // Check whether the total amount of (domain) evaluation errors is larger than the limit.
    // If yes, stop solve.
    IPOPT_current->dom_err_cnt += errcnt;
	if ( IPOPT_current->dom_err_cnt > m_impi->ipopt_opt_dom_limit ) {
		return false;
	}

    return true;
}


//@< The function |eval_g()| @> @;
bool IpoptProblemInstance::eval_g( Index n, const Number* x, bool new_x, Index m, Number* g )
{
	// This procedure returns the value of the constraints: g(x).
	
	// Calculate the function values.
	int errcnt = Aimms_Calculate_Rows( g, x );
	
	// Check whether the total amount of (domain) evaluation errors is larger than the limit.
    // If yes, stop solve.
    IPOPT_current->dom_err_cnt += errcnt;
	if ( IPOPT_current->dom_err_cnt > m_impi->ipopt_opt_dom_limit ) {
		return false;
	}

	return true;
}


//@< The function |eval_jac_g()| @> @;
bool IpoptProblemInstance::eval_jac_g( Index n, const Number* x, bool new_x,
									   Index m, Index nele_jac, Index* iRow, Index *jCol,
									   Number* values )
{
	// This procedure returns the structure or values of the Jacobian or evaluates
	// the Jacobian.
	
	if ( values == NULL ) {
		
		// Return the structure of the Jacobian.
    	           
    	for ( i=0; i<nelem; i++ ) {
			iRow[ i ] = rows[ i ];
    	    jCol[ i ] = cols[ i ];
    	}
    	
	} else {
		
		// Evaluate the Jacobian.
		int errcnt = Aimms_Calculate_Jacobian( x, values );
		
		// Check whether the total amount of (domain) evaluation errors is larger than the limit.
    	// If yes, stop solve.
    	IPOPT_current->dom_err_cnt += errcnt;
		if ( IPOPT_current->dom_err_cnt > m_impi->ipopt_opt_dom_limit ) {
			return false;
		}
		
	}

	return true;
}


//@< The function |eval_h()| @> @;
bool IpoptProblemInstance::eval_h( Index n, const Number* x, bool new_x,
								   Number obj_factor, Index m, const Number* lambda,
								   bool new_lambda, Index nele_hess, Index* iRow,
								   Index* jCol, Number* values )
{
	// This procedure returns the structure or values of the hessian, or evaluates
	// the Hessian.
	
	if ( values == NULL ) {
		
		// Return the structure of the Hessian.
		
	  	for ( i = 0; i < nhess; i++ ) {
			iRow[ i ] = hrows[ i ];
	    	jCol[ i ] = hcols[ i ];
		}
		
	} else {
		
		// Evaluate the Hessian - no need to execute this if there are no elements.
		
		if ( nhess > 0 ) {
		    // Evaluate the Hessian.
		    int errcnt = Aimms_Calculate_Hessian( nhess, obj_factor, x, lambda, values );
			
			// Check whether the total amount of (domain) evaluation errors is larger than the limit.
    		// If yes, stop solve.
    		IPOPT_current->dom_err_cnt += errcnt;
			if ( IPOPT_current->dom_err_cnt > m_impi->ipopt_opt_dom_limit ) {
				return false;
			}
		}
		
	}
	
	return true;
}


//@< The function |finalize_solution()| @> @;
void IpoptProblemInstance::finalize_solution( SolverReturn status,
											  Index n, const Number* x, const Number* z_L, const Number* z_U,
											  Index m, const Number* g, const Number* lambda,
											  Number obj_value, const IpoptData* ip_data,
											  IpoptCalculatedQuantities* ip_cq )
{
	
    int       i, ncols, nrows;
    double   *x_sol, *mult_x_L, *mult_x_U, *g_sol, *mult_g;
    double   *lb, *ub, *lev, *row_lb, *row_ub;
    
    // YDL: modified for Aimms.
    // --------------------------------------------------------------------------------
    // Assign related member of IPOPT_handle to transfer the final solution to Aimms.
    
    IPOPT_current->n = n;
    IPOPT_current->m = m;
    
    // The final objective value. If this is a maximization problem, we'll
	// need to reverse the sign.
	
	if ( direction == AOSI_DIRECTION_MAX ) {
		IPOPT_current->obj = -obj_value;
	} else {
		IPOPT_current->obj = obj_value;
	}
    
    ncols = IPOPT_current->ncols;       // Number of columns
    nrows = IPOPT_current->nrows;       // Number of rows
    
    x_sol    = (double *) IPOPT_current->solution;
    mult_x_L = (double *) ( x_sol    + ncols );
    mult_x_U = (double *) ( mult_x_L + ncols );
    g_sol    = (double *) ( mult_x_U + ncols );
    mult_g   = (double *) ( g_sol    + nrows );

    // In case of an equality row, IPOPT seems to subtract the right-hand-side
    // value from the row level value.
    
    lb     = (double *) IPOPT_current->model_area;
    ub     = (double *) ( lb     + ncols );
    lev    = (double *) ( ub     + ncols );
    row_lb = (double *) ( lev    + ncols );
    row_ub = (double *) ( row_lb + nrows );
    
    // Column solution.

    for ( i=0; i<n; i++ ) {
        x_sol[ i ] = x[ i ];
        mult_x_L[ i ] = z_L[ i ];
        mult_x_U[ i ] = z_U[ i ];
    }
    
    // Row solution.
    
    for ( i=0; i<m; i++ ) {
        g_sol[ i ] = g[ i ];
        
        // Multiply row marginal by -1 for minimization.
        
        if ( direction == AOSI_DIRECTION_MAX ) {
        	mult_g[ i ] = lambda[ i ];
        } else {
        	mult_g[ i ] = - lambda[ i ];
        }
        
        if ( row_lb[ i ] >= row_ub[ i ] ) {
        	g_sol[ i ] += row_lb[ i ];
        }
    }
    
    // Get the current constraint violation.
    
	IPOPT_current->constr_viol = ip_cq->unscaled_curr_nlp_constraint_violation( NORM_MAX );
	
	// Get the model and solver status. For models that did not finish in a normal way
	// (e.g., hitting time limit, user interrupt) the model status depends on the current
	// solution being feasible or not.
	
    m_impi->IPOPT_Get_Solution_Status( (int) status, x_sol, g_sol );
    
#ifdef NEVER
    // It seems that the x and obj_value as passed to finalize_solution do not always match.
    // It happened that the objective value was calculated in the last call to eval_f for a
    // certain x, and that the same objective value was passed to finalize_solution but the
    // x was slightly changed between the last call to eval_f and the call to finalize_solution.
    
    if ( IPOPT_current->model_status == AOSI_MODELSTAT_LOCAL_OPT     ||
         IPOPT_current->model_status == AOSI_MODELSTAT_INTER_NON_OPT ) {
        double   funcval;
    	Aimms_Calculate_Objective( x_sol, &funcval );
    	
    	IPOPT_current->obj = funcval;
    }
#endif
}


//@< The function |intermediate_callback()| @> @;
bool IpoptProblemInstance::intermediate_callback( AlgorithmMode mode, Index iter, Number obj_value, Number inf_pr, Number inf_du, Number mu,
	  											  Number d_norm, Number regularization_size, Number alpha_du, Number alpha_pr, Index ls_trials,
												  const IpoptData* ip_data, IpoptCalculatedQuantities* ip_cq )
{
    // If this is the first iteration, reset the progress iteration counter to zero.
    if ( iter == 0 ) {
		ProgressIterationCount = 0;
	} else {
		// Increment the counter.
		ProgressIterationCount ++;
	}

	// Update the iteration count in IPOPT handle.
	IPOPT_current->iter = iter;

	// See if it's time to report progress back to AIMMS.
	if ( ProgressIterationCount == m_impi->ipopt_opt_sol_progress ) {
		// Reset the counter, and report the current iteration data.
		ProgressIterationCount     = 0;
		
		// Update the sum of the primal infeasibilities.
		IPOPT_current->sum_inf     = inf_pr;
		
		// The current objective value. If this is a maximization problem, we'll
		// need to reverse the sign.
		
		if ( direction == AOSI_DIRECTION_MAX ) {
			IPOPT_current->obj     = -obj_value;
		} else {
			IPOPT_current->obj     = obj_value;
		}
		
		// Get the current constraint violation.
		IPOPT_current->constr_viol = ip_cq->unscaled_curr_nlp_constraint_violation( NORM_MAX );

		// Update the AIMMS progress window.
		m_impi->IPOPT_Progress_Info( /* end */ false );
		
		// Call the ProcessEvents method regularly in order to let AIMMS check for
        // a user interrupt, and to handle any pending window related messages.
        int res = m_impi->get_callback()->ProcessEvents();
        if ( res == AOSI_PROG_RET_INTR ) {
        	return false;   // IPOPT will terminate
        }
	}

	return true;
}


//@< The function |get_number_of_nonlinear_variables()| @> @;
Index IpoptProblemInstance::get_number_of_nonlinear_variables()
{
	return IPOPT_current->n_nl_cols;
}


//@< The function |get_list_of_nonlinear_variables()| @> @;
bool IpoptProblemInstance::get_list_of_nonlinear_variables( Index num_nonlin_vars, Index* pos_nonlin_vars )
{
	int        i, cnt;
    _LONG_T   *nl_obj_cols;
    bool      *nl_cols;
    
    nl_cols = (bool *) m_impi->get_mp()->AllocateMemory( ncols * sizeof( bool ) );
    if ( ! nl_cols ) {
    	return false;
    }
    
    memset( nl_cols, 0, ncols * sizeof( bool ) );
    
    nl_obj_cols = (_LONG_T *) m_impi->get_mp()->AllocateMemory( ncols * sizeof( _LONG_T ) );
    if ( ! nl_obj_cols ) {
    	m_impi->get_mp()->FreeMemory( nl_cols );
    	return false;
    }
    
    // Get nonlinear columns in objective.
    
    cnt = m_impi->get_mp()->GetNonlinearObjectiveColumns( ncols, nl_obj_cols );
    
    for ( i=0; i<cnt; i++ ) {
    	nl_cols[ nl_obj_cols[ i ] ] = true;
    }
    
    // Get nonlinear columns in matrix.
    
    for ( i=0; i<nelem; i++ ) {
    	if ( nlflag[ i ] ) {
    		nl_cols[ cols[ i ] ] = true;
    	}
    }
    
    cnt = 0;
    for ( i=0; i<ncols; i++ ) {
    	if ( nl_cols[i] ) {
    		pos_nonlin_vars[cnt] = i;
    		cnt ++;
    	}
    }
    
    assert( cnt == IPOPT_current->n_nl_cols );
    
    m_impi->get_mp()->FreeMemory( nl_cols     );
    m_impi->get_mp()->FreeMemory( nl_obj_cols );

	return true;
}


//@< The function |Aimms_Calculate_Objective()| @> @;
int IpoptProblemInstance::Aimms_Calculate_Objective( const Number* x, double *funcval )
{
	int   i, errcnt;
	
	// Calculate the objective value for IPOPT.
	
	errcnt = 0;
	
	// Calculate nonlinear part of objective value.
	
	if ( obj_is_nonlinear ) {
		// If the first argument of EvaluateRow equals -1 then the objective function will be used.
    
    	errcnt = m_impi->get_evaluator()->EvaluateRow( -1, (double *) x, /* new_point */ 1, funcval );
    } else {
    	*funcval = 0.0;
    }
    
    // Add linear part of objective value.
    
    for ( i=0; i<ncols; i++ ) {
        if ( ! obj_flag[ i ] ) {
        	// Linear objective coefficient
        	*funcval += obj_coef[ i ] * x[ i ];
        }
    }
    
    return errcnt;
}


//@< The function |Aimms_Calculate_Rows()| @> @;
int IpoptProblemInstance::Aimms_Calculate_Rows( Number* g, const Number* x )
{
    int      i, errcnt, new_point;
    double   funcval;

    errcnt = 0;
    
    // Calculate nonlinear parts of the rows.
    
    if ( is_linear_model ) {
    	memset( g, 0, nrows * sizeof( Number ) );
    } else {
    	new_point = 1;
    	
    	for ( i=0; i<nrows; i++ ) {
    	    errcnt += m_impi->get_evaluator()->EvaluateRow( i, (double *) x, new_point, &funcval );
    	    g[ i ] = (Number) funcval;
    	    
    	    new_point = 0;
    	}
    }
	
	// Add linear parts of the rows.
    
    for ( i=0; i<nlin; i++ ) {
        g[ rows_lin[ i ] ] += (double) coef_lin[ i ] * x[ cols_lin[ i ] ];
    }
	
    return errcnt;
}


//@< The function |Aimms_Calculate_Jacobian()| @> @;
int IpoptProblemInstance::Aimms_Calculate_Jacobian( const Number* x, Number* jacValues )
{
    int      i, j, errcnt, new_point;
    double   funcval;
    
    errcnt    = 0;
    new_point = 1;
    
    // Calculate nonlinear part of Jacobian of the rows.
	
	for ( i=0; i<nrows; i++ ) {
		if ( ! lin_row_ind[ i ] ) {
			// Nonlinear row
			errcnt += m_impi->get_evaluator()->EvaluateDiffRow( i, (double *) x, new_point, localJac, &funcval );
			
			new_point = 0;
		
			for ( j=jac_start[ i ]; j<jac_start[ i+1 ]; j++ ) {
				assert( 0 <= jac_map[ j ] && jac_map[ j ] <= ncols-1 );
				
				jacValues[ j ] = localJac[ jac_map[ j ] ];
			}
		}
	}
	
	// Add linear part of Jacobian of rows.
	
	for ( i=0; i<nlin; i++ ) {
		jacValues[ lin_ind[ i ] ] = coef_lin[ i ];
	}
	
    return errcnt;
}


//@< The function |Aimms_Calculate_Hessian()| @> @;
int IpoptProblemInstance::Aimms_Calculate_Hessian( int nhess, double obj_factor, const Number *x,
                                                   const Number* hWeights, Number* hvalues )
{
    int   errcnt;
    
    if ( direction == AOSI_DIRECTION_MAX ) {
    	obj_factor *= -1.0;
    }

    errcnt = m_impi->get_evaluator()->EvaluateHessian( (double *) x, (double *) hWeights, nhess,
                                                       (double *) hvalues, obj_factor );
	
    return errcnt;
}

