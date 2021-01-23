#include <math.h>
#include <windows.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


/*

Program that get an obj, displays it using transformations


*/
const float pi = 3.14159;
const int ScreenHigth = 720, ScreenWidth = 1280;  // Size of screen

//##################### Transformaation options ###################
const int opMat = 2;           // 0=scale; 1=trans; 2=rot;
const int opRot = 1;           // Rotation 0=x; 1=y; 2=z;
const float ScaleRateX = 0.5;
const float ScaleRateY = 0.5;
const float ScaleRateZ = 0.5;

const float TranslationX = 2;
const float TranslationY = 2;
const float TranslationZ = 2;
const int Angle = 0;

//###################### FILE ###########################
const string fl = "mono.obj";

//Some globals variables
float OperationMatrix[4][4];
vector<int> Vertexes;
int DrawX1, DrawX2, DrawY1, DrawY2, DrawZ1, DrawZ2;
typedef struct Coord {
  short x;
  short y;
  short z;
} CoordinaesAuxiliar;
CoordinaesAuxiliar aux;
vector<CoordinaesAuxiliar> CoordinatesVector;


HWND myconsole = GetConsoleWindow();
HDC mydc = GetDC(myconsole);

class FHDRaster {
    unsigned char data[ScreenWidth][ScreenHigth][3];

   public:
    FHDRaster(void) { memset(data, 0, ScreenWidth * ScreenHigth * 3); }
    void DrawPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g,
                   unsigned char b) {
      if (x >= ScreenWidth || y >= ScreenHigth) { return; }
      data[x][y][0] = r;
      data[x][y][1] = g;
      data[x][y][2] = b;
    }
    void write(void) {
      int x = 0;
      int y = 0;

      COLORREF COLOR;
      int pixel = 0;
      for (y = 0; y < ScreenHigth; y++) {
        for (x = 0; x < ScreenWidth; x++) {
          COLOR = RGB(data[x][y][0], data[x][y][1], data[x][y][2]);
          SetPixel(mydc, x, y, COLOR);
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

/*Make te transformations to the points*/
auto transformation(float OperationMatrix[4][4], const float opMat) {
  

  float Theta = Angle * pi / 180;

  if (opMat == 0) {
    OperationMatrix[0][0] = ScaleRateX;
    OperationMatrix[0][1] = 0;
    OperationMatrix[0][2] = 0;
    OperationMatrix[0][3] = 0;

    OperationMatrix[1][0] = 0;
    OperationMatrix[1][1] = ScaleRateY;
    OperationMatrix[1][2] = 0;
    OperationMatrix[1][3] = 0;

    OperationMatrix[2][0] = 0;
    OperationMatrix[2][1] = 0;
    OperationMatrix[2][2] = ScaleRateZ;
    OperationMatrix[2][3] = 0;

    OperationMatrix[3][0] = 0;
    OperationMatrix[3][1] = 0;
    OperationMatrix[3][2] = 0;
    OperationMatrix[3][3] = 1;
  } 
  else if (opMat == 1) {
    OperationMatrix[0][0] = TranslationX;
    OperationMatrix[0][1] = 0;
    OperationMatrix[0][2] = 0;
    OperationMatrix[0][3] = 0;

    OperationMatrix[1][0] = 0;
    OperationMatrix[1][1] = TranslationY;
    OperationMatrix[1][2] = 0;
    OperationMatrix[1][3] = 0;

    OperationMatrix[2][0] = 0;
    OperationMatrix[2][1] = 0;
    OperationMatrix[2][2] = TranslationZ;
    OperationMatrix[2][3] = 0;

    OperationMatrix[3][0] = 0;
    OperationMatrix[3][1] = 0;
    OperationMatrix[3][2] = 0;
    OperationMatrix[3][3] = 1;
  } 
  else 
    {if (opRot == 0) {
      OperationMatrix[0][0] = 1;
      OperationMatrix[0][1] = 0;
      OperationMatrix[0][2] = 0;
      OperationMatrix[0][3] = 0;

      OperationMatrix[1][0] = 0;
      OperationMatrix[1][1] = cos(Theta);
      OperationMatrix[1][2] = sin(Theta) * -1;
      OperationMatrix[1][3] = 0;

      OperationMatrix[2][0] = 0;
      OperationMatrix[2][1] = sin(Theta);
      OperationMatrix[2][2] = cos(Theta);
      OperationMatrix[2][3] = 0;

      OperationMatrix[3][0] = 0;
      OperationMatrix[3][1] = 0;
      OperationMatrix[3][2] = 0;
      OperationMatrix[3][3] = 1;
    } 
    else if (opRot == 1) {
      OperationMatrix[0][0] = cos(Theta);
      OperationMatrix[0][1] = 0;
      OperationMatrix[0][2] = -sin(Theta);
      OperationMatrix[0][3] = 0;

      OperationMatrix[1][0] = 0;
      OperationMatrix[1][1] = 1;
      OperationMatrix[1][2] = 0;
      OperationMatrix[1][3] = 0;

      OperationMatrix[2][0] = sin(Theta);
      OperationMatrix[2][1] = 0;
      OperationMatrix[2][2] = cos(Theta);
      OperationMatrix[2][3] = 0;

      OperationMatrix[3][0] = 0;
      OperationMatrix[3][1] = 0;
      OperationMatrix[3][2] = 0;
      OperationMatrix[3][3] = 1;
    } 
    else {
      OperationMatrix[0][0] = cos(Theta);
      OperationMatrix[0][1] = -sin(Theta);
      OperationMatrix[0][2] = 0;
      OperationMatrix[0][3] = 0;

      OperationMatrix[1][0] = sin(Theta);
      OperationMatrix[1][1] = cos(Theta);
      OperationMatrix[1][2] = 0;
      OperationMatrix[1][3] = 0;

      OperationMatrix[2][0] = 0;
      OperationMatrix[2][1] = 0;
      OperationMatrix[2][2] = 1;
      OperationMatrix[2][3] = 0;

      OperationMatrix[3][0] = 0;
      OperationMatrix[3][1] = 0;
      OperationMatrix[3][2] = 0;
      OperationMatrix[3][3] = 1;
    }
  }
    float Points[4][1] = {{(float) aux.x}, {(float) aux.y}, {(float) aux.z}, {1}};
        float Transformed[4][1] = {{0}, {0}, {0}, {0}};

        for (int row = 0; row < 4; row++) {
          for (int col = 0; col < 1; col++) {
            for (int inner = 0; inner < 4; inner++) {
              Transformed[row][col] += OperationMatrix[row][inner] * Points[inner][col];
            }
          }
        }
        //Get values
        aux.x = (int) Transformed[0][0] / Transformed[3][0];
        aux.y = (int) Transformed[1][0] / Transformed[3][0];
        aux.z = (int) Transformed[2][0] / Transformed[3][0];


        CoordinatesVector.push_back(aux);
}
//Draw the file with transformations
auto Draw (auto ChainSize, auto XPixels, auto HalfX, auto HalfY ){
   int HalfHeigth = ScreenHigth / 2, HalfWeigth = ScreenWidth / 2;
    drawLine(0, HalfHeigth, ScreenWidth, HalfHeigth, 255, 255, 255);
    drawLine(HalfWeigth, 0, HalfWeigth, ScreenHigth, 255, 255, 255);

    ChainSize = Vertexes.size();
        //Drawing
        for (int j = 0; j < ChainSize; j++) {
          DrawX1 = (int) ((CoordinatesVector [Vertexes[j] - 1].x * XPixels) / 1000 + HalfX);
          DrawY1 = (int) (HalfY - (CoordinatesVector[Vertexes[j] - 1].y * XPixels) / 1000);
          DrawX2 = (int) ((CoordinatesVector[Vertexes[(j + 1) % ChainSize] - 1].x * XPixels) / 1000 + HalfX);
          DrawY2 = (int) (HalfY - (CoordinatesVector[Vertexes[(j + 1) % ChainSize] - 1].y * XPixels / 1000));

          drawLine(DrawX1, DrawY1, DrawX2, DrawY2, 218, 165, 32);  
        }

}

auto ReadObj(){
  fstream File;

  

    string Chain, cache;
 

    float x, y, z;

    int  PositionA, Position, PositionB, VertexAux;


  File.open(fl, fstream::in);
  while (getline(File, Chain)) {


    if (Chain.size() > 15) {

      // Get vertexes and transform them
      if (Chain.compare(0, 2, "v ") == 0) {
          istringstream isv(Chain);
          if (!(isv >> cache >> x >> y >> z)) { break; }
          aux.x = (int) (x * 1000);
          aux.y = (int) (y * 1000);
          aux.z = (int) (z * 1000); /* Coordinates in a bigger scale*/
          //######################### TRASFORMATIONS ##############################


        transformation(OperationMatrix, opMat);
        
      
      }
      //Get Faces 

      if (Chain.compare(0, 2, "f ") == 0) {

        Position = 2;
        int ChainSize = Chain.size();
        int n = 10;

        int HalfX = ScreenWidth / 2, HalfY = ScreenHigth / 2;
        int XPixels = HalfX / n;
        while (Position < ChainSize) {

          PositionA = Chain.find(" ", Position);
          PositionB = Chain.find("/", Position);
          cache = Chain.substr(Position, PositionB - Position);
          istringstream isc(cache);

          if (!(isc >> VertexAux)) break;
          

          Vertexes.push_back(VertexAux);
          Position = PositionA + 1;
          if (Position == 0) Position = ChainSize;
        }


        Draw(ChainSize, XPixels, HalfX, HalfY);
        Vertexes.clear();
      }
    }
  }
  File.close();
}
};

  int main(int argc, char* argv[]) {
    
    FHDRaster* raster = new FHDRaster();

   
    raster->ReadObj();

    raster->write();

  ReleaseDC(myconsole, mydc);
  getchar();

  return 0;
}