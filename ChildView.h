// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__119127E4_A8C5_4B0E_B9C2_D8CA2EC731A8__INCLUDED_)
#define AFX_CHILDVIEW_H__119127E4_A8C5_4B0E_B9C2_D8CA2EC731A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <math.h>
/////////////////////////////////////////////////////////////////////////////
// CChildView window
class matrix3x3
{
public:
	union{
		float m[9];
		struct  
		{
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
		};
	};

	matrix3x3() {}
	matrix3x3(float _11, float _12, float _13,
			float _21, float _22, float _23,
			float _31, float _32, float _33)
			:_11(_11), _12(_12), _13(_13),
			_21(_21), _22(_22), _23(_23),
			_31(_31), _32(_32), _33(_33) {}
	
	float operator()(UINT nrow, UINT ncol) const
	{
		return m[nrow*3+ncol];
	}

	float &operator()(UINT nrow, UINT ncol)
	{
		return m[nrow*3+ncol];
	}

	matrix3x3 &mul(float o)
	{
		for (int i=0;i<9;i++)
			m[i]*=o;
		return *this;
	}

	matrix3x3 &mul(matrix3x3 &o1, matrix3x3 &o2)
	{
		float fSum;
		for (int i=0; i<3; i++)
		{
			for (int j=0; j<3; j++)
			{
				fSum=0;
				for (int k=0; k<3; k++)
				{
					fSum += o1(i,k)*o2(k,j);
				}
				(*this)(i,j)=fSum;
			}
		}
		return *this;
	}
	
	matrix3x3 &mul(matrix3x3 &o)
	{
		return mul(matrix3x3(*this), o);
	}

	matrix3x3 &lmul(matrix3x3 &o)
	{
		return mul(o, matrix3x3(*this));
	}

	matrix3x3 &identity()
	{
		for (int i=0; i<9; i++)
			m[i]=0;
		for (i=0; i<3; i++)
			(*this)(i,i)=1.0f;
		return *this;
	}

	matrix3x3 &translate(float x, float y)//平移矩阵
	{
		matrix3x3 m;
		m.identity();
		m._31 = x;
		m._32 = y;
		return mul(m);
	}

	matrix3x3 &rotate(float ang)//旋转矩阵
	{
		matrix3x3 m;
		m.identity();
		m._11 = cosf(ang); m._12=sinf(ang);
		m._21 =-sinf(ang); m._22=cosf(ang);
		return mul(m);
	}

	CPoint transform(CPoint pt)//转换矩阵
	{
		float x, y, z;
		x=pt.x*_11+pt.y*_21+_31;
		y=pt.x*_12+pt.y*_22+_32;
		z=pt.x*_13+pt.y*_23+_33;
		return CPoint((int)(x/z),(int)(y/z));
	}

	float det()
	{
		return _11*_22*_33+_12*_23*_31+_13*_21*_32-_11*_23*_32-_12*_21*_33-_13*_22*_31;
	}

	matrix3x3 &adjoint(matrix3x3 &o)
	{
		_11 = o(1,1)*o(2,2)-o(1,2)*o(2,1);
		_12 = o(0,2)*o(2,1)-o(0,1)*o(2,2);
		_13 = o(0,1)*o(1,2)-o(0,2)*o(1,1);
		_21 = o(1,2)*o(2,0)-o(1,0)*o(2,2);
		_22 = o(0,0)*o(2,2)-o(0,2)*o(2,0);
		_23 = o(0,2)*o(1,0)-o(0,0)*o(1,2);
		_31 = o(1,0)*o(2,1)-o(1,1)*o(2,0);
		_32 = o(0,1)*o(2,0)-o(0,0)*o(2,1);
		_33 = o(0,0)*o(1,1)-o(0,1)*o(1,0);
		return *this;
	}

	bool inverse(matrix3x3 &o)
	{
		float det = o.det();
		if (!det)
			return false;
		else
		{
			adjoint(o);
			mul(1.0f/det);
			return true;
		}
	}
};

struct Stick
{
	matrix3x3 offsetmatrix;
	Stick *next;
	Stick() : next(0) { offsetmatrix.identity(); }
	~Stick()
	{ 
		if (next)
			delete next;
	}

	Stick *ClickDetect(CPoint &point, int nPrecision = 1)//鼠标点击的检测函数
	{
		//matrix3x3 mInverse;
		//mInverse.inverse(offsetmatrix);
		//point = mInverse.transform(point);
// 		if (next)
// 		{
// 			CPoint pt = next->offsetmatrix.transform(CPoint(0,0));
// 			if (point.x >= 0 && point.x < pt.x
// 				&& abs(point.y) <= nPrecision)
// 			{
// 				return this;
// 			}
// 			return next->ClickDetect(point, nPrecision);
// 		}	
// 		else
// 		{
// 			if (point.x >= 0 && point.x < 4
// 				&& abs(point.y) <= nPrecision)
// 			{
// 				return this;
// 			}
// 			return NULL;
// 		}

		matrix3x3 mout1, mout2;
		mout1.identity();
		mout2.identity();
		Stick *pStickPtr = this;
		mout2.lmul(pStickPtr->offsetmatrix);
		CPoint pt1, pt2;
		while (pStickPtr)
		{
			mout1.lmul(pStickPtr->offsetmatrix);
			if (pStickPtr->next)
				mout2.lmul(pStickPtr->next->offsetmatrix);
			else
			{
				matrix3x3 mTmp;
				mTmp.translate(4,0);
				mout2.lmul(mTmp);
			}
			pt1 = mout1.transform(CPoint(0,0));
			pt2 = mout2.transform(CPoint(0,0));
			
			
			if (Dot(pt1, pt2, point) >=0
				&& Dot(pt2, pt1, point) >=0)
			{
				CPoint dir = pt2 - pt1;
				int d = dir.y*point.x-dir.x*point.y-dir.y*pt1.x+dir.x*pt1.y;
				float fValue = (float)d/sqrtf(dir.x*dir.x+dir.y*dir.y);
				if (abs(fValue)<=nPrecision)
					return pStickPtr;
			}
			pStickPtr = pStickPtr->next;
		}
		return NULL;
	}

	CPoint GetPointHierarchy(Stick *pStick, CPoint point)//获得点的等级
	{
		Stick *pStickPtr = this;
		matrix3x3 mout;
		mout.identity();
		while (pStickPtr && pStickPtr != pStick)
		{
			mout.lmul(pStickPtr->offsetmatrix);
			pStickPtr = pStickPtr->next;
		}
		if (pStickPtr)
			mout.lmul(pStickPtr->offsetmatrix);
		return mout.transform(point);
	}

	int Dot(CPoint pt1, CPoint pt2, CPoint point)
	{
		CPoint p1 = pt2 - pt1;
		CPoint p2 = point - pt1;
		return (p1.x*p2.x + p1.y*p2.y);
	}
};

enum eCursorState
{
	CUR_ROTATE=0,
	CUR_TRANSLATE,
	CUR_INVISIBLE
};

class CCursor//光标相关行为
{
public:
	CCursor() : m_eState(CUR_INVISIBLE), m_nArrowSize(5), m_nPrecision(3), m_nSize(50), m_ptPos(0,0), m_bDrag(0), m_curStickList(-1) {}
	CCursor(eCursorState eCurState, int nPrecision = 1) : m_eState(eCurState), m_nPrecision(nPrecision), m_bDrag(0), m_curStickList(-1) {}
	virtual ~CCursor() {}
	
	void DrawCursor(CDC *pDC, CWnd *pWnd)
	{
		if (m_eState == CUR_INVISIBLE)
			return;
		CPen pen, *pOldPen;
		CPoint point;
		GetCursorPos(&point);
		pWnd->ScreenToClient(&point);
		if (m_bDrag)
		{
			pen.CreatePen(PS_SOLID, 1, RGB(0,255,0));
		}
		else
		{
			if (PointDetected(point))
				pen.CreatePen(PS_SOLID, 1, RGB(0,0,255));
			else
				pen.CreatePen(PS_SOLID, 1, RGB(255,0,0));
		}
		
		pOldPen = pDC->SelectObject(&pen);
		
		if (m_eState == CUR_ROTATE)
		{
			CRect rect(-m_nSize,-m_nSize,m_nSize,m_nSize);
			rect.OffsetRect(m_ptPos);
			pDC->Ellipse(rect);

			if (m_bDrag)
			{
				CPoint pt = m_ptDragStart-m_ptPos;
				int nTemp = pt.y;
				pt.y = pt.x;
				pt.x = -nTemp;
				float fNormal = sqrtf(pt.x*pt.x+pt.y*pt.y);
				pt.x*=(40.0f/fNormal);
				pt.y*=(40.0f/fNormal);
				if (Cross(m_ptPos, m_ptDragStart, point)<0)
					pt=-pt;
				pDC->MoveTo(m_ptDragStart);
				pDC->LineTo(m_ptDragStart+pt);

			}
		}
		else if (m_eState == CUR_TRANSLATE)
		{
			pDC->MoveTo(m_ptPos);
			pDC->LineTo(m_ptPos+CPoint(m_nSize,0));
			pDC->MoveTo(m_ptPos);
			pDC->LineTo(m_ptPos+CPoint(0, -m_nSize));

			CPoint pt[3];
			pt[0]=m_ptPos+CPoint(m_nSize,0);
			pt[1]=m_ptPos+CPoint(m_nSize-m_nArrowSize, m_nArrowSize/2);
			pt[2]=m_ptPos+CPoint(m_nSize-m_nArrowSize, -m_nArrowSize/2);
			pDC->Polygon(pt, 3);
			
			pt[0]=m_ptPos-CPoint(0, m_nSize);
			pt[1]=m_ptPos+CPoint(-m_nArrowSize/2,m_nArrowSize-m_nSize);
			pt[2]=m_ptPos+CPoint(m_nArrowSize/2,m_nArrowSize-m_nSize);
			pDC->Polygon(pt, 3);
		}

		pDC->SelectObject(pOldPen);
	}//鼠标的转态 拖动链接 点击等

	
	bool PointDetected(CPoint point)//点的检测
	{
		if (m_eState == CUR_INVISIBLE)
			return false;
		else if (m_eState == CUR_ROTATE)
		{
			CPoint ptTmp = point - m_ptPos;
			float fLength = sqrtf(ptTmp.x*ptTmp.x+ptTmp.y*ptTmp.y);
			if (fLength <= (m_nSize+m_nPrecision) && fLength >= (m_nSize-m_nPrecision))
				return true;
		}
		else if (m_eState == CUR_TRANSLATE)
		{
			int nSize = m_nSize - m_nArrowSize;
			if (nSize <0)
				return false;
			CPoint ptArrowPos1 = m_ptPos + CPoint(nSize, 0);
			CPoint ptArrowPos2 = m_ptPos - CPoint(0, nSize);

			if (IsInTriangle(ptArrowPos1+CPoint(0,m_nArrowSize/2), ptArrowPos1-CPoint(0,m_nArrowSize/2), m_ptPos+CPoint(m_nSize,0), point))
				return true;
			if (IsInTriangle(ptArrowPos2+CPoint(m_nArrowSize/2,0), ptArrowPos2-CPoint(0,m_nArrowSize/2),m_ptPos-CPoint(0,m_nSize), point))
				return true;

			if (Dot(m_ptPos, ptArrowPos1, point) >=0
				&& Dot(ptArrowPos1, m_ptPos, point) >=0)
			{
				if (point.y >= (m_ptPos.y-m_nPrecision)
					&& point.y <= (m_ptPos.y+m_nPrecision))
				{
					return true;
				}
			}

			if (Dot(m_ptPos, ptArrowPos2, point) >=0
				&& Dot(ptArrowPos2, m_ptPos, point) >=0)
			{
				if (point.x>=(m_ptPos.x-m_nPrecision)
					&& point.x<=(m_ptPos.x+m_nPrecision))
					return true;
			}
		}
		return false;
	}

	int Dot(CPoint pt1, CPoint pt2, CPoint point)
	{
		CPoint p1 = pt2 - pt1;
		CPoint p2 = point - pt1;

		return (p1.x*p2.x + p1.y*p2.y);
	}

	int Cross(CPoint pt1, CPoint pt2, CPoint point)//交错
	{
		CPoint p1, p2;
		p1 = pt2-pt1;
		p2 = point - pt2;
		return (p1.x*p2.y-p1.y*p2.x);
	}

	bool IsInTriangle(CPoint pt1, CPoint pt2, CPoint pt3, CPoint point)//是否是三角形
	{
		if (Cross(pt1, pt2, point) >= 0
			&& Cross(pt2, pt3, point) >= 0
			&& Cross(pt3, pt1, point) >= 0)
			return true;
		return false;
	}

	eCursorState m_eState;
	int m_nSize;
	int m_nArrowSize;
	int m_nPrecision;
	CPoint m_ptPos;
	bool m_bDrag;
	CPoint m_ptDragStart;
	Stick *m_StickLock;
	int m_curStickList;
};

class CChildView : public CWnd//窗口
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void DrawLine(CDC *pDC, CPoint pt1, CPoint pt2);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildView();
	// Generated message map functions
protected:
	//{{AFX_MSG(CChildView)
	afx_msg void OnPaint();//绘制
	afx_msg void OnBeginFrame();//开始帧
	afx_msg void OnUpdateUI(CCmdUI *pCmdUI);//更新ui
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);//设置爱光标
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);//创建？？
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);//鼠标操作
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFrameEnd();//结束帧
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShowFrame();//显示帧
	afx_msg void OnTransTools(UINT nID);//转换工具
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool m_bDrawFrame;
	bool m_bStartFrame;
	bool m_bRotate;
	bool m_bTrans;
	HCURSOR m_hCursors[2];
	CPoint m_ptOld;
	CPoint m_ptCur;
	CMenu m_Popup;
	Stick *m_StickLists[50];
	int m_nNumStickLists;
	bool m_bShowFrame;
	CCursor m_cursor;
	matrix3x3 m_mRotate;
	CPoint m_ptTrans;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CHILDVIEW_H__119127E4_A8C5_4B0E_B9C2_D8CA2EC731A8__INCLUDED_)
