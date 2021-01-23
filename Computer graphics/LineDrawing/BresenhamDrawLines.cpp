#include<cstring>
#include<iostream>
#include<random>
#include<windows.h>
#include<math.h>
using namespace std;
/*

Program that makes a random lines by drawing lines with bresenham algorithm.


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

  void DrawPixel(unsigned int x, unsigned int y, 
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
///////////////////////////////////////////////////////////////

/*
Middle point algorithm 
We have x1, y1 and x2, y2.
And the color
*/
void drawLine(unsigned int x,unsigned int y,unsigned int x2, unsigned int y2,unsigned int r, unsigned int g, unsigned int b) {
    int Dx = x2 - x ;
    int Dy = y2 - y ;

    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;
    //Check the orientation of the line
    if (Dx < 0){
      dx1 = -1 ; 
      dx2 = -1 ; 

    }
    else if (Dx >0){
      dx1 = 1 ;
      dx2 = 1 ;
    }


    if (Dy<0) dy1 = -1 ; 

    else if (Dy>0) dy1 = 1 ;

    //In witch axis moves more
    int Longest = abs(Dx) ;
    int Shortest = abs(Dy) ;

    if (!(Longest>Shortest)) {
        Longest = abs(Dy) ;
        Shortest = abs(Dx) ;

        if (Dy < 0) {
          dy2 = -1 ;
           }

        else if  (Dy>0) {
         dy2 = 1 ;
        }
        
        dx2 = 0 ;            
    }

   
    int aux = Longest/2; 

    for (int i=0;i<=Longest;i++) {
        DrawPixel(x,y,r,g,b) ;
        aux += Shortest ;
        
        if (!(aux<Longest)) {
            aux -= Longest ;
            x += dx1 ;
            y += dy1 ;
        } 
        else {
            x += dx2 ;
            y += dy2 ;
        }
    }
}


/*Draw  lines for testing*/
void Random(){
  int x=960;
  int y=540;
  int d=500;
  int x1,y1;
  for (float i = 0; i < 100; i+=0.5)
  {
   
    drawLine(rand() % 1000,rand() % 1000,rand() % 1000,rand() % 1000,(int)i%255,(int)i%255,(int)i%255);

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