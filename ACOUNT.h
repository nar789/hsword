#pragma once
#include<stdio.h>

class ACOUNT {

private:
	bool D = false;
	int money = 0;
	int stockcnt = 0;
	int stockprice = 0;
	int state = 0;

public:
	void Debug(bool d) {
		D = d;
	}
	ACOUNT() {

	}
	ACOUNT(int m) {
		money = m;
	}
	bool HaveStock() {
		if (stockcnt > 0)return true;
		return false;
	}
	int GetState() {
		return state;
	}
	void UpState() {
		state++;
	}
	void InitState() {
		state = 0;
	}

	void SetMoney(int m) { money = m; }

	void Buy(int p, int cnt) {
		if (p > money) {
			if (D)
				printf("Can't this stock.\n");
		}
		else {
			if (D) {
				printf("\nBUY STOCK : %d\t%d\t%d!\n\n", p, cnt,p*cnt);
			}
			stockprice = p;
			stockcnt=cnt;
			money -= (p*cnt);
		}
	}

	void Sell(int p, int cnt) {
		InitState();
		money += (p*cnt);
		stockprice = 0;
		stockcnt = 0;
		if (D) {
			printf("\nSELL STOCK : %d\t%d\t%d\n", p,cnt,p*cnt);
			printf("=>MY MONEY : %d\n\n", money);
		}
	}

	int GetStockPrice() {
		return stockprice;
	}
	int GetMyMoney() {
		return money;
	}
	int GetStockCnt() {
		return stockcnt;
	}
};