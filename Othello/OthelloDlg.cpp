
// OthelloDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Othello.h"
#include "OthelloDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COthelloDlg �Ի���



COthelloDlg::COthelloDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OTHELLO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COthelloDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COthelloDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BuClose, &COthelloDlg::OnBnClickedBuclose)
    ON_BN_CLICKED(IDC_BuStart, &COthelloDlg::OnBnClickedBustart)
END_MESSAGE_MAP()


// COthelloDlg ��Ϣ�������

BOOL COthelloDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//��ʼ��ͼƬ��
    



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COthelloDlg::OnPaint()
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
HCURSOR COthelloDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//�رճ���ť�¼���Ӧ
void COthelloDlg::OnBnClickedBuclose()
{
    exit(0);
}

//�¿��ְ�ť�¼���Ӧ
void COthelloDlg::OnBnClickedBustart()
{
    if (bIsFirstStart == FALSE)
        if (
            MessageBox(_T("���¿�ʼһ�ֱ���?"),
                _T("�ڰ���"),
                MB_YESNO | MB_ICONINFORMATION)
            == IDNO
            )
                return; //�����¿�ʼ
    bIsFirstStart = FALSE;  //���


}
