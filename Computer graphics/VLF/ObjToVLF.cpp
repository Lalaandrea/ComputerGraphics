#include<cstring>
#include<iostream>
#include<random>
#include<windows.h>
#include<math.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const string fl="mono.obj";
const string fo="mono.vlf";
/*
Program that convert a Obj file into a VLF binary file.

*/

double x, y, z;
int ChainSize;
int NumVertexes, NumFaces, NumEdges;
vector<int> Vertexes;
vector<vector<int>> carasT;
vector<vector<int>> carasN;
vector<vector<int>> Edges;
typedef struct Coordinates{
    double x;
    double y;
    double z;
}CoordAux;

CoordAux aux;
vector<CoordAux> Coord;

auto ReadObj(){
  int PositionA, Position, PositionB,verticeAux;
  string Chain, cache;
  fstream File;

  File.open(fl, fstream::in);
    while(getline(File, Chain)){ 

        if(Chain.size()>15){ 
           if(Chain.compare(0,2,"v ") == 0){
                istringstream isv(Chain);
                if(!(isv >> cache >> x >> y >> z)){
                    break;
                }
                aux.x=x;
                aux.y=y;
                aux.z=z;

                Coord.push_back(aux);
               
            }
            if(Chain.compare(0,2,"f ") == 0){
                Position=2;
                ChainSize=Chain.size();
                while(Position<ChainSize){
                    PositionA=Chain.find(" ",Position);
                    PositionB=Chain.find("/",Position);
                    cache=Chain.substr(Position,PositionB-Position);
                    istringstream isc(cache);
                    
                    if(!(isc >> verticeAux)){
                        break;
                    }
                    
                    Vertexes.push_back(verticeAux);
                    Position=PositionA+1;
                    if(Position == 0)
                        Position=ChainSize;
                }
                ChainSize=Vertexes.size();
              if(ChainSize==3)
                carasT.push_back(Vertexes);
              else
                carasN.push_back(Vertexes);

              Vertexes.clear();
            }
        }
    } 
    File.close();
}
auto FacesAndVertexes(){
  int i=1, j,  FacesNSize;
  FacesNSize=carasN.size();
    for(i=0; i<FacesNSize; i++){
        Vertexes.push_back(carasN[i][0]); Vertexes.push_back(carasN[i][1]); Vertexes.push_back(carasN[i][2]);
        carasT.push_back(Vertexes);
        Vertexes.clear();

        Vertexes.push_back(carasN[i][2]); Vertexes.push_back(carasN[i][3]); Vertexes.push_back(carasN[i][0]);
        carasT.push_back(Vertexes);
        Vertexes.clear();
    }

    
    NumVertexes=Coord.size();
    NumFaces=carasT.size();
    NumEdges=NumFaces*3;

    for(i=0; i<NumFaces; i++){
        Vertexes.push_back(carasT[i][0]); Vertexes.push_back(carasT[i][1]);
        Edges.push_back(Vertexes);
        Vertexes.clear();

        Vertexes.push_back(carasT[i][1]); Vertexes.push_back(carasT[i][2]);
        Edges.push_back(Vertexes);
        Vertexes.clear();

        Vertexes.push_back(carasT[i][2]); Vertexes.push_back(carasT[i][0]);
        Edges.push_back(Vertexes);
        Vertexes.clear();
    }
}
auto WriteVLF(){
  int i, j;
  FILE *File;
  const char *a=fo.c_str();
  File=fopen(a,"wb");
 

  fwrite(&NumVertexes,sizeof(int),1,File);
  fwrite(&NumEdges,sizeof(int),1,File);
  fwrite(&NumFaces,sizeof(int),1,File);
  
  for(i=0; i<NumVertexes; i++){
      fwrite(&Coord[i],sizeof(CoordAux),1,File);
  }
  
  for(i=0; i<NumEdges; i++){
      Vertexes=Edges[i];
      ChainSize=Vertexes.size();
      for(j=0;j<ChainSize;j++){
          fwrite(&Vertexes[j],sizeof(int),1,File);
      }
  }

  for(i=0; i<NumFaces; i++){
      Vertexes=carasT[i];
      ChainSize=Vertexes.size();
      for(j=0;j<ChainSize;j++){
          fwrite(&Vertexes[j],sizeof(int),1,File);
      }
  }
  fclose(File);
}
int main(int argc, char * argv []) {
 
  ReadObj();
  FacesAndVertexes();
  WriteVLF();
   
}
