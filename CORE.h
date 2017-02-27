#pragma once
#include<stdio.h>
#include "ACOUNT.h"
#include "UTILS.h"
#include<Windows.h>
#define PERCENT false
#define PRICE true
#define CODE 6
#define PORT 4
#define ENDHOUR 10
#define ENDMINUTE 10
class CORE {
private:

	ACOUNT acount;
	
	bool D = false;
	bool P = false; 
	int X =0;
	int Y =0;
	int Z = 0;
	char code[255];
	char servername[100];
	char* env;
	char port[255];
	int codepos = 5;
	
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
			char file[255];
			char out[255];
			MakePath("up.py",file);
			MakePath("up.txt", out);
			sprintf(cmd, "py %s > %s",file,out);
			system(cmd);
			MakePath("up.txt", file);
			FILE *in = fopen(file, "r");
			if (in) {
				for (int i = 0; i < codepos; i++)
					fscanf(in, "%s", code);
				printf("code:%s\n", code);
				fclose(in);
			}
			else {
				printf("error!\n");
			}
		}
		
		if (strlen(code)==CODE && code[0]>='0' && code[0]<='9') {
			char uplog[255];
			MakePath("uplog.txt", uplog);
			FILE *in = fopen(uplog, "a");
			fprintf(in,"%s %2d:%2d:%2d\n",code,Utils::CurrentGetHour(),Utils::CurrentGetMinute(),Utils::CurrentGetSecond());
			fclose(in);
			sprintf(cmd, "hstcp %s -x %s", servername, code);
			system(cmd);
		}
		else
		{
			sprintf(cmd, "hstcp %s -e", servername);
			system(cmd);
		}
	}

	void Run() {
		
		printf("%s\n", servername);
		load();
		if (strlen(port) != PORT)
			return;
		do{
			GetCode();
			Sleep(500);
		} while (Collect());
	}

	int Collect() {
		if (strlen(code) != CODE || code[0]<'0' || code[0]>'9')
			return true;
		char file[255];
		MakePath("x.txt", file);
		FILE *in=fopen(file, "r");
		int prc=0;
		int sprc = 0;
		double ctrt=0;
		long long m = 0;
		if (in) {
			fscanf(in, "%d %lf %lld %d", &prc, &ctrt, &m, &sprc);
			m *= prc;
			fclose(in);
		}
		else {
			printf("x.txt read fail.\n");
			return 0;
		}
		if (prc&&ctrt&&m) {

			if (!acount.HaveStock() && ctrt >= X && m >= Z) {//BUY

				if (sprc < prc)
					Buy(prc);
				else
					return 1;
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

			if (acount.HaveStock() && acount.GetStockPrice()>prc)
			{
				Sell(prc);
				return 0;
			}
			if ((Utils::CurrentGetHour() >= ENDHOUR && Utils::CurrentGetMinute() > ENDMINUTE) || Utils::CurrentGetHour()>ENDHOUR)
			{
				if(acount.HaveStock())
					Sell(prc);
				return 0;
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
		int unit = 0;
		int level = 3;
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
		prc -= unit*level;
		char cmd[255];
		int cnt = acount.GetStockCnt();
		sprintf(cmd,"hstcp %s -s %s %d %d",servername,code,prc,cnt);
		system(cmd);
		prc += unit*level;
		acount.Sell(prc, cnt);
		
	}

	void Buy(int prc) {
		int unit = 0;
		int level = 3;
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
		prc += unit*level;
		char cmd[255];
		int cnt = acount.GetMyMoney() / prc;
		sprintf(cmd,"hstcp %s -b %s %d %d",servername,code,prc,cnt);
		system(cmd);
		prc -= unit*level;
		acount.Buy(prc, cnt);
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
		if (port[3] == '2')
			codepos = 6;
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