#include<cstring>
#include<iostream>
#include<random>
#include<windows.h>
#include<math.h>

using namespace std;
/*

Program that makes a random lines by drawing lines with dda algorithm


*/

HWND myconsole = GetConsoleWindow();
HDC mydc = GetDC(myconsole);
// Things to draw a pixel into the cmd
class FHDRaster {
  unsigned char data[1920][1080][3];
  public:
  FHDRaster(void) {
    memset(data, 0, 1920*1080*3);  
  }
  void
  DrawPixel(unsigned int x, unsigned int y, 
            unsigned char r, 
            unsigned char g, 
            unsigned char b) {
      if( x >= 1920 || y >= 1080) {
        return;
      }
      data[x][y][0] = r;
      data[x][y][1] = g;
      data[x][y][2] = b;
  }
  void
  write(void) {
   int x = 0;
   int y = 0;
   COLORREF COLOR;
   int pixel =0;

   for(y=0;y<1080;y++) {
     for(x=0;x<1920;x++) {
      COLOR = RGB(data[x][y][0],data[x][y][1],data[x][y][2]); 
      SetPixel(mydc,x,y,COLOR);
      
      
     } 
   }
  }

//Obtiene el signo del entero recibido
sgn(int x){
	if (x >= 0) return 1;
if (x < 0) return -1;

return 0;
}
///////////////////////////////////////////////////////////////


//Using the Digital Differential Analyzer algorithm
// We have x1, y1 and x2, y2.
//And the color
/*
First we obtain the steps and the the additions of x and y.
Obtains the initial point and draw the pixel and add
*/
 void drawLine( int x1, int y1, int x2, int y2, int r, int g, int b)
{
    float d, x, y;

    float dx = (x2-x1);
    float dy = (y2-y1);

    if ( abs(dx) > abs(dy) )		
       d = abs(dx);
    else
       d = abs(dy);
    float AddX = dx / d;				
    float AddY = dy / d;

    x = x1+sgn(x1)*0.5;				
    y = y1+sgn(y1)*0.5;

    for (int i = 0; i <= d; i++)

    {	DrawPixel(floor(x),floor(y),r,g,b);
        x = x + AddX;
        y = y + AddY;
    }
}

void Random (){

  for (float i = 0; i < 100; i+=0.5)
  {
    /* code */
  
    drawLine(rand()%1000,rand()%1000,rand()%1000,rand()%1000,(int)i%255,(int)i%255,(int)i%255);

  }
}
};
int main(void) {
 FHDRaster * raster = new FHDRaster();


 raster->Random();


 raster->write();
 
 ReleaseDC(myconsole, mydc);
 getchar();
 return 0;
}