#pragma once
struct Pieces//棋子判断单元
{
	bool bIsPieces;//是否已有棋
	bool bIsBlack;//黑棋或白棋
	int x,y;//棋子所在格
};

struct MyRectangle
{
	int x,y;//选中框核心坐标
};

//五子棋算法主类
class CMainGobang
{
public:
	CMainGobang(void);
	~CMainGobang(void);
private:
	/////basic information
	int iSize;//棋盘大小参数
	int xmax,xmin,ymax,ymin;//棋盘大小参数
	int idx;//棋盘每小格长度
	/////pieces information
	int iPieces;//棋子计数//当前棋子计数为iPieces-1
	int iPiecesNum;//棋子总数
	Pieces *pPieces;//棋子
	/////rectangle information
	int iRectangle;//当前选中框计数
	MyRectangle *pmyRectangle;//选中框
	/////for draw rectangle&draw pieces
	bool NoRectangle(int x,int y);//判断该位置上是否有选中框
	bool NoPieces(int x,int y);//判断该位置上是否有棋子
	/////for judge win
	bool IsBlack(int x,int y);//判断该位置上棋子颜色
	bool IsConnect(int iDir,int x,int y);//查找相连的棋子
	int ConnectNum();//返回相连棋子个数
	/////for machine fight
	//void MaConnectNum(int iConnectNum,int iDir);//返回弱相连棋子个数/iDir方向参数
	//int ConnectDir();//返回相连棋子方向
public:
	void DrawMainTable(CDC *pDC,CRect rect,int iTableSize);//绘制棋盘函数
	void SetPieces();//设参函数//将初始化代码与绘制棋盘代码分开
	void DrawRectangle(CDC *pDC,CPoint pt);//绘制选中框/返回是否绘制
	void DrawPieces(CDC *pDC,CRect rect,CPoint pt);//绘制棋子函数
	bool ClearPieces(CDC *pDC,bool &bBlack);//清除棋子函数/用于悔棋
	void KeepPieces(CDC *pDC,CRect rect);//循环绘制棋子 //用以解决刷新问题
	//void MachineDraw(CDC *pDC);//机器下棋函数

	bool Success();//判断胜利函数
	void ReplayMark(CDC *pDC);//复盘标记函数
};

