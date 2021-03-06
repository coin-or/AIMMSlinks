// Copyright (C) 2009 AIMMS B.V. and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// $Id$
//
// Author: Marcel Hunting

#include "cbc_interface.h"
#include "cbc_event.h"
#include "AimmsOSI-SolverCbcImpl.h"


//-------------------------------------------------------------------
// Default Constructor 
//-------------------------------------------------------------------
MyEventHandler3::MyEventHandler3 () 
  : CbcEventHandler()
{
}

//-------------------------------------------------------------------
// Copy constructor 
//-------------------------------------------------------------------
MyEventHandler3::MyEventHandler3 (const MyEventHandler3 & rhs) 
: CbcEventHandler(rhs)
{
}

// Constructor with pointer to model
MyEventHandler3::MyEventHandler3(CbcModel * model, CbcMathProgramInstance *cmpi)
  : CbcEventHandler(model),
    cmpi_(cmpi)
{
}

//-------------------------------------------------------------------
// Destructor 
//-------------------------------------------------------------------
MyEventHandler3::~MyEventHandler3 ()
{
}

//----------------------------------------------------------------
// Assignment operator 
//-------------------------------------------------------------------
MyEventHandler3 &
MyEventHandler3::operator=(const MyEventHandler3& rhs)
{
  if (this != &rhs) {
    CbcEventHandler::operator=(rhs);
  }
  return *this;
}
//-------------------------------------------------------------------
// Clone
//-------------------------------------------------------------------
CbcEventHandler * MyEventHandler3::clone() const
{
  MyEventHandler3 *event = new MyEventHandler3(*this);
  event->cmpi_ = cmpi_;
  return event;
}

CbcEventHandler::CbcAction 
MyEventHandler3::event(CbcEvent whichEvent)
{
	double   dum;
	int      res;
	bool     do_progress = false;
	
	CbcMathProgramInstance *cmpi = GetCmpi();
	cbc_handle_struct *cbc_current = cmpi->cbc_current;
	
#ifdef CBC_PARALLEL
	MUTEX_ACCESS;
#endif
	
	// Pass phase if changed.
	
	if ( whichEvent == node              ||
	     whichEvent == solution          ||
	     whichEvent == heuristicSolution ) {
		if ( cbc_current->phase != AOSI_PHASE_MIP ) {
			cbc_current->phase = AOSI_PHASE_MIP;
			
			cmpi->get_callback()->PassPhaseInfo( cbc_current->phase );
		}
	} else if ( cbc_current->phase != AOSI_PHASE_MIP ) {
		if ( cbc_current->phase != cmpi->cbc_lp_phase ) {
	    	cbc_current->phase = cmpi->cbc_lp_phase;
	    
        	cmpi->get_callback()->PassPhaseInfo( cbc_current->phase );
        }
    }
	
	// The events 'afterHeuristic' and 'endSearch' report different iterations.
	
	if ( ( whichEvent != afterHeuristic ) &&
	     ( whichEvent != endSearch      ) ) {
		cbc_current->iter = model_->getIterationCount();
	}
	
	// Pass progress information to AIMMS (after a certain interval of iterations
	// as determined by the AIMMS general solvers option 'progress solution').
	
	if ( ( cmpi->cbc_progress                                ) &&
	     ( cbc_current->iter >= cmpi->cbc_next_to_print_iter ) ) {
		do_progress = true;
		
		if ( cbc_current->iter == cmpi->cbc_next_to_print_iter ) {
            cmpi->cbc_next_to_print_iter += cmpi->cbc_progress;
        } else {
            dum = ceil( (double) cbc_current->iter / (double) cmpi->cbc_progress );
            cmpi->cbc_next_to_print_iter = cmpi->cbc_progress * (int) dum;
        }
	}
	
	if ( cmpi->cbc_progress_interval ) {
		int cur_time = cmpi->get_mp()->GetTime();
		
		if ( cur_time > cmpi->cbc_next_to_print_time ) {
			do_progress = true;
			
			cmpi->cbc_next_to_print_time = cur_time + cmpi->cbc_progress_interval;
		}
	}
	
	if ( ( cmpi->cbc_progress_interval                  ) ||
	     ( cmpi->cbc_progress                           ) ||
	     ( cbc_current->cb_flags & AOSI_CB_INT_SOLUTION ) ) {
	    if ( ( whichEvent == solution          ) ||
		     ( whichEvent == heuristicSolution ) ) {
			// The incumbent solution is that of the preprocessed model. The optimal
			// solution might be different from the last incumbent solution because
			// CBC performs a postsolve step before returning the optimal solution
			// (which is not done for the incumbent solutions).
	    	
	    	cmpi->cbc_incumbent       = model_->bestSolution();
	    	cmpi->cbc_original_cols   = model_->originalColumns();
	    	cmpi->cbc_nr_preproc_cols = model_->getNumCols();
	    	
    		cbc_current->obj_best        = cbc_current->obj_multiplier * model_->getObjValue();
    		cbc_current->mip_is_feasible = true;
    		cbc_current->phase           = AOSI_PHASE_MIP;
    	}
    	
    	cbc_current->nodes         = model_->getNodeCount();
    	cbc_current->mip_best_poss = cbc_current->obj_multiplier * model_->getBestPossibleObjValue();
    }
	
	if ( do_progress ) {
    	cmpi->cbc_progress_info( false );
        
        // Call the ProcessEvents method regularly in order to let AIMMS check for
        // a user interrupt, and to handle any pending window related messages.
        
        res = cmpi->get_callback()->ProcessEvents();
        if ( res == AOSI_PROG_RET_INTR ) {
        	cmpi->cbc_user_interrupt = true;
        	return stop;
        }
    }
    
    if ( cbc_current->iter >= cmpi->cbc_opt_iter_limit ) {
    	return stop;
    }
    
    // Pass incumbent solution to AIMMS if one has been found and the callback
    // has been installed in AIMMS.
    
    if ( ( whichEvent == solution || whichEvent == heuristicSolution ) &&
	     ( cbc_current->cb_flags & AOSI_CB_INT_SOLUTION              ) ) {
#ifdef DEBUG
		res = cmpi->cbc_is_feasible_solution();
#endif
	    
	    res = cmpi->cbc_aimms_callback( AOSI_CB_INT_SOLUTION );
	    if ( res == AOSI_CB_RET_INTR ) {
	    	cmpi->cbc_user_interrupt = true;
	    	return stop;
	    }
	}
	
	if ( cbc_current->cb_flags & AOSI_CB_ITERATION ) {
    	if ( ( cbc_current->cb_iter                      ) &&
    	     ( cbc_current->iter >= cbc_current->cb_iter ) ) {
    		cbc_current->obj_best        = cbc_current->obj_multiplier * model_->getObjValue();
    		cbc_current->nodes           = model_->getNodeCount();
    		cbc_current->mip_best_poss   = cbc_current->obj_multiplier * model_->getBestPossibleObjValue();
	    	
	    	res = cmpi->cbc_aimms_callback( AOSI_CB_ITERATION );
	    	if ( res == AOSI_CB_RET_INTR ) {
	    		cmpi->cbc_user_interrupt = true;
	    		return stop;
	    	}
    	}
  	}
	
	return noAction;   // Carry on
}

CbcMathProgramInstance * MyEventHandler3::GetCmpi()
{
	return cmpi_;
}



//-------------------------------------------------------------------
// Default Constructor 
//-------------------------------------------------------------------
MyEventHandler::MyEventHandler () 
  : ClpEventHandler()
{
}

//-------------------------------------------------------------------
// Copy constructor 
//-------------------------------------------------------------------
MyEventHandler::MyEventHandler (const MyEventHandler & rhs) 
: ClpEventHandler(rhs)
{  
}

// Constructor with pointer to model
MyEventHandler::MyEventHandler(CbcMathProgramInstance *cmpi)
  : cmpi_(cmpi)
{
}

//-------------------------------------------------------------------
// Destructor 
//-------------------------------------------------------------------
MyEventHandler::~MyEventHandler ()
{
}

//----------------------------------------------------------------
// Assignment operator 
//-------------------------------------------------------------------
MyEventHandler &
MyEventHandler::operator=(const MyEventHandler& rhs)
{
  if (this != &rhs) {
    ClpEventHandler::operator=(rhs);
  }
  return *this;
}
//-------------------------------------------------------------------
// Clone
//-------------------------------------------------------------------
ClpEventHandler * MyEventHandler::clone() const
{
  MyEventHandler *event = new MyEventHandler(*this);
  event->cmpi_ = cmpi_;
  return event;
}

int 
MyEventHandler::event(Event whichEvent)
{
	double   dum;
	int      res;
	bool     do_progress;
	
	if ( whichEvent != endOfIteration ) {
		return -1;
	}
	
	do_progress = false;
	
	CbcMathProgramInstance *cmpi = GetCmpi();
	cbc_handle_struct *cbc_current = cmpi->cbc_current;
	
#ifdef CBC_PARALLEL
	MUTEX_ACCESS;
#endif
	
	// It seems that when solving a MIP, this callback is called while solving
	// the LP on the nodes. The simplex iterations used to solve the nodes are
	// not seen as MIP iterations by CBC and therefore we should not use those
	// in the progress updates.
	
	if ( cbc_current->model_type == CBC_MIP_MODEL ) {
		// Pass phase if changed.
	
		if ( ( cbc_current->phase != AOSI_PHASE_MIP     ) &&
		     ( cbc_current->phase != cmpi->cbc_lp_phase ) ) {
		    cbc_current->phase = cmpi->cbc_lp_phase;
		    
    	    cmpi->get_callback()->PassPhaseInfo( cbc_current->phase );
    	}
		
		if ( cmpi->cbc_progress_interval ) {
			int cur_time = cmpi->get_mp()->GetTime();
			
			if ( cur_time > cmpi->cbc_next_to_print_time ) {
				do_progress = true;
				
				cmpi->cbc_next_to_print_time = cur_time + cmpi->cbc_progress_interval;
			}
		}
		
		if ( do_progress ) {
			// How to get information about the objective value and the sum of
			// infeasibilities from CLP? Maybe by using the messages printed
			// to CoinMessageHandler but that is ugly.
    		
    		cmpi->cbc_progress_info( false );
    	    
    	    // Call the ProcessEvents method regularly in order to let AIMMS check for
    	    // a user interrupt, and to handle any pending window related messages.
    	    
    	    res = cmpi->get_callback()->ProcessEvents();
    	    if ( res == AOSI_PROG_RET_INTR ) {
    	    	cmpi->cbc_user_interrupt = true;
    	    	return 5;   // Stop
    	    }
    	}
	} else {
		// Pass phase if changed.
		
		if ( cbc_current->phase != cmpi->cbc_lp_phase ) {
		    cbc_current->phase = cmpi->cbc_lp_phase;
		    
    	    cmpi->get_callback()->PassPhaseInfo( cbc_current->phase );
    	}
    	
    	// Pass progress information to AIMMS (after a certain interval of iterations
		// as determined by the AIMMS general solvers option 'progress solution').
		
		cbc_current->iter ++;
		
		if ( ( cmpi->cbc_progress                                ) &&
		     ( cbc_current->iter >= cmpi->cbc_next_to_print_iter ) ) {
			do_progress = true;
			
			if ( cbc_current->iter == cmpi->cbc_next_to_print_iter ) {
    	        cmpi->cbc_next_to_print_iter += cmpi->cbc_progress;
    	    } else {
    	        dum = ceil( (double) cbc_current->iter / (double) cmpi->cbc_progress );
    	        cmpi->cbc_next_to_print_iter = cmpi->cbc_progress * (int) dum;
    	    }
		}
		
		if ( cmpi->cbc_progress_interval ) {
			int cur_time = cmpi->get_mp()->GetTime();
			
			if ( cur_time > cmpi->cbc_next_to_print_time ) {
				do_progress = true;
				
				cmpi->cbc_next_to_print_time = cur_time + cmpi->cbc_progress_interval;
			}
		}
		
		if ( do_progress ) {
			// How to get information about the objective value and the sum of
			// infeasibilities from CLP? Maybe by using the messages printed
			// to CoinMessageHandler but that is ugly.
    		
    		cmpi->cbc_progress_info( false );
    	    
    	    // Call the ProcessEvents method regularly in order to let AIMMS check for
    	    // a user interrupt, and to handle any pending window related messages.
    	    
    	    res = cmpi->get_callback()->ProcessEvents();
    	    if ( res == AOSI_PROG_RET_INTR ) {
    	    	cmpi->cbc_user_interrupt = true;
    	    	return 5;   // Stop
    	    }
    	}
    	
    	if ( cbc_current->iter >= cmpi->cbc_opt_iter_limit ) {
    		return 5;   // Stop
    	}
    	
    	if ( cbc_current->cb_flags & AOSI_CB_ITERATION ) {
    		if ( ( cbc_current->cb_iter                      ) &&
    		     ( cbc_current->iter >= cbc_current->cb_iter ) ) {
		    	res = cmpi->cbc_aimms_callback( AOSI_CB_ITERATION );
		    	if ( res == AOSI_CB_RET_INTR ) {
		    		cmpi->cbc_user_interrupt = true;
		    		return 5;   // Stop
		    	}
    		}
  		}
  	}
	
	return -1;   // Carry on
}


CbcMathProgramInstance * MyEventHandler::GetCmpi()
{
	return cmpi_;
}

