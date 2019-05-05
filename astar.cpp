#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <stdlib.h>

#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

vector<vector<int>> gridTopo(956, vector<int>(0));  //input array randam col and row
vector<vector<int>> request(4001, vector<int>(0));  //input array randam col and row
vector<vector<int>> result(2001, vector<int>(0));  //input array randam col and row
vector<vector<int>> simpRes(1000, vector<int>(0));
int restcap[956][2];


void reverse_with_index(vector<vector<int>> vec);
void gridtopoToMemory();
void resultToMemory();
void requestToMemory();
int costCalc();
bool cmp(const vector<int> &a, const vector<int> &b);
//void simplifyRes();
void FindPath();
void choosePath();
void restcapfill();

void astar();

int main()
{
	gridtopoToMemory();
	cout << "success to read gridtopo" << endl;
	//resultToMemory();
	requestToMemory();
	cout << "success to read request" << endl;


	time_t start, stop;
	start = clock();
	//restcapfill();
	astar();

	//choosePath();
	//simplifyRes();
    //	sort(simpRes.begin(), simpRes.end(), cmp);

	//costCalc()

	//FindPath();
	//reverse_with_index(simpRes);
	stop = clock();
	cout << "The run time is: " << (double)( stop - start) / CLOCKS_PER_SEC << "s" << endl;
	cout << "ALL OK!!!!" << endl;
	getchar();
	//return 0;
}

void astar()
{
	vector<vector<int>> way(600, vector<int>(0));
	int risknum = 600;

	int findnowayflag = 0;
	int waycycleflag = 0;

	restcap[0][0] = 955;
	restcap[0][1] = 955;
	for (int i = 1; i < 956; i++)
		{
			restcap[i][0] = gridTopo[i][2] * 0.8;
			restcap[i][1] = gridTopo[i][2] * 0.8;
		}

	for (int i = 1; i <= 3997; i += 4)
				{
					int len = request[i + 1].size();
					int j=i/4;
					simpRes[j].push_back(request[i][0]);  //ID
					simpRes[j].push_back(request[i][1]);  //weight
					simpRes[j].push_back(request[i + 1][0]);  //start point
					simpRes[j].push_back(request[i + 1][len - 1]);  //end point
				}
			//cout << simpRes[0][0] <<" "<< simpRes[0][1] << " " << simpRes[0][2] << " " << simpRes[0][3] << endl;
			//cout << simpRes[999][0] << " " << simpRes[999][1] << " " << simpRes[999][2] << " " << simpRes[999][3] << endl;
	sort(simpRes.begin(), simpRes.end(), cmp);

			for (int riskid = 0; riskid < risknum; riskid++)
			{
				int weight = simpRes[riskid][1];
				int startp = simpRes[riskid][2];

				int endp = simpRes[riskid][3];
				int endrow = endp / 20;
				int endcol = endp - 20 * endrow;
				cout <<  weight << " " << startp << " " << endp <<endl;
				int curpos = startp;
			    int curposrow = curpos / 20;
				int curposcol = curpos - 20 * curposrow;

				if (findnowayflag == 1)
				{
					break;
				}

				int dirlimitflag = -1;


				way[riskid].push_back(curpos);
				int count = 0;
				while (curpos != endp)
				{
					count++;
					if (count >= 100)
					{
						cout << "找了100次还没找到？" << endl;
						findnowayflag = 1;
						break;
					}


					int mindis = -666666;
					int mincost = 0;
					int mincap = 0;

					int distancetemp = 0;
					int nextdir = -1;

					int leftcost = 0;
					int rightcost = 0;
					int upcost = 0;
					int downcost = 0;

					int linkcaptemp = 0;

					int discostsum[4][4];
					discostsum[0][0] = 0;
					discostsum[1][0] = 0;
					discostsum[2][0] = 0;
					discostsum[3][0] = 0;

					//移动之前行数之差和列数之差
					int percoldis, perrowdis;
					if (endcol >= curposcol)
					{
						percoldis = endcol - curposcol;
					}
					else
					{
						percoldis = curposcol - endcol;
					}
					if (endrow >= curposrow)
					{
						perrowdis = endrow - curposrow;
					}
					else
					{
						perrowdis = curposrow - endrow;
					}

					//Towards the left
					if (curposcol >= 1)
					{
						discostsum[0][0] = 1;

						int currowdis;
						int curposcoltemp = curposcol - 1;

						if (endcol >= curposcoltemp)
						{
							currowdis = endcol - curposcoltemp;
						}
						else
						{
							currowdis = curposcoltemp - endcol;
						}
						distancetemp = percoldis - currowdis;  //1 or -1
						leftcost = gridTopo[39 * curposrow + 2 * curposcoltemp + 1][3];
						//linkcaptemp = gridTopo[39 * curposrow + 2 * curposcoltemp + 1][2];
						linkcaptemp = restcap[39 * curposrow + 2 * curposcol - 1][1];

						discostsum[0][1] = distancetemp;
						discostsum[0][2] = leftcost;
						discostsum[0][3] = linkcaptemp;

						if (linkcaptemp < weight)
						{
							discostsum[0][0] = 0;
						}
					}
					cout << " 向左的权限是 " << discostsum[0][0] << endl;
					cout << " 向左的距离和成本 " << endl;
					cout << distancetemp << "  " << leftcost << "  " << linkcaptemp << endl;

					//Towards the right
					if (curposcol <= 18)
					{
						discostsum[1][0] = 1;

						int  curcoldis;
						int curposcoltemp = curposcol + 1;
						if (endcol >= curposcoltemp)
						{
							curcoldis = endcol - curposcoltemp;
						}
						else
						{
							curcoldis = curposcoltemp - endcol;
						}
						distancetemp = percoldis - curcoldis;  //1 or -1
						rightcost = gridTopo[39 * curposrow + 2 * curposcol + 1][3];
						//linkcaptemp = gridTopo[39 * curposrow + 2 * curposcol + 1][2];
						linkcaptemp = restcap[39 * curposrow + 2 * curposcol + 1][0];

						discostsum[1][1] = distancetemp;
						discostsum[1][2] = rightcost;
						discostsum[1][3] = linkcaptemp;

						if (linkcaptemp < weight)
						{
							discostsum[1][0] = 0;
						}
					}
					cout << " 向you的权限是 " << discostsum[1][0] << endl;
					cout << " 向右的距离和成本  " << endl;
					cout << distancetemp << "  " << rightcost << "  " << linkcaptemp << endl;


					//Towards the up
					if (curposrow >= 1)
					{
						discostsum[2][0] = 1;

						int  currowdis;
						int currowtemp = curposrow - 1;
						if (endrow >= currowtemp)
						{
							currowdis = endrow - currowtemp;
						}
						else
						{
							currowdis = currowtemp - endrow;
						}
						distancetemp = perrowdis - currowdis;  //1 or -1
						upcost = gridTopo[39 * (curposrow - 1) + 2 * (curposcol + 1)][3];
						//linkcaptemp = gridTopo[39 * (curposrow - 1) + 2 * (curposcol + 1)][2];
						linkcaptemp = restcap[39 * (curposrow - 1) + 2 * (curposcol + 1)][1];

						discostsum[2][1] = distancetemp;
						discostsum[2][2] = upcost;
						discostsum[2][3] = linkcaptemp;

						if (linkcaptemp < weight)
						{
							discostsum[2][0] = 0;
						}
					}
					cout << " 向shang的权限是 " << discostsum[2][0] << endl;
					cout << " 向上的距离和成本 " << endl;
					cout << distancetemp << "  " << upcost << "  " << linkcaptemp << endl;

					//Towards the down
					if (curposrow <= 23)
					{
						discostsum[3][0] = 1;
						int currowdis;
						int currowtemp = curposrow + 1;
						if (endrow >= currowtemp)
						{
							currowdis = endrow - currowtemp;
						}
						else
						{
							currowdis = currowtemp - endrow;
						}
						distancetemp = perrowdis - currowdis;  //1 or -1
						downcost = gridTopo[39 * curposrow + 2 * (curposcol + 1)][3];
						//linkcaptemp = gridTopo[39 * curposrow + 2 * (curposcol + 1)][2];
						linkcaptemp = restcap[39 * curposrow + 2 * (curposcol + 1)][0];



						discostsum[3][1] = distancetemp;
						discostsum[3][2] = downcost;
						discostsum[3][3] = linkcaptemp;

						if (linkcaptemp < weight)
						{
							discostsum[3][0] = 0;
						}
					}
					cout << " 向xia的权限是 " << discostsum[3][0] << endl;
					cout << " 向下的距离和成本 " << endl;
					cout << distancetemp << "  " << downcost << "  " << linkcaptemp << endl;

					//加入回头草方向限制
					switch (dirlimitflag)
					{
					case 0:
						discostsum[0][0] = 0;
						break;
					case 1:
						discostsum[1][0] = 0;
						break;
					case 2:
						discostsum[2][0] = 0;
						break;
					case 3:
						discostsum[3][0] = 0;
						break;
					default:
						break;
					}
					//比较路径
					for (int i = 0; i < 4; i++)
					{
						if (discostsum[i][0] == 1)
						{
							if (mindis < discostsum[i][1])
							{
								mindis = discostsum[i][1];
								mincost = discostsum[i][2];
								mincap = discostsum[i][3];
								nextdir = i;
							}
							else if (mindis == discostsum[i][1])  //靠近距离一样 比成本
							{
								if (mincost > discostsum[i][2])
								{
									mindis = discostsum[i][1];
									mincost = discostsum[i][2];
									mincap = discostsum[i][3];
									nextdir = i;
								}
								else if (mincost == discostsum[i][2])
								{
									if (mincap < discostsum[i][3])
									{
										mindis = discostsum[i][1];
										mincost = discostsum[i][2];
										mincap = discostsum[i][3];
										nextdir = i;
									}

								}
							}
						}

					}

					//cout << " 下个方向是 " << nextdir << endl;
					//cout << " 最大进步距离 " << mindis << endl;
					//cout << " 最小成本是 " << mincost << endl;
					//cout << " 最大带宽是 " << mincap << endl;
					if (nextdir == -1)
						printf("哪个方向都不行。。。。。");
					else
					{
						// 左右上下
						switch (nextdir)
						{
						case 0:
							curposcol -= 1;
							break;
						case 1:
							curposcol += 1;
							break;
						case 2:
							curposrow -= 1;
							break;
						case 3:
							curposrow += 1;
							break;
						default:
							break;
						}
					}

					curpos = curposrow * 20 + curposcol;
					cout << " 下个点是 " << curpos << endl;

					//出现了路径循环
					//for (auto itre = way[riskid].begin(); itre!= way[riskid].end(); ++itre)
					//{
					//	if (curpos == *itre)
					//	{
					//		waycycleflag = 1;
					//		//switch (nextdir)
					//		//{
					//		//case 0:
					//		//	rightlimitflag = 1;
					//		//	break;
					//		//case 1:
					//		//	leftlimitflag = 1;
					//		//	break;
					//		//case 2:
					//		//	downlimitflag = 1;
					//		//	break;
					//		//case 3:
					//		//	uplimitflag = 1;
					//		//	break;
					//		//default:
					//		//	break;
					//		//} 
					//	}
					//}

					//这次向左 下次就不能向右
					switch (nextdir)
					{
					case 0:
						dirlimitflag = 1;
						break;
					case 1:
						dirlimitflag = 0;
						break;
					case 2:
						dirlimitflag = 3;
						break;
					case 3:
						dirlimitflag = 2;
						break;
					default:
						break;
					} 

						way[riskid].push_back(curpos);
				}

				//update rescaptable

			
			}

			for (int i = 0; i < way.size(); i++)
			{
				for (int j = 0; j < way[i].size(); j++)
					cout << way[i][j] << " ";
				cout << endl;
			}

	
}
//PS：print items of 2D array
void reverse_with_index(vector<vector<int>> vec)
{
	if (vec.empty())
	{
		cout << "The vector is empty!" << endl;
		return;
	}

	int i, j;
	cout << "Use index : " << endl;
	for (i = 0; i < vec.size(); i++)
	{
		for (j = 0; j < vec[i].size(); j++)
			cout << vec[i][j] << " ";
		cout << endl;
	}
}

void  gridtopoToMemory()
{

	ifstream sumf("gridtopo.txt");

	if (!sumf)
		{
		  cout << "fail to open the file" << endl;
		}
	else
		{
		  cout << "open the file successfully" << endl;
		}

	const char *delim = " ";
	char *partline;
	std::string s;

	//datainsum.resize(4957); 
	int rownum = 0;  //row numer of input array

	while (getline(sumf, s))
	{
	//	cout << "read from file: " << s << endl;

		//string to const char*
		const char* readlinetemp = s.data();
		//const char* to char*
		char readline[1000];
		strcpy(readline, readlinetemp);

		vector<int> datainrow; 

	    partline = strtok(readline, delim); //将分隔符（空格）换成\0输出，完成分割任务
		while (partline)
		{
				datainrow.push_back(atoi(partline));  // ascii  to int
				partline = strtok(NULL, delim);  //接着上面的地址指针
		}

		//for (int i = 0; i<datainrow.size(); i++)
		//	cout << datainrow.at(i) << endl;

		int colnum = datainrow.size();
		//datainsum[rownum].resize(colnum);
		for (int j = 0; j<colnum; j++)
			gridTopo[rownum].push_back(datainrow[j]);

		rownum++;
	}	

}

void  requestToMemory()
{

	ifstream sumf("request.txt");

	if (!sumf)
	{
		cout << "fail to open the file" << endl;
	}
	else
	{
		cout << "open the file successfully" << endl;
	}


	const char *delim = " ";
	char *partline;
	std::string s;

	//datainsum.resize(4957); 
	int rownum = 0;  //row numer of input array

	while (getline(sumf, s))
	{
//		cout << "read from file: " << s << endl;

		//string to const char*
		const char* readlinetemp = s.data();
		//const char* to char*
		char readline[1000];
		strcpy(readline, readlinetemp);

		vector<int> datainrow;

		partline = strtok(readline, delim); //将分隔符（空格）换成\0输出，完成分割任务
		while (partline)
		{
			datainrow.push_back(atoi(partline));  // ascii  to int
			partline = strtok(NULL, delim);  //接着上面的地址指针
		}

		//for (int i = 0; i<datainrow.size(); i++)
		//	cout << datainrow.at(i) << endl;

		int colnum = datainrow.size();
		//datainsum[rownum].resize(colnum);
		for (int j = 0; j<colnum; j++)
			request[rownum].push_back(datainrow[j]);

		rownum++;
	}

}

void  resultToMemory()
{

	ifstream sumf("result.txt");

	if (!sumf)
	{
		cout << "fail to open the file" << endl;
	}
	else
	{
		cout << "open the file successfully" << endl;
	}


	const char *delim = " ";
	char *partline;
	std::string s;

	//datainsum.resize(4957); 
	int rownum = 0;  //row numer of input array

	while (getline(sumf, s))
	{
		cout << "read from file: " << s << endl;

		//string to const char*
		const char* readlinetemp = s.data();
		//const char* to char*
		char readline[1000];
		strcpy(readline, readlinetemp);

		vector<int> datainrow;

		partline = strtok(readline, delim); //将分隔符（空格）换成\0输出，完成分割任务
		while (partline)
		{
			datainrow.push_back(atoi(partline));  // ascii  to int
			partline = strtok(NULL, delim);  //接着上面的地址指针
		}

		//for (int i = 0; i<datainrow.size(); i++)
		//	cout << datainrow.at(i) << endl;

		int colnum = datainrow.size();
		//datainsum[rownum].resize(colnum);
		for (int j = 0; j<colnum; j++)
			result[rownum].push_back(datainrow[j]);

		rownum++;
	}

}

//cost calc
int costCalc()
{

	int j = 0; // business routh
	int k = 1;  //link of two points

	int consumpsum = 0;
	int weight = 0; 
	int resultLen = 2000;


	for (j = 2; j <= resultLen; j += 2)  //paths
	{
		weight = result[j - 1][1];
		int consumptemp = 0;
		int oilcost = 0;
		for (auto itre = result[j].begin(); (itre + 1) != result[j].end(); ++itre)  // consumption of each path
		{
			cout << *itre << " " << *(itre + 1) << endl;
			for (int k = 1; k <= 955; k++)  //links
			{
				//Link is bidirectional  so.....
				if ((*itre == gridTopo[k][0] && *(itre + 1) == gridTopo[k][1]) || 
					(*itre == gridTopo[k][1] && *(itre + 1) == gridTopo[k][0]))
				{
					cout << k << endl;   //position in grid
					cout << gridTopo[k][3] << endl;   //  oilcost part
					oilcost += gridTopo[k][3];
				}

			}
		
		}
//		cout << oilcost << endl;
		consumptemp += weight * oilcost;
		consumpsum += consumptemp;
	
	}

    cout << consumpsum << endl;
	return 0;
}

//simplify the request 
//void simplifyRes()
//{
//		int j = 0;
//		for (int i = 1; i <= 3997; i += 4)
//		{
//			int len = request[i + 1].size();
//			
//			simpRes[j].push_back(request[i][0]);  //ID
//			simpRes[j].push_back(request[i][1]);  //weight
//			simpRes[j].push_back(request[i + 1][0]);  //start point
//			simpRes[j].push_back(request[i + 1][len - 1]);  //end point
//			j++;
//		}
//
//}

// sort array 
bool cmp(const vector<int> &a, const vector<int> &b) 
{
	return a[1] > b[1];
}

void FindPath()
{
	int startp, endp, posing;
	startp = 36;
	endp = 400;
	posing = startp;
	vector<int> pathtemp;

	vector<vector<int>> nextptemp(4, vector<int>(0));
	int count = 0;
	while (count < 500)
	{
		int nextflag = 0;
		for (int j = 1; j < 956; j++)
		{
			if (gridTopo[j][0] == posing && gridTopo[j][1] <= endp)
			{
				nextptemp[0].push_back(gridTopo[j][0]);
				nextptemp[1].push_back(gridTopo[j][1]);
				nextptemp[2].push_back(gridTopo[j][2]);
				nextptemp[3].push_back(gridTopo[j][3]);
				nextflag = 1;
			}
		}
		if (!nextflag)
			break;

		posing = nextptemp[1][0];
		for (int k = 0; k < (nextptemp[0].size()-1); k++)
		{
			if (nextptemp[3][k + 1] / (nextptemp[1][k + 1] - nextptemp[0][k + 1])
				< nextptemp[3][k] / (nextptemp[1][k] - nextptemp[0][k]) )
			{
				posing = nextptemp[1][k+1];
			}
		}
		for (int i = 0; i<4; i++) nextptemp[i].clear();

		pathtemp.push_back(posing);
		count++;
	}

	for (int i = 0; i<pathtemp.size(); i++)
		cout << pathtemp.at(i) << endl;

}

//
//void choosePath()
//{
//	int j = 0;
//	int linkcaptemp;
//	int maxlinkcap=0;
//	bool firstflag = 0;
//	int waynum = 3;
//	int bestwayid = 0;
//	int weight = 0;
//	int risknum = 1;
//	int oilcost = 0;
//
//	for (int riskid = 0; riskid < risknum; riskid++)
//	{
//		weight = request[riskid*4+1][1];
//		cout << weight << endl;
//		j = riskid * 4 + 2;
//		maxlinkcap = 0;
//		bestwayid = 0;
//	//	cout << "这是第" << riskid << "个任务！！！！" << endl;
//
//		//to find the best way
//		for (int m = 0; m < waynum; m++)
//		{
//			firstflag = 0;
//			linkcaptemp = 0;
//			oilcost = 0;
//
//			for (auto itre = request[j + m].begin(); (itre + 1) != request[j + m].end(); ++itre)  // consumption of each path
//			{
//			//	cout << *itre << " " << *(itre + 1) << endl;
//				for (int k = 1; k <= 955; k++)  //links
//				{
//					//Link is bidirectional  so.....
//					if ((*itre == gridtopo[k][0] && *(itre + 1) == gridtopo[k][1]) ||
//						(*itre == gridtopo[k][1] && *(itre + 1) == gridtopo[k][0]))
//					{
//						oilcost += gridtopo[k][3];
//
//						if (firstflag == 0)
//						{
//							if (*itre == gridtopo[k][0] && *(itre + 1) == gridtopo[k][1])
//								linkcaptemp = restcap[k][0];
//							else
//								linkcaptemp = restcap[k][1];
//						}
//						firstflag = 1;
//
//						if (*itre == gridtopo[k][0] && *(itre + 1) == gridtopo[k][1])
//						{
//							if (firstflag == 1 && linkcaptemp > restcap[k][0])
//							{
//								linkcaptemp = restcap[k][0];
//							}
//						}
//						else
//						{
//							if (firstflag == 1 && linkcaptemp > restcap[k][1])
//							{
//								linkcaptemp = restcap[k][1];
//							}
//						}
//					//	cout << linkcaptemp << endl;
//
//					}
//
//				}
//
//			}
//			if (maxlinkcap < linkcaptemp)
//			{
//				maxlinkcap = linkcaptemp;
//				bestwayid = m;
//			}
//
//			costable[riskid][3] = bestwayid;
//			costable[riskid][m] = oilcost;
////			cout << "到现在为止最好的路是" << bestwayid << endl;
////			cout << "最好的路中最小的链路带宽是" << maxlinkcap << endl;
//		}
//
//
//		//找到最佳路径后  update the rest capicity 
//		for (auto itre = request[j + bestwayid].begin(); (itre + 1) != request[j + bestwayid].end(); ++itre)
//		{
//			for (int k = 1; k <= 955; k++)  //links
//			{
//				//Link is bidirectional  so.....
//				if ((*itre == gridtopo[k][0] && *(itre + 1) == gridtopo[k][1]))
//				{
//					restcap[k][0] = restcap[k][0] - weight;
//
//				}
//
//				if (*itre == gridtopo[k][1] && *(itre + 1) == gridtopo[k][0])
//				{
//					restcap[k][1] = restcap[k][1] - weight;
//
//				}
//
//			}
//		}
//	
//	}
//
//}

void choosePath()
{
	int risknum = 1000;
	int weight = 0;
	int waynum = 3;
	bool dirflag = true;  //direction flag
	bool findlink1flag;
	bool findlink2flag;
	int lowgate = 0;
	int highgate = 955;
	int middle = 0;

	int count = 0;
	int linkcaptemp;
	int oilcost;
	int totalcost=0;
    int minlinkcap=0;
	bool firstlinkflag;
	int maxmincap = 0;
    int bestwayid = 0;
	int costminwayid = 0;
	int costsecminid = 0;
	bool gloansflag;
	bool capoutranflag;
	bool capoutsecminflag;
	int restcap[956][2];
	int costable[1000][6];

	vector<vector<int>> way0linksto(2000, vector<int>(0));
	vector<vector<int>> way1linksto(2000, vector<int>(0));
	vector<vector<int>> way2linksto(2000, vector<int>(0));

	vector<vector<int>> simpRes(1000, vector<int>(0));

	//fill the restcap set
	restcap[0][0] = 955;
	restcap[0][1] = 955;
	for (int i = 1; i < 956; i++)
	{
		restcap[i][0] = gridTopo[i][2] * 0.8;
		restcap[i][1] = gridTopo[i][2] * 0.8;
	}

	//find the link cap 
	for (int riskid = 0; riskid < risknum; riskid++)
	{
	//	cout << "这是第" << riskid << "个任务" << endl;
		weight = request[riskid * 4 + 1][1];
		int j = riskid * 4 + 2;

		maxmincap = 0;


		for (int m = 0; m < waynum; m++)
		{
			oilcost = 0;
			minlinkcap = 0;
			firstlinkflag = true;

			for (auto itre = request[j + m].begin(); (itre + 1) != request[j + m].end(); ++itre) 
			{
				if (*itre < *(itre + 1))
					dirflag = true;
				else
					dirflag = false;

				findlink1flag = false;
				findlink2flag = false;
				lowgate = 0;
				highgate = 955;
				middle = 0;

				//找到每个link
				if (dirflag)
				{
					while(!findlink1flag)
					{
						middle = (lowgate + highgate) / 2;
						if (*itre == gridTopo[middle][0])
						{

							count = 0;
							while (!findlink2flag)
							{
								if (*(itre + 1) == gridTopo[middle][1])
								{
									linkcaptemp = restcap[middle][0];
									findlink2flag = true;
									oilcost += gridTopo[middle][3];
								}
								else if (*(itre + 1) > gridTopo[middle][1])
								{
									middle = middle + 1;
								}
								else
								{
									middle = middle - 1;
								}

								count++;
								if (count >= 100)
								{
									cout << *itre << "找到了，but" << *(itre + 1) << "不见了呢！" << endl;
									count = 0;
									findlink2flag = true;
								}
							}

						}
						else if (*itre > gridTopo[middle][0])
						{
							lowgate = middle + 1;
						}
						else
						{
							highgate = middle - 1;
						}

						if (lowgate > highgate)
						{
							findlink1flag = true;
							cout << "link 的第一节点丢了！" << endl;
						}
						if (findlink2flag == true)
						{
							findlink1flag = true;
						}
					}

				}
				else if (!dirflag)
				{
					while (!findlink1flag)
					{
						middle = (lowgate + highgate) / 2;
						if (*(itre+1) == gridTopo[middle][0])
						{

							count = 0;
							while (!findlink2flag)
							{
								if (*itre == gridTopo[middle][1])
								{
									linkcaptemp = restcap[middle][1];
									findlink2flag = true;
									oilcost += gridTopo[middle][3];
								}
								else if (*itre > gridTopo[middle][1])
								{
									middle = middle + 1;
								}
								else
								{
									middle = middle - 1;
								}

								count++;
								if (count >= 100)
								{
									cout << *(itre+1) << "第二个点先找到了，but" << *itre << "不见了呢！" << endl;
									count = 0;
									findlink2flag = true;
								}
							}

						}
						else if (*(itre+1) > gridTopo[middle][0])
						{
							lowgate = middle + 1;
						}
						else
						{
							highgate = middle - 1;
						}

						if (lowgate > highgate)
						{
							findlink1flag = true;
							cout << "link 的第一节点丢了！" << endl;
						}
						if (findlink2flag == true)
						{
							findlink1flag = true;
						}
					}
				}

				//每个链路的方向和位置保存
				if (m == 0)
				{				
					way0linksto[2* riskid].push_back(dirflag);
					way0linksto[2* riskid+1].push_back(middle);
				}
				else if (m == 1)
				{
					way1linksto[2 * riskid].push_back(dirflag);
					way1linksto[2 * riskid+1].push_back(middle);
				}
				else
				{
					way2linksto[2 * riskid].push_back(dirflag);
					way2linksto[2 * riskid+1].push_back(middle);
				}

				//找到最小link带宽
				if (firstlinkflag == true)
				{
					minlinkcap = linkcaptemp;
					firstlinkflag = false;
				}
				else
				{
					if (minlinkcap > linkcaptemp)
					{
						minlinkcap = linkcaptemp;
					}
				}
			}


			if (maxmincap < minlinkcap)
			{
				maxmincap = minlinkcap;
				bestwayid = m;
			}
		//	cout << "这条路成本是"<<oilcost << endl;
		//	cout << "这条路的最小带宽是"<< minlinkcap << endl;
		//	cout << "最好的路是" << bestwayid << endl;
			
			costable[riskid][m] = oilcost*weight;

		}
		costable[riskid][3] = bestwayid;

		if (maxmincap < 0)
		{
			gloansflag = false;
		}

		//update restcap
		switch (bestwayid)
		{
			int waylinkcount;
			case 0:
				waylinkcount = 0;
				for (auto itre = way0linksto[2 * riskid].begin(); itre != way0linksto[2 * riskid].end(); ++itre)
				{
				    int poi = way0linksto[2 * riskid+1][waylinkcount];

	//				cout << "任务重量是" << weight << endl;
					if (*itre == true)
					{
						restcap[poi][0] = restcap[poi][0] - weight;
					}
					else
					{
						restcap[poi][1] = restcap[poi][1] - weight;
					}
					waylinkcount++;
				}
				break;
			case 1:
				waylinkcount = 0;
				for (auto itre = way1linksto[2 * riskid].begin(); itre != way1linksto[2 * riskid].end(); ++itre)
				{
					int poi = way1linksto[2 * riskid+1][waylinkcount];
					if (*itre == true)
					{
						restcap[poi][0] = restcap[poi][0] - weight;
					}
					else
					{
						restcap[poi][1] = restcap[poi][1] - weight;
					}
					waylinkcount++;
				}
				break;
			case 2:
				waylinkcount = 0;
				for (auto itre = way2linksto[2 * riskid].begin(); itre != way2linksto[2 * riskid].end(); ++itre)
				{
					int poi = way2linksto[2 * riskid+1][waylinkcount];
					if (*itre == true)
					{
						restcap[poi][0] = restcap[poi][0] - weight;
					}
					else
					{
						restcap[poi][1] = restcap[poi][1] - weight;
					}
					waylinkcount++;
				}
				break;
			default:
				break;
		}

	}
	
	//calc costminwayid
	for (int n = 0; n < risknum; n++)
	{
		costminwayid = 0;
		int mincost = costable[n][0];
		if (mincost > costable[n][1])
		{
			mincost = costable[n][1];
			costminwayid =1;
		}
		if (mincost > costable[n][2])
		{
			mincost = costable[n][2];
			costminwayid = 2;
		}
		costable[n][4] = costminwayid;

		//find second costmin
		costsecminid = 0;
		switch (costminwayid)
		{
		case 0:
			if (costable[n][1] > costable[n][2])
			{
				costsecminid = 2;
			}
			else
			{
				costsecminid = 1;
			}
			break;
		case 1:
			if (costable[n][0] > costable[n][2])
			{
				costsecminid = 2;
			}
			else
			{
				costsecminid = 0;
			}
			break;
		case 2:
			if (costable[n][0] > costable[n][1])
			{
				costsecminid = 1;
			}
			else
			{
				costsecminid = 0;
			}
			break;
		default:
			break;
		}
		
		costable[n][5] = costsecminid;

//		cout << "成本最小的是" << costable[n][4]<<endl;
//		cout << "成本cicci小的是" << costable[n][5] << endl;
	}

	//sort id by weight
	for (int i = 1; i <= 3997; i += 4)
	{
		int riskid = request[i][0];
		simpRes[riskid].push_back(request[i][0]);  //ID
		simpRes[riskid].push_back(request[i][1]);  //weight
	}
	sort(simpRes.begin(), simpRes.end(), cmp);


	//replace the bestway with mincostway
	for (int n = 0; n < risknum; n++)
	{
		int riskid = simpRes[n][0];
	//	cout << "现在的任务号是" << riskid << endl;
		weight = request[riskid * 4 + 1][1];
		capoutranflag = false;
		capoutsecminflag = false;
		if (costable[riskid][3] != costable[riskid][4])
		{

			switch (costable[riskid][3])
			{
				int waylinkcount;
				case 0:
					waylinkcount = 0;
					for (auto itre = way0linksto[2 * riskid].begin(); itre != way0linksto[2 * riskid].end(); ++itre)
					{
						int poi = way0linksto[2 * riskid + 1][waylinkcount];

						//				cout << "任务重量是" << weight << endl;
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] + weight;
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] + weight;
						}
						waylinkcount++;
					}
					break;
				case 1:
					waylinkcount = 0;
					for (auto itre = way1linksto[2 * riskid].begin(); itre != way1linksto[2 * riskid].end(); ++itre)
					{
						int poi = way1linksto[2 * riskid + 1][waylinkcount];
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] + weight;
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] + weight;
						}
						waylinkcount++;
					}
					break;
				case 2:
					waylinkcount = 0;
					for (auto itre = way2linksto[2 * riskid].begin(); itre != way2linksto[2 * riskid].end(); ++itre)
					{
						int poi = way2linksto[2 * riskid + 1][waylinkcount];
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] + weight;
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] + weight;
						}
						waylinkcount++;
					}
					break;
				default:
					break;
			}

			switch (costable[riskid][4])
			{
				int waylinkcount;
			case 0:
				waylinkcount = 0;
				for (auto itre = way0linksto[2 * riskid].begin(); itre != way0linksto[2 * riskid].end(); ++itre)
				{
					int poi = way0linksto[2 * riskid + 1][waylinkcount];

					//				cout << "任务重量是" << weight << endl;
					if (*itre == true)
					{
						restcap[poi][0] = restcap[poi][0] - weight;
						if (restcap[poi][0] < 0)
						{
							capoutranflag = true;
						}
					}
					else
					{
						restcap[poi][1] = restcap[poi][1] - weight;
						if (restcap[poi][1] < 0)
						{
							capoutranflag = true;
						}
					}
					waylinkcount++;
				}
				break;
			case 1:
				waylinkcount = 0;
				for (auto itre = way1linksto[2 * riskid].begin(); itre != way1linksto[2 * riskid].end(); ++itre)
				{
					int poi = way1linksto[2 * riskid + 1][waylinkcount];
					if (*itre == true)
					{
						restcap[poi][0] = restcap[poi][0] - weight;
						if (restcap[poi][0] < 0)
						{
							capoutranflag = true;
						}
					}
					else
					{
						restcap[poi][1] = restcap[poi][1] - weight;
						if (restcap[poi][1] < 0)
						{
							capoutranflag = true;
						}
					}
					waylinkcount++;
				}
				break;
			case 2:
				waylinkcount = 0;
				for (auto itre = way2linksto[2 * riskid].begin(); itre != way2linksto[2 * riskid].end(); ++itre)
				{
					int poi = way2linksto[2 * riskid + 1][waylinkcount];
					if (*itre == true)
					{
						restcap[poi][0] = restcap[poi][0] - weight;
						if (restcap[poi][0] < 0)
						{
							capoutranflag = true;
						}
					}
					else
					{
						restcap[poi][1] = restcap[poi][1] - weight;
						if (restcap[poi][1] < 0)
						{
							capoutranflag = true;
						}
					}
					waylinkcount++;
				}
				break;
			default:
				break;
			}
		}

		//cap out of range then withdraw oprations
		if (capoutranflag == true)
		{

		//	cout << "这是第" << riskid << "个任务尝试最低替换out!!!" << endl;
			switch (costable[riskid][4])
			{
				int waylinkcount;
			case 0:
				waylinkcount = 0;
				for (auto itre = way0linksto[2 * riskid].begin(); itre != way0linksto[2 * riskid].end(); ++itre)
				{
					int poi = way0linksto[2 * riskid + 1][waylinkcount];

					//				cout << "任务重量是" << weight << endl;
					if (*itre == true)
					{
						restcap[poi][0] = restcap[poi][0] + weight;
					}
					else
					{
						restcap[poi][1] = restcap[poi][1] + weight;
					}
					waylinkcount++;
				}
				break;
			case 1:
				waylinkcount = 0;
				for (auto itre = way1linksto[2 * riskid].begin(); itre != way1linksto[2 * riskid].end(); ++itre)
				{
					int poi = way1linksto[2 * riskid + 1][waylinkcount];
					if (*itre == true)
					{
						restcap[poi][0] = restcap[poi][0] + weight;
					}
					else
					{
						restcap[poi][1] = restcap[poi][1] + weight;
					}
					waylinkcount++;
				}
				break;
			case 2:
				waylinkcount = 0;
				for (auto itre = way2linksto[2 * riskid].begin(); itre != way2linksto[2 * riskid].end(); ++itre)
				{
					int poi = way2linksto[2 * riskid + 1][waylinkcount];
					if (*itre == true)
					{
						restcap[poi][0] = restcap[poi][0] + weight;
					}
					else
					{
						restcap[poi][1] = restcap[poi][1] + weight;
					}
					waylinkcount++;
				}
				break;
			default:
				break;
			}

			switch (costable[riskid][3])
			{
				int waylinkcount;
			case 0:
				waylinkcount = 0;
				for (auto itre = way0linksto[2 * riskid].begin(); itre != way0linksto[2 * riskid].end(); ++itre)
				{
					int poi = way0linksto[2 * riskid + 1][waylinkcount];

					//				cout << "任务重量是" << weight << endl;
					if (*itre == true)
					{
						restcap[poi][0] = restcap[poi][0] - weight;
					}
					else
					{
						restcap[poi][1] = restcap[poi][1] - weight;
					}
					waylinkcount++;
				}
				break;
			case 1:
				waylinkcount = 0;
				for (auto itre = way1linksto[2 * riskid].begin(); itre != way1linksto[2 * riskid].end(); ++itre)
				{
					int poi = way1linksto[2 * riskid + 1][waylinkcount];
					if (*itre == true)
					{
						restcap[poi][0] = restcap[poi][0] - weight;
					}
					else
					{
						restcap[poi][1] = restcap[poi][1] - weight;
					}
					waylinkcount++;
				}
				break;
			case 2:
				waylinkcount = 0;
				for (auto itre = way2linksto[2 * riskid].begin(); itre != way2linksto[2 * riskid].end(); ++itre)
				{
					int poi = way2linksto[2 * riskid + 1][waylinkcount];
					if (*itre == true)
					{
						restcap[poi][0] = restcap[poi][0] - weight;
					}
					else
					{
						restcap[poi][1] = restcap[poi][1] - weight;
					}
					waylinkcount++;
				}
				break;
			default:
				break;
			}

			if (costable[riskid][3] != costable[riskid][5])
			{
				switch (costable[riskid][3])
				{
					int waylinkcount;
				case 0:
					waylinkcount = 0;
					for (auto itre = way0linksto[2 * riskid].begin(); itre != way0linksto[2 * riskid].end(); ++itre)
					{
						int poi = way0linksto[2 * riskid + 1][waylinkcount];

						//				cout << "任务重量是" << weight << endl;
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] + weight;
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] + weight;
						}
						waylinkcount++;
					}
					break;
				case 1:
					waylinkcount = 0;
					for (auto itre = way1linksto[2 * riskid].begin(); itre != way1linksto[2 * riskid].end(); ++itre)
					{
						int poi = way1linksto[2 * riskid + 1][waylinkcount];
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] + weight;
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] + weight;
						}
						waylinkcount++;
					}
					break;
				case 2:
					waylinkcount = 0;
					for (auto itre = way2linksto[2 * riskid].begin(); itre != way2linksto[2 * riskid].end(); ++itre)
					{
						int poi = way2linksto[2 * riskid + 1][waylinkcount];
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] + weight;
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] + weight;
						}
						waylinkcount++;
					}
					break;
				default:
					break;
				}

				switch (costable[riskid][5])
				{
					int waylinkcount;
				case 0:
					waylinkcount = 0;
					for (auto itre = way0linksto[2 * riskid].begin(); itre != way0linksto[2 * riskid].end(); ++itre)
					{
						int poi = way0linksto[2 * riskid + 1][waylinkcount];

						//				cout << "任务重量是" << weight << endl;
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] - weight;
							if (restcap[poi][0] < 0)
							{
								capoutsecminflag = true;
							}
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] - weight;
							if (restcap[poi][1] < 0)
							{
								capoutsecminflag = true;
							}
						}
						waylinkcount++;
					}
					break;
				case 1:
					waylinkcount = 0;
					for (auto itre = way1linksto[2 * riskid].begin(); itre != way1linksto[2 * riskid].end(); ++itre)
					{
						int poi = way1linksto[2 * riskid + 1][waylinkcount];
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] - weight;
							if (restcap[poi][0] < 0)
							{
								capoutsecminflag = true;
							}
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] - weight;
							if (restcap[poi][1] < 0)
							{
								capoutsecminflag = true;
							}
						}
						waylinkcount++;
					}
					break;
				case 2:
					waylinkcount = 0;
					for (auto itre = way2linksto[2 * riskid].begin(); itre != way2linksto[2 * riskid].end(); ++itre)
					{
						int poi = way2linksto[2 * riskid + 1][waylinkcount];
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] - weight;
							if (restcap[poi][0] < 0)
							{
								capoutsecminflag = true;
							}
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] - weight;
							if (restcap[poi][1] < 0)
							{
								capoutsecminflag = true;
							}
						}
						waylinkcount++;
					}
					break;
				default:
					break;
				}
			}

			if (capoutsecminflag == true)
			{
		//		cout << "这是第" << riskid << "个任务尝试cicici低替换out!!!" << endl;
				switch (costable[riskid][5])
				{
					int waylinkcount;
				case 0:
					waylinkcount = 0;
					for (auto itre = way0linksto[2 * riskid].begin(); itre != way0linksto[2 * riskid].end(); ++itre)
					{
						int poi = way0linksto[2 * riskid + 1][waylinkcount];

						//				cout << "任务重量是" << weight << endl;
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] + weight;
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] + weight;
						}
						waylinkcount++;
					}
					break;
				case 1:
					waylinkcount = 0;
					for (auto itre = way1linksto[2 * riskid].begin(); itre != way1linksto[2 * riskid].end(); ++itre)
					{
						int poi = way1linksto[2 * riskid + 1][waylinkcount];
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] + weight;
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] + weight;
						}
						waylinkcount++;
					}
					break;
				case 2:
					waylinkcount = 0;
					for (auto itre = way2linksto[2 * riskid].begin(); itre != way2linksto[2 * riskid].end(); ++itre)
					{
						int poi = way2linksto[2 * riskid + 1][waylinkcount];
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] + weight;
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] + weight;
						}
						waylinkcount++;
					}
					break;
				default:
					break;
				}

				switch (costable[riskid][3])
				{
					int waylinkcount;
				case 0:
					waylinkcount = 0;
					for (auto itre = way0linksto[2 * riskid].begin(); itre != way0linksto[2 * riskid].end(); ++itre)
					{
						int poi = way0linksto[2 * riskid + 1][waylinkcount];

						//				cout << "任务重量是" << weight << endl;
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] - weight;
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] - weight;
						}
						waylinkcount++;
					}
					break;
				case 1:
					waylinkcount = 0;
					for (auto itre = way1linksto[2 * riskid].begin(); itre != way1linksto[2 * riskid].end(); ++itre)
					{
						int poi = way1linksto[2 * riskid + 1][waylinkcount];
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] - weight;
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] - weight;
						}
						waylinkcount++;
					}
					break;
				case 2:
					waylinkcount = 0;
					for (auto itre = way2linksto[2 * riskid].begin(); itre != way2linksto[2 * riskid].end(); ++itre)
					{
						int poi = way2linksto[2 * riskid + 1][waylinkcount];
						if (*itre == true)
						{
							restcap[poi][0] = restcap[poi][0] - weight;
						}
						else
						{
							restcap[poi][1] = restcap[poi][1] - weight;
						}
						waylinkcount++;
					}
					break;
				default:
					break;
				}
			}
			else
			{
				costable[riskid][3] = costable[riskid][5];
			}

		}
		else
		{
			costable[riskid][3] = costable[riskid][4];
		}
	}



	//calc totol cost
	for (int n = 0; n < risknum; n++)
	{
		int chooseway = costable[n][3];
		totalcost += costable[n][chooseway];
	}

    //output the result
	cout << totalcost << endl;
	for (int k = 0; k < risknum; k++)
	{
		cout << k << " "<< request[k*4+1][1]<<endl;
		int chooseway= costable[k][3];
		for (auto itre = request[k * 4 + 2 + chooseway].begin(); itre != request[k * 4 + 2 + chooseway].end(); ++itre)
		{
			cout << *(itre) << " ";
		}
		cout<< endl;
	}

}

void restcapfill()
{
	restcap[0][0] = 955;
	restcap[0][1] = 955;
	for (int i = 1; i < 956; i++)
	{
		restcap[i][0] = gridTopo[i][2] * 0.8;
		restcap[i][1] = gridTopo[i][2] * 0.8;
	}

}