
// EasyFoodDlg.h : 头文件
//

#pragma once

#include "sqlite3.h"


// CEasyFoodDlg 对话框
class CEasyFoodDlg : public CDialogEx
{
// 构造
public:
	CEasyFoodDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_EASYFOOD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	sqlite3* conn;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();

	CString QueryExePath();

	void FreshData();
	afx_msg void OnBnClickedOk();
};
