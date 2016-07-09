#pragma once
#include <vector>
#include <stack>

class CChessBoardState;     //类的前向声明

typedef unsigned long UseTime;      //时间记录(单位sec)
typedef unsigned char Number;       //行列坐标数类型
typedef Number        LineNumber;   //行
typedef Number        ColNumber;    //列
typedef unsigned char ChessNumber;  //棋子数目
typedef int           Coord;        //坐标
//棋子序列(悔棋专用)
typedef std::pair<LineNumber, ColNumber>    pairLiCol;  //行列坐标
typedef std::vector<pairLiCol>              vecPairLiCol;//坐标序列
//棋盘序列
typedef std::stack<CChessBoardState>        stkBrdSte;

enum PointState     //棋盘上单点的状态
{
    e_psWriteDown   = 0x01,  //白子
    e_psBlackDown   = 0x02,  //黑子
    e_psBlank       = 0x03   //空(可落子)
};


class CChessBoardState  //当前棋盘的状态
{

    PointState  m_psState[8][8];    //当前状态
    ChessNumber m_cnWrite;          //白子数目
    ChessNumber m_cnBlack;          //黑子数目
public:
    //默认构造函数
    CChessBoardState()
    {
        init();
    }

    //指针需要深拷贝,不得不自己来
    CChessBoardState(const CChessBoardState & __c):
        m_cnBlack(__c.m_cnBlack),m_cnWrite(__c.m_cnWrite)
    {
        //深拷贝
        for (LineNumber i = 0; i < 8; i++)
            for (ColNumber j = 0; j < 8; j++)
                m_psState[i][j] = __c.m_psState[i][j];
    }

    //设置当前的棋子数目
    void set_chess_number(ChessNumber __w,ChessNumber __b)
    {
        m_cnWrite = __w;
        m_cnBlack = __b;
    }

    //设置棋子当前状态
    void set_chess_state(LineNumber __l,
        ColNumber __c,PointState __ps)
    {
        if (__l <= 0 && __l > 8 && __c <= 0 && __c > 8)
        {
            AfxMessageBox(_T("set_chess_state函数出现异常,下标越界"));
            exit(1);    //错误退出
        }
        m_psState[__l - 1][__c - 1] = __ps;
    }
    
    //获取指定位置的棋盘状态,为方便从1到8就可以
    PointState get_state_by_coord(LineNumber __l,
        ColNumber __c)const
    {
        if (__l <= 0 && __l > 8 && __c <= 0 && __c > 8)
        {
            AfxMessageBox(_T("get_state_by_coord函数出现异常,下标越界"));
            exit(1);    //错误退出
        }     
        return m_psState[__l - 1][__c - 1];
    }

    //获取当前的棋子数目
    void get_chess_number(ChessNumber * __w,
        ChessNumber * __b)const
    {
        *__w = m_cnWrite;
        *__b = m_cnBlack;
    }


    //初始化
    void init()
    {
        for (LineNumber i = 0; i < 8; i++)
            for (ColNumber j = 0; j < 8; j++)
                m_psState[i][j] = e_psBlank;
        m_cnBlack = m_cnWrite = 2;
        //初始情况下棋盘上的四个子
        m_psState[3][3] = m_psState[4][4] = e_psWriteDown;
        m_psState[3][4] = m_psState[4][3] = e_psBlackDown;
    }

};
