// FEE.cpp : 实现文件
//

#include "stdafx.h"
#include "EasyFood.h"
#include "FEE.h"
#include "afxdialogex.h"
#include "Member.h"


CString FEE::QueryExePath()
{
	TCHAR path[MAX_PATH];
	::GetModuleFileName(NULL,path,MAX_PATH);
	CString p(path);
	CString subp;
	int nPos = p.ReverseFind('\\');
	//ASSERT(-1!=nPos);
	return p.Left(nPos+1);
}

// FEE 对话框

IMPLEMENT_DYNAMIC(FEE, CDialogEx)

FEE::FEE(CWnd* pParent /*=NULL*/)
	: CDialogEx(FEE::IDD, pParent)
{
	conn = NULL;
}

FEE::~FEE()
{
	if(conn)
	{
		sqlite3_close(conn);
	}
}

BOOL FEE::OnInitDialog()
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

	str = "Name";
	m_Grid.SetItemText(0,0,str);
	str = "Money";
	m_Grid.SetItemText(0,1,str);
	str = "人数";
	m_Grid.SetItemText(0,2,str);

	FillGrid();
	return TRUE;
}

void FEE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID, m_Grid);             // associate the grid window with a C++ object
}


BEGIN_MESSAGE_MAP(FEE, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &FEE::OnBnClickedButton1)
END_MESSAGE_MAP()


// FEE 消息处理程序
void FEE::FillGrid()
{
	sqlite3_stmt* ppStmt = NULL;

	const char* getCount = "select count(*) from member";
	int ret = sqlite3_prepare(conn, getCount, -1, &ppStmt, /*&tail*/0);
	ret = sqlite3_step(ppStmt);
	int nCount = sqlite3_column_int(ppStmt, 0);
	m_Grid.SetRowCount(nCount+1);
	sqlite3_finalize(ppStmt);

	int i = 1;

	const char *ps ="select name,money from member";
	const char **pzTail = &ps;
	ret = sqlite3_prepare_v2(conn,ps,-1,&ppStmt,pzTail);

	ret = sqlite3_step(ppStmt);//sqlite3_step返回值
	CString strDes;

	//int nCount = sqlite3_;


	while(ret!=SQLITE_DONE)//当前表还没用读完
	{
		const char* name = (const char*)sqlite3_column_text(ppStmt,0);
		strDes.Format("%s",name);
		m_Grid.SetItemText(i,0,strDes);
		float money = sqlite3_column_double(ppStmt,1);
		strDes.Format("%f",money);
		m_Grid.SetItemText(i,1,strDes);
		ret = sqlite3_step(ppStmt);
		m_Grid.SetItemText(i,2,"1");
		i++;
	}

	for(i=1;i<nCount+1;i++)
	{
		m_Grid.SetItemState(i,0,GVIS_READONLY);
		m_Grid.SetItemState(i,1,GVIS_READONLY);
	}
	sqlite3_finalize(ppStmt);
}

void FEE::OnBnClickedButton1()
{
	CString strMoney;
	GetDlgItem(IDC_EDIT1)->GetWindowText(strMoney);
	if (strMoney.IsEmpty())
	{
		CString ss = _T("Money is Empty");
		MessageBox(ss);
		return;
	}

	double dCost = atof(strMoney);

	CArray <CString,CString&> m_NameArray;
	CArray <int,int&> m_NumArray;
	CArray <double,double&> m_MoneyArray;
	CArray <int,int&> m_ItemArray;

	int nCount = m_Grid.GetRowCount();
	int eatCount = 0;
	for(int i=1;i<nCount;i++)
	{
		BOOL bSelect = m_Grid.IsCellSelected(i,0);
		if (bSelect)
		{
			CString strName = m_Grid.GetItemText(i,0);
			m_NameArray.Add(strName);

			CString strNum = m_Grid.GetItemText(i,2);
			int nNum = atoi(strNum);
			m_NumArray.Add(nNum);

			CString strM = m_Grid.GetItemText(i,1);
			double dM = atof(strM);
			m_MoneyArray.Add(dM);
			m_ItemArray.Add(i);

			eatCount += nNum;
		}
	}

	double perCost = (dCost)/eatCount;//保证不赔，无损失

	sqlite3_stmt* ppStmt = NULL;
	CString strSql;

	for(int i=0;i<m_NameArray.GetCount();i++)
	{
		int nCount = m_NumArray[i];
		double dPerCost = perCost * nCount;

		double dMoney = m_MoneyArray[i] - dPerCost;

		CString strName = m_NameArray[i];

		strSql.Format("UPDATE member SET money = %f WHERE name='%s'",dMoney,strName);

		int ret = sqlite3_prepare(conn, strSql, -1, &ppStmt, /*&tail*/0);
		ret = sqlite3_step(ppStmt);
		sqlite3_finalize(ppStmt);

		CString strNum;
		strNum.Format("%f",dMoney);
		m_Grid.SetItemText(m_ItemArray[i],1,strNum);
		m_Grid.SetItemBkColour(m_ItemArray[i],1,0X0000FF);

		CString strResult;
		CTime time =  CTime::GetCurrentTime();
		CString date = time.Format("%Y-%m-%d %H:%M:%S %W-%A");
		strResult.Format("%s:%d person cost money %f at %s",strName,nCount,dPerCost,date);

		strSql.Format("INSERT INTO change ([des]) VALUES('%s')",strResult);

		char* pSql=strSql.GetBuffer();
		char* err_msg;
		// Create a prepared statement.

		sqlite3_exec(conn, pSql, 0, 0, &err_msg);
	}

	m_Grid.UpdateData();

	m_Grid.Invalidate();
}
