// hierarchy.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "hierarchy.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHierarchyApp

BEGIN_MESSAGE_MAP(CHierarchyApp, CWinApp)//hierarchyd的信息处理
	//{{AFX_MSG_MAP(CHierarchyApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CHierarchyApp::CHierarchyApp()//chierachyapp的构建与解构
{
	

/////////////////////////////////////////////////////////////////////////////
// 唯一的HierarchyApp 对象

CHierarchyApp theApp;



BOOL CHierarchyApp::InitInstance()
{
	AfxEnableControlContainer();

	// 状态初始化
	
#ifdef _AFXDLL
	Enable3dControls();			// 在共享的dll中使用mfc时调用
#else
	Enable3dControlsStatic();	//静态链接mfc时调用
#endif

	
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));//注册帧


	//主窗口对象
	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// 创建和调用帧，联通他的资源

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);




	//穿件显示唯一的主窗口 
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}

//信息处理

class CAboutDlg : public CDialog
{//定义对话框
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard生成，虚函数将被重写
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// 实现
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CHierarchyApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



