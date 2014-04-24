#pragma once

#include "sqlite3.h"
#include "./GridCtrl_src/GridCtrl.h"


// Member �Ի���

class Member : public CDialogEx
{
	DECLARE_DYNAMIC(Member)

public:
	Member(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Member();

	sqlite3* conn;

	CGridCtrl m_Grid;

// �Ի�������
	enum { IDD = IDD_MEMBER };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* pResult);

	void FillGrid();
	CString QueryExePath();
	afx_msg void OnBnClickedButton4();
};
