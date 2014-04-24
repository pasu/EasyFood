#pragma once

#include "sqlite3.h"
#include "./GridCtrl_src/GridCtrl.h"


// Member 对话框

class Member : public CDialogEx
{
	DECLARE_DYNAMIC(Member)

public:
	Member(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Member();

	sqlite3* conn;

	CGridCtrl m_Grid;

// 对话框数据
	enum { IDD = IDD_MEMBER };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* pResult);

	void FillGrid();
	CString QueryExePath();
	afx_msg void OnBnClickedButton4();
};
