#pragma once
#include<stdio.h>
#include "ACOUNT.h"
#include<Windows.h>
#define PERCENT false
#define PRICE true
class CORE {
private:

	ACOUNT acount;
	
	bool D = false;
	bool P = false; 
	int X =0;
	int Y =0;
	int Z = 0;
	char code[10];
	char servername[100];
	char* env;
	
public:

	void SetP(bool p) { P = p; }

	void MakePath(char* filename,char *ret) {
		strcpy(ret, env);
		strcat(ret, "\\");
		strcat(ret, filename);
	}

	void GetCode() {
		char cmd[255];
		if (!acount.HaveStock())
		{
			sprintf(cmd, "py %s\\up.py > %s\\up.txt",env,env);
			system(cmd);
			char file[255];
			MakePath("up.txt", file);
			FILE *in = fopen(file, "r");
			if (in) {
				for (int i = 0; i < 5; i++)
					fscanf(in, "%s", code);
				printf("code:%s\n", code);
				fclose(in);
			}
			else {
				printf("error!\n");
			}
		}
		if (code) {
			sprintf(cmd, "hstcp %s -x %s", servername, code);
			system(cmd);
		}
	}

	void Run() {
		
		printf("%s\n", servername);
		load();
		
		do{
			GetCode();
			Sleep(500);
		} while (Collect());
	}

	int Collect() {
		char file[255];
		MakePath("x.txt", file);
		FILE *in=fopen(file, "r");
		int prc=0;
		double ctrt=0;
		long long m = 0;
		if (in) {
			fscanf(in,"%d %lf %lld",&prc,&ctrt,&m);
			fclose(in);
		}
		else {
			printf("x.txt read fail.\n");
			return 0;
		}
		if (prc&&ctrt&&m) {

			if (!acount.HaveStock() && ctrt >= X && m >= Z) {//BUY
				Buy(prc);
			}
			if (P) {
				if (acount.HaveStock() && prc >= Y) {
					Sell(prc);
					return 0;
				}
			}
			else {
				double r = ((double)(prc - acount.GetStockPrice()) / (double)acount.GetStockPrice())*100.0f;
				if (acount.HaveStock() && r>= Y) {//SELL
					Sell(prc);
					return 0;
				}
			}
		}
		return 1;
	}

	void save(int prc) {
		char file[255];
		MakePath("buy.txt", file);
		FILE *out = fopen(file, "w");
		fprintf(out,"%s %d",code,prc);
		fclose(out);
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
		char cmd[255];
		int cnt = acount.GetStockCnt();
		sprintf(cmd,"hstcp %s -s %s %d %d",servername,code,prc,cnt);
		acount.Sell(prc, cnt);
		system(cmd);
	}

	void Buy(int prc) {
		char cmd[255];
		int cnt = acount.GetMyMoney() / prc;
		sprintf(cmd,"hstcp %s -b %s %d %d",servername,code,prc,cnt);
		acount.Buy(prc, cnt);
		system(cmd);
		save(prc);
	}
	CORE() {
		env = getenv("HSWORD_HOME");
		acount.SetMoney(10000000);
		strcpy(servername, "localhost");
	}

	CORE(int x,int y,int z) {
		this->CORE::CORE();
		X = x;
		Y = y;
		Z = z;
		printf("X=%d\n", X);
		printf("Y=%d\n", Y);
		printf("Z=%d\n", Z);
		if (Y > 10)SetP(true);
	}

	void SetMoney(int m) {
		acount.SetMoney(m);
		if (Z <= 10)Z = Z*m;
	}

	void SetSERVERNAME(char* s)
	{
		strcpy(servername, s);
	}

	void SetX(int x,int y) {
		X = x;
		Y = y;
	}
	
	void Debug(bool d) {
		D = d;
		acount.Debug(d);
	}
	void SetAcount(int m) {
		acount.SetMoney(m);
	}
	
	void Print() {
			printf("MY MONEY : %d\n", acount.GetMyMoney());
	}
};