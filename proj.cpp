//! [includes]
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _WIN32
  #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif


#include <iostream>
#include <string>
#include <vector>
#include <tuple>


using namespace cv;
using namespace std;
//using namespace gl;

struct View {
    Mat img;
    String imageName;
    vector <Point2f> imagePoints;
    vector <Point3f> modelPoints;
    vector <Point2f> projectedPoints;

};

vector<Point2f> imagePointsVec; //holds the points of all line intersections visible in the image
cv::Size imageSize;
int x_image;
int y_image;
int pointCounter;
bool finished;

vector<Point3f> createRugbyModel(){
    vector<Point3f> modelPoints;
    modelPoints.push_back(Point3f(34,88,0)); //0
    modelPoints.push_back(Point3f(58,88,0)); //1
    modelPoints.push_back(Point3f(337,88,0)); //2
    modelPoints.push_back(Point3f(368,88,0)); //3
    modelPoints.push_back(Point3f(34,184,0)); //4
    modelPoints.push_back(Point3f(58,184,0)); //5
    modelPoints.push_back(Point3f(337,184,0)); //6
    modelPoints.push_back(Point3f(368,184,0)); //7
    modelPoints.push_back(Point3f(34,250,0)); //8
    modelPoints.push_back(Point3f(58,250,0)); //9
    modelPoints.push_back(Point3f(337,250,0)); //10
    modelPoints.push_back(Point3f(368,250,0)); //11
    modelPoints.push_back(Point3f(34,302,0)); //12
    modelPoints.push_back(Point3f(58,302,0)); //13
    modelPoints.push_back(Point3f(337,302,0)); //14
    modelPoints.push_back(Point3f(368,302,0)); //15
    modelPoints.push_back(Point3f(34,352,0)); //16
    modelPoints.push_back(Point3f(58,352,0)); //17
    modelPoints.push_back(Point3f(337,352,0)); //18
    modelPoints.push_back(Point3f(368,352,0)); //19
    modelPoints.push_back(Point3f(34,424,0)); //20
    modelPoints.push_back(Point3f(58,424,0)); //21
    modelPoints.push_back(Point3f(337,424,0)); //22
    modelPoints.push_back(Point3f(368,424,0)); //23
    modelPoints.push_back(Point3f(34,522,0)); //24
    modelPoints.push_back(Point3f(58,522,0)); //25
    modelPoints.push_back(Point3f(337,522,0)); //26
    modelPoints.push_back(Point3f(368,522,0)); //27
    return modelPoints;
}

vector<Point3f> createFootballModel(){
	vector<Point3f> modelPoints;
	modelPoints.push_back(Point3f(45,27,0)); //1
	modelPoints.push_back(Point3f(333,27,0)); //2
	modelPoints.push_back(Point3f(626,27,0)); //3
	modelPoints.push_back(Point3f(45,151,0)); //4
	modelPoints.push_back(Point3f(125,151,0)); //5
	modelPoints.push_back(Point3f(546,151,0)); //6
	modelPoints.push_back(Point3f(626,151,0)); //7
	modelPoints.push_back(Point3f(45,203,0)); //8
	modelPoints.push_back(Point3f(73,203,0)); //9
	modelPoints.push_back(Point3f(599,203,0)); //10
	modelPoints.push_back(Point3f(626,203,0)); //11
	modelPoints.push_back(Point3f(125,214,0)); //12
	modelPoints.push_back(Point3f(333,201,0)); //13
	modelPoints.push_back(Point3f(546,214,0)); //14
	modelPoints.push_back(Point3f(626,231,0)); //15
	modelPoints.push_back(Point3f(45,266,0)); //16
	modelPoints.push_back(Point3f(626,266,0)); //17
	modelPoints.push_back(Point3f(125,281,0)); //18
	modelPoints.push_back(Point3f(333,292,0)); //19
	modelPoints.push_back(Point3f(546,282,0)); //20
	modelPoints.push_back(Point3f(45,290,0)); //21
	modelPoints.push_back(Point3f(73,290,0)); //22
	modelPoints.push_back(Point3f(599,290,0)); //23
	modelPoints.push_back(Point3f(626,290,0)); //24
	modelPoints.push_back(Point3f(45,347,0)); //25
	modelPoints.push_back(Point3f(125,347,0)); //26
	modelPoints.push_back(Point3f(546,347,0)); //27
	modelPoints.push_back(Point3f(626,347,0)); //28
	modelPoints.push_back(Point3f(45,469,0)); //29
	modelPoints.push_back(Point3f(333,469,0)); //30
	modelPoints.push_back(Point3f(627,469,0)); //31
	return modelPoints;
} 

void CallBackFunc(int event, int x, int y, int flags, void* userdata){

    if(event == EVENT_LBUTTONDOWN){
        Mat &image = *((Mat*)(userdata));
        x_image = x;
        y_image = y; 
        imagePointsVec.push_back(Point2f(x_image, y_image)); 
        pointCounter = pointCounter + 1;    
        //draw circle over area they have clicked on in red
        circle(image, Point (x_image, y_image), 16.0, Scalar(0,0,255), 1, 8);
        cout<<"Cick on point: "<<pointCounter+1<<" if you can see it, othwerwise press 's'"<<endl;
               
    }

}

bool isEdge(int node1, int node2, vector< vector<int> > graph)
{
	//method to test if there is an edge in graph between two nodes
  cout<<"is there an edge?"<<graph[node1][node2]<<endl;
  if(graph[node1][node2] == 1){
    //there is an edge between these nodes
    cout<<"There is an edge between"<<node1+1<<" and "<<node2+1<<endl;
    return true;
  }
  else{
    return false;
  }

  
}

vector< vector<int> > createFootballGraph()
{
	//method to create the graph for football model
	vector< vector<int> > graph;
  //populate each row in adjacency matrix
  int myints1[] = {0,1,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0};
  std::vector<int> row1 (myints1, myints1 + sizeof(myints1) / sizeof(int) );
  graph.push_back(row1);
  int myints2[] = {1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0};
  std::vector<int> row2 (myints2, myints2 + sizeof(myints2) / sizeof(int) );
  graph.push_back(row2);
  int myints3[] = {1,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,1};
  std::vector<int> row3 (myints3, myints3 + sizeof(myints3) / sizeof(int) );
  graph.push_back(row3);
  int myints4[] = {1,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0};
  std::vector<int> row4 (myints4, myints4 + sizeof(myints4) / sizeof(int) );
  graph.push_back(row4);
  int myints5[] = {0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0};
  std::vector<int> row5 (myints5, myints5 + sizeof(myints5) / sizeof(int) );
  graph.push_back(row5);
  int myints6[] = {0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0};
  std::vector<int> row6 (myints6, myints6 + sizeof(myints6) / sizeof(int) );
  graph.push_back(row6);
  int myints7[] = {0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,1};
  std::vector<int> row7 (myints7, myints7 + sizeof(myints7) / sizeof(int) );
  graph.push_back(row7);
  int myints8[] = {1,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0};
  std::vector<int> row8 (myints8, myints8 + sizeof(myints8) / sizeof(int) );
  graph.push_back(row8);
  int myints9[] = {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0};
  std::vector<int> row9 (myints9, myints9 + sizeof(myints9) / sizeof(int) );
  graph.push_back(row9);
  int myints10[] = {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0};
  std::vector<int> row10 (myints10, myints10 + sizeof(myints10) / sizeof(int) );
  graph.push_back(row10);
  int myints11[] = {0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,1};
  std::vector<int> row11 (myints11, myints11 + sizeof(myints11) / sizeof(int) );
  graph.push_back(row11);
  int myints12[] = {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0};
  std::vector<int> row12 (myints12, myints12 + sizeof(myints12) / sizeof(int) );
  graph.push_back(row12);
  int myints13[] = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0};
  std::vector<int> row13 (myints13, myints13 + sizeof(myints13) / sizeof(int) );
  graph.push_back(row13);
  int myints14[] = {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0};
  std::vector<int> row14 (myints14, myints14 + sizeof(myints14) / sizeof(int) );
  graph.push_back(row14);
  int myints15[] = {1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0};
  std::vector<int> row15 (myints15, myints15 + sizeof(myints15) / sizeof(int) );
  graph.push_back(row15);
  int myints16[] = {0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,1};
  std::vector<int> row16 (myints16, myints16 + sizeof(myints16) / sizeof(int) );
  graph.push_back(row16);
  int myints17[] = {1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0};
  std::vector<int> row17 (myints17, myints17 + sizeof(myints17) / sizeof(int) );
  graph.push_back(row17);
  int myints18[] = {0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1};
  std::vector<int> row18 (myints18, myints18 + sizeof(myints18) / sizeof(int) );
  graph.push_back(row18);
  int myints19[] = {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0};
  std::vector<int> row19 (myints19, myints19 + sizeof(myints19) / sizeof(int) );
  graph.push_back(row19);
  int myints20[] = {0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0};
  std::vector<int> row20 (myints20, myints20 + sizeof(myints20) / sizeof(int) );
  graph.push_back(row20);
  int myints21[] = {0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0};
  std::vector<int> row21 (myints21, myints21 + sizeof(myints21) / sizeof(int) );
  graph.push_back(row21);
  int myints22[] = {1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0};
  std::vector<int> row22 (myints22, myints22 + sizeof(myints22) / sizeof(int) );
  graph.push_back(row22);
  int myints23[] = {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0};
  std::vector<int> row23 (myints23, myints23 + sizeof(myints23) / sizeof(int) );
  graph.push_back(row23);
  int myints24[] = {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0};
  std::vector<int> row24 (myints24, myints24 + sizeof(myints24) / sizeof(int) );
  graph.push_back(row24);
  int myints25[] = {0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,1};
  std::vector<int> row25 (myints25, myints25 + sizeof(myints25) / sizeof(int) );
  graph.push_back(row25);
  int myints26[] = {1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0};
  std::vector<int> row26 (myints26, myints26 + sizeof(myints26) / sizeof(int) );
  graph.push_back(row26);
  int myints27[] = {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0};
  std::vector<int> row27 (myints27, myints27 + sizeof(myints27) / sizeof(int) );
  graph.push_back(row27);
  int myints28[] = {0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0};
  std::vector<int> row28 (myints28, myints28 + sizeof(myints28) / sizeof(int) );
  graph.push_back(row28);
  int myints29[] = {0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,1,0,1,0,1};
  std::vector<int> row29 (myints29, myints29 + sizeof(myints29) / sizeof(int) );
  graph.push_back(row29);
  int myints30[] = {1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1};
  std::vector<int> row30 (myints30, myints30 + sizeof(myints30) / sizeof(int) );
  graph.push_back(row30);
  int myints31[] = {0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1};
  std::vector<int> row31 (myints31, myints31 + sizeof(myints31) / sizeof(int) );
  graph.push_back(row31);
  int myints32[] = {0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1,1,1,0};
  std::vector<int> row32 (myints32, myints32 + sizeof(myints32) / sizeof(int) );
  graph.push_back(row32);
  return graph;

		
}

vector< vector<int> > createRugbyGraph()
{
	//method to create the graph for rugby model
	vector< vector<int> > graph;

	int myints1[] = {0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0};
  	std::vector<int> row1 (myints1, myints1 + sizeof(myints1) / sizeof(int) );
  	graph.push_back(row1);
  	int myints2[] = {1,0,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0};
  	std::vector<int> row2 (myints2, myints2 + sizeof(myints2) / sizeof(int) );
  	graph.push_back(row2);
  	int myints3[] = {1,1,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0};
  	std::vector<int> row3 (myints3, myints3 + sizeof(myints3) / sizeof(int) );
  	graph.push_back(row3);
  	int myints4[] = {1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1};
  	std::vector<int> row4 (myints4, myints4 + sizeof(myints4) / sizeof(int) );
  	graph.push_back(row4);
  	int myints5[] = {1,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0};
  	std::vector<int> row5 (myints5, myints5 + sizeof(myints5) / sizeof(int) );
  	graph.push_back(row5);
  	int myints6[] = {0,1,0,0,1,0,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0};
  	std::vector<int> row6 (myints6, myints6 + sizeof(myints6) / sizeof(int) );
  	graph.push_back(row6);
  	int myints7[] = {0,0,1,0,1,1,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0};
  	std::vector<int> row7 (myints7, myints7 + sizeof(myints7) / sizeof(int) );
  	graph.push_back(row7);
  	int myints8[] = {0,0,0,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1};
  	std::vector<int> row8 (myints8, myints8 + sizeof(myints8) / sizeof(int) );
  	graph.push_back(row8);
  	int myints9[] = {1,0,0,0,1,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0};
  	std::vector<int> row9 (myints9, myints9 + sizeof(myints9) / sizeof(int) );
  	graph.push_back(row9);
  	int myints10[] = {0,1,0,0,0,1,0,0,1,0,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0};
  	std::vector<int> row10 (myints10, myints10 + sizeof(myints10) / sizeof(int) );
  	graph.push_back(row10);
  	int myints11[] = {0,0,1,0,0,0,1,0,1,1,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0};
  	std::vector<int> row11 (myints11, myints11 + sizeof(myints11) / sizeof(int) );
  	graph.push_back(row11);
  	int myints12[] = {0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1};
  	std::vector<int> row12 (myints12, myints12 + sizeof(myints12) / sizeof(int) );
  	graph.push_back(row12);
  	int myints13[] = {1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0};
  	std::vector<int> row13 (myints13, myints13 + sizeof(myints13) / sizeof(int) );
  	graph.push_back(row13);
  	int myints14[] = {0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,1,0,0,0,1,0,0,0,1,0,0};
  	std::vector<int> row14 (myints14, myints14 + sizeof(myints14) / sizeof(int) );
  	graph.push_back(row14);
  	int myints15[] = {0,0,1,0,0,0,1,0,0,0,1,0,1,1,0,1,0,0,1,0,0,0,1,0,0,0,1,0};
  	std::vector<int> row15 (myints15, myints15 + sizeof(myints15) / sizeof(int) );
  	graph.push_back(row15);
  	int myints16[] = {0,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1};
  	std::vector<int> row16 (myints16, myints16 + sizeof(myints16) / sizeof(int) );
  	graph.push_back(row16);
  	int myints17[] = {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0};
  	std::vector<int> row17 (myints17, myints17 + sizeof(myints17) / sizeof(int) );
  	graph.push_back(row17);
  	int myints18[] = {0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,1,1,0,1,0,0,0,1,0,0};
  	std::vector<int> row18 (myints18, myints18 + sizeof(myints18) / sizeof(int) );
  	graph.push_back(row18);
  	int myints19[] = {0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,0,1,0,0,1,0,0,0,1,0};
  	std::vector<int> row19 (myints19, myints19 + sizeof(myints19) / sizeof(int) );
  	graph.push_back(row19);
  	int myints20[] = {0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,1,0,0,0,1};
  	std::vector<int> row20 (myints20, myints20 + sizeof(myints20) / sizeof(int) );
  	graph.push_back(row20);
  	int myints21[] = {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,0,0,0};
  	std::vector<int> row21 (myints21, myints21 + sizeof(myints21) / sizeof(int) );
  	graph.push_back(row21);
  	int myints22[] = {0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,1,1,0,1,0,0};
  	std::vector<int> row22 (myints22, myints22 + sizeof(myints22) / sizeof(int) );
  	graph.push_back(row22);
  	int myints23[] = {0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,0,1,0,0,1,0};
  	std::vector<int> row23 (myints23, myints23 + sizeof(myints23) / sizeof(int) );
  	graph.push_back(row23);
  	int myints24[] = {0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,1};
  	std::vector<int> row24 (myints24, myints24 + sizeof(myints24) / sizeof(int) );
  	graph.push_back(row24);
  	int myints25[] = {1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1};
  	std::vector<int> row25 (myints25, myints25 + sizeof(myints25) / sizeof(int) );
  	graph.push_back(row25);
  	int myints26[] = {0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,1,1};
  	std::vector<int> row26 (myints26, myints26 + sizeof(myints26) / sizeof(int) );
  	graph.push_back(row26);
  	int myints27[] = {0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,0,1};
  	std::vector<int> row27 (myints27, myints27 + sizeof(myints27) / sizeof(int) );
  	graph.push_back(row27);
  	int myints28[] = {0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,0};
  	std::vector<int> row28 (myints28, myints28 + sizeof(myints28) / sizeof(int) );
  	graph.push_back(row28);
  	return graph;

}

void drawLine(Point2f p1, Point2f p2, Mat image)
{
  //draws a line between two points in the image
  cout<<Point(p1)<<" , "<<Point(p2)<<endl;
  line(image, Point(p1), Point(p2), Scalar(0,0,255), 2, 8);

}





int main( int argc, char** argv )
{

	//navigate to image directory
	String folderpath = "/Users/caitlinmcardle/Documents/ThirdYear/InidividualProject/HawkeyeData/sampledata/*.png";
    vector<String> filenames;
    cv::glob(folderpath, filenames);   
    
    //load rugby and football graphs
    vector< vector<int> > rugbyGraph;
    vector< vector<int> > footballGraph;
    rugbyGraph = createRugbyGraph();
    footballGraph = createFootballGraph();


    //iterate through images
    for(size_t i=0; i<filenames.size(); i++)
    {
        cout<<"Press 'n' for next image"<<endl;
        //create the following variables for each image 
        struct View view;
        Mat model;
        vector<Point3f> allModelPoints; //holds all possible line intersections for that model        
		    vector<Point3f> corrModelPointsVec; //holds the corresponding model points to the ones found in imagePointsVec
		    vector<Point2f> corrImagePointsVec; //like imagePointsVec but without null entries
		    vector<int> nodes; //list of nodes (visible in this image) to lookup in main graph and test for edges
        vector< vector<int> > graph; //generic graph which will either be rugby or football
        int limit;

		    bool terminate; //ends program

		    //initialise point counter to 0
		    pointCounter = 0;

		    //set finished to false for point selecetion
		    finished = false;

        //check whether to load football or rugby UI
        if(filenames[i].find("rugby") != string::npos){
        	//load rugby model
    		model = imread("rugbymodelUI.png");
    		namedWindow("Model", WINDOW_AUTOSIZE);
    		//resize(model, model, Size(model.cols/5, model.rows/5));  -- do resize on unedited photo
    		moveWindow("Model", 100, 100);
    		allModelPoints = createRugbyModel();
        graph = rugbyGraph;
        limit = 28; //28 intersections on rugby pitch

        }
        else{
        	//load football model
        	model = imread("footballmodelUI.png");
        	namedWindow("Model", WINDOW_AUTOSIZE);
    		  //resize(model, model, Size(model.cols/3, model.rows/3));  -- do resize on unedited photo
    		  moveWindow("Model", 10, 100);
    		  allModelPoints = createFootballModel();
          graph = footballGraph;
          limit = 32; //32 intersections on football pitch
        }


        //read an image
        Mat image = imread(filenames[i]);
        if( image.empty() )                      // Check for invalid input
        {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
        }
        imageSize = image.size();
        

        // window for broadcast image 
        namedWindow( "Broadcast Image", WINDOW_AUTOSIZE ); // Create a window for display.
        resize(image, image, Size(image.cols/3, image.rows/3));
        moveWindow("Broadcast Image", 750, 100);
        //set event callback
        setMouseCallback("Broadcast Image", CallBackFunc, &image);

        Mat result = image.clone(); //make clone to use as result

        

        while(finished == false){
        	
	        //show the model and image 
	        imshow( "Broadcast Image", image ); 
	        imshow("Model", model);

	        unsigned char k = waitKey(20);
	        if(k == 'x'){
	        	finished = true;
	        	terminate = true;
	        	destroyAllWindows();
	        }
	        if(k == 'n'){
	        	//move to next image
	        	finished = true;     
	        }
	        if(k == 's'){
	        	//skip this point as not in image
	        	pointCounter ++;	        	
	        	std::cout << "Skipped point "<<pointCounter<<endl;
	        	cout<<"Click on point: "<<pointCounter+1<<" if you can see it, othwerwise press 's'"<<endl;
	        	imagePointsVec.push_back(Point2f(-1,-1));
            if(pointCounter >= limit){
              finished = true;
            }
	        }	        
	    }


        //now we need to get two arrays of corresponding image and model points 
        // iterate through image points, if not (-1,-1) then add to corrimage
        //lookup this index in all model points and add to corrmodel
        //add these indices to a list of nodes to lookup in the graph 
        for(int i=0; i< imagePointsVec.size(); ++i)
        {
        	Point2f nullValue = Point2f(-1,-1);
            if(imagePointsVec[i] != nullValue){
            	corrImagePointsVec.push_back(imagePointsVec[i]);
            	corrModelPointsVec.push_back(allModelPoints[i]);
            	nodes.push_back(i);
            }
        }

        //draw lines on pitch
        //iterate through all points that have been selected -- using nodes list 
        //value at node[i] gives us the index we need to look at in the graph -- pass this to boolean method
        //check if egdge between them in graph
        //if true, draw line between those image points
        int node1;
        int node2;
        bool lineExists = false;        
        for(int i=0; i < nodes.size(); ++i)
        {
          for(int j=0; j < nodes.size(); ++j)
          {
            node1 = nodes[i];
            node2 = nodes[j];
            //lineExists = isEdge(node1, node1, graph);
            if(graph[node1][node2] == 1)
            {
              lineExists = true;
            }
            else
            {
              lineExists = false;
            }
            if(lineExists){
              line(result, Point(imagePointsVec[node1]), Point(imagePointsVec[node2]), Scalar(0,0,255), 2, 8);
            }
          }
        }

        //dispay result
        namedWindow("Result", WINDOW_AUTOSIZE);
        moveWindow("Result", 0, 100);
        imshow("Result", result);

        //do the calibration for this view
        std::vector< std::vector <cv::Vec3f> > calibObjectPoints; //correct format for model points list
        std::vector< std::vector <cv::Vec2f> > calibImagePoints; //correct format for image points list
        
        //loop through corrImagePoints and add to imagePoints
        for(int i=0; i< corrImagePointsVec.size(); ++i)
        {
          float x_component = corrImagePointsVec[i].x;
          float y_component = corrImagePointsVec[i].y;
          Vec2f point = Vec2f(x_component, y_component);
          vector<Vec2f>  points;
          points.push_back(point);
          calibImagePoints.push_back(points);

        }
        //loop through corrModelPoints and add to objectPoints
        for(int i=0; i< corrModelPointsVec.size(); ++i)
        {
          float x_component = corrModelPointsVec[i].x;
          float y_component = corrModelPointsVec[i].y;
          float z_component = corrModelPointsVec[i].z;
          Vec3f point = Vec3f(x_component, y_component, z_component);
          vector<Vec3f> points;
          points.push_back(point);
          calibObjectPoints.push_back(points);

        }

        //print statements for debugging
        for (int i = 0; i < calibObjectPoints.size(); i++){
          for (int j = 0; j < calibObjectPoints[i].size(); j++){
            cout << calibObjectPoints[i][j];
          }
        }

        for (int i = 0; i < calibImagePoints.size(); i++){
          for (int j = 0; j < calibImagePoints[i].size(); j++){
            cout << calibImagePoints[i][j];
          }
        }
        

        //define variables to hold the unknowns
        Mat cameraMatrix = Mat(3, 3, CV_32FC1); 
        Mat distCoeffs; 
        vector<Mat> rvecs; 
        vector<Mat> tvecs;

        //debug - check if vectors are empty before calibration -- first frame always seems to be empty
        if(calibImagePoints[0].size() != 0 and calibObjectPoints[0].size() != 0){
          //calibrate
          double calibrationResults = calibrateCamera(calibObjectPoints, calibImagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs);
          //print statements for debugging
        // cout<<"Camera Matrix"<<cameraMatrix<<endl;
        // cout<<"DistCoeffs"<<distCoeffs;
        // cout<<"Rvecs:"<<endl;
        // for(int i=0; i< rvecs.size(); ++i)
        // { 
        //   cout<<"Rvec: "<<rvecs[i]<<endl;

        // }
        // cout<<"Tvecs:"<<endl;
        // for(int i=0; i< tvecs.size(); ++i)
        // {
        //   cout<<"Tvec: "<<tvecs[i]<<endl;
        // }

        }

        

        
        

        //moving to next image
        imagePointsVec.clear(); 

	    //breaks out of main
	    if(terminate == true){
	    	break;
	    }
       
    
        
        
	}
	return 0;
}
