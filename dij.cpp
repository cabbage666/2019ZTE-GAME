#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <stdlib.h>

#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>

//���ύ��������������������������
using namespace std;
vector<vector<int>> gridTopo(956, vector<int>(0));  //input array randam col and row
vector<vector<int>> request(4001, vector<int>(0));  //input array randam col and row
//���ύ��������������������������

vector<vector<int>> simpRes(1000, vector<int>(0));
vector<vector<int>> finalway(1000, vector<int>(0));

//����dijstraѰ·
struct graph_point
{
	int point_value;
	int mincost; 
	int perpoint;
	struct graph_line* next;
};

struct graph_line
{
	int linep_value;
	//int linecap;
	int linecost;
	struct graph_line* next;
};

typedef struct graph_point Gpoint;
typedef struct graph_line Gline;
//����dijstraѰ·

//����ʣ����������
struct finalgraph_point
{
	int point_value;
	struct finalgraph_line* next;
};

struct finalgraph_line
{
	int linep_value;
	int linecap;
	struct finalgraph_line* next;
};

typedef struct finalgraph_point finGpoint;
typedef struct finalgraph_line finGline;
//����ʣ����������

int restcap[956][2];
int costable[1000][4];
finGpoint fingraph[500];
int waycost[1000];
int dijcost;
int sumcost;

//���ύ����������������������������������������������������������
void gridtopoToMemory();
void requestToMemory();
//���ύ����������������������������������������������������������

bool cmp(const vector<int> &a, const vector<int> &b);
bool mincostcmp(const  Gpoint &a, const  Gpoint &b);
void simplifyRes();
void choosePath();
void restcapfill();

vector<int> dijstra(int start_p, int end_p);
void restcapTogra();
void replaceway();
void output();

int main()
{
	gridtopoToMemory();
	cout << "gridtopo read success������" << endl;
	//resultToMemory();
	requestToMemory();
	cout << "request read success������" << endl;


	time_t start, stop;
	start = clock();
	//submit************************************
	restcapfill();
	choosePath();

	simplifyRes();
	sort(simpRes.begin(), simpRes.end(), cmp);//������������С����
	
    restcapTogra();

	replaceway();


	output();
	//submit************************************

	stop = clock();
	cout << "The run time is: " << (double)( stop - start) / CLOCKS_PER_SEC << "s" << endl;
	cout << "ALL OK!!!!" << endl;
	getchar();
	//return 0;
}

//���ύ��������������������������
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

		partline = strtok(readline, delim); //���ָ������ո񣩻���\0�������ɷָ�����
		while (partline)
		{
			datainrow.push_back(atoi(partline));  // ascii  to int
			partline = strtok(NULL, delim);  //��������ĵ�ַָ��
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

		partline = strtok(readline, delim); //���ָ������ո񣩻���\0�������ɷָ�����
		while (partline)
		{
			datainrow.push_back(atoi(partline));  // ascii  to int
			partline = strtok(NULL, delim);  //��������ĵ�ַָ��
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
//���ύ��������������������������


void restcapTogra()
{
	//create finalgarph
	for (int i = 0; i < 500; i++)
	{
		(fingraph + i)->point_value = i;
		(fingraph + i)->next = NULL;
	}

	//insert line
	for (int i = 1; i < 956; i++)
	{
		finGline* line;

		//����洢
		line = (finGline*)malloc(sizeof(finGline));

		int startp = gridTopo[i][0];
		line->next = (fingraph + startp)->next;
		line->linep_value = gridTopo[i][1];
		line->linecap = restcap[i][0];
		(fingraph + startp)->next = line;

		//����洢
		line = (finGline*)malloc(sizeof(finGline));

		startp = gridTopo[i][1];
		line->next = (fingraph + startp)->next;
		line->linep_value = gridTopo[i][0];
		line->linecap = restcap[i][1];
		(fingraph + startp)->next = line;
	}

	//cout << "����3��4����ʣ�� " << restcap[7][0]<<endl;

	////cout << "����21��1����ʣ�� " << restcap[4][1]<<endl;

	//cout << "tu��3��4����ʣ�� " ;

	//finGline* linetemp;
	//linetemp = (finGline*)malloc(sizeof(finGline));
	//linetemp = (fingraph + 3)->next;
	//while (linetemp!=NULL)
	//{
	//	if (linetemp->linep_value == 4)
	//	{
	//		cout << linetemp->linecap << endl;
	//		break;
	//	}
	//	else
	//	{
	//		linetemp = linetemp->next;
	//	}
	//}
	//cout << "tu��21��1����ʣ�� " << restcap[4][1] << endl;


}

//simplify the request 
void simplifyRes()
{
	int j = 0;
	for (int i = 1; i <= 3997; i += 4)
	{
		int len = request[i + 1].size();

		simpRes[j].push_back(request[i][0]);  //ID
		simpRes[j].push_back(request[i][1]);  //weight
		simpRes[j].push_back(request[i + 1][0]);  //start point
		simpRes[j].push_back(request[i + 1][len - 1]);  //end point
		j++;
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

void choosePath()
{
	int risknum = 1000;
	int weight = 0;
	int waynum = 3;
	bool dirflag = true;   //true���� ���� false������·
	bool findlink1flag;
	bool findlink2flag;
	int lowgate = 0;
	int highgate = 955;
	int middle = 0;

	int count = 0;
	int linkcaptemp;
	int oilcost;
	int totalcost = 0;
	int minlinkcap = 0;
	bool firstlinkflag;
	int maxmincap = 0;
	int bestwayid = 0;

	bool gloansflag;


	for (int riskid = 0; riskid < risknum; riskid++)
	{
		//	cout << "���ǵ�" << riskid << "������" << endl;
		weight = request[riskid * 4 + 1][1];
		int j = riskid * 4 + 2;

		maxmincap = 0;

		//waylinksto[0]����������� [1]������·��gridtopo��λ��
		vector<vector<int>> way0linksto(2, vector<int>(0));
		vector<vector<int>> way1linksto(2, vector<int>(0));
		vector<vector<int>> way2linksto(2, vector<int>(0));

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

				//�ҵ�ÿ��link
				if (dirflag)
				{
					while (!findlink1flag)
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
								//	cout << *itre << "�ҵ��ˣ�but" << *(itre + 1) << "�������أ�" << endl;
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
				//			cout << "link �ĵ�һ�ڵ㶪�ˣ�" << endl;
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
						if (*(itre + 1) == gridTopo[middle][0])
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
						//			cout << *(itre + 1) << "�ڶ��������ҵ��ˣ�but" << *itre << "�������أ�" << endl;
									count = 0;
									findlink2flag = true;
								}
							}

						}
						else if (*(itre + 1) > gridTopo[middle][0])
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
				//			cout << "link �ĵ�һ�ڵ㶪�ˣ�" << endl;
						}
						if (findlink2flag == true)
						{
							findlink1flag = true;
						}
					}
				}

				//ÿ����·�ķ����λ�ñ���
				if (m == 0)
				{
					way0linksto[0].push_back(dirflag);
					way0linksto[1].push_back(middle);
				}
				else if (m == 1)
				{
					way1linksto[0].push_back(dirflag);
					way1linksto[1].push_back(middle);
				}
				else
				{
					way2linksto[0].push_back(dirflag);
					way2linksto[1].push_back(middle);
				}

				//�ҵ���Сlink����
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
			//	cout << "����·�ɱ���"<<oilcost << endl;
			//	cout << "����·����С������"<< minlinkcap << endl;
			//	cout << "��õ�·��" << bestwayid << endl;

			costable[riskid][m] = oilcost;

		}
		costable[riskid][3] = bestwayid;

		if (maxmincap < 0)
		{
			gloansflag = false;
		}

		//update restcap*******************
		switch (bestwayid)
		{
			int waylinkcount;
		case 0:
			waylinkcount = 0;
			for (auto itre = way0linksto[0].begin(); itre != way0linksto[0].end(); ++itre)
			{
				int poi = way0linksto[1][waylinkcount];

				//				cout << "����������" << weight << endl;
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
			for (auto itre = way1linksto[0].begin(); itre != way1linksto[0].end(); ++itre)
			{
				int poi = way1linksto[1][waylinkcount];
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
			for (auto itre = way2linksto[0].begin(); itre != way2linksto[0].end(); ++itre)
			{
				int poi = way2linksto[1][waylinkcount];
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
		//update restcap*******************
	}

	//calc totol cost
	for (int n = 0; n < risknum; n++)
	{
		int chooseway = costable[n][3];
		//totalcost += costable[n][chooseway];
		waycost[n] = costable[n][chooseway];
	}

	//cout << "�ܵĳɱ���" << totalcost << endl;


	for (int k = 0; k < risknum; k++)
	{
		//cout << k << " "<< request[k*4+1][1]<<endl;
		int chooseway = costable[k][3];
		for (auto itre = request[k * 4 + 2 + chooseway].begin(); itre != request[k * 4 + 2 + chooseway].end(); ++itre)
		{
			finalway[k].push_back(*itre);
		}
	}

}


vector<int> dijstra(int start_p,int end_p)
{

	//creat graph******************************
	Gpoint graph[500];
	//�����
	for (int i = 0; i < 500; i++)
	{
		(graph + i)->point_value = i;
		(graph + i)->mincost = -1;
		(graph + i)->perpoint = -1;
		(graph + i)->next = NULL;
	}
	//(graph + 0)->mincost = 0;

	//insert line
	for (int i = 1; i < 956; i++)
	{
		Gline* line;

		//����洢
		line = (Gline*)malloc(sizeof(Gline));

		int startp = gridTopo[i][0];
		line->next = (graph + startp)->next;
		line->linep_value = gridTopo[i][1];
	//	line->linecap = gridtopo[i][2];
		line->linecost = gridTopo[i][3];

		(graph + startp)->next = line;

		//����洢
		line = (Gline*)malloc(sizeof(Gline));

		startp = gridTopo[i][1];
		line->next = (graph + startp)->next;
		line->linep_value = gridTopo[i][0];
	//	line->linecap = gridtopo[i][2];
		line->linecost = gridTopo[i][3];

		(graph + startp)->next = line;
	}
	 vector<Gpoint> activelist;
	 vector<Gpoint> adjtemp;
	 //creat graph******************************

	 //dijstra******************************
	 (graph + start_p)->mincost = 0;
	 activelist.push_back(graph[start_p]);


	 while (activelist.size() != 0 )
	 {
		// count++;
		 sort(activelist.begin(), activelist.end(), mincostcmp);
		 Gline* line;
		 line = activelist.back().next;  //���һ��Ԫ�ص��ڽӱ�
		// cout << "�������С�ܳɱ��ĵ���" << activelist.back().point_value;

		 while (line != NULL)
			{
				int adjpoint = line->linep_value; //�ڽӵĵ�
			//	cout << "��������ڽӵ���" << adjpoint << endl;
				if ((graph + adjpoint)->mincost == -1)
				{
					(graph + adjpoint)->mincost = line->linecost + activelist.back().mincost;
					adjtemp.push_back(graph[adjpoint]);

					(graph + adjpoint)->perpoint = activelist.back().point_value; //ǰ���

			//		cout << "��������С�ɱ���" << activelist.back().mincost<< "�߶γɱ���" << line->linecost << endl;
			//		cout << "�ڽӵ�" << adjpoint << "���ܳɱ���" << (graph + adjpoint)->mincost << endl;
				}
				else
				{
				//	(graph + activelist.back().point_value)->mincost;
					if ((graph + adjpoint)->mincost > (line->linecost + activelist.back().mincost))
					{
						(graph + adjpoint)->mincost = line->linecost + activelist.back().mincost;
						(graph + adjpoint)->perpoint = activelist.back().point_value; //ǰ���
						
						for (auto oldnum = activelist.begin(); oldnum != activelist.end(); oldnum++)
						{
							if (adjpoint == (*oldnum).point_value)
							{
								(*oldnum).mincost = line->linecost + activelist.back().mincost;
								(*oldnum).perpoint = activelist.back().point_value; 
							}
						}

				//		cout << "�ϸ�����С�ɱ���" << activelist.back().mincost << "�߶γɱ���" << line->linecost << endl;
				//		cout << "�ڽӵ�" << adjpoint << "���ܳɱ���" << (graph + adjpoint)->mincost << endl;
					}
					else
					{
				//		cout << "��������֮ǰ�ɱ��ߣ��ɱ���" << (graph + adjpoint)->mincost << endl;
					}
				}
				line = line->next;
			}

		 activelist.pop_back();  //�����һ��Ҳ���ܳɱ���С�ĵ���
		 for (auto itre = adjtemp.begin(); itre != adjtemp.end(); itre++)
		 {
			activelist.push_back(*itre);
		 }
		 adjtemp.clear();

		//cout << " ������е�" << endl;
		//for (auto itre = activelist.begin(); itre != activelist.end(); itre++)
		//{
		//	cout << (*itre).point_value<< " " ;
		//	cout << endl;
		//}
		
	 }
	 //dijstra******************************

		//cout <<"��2�ĳɱ���"<< (graph+2)->mincost << " ";
		////cout <<"0��121�ĳɱ���"<< (graph+121)->mincost << " ";
	 //   cout << (graph + end_p)->mincost << endl;
		dijcost = (graph + end_p)->mincost;

	    vector<int> dijwaytemp;

	//	cout << end_p<< endl;
		dijwaytemp.push_back(end_p);

		int count = 0;
		while (end_p != start_p)
		{
			count++;
			if (count > 100)
			{
	    	cout << "100�ε�·̫���˰�....." << endl;
				break;
			}

		//	cout << (graph + end_p)->perpoint << endl;
			dijwaytemp.push_back((graph + end_p)->perpoint);
			end_p = (graph + end_p)->perpoint;
		}

		
	//	cout << (graph + end_p)->mincost << endl;
		return dijwaytemp;

	//	forbidlist.push_back(chooselist[0]);
	//	chooselist.erase(chooselist.begin());  //delete first element
	//	lastvisited.clear();
	//	lastvisited.assign(curvisit.begin(), curvisit.end());  //��ֵ
	//	curvisit.clear();

}

// sort array 
bool cmp(const vector<int> &a, const vector<int> &b) 
{
	return a[1] > b[1];
}

bool mincostcmp(const  Gpoint &a, const  Gpoint &b)
{
	return a.mincost > b.mincost;
}

void replaceway()
{
	int dijable = 0;
	int dijno = 0;
	for (int riskid = 0; riskid < 1000; riskid++)
	{
		int riskID = simpRes[riskid][0];
		int riskWEI = simpRes[riskid][1];
		int riskSTA = simpRes[riskid][2];
		int riskEND = simpRes[riskid][3];

		cout << riskID << " " << riskWEI << " " << riskSTA << " " << riskEND << endl;

		//����Ҫ�滻��·��ÿ����·��������
		for (auto itre = finalway[riskID].begin(); (itre + 1) != finalway[riskID].end(); itre++)
		{
			int frontp = *itre;
			int backp = *(itre + 1);

			finGline* linetemp;
			linetemp = (finGline*)malloc(sizeof(finGline));
			linetemp = (fingraph + frontp)->next;
			int adjnum = 1;
			//	cout << frontp << " " << backp << endl;
			while (linetemp != NULL)
			{
				//cout << "�ڽӵ���" << (fingraph + frontp)->next->linep_value << endl;
				if (linetemp->linep_value == backp && adjnum == 1)
				{
				//	cout << "��" << frontp << "��" << backp << "��ʣ��cap��";
				//	cout << (fingraph + frontp)->next->linecap << endl;
					(fingraph + frontp)->next->linecap += riskWEI;
				//	cout << "��" << frontp << "��" << backp << "��ʣ��cap��";
				//	cout << (fingraph + frontp)->next->linecap << endl;
					break;
				}
				else if (linetemp->linep_value == backp && adjnum == 2)
				{
					(fingraph + frontp)->next->next->linecap += riskWEI;
					//	cout << "��" << frontp << "��" << backp << "��ʣ��cap��";
					//	cout << (fingraph + frontp)->next->linecap << endl;
					break;
				}
				else if (linetemp->linep_value == backp && adjnum == 3)
				{
					(fingraph + frontp)->next->next->next->linecap += riskWEI;
					break;
				}
				else if (linetemp->linep_value == backp && adjnum == 4)
				{
					(fingraph + frontp)->next->next->next->next->linecap += riskWEI;
					break;
				}
				else
				{
					adjnum++;
					linetemp = linetemp->next;
				}
			}
		}

		//��dij�ҵ�һ��·******************************
		vector<int> dijway;
		dijway = dijstra(riskSTA, riskEND);
		sort(dijway.begin(), dijway.end());

		//for (auto itre = dijway.begin(); itre != dijway.end(); itre++)
		//{
		//	cout << *itre << " ";
		//}
		//cout << endl;
		//��dij�ҵ�һ��·******************************

		//�ж�dij��·�Ƿ����******************************
		bool dijwaycanflag = true;

		for (auto itre = dijway.begin(); (itre + 1) != dijway.end(); itre++)
		{
			int frontp = *itre;
			int backp = *(itre + 1);

			finGline* linetemp;
			linetemp = (finGline*)malloc(sizeof(finGline));
			linetemp = (fingraph + frontp)->next;

			//	cout << frontp << " " << backp << endl;
			while (linetemp != NULL)
			{
				//cout << "�ڽӵ���" << (fingraph + frontp)->next->linep_value << endl;
				if (linetemp->linep_value == backp)
				{
					cout << "��" << frontp << "��" << backp << "��ʣ��cap��";
					cout << linetemp->linecap << endl;
					cout << "����������";
					cout << riskWEI << endl;

					if (linetemp->linecap < riskWEI)
					{
						dijwaycanflag = false;
					}
					break;
				}
				else
				{
					linetemp = linetemp->next;
				}
			}

			//��һ����·��ͨ ������·cycle abort
			if (dijwaycanflag == false)
			{
				break;
			}
		}
		//�ж�dij��·�Ƿ����******************************

		//����dij�Ƿ���� �������Ƿ��滻
		if (dijwaycanflag == true)
		{
			cout << "dij�ǿ��Եģ�����" << endl;
			dijable++;

			//��ͼ�е�dij·��ÿ����·��ȥ����
			for (auto itre = dijway.begin(); (itre + 1) != dijway.end(); itre++)
			{
				int frontp = *itre;
				int backp = *(itre + 1);

				finGline* linetemp;
				linetemp = (finGline*)malloc(sizeof(finGline));
				linetemp = (fingraph + frontp)->next;
				//	cout << frontp << " " << backp << endl;
				int adjnum = 1;
				//	cout << frontp << " " << backp << endl;
				while (linetemp != NULL)
				{
					//cout << "�ڽӵ���" << (fingraph + frontp)->next->linep_value << endl;
					if (linetemp->linep_value == backp && adjnum == 1)
					{
						//	cout << "��" << frontp << "��" << backp << "��ʣ��cap��";
						//	cout << (fingraph + frontp)->next->linecap << endl;
						(fingraph + frontp)->next->linecap -= riskWEI;
						//	cout << "��" << frontp << "��" << backp << "��ʣ��cap��";
						//	cout << (fingraph + frontp)->next->linecap << endl;
						break;
					}
					else if (linetemp->linep_value == backp && adjnum == 2)
					{
						(fingraph + frontp)->next->next->linecap -= riskWEI;
						//	cout << "��" << frontp << "��" << backp << "��ʣ��cap��";
						//	cout << (fingraph + frontp)->next->linecap << endl;
						break;
					}
					else if (linetemp->linep_value == backp && adjnum == 3)
					{
						(fingraph + frontp)->next->next->next->linecap -= riskWEI;
						break;
					}
					else if (linetemp->linep_value == backp && adjnum == 4)
					{
						(fingraph + frontp)->next->next->next->next->linecap -= riskWEI;
						break;
					}
					else
					{
						adjnum++;
						linetemp = linetemp->next;
					}
				}
			}

			//update  finalway
			finalway[riskID].clear();
			finalway[riskID].assign(dijway.begin(), dijway.end());
			//for (auto itre = finalway[riskID].begin(); itre != finalway[riskID].end(); itre++)
			//{
			//	cout << *itre << " ";
			//}
			//cout << endl;

			//update  waycost ��
		//    cout << dijcost <<endl;
		//	cout << waycost[riskID]<<endl;
			waycost[riskID] = dijcost;
			
		}
		else
		{
			cout << "dij�������أ�����" << endl;
			dijno++;
			//������֮ǰ��· update  fingraph*******************************
			for (auto itre = finalway[riskID].begin(); (itre + 1) != finalway[riskID].end(); itre++)
			{
				int frontp = *itre;
				int backp = *(itre + 1);

				finGline* linetemp;
				linetemp = (finGline*)malloc(sizeof(finGline));
				linetemp = (fingraph + frontp)->next;
				//	cout << frontp << " " << backp << endl;
				int adjnum = 1;
				//	cout << frontp << " " << backp << endl;
				while (linetemp != NULL)
				{
					//cout << "�ڽӵ���" << (fingraph + frontp)->next->linep_value << endl;
					if (linetemp->linep_value == backp && adjnum == 1)
					{
						//	cout << "��" << frontp << "��" << backp << "��ʣ��cap��";
						//	cout << (fingraph + frontp)->next->linecap << endl;
						(fingraph + frontp)->next->linecap -= riskWEI;
						//	cout << "��" << frontp << "��" << backp << "��ʣ��cap��";
						//	cout << (fingraph + frontp)->next->linecap << endl;
						break;
					}
					else if (linetemp->linep_value == backp && adjnum == 2)
					{
						(fingraph + frontp)->next->next->linecap -= riskWEI;
						//	cout << "��" << frontp << "��" << backp << "��ʣ��cap��";
						//	cout << (fingraph + frontp)->next->linecap << endl;
						break;
					}
					else if (linetemp->linep_value == backp && adjnum == 3)
					{
						(fingraph + frontp)->next->next->next->linecap -= riskWEI;
						break;
					}
					else if (linetemp->linep_value == backp && adjnum == 4)
					{
						(fingraph + frontp)->next->next->next->next->linecap -= riskWEI;
						break;
					}
					else
					{
						adjnum++;
						linetemp = linetemp->next;
					}
				}
			}
			//������֮ǰ��· update  fingraph*******************************

		}

	}

	//cout << dijable << endl;
	//cout << dijno << endl;
}

void output()
{
	for (int i = 0; i < 1000; i++)
	{
		sumcost += waycost[i];
	}
	cout << sumcost << endl;

	//for (int i = 0; i < 1000; i++)
	//{
	//	cout << request[i * 4 + 1][0]<< " ";
	//	cout << request[i * 4 + 1][1] <<endl;
	//	for (auto itre = finalway[i].begin(); itre!= finalway[i].end(); itre++)
	//	{
	//		cout << *itre << " ";
	//	}
	//	cout << endl;
	//}
	int testcap[956][2];
	testcap[0][0] = 955;
	testcap[0][1] = 955;
	for (int i = 1; i < 956; i++)
	{
		testcap[i][0] = gridTopo[i][2] * 0.8;
		testcap[i][1] = gridTopo[i][2] * 0.8;
	}


	for (int i = 1; i < 956; i++)
	{
		
	}
	//for (int j = 0; j < 500; j++)
	//{
	//	cout << "���ǵ�" << j << "����" << endl;
	//	while ((fingraph + j)->next != NULL)
	//	{
	//		cout << (fingraph + j)->next->linecap << " " ;
	//		(fingraph + j)->next = (fingraph + j)->next->next;
	//	}
	//	cout << endl;
	//}


}


