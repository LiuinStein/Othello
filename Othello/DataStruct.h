#pragma once
#include <vector>
#include <stack>

class CChessBoardState;     //���ǰ������

typedef unsigned long UseTime;      //ʱ���¼(��λsec)
typedef unsigned char Number;       //��������������
typedef Number        LineNumber;   //��
typedef Number        ColNumber;    //��
typedef unsigned char ChessNumber;  //������Ŀ
typedef int           Coord;        //����
//��������(����ר��)
typedef std::pair<LineNumber, ColNumber>    pairLiCol;  //��������
typedef std::vector<pairLiCol>              vecPairLiCol;//��������
//��������
typedef std::stack<CChessBoardState>        stkBrdSte;

enum PointState     //�����ϵ����״̬
{
    e_psWriteDown   = 0x01,  //����
    e_psBlackDown   = 0x02,  //����
    e_psBlank       = 0x03   //��(������)
};


class CChessBoardState  //��ǰ���̵�״̬
{

    PointState  m_psState[8][8];    //��ǰ״̬
    ChessNumber m_cnWrite;          //������Ŀ
    ChessNumber m_cnBlack;          //������Ŀ
public:
    //Ĭ�Ϲ��캯��
    CChessBoardState()
    {
        init();
    }

    //ָ����Ҫ���,���ò��Լ���
    CChessBoardState(const CChessBoardState & __c):
        m_cnBlack(__c.m_cnBlack),m_cnWrite(__c.m_cnWrite)
    {
        //���
        for (LineNumber i = 0; i < 8; i++)
            for (ColNumber j = 0; j < 8; j++)
                m_psState[i][j] = __c.m_psState[i][j];
    }

    //���õ�ǰ��������Ŀ
    void set_chess_number(ChessNumber __w,ChessNumber __b)
    {
        m_cnWrite = __w;
        m_cnBlack = __b;
    }

    //�������ӵ�ǰ״̬
    void set_chess_state(LineNumber __l,
        ColNumber __c,PointState __ps)
    {
        if (__l <= 0 && __l > 8 && __c <= 0 && __c > 8)
        {
            AfxMessageBox(_T("set_chess_state���������쳣,�±�Խ��"));
            exit(1);    //�����˳�
        }
        m_psState[__l - 1][__c - 1] = __ps;
    }
    
    //��ȡָ��λ�õ�����״̬,Ϊ�����1��8�Ϳ���
    PointState get_state_by_coord(LineNumber __l,
        ColNumber __c)const
    {
        if (__l <= 0 && __l > 8 && __c <= 0 && __c > 8)
        {
            AfxMessageBox(_T("get_state_by_coord���������쳣,�±�Խ��"));
            exit(1);    //�����˳�
        }     
        return m_psState[__l - 1][__c - 1];
    }

    //��ȡ��ǰ��������Ŀ
    void get_chess_number(ChessNumber * __w,
        ChessNumber * __b)const
    {
        *__w = m_cnWrite;
        *__b = m_cnBlack;
    }


    //��ʼ��
    void init()
    {
        for (LineNumber i = 0; i < 8; i++)
            for (ColNumber j = 0; j < 8; j++)
                m_psState[i][j] = e_psBlank;
        m_cnBlack = m_cnWrite = 2;
        //��ʼ����������ϵ��ĸ���
        m_psState[3][3] = m_psState[4][4] = e_psWriteDown;
        m_psState[3][4] = m_psState[4][3] = e_psBlackDown;
    }

};
