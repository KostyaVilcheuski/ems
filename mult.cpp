#include <algorithm>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

typedef char T;

const int constSize = 4;
const int constElemSize = 1;

const int K = 75;

void genSample() {
	ofstream out("input.bin", ios :: binary | ios :: out);
	int n = 3;
	int m = 3;
	char a[15], b[15];                             
	a[1] = 2, a[2] = 0, a[3] = 0, a[4] = 0, a[5] = 1, a[6] = 0, a[7] = 0, a[8] = 0, a[9] = 1;
	b[1] = 1, b[2] = 2, b[3] = 3, b[4] = 4, b[5] = 5, b[6] = 6, b[7] = 7, b[8] = 8, b[9] = 9;
	out.write((char*)&n, constSize);
	out.write((char*)&m, constSize);
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			char val = a[(i - 1) * m + j];
			out.write((char*)&val, constElemSize);
		}
	}      	
	out.write((char*)&n, constSize);
	out.write((char*)&m, constSize);
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= n; ++j) {
			char val = b[(i - 1) * m + j];
			out.write((char*)&val, constElemSize);
		}
	}
	out.close();
}

void print(char * name) {
	ifstream in(name, ios :: binary | ios :: out);
	int n, m;
	in.read((char*)&n, constSize);
	in.read((char*)&m, constSize);
	cout << n << " " << m << " ";
	for (int i = 1; i <= n * m; ++i) {
		char val;
		in.read((char*)&val, constElemSize);
		cout << (int)val << " ";
	}
	cout << endl;
	in.close();
} 

int n, m;

int main() {
	genSample();  
	//gen();
	ifstream in("input.bin", ios :: binary | ios :: in);
	ofstream out("output.bin", ios :: binary | ios :: out);
	in.read((char*)&n, constSize);
	in.read((char*)&m, constSize);
	out.write((char*)&n, constSize);
	out.write((char*)&m, constSize);
	int cnt = (n + K - 1) / K;                                                        
	vector<T> * a1 = new vector<T>(1);
	vector<T> * a2 = new vector<T>(1);
	vector<T> * tmp = new vector<T> (1);
	for (int num1 = 1; num1 <= cnt; ++num1) {
		int l1 = (num1 - 1) * K + 1, r1 = num1 * K;
		if (r1 >= n) r1 = n;
		int posBinaryFile1 = (l1 - 1) * n * constElemSize + 2 * constSize;
		a1 -> resize((r1 - l1 + 1) * n);
		in.seekg(posBinaryFile1);
		in.read((char*)a1 -> data(), constElemSize * (r1 - l1 + 1) * n);
		tmp -> resize((r1 - l1 + 1) * n);
		for (int ind = 0; ind < (r1 - l1 + 1) * n; ++ind) (*tmp)[ind] = 0;
		for (int num2 = 1; num2 <= cnt; ++num2) {   	
			int l2 = (num2 - 1) * K + 1, r2 = num2 * K;
			if (r2 >= n) r2 = n;
			int posBinaryFile2 = (l2 - 1 + n) * n * constElemSize + 4 * constSize;
			a2 -> resize((r2 - l2 + 1) * n);
			in.seekg(posBinaryFile2);
			in.read((char*)a2 -> data(), constElemSize * (r2 - l2 + 1) * n);
			//out.seekg(2 * constSize + (l1 - 1) * n * constElemSize);
			for (int i = l1; i <= r1; ++i) {
				for (int j = 1; j <= n; ++j) {
					int pos0 = (i - l1) * n + j - 1; 
					int cur = (int)(*tmp)[pos0]; 						
					for (int k = l2; k <= r2; ++k) {
						//a[i][j] += b[i][k] * c[k][j]
						int val1 = (int)(*a1)[(i - l1) * n + k - 1];
						int val2 = (int)(*a2)[(k - l2) * n + j - 1];
						cur += val1 * val2;
					}
					if (cur >= 256) cur %= 256;
					(*tmp)[pos0] = (T)(cur);        					
				}
			}
		}
		out.seekp(2 * constSize + (l1 - 1) * n * constElemSize);
		out.write((char*)tmp -> data(), (r1 - l1 + 1) * n * constElemSize);  			
	}
	delete a1;
	delete a2;
	in.close();
	out.close();
	print("output.bin");
	return 0;
}