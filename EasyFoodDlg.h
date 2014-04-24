
// EasyFoodDlg.h : ͷ�ļ�
//

#pragma once

#include "sqlite3.h"


// CEasyFoodDlg �Ի���
class CEasyFoodDlg : public CDialogEx
{
// ����
public:
	CEasyFoodDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EASYFOOD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	sqlite3* conn;

	// ���ɵ���Ϣӳ�亯��
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
