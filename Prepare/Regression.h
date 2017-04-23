#pragma once

#define STACKSIZE 10000

class RET {

private:
	int x[STACKSIZE];
	int y[STACKSIZE];
	int idx;
public:
	RET() {
		idx = 0;
	}
	void init() {
		idx = 0;
	}
	void save(int _x, int _y) {
		x[idx] = _x;
		y[idx] = _y; idx++;
	}
	double calculate() {
		double _x = 0, _y = 0;
		double _a = 0, _b = 0;
		for (int i = 0; i < idx; i++)
		{
			_x += x[i]; _y += y[i];
		} _x /= idx; _y /= idx;
		for (int i = 0; i < idx; i++) {
			_a += (x[i] - _x)*(y[i] - _y);
			_b += (x[i] - _x)*(x[i] - _x);
		}
		init();
		if (_b == 0)return 0;
		return _a / _b;
	}
};