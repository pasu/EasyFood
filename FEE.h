#pragma once

#include "sqlite3.h"
#include "./GridCtrl_src/GridCtrl.h"

// FEE 对话框

class FEE : public CDialogEx
{
	DECLARE_DYNAMIC(FEE)

public:
	FEE(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~FEE();

	sqlite3* conn;

	CGridCtrl m_Grid;


// 对话框数据
	enum { IDD = IDD_FEE };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CString QueryExePath();
	void FillGrid();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
