// ACE400_ShortTest.h : main header file for the ACE400_ShortTest application
//

#if !defined(AFX_ACE400_ShortTest_H__CB3F7139_0C6D_46B6_A240_563945A8EA0D__INCLUDED_)
#define AFX_ACE400_ShortTest_H__CB3F7139_0C6D_46B6_A240_563945A8EA0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CACE400_ShortTestApp:
// See ACE400_ShortTest.cpp for the implementation of this class
//

class CACE400_ShortTestApp : public CWinApp
{
public:
	CACE400_ShortTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACE400_ShortTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CACE400_ShortTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACE400_ShortTest_H__CB3F7139_0C6D_46B6_A240_563945A8EA0D__INCLUDED_)
