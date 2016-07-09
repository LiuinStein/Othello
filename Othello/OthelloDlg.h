
// OthelloDlg.h : 头文件
//

#pragma once
//#include "NowGame.h"
#include "DataStruct.h"
#include <vector>


/// COthelloDlg 对话框
class COthelloDlg : public CDialogEx
{
///棋局变量部分
protected:    
    ChessNumber      m_bWriteChessNum;     //当前棋盘上白子的数目
    ChessNumber      m_bBlackChessNum;     //当前棋盘上黑子的数目
    UseTime          m_tWriteUsingTime;    //白子用时sec
    UseTime          m_tBlackUsingTime;    //黑子用时sec
    CImage           m_cbmpChessBoard;     //棋盘背景图
    CImage           m_cbmpBlank;          //棋盘无子(悔棋专用)
    CImage           m_cbmpBlackChess;     //黑子位图
    CImage           m_cbmpWriteChess;     //白子位图
    CImage           m_cbmpDlgBack;        //对话框背景图
    BOOL             m_bIsFirstStart;      //第一次开局为TRUE,第n(>1)为FALSE
    BOOL             m_bIsWriteDownChess;  //当前一步落子一方,TRUE为白子
    BOOL             m_bIsGameStart;       //当前游戏是否开始
    BOOL             m_bPlayerNotChange;   //悔棋的上一步未换手时为TRUE
    CChessBoardState m_psNow;              //当前棋盘上每个点的状态
    stkBrdSte        m_stpsStep;           //棋盘记录,用于悔棋过程

///棋局处理函数部分
private:    
    //游戏(重新)开始,初始化数据
    void init_game_start(); 

    //在棋盘指定坐标绘制棋子
    void draw_chess_by_coord(BOOL __ISW,
        LineNumber __l, ColNumber __c);

    //棋手落子
    void chess_player_down(LineNumber __l, 
        ColNumber __c);

    //logo换手
    void player_logo_change();

    //有效性判断及翻子,返回TRUE即这一步有效,否则无效
    //需要翻转过来的棋子存放在vector里面
    BOOL down_check(LineNumber __l, ColNumber __c,
        vecPairLiCol * __v) const;

    //横纵斜4向检查,可走返回TRUE
    BOOL reverse_chess(LineNumber __l, ColNumber __c,BOOL __ISW,
        vecPairLiCol * __v = nullptr) const;

    //刷新界面上的棋子个数
    void refresh_chess_num() const;

    //对一方下一步的可行性作出判断
    //单方不可行,下一步不能换手,双方均不可行(棋盘已满),
    //以棋子数目分胜负
    BOOL can_do_next(LineNumber __l, ColNumber __c, BOOL __ISW)const;

    //定行检列,服务于can_do_next
    BOOL can_do_nexe_col(LineNumber __l, ColNumber __c, BOOL __ISW)const;


/// 构造
public:
	COthelloDlg(CWnd* pParent = nullptr);	// 标准构造函数

/// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OTHELLO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


/// 实现
protected:
	HICON m_hIcon;  //图标

/// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
///消息映射响应部分
    //点击关闭按钮
    afx_msg void OnBnClickedBuclose();  
    //点击开始游戏按钮
    afx_msg void OnBnClickedBustart();
    //虚函数重写,响应在棋盘区域中点击鼠标消息
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //点击悔棋按钮
    afx_msg void OnBnClickedBuundo();
    //虚函数重写,计时器回调函数
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedBulose();
};
