
#include "shig_utility.h"



namespace shig {

	
	long long secure_add(long long a, long long b){

		constexpr long long mxm = numeric_limits<long long>::max();
		constexpr long long mum = numeric_limits<long long>::min();

		if (b == 0)return a;
		else if (b > 0) {
			if (a < mxm - b)return a + b;
			else return mxm - 1;
		}
		else {
			if (mum - b < a)return a + b;
			else return mum + 1;
		}

		return long long(0);
	}









}