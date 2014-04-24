// Member.cpp : 实现文件
//

#include "stdafx.h"
#include "EasyFood.h"
#include "Member.h"
#include "afxdialogex.h"

#include "sqlite3.h"

CString Member::QueryExePath()
{
	TCHAR path[MAX_PATH];
	::GetModuleFileName(NULL,path,MAX_PATH);
	CString p(path);
	CString subp;
	int nPos = p.ReverseFind('\\');
	//ASSERT(-1!=nPos);
	return p.Left(nPos+1);
}

// Member 对话框

IMPLEMENT_DYNAMIC(Member, CDialogEx)

Member::Member(CWnd* pParent /*=NULL*/)
	: CDialogEx(Member::IDD, pParent)
{
	conn = NULL;
}

BOOL Member::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strPath = QueryExePath();
	strPath += _T("Food.sqlite");
	char *path;
	path=strPath.GetBuffer(strPath.GetLength()); 
	strPath.ReleaseBuffer();
	// 打开数据库, 创建连接
	if (SQLITE_OK != sqlite3_open(path, &conn))
	{
		GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("DBerror"));
	}
	
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));

	m_Grid.SetFixedColumnSelection(TRUE);
	m_Grid.SetFixedRowSelection(TRUE);
	m_Grid.EnableColumnHide();

	//m_Grid.EnableScrollBars(SB_VERT,1);

	m_Grid.SetColumnCount(3); 
	m_Grid.SetRowCount(5);
	m_Grid.SetFixedRowCount(1);
	
	int a = m_Grid.IsVisibleVScroll();
	CString str;
	
	str = "ID";
	m_Grid.SetItemText(0,0,str);
	str = "Name";
	m_Grid.SetItemText(0,1,str);
	str = "Money";
	m_Grid.SetItemText(0,2,str);

	//m_Grid.ExpandColumnsToFit(TRUE);
	FillGrid();
	return TRUE;
}

Member::~Member()
{
	if(conn)
	{
		sqlite3_close(conn);
	}
}

void Member::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);             // associate the grid window with a C++ object
}


BEGIN_MESSAGE_MAP(Member, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Member::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &Member::OnBnClickedOk)

	ON_NOTIFY(NM_DBLCLK, IDC_GRID, OnGridDblClick)
	ON_BN_CLICKED(IDC_BUTTON4, &Member::OnBnClickedButton4)
END_MESSAGE_MAP()


// Member 消息处理程序


void Member::OnBnClickedButton1()
{
	CString strName,strMoney;
	GetDlgItem(IDC_EDIT1)->GetWindowText(strName);
	if (strName.IsEmpty())
	{
		CString ss = _T("Name is Empty");
		MessageBox(ss);

		return;
	}

	GetDlgItem(IDC_EDIT2)->GetWindowText(strMoney);
	if (strMoney.IsEmpty())
	{
		CString ss = _T("Money is Empty");
		MessageBox(ss);

		return;
	}

	float fMoney = atof(strMoney);
	//////////////////////////////////////////
	sqlite3_stmt* ppStmt = NULL;

	CString strSql;
	strSql.Format("select id, money from member where name = '%s'",strName);

	int ret = sqlite3_prepare(conn, strSql, -1, &ppStmt, /*&tail*/0);
	ret = sqlite3_step(ppStmt);

	int id = -1;

	if(ret!=SQLITE_DONE)
	{
		id = sqlite3_column_double(ppStmt,0);
		double dMoney = sqlite3_column_double(ppStmt,1);
		dMoney += fMoney;		
		sqlite3_finalize(ppStmt);

		strSql.Format("UPDATE member SET money = %f WHERE name='%s'",dMoney,strName);

		ret = sqlite3_prepare(conn, strSql, -1, &ppStmt, /*&tail*/0);
		ret = sqlite3_step(ppStmt);

		for(int i=1;i<m_Grid.GetRowCount();i++)
		{
			CString str = m_Grid.GetItemText(i,0);
			int n = atoi(str);
			if(n == id)
			{
				CString strNum;
				strNum.Format("%f",dMoney);
				m_Grid.SetItemText(i,2,strNum);
				m_Grid.SetItemBkColour(i,2,0X00F0F0);
				break;;
			}
		}
		m_Grid.UpdateData();

		m_Grid.Invalidate();

		if(ret != SQLITE_DONE)
			MessageBox("充值成功，请查看高亮处是否准确");
		sqlite3_finalize(ppStmt);
	}
	else
	{
		strSql.Format("INSERT INTO member ([name],[money]) VALUES('%s',%f)",strName,fMoney);

		char *pSql;
		pSql=strSql.GetBuffer();
		char *err_msg = NULL;

		sqlite3_stmt *stmt;

		// Create a prepared statement.

		if (SQLITE_OK != sqlite3_exec(conn, pSql, 0, 0, &err_msg))
		{
			CString strContent(err_msg);
			MessageBox(strContent);
		}
		else
		{
			GetDlgItem(IDC_EDIT1)->SetWindowText(_T(""));
			GetDlgItem(IDC_EDIT2)->SetWindowText(_T(""));
		}

		FillGrid();
	}

	////////////////////////////////////////////
	//delete pSql;

	CString strResult;
	CTime time =  CTime::GetCurrentTime();
	CString date = time.Format("%Y-%m-%d %H:%M:%S %W-%A");
	strResult.Format("%d:%s add money %s at %s",id,strName,strMoney,date);

	strSql.Format("INSERT INTO change ([des]) VALUES('%s')",strResult);

	char* pSql=strSql.GetBuffer();
	char* err_msg;
	// Create a prepared statement.

	sqlite3_exec(conn, pSql, 0, 0, &err_msg);
}


void Member::OnBnClickedOk()
{
	// TODO: ?????í???????¨?????í???ò?ú??
	CDialogEx::OnOK();
}

void Member::FillGrid()
{
	sqlite3_stmt* ppStmt = NULL;

	const char* getCount = "select count(*) from member";
	int ret = sqlite3_prepare(conn, getCount, -1, &ppStmt, /*&tail*/0);
	ret = sqlite3_step(ppStmt);
	int nCount = sqlite3_column_int(ppStmt, 0);
	m_Grid.SetRowCount(nCount+1);
	sqlite3_finalize(ppStmt);

	int i = 1;

	const char *ps ="select * from member";
	const char **pzTail = &ps;
	ret = sqlite3_prepare_v2(conn,ps,-1,&ppStmt,pzTail);
	
	ret = sqlite3_step(ppStmt);//sqlite3_step返回值
	CString strDes;

	//int nCount = sqlite3_;
	

	while(ret!=SQLITE_DONE)//当前表还没用读完
	{
		int id = sqlite3_column_int(ppStmt,0);
		strDes.Format("%d",id);
		m_Grid.SetItemText(i,0,strDes);
		const char* name = (const char*)sqlite3_column_text(ppStmt,1);
		strDes.Format("%s",name);
		m_Grid.SetItemText(i,1,strDes);
		float money = sqlite3_column_double(ppStmt,2);
		strDes.Format("%f",money);
		m_Grid.SetItemText(i,2,strDes);
		ret = sqlite3_step(ppStmt);
		i++;
	}
	
	for(i=1;i<nCount+1;i++)
	{
		m_Grid.SetItemState(i,0,GVIS_READONLY);
		m_Grid.SetItemState(i,1,GVIS_READONLY);
		m_Grid.SetItemState(i,2,GVIS_READONLY);
	}
	sqlite3_finalize(ppStmt);

	//m_Grid.EnableScrollBars(SB_VERT,1);
}

void Member::OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	 NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	 int i = pItem->iRow;

	 CString strName = m_Grid.GetItemText(i,1);

	 GetDlgItem(IDC_EDIT1)->SetWindowText(strName);
	 GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("充值"));

}

void Member::OnBnClickedButton4()
{
	CString strName,strMoney;
	GetDlgItem(IDC_EDIT1)->GetWindowText(strName);
	if (strName.IsEmpty())
	{
		CString ss = _T("Name is Empty");
		MessageBox(ss);

		return;
	}

	//////////////////////////////////////////
	sqlite3_stmt* ppStmt = NULL;

	CString strSql;
	strSql.Format("select id, money from member where name = '%s'",strName);

	int ret = sqlite3_prepare(conn, strSql, -1, &ppStmt, /*&tail*/0);
	ret = sqlite3_step(ppStmt);

	int id = -1;
	if(ret!=SQLITE_DONE)
	{
		id = sqlite3_column_double(ppStmt,0);
		double dMoney = sqlite3_column_double(ppStmt,1);		
		sqlite3_finalize(ppStmt);

		if (dMoney<0)
		{
			CString ss = _T("该用户目前欠费，不能消除！");
			MessageBox(ss);

			return;
		}

		strSql.Format("Delete from member WHERE name='%s'",strName);

		ret = sqlite3_prepare(conn, strSql, -1, &ppStmt, /*&tail*/0);
		ret = sqlite3_step(ppStmt);

		FillGrid();



		sqlite3_finalize(ppStmt);
	}

	////////////////////////////////////////////
	//delete pSql;

	CString strResult;
	CTime time =  CTime::GetCurrentTime();
	CString date = time.Format("%Y-%m-%d %H:%M:%S %W-%A");
	strResult.Format("delete the %d:%s at %s",id,strName,date);

	strSql.Format("INSERT INTO change ([des]) VALUES('%s')",strResult);

	char* pSql=strSql.GetBuffer();
	char* err_msg;
	// Create a prepared statement.

	sqlite3_exec(conn, pSql, 0, 0, &err_msg);
}
