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
#include <map>
#include <queue>

using namespace std;

const string FileInput="cubo.vlf";
const string FileOutput="Ocubo.vlf";
/*
Program that convert a Obj file into a VLF binary file.

*/
typedef struct Coordinates{
    double x;
    double y;
    double z;
}CoordAux;

struct Point{
	int x;
	int y;
	int z;
};

struct Edge{
	int v;
	bool same;
	
};

double x, y, z;
int ChainSize;
int NumVertexes, NumFaces, NumEdges;
vector<int> Vertexes;
int cambios=0;
vector<int> VertexesAux;
vector<vector<int>> Faces;
vector<vector<int>> carasN;
vector<vector<int>> Edges;
int n;

  
  map<pair<Point,Point>,int> MapSerch;
  Point a,b;
   CoordAux InputCoord;



CoordAux aux;
vector<CoordAux> vertex;

bool operator<(Point p, Point q){
	return tie(p.x,p.y,p.z) < tie(q.x,q.y,q.z);

}
auto ReadObj(){
  const char *cc=FileInput.c_str();
  FILE *fin;
  fin=fopen(cc,"rb");
    
  fread(&NumVertexes,sizeof(int),1,fin);
  fread(&NumEdges,sizeof(int),1,fin);
  fread(&NumFaces,sizeof(int),1,fin);
  for(int i=0;i<NumVertexes;i++){
    fread(&InputCoord,sizeof(CoordAux),1,fin);
    vertex.push_back(InputCoord);
  }
  for(int i=0; i<NumEdges; i++){
    for(int j=0; j<2; j++){
      fread(&n,sizeof(int),1,fin);
      VertexesAux.push_back(n);
    }
    Edges.push_back(VertexesAux);
    VertexesAux.clear();
  }
  for(int i=0; i<NumFaces; i++){
    for(int j=0; j<3; j++){
      fread(&n,sizeof(int),1,fin);
      VertexesAux.push_back(n);
    }
    Faces.push_back(VertexesAux);
    VertexesAux.clear();
  }
  fclose(fin);

}
auto FacesAndVertexes(){
  vector<vector<Edge>> g(NumFaces);

  for(int u=0; u<NumFaces; u++){
    for(int v=0; v<3; v++){
      a.x=vertex[Faces[u][v]].x; a.y=vertex[Faces[u][v]].y; a.z=vertex[Faces[u][v]].z;
      b.x=vertex[Faces[u][(v+1)%3]].x; b.y=vertex[Faces[u][(v+1)%3]].y; b.z=vertex[Faces[u][(v+1)%3]].z;
   
      if(MapSerch.count({a,b})){
        int v=MapSerch[{a,b}];
        g[u].push_back({v,true});
        g[v].push_back({u,true});
        cambios++;
      } else if (MapSerch.count({b,a})){
        int v=MapSerch[{b,a}];
        g[u].push_back({v,false});
        g[v].push_back({u,false});
      } else{
        MapSerch[{a,b}] = u;
      }/**/

    }

  }

  n=NumFaces;
  vector<bool> FacesToChange(n,false), flip(n);
  flip[0]=false;
  queue<int> QueueForWhile;
  QueueForWhile.push(0);
  while(!QueueForWhile.empty()){
    int u=QueueForWhile.front();
   
    QueueForWhile.pop();
    for(Edge e : g[u]){
      if(!FacesToChange[e.v]){
        FacesToChange[e.v] = true;
        flip[e.v]=(flip[u]^e.same);
        
        QueueForWhile.push(e.v);
        
      }

    }

  }


  //FLIP
  for(int u=0; u<n; u++)
    if(flip[u]){
      int FaceA, FaceB, FaceC;
      FaceA=Faces[u][0];
      FaceB=Faces[u][1];
      FaceC=Faces[u][2];
      Faces[u][2]=FaceA;
      Faces[u][1]=FaceB;
      Faces[u][0]=FaceC;
    }

}
void WriteVLF(){
  FILE *fout;
  const char *cco=FileOutput.c_str();
  fout=fopen(cco,"wb");
 

  fwrite(&NumVertexes,sizeof(int),1,fout);
  fwrite(&NumEdges,sizeof(int),1,fout);
  fwrite(&NumFaces,sizeof(int),1,fout);
  

  for(int i=0; i<NumVertexes; i++){
    fwrite(&vertex[i],sizeof(CoordAux),1,fout);
  }

  for(int i=NumEdges-1; i>=0; i--){
    VertexesAux=Edges[i];
    n=VertexesAux.size();
    for(int j=0; j<n; j++){
      fwrite(&VertexesAux[j],sizeof(int),1,fout);
    }
  }

  for(int i=NumFaces-1; i>=0; i--){
    VertexesAux=Faces[i];
    n=VertexesAux.size();
    for(int j=0; j<n; j++){
      fwrite(&VertexesAux[j],sizeof(int),1,fout);
    }
  }

  fclose(fout);
}
int main(int argc, char * argv []) {
 
  ReadObj();
  FacesAndVertexes();
  WriteVLF();
   
}
