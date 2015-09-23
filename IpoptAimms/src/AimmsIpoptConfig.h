// Copyright (C) 2009 AIMMS B.V. and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// $Id$
//
// Author: Chuck Teeter, Marcel Hunting

/*
 * Include file for the configuration of the AIMMS - IPOPT link.
 *
 * This configuration file is needed because most of the linear solvers
 * have similar options, but different default values.  By defining which
 * linear solver is present in the link, the options can be tailored to
 * the specific configuration.
 */

#ifndef __AIMMSIPOPTCONFIG_H__
#define __AIMMSIPOPTCONFIG_H__


/* Linear solver availability */


/* Define to 1 if MA27 is available */
#define MA27_IS_AVAILABLE 1

/* Define to 1 if MA57 is available */
#define MA57_IS_AVAILABLE 1

/* Define to 1 if MUMPS is available */
#define MUMPS_IS_AVAILABLE 1

/* Define to 1 if Pardiso is available */
#define PARDISO_IS_AVAILABLE 1

/* Define to 1 if you are using the parallel version of Pardiso */
#define PARDISO_PARALLEL_IS_AVAILABLE 1

/* Define to 1 if TAUCS is available */
// #define TAUCS_IS_AVAILABLE 1

/* Define to 1 if WSMP is available */
// #define WSMP_IS_AVAILABLE



/* Availability of the Harwell Subroutine MC19 for scaling of the linear
   systems before they are passed to the linear solver. This may be
   particularly useful if IPOPT is used with MA27 or MA57. However, it is
   not required to have MC19 to compile IPOPT; if this routine is missing,
   the scaling is never performed. */


/* Define to 1 if MC19 is available */
#define MC19_IS_AVAILABLE 1


#endif /* __AIMMSIPOPTCONFIG_H__ */
