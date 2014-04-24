#pragma once

#include "sqlite3.h"
#include "./GridCtrl_src/GridCtrl.h"

// FEE �Ի���

class FEE : public CDialogEx
{
	DECLARE_DYNAMIC(FEE)

public:
	FEE(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~FEE();

	sqlite3* conn;

	CGridCtrl m_Grid;


// �Ի�������
	enum { IDD = IDD_FEE };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	CString QueryExePath();
	void FillGrid();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
