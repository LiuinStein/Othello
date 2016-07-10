
// OthelloDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Othello.h"
#include "OthelloDlg.h"
#include "afxdialogex.h"
#include "Data.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COthelloDlg 对话框


//游戏(重新)开始,初始化数据
void COthelloDlg::init_game_start()
{
    m_bWriteChessNum = m_bBlackChessNum = 2;
    m_tWriteUsingTime = m_tBlackUsingTime = 0UL;
    m_bIsWriteDownChess = TRUE;
    m_psNow.init();
    while (m_stpsStep.size() != 0)
        m_stpsStep.pop();   //清栈
    if (m_bIsFirstStart == TRUE)
    {
        //第一次进入游戏时载入BMP位图资源
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

//在棋盘指定坐标绘制棋子
void COthelloDlg::draw_chess_by_coord(BOOL __ISW,
    LineNumber __l, ColNumber __c)
{
    if (__l < 1 || __l > 8 || __c < 1 || __c > 8)
    {
        AfxMessageBox(_T("draw_Chess_by_coord出现越界异常"));
        exit(1);
    }
    //获取当前绘制棋子位图
    CImage * drawChess{ nullptr };
    if (__ISW != -1)
        drawChess = &(__ISW == WriteChess ?
            m_cbmpWriteChess : m_cbmpBlackChess);
    else
        drawChess = &m_cbmpBlank;
    //计算棋子坐标
    Coord drawX{//X
        ChessBoardLeftUppX + ChessBoard_1A_LeftUppX
        + (__c - 1) * ChessSize + 1
    };
    Coord drawY{//Y
        ChessBoardLeftUppY + ChessBoard_1A_LeftUppY
        + (__l - 1) * ChessSize + 1
    };
    //绘图
    if (drawChess->Draw(*(this->GetDC()), drawX,
        drawY) == FALSE)
    {
        AfxMessageBox(_T("draw_Chess_by_coord出现异常,绘图失败"));
        exit(1);
    }

}

//棋手落子
void COthelloDlg::chess_player_down(
    LineNumber __l, ColNumber __c)
{
    //存放需要翻转的棋子坐标,行列坐标放入序偶  
    vecPairLiCol vecReverseChess;
    //合法性判断
    if (down_check(__l, __c, &vecReverseChess) 
        == FALSE)
        return;     //此步不可行
    //记录悔棋日志
    m_psNow.set_chess_number(m_bWriteChessNum, m_bBlackChessNum);
    CChessBoardState m_psNowTmp = m_psNow;
    //步数计算
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
    //画子
    for (vecPairLiCol::iterator iter = vecReverseChess.begin();
        iter != vecReverseChess.end(); ++iter)
    {
        draw_chess_by_coord(m_bIsWriteDownChess,
            iter->first, iter->second);
        // 改变当前状态
        m_psNow.set_chess_state(iter->first, iter->second,
            m_bIsWriteDownChess == WriteChess ? e_psWriteDown :
            e_psBlackDown);
    }      
    draw_chess_by_coord(m_bIsWriteDownChess, __l, __c);  
    //修改棋子当前状态
    m_psNow.set_chess_state(__l, __c,
        m_bIsWriteDownChess == WriteChess ? e_psWriteDown :
        e_psBlackDown); 

    //换手
    //换手就是对对方是否可走作出判断,要遍历棋盘空位
    m_bPlayerNotChange = FALSE;
    if (can_do_next(__l, __c,
        m_bIsWriteDownChess == WriteChess ? BlackChess : WriteChess
    ) == TRUE)
    {
        player_logo_change();   //logo换手
        m_bIsWriteDownChess = !m_bIsWriteDownChess; //换手
    }
    else  //不可以换手,当前棋手继续下一步
        if(m_bWriteChessNum + m_bBlackChessNum != 64 &&
            can_do_next(__l, __c,
                m_bIsWriteDownChess == WriteChess ? WriteChess : BlackChess
            ) == TRUE)
        {
            //棋盘没满
            CString cstrMsg;
            CString cstrNext = m_bIsWriteDownChess == WriteChess ?
                _T("白方") : _T("黑方");
            CString cstrNotNext = m_bIsWriteDownChess == WriteChess ?
                _T("黑方") : _T("白方");

            cstrMsg.Format(_T("%s无法继续下一步,由%s继续下一步"),
                cstrNotNext, cstrNext);
            MessageBox(cstrMsg, _T("黑白棋"), MB_OK | MB_ICONINFORMATION);
            m_bPlayerNotChange = TRUE;
        }
        else
        {
            //如果是因为棋盘满了导致can_do_next返回FALSE
            //这就是该分胜负的时候了
            KillTimer(1);
            CString cstrWinMsg;
            if (m_bBlackChessNum != m_bWriteChessNum)
                cstrWinMsg = m_bBlackChessNum > m_bWriteChessNum ?
                _T("黑方胜") : _T("白方胜");
            else if (m_tWriteUsingTime != m_tBlackUsingTime)
                cstrWinMsg = m_tWriteUsingTime > m_tBlackUsingTime ?
                _T("黑方胜") : _T("白方胜");
            else
                cstrWinMsg = _T("平局");
            m_bIsGameStart = FALSE; //标记游戏结束
            MessageBox(cstrWinMsg, _T("黑白棋"),
                MB_ICONINFORMATION | MB_OK);   
        }   
    m_psNowTmp.set_player_not_change(m_bPlayerNotChange);
    m_stpsStep.push(m_psNowTmp);
}

//logo换手
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

//落子合法性检查
BOOL COthelloDlg::down_check(LineNumber __l,
    ColNumber __c, vecPairLiCol * __v) const
{
    if (m_psNow.get_state_by_coord(__l, __c) == e_psBlank &&
        reverse_chess(__l, __c, m_bIsWriteDownChess, __v) 
        == TRUE  //横纵斜4向检查,看看是否可以翻转对方
        )
        return TRUE;
    return FALSE;
}

//横纵斜4向检查,可走返回TRUE
BOOL COthelloDlg::reverse_chess(
    LineNumber __l, ColNumber __c,  //检查中心点坐标
    BOOL __ISW,     //检查中心点棋子颜色
    vecPairLiCol* __v) const
{
    pairLiCol tmp;
    //查找匹配对象
    PointState psMatch = __ISW == WriteChess ?
        e_psWriteDown : e_psBlackDown;
    //从检查点往上走,找第一个
    for (LineNumber i = __l - 1; i >= 1; i--)
    {
        PointState psNow = m_psNow.get_state_by_coord(i, __c);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr && i != __l - 1)
                return TRUE;
            //找到匹配,从此往下全部压入vector
            for (LineNumber j = i + 1; j < __l; j++)
            {
                tmp.first = j;
                tmp.second = __c;
                __v->push_back(tmp);
            }
            break;
        }
    }
        
    //从检查点往下走,找第一个
    for (LineNumber i = __l + 1; i <= 8; i++)
    {
        PointState psNow = m_psNow.get_state_by_coord(i, __c);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr && i != __l + 1)
                return TRUE;
            //找到匹配,从此往上压入vector
            for (LineNumber j = i - 1; j > __l; j--)
            {
                tmp.first = j;
                tmp.second = __c;
                __v->push_back(tmp);
            }
            break;
        }
    }
        
    //从检查点往左走,找第一个
    for (ColNumber i = __c - 1; i >= 1; i--)
    {
        PointState psNow = m_psNow.get_state_by_coord(__l, i);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr && i != __c - 1)
                return TRUE;
            //找到匹配,从此往右压入vector
            for (ColNumber j = i + 1; j < __c; j++)
            {
                tmp.first = __l;
                tmp.second = j;
                __v->push_back(tmp);
            }
            break;
        }
    }
        
    //从检查点往右走,找第一个
    for (ColNumber i = __c + 1; i <= 8; i++)
    {
        PointState psNow = m_psNow.get_state_by_coord(__l, i);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr && i != __c + 1)
                return TRUE;
            //找到匹配,从此往左压入vector
            for (ColNumber j = i - 1; j > __c; j--)
            {
                tmp.first = __l;
                tmp.second = j;
                __v->push_back(tmp);
            }
            break;
        }
    }
        
    //从检查点往左上走,找第一个
    for (Number i = __c - 1, j = __l - 1;
        i >= 1 && j >= 1; i--, j--)
    {
        PointState psNow = m_psNow.get_state_by_coord(j, i);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr && i != __c - 1 && j != __l - 1)
                return TRUE;
            //找到匹配,从此往右下压入vector
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
        
    //从检查点往左下走,找第一个
    for (Number i = __l + 1, j = __c - 1;
        i <= 8 && j <= 8; i++, j--)
    {
        PointState psNow = m_psNow.get_state_by_coord(i, j);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr && i != __l + 1 && j != __c - 1)
                return TRUE;
            //找到匹配,从此往右上压入vector
            for (Number p = i - 1, r = j + 1;
                p >= __l + 1 && r <= __c - 1; p--, r++)
            {
                tmp.first = p;
                tmp.second = r;
                __v->push_back(tmp);
            }
            break;
        }
    }  

    //从检查点往右上走,找第一个
    for (Number i = __l - 1, j = __c + 1;
        i >= 1 && j <= 8; i--, j++)
    {
        PointState psNow = m_psNow.get_state_by_coord(i, j);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr&&i != __l - 1 && j != __c + 1)
                return TRUE;
            //找到匹配,从此往左下压入vector
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

    //从检查点往右下走,找第一个
    for (Number i = __l + 1, j = __c + 1;
        i <= 8 && j <= 8; i++, j++)
    {
        PointState psNow = m_psNow.get_state_by_coord(i, j);
        if (psNow == e_psBlank)
            break;
        if (psNow == psMatch)
        {
            if (__v == nullptr && i != __l + 1 && j != __c + 1)
                return TRUE;
            //找到匹配,从此往左上压入vector
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
    return __v == nullptr ? FALSE :
        __v->size() != 0 ? TRUE : FALSE;
}

//刷新界面上的棋子个数
void COthelloDlg::refresh_chess_num() const
{
    CString cstrStep;
    cstrStep.Format(_T("白棋目前存活%d子"), m_bWriteChessNum);
    GetDlgItem(IDC_StaWrNum)->SetWindowTextW(cstrStep);
    cstrStep.Format(_T("黑棋目前存活%d子"), m_bBlackChessNum);
    GetDlgItem(IDC_StBlNum)->SetWindowTextW(cstrStep);
}

//对一方下一步的可行性作出判断
//(__l,__c)为扩散点,以此点为中心(检测时不包含此点)
//先定住行不变,以列为基本单位,向左右扩散,
//主要发现一个可行点即return
//__ISW作为检测色,检测该色棋子的可行性
//有了处理列的can_do_nexe_col函数,那我们只在这里处理行
BOOL COthelloDlg::can_do_next(LineNumber __l, ColNumber __c,
    BOOL __ISW)const
{
    //退化情况:棋盘满了,直接返回FALSE
    if (m_bWriteChessNum + m_bBlackChessNum == 64)
        return FALSE;
    //向上
    for (LineNumber i = __l; i >= 1; i--)
        if (can_do_nexe_col(i, __c, __ISW) == TRUE)
            return TRUE;
    //向下
    for (LineNumber i = __l + 1; i <= 8; i++)
        if (can_do_nexe_col(i, __c, __ISW) == TRUE)
            return TRUE;
    return FALSE;
}

//定行检列,行为定住的那一行,列为中心点的列号
//__ISW做检测色
BOOL COthelloDlg::can_do_nexe_col(LineNumber __l, ColNumber __c,
    BOOL __ISW) const
{
    //向左检列,列-
    for (ColNumber i = __c; i >= 1; i--)
        if (m_psNow.get_state_by_coord(__l, i) == e_psBlank &&
            reverse_chess(__l, i, __ISW) == TRUE)
            return TRUE;
    //向右检测,列+
    for (ColNumber i = __c; i <= 8; i++)
        if (m_psNow.get_state_by_coord(__l, i) == e_psBlank &&
            reverse_chess(__l, i, __ISW) == TRUE)
            return TRUE;
    return FALSE;
}

//构造函数
COthelloDlg::COthelloDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OTHELLO_DIALOG, pParent),
    m_bIsFirstStart(TRUE), m_bIsGameStart(FALSE),
    m_bPlayerNotChange(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    init_game_start();  //初始化游戏数据
}

void COthelloDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


//消息映射
BEGIN_MESSAGE_MAP(COthelloDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BuClose, &COthelloDlg::OnBnClickedBuclose)
    ON_BN_CLICKED(IDC_BuStart, &COthelloDlg::OnBnClickedBustart)
    ON_BN_CLICKED(IDC_BuUndo, &COthelloDlg::OnBnClickedBuundo)
    ON_WM_LBUTTONDOWN()     //消息映射中接受鼠标消息
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BuLose, &COthelloDlg::OnBnClickedBulose)
END_MESSAGE_MAP()


// COthelloDlg 消息处理程序

BOOL COthelloDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COthelloDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR COthelloDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//关闭程序按钮事件响应
void COthelloDlg::OnBnClickedBuclose()
{
    exit(0);
}

//新开局按钮事件响应
void COthelloDlg::OnBnClickedBustart()
{
    if (m_bIsFirstStart == FALSE)
        if (
            MessageBox(_T("重新开始一局比赛?"),
                _T("黑白棋"),
                MB_YESNO | MB_ICONINFORMATION)
            == IDNO
            )
                return; //不重新开始
    m_bIsFirstStart = FALSE;  //标记已经不是第一局了
    m_bIsGameStart = TRUE;  //标记游戏开始,对鼠标消息作出反应
    init_game_start();      //初始化游戏数据

    //初始化棋盘显示数据
    GetDlgItem(IDC_StaWrTime)->SetWindowTextW(_T("白棋用时:       0s"));
    GetDlgItem(IDC_StaBlTime)->SetWindowTextW(_T("黑棋用时:       0s"));
    GetDlgItem(IDC_StaWrNum)->SetWindowTextW(_T("白棋目前存活2子"));
    GetDlgItem(IDC_StBlNum)->SetWindowTextW(_T("黑棋目前存活2子"));
    //绘制白方行棋logo
    m_cbmpWriteChess.Draw(*this->GetDC(),
        WritePlayerLogoX, WritePlayerLogoY);
    m_cbmpDlgBack.Draw(*this->GetDC(),
        BlackPlayerLogoX, BlackPlayerLogoY);
    //绘制棋盘
    m_cbmpChessBoard.Draw(*this->GetDC(),
        ChessBoardLeftUppX, ChessBoardLeftUppY);
    //绘制初始的四个棋子
    draw_chess_by_coord(WriteChess, 4, 4);
    draw_chess_by_coord(WriteChess, 5, 5);
    draw_chess_by_coord(BlackChess, 4, 5);
    draw_chess_by_coord(BlackChess, 5, 4);
    //计时器计时开始
    SetTimer(1, 1000, nullptr);
}

//处理鼠标点击事件
void COthelloDlg::OnLButtonDown(UINT nFlags, 
    CPoint point)
{
    if (m_bIsGameStart == FALSE)
        return; //游戏未开始不对鼠标消息作出响应
    if (point.x<MouseAnswerFarLeftX ||
        point.x>MouseAnswerFarRightX ||
        point.y<MouseAnswerFarUpY ||
        point.y>MouseAnswerFarDownY)
        return;     //超出棋盘范围不做响应
    //计算鼠标点击的棋盘行列号
    LineNumber ClickLineNumber{ //行号
        static_cast<LineNumber>(
            (point.y - MouseAnswerFarUpY) / ChessSize + 1
        )
    };
    ColNumber ClickColumnNumber{ //列号
        static_cast<ColNumber>(
            (point.x - MouseAnswerFarLeftX) / ChessSize + 1
        )
    };
    //落子处理
    chess_player_down(ClickLineNumber, ClickColumnNumber);
}

//悔棋
void COthelloDlg::OnBnClickedBuundo()
{   
    if (m_bIsGameStart == FALSE)
    {
        MessageBox(_T("游戏未开始,不能悔棋"), _T("黑白棋"),
            MB_ICONERROR | MB_OK);
        return; //退化情况
    }      
    if (m_stpsStep.size() == 0)
    {
        MessageBox(_T("无棋可悔"),_T("黑白棋"),
            MB_ICONWARNING);
        return;
    }
    CString cstrMsg;
    cstrMsg.Format(_T("%s棋手,你想让对方悔棋吗?对方将罚时60s!"),
        m_bIsWriteDownChess == WriteChess ?
        _T("白方") : _T("黑方"));
    if (MessageBox(cstrMsg, _T("黑白棋"),
        MB_YESNO | MB_ICONINFORMATION) == IDNO)
        return;
    //罚时
    m_bIsWriteDownChess == WriteChess ?
        m_tBlackUsingTime += 60 :
        m_tWriteUsingTime += 60;
    //复原棋盘
    m_psNow = m_stpsStep.top(); //取顶
    m_stpsStep.pop();           //弹出  
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
    if (m_bPlayerNotChange != TRUE)
    {
        //上一步换手了就再换回去
        player_logo_change();   //logo换手
        m_bIsWriteDownChess = !m_bIsWriteDownChess; //换手
    }   
}

//计时器回调函数
void COthelloDlg::OnTimer(UINT_PTR nIDEvent)
{
    CString cstrTime;
    if (m_bIsWriteDownChess == WriteChess)
    {
        m_tWriteUsingTime++;
        cstrTime.Format(_T("白棋用时:%7lds"), m_tWriteUsingTime);
        GetDlgItem(IDC_StaWrTime)->SetWindowTextW(cstrTime);
    }
    else
    {
        m_tBlackUsingTime++;
        cstrTime.Format(_T("黑棋用时:%7lds"), m_tBlackUsingTime);
        GetDlgItem(IDC_StaBlTime)->SetWindowTextW(cstrTime);
    }
    CDialogEx::OnTimer(nIDEvent);
}

//单击认输按钮
void COthelloDlg::OnBnClickedBulose()
{
    if (m_bIsGameStart == FALSE)
    {
        MessageBox(_T("游戏未开始,不能认输"), _T("黑白棋"),
            MB_ICONERROR | MB_OK);
        return; //退化情况
    }
    CString cstrLoser;
    cstrLoser = m_bIsWriteDownChess == WriteChess ?
        _T("白方") : _T("黑方");
    CString cstrMsg;
    cstrMsg.Format(_T("%s,确定认输吗?"), cstrLoser);
    if (MessageBox(cstrMsg, _T("黑白棋"), MB_YESNO | MB_ICONWARNING) ==
        IDNO)
        return;
    KillTimer(1);
    m_bIsGameStart = FALSE;
    cstrMsg.Format(_T("%s认输"), cstrLoser);
    MessageBox(cstrMsg, _T("黑白棋"), MB_OK | MB_ICONINFORMATION);   
}
