#pragma once
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
class Utils {
public:
	static int CurrentGetMinute() {
		time_t t = time(NULL);
		tm* cur;
		cur = localtime(&t);
		return cur->tm_min;
	}
	static int CurrentGetHour() {
		time_t t = time(NULL);
		tm* cur;
		cur = localtime(&t);
		return cur->tm_hour;
	}
	static int CharToInt(char* str) {
		int d = 1;
		int ret = 0;
		for (int i = strlen(str) - 1; i >= 0; i--) {
			int j = (int)str[i] - '0';
			j *= d;
			d *= 10;
			ret += j;
		}
		return ret;
	}
	static int GetMyMONEY() {
		char env[255];
		strcpy(env, getenv("HSWORD_HOME"));
		env[strlen(env) - 3] = 0;
		strcat(env, "config.ini");

		FILE *in = fopen(env, "r");
		if (in) {
			char str[255];
			for (int i = 0; i < 4; i++)
				fscanf(in, "%s", str);
			char num[255];
			int idx = 0;

			for (int i = 6; i < strlen(str); i++)
			{
				num[idx++] = str[i];
			}
			num[idx] = 0;
			fclose(in);
			return CharToInt(num);
		}
		return 0;
	}
};