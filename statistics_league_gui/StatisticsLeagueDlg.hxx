
// StatisticsLeagueDlg.hxx : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

// CStatisticsLeagueDlg �Ի���
class CStatisticsLeagueDlg : public CDialogEx
{
// ����
public:
	CStatisticsLeagueDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_STATISTICS_LEAGUE_GUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConfirm();
private:
	CComboBox mCbSeason;
	CComboBox mCbType;
};