// hierarchy.h : 

#if !defined(AFX_HIERARCHY_H__5289203F_EEFF_4710_8717_B43FC1F1C7A3__INCLUDED_)
#define AFX_HIERARCHY_H__5289203F_EEFF_4710_8717_B43FC1F1C7A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"      
/////////////////////////////////////////////////////////////////////////////
// CHierarchyApp:
//

class CHierarchyApp : public CWinApp//只是构建了cgierachyapp
{
public:
	CHierarchyApp();

///覆盖
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHierarchyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL


public:
	//{{AFX_MSG(CHierarchyApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_HIERARCHY_H__5289203F_EEFF_4710_8717_B43FC1F1C7A3__INCLUDED_)
