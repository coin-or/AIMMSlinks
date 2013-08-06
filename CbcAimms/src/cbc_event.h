// Copyright (C) 2009 Paragon Decision Technology B.V. and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// $Id$
//
// Author: Marcel Hunting

#ifndef _DEFINED_CBC_EVENT_H
#define _DEFINED_CBC_EVENT_H

class MyEventHandler3 : public CbcEventHandler {
  
public:
	/**@name Overrides */
	//@{
	virtual CbcAction event(CbcEvent whichEvent);
	//@}
	
	/**@name Constructors, destructor etc*/
	//@{
	/** Default constructor. */
	MyEventHandler3();
	/// Constructor with pointer to model (redundant as setEventHandler does)
	MyEventHandler3(CbcModel * model, CbcMathProgramInstance *cmpi);
	/** Destructor */
	virtual ~MyEventHandler3();
	/** The copy constructor. */
	MyEventHandler3(const MyEventHandler3 & rhs);
	/// Assignment
	MyEventHandler3& operator=(const MyEventHandler3 & rhs);
	/// Clone
	virtual CbcEventHandler * clone() const ;
	//@}
	CbcMathProgramInstance * GetCmpi();

protected:
	CbcMathProgramInstance *cmpi_;
};


class MyEventHandler : public ClpEventHandler {
  
public:
	/**@name Overrides */
	//@{
	virtual int event(Event whichEvent);
	//@}
	
	/**@name Constructors, destructor etc*/
	//@{
	/** Default constructor. */
	MyEventHandler();
	/// Constructor with pointer to model (redundant as setEventHandler does)
	MyEventHandler(CbcMathProgramInstance *cmpi);
	/** Destructor */
	virtual ~MyEventHandler();
	/** The copy constructor. */
	MyEventHandler(const MyEventHandler & rhs);
	/// Assignment
	MyEventHandler& operator=(const MyEventHandler & rhs);
	/// Clone
	virtual ClpEventHandler * clone() const ;
	//@}
	CbcMathProgramInstance * GetCmpi();

protected:
	CbcMathProgramInstance *cmpi_;
};

#endif // _DEFINED_CBC_EVENT_H

