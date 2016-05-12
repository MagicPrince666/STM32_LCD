/*******************************************************************************

GUI显示函数

*******************************************************************************/

#include "stm32f10x.h"
#include "ili9320.h"
#include "HzLib_65k.h"  //字库

/****************************************************************************
* 名    称：GUI_CmpColor()
* 功    能：判断颜色值是否一致。
* 入口参数：color1		颜色值1
*		        color2		颜色值2
* 出口参数：返回1表示相同，返回0表示不相同。
* 说    明：
****************************************************************************/
#define  GUI_CmpColor(color1, color2)	( (color1&0x01) == (color2&0x01) )
/****************************************************************************
* 名    称：u16 GUI_Color565(u32 RGB)
* 功    能：将RGB颜色转换为16位颜色。
* 入口参数：RGB  颜色值
* 出口参数：返回16位RGB颜色值。
* 说    明：
* 调用方法：i=GUI_Color565(0xafafaf);
****************************************************************************/
u16 GUI_Color565(u32 RGB)
{
  u8  r, g, b;

  b = ( RGB >> (0+3) ) & 0x1f;		// 取B色的高5位
  g = ( RGB >> (8+2) ) & 0x3f;		// 取G色的高6位
  r = ( RGB >> (16+3)) & 0x1f;		// 取R色的高5位

  return( (r<<11) + (g<<5) + (b<<0) );
}

/****************************************************************************
* 名    称：void GUI_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor)
* 功    能：在指定座标显示字符串
* 入口参数：x      行座标
*           y      列座标
*           *str   字符串
*           len    字符串长度
*           Color  字符颜色
*           bkColor字符背景颜色
* 出口参数：无
* 说    明：
* 调用方法：GUI_Text(0,0,"0123456789",10,0x0000,0xffff);
****************************************************************************/
void GUI_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor)
{
  u8 i;

  for (i=0;i<len;i++)
  {
    ili9320_PutChar((x+8*i),y,*str++,Color,bkColor);
  }
}

/****************************************************************************
* 名    称：void GUI_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor)
* 功    能：在指定座标显示汉字文本
* 入口参数：x      行座标
*           y      列座标
*           *str   汉字串
*           len    字串长度
*           Color  字符颜色
*           bkColor字符背景颜色
* 出口参数：无
* 说    明：一个汉字两个字符，记得哟，5个汉字len就是10
* 调用方法：GUI_Text(0,0,"成都贝一特",10,0x0000,0xffff);
****************************************************************************/
void GUI_Chinese_Text(u16 x,u16 y, u8 str[],u8 len,u16 charColor,u16 bkColor)
{
    u16 i=0,b;
    u16 j=0;
	u16 x_add,y_add;
    u16 tmp_char=0,index=0;
	x_add=x;
	y_add=y;

	for(b=0;b<len/2;b++)
	{
			index=(94*(str[b*2] - 0xa1)+(str[b*2+1] - 0xa1));	    //计算区位
			for (i=0;i<16;i++)
		    {
			    for (j=0;j<8;j++)
			    {
				    tmp_char=HzLib[index*32+i*2];					//查字库
			        if ( (tmp_char << j) & 0x80)
			        {
			          ili9320_SetPoint(x_add+j,y_add+i,charColor);  // 字符颜色
			        }
			        else
			        {
			          ili9320_SetPoint(x_add+j,y_add+i,bkColor);   // 背景颜色
			        }
			    }
			   	for (j=0;j<8;j++)
			    {
				    tmp_char=HzLib[index*32+i*2+1];				    //查字库
			        if ( (tmp_char << j) & 0x80)
			        {
			          ili9320_SetPoint(x_add+j+8,y_add+i,charColor); // 字符颜色
			        }
			        else
			        {
			          ili9320_SetPoint(x_add+j+8,y_add+i,bkColor);   // 背景颜色
			        }
			    }
		     }
			 x_add=x_add+17;										 //地址累加
	}
}
/*******************************************************************************
函数名     ： DispWord
功能       ： 显示9位以下数字，最大显示999999999
参数       ： x     : 坐标X
              y     : 坐标Y
			  maxFb : 要显示的位数，注：小于要显示的位数前面补0。
			  Da    : 要显示的数据
			  PosFb : 反现位置  从零到maxFb
			  Point : 小数点的位数
返回值     ： 无
*******************************************************************************///
void GUI_Word(u16  x, u16  y,u16  maxXS, u32  Da,  u16 Point, u16 Color, u16 bkColor)
{
 	u8  i;//,Fb;

	u32 NUM;
	NUM=1;
 	for(i=0; i<(maxXS-1); i++)  NUM=NUM*10;//留做要显示的位数取位用
	if(Point!=0)ili9320_PutChar((x+(maxXS -Point)*8),y,0x2e,Color,bkColor);//如果小数点位数非零显示小数点

	for(i=0; i<(maxXS-0); i++)  //从最高位依次显示要显示的位数
	{
		if(i<=(maxXS-Point-1))  //如果该位是小数点所在的位以前的位（包括小数点所在的位）则正常显示
		{
		   ili9320_PutChar((x+i*8),y,((Da/NUM)%10+0x30),Color,bkColor);
		   NUM=NUM/10;

		}
		else                    //否则显示数字移位一位数字用来显示小数点
		{
		   ili9320_PutChar((x+i*8+8),y,((Da/NUM)%10+0x30),Color,bkColor);
		   NUM=NUM/10;
		}
	}
	return;
}


/****************************************************************************
* 名    称：void GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color)
* 功    能：在指定座标画直线
* 入口参数：x0     A点行座标
*           y0     A点列座标
*           x1     B点行座标
*           y1     B点列座标
*           color  线颜色
* 出口参数：无
* 说    明：
* 调用方法：GUI_Line(0,0,240,320,0x0000);
****************************************************************************/
void GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color)
{
 	u16 x,y;
 	u16 dx;// = abs(x1 - x0);
 	u16 dy;// = abs(y1 - y0);

	if(y0==y1)
	{
		if(x0<=x1)
		{
			x=x0;
		}
		else
		{
			x=x1;
			x1=x0;
		}
  		while(x <= x1)
  		{
   			ili9320_SetPoint(x,y0,color);
   			x++;
  		}
  		return;
	}
	else if(y0>y1)
	{
		dy=y0-y1;
	}
	else
	{
		dy=y1-y0;
	}

 	if(x0==x1)
	{
		if(y0<=y1)
		{
			y=y0;
		}
		else
		{
			y=y1;
			y1=y0;
		}
  		while(y <= y1)
  		{
   			ili9320_SetPoint(x0,y,color);
   			y++;
  		}
  		return;
	}
	else if(x0 > x1)
 	{
		dx=x0-x1;
  		x = x1;
  		x1 = x0;
  		y = y1;
  		y1 = y0;
 	}
 	else
 	{
		dx=x1-x0;
  		x = x0;
  		y = y0;
 	}

 	if(dx == dy)
 	{
  		while(x <= x1)
  		{

   			x++;
			if(y>y1)
			{
				y--;
			}
			else
			{
   				y++;
			}
   			ili9320_SetPoint(x,y,color);
  		}
 	}
 	else
 	{
 		ili9320_SetPoint(x, y, color);
  		if(y < y1)
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
    			s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
     				{
      					p += twoDy;
     				}
     				else
     				{
      					y++;
      					p += twoDyMinusDx;
     				}
     				ili9320_SetPoint(x, y,color);
    			}
   			}
   			else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
    			s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y < y1)
    			{
     				y++;
     				if(p < 0)
     				{
      					p += twoDx;
     				}
     				else
     				{
      					x++;
      					p+= twoDxMinusDy;
     				}
     				ili9320_SetPoint(x, y, color);
    			}
   			}
  		}
  		else
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
	    		s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
	     			{
    	  				p += twoDy;
     				}
     				else
     				{
      					y--;
	      				p += twoDyMinusDx;
    	 			}
     				ili9320_SetPoint(x, y,color);
    			}
   			}
	   		else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
	    		s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y1 < y)
    			{
     				y--;
     				if(p < 0)
	     			{
    	  				p += twoDx;
     				}
     				else
     				{
      					x++;
	      				p+= twoDxMinusDy;
    	 			}
     				ili9320_SetPoint(x, y,color);
    			}
   			}
  		}
 	}
}

/****************************************************************************
* 名    称：void GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
* 功    能：在指定座标画圆，可填充
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
****************************************************************************/
void GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
{
	u16 x,y;
	s16 delta,tmp;
	x=0;
	y=r;
	delta=3-(r<<1);

	while(y>x)
	{
		if(fill)
		{
			GUI_Line(cx+x,cy+y,cx-x,cy+y,color);
			GUI_Line(cx+x,cy-y,cx-x,cy-y,color);
			GUI_Line(cx+y,cy+x,cx-y,cy+x,color);
			GUI_Line(cx+y,cy-x,cx-y,cy-x,color);
		}
		else
		{
			ili9320_SetPoint(cx+x,cy+y,color);
			ili9320_SetPoint(cx-x,cy+y,color);
			ili9320_SetPoint(cx+x,cy-y,color);
			ili9320_SetPoint(cx-x,cy-y,color);
			ili9320_SetPoint(cx+y,cy+x,color);
			ili9320_SetPoint(cx-y,cy+x,color);
			ili9320_SetPoint(cx+y,cy-x,color);
			ili9320_SetPoint(cx-y,cy-x,color);
		}
		x++;
		if(delta>=0)
		{
			y--;
			tmp=(x<<2);
			tmp-=(y<<2);
			delta+=(tmp+10);
		}
		else
		{
			delta+=((x<<2)+6);
		}
	}
}

/****************************************************************************
* 名    称：void GUI_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill)
* 功    能：在指定区域画矩形，可填充颜色
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
****************************************************************************/
void GUI_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill)
{
	if(fill)
	{
		u16 i;
		if(x0>x1)
		{
			i=x1;
			x1=x0;
		}
		else
		{
			i=x0;
		}
		for(;i<=x1;i++)
		{
			GUI_Line(i,y0,i,y1,color);
		}
		return;
	}
	GUI_Line(x0,y0,x0,y1,color);
	GUI_Line(x0,y1,x1,y1,color);
	GUI_Line(x1,y1,x1,y0,color);
	GUI_Line(x1,y0,x0,y0,color);
}

/****************************************************************************
* 名    称：void  GUI_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill)
* 功    能：在指定区域画正方形，可填充颜色
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
****************************************************************************/
void  GUI_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill)
{
	GUI_Rectangle(x0, y0, x0+with, y0+with, color,fill);
}
