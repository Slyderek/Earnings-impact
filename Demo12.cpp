// Demo12.cpp : Defines the entry point for the console application.
//

//#include "EPS.h"
#include "Stock.h"
#include "stdafx.h"
#include "Data.h"
#include "Operator.h"
#include "Working.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <istream>
#include <random>
#include "ShowData.h"

using namespace std;

void stockinfo(map<string, Stock> :: iterator itrm);
vector<string> StockMatchDate(vector<string> date, string);

int main()
{

	cout << " ***** Financial Computing Team 6 Project *****";

	SPY *YahooData = new SPY;
	Stock *stockData = new Stock;

	map <string, SPY> spySeries;
	map <string, Stock> stockSeries;
	map <string, Stock> ::iterator itrf;

	vector<map<int, string>> BootStrapGroup;
	vector<map<string, class Stock>> MatchedGroup;
	vector<vector<vector<double>>> matrix;

	vector<string> group = { "beat", "meet", "miss" };
	
	int option;

	
	do
	{
		cout << "**MENU Options:**" << endl;
		cout << "Option 1: Retrieve historical price data for all stocks and than do the grouping" << endl;
		cout << "Option 2: Pull stock's information from one group" << endl;
		cout << "Option 3: Show AAR or CAAR for one group" << endl;
		cout << "Option 4: Show the Excel graph with CAAR for all 3 groups" << endl;
		cout << "Option 5: Exit this program" << endl << endl;

		cout << "Enter your choice: ";
		cin >> option;

		if (option == 1)
		{
			YahooData->setSymbolSPY("^GSPC");
			YahooData->Data(spySeries);
			cout << " SPY downloaded " << endl;
			//cout << "  " << spySeries["^GSPC"].getdateSPY().size() << "  " << spySeries["^GSPC"].getpriceSPY().size() << endl;

			cout << "downloading....stocks...." << endl << endl;
			remove("Results.txt"); // ?
			string symbol, date2;
			double est_eps, act_eps;
			vector<string> symbols;
			ifstream filein;
			ofstream fileout;
			filein.clear();
			filein.open("BEST.txt");
			while (filein >> symbol >> est_eps >> act_eps >> date2) { // until what?? (what's inside the ()??)
				symbols.push_back(symbol); // reading and storing all the tickers inside the vector<string>
				stockData->setSymbol(symbol);
				//cout << date2.size() << endl;
				stockData->setAnnounce(date2);
				stockData->setdate(StockMatchDate(spySeries["^GSPC"].getdateSPY(), stockData->getannounce()));
				stockData->Data(stockSeries);
			}

			cout << "data for all the stocks downloaded!" << endl;

			cout << "historical prices for all stocks are  successfully retreived!" << endl;

			cout << "now grouping of the stocks in 3 categories starts...." << endl << endl;
			BootStrapGroup = sortGroupBootStrap(group, stockSeries);
			//cout << " boot grp size = " << BootStrapGroup.size() << endl;

			/*
			for (itrf = stockSeries.begin(); itrf != stockSeries.end(); itrf++)
			{
				cout << endl << itrf->first << " " << (itrf->second.getprice()).size() << "  " << (itrf->second.getdate()).size() << endl;
			}
			*/

			spy500(stockSeries, spySeries["^GSPC"].getpriceSPY());
			cout << "grouping complete." << endl;
			
		}
		
		else if (option == 2)
		{
			cout << "\nenter the ticker of the stock you want to get information of" << endl;
			string ticker;
			cin >> ticker;
			
			cout << "time series for the " << ticker << " stock is as following." << endl << endl;
			cout << "  date               stock price  " << endl;
			for (unsigned int i = 0; i < (stockSeries[ticker].getdate()).size(); i++)
			{
				cout << stockSeries[ticker].getdate()[i] << "        " << stockSeries[ticker].getprice()[i] << endl;
			}
			cout << endl;
			cout << "informationn for the given stock is as following(from -60 days to +30 days from the announcement date)." << endl;
			map<string, Stock> :: iterator itrm;
			itrm = stockSeries.find(ticker);
			stockinfo(itrm);
			
		}
		else if (option == 3)
		{
			/*
			map<string, Stock> ::iterator itrg;
			for (itrg = stockSeries.begin(); itrg != stockSeries.end(); itrg++)
			{
				cout << itrg->first << "  " << itrg->second.getARreturn().size() << endl;
			}
			*/
			cout << " boot grp size = " << BootStrapGroup.size() << endl;
			matrix = measurer(BootStrapGroup, stockSeries);
			int group;
			cout << "\n For Beat group enter N = 0. ";
			cout << "\n For Meet group enter N = 1. ";
			cout << "\n For Miss group enter N = 2. ";
			cout << "\nenter the group number from the above mentioned groups." << endl;
			cin >> group;
			cout << "AAR and CAAR for the group " << group << " is as following(for  90  reference days). " << endl;
			cout << "\n      AAR                          CAAR\n";
			for (unsigned int i = 0; i < matrix[group][0].size(); i++)
			{
				cout << i+1 << "  " << matrix[group][0][i] << "                  " << matrix[group][1][i] << endl;
			}
			cout << "\n option 3 finished.\n select the next option." << endl;
		}
		else if (option == 4)
		{
			ShowPic(matrix[0][1], matrix[1][1], matrix[2][1]);
			cout << " Excel graph with CAAR for all 3 groups will be shown shortly!" << endl;
		}
		else if (option == 5)
		{
			cout << "Exiting this program." << endl;
			break;
		}
		else
		{
			cout << "please input a number from 1 to 5!" << endl << "if you want to exit, please input 5" << endl;
		}

	} while (option != 5);

	
	cout << "/n here it ends";
	system("pause");

	return 0;
}

void stockinfo(map<string, Stock> ::iterator itrm)
{
	cout << "Ticker = " << itrm->second.getsymbol() << endl;
	cout << "Announcement date = " << itrm->second.getannounce() << endl;
	cout << "Estimated EPS = " << itrm->second.getesteps() << endl;
	cout << "Actual Earnings = " << itrm->second.getacteps() << endl;
	cout << " Group = " << itrm->second.getgroup() << endl;
	cout << "Abnormal Returns are as following (for thr 90 days period)." << endl << endl;
	cout << "  date            Abnormal Returns  " << endl;
	for (unsigned int i = 0; i < (itrm->second.getdate()).size()-1; i++)
	{
		cout << itrm->second.getdate()[i] << "        " << itrm->second.getARreturn()[i] << endl;
	}
}

vector<string> StockMatchDate(vector<string> datef, string announce)
{
	vector<string> vdate;
	unsigned int index = 0;
	vector<string> proxy;
	for (unsigned int i = 0; i < datef.size(); i++)
	{
		if (datef[i] == announce)
		{
			index = i;
		}
	}

	for (unsigned int j = index - 60; j <= index + 30; j++) {
		proxy.push_back(datef[j]);
	}
	vdate = proxy;
	return vdate;
}

