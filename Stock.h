#ifndef Stock_h
#define Stock_h

#include "stdafx.h"
#include "Data.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include "Working.h"
using namespace std;

class Stock;

class SPY {
protected:
	string symbolSPY; // ticker
	vector<double> priceSPY;
	vector<string> dateSPY;

	FILE *fp;

	const char outfilename[FILENAME_MAX] = "Output.txt";
	const char resultfilename[FILENAME_MAX] = "Results.txt";

	CURL *handle;
	CURLcode result;

	string sCookies, sCrumb;
	
public:
	void setSymbolSPY(string symbol_);

	SPY() 
	{ 
		curl_global_init(CURL_GLOBAL_ALL); 
		handle = curl_easy_init(); 
	}

	void setdateSPY(vector<string> date_) { dateSPY = date_; }
	void setpriceSPY(vector<double> price_) { priceSPY = price_; }
	vector<double> getpriceSPY() const { return priceSPY; }
	vector<string> getdateSPY() const { return dateSPY; }
	string getsymbolSPY() const { return symbolSPY; }
	//void input();

	int Data(map <string, SPY> &stockseries);

	~SPY()
	{ 
		curl_easy_cleanup(handle);
		curl_global_cleanup();
	}
};

class Stock {
private:
	string symbol; // ticker
	vector<double> price;
	vector<string> date;

	double est_eps;
	double act_eps;
	double eps_percentage;
	string announce;
	string group;
	vector<double> spy, stockreturn, spyreturn, ARreturn;

	FILE *fp;

	const char outfilename[FILENAME_MAX] = "Output.txt";
	const char resultfilename[FILENAME_MAX] = "Results.txt";

	CURL *handle;
	CURLcode result;

	string sCookies, sCrumb;

public:
	Stock()
	{
		curl_global_init(CURL_GLOBAL_ALL);
		handle = curl_easy_init();
	}

	// setters
	void setvalue(string symbol_, double est_eps, double act_eps, string announce_);
	//void setvalue(string symbol_, map<string, Stock> myMap, vector<double> spy_);
	void setgroup(string group_) { group = group_; }

	void setSymbol(string symbol_) { symbol = symbol_; }
	void setdate(vector<string> date_) { date = date_; }
	void setprice(vector<double> price_) { price = price_; }
	void setARSPYreturn(vector<double> samplevector) { ARreturn = samplevector; }
	void setAnnounce(string ann) { announce = ann; }

	// getters
	double getpercentage() const { return eps_percentage; }
	double getacteps() const { return act_eps; }
	double getesteps() const { return est_eps; }
	string getannounce() const { return announce; }
	string getgroup() const { return group; }
	vector<double> getspy() const { return spy; }
	vector<double> getstockreturn() const { return stockreturn; }
	vector<double> getspyreturn() const { return spyreturn; }
	vector<double> getARreturn() const { return ARreturn; }

	vector<double> getprice() const { return price; }
	vector<string> getdate() const { return date; }
	string getsymbol() const { return symbol; }

	// methods
	void matchdate(vector<double> spy_);
	void calculation();


	void operator=(const Stock &right)
	{
		symbol = right.getsymbol();
		price = right.getprice();
		date = right.getdate();
		est_eps = right.getesteps();
		act_eps = right.getacteps();
		eps_percentage = right.getpercentage();
		announce = right.getannounce();
		group = right.getgroup();
		spy = right.getspy();
		stockreturn = right.getstockreturn();
		spyreturn = right.getspyreturn();
		ARreturn = right.getARreturn();
	}

	int Data(map <string, Stock> &stockseries);

	~Stock()
	{
		curl_easy_cleanup(handle);
		curl_global_cleanup();
	}

};

void Stock::setvalue(string symbol_, double est_eps_, double act_eps_, string announce_) {
	symbol = symbol_;
	est_eps = est_eps_;
	act_eps = act_eps_;
	announce = announce_;
	if (est_eps >= 0) {
		eps_percentage = act_eps / est_eps - 1;
	}
	else {
		eps_percentage = 1 - act_eps / est_eps; 
	}
	//input();
}

//void Stock::setvalue(string symbol_, map<string, class Stock> &myMap, vector<double> spy_) {
	//symbol = symbol_;
	//announce = myMap.find(symbol)->second.getannounce();
	//est_eps = myMap.find(symbol)->second.getesteps();
	//act_eps = myMap.find(symbol)->second.getacteps();
	//date = myMap.find(symbol)->second.getdate();
	//group = myMap.find(symbol)->second.getgroup();
	//price = myMap.find(symbol)->second.getprice();
	//spy = spy_;
	//matchdate();
//}

/*

void SPY::input() {
	string date1;
	double price1;
	vector<string> proxy1;
	vector<double> proxy2;
	ifstream filein;
	filein.open(symbol + ".txt");
	while (filein >> date1 >> price1) {
		proxy1.push_back(date1);
		proxy2.push_back(price1);
	}
	filein.close();
	price = proxy2;
	date = proxy1;
}

*/

void SPY::setSymbolSPY(string symbol_) {
	symbolSPY = symbol_;
	//input(); // why this?
}

//moving the date 60 backward and 30 forward
void Stock::matchdate(vector<double> spy_) {
	spy = spy_;
	unsigned int index = 0;
	vector<double> proxy2;
	for (unsigned int i = 0; i < date.size(); i++) {
		if (date[i] == announce) {
			index = i;
		}
	}
	for (unsigned int j = index - 60; j <= index + 30; j++) {
		proxy2.push_back(spy[j]);
	}
	spy = proxy2;
}

void Stock::calculation() {
	double stockreturn1, spyreturn1, ARreturn1;
	vector<double> proxy, proxy1, proxy2;
	for (unsigned int i = 0; i < date.size() - 1; i++) {
		stockreturn1 = price[i + 1]/price[i] - 1;
		spyreturn1 = spy[i + 1]/spy[i] - 1;
		ARreturn1 = stockreturn1 - spyreturn1;

		proxy.push_back(stockreturn1);
		proxy1.push_back(spyreturn1);
		if (ARreturn1 > 1 || ARreturn1 < -1)
		{
			ARreturn1 = proxy2.back();
		}
		proxy2.push_back(ARreturn1);
	}

	stockreturn = proxy;
	spyreturn = proxy1;
	ARreturn = proxy2;
}

int SPY::Data(map <string, SPY> &stockseries) {


	string startTime = getTimeinSeconds("2017-07-05T16:00:00");
	string endTime = getTimeinSeconds("2018-05-05T16:00:00");

	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;

	//file pointer to create file that store the data  
	//FILE *fp;

	//name of files  
	//const char outfilename[FILENAME_MAX] = "Output.txt";
	//const char resultfilename[FILENAME_MAX] = "Results.txt";

	/* declaration of an object CURL */
	//CURL *handle;

	/* result of the whole process */
	//CURLcode result;

	/* the first functions */
	/* set up the program environment that libcurl needs */
	//curl_global_init(CURL_GLOBAL_ALL);

	/* curl_easy_init() returns a CURL easy handle that you're gonna reuse in other easy functions*/
	//handle = curl_easy_init();
	//string sCookies, sCrumb;
	/* if everything's all right with the easy handle... */
	if (handle)
	{
		if (sCookies.length() == 0 || sCrumb.length() == 0)
		{
			fp = fopen(outfilename, "w");
			//curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
			curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
			curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

			curl_easy_setopt(handle, CURLOPT_ENCODING, "");
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
			result = curl_easy_perform(handle);
			fclose(fp);

			/* Check for errors */
			if (result != CURLE_OK)
			{
				/* if errors have occurred, tell us what is wrong with 'result'*/
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);

			/* perform, then store the expected code in 'result'*/
			result = curl_easy_perform(handle);

			/* Check for errors */
			if (result != CURLE_OK)
			{
				/* if errors have occured, tell us what is wrong with 'result'*/
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}

			char cKey[] = "CrumbStore\":{\"crumb\":\"";
			char *ptr1 = strstr(data.memory, cKey);
			char *ptr2 = ptr1 + strlen(cKey);
			char *ptr3 = strstr(ptr2, "\"}");
			if (ptr3 != NULL)
				*ptr3 = NULL;

			sCrumb = ptr2;

			fp = fopen("cookies.txt", "r");
			char cCookies[100];
			if (fp) {
				while (fscanf(fp, "%s", cCookies) != EOF);
				fclose(fp);
			}
			else
				cerr << "cookies.txt does not exists" << endl;

			sCookies = cCookies;
			free(data.memory);
			data.memory = NULL;
			data.size = 0;
		}

		//if (itr == SymbolList.end())
		//break;

		string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
		//string symbol_ = Symbol1;
		string urlB = "?period1=";
		string urlC = "&period2=";
		string urlD = "&interval=1d&events=history&crumb=";
		string url = urlA + symbolSPY + urlB + startTime + urlC + endTime + urlD + sCrumb;
		const char * cURL = url.c_str();
		const char * cookies = sCookies.c_str();
		curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
		curl_easy_setopt(handle, CURLOPT_URL, cURL);
		fp = fopen(resultfilename, "ab");  // going to write Result.txt!
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp); // here!
		result = curl_easy_perform(handle);
		fclose(fp);

		/* Check for errors */
		if (result != CURLE_OK)
		{
			/* if errors have occurred, tell us what is wrong with 'result'*/
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return 1;
		}
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
		result = curl_easy_perform(handle);

		/* Check for errors */
		if (result != CURLE_OK)
		{
			/* if errors have occurred, tell us what is wrong with 'result'*/
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return 1;
		}

		/*stringstream sData;
		sData.str(data.memory);
		string line;
		getline(sData, line);
		cout << line << endl;
		for (; getline(sData, line); )
		cout << line << endl;*/


		stringstream sData;
		sData.str(data.memory);
		string sValue, sDate;
		double dValue = 0;
		string line;
		getline(sData, line);
		while (getline(sData, line)) {
			//cout << line << endl;
			//sDate = line.substr(0, line.find_first_of(','));
			//line.erase(line.find_last_of(','));
			//sValue = line.substr(line.find_last_of(',') + 1);
			//dValue = strtod(sValue.c_str(), NULL);
			//cout << " " << std::fixed << ::setprecision(6) << dValue << endl;

			getline(sData, line, ',');
			sDate = line;

			getline(sData, line, ',');
			getline(sData, line, ',');
			getline(sData, line, ',');
			getline(sData, line, ',');
			getline(sData, line, ',');

			sValue = line;
			dValue = strtod(sValue.c_str(), NULL);

			dateSPY.push_back(sDate);
			priceSPY.push_back(dValue);
		}
		free(data.memory);
		data.size = 0;
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}

	stockseries[symbolSPY].setdateSPY(dateSPY);
	stockseries[symbolSPY].setpriceSPY(priceSPY);
	
	dateSPY.clear();
	priceSPY.clear();

	/* cleanup since you've used curl_easy_init */
	//curl_easy_cleanup(handle);

	/* this function releases resources acquired by curl_global_init() */
	//curl_global_cleanup();

	return 0;
}


int Stock::Data(map <string, Stock> &stockseries) {

	string startTime = getTimeinSeconds(getdate().front() + "T16:00:00");
	string endTime = getTimeinSeconds(getdate().back() + "T16:00:00");


	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;

	//file pointer to create file that store the data  
	//FILE *fp;

	//name of files  
	//const char outfilename[FILENAME_MAX] = "Output.txt";
	//const char resultfilename[FILENAME_MAX] = "Results.txt";

	/* declaration of an object CURL */
	//CURL *handle;

	/* result of the whole process */
	//CURLcode result;

	/* the first functions */
	/* set up the program environment that libcurl needs */
	//curl_global_init(CURL_GLOBAL_ALL);

	/* curl_easy_init() returns a CURL easy handle that you're gonna reuse in other easy functions*/
	//handle = curl_easy_init();
	//string sCookies, sCrumb;
	/* if everything's all right with the easy handle... */
	if (handle)
	{
		if (sCookies.length() == 0 || sCrumb.length() == 0)
		{
			fp = fopen(outfilename, "w");
			//curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
			curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
			curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

			curl_easy_setopt(handle, CURLOPT_ENCODING, "");
			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
			result = curl_easy_perform(handle);
			fclose(fp);

			/* Check for errors */
			if (result != CURLE_OK)
			{
				/* if errors have occurred, tell us what is wrong with 'result'*/
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);

			/* perform, then store the expected code in 'result'*/
			result = curl_easy_perform(handle);

			/* Check for errors */
			if (result != CURLE_OK)
			{
				/* if errors have occured, tell us what is wrong with 'result'*/
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}

			char cKey[] = "CrumbStore\":{\"crumb\":\"";
			char *ptr1 = strstr(data.memory, cKey);
			char *ptr2 = ptr1 + strlen(cKey);
			char *ptr3 = strstr(ptr2, "\"}");
			if (ptr3 != NULL)
				*ptr3 = NULL;

			sCrumb = ptr2;

			fp = fopen("cookies.txt", "r");
			char cCookies[100];
			if (fp) {
				while (fscanf(fp, "%s", cCookies) != EOF);
				fclose(fp);
			}
			else
				cerr << "cookies.txt does not exists" << endl;

			sCookies = cCookies;
			free(data.memory);
			data.memory = NULL;
			data.size = 0;
		}

		//if (itr == SymbolList.end())
		//break;

		string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
		//string symbol_ = Symbol1;
		string urlB = "?period1=";
		string urlC = "&period2=";
		string urlD = "&interval=1d&events=history&crumb=";
		string url = urlA + symbol + urlB + startTime + urlC + endTime + urlD + sCrumb;
		const char * cURL = url.c_str();
		const char * cookies = sCookies.c_str();
		curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
		curl_easy_setopt(handle, CURLOPT_URL, cURL);
		fp = fopen(resultfilename, "ab");  // going to write Result.txt!
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp); // here!
		result = curl_easy_perform(handle);
		fclose(fp);

		/* Check for errors */
		if (result != CURLE_OK)
		{
			/* if errors have occurred, tell us what is wrong with 'result'*/
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return 1;
		}
		curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
		curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
		result = curl_easy_perform(handle);

		/* Check for errors */
		if (result != CURLE_OK)
		{
			/* if errors have occurred, tell us what is wrong with 'result'*/
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
			return 1;
		}

		/*stringstream sData;
		sData.str(data.memory);
		string line;
		getline(sData, line);
		cout << line << endl;
		for (; getline(sData, line); )
		cout << line << endl;*/


		stringstream sData;
		sData.str(data.memory);
		string sValue, sDate;
		double dValue = 0;
		string line;
		getline(sData, line);
		while (getline(sData, line)) {
			//cout << line << endl;
			//sDate = line.substr(0, line.find_first_of(','));
			//line.erase(line.find_last_of(','));
			//sValue = line.substr(line.find_last_of(',') + 1);
			//dValue = strtod(sValue.c_str(), NULL);
			//cout << " " << std::fixed << ::setprecision(6) << dValue << endl;

			getline(sData, line, ',');
			sDate = line;

			getline(sData, line, ',');
			getline(sData, line, ',');
			getline(sData, line, ',');
			getline(sData, line, ',');
			getline(sData, line, ',');

			sValue = line;
			dValue = strtod(sValue.c_str(), NULL);

			//date.push_back(sDate);
			price.push_back(dValue);
		}
		free(data.memory);
		data.size = 0;
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}
	//cout << price.size() << endl;
	stockseries[symbol].setdate(date);
	stockseries[symbol].setprice(price);

	if (stockseries[symbol].getprice().size() < 80)
	{
		stockseries.erase(symbol);
	}

	price.clear();

	/* cleanup since you've used curl_easy_init */
	//curl_easy_cleanup(handle);

	/* this function releases resources acquired by curl_global_init() */
	//curl_global_cleanup();

	return 0;
}


#endif