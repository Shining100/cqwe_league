
// StatisticsLeagueDlg.cxx : ʵ���ļ�
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


// CStatisticsLeagueDlg �Ի���



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


// CStatisticsLeagueDlg ��Ϣ�������

BOOL CStatisticsLeagueDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	mCbSeason.AddString("2012b");
	mCbSeason.SetCurSel(0);
	mCbType.AddString("����");
	mCbType.AddString("����");
	mCbType.AddString("���");
	mCbType.AddString("����");
	mCbType.AddString("����");
	mCbType.AddString("���ֲ�");
	mCbType.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CStatisticsLeagueDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		selType.GetBuffer(0)) ? AfxMessageBox("ͳ�Ƴɹ�") :
		AfxMessageBox("ͳ��ʧ��");
}
