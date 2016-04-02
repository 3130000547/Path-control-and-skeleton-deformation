// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "hierarchy.h"
#include "ChildView.h"
#include <vector>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()//创建窗口，这个窗口包含绘制帧，开始帧，显示帧的函数
: m_bDrawFrame(false), m_bStartFrame(FALSE),
m_ptOld(0,0), m_ptCur(0,0), m_nNumStickLists(0), m_bShowFrame(0),
m_bRotate(0), m_bTrans(0)
{
	memset(m_StickLists,0,sizeof(Stick*)*50);
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )//定义一系列信息事件
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_COMMAND(ID_BEGINFRAME, OnBeginFrame)
	ON_UPDATE_COMMAND_UI(ID_BEGINFRAME, OnUpdateUI)
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(IDC_FRAME, OnFrameEnd)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_SHOWFRAME, OnShowFrame)
	ON_UPDATE_COMMAND_UI(ID_SHOWFRAME, OnUpdateUI)
	ON_COMMAND_EX(ID_ROTATE,OnTransTools)
	ON_COMMAND_EX(ID_TRANS, OnTransTools)
	ON_UPDATE_COMMAND_UI(ID_ROTATE, OnUpdateUI)
	ON_UPDATE_COMMAND_UI(ID_TRANS, OnUpdateUI)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView 信息处理

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) //创建新窗口
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::DrawLine(CDC *pDC, CPoint pt1, CPoint pt2)//画线
{
	if (!m_bShowFrame)
	{
		pDC->MoveTo(pt1);
		pDC->LineTo(pt2);
	}
	else
	{
		CPoint dir=pt2-pt1;
		int temp = dir.x;
		dir.x=dir.y;
		dir.y=-temp;
		temp = (int)sqrtf((float)(dir.x*dir.x+dir.y*dir.y));//两点之间的距离
		dir.x*=5;
		dir.y*=5;
		if (temp)
		{
			dir.x/=temp;//放大五倍，但同除以距离，定义比较稳定的骨骼宽度
			dir.y/=temp;
		}
		pDC->MoveTo(pt1+dir);//显示骨骼时调用的函数
		pDC->LineTo(pt2+dir);
		pDC->MoveTo(CPoint(pt1-dir));
		pDC->LineTo(CPoint(pt2-dir));
		pDC->MoveTo(pt1+dir);
		pDC->LineTo(CPoint(pt1-dir));
		pDC->MoveTo(pt2+dir);
		pDC->LineTo(CPoint(pt2-dir));//绘制骨骼长方形
	}
}

void CChildView::OnPaint() //绘制
{
	CPaintDC dc(this); // device context for painting
	//绘制的设备信息
	
	// TODO: Add your message handler code here
	//双缓冲绘制结果
	CRect rect;
	GetClientRect(rect);
	CBitmap bitmap;//贴图，可以选择
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	memDC.SelectObject(&bitmap);
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(255,0,0));//红色的笔
	memDC.SelectObject(&pen);
	memDC.FillSolidRect(rect, RGB(255, 255, 255));//用白色填充
	//绘制光标
	m_cursor.DrawCursor(&memDC, this);
	matrix3x3 mout;
	Stick *pStickPtr;
	CPoint points[255];
	int nNumPoints;
	for (int i=0;i<m_nNumStickLists+1;i++)
	{
		pStickPtr = m_StickLists[i];
		mout.identity();
		nNumPoints=0;
		while (pStickPtr)
		{
			mout.lmul(pStickPtr->offsetmatrix);
			points[nNumPoints]=mout.transform(CPoint(0,0));
			pStickPtr = pStickPtr->next;
			nNumPoints++;
		}
		
		if (nNumPoints>1)
		{
			for (int j=0;j<nNumPoints-1;j++)
			{
				DrawLine(&memDC, points[j], points[j+1]);
			}
		}
	}

	if (m_bDrawFrame && m_bStartFrame)//如果的绘制帧或者是开始帧，画线
	{
		DrawLine(&memDC, m_ptOld, m_ptCur);
		mout.identity();
		Stick *pStickPtr = m_StickLists[m_nNumStickLists];
		if (pStickPtr->next)
		{
			while (pStickPtr->next)//
			{
				mout.lmul(pStickPtr->offsetmatrix);
				pStickPtr = pStickPtr->next;
			}
			
			CPoint ptOriginTrans = mout.transform(CPoint(0,0));
			DrawLine(&memDC, ptOriginTrans, m_ptOld);
		}
	}

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);

	// Do not call CWnd::OnPaint() for painting messages
}

void CChildView::OnBeginFrame()//开始帧
{
	m_bDrawFrame=!m_bDrawFrame;
	if (!m_bDrawFrame)
		m_bStartFrame = FALSE;
	else
	{
		m_bRotate = FALSE;//不可以旋转或平移
		m_bTrans = FALSE;
	}
}

void CChildView::OnUpdateUI(CCmdUI *pCmdUI)//更新ui
{
	if (pCmdUI->m_nID == ID_BEGINFRAME)
		pCmdUI->SetCheck(m_bDrawFrame);//h绘制帧
	else if(pCmdUI->m_nID == ID_SHOWFRAME)
		pCmdUI->SetCheck(m_bShowFrame);//显示帧
	else if(pCmdUI->m_nID == ID_ROTATE)//旋转
		pCmdUI->SetCheck(m_bRotate);
	else if(pCmdUI->m_nID == ID_TRANS)//平移
		pCmdUI->SetCheck(m_bTrans);
}

BOOL CChildView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) //设置光标
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDrawFrame)
		SetClassLong(m_hWnd, GCL_HCURSOR, (LONG)m_hCursors[1]);
	else
		SetClassLong(m_hWnd, GCL_HCURSOR, (LONG)m_hCursors[0]);
	return CWnd ::OnSetCursor(pWnd, nHitTest, message);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) //创建childView结构
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_hCursors[0] = (HCURSOR)GetClassLong(m_hWnd, GCL_HCURSOR);
	m_hCursors[1] = LoadCursor(NULL, IDC_CROSS);
	// 
	m_Popup.LoadMenu(IDR_POPUP);
	m_cursor.m_eState = CUR_ROTATE;
	return 0;
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point) //右键鼠标抬起
{
	
	if (m_bDrawFrame & m_bStartFrame)
	{
		CMenu *pSubMenu = m_Popup.GetSubMenu(0);
		CPoint pt(point);
		ClientToScreen(&pt);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN,pt.x,pt.y,this);//出现提示菜单
	}
	CWnd ::OnRButtonUp(nFlags, point);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point) //左键鼠标按下
{
	//
	if (m_bDrawFrame)
	{
		m_bStartFrame = TRUE;
	
		Stick *pStickPtr = m_StickLists[m_nNumStickLists];
		while (pStickPtr && pStickPtr->next) pStickPtr = pStickPtr->next;
		if (!pStickPtr)
			m_StickLists[m_nNumStickLists] = new Stick;
		else
		{
			pStickPtr->next = new Stick;	
			matrix3x3 mout,m;
			mout.identity();
			Stick *pStickPtrTmp = m_StickLists[m_nNumStickLists];
			while (pStickPtrTmp != pStickPtr)
			{
				m.inverse(pStickPtrTmp->offsetmatrix);//转置的目的是
				mout.mul(m);
				pStickPtrTmp = pStickPtrTmp->next;
			}
			CPoint ptOldTrans=mout.transform(m_ptOld);
			CPoint pointTrans=mout.transform(point);			
			CPoint pt = pointTrans-ptOldTrans;
			pStickPtr->offsetmatrix.rotate(atan2f((float)pt.y, (float)pt.x));//定义旋转时鼠标按下的行为
			pStickPtr->offsetmatrix.translate((float)ptOldTrans.x,(float)ptOldTrans.y);		//定义平移式鼠标右键按下的行为
		}
		m_ptOld = point;
	}

	else if (m_bRotate || m_bTrans)
	{
		Stick *pStickPtr = NULL;
		int nPrecision = !m_bShowFrame?2:5;
		CPoint pt;
		for (int i=0;i<m_nNumStickLists; i++)
		{
			pt = point;
			if (pStickPtr = m_StickLists[i]->ClickDetect(pt, nPrecision))
			{
				m_cursor.m_ptPos = m_StickLists[i]->GetPointHierarchy(pStickPtr, CPoint(0,0));
				m_cursor.m_StickLock = pStickPtr;
				m_cursor.m_curStickList = i;
				OnMouseMove(0, point);//触发鼠标经过事件，在按左键后捕捉这一事件来决定旋转的角度和平移的量

				break;
			}
		}
		if (m_cursor.PointDetected(point))
		{
			m_cursor.m_ptDragStart = point;
			m_cursor.m_bDrag = TRUE;
			matrix3x3 mTrans;
			mTrans.identity();
			m_ptTrans = m_cursor.m_StickLock->offsetmatrix.transform(CPoint(0,0));
			mTrans.translate(-m_ptTrans.x, -m_ptTrans.y);
			m_mRotate = m_cursor.m_StickLock->offsetmatrix;
			m_mRotate.mul(mTrans);
		}
	}

	CWnd ::OnLButtonDown(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)//左键鼠标抬起
{
	m_cursor.m_bDrag = FALSE;
	Invalidate();
	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnFrameEnd()
{//定义结束帧
	m_bStartFrame = FALSE;
	Stick *pStickPtr = m_StickLists[m_nNumStickLists];
	matrix3x3 mout,m;
	mout.identity();
	while (pStickPtr->next)//指针都向下一层移位
	{
		m.inverse(pStickPtr->offsetmatrix);
		mout.mul(m);
		pStickPtr = pStickPtr->next;
	}
	CPoint ptOldTrans=mout.transform(m_ptOld);
	pStickPtr->offsetmatrix.translate((float)ptOldTrans.x, (float)ptOldTrans.y);
	m_nNumStickLists++;
	Invalidate();
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point) //鼠标经过事件
{
	// TODO: Add your message handler code here and/or call default
	m_ptCur=point;
	if (m_bDrawFrame && m_bStartFrame)
		Invalidate();
	else if (m_bRotate)
	{
		m_cursor.m_eState = CUR_ROTATE;
		if (m_cursor.m_bDrag)
		{
			CPoint dir = m_cursor.m_ptDragStart - m_cursor.m_ptPos;//拖拽起点减去终点
			int nTemp = dir.y;
			dir.y = dir.x;
			dir.x = -nTemp;
			float fNormal = sqrtf(dir.x*dir.x+dir.y*dir.y);//单位化
			float fDirX=(float)dir.x/fNormal;
			float fDirY=(float)dir.y/fNormal;
			CPoint ptDir = point-m_cursor.m_ptDragStart;
			float Dot = ptDir.x*fDirX+ptDir.y*fDirY;
			m_cursor.m_StickLock->offsetmatrix.identity().mul(m_mRotate).rotate(Dot/100.0f).translate(m_ptTrans.x, m_ptTrans.y);
		}//偏移和旋转矩阵层级变换
		Invalidate();
	}
	else if (m_bTrans)
	{
		m_cursor.m_eState = CUR_TRANSLATE;
		if (m_cursor.m_bDrag && m_cursor.m_curStickList != -1)
		{
			vector<Stick*> PtrStack;//存入堆栈
			Stick* StickPtr = m_StickLists[m_cursor.m_curStickList];
			PtrStack.clear();
			while (StickPtr!=m_cursor.m_StickLock)
			{
				PtrStack.push_back(StickPtr);
				StickPtr = StickPtr->next;
			}
			
			float eps;
			int MAX_NUM = 20;//最大20个组件
			int num=0;//计数器
			do 
 			{
				eps = 0.0f;
				CPoint ptGoal = point;
				for (vector<Stick*>::reverse_iterator it=PtrStack.rbegin(); it!=PtrStack.rend(); ++it)
				{
					matrix3x3 mout, mout2;
					mout.identity();
					StickPtr = m_StickLists[m_cursor.m_curStickList];
					while (StickPtr != (*it)->next)
					{
						mout.lmul(StickPtr->offsetmatrix);
						StickPtr = StickPtr->next;
					}
					CPoint ptStart = mout.transform(CPoint(0,0));//偏移，旋转矩阵 的具体变换
					mout2 = mout;
					mout2.lmul(StickPtr->offsetmatrix);
					CPoint ptEnd = mout2.transform(CPoint(0,0));
					CPoint Dir1 = ptEnd - ptStart;//逆向
					float rad1 = atan2f(Dir1.y, Dir1.x);
					CPoint Dir2 = ptGoal - ptStart;
					float rad2 = atan2f(Dir2.y, Dir2.x);
					float radinc = rad2 - rad1;
					eps += fabsf(radinc);
					float xtrans = (*it)->offsetmatrix(2,0);
					float ytrans = (*it)->offsetmatrix(2,1);
					(*it)->offsetmatrix(2,0)=0;
					(*it)->offsetmatrix(2,1)=0;
					(*it)->offsetmatrix.rotate(radinc);
					(*it)->offsetmatrix.translate(xtrans, ytrans);
					if ((*it)->next->next)
					{
						xtrans = (*it)->next->offsetmatrix(2,0);
						ytrans = (*it)->next->offsetmatrix(2,1);
						(*it)->next->offsetmatrix(2,0)=0;
						(*it)->next->offsetmatrix(2,1)=0;
						(*it)->next->offsetmatrix.rotate(-radinc);
						(*it)->next->offsetmatrix.translate(xtrans, ytrans);
					}

					mout.identity();
					StickPtr = m_StickLists[m_cursor.m_curStickList];
					while (StickPtr != (*it)->next)
					{
						mout.lmul(StickPtr->offsetmatrix);
						StickPtr = StickPtr->next;
					}
					ptStart = mout.transform(CPoint(0,0));
					mout2 = mout;
					mout2.lmul(StickPtr->offsetmatrix);
					ptEnd = mout2.transform(CPoint(0,0));
					Dir1 = ptEnd - ptStart;

					ptGoal-=Dir1;
					num++;
				}
			} while (eps > 0.1f && num<MAX_NUM);
			m_cursor.m_ptPos = point;
		}
		Invalidate();
	}
	CWnd ::OnMouseMove(nFlags, point);
}

void CChildView::OnShowFrame()//显示帧
{
	m_bShowFrame = !m_bShowFrame;
	Invalidate();
}

void CChildView::OnTransTools(UINT nID)
{//工具切换，激活和非激活状态，决定于按钮按下与否
	m_bDrawFrame=FALSE;
	if (m_bStartFrame)
		OnFrameEnd();
	switch (nID)
	{
	case ID_ROTATE:
		m_bRotate = !m_bRotate;
		m_bTrans = m_bRotate?FALSE:m_bTrans;
		break;
	case ID_TRANS:
		m_bTrans = !m_bTrans;
		m_bRotate = m_bTrans?FALSE:m_bRotate;
		break;
	default:
		break;
	}
}
