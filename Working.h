#ifndef Working_h
#define Working_h

#include "Stock.h"
#include "stdafx.h"
#include "Data.h"
#include "Operator.h"
#include <fstream>
#include <string>
#include <istream>
#include <iostream>
#include <vector>
#include <map>
#include <random>

using namespace std;

using vec = vector<string>;
using smatrix = vector<vec>;

vector<map<int, string>> sortGroupBootStrap(vector<string> group, map < string, Stock > &stockSeries);
//vector<map<string, class Stock> > &sortGroup(vector<string> group, map < string, Stock > &stockSeries);
void spy500(map<string, Stock> &resultMap, vector<double> spy);
vector<vector<vector<double>>> measurer(vector<map<int, string>> &sortMap, map<string, Stock> &AR_map_Stock);
vector<double>  BootstrapAAR(map<int, string> &sortedgroup, unsigned int numberPick, map<string, Stock> &AR_map_Stock);
vector<double> CAARer(vector<double> AARvector);

vector<map<int, string>> sortGroupBootStrap(vector<string> group, map < string, Stock > &stockSeries) {
	string symbol, date3;
	Stock stock;
	double est_eps, act_eps;
	double up = 0.05;
	double low = -0.05;
	vector<string> symbols;
	vector<map<int, string>> resultMap(group.size());
	ifstream filein;
	filein.open("BEST.txt");
	//stockSeries;
	int i = 0;
	//cout << i;
	while (filein >> symbol >> est_eps >> act_eps >> date3) {
		symbols.push_back(symbol);
		if (stockSeries[symbol].getdate().size()>80)
		{
			stockSeries[symbol].setvalue(symbol, est_eps, act_eps, date3);
			i++;
		}
		else
		{
			stockSeries.erase(symbol);
			symbols.erase(--symbols.end());
		}
	}

	//cout << endl << i;

	filein.close();
	int x = 0;
	//cout << endl << x;
	int gr0 = 0; int gr1 = 0; int gr2 = 0;
	for (unsigned int i = 0; i < symbols.size(); i++) { // tag a group for each stock
		if (stockSeries[symbols[i]].getpercentage() >= up) {
			stockSeries[symbols[i]].setgroup(group[0]);
			resultMap[0][gr0] = symbols[i]; // beat
			gr0++;
		}
		else if (stockSeries[symbols[i]].getpercentage() <= low) {
			stockSeries[symbols[i]].setgroup(group[2]);
			resultMap[2][gr2] = symbols[i]; // miss
			gr2++;
		}
		else {
			stockSeries[symbols[i]].setgroup(group[1]);
			resultMap[1][gr1] = symbols[i]; // meet
			gr1++;
		}
		x++;
	}

	//cout << endl << x;
	//cout << endl << " in the function size = " << resultMap.size() << endl;

	return resultMap;
}

// need to modify this... to vector<map<int, Stock>>
void spy500(map<string, Stock> &resultMap, vector<double> spy1) {
	map<string, Stock>::iterator itr;
	for (itr = resultMap.begin(); itr != resultMap.end(); itr++) {
		(itr->second).matchdate(spy1);
		(itr->second).calculation();
	}
}

vector<double> BootstrapAAR(map<int, string> &sortedgroup, unsigned int numberPick, map<string, Stock> &AR_map_Stock) { // one-trial of Bootstrapping
	unsigned int poolNumber = sortedgroup.size(); // among this stocks
	//cout << endl << poolNumber;
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(0, poolNumber-1); // guaranteed unbiased in (0,max), check boundary

	vector<double> temp(90); // I know this is 91 (days) but can change into general variable
	for (unsigned int i = 0; i < numberPick; i++) {
		auto stocknumber = uni(rng);
		auto stock = sortedgroup[stocknumber]; // choose numberPick(60) stocks. need to make the case check what if # < 60
		//cout << "  " << stock << "  \n";
		//cout << " v b size =" << AR_map_Stock[stock].getARreturn().size() << endl;
		//cout << " v a size = " << temp.size() << endl;
		if (AR_map_Stock[stock].getARreturn().size() == 90 )
		{
			temp += AR_map_Stock[stock].getARreturn(); // vector of 91 days
		}
		else
		{
			i--;
		}
	}
	temp /= numberPick; // divide each element by numberPick(60) to get Average AR
	return temp;
}

vector<vector<vector<double>>> measurer(vector<map<int, string>> &sortMap, map<string, Stock> &AR_map_Stock) {
	vector<vector<double>> AARvector(3), CAARvector(3);
	//const int groupkind = group.size();
	const int groupkind = 3;
	vector<vector<double>> avrAARvector(groupkind), avrCAARvector(groupkind);
	const int simultrial = 30; // we do bootstrapping 30 times
	unsigned int numberPick = 60; // we pick 60 stocks from each group
	//vector<double> aar, caar, avrCaar;
	for (unsigned int i = 0; i < simultrial; i++) {
		for (unsigned int j = 0; j < groupkind; j++) { // bootstrapping, 60 per each group
			AARvector[j] = BootstrapAAR(sortMap[j], numberPick, AR_map_Stock); // stores vector<double> for each map slot
			CAARvector[j] = CAARer(AARvector[j]);// with the AAR vector of the group j, get CAAR[j] here
			// we have AARvector and CAARvector for each group (total 6!). now we need to average this by 30 trials
			avrAARvector[j].resize(AARvector[j].size());
			avrCAARvector[j].resize(AARvector[j].size());
			avrAARvector[j] += AARvector[j];
			avrCAARvector[j] += CAARvector[j];
		}
	}
	// as above avr~ vectors are 30 trial's summation per each time period, we need to devide by 30
	for (unsigned int j = 0; j < groupkind; j++) {
		avrAARvector[j] /= simultrial;
		avrCAARvector[j] /= simultrial;
	}

	vector<vector<vector<double>>> AR_CAAR_Matrix;
	for (unsigned int s = 0; s < 3; s++)
	{
		vector<vector<double>> temp;
		temp.push_back(avrAARvector[s]);
		temp.push_back(avrCAARvector[s]);

		AR_CAAR_Matrix.push_back(temp);

		temp.clear();
	}

	/*

	const int totalDay = 91; // is there a way of getting this number out of the given input params?
	double sumAAR[groupkind];
	for (int j = 0; j != groupkind; j++) {
		for (int day = 0; day != totalDay; day++) // anyways, need to get average AAR for the whole timeframe
		{
			sumAAR[j] = sumAAR[j] + avrAARvector[j][day];
		}
		sumAAR[j] /= totalDay;
	}
	// prints table : menu 4
	cout << "Group       AAR       CAAR" << endl;
	cout << "Beat " << setw(10) << sumAAR[0] << avrCAARvector[0][90] << endl;
	cout << "Meet " << setw(10) << sumAAR[1] << avrCAARvector[1][90] << endl;
	cout << "Miss " << setw(10) << sumAAR[2] << avrCAARvector[2][90] << endl;

	*/
	return AR_CAAR_Matrix; // this map will only be needed to draw the graph
}

vector<double> CAARer(vector<double> AARvector) {
	vector<double> result;
	for (unsigned int i = 0; i < AARvector.size(); i++) {
		if (i == 0) result.push_back(AARvector[i]);
		else result.push_back(result[i - 1] + AARvector[i]);
	}
	return result;
}

#endif
