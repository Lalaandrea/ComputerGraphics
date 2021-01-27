#include<cstring>
#include<iostream>
#include<random>
#include<windows.h>
#include<math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#define Inf -9999999999999999999999999999.99


using namespace std;
const float pi = 3.14159;

const int PPM=0, ScreenHeight =720, ScreenWidth=1280; //win 0 == true;



const string fl="Omono.vlf";

//######################## transformation options #####################
  const int TransformOption=2; //0=scale; 1=trans; 2=rot;
  const int RotationAxis=1; //0=x; 1=y; 2=z;
  const float ScaleY=0.5;
  const float ScaleZ=0.5;
  const float ScaleX=0.5;

  const float TranslationX=2;
  const float TranslationY=2;
  const float TranslationZ=200;
  const int Angle=0;
  float OperationMatrix[4][4];

//###################################################################

                  

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


typedef struct norm{
  short x;
  short y;
  short z;
  int   face; //indice de la cara
  double   dotx;
  double   doty;
  double   dotz;
}StructNormal;

Coord aux;
DoubleCoord DCoord;
vector<Coord> CoordAux;

vector<int> Vertexes;
vector<vector<int>> Faces;
vector<vector<int>> Edges;

vector<vector<int>> VisibleFaces;
vector<vector<int>> NotVisibleFaces;

vector<StructNormal> NormalsOfFaces;
  vector<StructNormal> NVFNormals, VFNormals; //Not visible faces Normal faces// Visible Faces Normal faces

int FacesSize;

HWND myconsole = GetConsoleWindow();
HDC mydc = GetDC(myconsole);
/* ##############################For ZBuffer */
array<array<float, ScreenHeight>, ScreenWidth> MatrixZBuffer = {}; //w,h,double
typedef struct ZBufferDepthSurface{
    float A;
    float B;
    float C;
    float D;
}ZBufferCoefficent;
/* ##############################For ZBuffer */



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

  /*ScanLine*/
  auto ScanLine(int x1, int y1, int x2, int y2, int x3, int y3){
            double i,j,k,dy,dx;
            double x,y, temp;
            vector<double> PointsX={0,0};
            vector<double> slope;
            vector<vector<double>> Coord;
            vector<double> Aux;

                Aux.push_back(x1);
                Aux.push_back(y1);
                Coord.push_back(Aux);
                Aux.clear();
                
                Aux.push_back(x2);
                Aux.push_back(y2);
                Coord.push_back(Aux);
                Aux.clear();
                
                Aux.push_back(x3);
                Aux.push_back(y3);
                Coord.push_back(Aux);
                Aux.clear();

                Aux.push_back(x1);
                Aux.push_back(y1);
                Coord.push_back(Aux);
                Aux.clear();

            for(i=0;i<3;i++){ 
                dy=Coord[i+1][1]-Coord[i][1];//distancia x
                dx=Coord[i+1][0]-Coord[i][0];// distancia y
                
                if(dy==0) slope.push_back(1.0);

                if(dx==0) slope.push_back(0.0);
                 /*- calculate inverse slope -*/
                if((dy!=0)&&(dx!=0)) slope.push_back(dx/dy);
            }
            
            for(y=0;y< 1000;y++){
              PointsX.clear();
              slope.clear();
              k=0;
              for(i=0;i<3;i++){
                
                  if( ((Coord[i][1]<=y)&&(Coord[i+1][1]>y))|| ((Coord[i][1]>y)&&(Coord[i+1][1]<=y))){
                      PointsX.push_back(Coord[i][0]+slope[i]*(y-Coord[i][1]));
                      k++;
                  }   
              }
              
              for(j=0;j<k-1;j++) {/*- Arrange x-intersections in order -*/
                for(i=0;i<k-1;i++){
                  if(PointsX[i] > PointsX[i+1]){
                    temp= PointsX[i];
                    PointsX[i]=PointsX[i+1];
                    PointsX[i+1]=temp;
                  }
                }
              }
                for(i=0;i<k;i+=2){
                drawLine(PointsX[i], y, PointsX[i+1]+1 ,y,rand()%255,rand()%255,rand()%255); 
              }
            }

            for(i=0; i < 3;i++){
              drawLine(Coord[i][0],Coord[i][1],Coord[i+1][0],Coord[i+1][1],255,0,0);    
            }
  }
/*ScanLine*/
/* testing */


/*ZBuffer*/
ZBufferCoefficent CoefficentZBuffer(int x1, int y1, int z1 ,int x2,int y2,  int z2, int x3, int y3, int z3)
{

    ZBufferCoefficent Coefficent;
    //Point.A = (pt2.z - pt3.z) * (pt1.y - pt2.y) - (pt1.z - pt2.z) * (pt2.y - pt3.y);
    Coefficent.A = (z2 - z3) * (y1 - y2) - (z1 - z2) * (y2 - y3);
    Coefficent.B = (x2 - x3) * (z1 - z2) - (x1 - x2) * (z2 - z3);
    Coefficent.C = (y2 - y3) * (x1 - x2) - (y1 - y2) * (x2 - x3);
    Coefficent.D = - x1 * (y2 * z3 - z2 * y3) + y1 * (x2 * z3 - z2 * x3) - z1 * (x2 * y3 - y2 * x3);
    return Coefficent;

}

auto ZBufferValue(ZBufferCoefficent Coefficent,int x,int y)
{
    double z = 0;
    if (Coefficent.B != 0)
        z = (-Coefficent.A * x - Coefficent.B * y - Coefficent.D) / Coefficent.C;
    return z;
}
auto drawLineZBuffer(ZBufferCoefficent Coefficent, unsigned int x,unsigned int y,unsigned int x2, unsigned int y2,unsigned int r, unsigned int g, unsigned int b) {
    int DeltaX = x2 - x ;
    int DeltaY = y2 - y ;

    int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0 ;
    float z, cosarara;
    bool temp;

    if (DeltaX<0){
      dx1 = -1 ; 
      dx2 = -1 ; 

    }
    else if (DeltaX>0){
      dx1 = 1 ;
      dx2 = 1 ;
    }

    if (DeltaY<0) dy1 = -1 ; 
    else if (DeltaY>0) dy1 = 1 ;

    int longest = abs(DeltaX) ;
    int shortest = abs(DeltaY) ;
    if (!(longest>shortest)) {
        longest = abs(DeltaY) ;
        shortest = abs(DeltaX) ;

        if (DeltaY<0) dy2 = -1 ; 

        else if (DeltaY>0) dy2 = 1 ;
        
        dx2 = 0 ;            
    }

    int numerator = longest >> 1 ; 
    for (int i=0;i<=longest;i++) {
       temp=0;

      /*ZBuffer*/
      z= ZBufferValue(Coefficent, x, y);

    
        if (MatrixZBuffer[x][y]<=z)
        { 
          MatrixZBuffer[x][y]=z;
          temp=1;
        }
        else{
          temp = 0;
        }
      
      
      if (temp)
      {
        DrawPixel(x,y,r,g,b) ;
        numerator += shortest ;
      }
      // ZBuffer**********************************
        if (!(numerator<longest)) {
            numerator -= longest ;
            x += dx1 ;
            y += dy1 ;
        } 
        else {
            x += dx2 ;
            y += dy2 ;
        }
    }

}
auto ScanLineZBuffer(ZBufferCoefficent Coefficent, int x1, int y1, int x2, int y2, int x3, int y3,int r,int g,int b){
  
  double i,j,k,dy,dx;
  double x,y, temp;
  vector<double> PointsX={0,0};
  vector<double> slope;
  vector<vector<double>> Coord;
  vector<double> Aux;
  /*Set Coordinates*/
      Aux.push_back(x1);
      Aux.push_back(y1);
      Coord.push_back(Aux);
      Aux.clear();
      
      Aux.push_back(x2);
      Aux.push_back(y2);
      Coord.push_back(Aux);
      Aux.clear();
      
      Aux.push_back(x3);
      Aux.push_back(y3);
      Coord.push_back(Aux);
      Aux.clear();

      Aux.push_back(x1);
      Aux.push_back(y1);
      Coord.push_back(Aux);
      Aux.clear();
  /*Set Coordinates*/
  for(i=0;i<3;i++){ 
      dy=Coord[i+1][1]-Coord[i][1];//distancia x
      dx=Coord[i+1][0]-Coord[i][0];// distancia y
      
      if(dy==0) slope.push_back(1.0);

      if(dx==0) slope.push_back(0.0);
       /*- calculate inverse slope -*/
      if((dy!=0)&&(dx!=0)) slope.push_back(dx/dy);
  }
  
  for(y=0;y< 1000;y++){
    PointsX.clear();
    slope.clear();
    k=0;
    for(i=0;i<3;i++){
      
        if( ((Coord[i][1]<=y)&&(Coord[i+1][1]>y))|| ((Coord[i][1]>y)&&(Coord[i+1][1]<=y))){
            PointsX.push_back(Coord[i][0]+slope[i]*(y-Coord[i][1]));
            k++;
        }   
    }
    
    for(j=0;j<k-1;j++) {/*- Arrange x-intersections in order -*/
      for(i=0;i<k-1;i++){
        if(PointsX[i] > PointsX[i+1]){
          temp= PointsX[i];
          PointsX[i]=PointsX[i+1];
          PointsX[i+1]=temp;
        }
      }
    }
    
      for(i=0;i<k;i+=2){
      drawLineZBuffer(Coefficent ,PointsX[i], y, PointsX[i+1] ,y,r,g,b);//+1 ?
    }
  }
  for(i=0; i < 3;i++){
   // drawLineZBuffer(Coefficent ,Coord[i][0],Coord[i][1],Coord[i+1][0],Coord[i+1][1],255,255,255);    
  }
}


auto ZBufferFunction(int x1, int y1, int z1 ,int x2,int y2,  int z2, int x3, int y3, int z3,int r, int g, int b){
  ZBufferCoefficent FaceCoefficents;
  FaceCoefficents= CoefficentZBuffer(x1,y1,z1,x2,y2,z2,x3,y3,z3);
  ScanLineZBuffer(FaceCoefficents, x1,y1,x2,y2,x3,y3,r,g,b);
}
/*ZBuffer*/  

auto Transformations (float OperationMatrix[4][4], const int TransformOption){


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
      OperationMatrix[0][0]=1;
      OperationMatrix[0][1]=0;
      OperationMatrix[0][2]=0;
      OperationMatrix[0][3]=TranslationX;

      OperationMatrix[1][0]=0;
      OperationMatrix[1][1]=1;
      OperationMatrix[1][2]=0;
      OperationMatrix[1][3]=TranslationY;

      OperationMatrix[2][0]=0;
      OperationMatrix[2][1]=0;
      OperationMatrix[2][2]=1;
      OperationMatrix[2][3]=TranslationZ;

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

auto Normals(){
  //****************************Posible normal********************
   int x1,x2,y1,y2,z1,z2;
  FacesSize=Faces.size();
  
  Coordinates vecX, vecY, vecZ;
  vecX.x=6; vecX.y=0; vecX.z=0;
  vecY.x=0; vecY.y=6; vecY.z=0;
  vecZ.x=0; vecZ.y=0; vecZ.z=6;

  for(int i=0; i<FacesSize; i++){
    StructNormal auxnormal;
    Coordinates A,B,C;
    A=CoordAux[Faces[i][0]];
    B=CoordAux[Faces[i][1]];
    C=CoordAux[Faces[i][2]];
    //B-A
    x1=B.x-A.x;
    y1=B.y-A.y;
    z1=B.z-A.z;
    //C-A
    x2=C.x-A.x;
    y2=C.y-A.y;
    z2=C.z-A.z;

    auxnormal.x=y1*z2-z1*y2;
    auxnormal.y=z1*x2-x1*z2;
    auxnormal.z=x1*y2-y1*x2;
    auxnormal.face=i;

    double modn=sqrt( auxnormal.x*auxnormal.x + auxnormal.y*auxnormal.y + auxnormal.z*auxnormal.z );
    auxnormal.dotx=auxnormal.x/modn;
    auxnormal.doty=auxnormal.y/modn;
    auxnormal.dotz=auxnormal.z/modn;

    NormalsOfFaces.push_back(auxnormal);
  }
  
//****************************Posible normal********************
}


auto Draw(){
  int x1,x2,y1,y2,z1,z2;
  drawLine(0,ScreenHeight/2,ScreenWidth,ScreenHeight/2,255,255,255);
  drawLine(ScreenWidth/2,0,ScreenWidth/2,ScreenHeight,255,255,255);
 
  int n=11;
  int HalfWidth=ScreenWidth/4, HalfHeight=ScreenHeight/4;

  int XPixels=HalfWidth/n;

  
  int EdgesSize=Edges.size();

  
  for(int i=0; i<EdgesSize; i++){ 
    Vertexes=Edges[i];
    x1 = (int) (CoordAux[Vertexes[0]-1].x*XPixels/1000+HalfWidth);
    y1 = (int) ((HalfHeight)-(CoordAux[Vertexes[0]-1].y*XPixels)/1000);
    x2 = (int) ((CoordAux[Vertexes[1]-1].x*XPixels)/1000+HalfWidth);
    y2 = (int) ((HalfHeight)-(CoordAux[Vertexes[1]-1].y*XPixels/1000)); 

    drawLine(x1, y1, x2, y2, 218, 165, 32);  
  }
  
  for(int i=0; i<EdgesSize; i++){ 
    Vertexes=Edges[i];
    z1 = (int) (CoordAux[Vertexes[0]-1].z*XPixels/1000+(HalfWidth*3));
    y1 = (int) ((HalfHeight)-(CoordAux[Vertexes[0]-1].y*XPixels)/1000);
    z2 = (int) ((CoordAux[Vertexes[1]-1].z*XPixels)/1000+(HalfWidth*3));
    y2 = (int) ((HalfHeight)-(CoordAux[Vertexes[1]-1].y*XPixels/1000)); 

    drawLine(z1, y1, z2, y2, 218, 165, 32);  
  }
 
  for(int i=0; i<EdgesSize; i++){ 
    Vertexes=Edges[i];
    x1 = (int) (CoordAux[Vertexes[0]-1].x*XPixels/1000+HalfWidth);
    z1 = (int) ((HalfHeight*3)-(CoordAux[Vertexes[0]-1].z*XPixels)/1000);
    x2 = (int) ((CoordAux[Vertexes[1]-1].x*XPixels)/1000+HalfWidth);
    z2 = (int) ((HalfHeight*3)-(CoordAux[Vertexes[1]-1].z*XPixels/1000)); 

    drawLine(x1, z1, x2, z2, 218, 165, 32);  
  }
 
  for(int i=0; i<EdgesSize; i++){ 
    Vertexes=Edges[i];
    y1 = (int) (CoordAux[Vertexes[0]-1].y*XPixels/1000+(HalfWidth*3));
    z1 = (int) ((HalfHeight*3)-(CoordAux[Vertexes[0]-1].z*XPixels)/1000);
    y2 = (int) ((CoordAux[Vertexes[1]-1].y*XPixels)/1000+(HalfWidth*3));
    z2 = (int) ((HalfHeight*3)-(CoordAux[Vertexes[1]-1].z*XPixels/1000)); 

    drawLine(y1, z1, y2, z2, 218, 165, 32);  
  }
  


}

auto DrawZbuffer(){
 
  int x1,x2,y1,y2,z1,z2;
  int n=11,i,j;
  int HalfWidth=ScreenWidth/4, HalfHeight=ScreenHeight/4;
  int XPixels=HalfWidth/n; 

  vector<int> ZBufferAuxiliar;
  Normals();

  for(i=0; i<FacesSize; i++){
    if(false){        //False Dibuja todas las caras NormalsOfFaces[i].dotz<=0
      NVFNormals.push_back(NormalsOfFaces[i]);
      NotVisibleFaces.push_back(Faces[i]);
    }
    else{
      VFNormals.push_back(NormalsOfFaces[i]);
      VisibleFaces.push_back(Faces[i]);
    }
  }
  int fvsz=VisibleFaces.size(), fnsz=NotVisibleFaces.size();
  for( i=0;i<fvsz; i++){//XY
    ZBufferAuxiliar.clear();
      for(j=0; j<3; j++){
        x1 = (int) (CoordAux[ VisibleFaces[i][j] ].x*XPixels/1000+HalfWidth);
        y1 = (int) ((HalfHeight)-(CoordAux[ VisibleFaces[i][j] ].y*XPixels)/1000);
        x2 = (int) ((CoordAux[ VisibleFaces[i][(j+1)%3] ].x*XPixels)/1000+HalfWidth);
        y2 = (int) ((HalfHeight)-(CoordAux[ VisibleFaces[i][(j+1)%3] ].y*XPixels/1000)); 
        
        ZBufferAuxiliar.push_back((int)x1);
        ZBufferAuxiliar.push_back((int)y1);
        ZBufferAuxiliar.push_back((int)CoordAux[ VisibleFaces[i][j] ].z*XPixels/1000+HalfWidth);
        
      }
      ZBufferFunction(ZBufferAuxiliar[0],ZBufferAuxiliar[1],ZBufferAuxiliar[2],ZBufferAuxiliar[3],ZBufferAuxiliar[4],ZBufferAuxiliar[5],ZBufferAuxiliar[6],ZBufferAuxiliar[7],ZBufferAuxiliar[8],rand()%255,rand()%255,rand()%255);
     //ScanLine(ZBufferAuxiliar[0],ZBufferAuxiliar[1],ZBufferAuxiliar[2],ZBufferAuxiliar[3],ZBufferAuxiliar[4],ZBufferAuxiliar[5]);
  }
  for(i=0;i<fvsz; i++){
     ZBufferAuxiliar.clear();
      for(j=0; j<3; j++){
        z1 = (int) (CoordAux[ VisibleFaces[i][j] ].z*XPixels/1000+HalfWidth*3);
        y1 = (int) ((HalfHeight)-(CoordAux[ VisibleFaces[i][j] ].y*XPixels)/1000);
        z2 = (int) ((CoordAux[ VisibleFaces[i][(j+1)%3] ].z*XPixels)/1000+HalfWidth*3);
        y2 = (int) ((HalfHeight)-(CoordAux[ VisibleFaces[i][(j+1)%3] ].y*XPixels/1000)); 
         ZBufferAuxiliar.push_back((int)z1);
        ZBufferAuxiliar.push_back((int)y1);
        ZBufferAuxiliar.push_back((int)CoordAux[ VisibleFaces[i][j] ].x*XPixels/1000+HalfWidth);
      }
     ZBufferFunction(ZBufferAuxiliar[0],ZBufferAuxiliar[1],ZBufferAuxiliar[2],ZBufferAuxiliar[3],ZBufferAuxiliar[4],ZBufferAuxiliar[5],ZBufferAuxiliar[6],ZBufferAuxiliar[7],ZBufferAuxiliar[8],rand()%255,rand()%255,rand()%255);

  }
 
 //Uso de ScanLine
  for(i=0;i<fnsz;i++){
     ZBufferAuxiliar.clear();
      for(j=0; j<3; j++){
        x1 = (int) (CoordAux[ NotVisibleFaces[i][j] ].x*XPixels/1000+(HalfWidth));
        y1 = (int) ((HalfHeight*3)-(CoordAux[ NotVisibleFaces[i][j] ].y*XPixels)/1000);
        x2 = (int) ((CoordAux[ NotVisibleFaces[i][(j+1)%3] ].x*XPixels)/1000+(HalfWidth));
        y2 = (int) ((HalfHeight*3)-(CoordAux[ NotVisibleFaces[i][(j+1)%3] ].y*XPixels/1000)); 

         ZBufferAuxiliar.push_back((int)x1);
        ZBufferAuxiliar.push_back((int)y1);
      }
      ScanLine(ZBufferAuxiliar[0],ZBufferAuxiliar[1],ZBufferAuxiliar[2],ZBufferAuxiliar[3],ZBufferAuxiliar[4],ZBufferAuxiliar[5]);

  }
  for(i=0;i<fnsz;i++){
     ZBufferAuxiliar.clear();
      for(j=0; j<3; j++){
        z1 = (int) (CoordAux[ NotVisibleFaces[i][j] ].z*XPixels/1000+(HalfWidth*3));
        y1 = (int) ((HalfHeight*3)-(CoordAux[ NotVisibleFaces[i][j] ].y*XPixels)/1000);
        z2 = (int) ((CoordAux[ NotVisibleFaces[i][(j+1)%3] ].z*XPixels)/1000+(HalfWidth*3));
        y2 = (int) ((HalfHeight*3)-(CoordAux[ NotVisibleFaces[i][(j+1)%3] ].y*XPixels/1000)); 

         ZBufferAuxiliar.push_back((int)z1);
        ZBufferAuxiliar.push_back((int)y1);
      }
      ScanLine(ZBufferAuxiliar[0],ZBufferAuxiliar[1],ZBufferAuxiliar[2],ZBufferAuxiliar[3],ZBufferAuxiliar[4],ZBufferAuxiliar[5]);

  }
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
   for (int i = 0; i < ScreenWidth; i++)
  {
    for(int j=0; j<ScreenHeight; j++){
      MatrixZBuffer[i][j]=Inf;
    }
  }
 
 FHDRaster * raster = new FHDRaster();



raster-> ReadVLF();
    
raster-> DrawZbuffer();


 raster->write();
 
 if(PPM==0){
  ReleaseDC(myconsole, mydc);
  getchar();
 }
 return 0;
}