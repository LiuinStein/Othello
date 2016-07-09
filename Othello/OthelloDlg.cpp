
// OthelloDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Othello.h"
#include "OthelloDlg.h"
#include "afxdialogex.h"
#include "Data.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COthelloDlg �Ի���


//��Ϸ(����)��ʼ,��ʼ������
void COthelloDlg::init_game_start()
{
    m_bWriteChessNum = m_bBlackChessNum = 2;
    m_tWriteUsingTime = m_tBlackUsingTime = 0UL;
    m_bIsWriteDownChess = TRUE;
    m_psNow.init();
    while (m_stpsStep.size() != 0)
        m_stpsStep.pop();   //��ջ
    if (m_bIsFirstStart == TRUE)
    {
        //��һ�ν�����Ϸʱ����BMPλͼ��Դ
        m_cbmpBlackChess.LoadFromResource(
            ::AfxGetInstanceHandle(), 
            IDB_ChessBlack);
        m_cbmpBlank.LoadFromResource(
            ::AfxGetInstanceHandle(),
            IDB_Blank);
        m_cbmpChessBoard.LoadFromResource(
            ::AfxGetInstanceHandle(), 
            IDB_ChessBoard);
        m_cbmpWriteChess.LoadFromResource(
            ::AfxGetInstanceHandle(), 
            IDB_ChessWrite);
        m_cbmpDlgBack.LoadFromResource(
            ::AfxGetInstanceHandle(),
            IDB_Back);
    }
}

//������ָ�������������
void COthelloDlg::draw_chess_by_coord(BOOL __ISW,
    LineNumber __l, ColNumber __c)
{
    if (__l < 1 || __l > 8 || __c < 1 || __c > 8)
    {
        AfxMessageBox(_T("draw_Chess_by_coord����Խ���쳣"));
        exit(1);
    }
    //��ȡ��ǰ��������λͼ
    CImage * drawChess{ nullptr };
    if (__ISW != -1)
        drawChess = &(__ISW == WriteChess ?
            m_cbmpWriteChess : m_cbmpBlackChess);
    else
        drawChess = &m_cbmpBlank;
    //������������
    Coord drawX{//X
        ChessBoardLeftUppX + ChessBoard_1A_LeftUppX
        + (__c - 1) * ChessSize + 1
    };
    Coord drawY{//Y
        ChessBoardLeftUppY + ChessBoard_1A_LeftUppY
        + (__l - 1) * ChessSize + 1
    };
    //��ͼ
    if (drawChess->Draw(*(this->GetDC()), drawX,
        drawY) == FALSE)
    {
        AfxMessageBox(_T("draw_Chess_by_coord�����쳣,��ͼʧ��"));
        exit(1);
    }

}

//��������
void COthelloDlg::chess_player_down(
    LineNumber __l, ColNumber __c)
{
    //�����Ҫ��ת����������,�������������ż  
    vecPairLiCol vecReverseChess;
    //�Ϸ����ж�
    if (down_check(__l, __c, &vecReverseChess) 
        == FALSE)
        return;     //�˲�������
    //��¼������־
    m_psNow.set_chess_number(m_bWriteChessNum, m_bBlackChessNum);
    m_stpsStep.push(m_psNow);
    //��������
    size_t deltaNum = vecReverseChess.size();
    CString cstrStep;
    if (m_bIsWriteDownChess == WriteChess)
    {
        m_bWriteChessNum += 1 + deltaNum;
        m_bBlackChessNum -= deltaNum;
    }     
    else
    {
        m_bBlackChessNum += 1 + deltaNum;
        m_bWriteChessNum -= deltaNum;
    }
    refresh_chess_num();
    //����
    for (vecPairLiCol::iterator iter = vecReverseChess.begin();
        iter != vecReverseChess.end(); ++iter)
    {
        draw_chess_by_coord(m_bIsWriteDownChess,
            iter->first, iter->second);
        // �ı䵱ǰ״̬
        m_psNow.set_chess_state(iter->first, iter->second,
            m_bIsWriteDownChess == WriteChess ? e_psWriteDown :
            e_psBlackDown);
    }      
    draw_chess_by_coord(m_bIsWriteDownChess, __l, __c);  
    //�޸����ӵ�ǰ״̬
    m_psNow.set_chess_state(__l, __c,
        m_bIsWriteDownChess == WriteChess ? e_psWriteDown :
        e_psBlackDown); 

    //����
    //���־��ǶԶԷ��Ƿ���������ж�,Ҫ�������̿�λ
    if(can_do_next(__l,__c,m_bIsWriteDownChess)==TRUE)
    {
        player_logo_change();   //logo����
        m_bIsWriteDownChess = ~m_bIsWriteDownChess; //����
    }
    else  //�����Ի���,��ǰ���ּ�����һ��
        if(m_bWriteChessNum + m_bBlackChessNum != 64)
        {
            //����û��
            CString cstrMsg;
            CString cstrNext = m_bIsWriteDownChess == WriteChess ?
                _T("�׷�") : _T("�ڷ�");
            CString cstrNotNext = m_bIsWriteDownChess == WriteChess ?
                _T("�ڷ�") : _T("�׷�");

            cstrMsg.Format(_T("%s�޷�������һ��,��%s������һ��"),
                cstrNotNext, cstrNext);
        }
        else
        {
            //�������Ϊ�������˵���can_do_next����FALSE
            //����Ǹ÷�ʤ����ʱ����
            CString cstrWinMsg;
            if (m_bBlackChessNum != m_bWriteChessNum)
                cstrWinMsg = m_bBlackChessNum > m_bWriteChessNum ?
                _T("�ڷ�ʤ") : _T("�׷�ʤ");
            else if (m_tWriteUsingTime != m_tBlackUsingTime)
                cstrWinMsg = m_tWriteUsingTime > m_tBlackUsingTime ?
                _T("�ڷ�ʤ") : _T("�׷�ʤ");
            else
                cstrWinMsg = _T("ƽ��");
            m_bIsGameStart = FALSE; //�����Ϸ����
            MessageBox(cstrWinMsg, _T("�ڰ���"),
                MB_ICONINFORMATION | MB_OK);   
            KillTimer(1);
        }   
}

//logo����
void COthelloDlg::player_logo_change()
{
    if (m_bIsWriteDownChess == WriteChess)
    {
        m_cbmpDlgBack.Draw(*this->GetDC(),
            WritePlayerLogoX, WritePlayerLogoY);
        m_cbmpBlackChess.Draw(*this->GetDC(),
            BlackPlayerLogoX, BlackPlayerLogoY);
    }
    else
    {
        m_cbmpDlgBack.Draw(*this->GetDC(),
            BlackPlayerLogoX, BlackPlayerLogoY);
        m_cbmpWriteChess.Draw(*this->GetDC(),
            WritePlayerLogoX, WritePlayerLogoY);
    }  
}

//���ӺϷ��Լ��
BOOL COthelloDlg::down_check(LineNumber __l,
    ColNumber __c, vecPairLiCol * __v) const
{
    if (m_psNow.get_state_by_coord(__l, __c) == e_psBlank &&
        reverse_chess(__l, __c, m_bIsWriteDownChess, __v) 
        == TRUE  //����б4����,�����Ƿ���Է�ת�Է�
        )
        return TRUE;
    return FALSE;
}

//����б4����,���߷���TRUE
BOOL COthelloDlg::reverse_chess(
    LineNumber __l, ColNumber __c,  //������ĵ�����
    BOOL __ISW,     //������ĵ�������ɫ
    vecPairLiCol* __v) const
{
    pairLiCol tmp;
    //����ƥ�����
    PointState psMatch = __ISW == WriteChess ?
        e_psWriteDown : e_psBlackDown;
    //�Ӽ���������,�ҵ�һ��
    for (LineNumber i = __l - 1; i >= 1; i--)
    {
        PointState psNow = m_psNow.get_state_by_coord(i, __c);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr)
                return TRUE;
            //�ҵ�ƥ��,�Ӵ�����ȫ��ѹ��vector
            for (LineNumber j = i + 1; j < __l; j++)
            {
                tmp.first = j;
                tmp.second = __c;
                __v->push_back(tmp);
            }
            break;
        }
    }
        
    //�Ӽ���������,�ҵ�һ��
    for (LineNumber i = __l + 1; i <= 8; i++)
    {
        PointState psNow = m_psNow.get_state_by_coord(i, __c);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr)
                return TRUE;
            //�ҵ�ƥ��,�Ӵ�����ѹ��vector
            for (LineNumber j = i - 1; j > __l; j--)
            {
                tmp.first = j;
                tmp.second = __c;
                __v->push_back(tmp);
            }
            break;
        }
    }
        
    //�Ӽ���������,�ҵ�һ��
    for (ColNumber i = __c - 1; i >= 1; i--)
    {
        PointState psNow = m_psNow.get_state_by_coord(__l, i);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr)
                return TRUE;
            //�ҵ�ƥ��,�Ӵ�����ѹ��vector
            for (ColNumber j = i + 1; j < __c; j++)
            {
                tmp.first = __l;
                tmp.second = j;
                __v->push_back(tmp);
            }
            break;
        }
    }
        
    //�Ӽ���������,�ҵ�һ��
    for (ColNumber i = __c + 1; i <= 8; i++)
    {
        PointState psNow = m_psNow.get_state_by_coord(__l, i);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr)
                return TRUE;
            //�ҵ�ƥ��,�Ӵ�����ѹ��vector
            for (ColNumber j = i - 1; j > __c; j--)
            {
                tmp.first = __l;
                tmp.second = j;
                __v->push_back(tmp);
            }
            break;
        }
    }
        
    //�Ӽ�����������,�ҵ�һ��
    for (Number i = __c - 1, j = __l - 1;
        i >= 1 && j >= 1; i--, j--)
    {
        PointState psNow = m_psNow.get_state_by_coord(j, i);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr)
                return TRUE;
            //�ҵ�ƥ��,�Ӵ�������ѹ��vector
            for (Number p = i + 1, r = j + 1;
                p < __c && r < __l; p++, r++)
            {
                tmp.first = r;
                tmp.second = p;
                __v->push_back(tmp);
            }
            break;
        }
    }
        
    //�Ӽ�����������,�ҵ�һ��
    for (Number i = __l + 1, j = __c - 1;
        i <= 8 && j <= 8; i++, j--)
    {
        PointState psNow = m_psNow.get_state_by_coord(i, j);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr)
                return TRUE;
            //�ҵ�ƥ��,�Ӵ�������ѹ��vector
            for (Number p = i - 1, r = j + 1;
                p >= __l + 1 && r <= __c - 1; p--, j++)
            {
                tmp.first = p;
                tmp.second = r;
                __v->push_back(tmp);
            }
            break;
        }
    }  

    //�Ӽ�����������,�ҵ�һ��
    for (Number i = __l - 1, j = __c + 1;
        i >= 1 && j <= 8; i--, j++)
    {
        PointState psNow = m_psNow.get_state_by_coord(i, j);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr)
                return TRUE;
            //�ҵ�ƥ��,�Ӵ�������ѹ��vector
            for (Number p = i + 1, r = j - 1;
                p < __l && r > __c; p++, r--)
            {
                tmp.first = p;
                tmp.second = r;
                __v->push_back(tmp);
            }
            break;
        }
    }

    //�Ӽ�����������,�ҵ�һ��
    for (Number i = __l + 1, j = __c + 1;
        i <= 8 && j <= 8; i++, j++)
    {
        PointState psNow = m_psNow.get_state_by_coord(i, j);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr)
                return TRUE;
            //�ҵ�ƥ��,�Ӵ�������ѹ��vector
            for (Number r = i - 1, p = j - 1;
                r > __l&&p > __c; r--, p--)
            {
                tmp.first = r;
                tmp.second = p;
                __v->push_back(tmp);
            }
            break;
        }
    }
    return __v->size() != 0 ? TRUE : FALSE;
}

//ˢ�½����ϵ����Ӹ���
void COthelloDlg::refresh_chess_num() const
{
    CString cstrStep;
    cstrStep.Format(_T("����Ŀǰ���%d��"), m_bWriteChessNum);
    GetDlgItem(IDC_StaWrNum)->SetWindowTextW(cstrStep);
    cstrStep.Format(_T("����Ŀǰ���%d��"), m_bBlackChessNum);
    GetDlgItem(IDC_StBlNum)->SetWindowTextW(cstrStep);
}

//��һ����һ���Ŀ����������ж�
//(__l,__c)Ϊ��ɢ��,�Դ˵�Ϊ����(���ʱ�������˵�)
//�ȶ�ס�в���,����Ϊ������λ,��������ɢ,
//��Ҫ����һ�����е㼴return
//__ISW��Ϊ���ɫ,����ɫ���ӵĿ�����
//���˴����е�can_do_nexe_col����,������ֻ�����ﴦ����
BOOL COthelloDlg::can_do_next(LineNumber __l, ColNumber __c,
    BOOL __ISW)const
{
    //�˻����:��������,ֱ�ӷ���FALSE
    if (m_bWriteChessNum + m_bBlackChessNum == 64)
        return FALSE;
    //����
    for (LineNumber i = __l; i >= 1; i--)
        if (can_do_nexe_col(i, __c, __ISW) == TRUE)
            return TRUE;
    //����
    for (LineNumber i = __l + 1; i <= 8; i++)
        if (can_do_nexe_col(i, __c, __ISW) == TRUE)
            return TRUE;
    return FALSE;
}

//���м���,��Ϊ��ס����һ��,��Ϊ���ĵ���к�
//__ISW�����ɫ
BOOL COthelloDlg::can_do_nexe_col(LineNumber __l, ColNumber __c,
    BOOL __ISW) const
{
    //�������,��-
    for (ColNumber i = __c - 1; i >= 1; i--)
        if (m_psNow.get_state_by_coord(__l, i) == e_psBlank &&
            reverse_chess(__l, i, __ISW) == TRUE)
            return TRUE;
    //���Ҽ��,��+
    for (ColNumber i = __c + 1; i <= 8; i++)
        if (m_psNow.get_state_by_coord(__l, i) == e_psBlank &&
            reverse_chess(__l, i, __ISW) == TRUE)
            return TRUE;
    return FALSE;
}

//���캯��
COthelloDlg::COthelloDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OTHELLO_DIALOG, pParent),
    m_bIsFirstStart(TRUE), m_bIsGameStart(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    init_game_start();  //��ʼ����Ϸ����
}

void COthelloDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


//��Ϣӳ��
BEGIN_MESSAGE_MAP(COthelloDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BuClose, &COthelloDlg::OnBnClickedBuclose)
    ON_BN_CLICKED(IDC_BuStart, &COthelloDlg::OnBnClickedBustart)
    ON_BN_CLICKED(IDC_BuUndo, &COthelloDlg::OnBnClickedBuundo)
    ON_WM_LBUTTONDOWN()     //��Ϣӳ���н��������Ϣ
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BuLose, &COthelloDlg::OnBnClickedBulose)
END_MESSAGE_MAP()


// COthelloDlg ��Ϣ�������

BOOL COthelloDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
    if (m_bIsFirstStart == FALSE)
        if (
            MessageBox(_T("���¿�ʼһ�ֱ���?"),
                _T("�ڰ���"),
                MB_YESNO | MB_ICONINFORMATION)
            == IDNO
            )
                return; //�����¿�ʼ
    m_bIsFirstStart = FALSE;  //����Ѿ����ǵ�һ����
    m_bIsGameStart = TRUE;  //�����Ϸ��ʼ,�������Ϣ������Ӧ
    init_game_start();      //��ʼ����Ϸ����

    //��ʼ��������ʾ����
    GetDlgItem(IDC_StaWrTime)->SetWindowTextW(_T("������ʱ:       0s"));
    GetDlgItem(IDC_StaBlTime)->SetWindowTextW(_T("������ʱ:       0s"));
    GetDlgItem(IDC_StaWrNum)->SetWindowTextW(_T("����Ŀǰ���2��"));
    GetDlgItem(IDC_StBlNum)->SetWindowTextW(_T("����Ŀǰ���2��"));
    //���ư׷�����logo
    m_cbmpWriteChess.Draw(*this->GetDC(),
        WritePlayerLogoX, WritePlayerLogoY);
    m_cbmpDlgBack.Draw(*this->GetDC(),
        BlackPlayerLogoX, BlackPlayerLogoY);
    //��������
    m_cbmpChessBoard.Draw(*this->GetDC(),
        ChessBoardLeftUppX, ChessBoardLeftUppY);
    //���Ƴ�ʼ���ĸ�����
    draw_chess_by_coord(WriteChess, 4, 4);
    draw_chess_by_coord(WriteChess, 5, 5);
    draw_chess_by_coord(BlackChess, 4, 5);
    draw_chess_by_coord(BlackChess, 5, 4);
    //��ʱ����ʱ��ʼ
    SetTimer(1, 1000, nullptr);
}

//����������¼�
void COthelloDlg::OnLButtonDown(UINT nFlags, 
    CPoint point)
{
    if (m_bIsGameStart == FALSE)
        return; //��Ϸδ��ʼ���������Ϣ������Ӧ
    if (point.x<MouseAnswerFarLeftX ||
        point.x>MouseAnswerFarRightX ||
        point.y<MouseAnswerFarUpY ||
        point.y>MouseAnswerFarDownY)
        return;     //�������̷�Χ������Ӧ
    //������������������к�
    LineNumber ClickLineNumber{ //�к�
        static_cast<LineNumber>(
            (point.y - MouseAnswerFarUpY) / ChessSize + 1
        )
    };
    ColNumber ClickColumnNumber{ //�к�
        static_cast<ColNumber>(
            (point.x - MouseAnswerFarLeftX) / ChessSize + 1
        )
    };
    //���Ӵ���
    chess_player_down(ClickLineNumber, ClickColumnNumber);
}

//����
void COthelloDlg::OnBnClickedBuundo()
{   
    if (m_bIsGameStart == FALSE)
    {
        MessageBox(_T("��Ϸδ��ʼ,���ܻ���"), _T("�ڰ���"),
            MB_ICONERROR | MB_OK);
        return; //�˻����
    }      
    if (m_stpsStep.size() == 0)
    {
        MessageBox(_T("����ɻ�"),_T("�ڰ���"),
            MB_ICONWARNING);
        return;
    }
    CString cstrMsg;
    cstrMsg.Format(_T("%s����,�����öԷ�������?�Է�����ʱ60s!"),
        m_bIsWriteDownChess == WriteChess ?
        _T("�׷�") : _T("�ڷ�"));
    if (MessageBox(cstrMsg, _T("�ڰ���"),
        MB_YESNO | MB_ICONINFORMATION) == IDNO)
        return;
    //��ʱ
    m_bIsWriteDownChess == WriteChess ?
        m_tBlackUsingTime += 60 :
        m_tWriteUsingTime += 60;
    //��ԭ����
    m_psNow = m_stpsStep.top(); //ȡ��
    m_stpsStep.pop();           //����  
    for (LineNumber i = 1; i <= 8; i++)
        for (ColNumber j = 1; j <= 8; j++)
        {
            PointState psNPS = m_psNow.get_state_by_coord(i, j);
            BOOL bPs{ psNPS == e_psBlank ? TRUE : FALSE };
            bPs = bPs == TRUE ? -1 :
                (psNPS == e_psWriteDown ? TRUE : FALSE);
            draw_chess_by_coord(bPs, i, j);
        }    
    m_psNow.get_chess_number(&m_bWriteChessNum,
        &m_bBlackChessNum);
    refresh_chess_num();
    player_logo_change();   //logo����
    m_bIsWriteDownChess = ~m_bIsWriteDownChess; //����
}

//��ʱ���ص�����
void COthelloDlg::OnTimer(UINT_PTR nIDEvent)
{
    CString cstrTime;
    if (m_bIsWriteDownChess == WriteChess)
    {
        m_tWriteUsingTime++;
        cstrTime.Format(_T("������ʱ:%7lds"), m_tWriteUsingTime);
        GetDlgItem(IDC_StaWrTime)->SetWindowTextW(cstrTime);
    }
    else
    {
        m_tBlackUsingTime++;
        cstrTime.Format(_T("������ʱ:%7lds"), m_tBlackUsingTime);
        GetDlgItem(IDC_StaBlTime)->SetWindowTextW(cstrTime);
    }
    CDialogEx::OnTimer(nIDEvent);
}

//�������䰴ť
void COthelloDlg::OnBnClickedBulose()
{
    if (m_bIsGameStart == FALSE)
    {
        MessageBox(_T("��Ϸδ��ʼ,��������"), _T("�ڰ���"),
            MB_ICONERROR | MB_OK);
        return; //�˻����
    }
    CString cstrLoser;
    cstrLoser = m_bIsWriteDownChess == WriteChess ?
        _T("�׷�") : _T("�ڷ�");
    CString cstrMsg;
    cstrMsg.Format(_T("%s,ȷ��������?"), cstrLoser);
    if (MessageBox(cstrMsg, _T("�ڰ���"), MB_YESNO | MB_ICONWARNING) ==
        IDNO)
        return;
    m_bIsGameStart = FALSE;
    cstrMsg.Format(_T("%s����"), cstrLoser);
    MessageBox(cstrMsg, _T("�ڰ���"), MB_OK | MB_ICONINFORMATION);
    KillTimer(1);
}
