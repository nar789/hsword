#pragma once
#include<stdio.h>
#include <iostream>
#include "ACOUNT.h"
#include "UTILS.h"
#include<Windows.h>
#include <cmath>

#define PERCENT false
#define PRICE true
#define CODE 6
#define PORT 4
#define STARTHOUR 9
#define STARTMINUTE 1
#define CODEPOS 0

using namespace std;

class CORE {
private:

	ACOUNT acount;
	bool D = false; bool P = false; 
	int X =0;int Y =0;int Z = 0;
	char code[255];
	double topratio=0.0f;
	char servername[100];
	char* env;
	char port[255];
	int cnt = 0;
	
public:
	void SetP(bool p) { P = p; }
	void MakePath(char* filename,char *ret) {
		strcpy(ret, env);
		strcat(ret, "\\..\\");
		strcat(ret, port);
		strcat(ret, "\\");
		strcat(ret, filename);
	}
	void GetCode() {
		char cmd[255];
		if (!acount.HaveStock())
		{
			strcpy(code, "");
			printf("%02d:%02d\n",Utils::CurrentGetHour(),Utils::CurrentGetMinute());
			if (Utils::CurrentGetHour()>=STARTHOUR && Utils::CurrentGetMinute()>=STARTMINUTE) {
				
				if (Utils::CurrentGetMinute() >= STARTMINUTE + 30) {printf("\nLimit time is over.\n");exit(0);}

				char file[255];
				strcpy(file, env);
				strcat(file, "\\up.txt");
				FILE *in = fopen(file, "r");

				if (in) {
					fscanf(in, "%s %lf", code,&topratio);
					printf("code:%s %.2f\n", code, topratio);
					fclose(in);
				}
			}
			else return;
		}
		
		if (strlen(code)==CODE && code[0]>='0' && code[0]<='9') {
			
			char uplog[255];
			MakePath("uplog.txt", uplog);
			FILE *in = fopen(uplog, "a");
			if (in) {
				fprintf(in, "%s %02d:%02d:%02d\n", code, Utils::CurrentGetHour(), Utils::CurrentGetMinute(), Utils::CurrentGetSecond());
				fclose(in);
			}
			printf("%s %s\n",servername,code);
			sprintf(cmd, "hstcp %s -x %s", servername, code);
			system(cmd);
		}
	}

	void Run() {
		
		printf("%s\n", servername);
		load();
		if (strlen(port) != PORT)return;
		do{
			MSG msg;
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				GetCode();
				fflush(stdout);
			}
			printf("[%d]",cnt++);
			if (cnt > pow(10, 8))cnt = 0;
			Sleep(10);
		} while (Collect());
	}

	int Collect() {
		if (strlen(code) != CODE || code[0]<'0' || code[0]>'9')
			return true;
		char file[255];
		MakePath("x.txt", file);
		FILE *in=fopen(file, "r");
		int prc=0;
		double ctrt=0;
		int hgpr = 0;
		
		if (in) { fscanf(in, "%d %lf %d", &prc, &ctrt, &hgpr); fclose(in);
		} else { printf("x.txt read fail.\n");return 0; }

		if (prc && ctrt && hgpr) {
			
			bool close = (((double)abs(hgpr - prc) / (double)GetUnit(prc)) <= 1.0f) ? true : false;
			if (!acount.HaveStock() && ctrt >= X && close ) Buy(prc);
			
			int r = 0;
			if (acount.GetStockPrice()) r = prc - acount.GetStockPrice();

			if (P) { if (acount.HaveStock() && prc >= Y) {Sell(prc);return 0;}
			}
			else {	if (acount.HaveStock() && r>= GetUnit(prc)*Y) {//SELL
					acount.Sell(prc, acount.GetStockCnt());
					return 0; }
			}

			if (acount.HaveStock() && r <= -(GetUnit(prc)*Y)){ Sell(prc);return 0; }
		}
		return 1;
	}

	void save(int prc) {
		char file[255];
		MakePath("buy.txt", file);
		FILE *out = fopen(file, "w");
		if (out) {
			fprintf(out, "%s %d", code, prc);
			fclose(out);
		}
	}

	void load() {
		char file[255];
		MakePath("buy.txt", file);
		FILE *in = fopen(file, "r");
		if (in) {
			int p;
			fscanf(in,"%s %d",code,&p);
			printf("Loaded:%s %d.\n",code,p);
			int cnt = acount.GetMyMoney() / p;
			acount.Buy(p, cnt);
			fclose(in);
		}
	}

	void Sell(int prc) {
		int unit = GetUnit(prc);
		const int level = 3;
		prc -= unit*level;
		char cmd[255];
		int cnt = acount.GetStockCnt();
		sprintf(cmd,"hstcp %s -s %s %d %d",servername,code,prc,cnt);
		system(cmd);
		prc += unit*level;
		acount.Sell(prc, cnt);
	}

	void directSell(int prc) {
		char cmd[255];
		int cnt = acount.GetStockCnt();
		sprintf(cmd, "hstcp %s -s %s %d %d", servername, code, prc, cnt);
		system(cmd);
	}

	int GetUnit(int &prc) {
		int unit = 1;
		if (prc >= 500000)
			unit = 1000;
		else if (prc >= 100000)
			unit = 500;
		else if (prc >= 50000)
			unit = 100;
		else if (prc >= 10000)
			unit = 50;
		else if (prc >= 5000)
			unit = 10;
		else if (prc >= 1000)
			unit = 5;
		else
			unit = 1;
		return unit;
	}

	void Buy(int prc) {
		int unit = GetUnit(prc);
		const int level = 3;
		prc += unit*level;
		char cmd[255];
		int cnt = acount.GetMyMoney() / prc;
		sprintf(cmd,"hstcp %s -b %s %d %d",servername,code,prc,cnt);
		system(cmd);
		prc -= unit*level;
		acount.Buy(prc, cnt);
		save(prc);
		directSell(prc+unit*Y);
	}

	CORE() {
		env = getenv("HSWORD_HOME");
		acount.SetMoney(10000000);
		strcpy(servername, "localhost");
	}
	CORE(int x,int y,int z) {
		this->CORE::CORE();
		X = x;Y = y;Z = z;
		printf("X=%d\n", X);
		printf("Y=%d\n", Y);
		printf("Z=%d\n", Z);
		if (Y > 10)SetP(true);
		if (Z <= 10)Z = Z * 10000;
	}
	void SetMoney(int m) {acount.SetMoney(m);}
	void SetSERVERNAME(char* s)
	{
		strcpy(servername, s);
		int psi = strlen(s);
		int pi = 0;
		for (int i = 0; i < strlen(s); i++) {
			if (s[i] == ':')
				psi = i;
			if (psi < i) {
				port[pi++] = s[i];
			}
		}
		port[pi] = 0;
	}
	void SetX(int x,int y) {X = x;Y = y;}
	void Debug(bool d) {D = d;acount.Debug(d);}
	void SetAcount(int m) {acount.SetMoney(m);}
	void Print() {printf("MY MONEY : %d\n", acount.GetMyMoney());}
};