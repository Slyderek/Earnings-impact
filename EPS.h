#ifndef EPS_h
#define EPS_h

#include<iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
using vec = vector<string>;
using matrix = vector<vec>;



matrix readCSV(string filename)
{
	char separator = ',';
	matrix result;
	string row, item;

	ifstream in(filename);
	while (getline(in, row))
	{
		vec R;
		stringstream ss(row);
		while (getline(ss, item, separator)) R.push_back(item);
		result.push_back(R);
	}
	in.close();
	return result;
}

class EPS {
private: 
	vec ticker;
	vec startdate;
	vec enddate;
	vec b_est;
	vec eps;
	vec group;
public:
	matrix data = readCSV("C:/Users/axlez/Desktop/csvdata1.csv");
	EPS() {
		int size = data.size();
		for (int i = 0; i < 4; i++) {
			ticker.push_back(data[i][0]);
			startdate.push_back(data[i][1]);
			enddate.push_back(data[i][3]);
			b_est.push_back(data[i][4]);
			eps.push_back(data[i][5]);
			group.push_back(data[i][6]);
		}
	}

	vec getTicker() {
		return ticker;
	}
};



#endif