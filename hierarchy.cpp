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

BEGIN_MESSAGE_MAP(CHierarchyApp, CWinApp)//hierarchyd����Ϣ����
	//{{AFX_MSG_MAP(CHierarchyApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CHierarchyApp::CHierarchyApp()//chierachyapp�Ĺ�����⹹
{
	

/////////////////////////////////////////////////////////////////////////////
// Ψһ��HierarchyApp ����

CHierarchyApp theApp;



BOOL CHierarchyApp::InitInstance()
{
	AfxEnableControlContainer();

	// ״̬��ʼ��
	
#ifdef _AFXDLL
	Enable3dControls();			// �ڹ����dll��ʹ��mfcʱ����
#else
	Enable3dControlsStatic();	//��̬����mfcʱ����
#endif

	
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));//ע��֡


	//�����ڶ���
	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// �����͵���֡����ͨ������Դ

	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);




	//������ʾΨһ�������� 
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
}

//��Ϣ����

class CAboutDlg : public CDialog
{//����Ի���
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard���ɣ��麯��������д
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// ʵ��
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



