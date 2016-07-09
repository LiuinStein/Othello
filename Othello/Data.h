#pragma once

//常数表
enum
{
    ChessBoardLeftUppX      = 150,  //棋盘左上角X
    ChessBoardLeftUppY      = 20,   //棋盘左上角Y
    ChessSize               = 45,   //棋子的大小,已考虑边界因素
    ChessBoard_1A_LeftUppX  = 20,   //棋盘1A位左上角X坐标
    ChessBoard_1A_LeftUppY  = 20,   //棋盘1A位左上角Y坐标
    WriteChess              = TRUE, //用于IsWriteChess BOOL类型判断
    BlackChess              = FALSE,//用于IsWriteChess BOOL类型判断
    //鼠标点击事件,驱动响应范围坐标(1A左上角确定的棋盘边界线)
    MouseAnswerFarLeftX     = 170,  //鼠标响应最左端
    MouseAnswerFarRightX    = 530,  //鼠标响应最右端
    MouseAnswerFarUpY       = 40,   //鼠标响应最上端
    MouseAnswerFarDownY     = 400,  //鼠标响应最下端
    //走棋方标示位图左上角坐标
    WritePlayerLogoX        = 60,   //白方行棋标示X
    WritePlayerLogoY        = 80,   //白方行棋标示Y
    BlackPlayerLogoX        = 60,   //黑方行棋标示X
    BlackPlayerLogoY        = 310   //黑方行棋标示Y
};
