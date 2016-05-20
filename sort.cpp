#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <memory.h>
#include <time.h>
#include <queue>    
#include <set>
        
using namespace std;

typedef unsigned long long ll;

const int constSize = 8;

ll K, B, M, T;

const int maxQ = 150;

int L[maxQ], R[maxQ], lPos[maxQ], rPos[maxQ], curNumBlock[maxQ], cnt[maxQ], n;


void getBKM(ll n) {  	
	B = min(n, (ll)maxQ);         //size 1 Block
	M = (n + B - 1) / B;           	 //count block (size B)
	K = 200;	      		   //count block size B in 1 group
	T = (M + K - 1) / K;
	R[0] = 0;
	rPos[0] = 0;
	for (int i = 1; i <= T; ++i) {
		L[i] = R[i - 1] + 1;
		R[i] = L[i] + K - 1;
		lPos[i] = rPos[i - 1] + 1;
		rPos[i] = lPos[i] + K * B - 1;
		if (rPos[i] >= n) rPos[i] = n;
		if (R[i] >= M) R[i] = M;
	}
}

multiset<pair<ll, ll> > my;   

pair<ll, ll> get(ll num, ll n) {
	ll A = (num - 1) * B + 1, C = num * B;
	if (C >= n) C = n;
	return make_pair(A, C);
}

void precalc() {
	ifstream in("input.bin", ios :: binary | ios :: in);
	ofstream out("input2.bin", ios :: binary | ios :: out);
	in.read((char*)&n, constSize);
	getBKM(n);
	vector<ll> * curBlock = new vector<ll>(1);
	out.write((char*)&n, constSize);
	for (int i = 1; i <= T; ++i) {
		ll curPos = (rPos[i - 1] + 1);
		in.seekg(curPos * constSize);
		curBlock->resize(rPos[i] - lPos[i] + 1);
		in.read((char*)curBlock->data(), (rPos[i] - lPos[i] + 1) * constSize);
		sort(curBlock->begin(), curBlock->end());
		out.write((char*)curBlock->data(), (rPos[i] - lPos[i] + 1) * constSize);
		//cout << curBlock -> size() * 8 << endl;	
	}  	
	delete curBlock;
	out.close();
	in.close();
}
                   
void ExternalSort() {
	precalc();
	ifstream in("input2.bin", ios :: binary | ios :: in);
	ofstream out("output.bin", ios :: binary | ios :: out);
	out.write((char*)&n, constSize);
	vector<ll> * tmp = new vector<ll>(1);
	for (int i = 1; i <= T; ++i) {
		curNumBlock[i] = L[i];
		pair<ll, ll> cur = get(curNumBlock[i], n);	
		curNumBlock[i] += 1;
		cnt[i] = cur.second - cur.first + 1;
		tmp->resize(cnt[i]);
		ll curPos = cur.first;
		in.seekg(curPos * constSize);
		in.read((char*)tmp->data(), (cnt[i]) * constSize);
		for (int j = 0; j < cnt[i]; ++j) my.insert(make_pair((*tmp)[j], i));	
	}  
	vector<ll> * print = new vector<ll>(B * K);
	int counter = 0;
	for (int i = 1; i <= n; ++i) {
		if (my.size() == 0) break;
		++counter;
		multiset<pair<ll, ll> > :: iterator it = my.begin();
		pair<ll, ll> currentPair = *it;
		my.erase(it);
		(*print)[counter - 1] = currentPair.first;
		int currentNumber = currentPair.second;
		cnt[currentNumber] -= 1;
		if (cnt[currentNumber] == 0 && curNumBlock[currentNumber] <= R[currentNumber]) {
			pair<ll, ll> cur = get(curNumBlock[currentNumber], n);
			curNumBlock[currentNumber] += 1;
			cnt[currentNumber] = cur.second - cur.first + 1;
			tmp->resize(cnt[currentNumber]);
 			ll curPos = cur.first;
			in.seekg(curPos * constSize);
			in.read((char*)tmp->data(), (cnt[currentNumber]) * constSize);
			for (int j = 0; j < cnt[currentNumber]; ++j) my.insert(make_pair((*tmp)[j], currentNumber));	
		} 
		if (counter == B * K) {
			out.write((char*)print->data(), constSize * B * K);
			counter = 0;
		}
	}         
	print->resize(counter);
	out.write((char*)print->data(), constSize * counter);             	
	delete tmp;
	delete print;
	in.close();
	out.close();
}

void SampleBinaryFileGen() {
	srand(time(0));
	ofstream out("input.bin", ios::binary | ios::out);
	ll maxN = 100;
	out.write((char*)&maxN, constSize);
	srand(time(0));
	for (int i = 1; i <= maxN; ++i) {
		ll val = rand() % 1000 + 1;
		val *= 2;
		out.write((char*)&val, constSize);
	}
	out.close();        
}

void printFile() {
	ifstream in("output.bin", ios :: in | ios :: binary);
	in.seekg(0);
	ll val;
	for (int i = 1; i <= 100 + 1; ++i) {
		in.read((char*)&val, constSize);
		cout << val << " ";
	}
	cout << endl;
	in.close();
}
        
int main() {
	SampleBinaryFileGen();
	ExternalSort();
	printFile();
	return 0;
}