#include<cstring>
#include<iostream>
#include<random>
#include<windows.h>
#include<math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


using namespace std;
const float pi = 3.14159;

const int PPM=0, ScreenHeight =720, ScreenWidth=1280; //win 0 == true;



const string fl="mono.vlf";

//######################## transformation options #####################
const int TransformOption=2; //0=scale; 1=trans; 2=rot;
const int RotationAxis=1; //0=x; 1=y; 2=z;
const float ScaleX=0.5;
const float ScaleY=0.5;
const float ScaleZ=0.5;

const float TranslationX=2;
const float TranslationY=2;
const float TranslationZ=2;
const int Angle=0;

                  

float OperationMatrix[4][4];
typedef struct Coordinates{
    short x;
    short y;
    short z;
}Coord;

typedef struct DoubleCoordinates{
    double x;
    double y;
    double z;
}DoubleCoord;
Coord aux;
DoubleCoord DCoord;
vector<Coord> CoordAux;

vector<int> Vertexes;
vector<vector<int>> Faces;
vector<vector<int>> Edges;

HWND myconsole = GetConsoleWindow();
HDC mydc = GetDC(myconsole);

class FHDRaster {
  unsigned char data[ScreenWidth][ScreenHeight][3];
  public:
  FHDRaster(void) {
    memset(data, 0, ScreenWidth*ScreenHeight*3);  
  }
  
  auto DrawPixel(unsigned int x, unsigned int y, 
            unsigned char r, 
            unsigned char g, 
            unsigned char b) {
      if( x >= ScreenWidth || y >= ScreenHeight) {
        return;
      }
      data[x][y][0] = r;
      data[x][y][1] = g;
      data[x][y][2] = b;
  }
 
 auto write(void) {
   int x = 0;
   int y = 0;
   /*HWND myconsole = GetConsoleWindow();
   HDC mydc = GetDC(myconsole);*/
   COLORREF COLOR;
   int pixel =0;
   cout<<"P6\n1920 1080 255\n";
  
   for(y=0;y<ScreenHeight;y++) {
     for(x=0;x<ScreenWidth;x++) {
      if(PPM==0){
        COLOR = RGB(data[x][y][0],data[x][y][1],data[x][y][2]); 
        SetPixel(mydc,x,y,COLOR);
      }
      else{
        cout<<data[x][y][0];
        cout<<data[x][y][1];
        cout<<data[x][y][2];
      }
     } 
   }
  }



 /*
  Middle point algorithm
  We have x1, y1 and x2, y2.
  And the color
  */
  auto drawLine(unsigned int x, unsigned int y, unsigned int x2, unsigned int y2, unsigned int r, unsigned int g, unsigned int b) {
      int Dx = x2 - x;
      int Dy = y2 - y;

      int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
      // Check the orientation of the line
      if (Dx < 0) {
        dx1 = -1;
        dx2 = -1;

      } else if (Dx > 0) {
        dx1 = 1;
        dx2 = 1;
      }

      if (Dy < 0) dy1 = -1;

      else if (Dy > 0)
        dy1 = 1;

      // In witch axis moves more
      int Longest = abs(Dx);
      int Shortest = abs(Dy);

      if (!(Longest > Shortest)) {
        Longest = abs(Dy);
        Shortest = abs(Dx);

        if (Dy < 0) {
          dy2 = -1;
        }

        else if (Dy > 0) {
          dy2 = 1;
        }

        dx2 = 0;
      }

      int aux = Longest / 2;

      for (int i = 0; i <= Longest; i++) {
        DrawPixel(x, y, r, g, b);
        aux += Shortest;

        if (!(aux < Longest)) {
          aux -= Longest;
          x += dx1;
          y += dy1;
        } else {
          x += dx2;
          y += dy2;
        }
      }
  }

auto Transformations (float OperationMatrix[4][4], const float TransformOption){


    float Theta= Angle*pi/180;
    if(TransformOption==0){
      OperationMatrix[0][0]=ScaleX;
      OperationMatrix[0][1]=0;
      OperationMatrix[0][2]=0;
      OperationMatrix[0][3]=0;

      OperationMatrix[1][0]=0;
      OperationMatrix[1][1]=ScaleY;
      OperationMatrix[1][2]=0;
      OperationMatrix[1][3]=0;

      OperationMatrix[2][0]=0;
      OperationMatrix[2][1]=0;
      OperationMatrix[2][2]=ScaleZ;
      OperationMatrix[2][3]=0;

      OperationMatrix[3][0]=0;
      OperationMatrix[3][1]=0;
      OperationMatrix[3][2]=0;
      OperationMatrix[3][3]=1;
    }
    else if(TransformOption==1){
      OperationMatrix[0][0]=TranslationX;
      OperationMatrix[0][1]=0;
      OperationMatrix[0][2]=0;
      OperationMatrix[0][3]=0;

      OperationMatrix[1][0]=0;
      OperationMatrix[1][1]=TranslationY;
      OperationMatrix[1][2]=0;
      OperationMatrix[1][3]=0;

      OperationMatrix[2][0]=0;
      OperationMatrix[2][1]=0;
      OperationMatrix[2][2]=TranslationZ;
      OperationMatrix[2][3]=0;

      OperationMatrix[3][0]=0;
      OperationMatrix[3][1]=0;
      OperationMatrix[3][2]=0;
      OperationMatrix[3][3]=1;
    }
    else{
      if(RotationAxis==0){
        OperationMatrix[0][0]=1;
        OperationMatrix[0][1]=0;
        OperationMatrix[0][2]=0;
        OperationMatrix[0][3]=0;

        OperationMatrix[1][0]=0;
        OperationMatrix[1][1]=cos(Theta);
        OperationMatrix[1][2]=sin(Theta)*-1;
        OperationMatrix[1][3]=0;

        OperationMatrix[2][0]=0;
        OperationMatrix[2][1]=sin(Theta);
        OperationMatrix[2][2]=cos(Theta);
        OperationMatrix[2][3]=0;

        OperationMatrix[3][0]=0;
        OperationMatrix[3][1]=0;
        OperationMatrix[3][2]=0;
        OperationMatrix[3][3]=1;
      }
      else if(RotationAxis==1){
        OperationMatrix[0][0]=cos(Theta);
        OperationMatrix[0][1]=0;
        OperationMatrix[0][2]=-sin(Theta);
        OperationMatrix[0][3]=0;

        OperationMatrix[1][0]=0;
        OperationMatrix[1][1]=1;
        OperationMatrix[1][2]=0;
        OperationMatrix[1][3]=0;

        OperationMatrix[2][0]=sin(Theta);
        OperationMatrix[2][1]=0;
        OperationMatrix[2][2]=cos(Theta);
        OperationMatrix[2][3]=0;

        OperationMatrix[3][0]=0;
        OperationMatrix[3][1]=0;
        OperationMatrix[3][2]=0;
        OperationMatrix[3][3]=1;
      }
      else{
        OperationMatrix[0][0]=cos(Theta);
        OperationMatrix[0][1]=-sin(Theta);
        OperationMatrix[0][2]=0;
        OperationMatrix[0][3]=0;

        OperationMatrix[1][0]=sin(Theta);
        OperationMatrix[1][1]=cos(Theta);
        OperationMatrix[1][2]=0;
        OperationMatrix[1][3]=0;

        OperationMatrix[2][0]=0;
        OperationMatrix[2][1]=0;
        OperationMatrix[2][2]=1;
        OperationMatrix[2][3]=0;

        OperationMatrix[3][0]=0;
        OperationMatrix[3][1]=0;
        OperationMatrix[3][2]=0;
        OperationMatrix[3][3]=1;
      }
    }
    float Points[4][1] = {{(float)DCoord.x},{(float)DCoord.y},{(float)DCoord.z},{1}};
    float Scaled[4][1] = {{0},{0},{0},{0}};

    for (int row = 0; row < 4; row++) {
      for (int col = 0; col < 1; col++) {
          for (int inner = 0; inner < 4; inner++) {
                            
               Scaled[row][col] += OperationMatrix[row][inner] * Points[inner][col];
          }
                        
      }
                    
    }
    DCoord.x = Scaled[0][0]/Scaled[3][0];
    DCoord.y = Scaled[1][0]/Scaled[3][0];
    DCoord.z = Scaled[2][0]/Scaled[3][0];

    aux.x=(int)(DCoord.x*1000);
    aux.y=(int)(DCoord.y*1000);
    aux.z=(int)(DCoord.z*1000);

}

auto Draw( auto x1, auto x2, auto y1, auto y2, auto z1, auto z2){

  drawLine(0,ScreenHeight/2,ScreenWidth,ScreenHeight/2,255,255,255);
 drawLine(ScreenWidth/2,0,ScreenWidth/2,ScreenHeight,255,255,255);
 
  int n=11;
  int HalfWidth=ScreenWidth/4, HalfHeight=ScreenHeight/4;

  int XPixels=HalfWidth/n;

  int AdjustmentY=0;
  int EdgesSize=Edges.size();
  //XY Proyection
  for(int i=0; i<EdgesSize; i++){ 
    Vertexes=Edges[i];
    x1 = (int) (CoordAux[Vertexes[0]-1].x*XPixels/1000+HalfWidth);
    y1 = (int) ((HalfHeight-AdjustmentY)-(CoordAux[Vertexes[0]-1].y*XPixels)/1000);
    x2 = (int) ((CoordAux[Vertexes[1]-1].x*XPixels)/1000+HalfWidth);
    y2 = (int) ((HalfHeight-AdjustmentY)-(CoordAux[Vertexes[1]-1].y*XPixels/1000)); 

    drawLine(x1, y1, x2, y2, 218, 165, 32);  
  }
  //ZY Proyection
  for(int i=0; i<EdgesSize; i++){ 
    Vertexes=Edges[i];
    z1 = (int) (CoordAux[Vertexes[0]-1].z*XPixels/1000+(HalfWidth*3));
    y1 = (int) ((HalfHeight-AdjustmentY)-(CoordAux[Vertexes[0]-1].y*XPixels)/1000);
    z2 = (int) ((CoordAux[Vertexes[1]-1].z*XPixels)/1000+(HalfWidth*3));
    y2 = (int) ((HalfHeight-AdjustmentY)-(CoordAux[Vertexes[1]-1].y*XPixels/1000)); 

    drawLine(z1, y1, z2, y2, 218, 165, 32);  
  }
  //XZ Proyection
  for(int i=0; i<EdgesSize; i++){ 
    Vertexes=Edges[i];
    x1 = (int) (CoordAux[Vertexes[0]-1].x*XPixels/1000+HalfWidth);
    z1 = (int) ((HalfHeight*3)-(CoordAux[Vertexes[0]-1].z*XPixels)/1000);
    x2 = (int) ((CoordAux[Vertexes[1]-1].x*XPixels)/1000+HalfWidth);
    z2 = (int) ((HalfHeight*3)-(CoordAux[Vertexes[1]-1].z*XPixels/1000)); 

    drawLine(x1, z1, x2, z2, 218, 165, 32);  
  }
 //YZ Proyection
  for(int i=0; i<EdgesSize; i++){ 
    Vertexes=Edges[i];
    y1 = (int) (CoordAux[Vertexes[0]-1].y*XPixels/1000+(HalfWidth*3));
    z1 = (int) ((HalfHeight*3)-(CoordAux[Vertexes[0]-1].z*XPixels)/1000);
    y2 = (int) ((CoordAux[Vertexes[1]-1].y*XPixels)/1000+(HalfWidth*3));
    z2 = (int) ((HalfHeight*3)-(CoordAux[Vertexes[1]-1].z*XPixels/1000)); 

    drawLine(y1, z1, y2, z2, 218, 165, 32);  
  }
  
/***************************drawing*******************************/

}

auto ReadVLF(){

   FILE *FileInput;
    
    int i, j,  verticeAux;

    const char *a=fl.c_str();
    FileInput=fopen(a,"rb");
    int NumVertexes,NumFaces,NumEdges;

    fread(&NumVertexes,sizeof(int),1,FileInput);
    fread(&NumEdges,sizeof(int),1,FileInput);
    fread(&NumFaces,sizeof(int),1,FileInput);

    for(i=0; i<NumVertexes; i++){
        fread(&DCoord,sizeof(DoubleCoord),1,FileInput);
         
        Transformations(OperationMatrix, TransformOption);

        CoordAux.push_back(aux);
    }   
   
    for(i=0;i<NumEdges;i++){
        for(j=0;j<2;j++){
            fread(&verticeAux,sizeof(int),1,FileInput);
            Vertexes.push_back(verticeAux);
        }
        Edges.push_back(Vertexes);
        Vertexes.clear();
       
    }
    for(i=0;i<NumFaces;i++){
        for(j=0;j<3;j++){
            fread(&verticeAux,sizeof(int),1,FileInput);
            Vertexes.push_back(verticeAux);
        }
        Faces.push_back(Vertexes);
        Vertexes.clear();
    }
    fclose(FileInput);
    
}

};
int main(int argc, char * argv []) {
 
 FHDRaster * raster = new FHDRaster();
 int x1,x2,y1,y2,z1,z2;


raster-> ReadVLF();
    
raster-> Draw(x1,x2,y1,y2,z1,z2);


 raster->write();
 
 if(PPM==0){
  ReleaseDC(myconsole, mydc);
  getchar();
 }
 return 0;
}