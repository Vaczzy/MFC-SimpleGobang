
// MyGobang_DHKDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyGobang_DHK.h"
#include "MyGobang_DHKDlg.h"
#include "afxdialogex.h"
#include "MainGobang.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()

public:
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CMyGobang_DHKDlg 对话框




CMyGobang_DHKDlg::CMyGobang_DHKDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyGobang_DHKDlg::IDD, pParent)
{
	/*m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);*/
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CMyGobang_DHKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MainTable, picMain);
}

BEGIN_MESSAGE_MAP(CMyGobang_DHKDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Start, &CMyGobang_DHKDlg::OnBnClickedButtonStart)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_Machine, &CMyGobang_DHKDlg::OnBnClickedButtonMachine)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_Withdraw, &CMyGobang_DHKDlg::OnBnClickedButtonWithdraw)
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CMyGobang_DHKDlg 消息处理程序

BOOL CMyGobang_DHKDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	((CButton *)GetDlgItem(IDC_RADIO_Normol))->SetCheck(true);//默认选中标准棋盘
	bIsReady=false;
	bIsMachine=false;
	bWithdraw = true;
	//获取客户区大小
	CRect rect;
	GetClientRect(&rect);  
	oldSize.x = rect.right - rect.left;//客户区宽
	oldSize.y = rect.bottom - rect.top;//客户区高
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyGobang_DHKDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyGobang_DHKDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	if(bIsReady)
	{
		//绘制棋盘
		CDC *pDC;
		pDC = picMain.GetDC();//获取该控件的画布//2020.02.23
		CRect rect;
		picMain.GetClientRect(&rect);//获取控件大小
		if (((CButton *)GetDlgItem(IDC_RADIO_Normol))->GetCheck())
		{
			gobang.DrawMainTable(pDC,&rect,15);//标准棋盘
		}
		if (((CButton *)GetDlgItem(IDC_RADIO_Middle))->GetCheck())
		{
			gobang.DrawMainTable(pDC,&rect,17);//17*17棋盘
		}
		if (((CButton *)GetDlgItem(IDC_RADIO_Large))->GetCheck())
		{
			gobang.DrawMainTable(pDC,&rect,19);//19*19棋盘
		}
		ReleaseDC(pDC);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyGobang_DHKDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//开始按钮
void CMyGobang_DHKDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	bIsReady=true;
	OnPaint();//参数设置完成后 再绘制棋盘
	gobang.SetPieces();
	SetTimer(1, 40, NULL);
	MessageBox(_T("黑棋先手"));
}

//鼠标移动函数
void CMyGobang_DHKDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pt;
	CRect rect;
	GetDlgItem(IDC_MainTable)->GetWindowRect(&rect);//获取控件屏幕坐标
	GetCursorPos(&pt);//获取鼠标点击坐标(屏幕坐标坐标)
	//该位置判断前移至此函数中2020.03.05
	if (rect.PtInRect(pt)) //如果鼠标在这个范围之内
	{
		pt.x -= rect.left;//转化为控件中坐标
		pt.y -= rect.top;
		CDC *pDC = picMain.GetDC();//获取该控件的画布
		if (bIsReady)
		{
			gobang.DrawRectangle(pDC, pt);
		}
		ReleaseDC(pDC);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

//点击鼠标左键
void CMyGobang_DHKDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(!bIsReady)
	{
		MessageBox(_T("请先点击开始！"));
	}
	if(bIsReady)
	{
		CRect rect;
	    GetDlgItem(IDC_MainTable)->GetWindowRect(&rect);//获取控件屏幕坐标
		CPoint pt;
		GetCursorPos(&pt);//获取鼠标点击坐标(屏幕坐标坐标)
		//该位置判断前移至此函数中2020.03.05
		if (rect.PtInRect(pt)) //如果鼠标在这个范围之内
		{
			pt.x -= rect.left;//转化为控件中坐标
			pt.y -= rect.top;
			CDC *pDC = picMain.GetDC();//获取该控件的画布
			gobang.DrawPieces(pDC, rect, pt);//绘制棋子
			if (gobang.Success())//执行胜利判断函数
			{
				KillTimer(1);
				gobang.ReplayMark(pDC);
				bIsReady = false;
				bWithdraw = false;
			}
			ReleaseDC(pDC);
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

//人机对抗
void CMyGobang_DHKDlg::OnBnClickedButtonMachine()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("相关功能正在测试中"));
}

//刷新计时器
void CMyGobang_DHKDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC *pDC=picMain.GetDC();//获取该控件的画布
	CRect rect;
	GetDlgItem(IDC_MainTable)->GetWindowRect(&rect);//获取控件屏幕坐标
	switch(nIDEvent)
	{
	case 1:
		gobang.KeepPieces(pDC,rect);
		break;
	}
	ReleaseDC(pDC);
	CDialogEx::OnTimer(nIDEvent);
}



//悔棋按钮
void CMyGobang_DHKDlg::OnBnClickedButtonWithdraw()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bBlack;
	if(bWithdraw)
	{
		CDC *pDC=picMain.GetDC();//获取该控件的画布
	    if(gobang.ClearPieces(pDC,bBlack))//清除最近一步棋子
		{
			bWithdraw=false;//只允许悔一步棋
		}
		else
		{
			MessageBox(_T("您还没有下棋！"));
		}
		ReleaseDC(pDC);
	}
	else if(bIsReady)
	{
		MessageBox(_T("您只能悔一次棋！"));
	}
}

//改变鼠标形状
BOOL CMyGobang_DHKDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint pos;
	GetCursorPos(&pos);  //获取当前鼠标位置
	CRect rc;
	GetDlgItem(IDC_MainTable)->GetWindowRect(&rc); 
	if (rc.PtInRect(pos)) //如果鼠标在这个范围之内
	{
		SetCursor(LoadCursor(NULL,IDC_HAND)); //设置成手状
		return TRUE; 
	}
	GetDlgItem(IDC_BUTTON_Withdraw)->GetWindowRect(&rc); 
	if((rc.PtInRect(pos))&&((!bWithdraw)||(!bIsReady)))
	{
		SetCursor(LoadCursor(NULL,IDC_NO));
		return TRUE; 
	}
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

//改变窗口大小
void CMyGobang_DHKDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		ReSize();//  自己写的函数
	}

}

//  对话框中的所有控件都进行等比例缩放
void CMyGobang_DHKDlg::ReSize()
{
	float fsp[4];
	POINT newSize; //获取现在对话框的大小  
	CRect recta;
	GetClientRect(&recta);     //取客户区大小    
	newSize.x = recta.right - recta.left;
	newSize.y = recta.bottom - recta.top;
	fsp[0] = (float)newSize.x / oldSize.x;
	fsp[1] = (float)newSize.y / oldSize.y;
	fsp[2] = (float)newSize.y - oldSize.y;
	fsp[3] = (float)newSize.x - oldSize.x;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //左上角  
	CPoint OldBRPoint, BRPoint; //右下角  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  ////取得第一个控件的句柄,用于遍历所有控件   
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID 
				/*所有控件全都等比例扩大，除了上面的按钮*/
		GetDlgItem(woc)->GetWindowRect(Rect);//获得相对于屏幕左上角的坐标    
		ScreenToClient(Rect);//将屏幕坐标转换成相对客户窗口左上角的坐标

		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x * fsp[0]);//用比例得出左上角的x坐标
		TLPoint.y = long(OldTLPoint.y * fsp[1]); //用比例得出左上角的y坐标

		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x * fsp[0]); //用比例得出右下角的x坐标
		BRPoint.y = long(OldBRPoint.y * fsp[1]);//用比例得出右下角的y坐标

		Rect.SetRect(TLPoint, BRPoint);//设置最新的Rect
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);//获得下一个控件的句柄

	}
	oldSize = newSize;
}

void CMyGobang_DHKDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CPoint   pt(100, 100); //定义宽和高  
	lpMMI->ptMinTrackSize = pt; //限制最小宽和高  
	//CDialogEx::OnGetMinMaxInfo(lpMMI);

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}
