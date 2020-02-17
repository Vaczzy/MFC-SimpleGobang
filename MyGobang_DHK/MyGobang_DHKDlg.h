
// MyGobang_DHKDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "MainGobang.h"

// CMyGobang_DHKDlg 对话框
class CMyGobang_DHKDlg : public CDialogEx
{
// 构造
public:
	CMyGobang_DHKDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MYGOBANG_DHK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMainGobang gobang;
	CStatic picMain;//画图控件
	CPoint pt;//鼠标点击点
	bool bIsReady;//可以开始human vs human
	bool bDrawPieces;//可以开始画棋子
	bool bIsMachine;//human vs machine
	bool bWithdraw;//悔棋次数
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonMachine();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonWithdraw();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
