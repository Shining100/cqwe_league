
// StatisticsLeagueDlg.cxx : 实现文件
//

#include "stdafx.h"
#include "statistics_league_gui/StatisticsLeagueApp.hxx"
#include "statistics_league_gui/StatisticsLeagueDlg.hxx"
#include "statistics/Singleton.hxx"
#include "statistics_league_gui/StatisticsLeague.hxx"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStatisticsLeagueDlg 对话框



CStatisticsLeagueDlg::CStatisticsLeagueDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStatisticsLeagueDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStatisticsLeagueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEASON, mCbSeason);
	DDX_Control(pDX, IDC_TYPE, mCbType);
}

BEGIN_MESSAGE_MAP(CStatisticsLeagueDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONFIRM, &CStatisticsLeagueDlg::OnBnClickedConfirm)
END_MESSAGE_MAP()


// CStatisticsLeagueDlg 消息处理程序

BOOL CStatisticsLeagueDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	mCbSeason.AddString("2012b");
	mCbSeason.SetCurSel(0);
	mCbType.AddString("工资");
	mCbType.AddString("生年");
	mCbType.AddString("身高");
	mCbType.AddString("总评");
	mCbType.AddString("国籍");
	mCbType.AddString("俱乐部");
	mCbType.SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CStatisticsLeagueDlg::OnPaint()
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
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CStatisticsLeagueDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStatisticsLeagueDlg::OnBnClickedConfirm()
{
	CString selSeason;
	mCbSeason.GetLBText(mCbSeason.GetCurSel(), selSeason);
	CString selType;
	mCbType.GetLBText(mCbType.GetCurSel(), selType);
	Singleton<StatisticsLeague>::instance().statistics(selSeason.GetBuffer(0),
		selType.GetBuffer(0)) ? AfxMessageBox("统计成功") :
		AfxMessageBox("统计失败");
}
