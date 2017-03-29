#include<stdio.h>
#include "ACOUNT.h"
#include "CORE.h"
#include "UTILS.h"

int main(int argc, char *argv[]) {
	if (argc<=3)
	{
		printf("HSword +----- version 1.0.3.29\n");
		printf("usage: hs X Y Z [server-name]\n");
		return 0;
	}
	int X = Utils::CharToInt(argv[1]);
	int Y = Utils::CharToInt(argv[2]);
	int Z = Utils::CharToInt(argv[3]);
	int MONEY = Utils::GetMyMONEY();
	char* SERVERNAME=NULL;
	if (argv[4]) {
		SERVERNAME = argv[4];
	}
	CORE c(X, Y, Z);
	if (SERVERNAME)c.SetSERVERNAME(SERVERNAME);
	if (MONEY)c.SetMoney(MONEY);
	c.Debug(true);
	c.Run();
	c.Print();
	return 0;
}