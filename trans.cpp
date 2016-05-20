#include <algorithm>
#include <iostream>
#include <cmath>
#include <fstream>
#include <time.h>
#include <vector>

using namespace std;

typedef char Char;

const int maxN = 300;

const int P = 100;

vector<vector<Char> > a;

void print() {
	ifstream in("output.bin", ios :: in | ios :: binary);
	int n, m;
	in.read((char*)&n, 4);
	in.read((char*)&m, 4);
	for (int i = 0; i < n * m; ++i) {
		Char val;
		in.read((char*)&val, 1);
		cout << val << " ";
		if (i % m == m - 1) cout << endl;
	}
	cout << endl;
	in.close();
}

void print2() {
	ifstream in("input.bin", ios :: in | ios :: binary);
	int n, m;
	in.read((char*)&n, 4);
	in.read((char*)&m, 4);
	for (int i = 0; i < n * m; ++i) {
		Char val;
		in.read((char*)&val, 1);
		cout << val << " ";
		if (i % m == m - 1) cout << endl;
	}
	cout << endl;
	in.close();
}

void gen() {
	ofstream out("input.bin", ios :: out | ios :: binary);
	int n = 25;
	int m = 4;
	//swap(m, n);
	out.write((char*)&n, 4);
	out.write((char*)&m, 4);
	for (int i = 1; i <= (n * m); ++i) {
		Char val = i;
		val = val % (256 - 64) + 64;
		out.write((char*)&val, 1);
	}
	out.close();
}

int main() {
	gen(); print2();
	//gen(); //print2();
	ifstream in("input.bin", ios :: in | ios :: binary);
	ofstream out("output.bin", ios :: out | ios :: binary);
	int n, m;
	in.read((char*)&n, 4);
	in.read((char*)&m, 4);
	out.write((char*)&m, 4);
	out.write((char*)&n, 4);
	//cout << "------------" << endl;
	if (n <= P && n <= m) {
		int l = 0;
		int N = n;
		int M = maxN * maxN / N;
		while (l < m) {
			int r = l + M - 1;
			if (r > m - 1) r = m - 1;
			vector<Char> ans;
			ans.clear();
			vector<Char> * cur = new vector<Char>(r - l + 1);
			for (int i = 0; i < N; ++i) {
				in.seekg(8 + i * m + l);
				in.read((char*)cur->data(), r - l + 1);
				for (int i = 0; i < (r - l + 1); ++i) ans.push_back((*cur)[i]);	
				//for (int i = 0; i < (r - l + 1); ++i) cout << (*cur)[i] << " "; cout << endl;
			}
			delete cur;
			//for (int i = 0; i < ans.size(); ++i) cout << ans[i] << " "; cout << endl;
			vector<Char> * p = new vector<Char>(ans.size());
			int cnt = 0;
			for (int j = 0; j < (r - l + 1); ++j)
				for (int i = 0; i < N; ++i) {	
					int pos = i * (r - l + 1) + j;
					(*p)[cnt] = ans[pos];
					cnt += 1;
				}			
			out.write((char*)p->data(), ans.size());
			l = r + 1;
			delete p;
		}
	} else 
	if (m <= P) {
		int cntRow = 0;
		while (cntRow * m <= maxN * maxN) ++cntRow; --cntRow;
		int l = 0, cnt = 0;
		while (l < n) {
			int r = l + cntRow - 1;
			if (r >= n) r = n - 1;
			vector<Char> * p = new vector<Char>((r - l + 1) * m);
			in.read((char*)p->data(), (r - l + 1) * m); 
			for (int i = 0; i < m; ++i) {
				vector<Char> * p2 = new vector<Char>(r - l + 1);
				for (int j = 0; j < (r - l + 1); ++j)
					(*p2)[j] = (*p)[j * m + i];
				out.seekp(i * n + 8 + cnt);
				out.write((char*)p2->data(), (r - l + 1));
				delete p2;
			}                  
			cnt += (r - l + 1);
			l += cntRow;       
			delete p;
		}
	} else {
	        int p1 = maxN, p2 = maxN;
		a.resize(p1 + 5);
		for (int i = 0; i < p1; ++i) {
			a[i].resize(p2 + 5, 0);	
		}
		int c1 = (n + p1 - 1) / p1, c2 = (m + p2 - 1) / p2;
		for (int i = 0; i < c1; ++i) {
			for (int j = 0; j < c2; ++j) {
				int x1 = i * p1, x2 = x1 + p1 - 1;
				if (x2 >= n) x2 = n - 1;
				int y1 = j * p2, y2 = y1 + p2 - 1;
				if (y2 >= m) y2 = m - 1;                                      
				for (int X = x1; X <= x2; ++X) {
					vector<Char> * cur = new vector<Char>(y2 - y1 + 1);
					in.seekg(8 + (X * m + y1));
					in.read((char*)cur->data(), (y2 - y1 + 1));
					for (int k = 0; k < (y2 - y1 + 1); ++k) a[X - x1][k] = (*cur)[k]; 
					delete cur;
				}
				for (int Y = y1; Y <= y2; ++Y) {
					vector<Char> * curP = new vector<Char>(x2 - x1 + 1);
					for (int k = 0; k < (x2 - x1 + 1); ++k) (*curP)[k] = a[k][Y - y1];
					out.seekp(8 + (Y * n + x1));
					out.write((char*)curP->data(), (x2 - x1 + 1));
					delete curP;
				}
			}
		}
	}  
	in.close();
	out.close();  
	print();
	return 0;
}