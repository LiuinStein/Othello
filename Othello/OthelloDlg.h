
// OthelloDlg.h : ͷ�ļ�
//

#pragma once
//#include "NowGame.h"
#include "DataStruct.h"
#include <vector>


/// COthelloDlg �Ի���
class COthelloDlg : public CDialogEx
{
///��ֱ�������
protected:    
    ChessNumber      m_bWriteChessNum;     //��ǰ�����ϰ��ӵ���Ŀ
    ChessNumber      m_bBlackChessNum;     //��ǰ�����Ϻ��ӵ���Ŀ
    UseTime          m_tWriteUsingTime;    //������ʱsec
    UseTime          m_tBlackUsingTime;    //������ʱsec
    CImage           m_cbmpChessBoard;     //���̱���ͼ
    CImage           m_cbmpBlank;          //��������(����ר��)
    CImage           m_cbmpBlackChess;     //����λͼ
    CImage           m_cbmpWriteChess;     //����λͼ
    CImage           m_cbmpDlgBack;        //�Ի��򱳾�ͼ
    BOOL             m_bIsFirstStart;      //��һ�ο���ΪTRUE,��n(>1)ΪFALSE
    BOOL             m_bIsWriteDownChess;  //��ǰһ������һ��,TRUEΪ����
    BOOL             m_bIsGameStart;       //��ǰ��Ϸ�Ƿ�ʼ
    BOOL             m_bPlayerNotChange;   //�������һ��δ����ʱΪTRUE
    CChessBoardState m_psNow;              //��ǰ������ÿ�����״̬
    stkBrdSte        m_stpsStep;           //���̼�¼,���ڻ������

///��ִ���������
private:    
    //��Ϸ(����)��ʼ,��ʼ������
    void init_game_start(); 

    //������ָ�������������
    void draw_chess_by_coord(BOOL __ISW,
        LineNumber __l, ColNumber __c);

    //��������
    void chess_player_down(LineNumber __l, 
        ColNumber __c);

    //logo����
    void player_logo_change();

    //��Ч���жϼ�����,����TRUE����һ����Ч,������Ч
    //��Ҫ��ת���������Ӵ����vector����
    BOOL down_check(LineNumber __l, ColNumber __c,
        vecPairLiCol * __v) const;

    //����б4����,���߷���TRUE
    BOOL reverse_chess(LineNumber __l, ColNumber __c,BOOL __ISW,
        vecPairLiCol * __v = nullptr) const;

    //ˢ�½����ϵ����Ӹ���
    void refresh_chess_num() const;

    //��һ����һ���Ŀ����������ж�
    //����������,��һ�����ܻ���,˫����������(��������),
    //��������Ŀ��ʤ��
    BOOL can_do_next(LineNumber __l, ColNumber __c, BOOL __ISW)const;

    //���м���,������can_do_next
    BOOL can_do_nexe_col(LineNumber __l, ColNumber __c, BOOL __ISW)const;


/// ����
public:
	COthelloDlg(CWnd* pParent = nullptr);	// ��׼���캯��

/// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OTHELLO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


/// ʵ��
protected:
	HICON m_hIcon;  //ͼ��

/// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
///��Ϣӳ����Ӧ����
    //����رհ�ť
    afx_msg void OnBnClickedBuclose();  
    //�����ʼ��Ϸ��ť
    afx_msg void OnBnClickedBustart();
    //�麯����д,��Ӧ�����������е�������Ϣ
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //������尴ť
    afx_msg void OnBnClickedBuundo();
    //�麯����д,��ʱ���ص�����
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedBulose();
};
