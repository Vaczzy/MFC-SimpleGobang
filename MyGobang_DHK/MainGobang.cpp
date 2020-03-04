#include "StdAfx.h"
#include "MainGobang.h"


CMainGobang::CMainGobang(void)
{
	//AfxMessageBox(_T("主类执行了构造函数"));
}


CMainGobang::~CMainGobang(void)
{
	if(pPieces!=NULL)
	{
		delete []pPieces;
		pPieces=NULL;
	}
	if(pmyRectangle!=NULL)
	{
		delete []pmyRectangle;
		pmyRectangle=NULL;
	}
	//AfxMessageBox(_T("主类执行了析构函数"));
}

//绘制棋盘函数
//后期可添加棋盘大小参数
//控件坐标
void CMainGobang::DrawMainTable(CDC *pDC,CRect rect,int iTableSize)
{
	iSize=iTableSize;
	//先覆盖背景
	CRect Back;
	Back.top=rect.top+1;
	Back.bottom=rect.bottom-1;
	Back.left=rect.left+1;
	Back.right=rect.right-1;
	pDC->FillSolidRect(&Back,RGB(240,240,240));//填充
	//计算每格长度
	idx=(((rect.right-rect.left)/(iTableSize+1))>((rect.bottom-rect.top)/(iTableSize+1)))?
		(rect.bottom-rect.top)/(iTableSize+1):
	    (rect.right-rect.left)/(iTableSize+1);
	CBrush brush(RGB(0,0,0));
	CBrush *OldBrush=pDC->SelectObject(&brush);
	int ixp,iyp;//棋盘星坐标
	//绘制天元
	ixp=(rect.right-rect.left)/2;
	iyp=(rect.bottom-rect.top)/2;
	pDC->Ellipse(ixp-3,iyp-3,ixp+3,iyp+3);
	//绘制星
	ixp-=4*idx;
	iyp-=4*idx;//左上角
	pDC->Ellipse(ixp-3,iyp-3,ixp+3,iyp+3);
	iyp+=8*idx;//左下角
	pDC->Ellipse(ixp-3,iyp-3,ixp+3,iyp+3);
	ixp+=8*idx;//右下角
	pDC->Ellipse(ixp-3,iyp-3,ixp+3,iyp+3);
	iyp-=8*idx;//右上角
	pDC->Ellipse(ixp-3,iyp-3,ixp+3,iyp+3);
	pDC->SelectObject(OldBrush);//恢复设备环境中原来的画笔
	brush.DeleteObject();//释放绘图资源
	//计算棋盘大小并放置于窗口中间
	ymin=(rect.bottom-rect.top)/2-(iTableSize-1)/2*idx;
	ymax=ymin+(iTableSize-1)*idx;
	xmin=(rect.right-rect.left)/2-(iTableSize-1)/2*idx;
	xmax=xmin+(iTableSize-1)*idx;
	for(int x=xmin;x<=xmax;x+=idx)
	{
		pDC->MoveTo(x,ymin);
		pDC->LineTo(x,ymax);
	}
	for(int y=ymin;y<=ymax;y+=idx)
	{
		pDC->MoveTo(xmin,y);
		pDC->LineTo(xmax,y);
	}
}

//设参函数
void CMainGobang::SetPieces()
{
	iPiecesNum=iSize*iSize;//棋子判断单元数
	iPieces=0;//棋子计数
	pPieces=new Pieces[iPiecesNum];//生成存储棋子判断
	for(int i=0;i<iPiecesNum;i++)
	{
		pPieces[i].bIsPieces=false;//默认该位置没有棋子
		if(i%2!=0)
		{
			pPieces[i].bIsBlack=false;//双数为白棋子
		}
		else
		{
			pPieces[i].bIsBlack=true;//单数为黑棋子
		}
	}
	iRectangleNum=0;
	pmyRectangle=new MyRectangle[2];//生成选中框存储单元
}

//绘制选中框
void CMainGobang::DrawRectangle(CDC *pDC,CPoint pt)
{
	//修复鼠标位置在窗口外的情况2020.03.04
	if (pt.x >= 0 && pt.y >= 0)
	{
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));//创建画笔
		CPen *OldPen = pDC->SelectObject(&pen);
		//获取离鼠标位置最近的棋格
		int x, y;//棋格坐标
		x = xmin+int(double(pt.x - xmin) / idx + 0.5)*idx;
		y = ymin+int(double(pt.y - ymin) / idx + 0.5)*idx;
		x = x > xmax ? xmax : x;
		y = y > ymax ? ymax : y;
		//若此处无棋子 无选中框
		if ((NoPieces(x, y)) && (NoRectangle(x, y)))
		{
			//画选中框
			pDC->MoveTo(x - 10, y - 5);
			pDC->LineTo(x - 10, y - 10);
			pDC->LineTo(x - 5, y - 10);

			pDC->MoveTo(x + 10, y - 5);
			pDC->LineTo(x + 10, y - 10);
			pDC->LineTo(x + 5, y - 10);

			pDC->MoveTo(x - 10, y + 5);
			pDC->LineTo(x - 10, y + 10);
			pDC->LineTo(x - 5, y + 10);

			pDC->MoveTo(x + 10, y + 5);
			pDC->LineTo(x + 10, y + 10);
			pDC->LineTo(x + 5, y + 10);
			//赋值
			pmyRectangle[iRectangleNum].x = x;
			pmyRectangle[iRectangleNum].y = y;
			iRectangleNum++;
		}
		if (iRectangleNum == 2)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(240, 240, 240));//创建画笔
			CPen *OldPen = pDC->SelectObject(&pen);
			int x, y;
			x = pmyRectangle[0].x;
			y = pmyRectangle[0].y;
			//清除选中框
			pDC->MoveTo(x - 10, y - 5);
			pDC->LineTo(x - 10, y - 10);
			pDC->LineTo(x - 5, y - 10);

			pDC->MoveTo(x + 10, y - 5);
			pDC->LineTo(x + 10, y - 10);
			pDC->LineTo(x + 5, y - 10);

			pDC->MoveTo(x - 10, y + 5);
			pDC->LineTo(x - 10, y + 10);
			pDC->LineTo(x - 5, y + 10);

			pDC->MoveTo(x + 10, y + 5);
			pDC->LineTo(x + 10, y + 10);
			pDC->LineTo(x + 5, y + 10);
			//重置赋值
			pmyRectangle[0].x = pmyRectangle[1].x;
			pmyRectangle[0].y = pmyRectangle[1].y;
			iRectangleNum = 1;//重置计数
		}
		pDC->SelectObject(OldPen);//恢复设备环境中原来的画笔
		pen.DeleteObject();//释放绘图资源
	}
}

//绘制棋子函数
void CMainGobang::DrawPieces(CDC *pDC,CRect rect,CPoint pt)
{
	int iTmp;
	if(pPieces[iPieces].bIsBlack==true)//黑棋子
	{
		iTmp=0;
	}
	if(pPieces[iPieces].bIsBlack==false)//白棋子
	{
		iTmp=255;
	}
	//修复鼠标位置在窗口外的情况2020.03.04
	if (pt.x >= 0 && pt.y >= 0)
	{
		//获取离鼠标位置最近的棋格
		int x, y;//棋格坐标
		x = xmin + int(double(pt.x - xmin) / idx + 0.5)*idx;
		y = ymin + int(double(pt.y - ymin) / idx + 0.5)*idx;
		x = x > xmax ? xmax : x;
		y = y > ymax ? ymax : y;
		if (NoPieces(x, y))
		{
			//画棋子
			CBrush brush(RGB(iTmp, iTmp, iTmp));
			CBrush *OldBrush = pDC->SelectObject(&brush);
			pDC->Ellipse(x - 10, y - 10, x + 10, y + 10);
			pDC->SelectObject(OldBrush);//恢复设备环境中原来的画笔
			brush.DeleteObject();//释放绘图资源
			//清除标记
			CPen penClearMark;//用于清除标记的画笔
			penClearMark.CreatePen(PS_SOLID, 1, RGB(abs(iTmp - 255), abs(iTmp - 255), abs(iTmp - 255)));
			CPen *OldPenClearMark = pDC->SelectObject(&penClearMark);
			pDC->MoveTo(pPieces[iPieces - 1].x, pPieces[iPieces - 1].y - 3);
			pDC->LineTo(pPieces[iPieces - 1].x, pPieces[iPieces - 1].y + 4);
			pDC->MoveTo(pPieces[iPieces - 1].x - 3, pPieces[iPieces - 1].y);
			pDC->LineTo(pPieces[iPieces - 1].x + 4, pPieces[iPieces - 1].y);
			pDC->SelectObject(OldPenClearMark);//恢复设备环境中原来的画笔
			penClearMark.DeleteObject();//释放绘图资源
			//画标记
			CPen penMark;//用于绘制标记的画笔
			penMark.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			CPen *OldPenMark = pDC->SelectObject(&penMark);
			pDC->MoveTo(x, y - 3);
			pDC->LineTo(x, y + 4);//弥补增量太小导致的不对称
			pDC->MoveTo(x - 3, y);
			pDC->LineTo(x + 4, y);
			pDC->SelectObject(OldPenMark);//恢复设备环境中原来的画笔
			penMark.DeleteObject();//释放绘图资源
			//赋值
			pPieces[iPieces].bIsPieces = true;
			pPieces[iPieces].x = x;
			pPieces[iPieces].y = y;
			//计数加1
			iPieces++;
		}
	}
}

//清除棋子函数/用于悔棋
////////////////////////////////////////////////////problem:悔棋次数双方加起来一共只能悔一次 应为各方各能悔一次
bool CMainGobang::ClearPieces(CDC *pDC,bool &bBlack)
{
	CRect rect;
	if(pPieces[0].bIsPieces)
	{
		rect.bottom=pPieces[iPieces-1].y+10;
		rect.top=pPieces[iPieces-1].y-10;
		rect.left=pPieces[iPieces-1].x-10;
		rect.right=pPieces[iPieces-1].x+10;
		pDC->FillSolidRect(&rect,RGB(240,240,240));//填充
	
		CPen pen;
		pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
		CPen *OldPen=pDC->SelectObject(&pen);
		pDC->MoveTo(pPieces[iPieces-1].x,pPieces[iPieces-1].y-10);
		pDC->LineTo(pPieces[iPieces-1].x,pPieces[iPieces-1].y+10);
		pDC->MoveTo(pPieces[iPieces-1].x-10,pPieces[iPieces-1].y);
		pDC->LineTo(pPieces[iPieces-1].x+10,pPieces[iPieces-1].y);
		pDC->SelectObject(OldPen);//恢复设备环境中原来的画笔
		pen.DeleteObject();//释放绘图资源

		pPieces[iPieces-1].x=NULL;
		pPieces[iPieces-1].y=NULL;
		pPieces[iPieces-1].bIsPieces=false;
		if((iPieces-1)%2==0)
		{
			bBlack=false;
		}
		else
		{
			bBlack=true;
		}
		iPieces--;//计数减1
		return true;
	}
	else
	{
		return false;
	}
}

//循环绘制棋子 //用以解决刷新问题
void CMainGobang::KeepPieces(CDC *pDC,CRect rect,int iTableSize)
{
	ymin=(rect.bottom-rect.top)/2-(iTableSize-1)/2*idx;
	ymax=ymin+(iTableSize-1)*idx;
	xmin=(rect.right-rect.left)/2-(iTableSize-1)/2*idx;
	xmax=xmin+(iTableSize-1)*idx;
	int iTmp;
	for(int i=0;i<iPieces;i++)
	{
		if(!NoPieces(pPieces[i].x,pPieces[i].y))
		{
			if(i%2==0)//黑棋子
			{
				iTmp=0;
			}
			else//白棋子
			{
				iTmp=255;
			}
			//画棋子
			CBrush brush(RGB(iTmp,iTmp,iTmp));
			CBrush *OldBrush=pDC->SelectObject(&brush);
			pDC->Ellipse(pPieces[i].x-10,pPieces[i].y-10,pPieces[i].x+10,pPieces[i].y+10);
			pDC->SelectObject(OldBrush);//恢复设备环境中原来的画笔
			brush.DeleteObject();//释放绘图资源
			//画标记
			CPen penMark;//用于绘制标记的画笔
			penMark.CreatePen(PS_SOLID,1,RGB(255,0,0));
			CPen *OldPenMark=pDC->SelectObject(&penMark);
			pDC->MoveTo(pPieces[iPieces-1].x,pPieces[iPieces-1].y-3);
			pDC->LineTo(pPieces[iPieces-1].x,pPieces[iPieces-1].y+4);//弥补增量太小导致的不对称
			pDC->MoveTo(pPieces[iPieces-1].x-3,pPieces[iPieces-1].y);
			pDC->LineTo(pPieces[iPieces-1].x+4,pPieces[iPieces-1].y);
			pDC->SelectObject(OldPenMark);//恢复设备环境中原来的画笔
			penMark.DeleteObject();//释放绘图资源
		}
	}
}

//判断该位置上是否有选中框
bool CMainGobang::NoRectangle(int x,int y)
{
	bool bNoRectangle;
	bNoRectangle=true;
	for(int i=0;i<2;i++)
	{
		if((x==pmyRectangle[i].x)&&(y==pmyRectangle[i].y))
		{
			bNoRectangle=false;
		}
	}
	return bNoRectangle;
}

//判断该位置上是否有棋子
bool CMainGobang::NoPieces(int x,int y)
{
	for(int i=0;i<iPiecesNum;i++)
	{
		if((x==pPieces[i].x)&&(y==pPieces[i].y))//没有下棋时，pPieces的x,y皆为空值
		{
			if (pPieces[i].bIsPieces)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	//2020.03.04修复了没有返回值的情况
	return true;
}

//判断该位置上棋子颜色
bool CMainGobang::IsBlack(int x,int y)
{
	for(int i=0;i<iPiecesNum;i++)
	{
		if ((x == pPieces[i].x) && (y == pPieces[i].y))
		{
			if (!pPieces[i].bIsBlack)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	//2020.03.04修复了没有返回值的情况
	return true;
}

//查找相连的棋子
//up1,down2,left3,right4,up-left5,down-right6,up-right7,down-left8
bool CMainGobang::IsConnect(int iDir,int x,int y)
{
	bool bIsConnect=false;
	if(((iDir==1)&&(IsBlack(x,y)==IsBlack(x,y-idx))&&(!NoPieces(x,y-idx)))//向上查找
		||((iDir==2)&&(IsBlack(x,y)==IsBlack(x,y+idx))&&(!NoPieces(x,y+idx)))//向下查找
		||((iDir==3)&&(IsBlack(x,y)==IsBlack(x-idx,y))&&(!NoPieces(x-idx,y)))//向左查找
		||((iDir==4)&&(IsBlack(x,y)==IsBlack(x+idx,y))&&(!NoPieces(x+idx,y)))//向右查找
		||((iDir==5)&&(IsBlack(x,y)==IsBlack(x-idx,y-idx))&&(!NoPieces(x-idx,y-idx)))//向左上查找
		||((iDir==6)&&(IsBlack(x,y)==IsBlack(x+idx,y+idx))&&(!NoPieces(x+idx,y+idx)))//向右下查找
		||((iDir==7)&&(IsBlack(x,y)==IsBlack(x+idx,y-idx))&&(!NoPieces(x+idx,y-idx)))//向右上查找
		||((iDir==8)&&(IsBlack(x,y)==IsBlack(x-idx,y+idx))&&(!NoPieces(x-idx,y+idx))))//向左下查找
	{
		bIsConnect=true;
	}
	return bIsConnect;
}

//返回相连棋子个数
int CMainGobang::ConnectNum()
{
	int iConnect;//连接棋子数
	int iGetNum;//查找棋子数
	int icx,icy;//坐标位移控制量
	int iTmp;//中间变量
	//纵轴方向***************
	iGetNum=iConnect=1;
	icy=0;
	while(iGetNum<5)//向上查找5次
	{
		if(IsConnect(1,pPieces[iPieces-1].x,pPieces[iPieces-1].y+icy*idx))
		{
			iConnect++;
			icy--;
		}
		else
		{
			break;
		}
		iGetNum++;
	}
	iGetNum=1;
	icy=0;
	while(iGetNum<5)//再向下查找
	{
		if(IsConnect(2,pPieces[iPieces-1].x,pPieces[iPieces-1].y+icy*idx))
		{
			iConnect++;
			icy++;
		}
		else
		{
			break;
		}
		iGetNum++;
	}
	iTmp=iConnect;
	//横轴方向****************
	iGetNum=iConnect=1;
	icx=0;
	while(iGetNum<5)
	{
		if(IsConnect(3,pPieces[iPieces-1].x+icx*idx,pPieces[iPieces-1].y))
		{
			iConnect++;
			icx--;
		}
		else
		{
			break;
		}
		iGetNum++;
	}
	iGetNum=1;
	icx=0;
	while(iGetNum<5)
	{
		if(IsConnect(4,pPieces[iPieces-1].x+icx*idx,pPieces[iPieces-1].y))
		{
			iConnect++;
			icx++;
		}
		else
		{
			break;
		}
		iGetNum++;
	}
	iTmp=max(iTmp,iConnect);
	//左上-右下方向****************
	iGetNum=iConnect=1;
	icx=icy=0;
	while(iGetNum<5)
	{
		if(IsConnect(5,pPieces[iPieces-1].x+icx*idx,pPieces[iPieces-1].y+icy*idx))
		{
			iConnect++;
			icx--;
			icy--;
		}
		else
		{
			break;
		}
		iGetNum++;
	}
	iGetNum=1;
	icx=icy=0;
	while(iGetNum<5)
	{
		if(IsConnect(6,pPieces[iPieces-1].x+icx*idx,pPieces[iPieces-1].y+icy*idx))
		{
			iConnect++;
			icx++;
			icy++;
		}
		else
		{
			break;
		}
		iGetNum++;
	}
	iTmp=max(iTmp,iConnect);
	//右上-左下方向****************
	iGetNum=iConnect=1;
	icx=icy=0;
	while(iGetNum<5)
	{
		if(IsConnect(7,pPieces[iPieces-1].x+icx*idx,pPieces[iPieces-1].y+icy*idx))
		{
			iConnect++;
			icx++;
			icy--;
		}
		else
		{
			break;
		}
		iGetNum++;
	}
	iGetNum=1;
	icx=icy=0;
	while(iGetNum<5)
	{
		if(IsConnect(8,pPieces[iPieces-1].x+icx*idx,pPieces[iPieces-1].y+icy*idx))
		{
			iConnect++;
			icx--;
			icy++;
		}
		else
		{
			break;
		}
		iGetNum++;
	}
	iConnect=max(iTmp,iConnect);
	return iConnect;
}

////返回相连棋子方向
//int CMainGobang::ConnectDir()
//{
//	int iDir;
//	for(iDir=1;iDir<9;iDir++)
//	{
//		if(IsConnect(iDir,pPieces[iPieces-1].x,pPieces[iPieces-1].y))
//		{
//			return iDir;
//		}
//	}
//}

//机器下棋函数
//void CMainGobang::MachineDraw(CDC *pDC)
//{
//	int x,y;
//	///////////First Machine Pieces////////////////////////////////
//	if(iPieces==1)
//	{
//		//Always in top
//		x=pPieces[iPieces-1].x+idx;
//		y=pPieces[iPieces-1].y;
//		pDC->Ellipse(x-10,y-10,x+10,y+10);
//		//赋值
//		pPieces[iPieces].bIsPieces=true;
//		pPieces[iPieces].x=x;
//		pPieces[iPieces].y=y;
//		//计数加1
//		iPieces++;
//	}
//	////////////////////////////////////////////////
//	///////////Defence Part
//	///////////////////////////////////////////////
//	if(ConnectNum()==2)
//	{
//		if(ConnectDir()==1)
//		{
//			x=pPieces[iPieces-1].x;
//			y=abs(pPieces[iPieces-1].y+idx-pPieces[iPieces-2].y)<abs(pPieces[iPieces-1].y-2*idx-pPieces[iPieces-2].y)?
//				pPieces[iPieces-1].y+idx:pPieces[iPieces-1].y-2*idx;
//			pDC->Ellipse(x-10,y-10,x+10,y+10);
//			//赋值
//			pPieces[iPieces].bIsPieces=true;
//			pPieces[iPieces].x=x;
//			pPieces[iPieces].y=y;
//			//计数加1
//			iPieces++;
//		}
//		if(ConnectDir()==2)
//		{
//			x=pPieces[iPieces-1].x;
//			y=abs(pPieces[iPieces-1].y+2*idx-pPieces[iPieces-2].y)<abs(pPieces[iPieces-1].y-idx-pPieces[iPieces-2].y)?
//				pPieces[iPieces-1].y+2*idx:pPieces[iPieces-1].y-idx;
//			pDC->Ellipse(x-10,y-10,x+10,y+10);
//			//赋值
//			pPieces[iPieces].bIsPieces=true;
//			pPieces[iPieces].x=x;
//			pPieces[iPieces].y=y;
//			//计数加1
//			iPieces++;
//		}
//	}
//	///////////////////////////////////////
//	//进攻模块
//	///////////////////////////////////////
//
//}

//判断是否胜利
bool CMainGobang::Success()
{
	bool bSuccess;
	bSuccess=false;
	if(ConnectNum()>=5)
	{
		if(pPieces[iPieces-1].bIsBlack)
		{
			AfxMessageBox(_T("黑棋胜利!"));
		}
		if(!pPieces[iPieces-1].bIsBlack)
		{
			AfxMessageBox(_T("白棋胜利!"));
		}
		bSuccess=true;
	}
	return bSuccess;
}

//复盘标记函数
void CMainGobang::ReplayMark(CDC *pDC)
{
	CString strNum;//数字标记
	//设定字体
	CFont m_newFont;//用于输出坐标字符串的字体
	CFont *m_OldFont;//用于还原原来的字体
	m_newFont.CreatePointFont(80,_T("宋体"));//创建字体，8*n个像素高的宋体
	m_OldFont = (CFont*)pDC->SelectObject(&m_newFont);//将字体赋予设备上下文
	pDC->SetBkMode(TRANSPARENT);//设置输出字体的背景透明
	//输出坐标值到绘图控件
	for(int i=0;i<iPieces;i++)
	{
		strNum.Format(_T("%d"),i+1);
		if(i%2==0)
		{
			pDC->SetTextColor(RGB(255,255,255));//设置字体颜色
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,0));//设置字体颜色
		}
		pDC->TextOut(pPieces[i].x-5,pPieces[i].y-5,strNum);
	}
	pDC->SelectObject(m_OldFont);//恢复原字体
	if(pPieces!=NULL)
	{
		delete []pPieces;
		pPieces=NULL;
	}
}
