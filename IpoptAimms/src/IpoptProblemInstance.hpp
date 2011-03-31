// Copyright (C) 2009 Paragon Decision Technology B.V. and others.
// All Rights Reserved.
// This code is published under the Common Public License.
//
// $Id$
//
// Author: Chuck Teeter

#ifndef __IPOPT_PROBLEM_INSTANCE_HPP__
#define __IPOPT_PROBLEM_INSTANCE_HPP__

#include "IpTNLP.hpp"
// Needed for definition of NORM_MAX used when calculating the current constraint
// violation in intermediate_callback
#include "IpIpoptCalculatedQuantities.hpp"
#include "libIpopt.h"
#include "AimmsOSI-SolverIpoptImpl.h"



using namespace Ipopt;


class IpoptProblemInstance : public TNLP
{
  public:
  /** default constructor */
  IpoptProblemInstance( IpoptMathProgramInstance *impi );

  /** default destructor */
  virtual ~IpoptProblemInstance();

  /**@name Overloaded from TNLP */
  //@{
  /** Method to return some info about the nlp */
  virtual bool get_nlp_info( Index& n, Index& m, Index& nnz_jac_g,
                             Index& nnz_h_lag, IndexStyleEnum& index_style );

  /** Method to return the bounds for my problem */
  virtual bool get_bounds_info( Index n, Number* x_l, Number* x_u,
                                Index m, Number* g_l, Number* g_u );

  /** Method to return the starting point for the algorithm */
  virtual bool get_starting_point( Index n, bool init_x, Number* x,
                                   bool init_z, Number* z_L, Number* z_U,
                                   Index m, bool init_lambda,
                                   Number* lambda );

  /** Method to return the objective value */
  virtual bool eval_f( Index n, const Number* x, bool new_x, Number& obj_value );

  /** Method to return the gradient of the objective */
  virtual bool eval_grad_f( Index n, const Number* x, bool new_x, Number* grad_f );

  /** Method to return the constraint residuals */
  virtual bool eval_g( Index n, const Number* x, bool new_x, Index m, Number* g );

  /** Method to return:
   *   1) The structure of the jacobian (if "values" is NULL)
   *   2) The values of the jacobian (if "values" is not NULL)
   */
  virtual bool eval_jac_g( Index n, const Number* x, bool new_x,
                           Index m, Index nele_jac, Index* iRow, Index *jCol,
                           Number* values );

  /** Method to return:
   *   1) The structure of the hessian of the lagrangian (if "values" is NULL)
   *   2) The values of the hessian of the lagrangian (if "values" is not NULL)
   */
  virtual bool eval_h( Index n, const Number* x, bool new_x,
                       Number obj_factor, Index m, const Number* lambda,
                       bool new_lambda, Index nele_hess, Index* iRow,
                       Index* jCol, Number* values );


  //@}

  /** @name Solution Methods */
  //@{
  /** This method is called when the algorithm is complete so the TNLP can store/write the solution */
  virtual void finalize_solution( SolverReturn status,
                                  Index n, const Number* x, const Number* z_L, const Number* z_U,
                                  Index m, const Number* g, const Number* lambda,
                                  Number obj_value,
				                  const IpoptData* ip_data,
				                  IpoptCalculatedQuantities* ip_cq );

  /** This method is called once per iteration to provide the current status of the solution process.  The information
   *  provided by the entities in the argument list correspond to what Ipopt prints in the iteration summary.  More detailed 
   *  information is available from the ip_data and ip_cq objects.  The default (base) implementation always returns true.  If
   *  false is returned by this override, then Ipopt will terminate with the 'User_Requested_Stop' status. */
  virtual bool intermediate_callback( AlgorithmMode mode, Index iter, Number obj_value, Number inf_pr, Number inf_du, Number mu,
									  Number d_norm, Number regularization_size, Number alpha_du, Number alpha_pr, Index ls_trials,
									  const IpoptData* ip_data, IpoptCalculatedQuantities* ip_cq );


   /** This method is called by Ipopt only if the limited memory quasi-Newton option is used.  It is used to return the number of variables 
    *  that appear nonlinearly in the objective function or at least one constraint function.  If a negative number is returned, Ipopt assumes
	*  that all variables are nonlinear.  The default implementation is to return -1; ie, all variables are assumed to be nonlinear. */

	virtual Index get_number_of_nonlinear_variables();


   /** This method is called by Ipopt only if the limited memory quasi-Newton option is used, and if the get_number_of_nonlinear_variables()
    *  function returns a positive number.  This number is then identical to 'num_nonlin_vars' and the length of the array 'pos_nonlin_vars'.
	*  In this call, you need to list the indices of all nonlinear variables in 'pos_nonlin_vars', where the numbering starts with zero or one,
	* depending on the numbering style determined in get_nlp_info(). */

	virtual bool get_list_of_nonlinear_variables( Index num_nonlin_vars, Index* pos_nonlin_vars );

  
  //@}

protected:
	// Pointer to math program instance class
	IpoptMathProgramInstance *m_impi;

private:

	IPOPT_handle_struct        *IPOPT_current;    // Pointer to structure holding IPOPT problem data
	
	bool      is_linear_model, obj_is_nonlinear;

    int       i, ncols, nrows, nelem, nlin, nhess;
    int      *rows_lin, *cols_lin, *lin_ind;
    int      *hrows, *hcols;
    int       cnt_lin;

    double   *lb, *ub, *lev, *row_lb, *row_ub, *row_lev, *fv, *coef;
    double   *coef_lin;

    _LONG_T  *cols, *rows, *col_flag, *row_flag, *nlflag;
    
    double   *obj_coef;
    int      *obj_flag;

    int      *jac_start, *jac_map;
    bool     *lin_row_ind;

	// Used to hold variable data for the Jacobian evaluation function
	double   *localJac;		// Used in Aimms_Calculate_Jacobian()
	
	_LONG_T   direction;

	_LONG_T   ProgressIterationCount;	// Used to determine when a solution progress update should be made to AIMMS

	
	// Evaluates the objective value
	int    Aimms_Calculate_Objective( const Number* x, double *funcval );
	
	// Evaluates the constraint values
    int    Aimms_Calculate_Rows( Number* g, const Number* x );

	// Evaluates or returns the structure of the Jacobian
    int    Aimms_Calculate_Jacobian( const Number* x, Number* jacValues );

	// Evaluates or returns the structure of the Hessian
	int    Aimms_Calculate_Hessian( int nhess, double obj_factor, const Number* x, const Number* hWeights,
	                                Number* hvalues );


	/**@name Methods to block default compiler methods.
	 * The compiler automatically generates the following three methods.
	 *  Since the default compiler implementation is generally not what
	 *  you want (for all but the most simple classes), we usually 
	 *  put the declarations of these methods in the private section
	 *  and never implement them. This prevents the compiler from
	 *  implementing an incorrect "default" behavior without us
	 *  knowing. (See Scott Meyers book, "Effective C++")
	 *  
	 */
	//@{
	//  IpoptProblemInstance();
	IpoptProblemInstance( const IpoptProblemInstance& );
	IpoptProblemInstance& operator=( const IpoptProblemInstance& );
	//@}
};


#endif
