#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include "geometry_msgs/Point.h"
#include "std_msgs/String.h"
#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include<typeinfo>
#include<math.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#include<termios.h>
#include<stdio.h>
#include<stack>
#include<bits/stdc++.h>
#include "opencv/cv.h"
using namespace cv;
using namespace std;
Point pt, ptf;
void dfs(Point);
void bfs(Point);
double dis(Point, Point);

Mat pic = imread("ps1.jpg",1);
Mat img(pic.rows, pic.cols, CV_8UC3, Scalar(0,0,0));
Mat path=imread("ps1.jpg",1);
vector<Point> pp;
char pre[200][200] = {0};

int main(int argc, char **argv)
{
  //cvtColor(pic, img, CV_BGR2GRAY);
  //cvtColor(pic, path, CV_BGR2GRAY);
  Point pt1;
  pt.x =  31;
  pt.y = 27;
  ptf.x = 164;
  ptf.y = 170;
  int flag =0;

  for(int i=pt.x;;i++)
    {
      for(int j=pt.y;;j++)
	{
	  if (pic.at<Vec3b>(i, j)[0] == 0 && pic.at<Vec3b>(i,j)[1]== 0 && pic.at<Vec3b>(i,j)[2]== 0)
	    {
	      flag=1;
	      pt1.x=i;
	      pt1.y=j;
	      break;
	    }
	}
      if(flag==1)
	break;
    }
  
  bfs(pt1);
  ros::init(argc, argv, "talker"); 
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Point>("chatter", 1000);
  ros::Rate loop_rate(10);
int k=0;
while(k<pp.size())
{
geometry_msgs:: Point msg;
    msg.x = pp[k].x;
    msg.y = pp[k].y ;
    cout<<"..."<<msg.x<<" "<<msg.y<<endl;
    chatter_pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
    k++;
}
  

 /* imshow("Path", pic);
  waitKey(0);*/
  return 0;
}

bool isWhite(int &xx,int &yy){
  return (pic.at<Vec3b>(xx,yy)[0] >= 250 && pic.at<Vec3b>(xx,yy)[1] >= 250 && pic.at<Vec3b>(xx,yy)[2] >= 250) ;
};

bool isGreen(int &xx,int &yy){
  return (pic.at<Vec3b>(xx,yy)[0] <=10 && pic.at<Vec3b>(xx,yy)[1] >= 250 && pic.at<Vec3b>(xx,yy)[2] <= 10) ;
};

void bfs(Point p){
  queue<Point> Q;
  Q.push(p);
  Point t;
  int i,j;
  while(!Q.empty()){
    t = Q.front(),Q.pop();

    if(isGreen(t.x,t.y)){
      break;
    }

    for(i=t.x+1;i>=t.x-1;i--)
      for(j=t.y+1;j>=t.y-1;j--)
	if( ( (0<i && i<pic.rows) && (0<j && j<pic.cols)) && pre[i][j]==0 && !isWhite(i,j))
	  {
	    Q.push(Point(i,j));
	    if(i<t.x){
	      if(j<t.y)
		pre[i][j]=9;
	      else if(j==t.y)
		pre[i][j]=6;
	      else
		pre[i][j]=3;
	    }else if (i==t.x){

	      if(j<t.y)
		pre[i][j]=8;
	      else if(j>t.y)
		pre[i][j]=2;

	    }else{
	      
	      if(j<t.y)
		pre[i][j]=7;
	      else if(j==t.y)
		pre[i][j]=4;
	      else
		pre[i][j]=1;
	    }

      }

  }

  i=t.x,j=t.y;
  while( !(i==p.x && j==p.y) ){   // not start point
pp.push_back(Point(i,j));
    pic.at<Vec3b>(i,j)[0]=(255,255,255);
    switch(pre[i][j]){
    case 1:{
      i--,j--;
    }break;
    case 2:{
      j--;
    }break;
    case 3:{
      i++,j--;
    }break;
    case 4:{
      i--;
    }break;
    case 6:{
      i++;
    }break;
    case 7:{
      i--,j++;
    }break;
    case 8:{
      j++;
    }break;
    case 9:{
      i++,j++;
    }break;

    }
  }
}

/*

double dis(Point a,Point b)
{
  double dis=sqrt(pow((a.x-b.x),2)+pow((a.y-b.y),2));
  return dis;
}

void dfs(Point start)
{
  stack<Point>s;
  s.push(start);
  Point u;
  Point temp;
  
  while (!s.empty())
    {
      u = s.top();s.pop();
      cout << u.x << ',' << u.y << endl;
      if (pic.at<Vec3b>(u.x, u.y)[0] == 0 && pic.at<Vec3b>(u.x, u.y)[1]== 0 && pic.at<Vec3b>(u.x, u.y)[2]== 0)//black pixel
	{
	  pic.at<Vec3b>(u.x, u.y)=(255,255,255);
	  visited[u.x][u.y] = 1;
	}
      double dist = 0.0, min = 1000.0;int flag=0,a,b;

      for (a = u.x - 1; a <= u.x + 1; a++)
	{
	  for (b = u.y - 1; b <= u.y + 1; b++)
	    {
	      if(pic.at<Vec3b>(a,b)[1]>=250)
		{
		  //line(path,u,ptf,(255,255,255),1,1);
		  flag=1;
		  break;
		}
	    }
	  if(flag==1)
	    break;
	  if ( a >= 0 && b >= 0 && a < pic.rows && b < pic.cols && visited[a][b] == 0 && 
	       (pic.at<Vec3b>(u.x, u.y)[0] == 0 && pic.at<Vec3b>(u.x, u.y)[1] == 0 && pic.at<Vec3b>(u.x, u.y)[2] == 0) )
	    {
	      Point p;
	      p.x = a;
	      p.y = b;
	      s.push(p);
	      	      
	      dist = dis(p, ptf);
	      if (dist < min)
		{
		  min=dist;
		  temp=p; 
		}
	    }
	}
      if(flag==1)
	break;
      //s.push(temp);
      //line(path,u,temp,(255,255,255),1,1);
    }
 
}
*/
