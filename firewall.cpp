#include<bits/stdc++.h>
using namespace std;
//space: O(nlogn^2)
//query: O(logn^2)
#define M 65535 //Port range: x 
#define N 1 << 32 //IP Range: y

//Horizontal Segment
struct SegmentX{
	int x1 , x2;
};

//Vertical Segment
struct SegmentY{
	unsigned long int y1 , y2;
};

vector<SegmentX>SegX1;
vector<SegmentX>SegX2;
vector<SegmentX>SegX3;
vector<SegmentX>SegX4;

vector<pair<SegmentY , int>>SegY1; 
vector<pair<SegmentY , int>>SegY2;
vector<pair<SegmentY , int>>SegY3;
vector<pair<SegmentY , int>>SegY4;

//4 X-segment tree
struct XSegTree{
	vector<SegmentX>Seq;
	YSegTree YTree;
};

struct XSegTree XTree1[4 * M]; //(inbound , tcp)
struct XSegTree XTree2[4 * M]; //(inbound , udp)
struct XSegTree XTree3[4 * M]; //(outbound , tcp)
struct XSegTree XTree4[4 * M]; //(outbound , udp)

//Y-segment tree
struct YSegTree{
	vector<SegmentY>Seq;
};

//build segment tree for vertical segments
void buildTreeY(struct YSegTree YTree[] , vector<pair<SegmentY , int>> &SegY, int node , int left , int right , int pos){
	if(SegY[pos].first.y1 <= left && SegY[pos].first.y2 >= right){
		YTree[node].Seq.push_back(SegY[pos].first);
	}
	else{
		int middle = left + (right - left) / 2;
		if(Segy[pos].first.y1 <= middle){
			buildTreeY(YTree , SegY, node * 2 , left , middle , pos);
		}
		if(SegY[pos].first.y2 >= middle){
			buildTreeY(YTree , SegY, node * 2 + 1 , middle + 1 , right , pos);
		}
	}
}

//build segment tree for horizontal segment
void buildTreeX(struct XSegTree XTree[] , vector<pair<SegmentY , int>> &SegY, vector<SegmentX> &SegX, int node , int left , int right , int pos){
	if(SegX[pos].first.x1 <= left && SegX[pos].first.x2 >= right){
		XTree[node].Seq.push_back(SegX[pos].first);
	}
	else{
		int middle = left + (right - left) / 2;
		if(SegX[pos].first.x1 <= middle){
			buildTreeX(XTree ,SegY, SegX , node * 2 , left , middle , pos);
		}
		if(SegX[pos].first.x2 >= middle){
			buildTreeX(XTree ,SegY, SegX , node * 2 + 1 , middle + 1 , right , pos);
		}
	}
	struct YSegTree YTree[4 * N];
	for(int i = 0 ; i < SegY.size() ; i++){
		//only consider vertical segments that is belong to current interval
		if(SegY[i].second >= left && SegY[i].second <= right){
			buildTreeY(YTree , SegY , 1 , 0 , N - 1, i);
		}
	}
	XTree[node].YSegTree = YTree;
}

//query in X-segment tree
bool QueryX(struct XSegTree XTree[] , int node , int left , int right , pair<int , unsigned long int> &points){
	bool res = false;
	if(left > right){
		return false;
	}
	else{
		vector<SegmentX>XSegment = XTree[node].Seq;
		for(auto segments : XSegment){
			if(segments.x1 <= points.first && segments.x2 >= points.first){
				return QueryY(XTree[node].YSegTree , 1 , 0 , N - 1 , points);
			}
		}
		int middle = left + (right - left) / 2;
		if(points.first <= middle){
			res = QueryX(XTree , node * 2 , left , middle , points);
		}
		else{
			res = QueryX(XTree , node * 2 + 1 , middle + 1 , right , points);
		}
	}
	return res;
}
//query in y-segment tree
bool QueryY(struct YSegTree YTree[] , int node, int left , int right , pair<int , unsigned long int> &points){
	bool res = false;
	if(left > right){
		return false;
	}
	else{
		vector<SegmentY>YSegment = YTree[node].Seq;
		for(auto segments : YSegment){
			if(segments.y1 <= points.second && segments.y2 >= points.second){
				return true;
			}
		}
		int middle = left + (right - left) / 2;
		if(points.second <= middle){
			res = QueryY(YTree , node * 2 , left , middle  , points);
		}
		else{
			res = Query(YTree , node * 2 + 1 , middle + 1 , right , points);
		}
	}
	return res;
}

class FireWall{
	string _filename;
public:
	FireWall(string filename){
		_filename = filename;
	}
	unsigned long int ipToLong(string IPAddress);
	void preprocess(string filename);
	void buildSegTree();
	bool accept_package(string direction , string protocol , string port , string IP);
};

unsigned long int FireWall::iptoLong(string IPAddress){
	unsigned long int num = 0 , val;
	char *tok , *ptr;
	tok = strtok(IPAddress , ".");
	while(tok != NULL){
		val = strtoul(tok , &ptr , 0);
		num = (num << 8) + val;
		tok = strtok(TPAddress , ".");
	}
	return (num);
}

void FireWall::preprocess(string filename){
	ifstream inputFile(filename);
	while(inputFile){
		string s;
		if(!getline(inputFile , s)) break;
		isstringstream ss(s);
		vector<string>record;
		while(ss){
			string line;
			if(!getline(ss , line , ',')) break;
			else{
				record.push_back(line);
			}
		}
		vector<SegmentX>tempX;
		vector<pair<SegmentY , int>>tempY;
		if(record[0] == "inbound"){
			if(record[1] == "tcp"){
				tempX = SegX1;
				tempY = SegY1;
			}
			else{
				tempX = SegX2;
				tempY = SegY2;
			}
		}
		else{
			if(record[1] == "tcp"){
				tempX = SegX3;
				tempY = SegY3;
			}
			else{
				tempX = SegX4;
				tempY = SegY4;
			}
		}
		int pos1 = record[2].find_first_of("-");
		int pos2 = record[3].find_first_of("-");
		if(pos1 != -1){
			int minPort = stoi(record[2].substr(0 , pos1 - 1));
			int maxPort = stoi(record[2].substr(pos1 + 1));
			SegmentX xSeg = {minPort , maxPort};
			tempX.push_back(xSeg);
			if(pos2 != -1){
				unsigned long int minIP = iptoLong(record[3].substr(0 , pos2 - 1));
				unsigned long int maxIP = iptoLong(record[3].substr(pos2 + 1));
				SegmentY ySeg = {minIP , maxIP};
				tempY.push_back(make_pair(ySeg , minPort));
			}
			else{
				unsigned long int IP = iptoLong(record[3]);
				SegmentY ySeg = {IP , IP};
				tempY.push_back(make_pair(ySeg , minPort));
			}
		}
		else{
			int Port = stoi(record[2]);
			SegmentX xSeg = {Port , Port};
			tempX.push_back(xSeg);
			if(pos2 != -1){
				unsigned long int minIP = iptoLong(record[3].substr(0 , pos2 - 1));
				unsigned long int maxIP = iptoLong(record[3].substr(pos2 + 1));
				SegmentY ySeg = {minIP , maxIP};
				tempY.push_back(make_pair(ySeg , Port));
			}
			else{
				unsigned long int IP = iptoLong(record[3]);
				SegmentY ySeg = {IP , IP};
				tempY.push_back(make_pair(ySeg , Port));
			}
		}
	}
}

void FireWall::buildSegTree(){
	for(int i = 0 ; i < SegX1.size() ; i++){
		buildTreeX(XTree1 , SegY1 , SegX1 , 1 , 1 , M , i);
	}
	for(int i = 0 ; i < SegX2.size() ; i++){
		buildTreeX(XTree2 ,  SegY2 , SegX2 , 1 , 1 , M , i);
	}
	for(int i = 0 ; i < SegX3.size() ; i++){
		buildTreeX(XTree3 , SegY3 , SegX3 , 1 , 1 , M , i);
	}
	for(int i = 0 ; i < SegX4.size() ; i++){
		buildTreeX(XTree4 , SegY4 , SegX4 , 1 , 1 , M , i);
	}
}

bool FireWall::accept_package(string direction , string protocol , string port , string IP){
	int Port = stoi(protocol);
	unsigned long int Ip = iptoLong(IP);
	pair<int , unsigned long int>points = make_pair(Port , Ip);
	if(direction == "inbound"){
		if(protocol == "tcp"){
			return QueryX(XTree1 , 1 , 1 , M , points);
		}
		else{
			return QueryX(XTree2 , 1 , 1 , M , points);
		}
	}
	else{
		if(protocol == "tcp"){
			return QueryX(XTree3 , 1 , 1 , M , points);
		}
		else{
			return QueryX(XTree4 , 1 , 1 , M , points);
		}
	}
}
