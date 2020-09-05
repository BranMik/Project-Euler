
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;

const short MAX_K = 50;
uint64_t N;
short K;

void solve(uint[][50]);
uint optiPowerMod(int_fast64_t );

int main() {
    cin>>N;
    cin>>K;
    uint sumsCounter[2][MAX_K] = { 0 };
    sumsCounter[0][0] = 1, sumsCounter[1][0] = 1;
    solve(sumsCounter);
    cout << sumsCounter[0][0];
    return 0;
}

void solve(uint sumsCounter[][MAX_K]){
	bool currentSumArray = true;
	for(uint64_t n = 1 ; n <= N ; n++){
		uint num = optiPowerMod(n);
		short updateFromSum = K - num;
		for(short sum = 0 ; sum < K ; sum++){
			if(updateFromSum == K)updateFromSum = 0;
			sumsCounter[currentSumArray][sum] = (sumsCounter[!currentSumArray][sum] + sumsCounter[!currentSumArray][(updateFromSum++)%K]) % 1000000000;
		}
		currentSumArray = !currentSumArray;
	}
	sumsCounter[!currentSumArray][0]--;
	copy(sumsCounter[!currentSumArray] , sumsCounter[!currentSumArray] + MAX_K , sumsCounter[currentSumArray]);
}

uint optiPowerMod(int_fast64_t n){
    uint res = 1;
    uint pow = n;
    n = n % K;
    if (n == 0) return 0;
    while (pow > 0)
    {
        if((pow & 1) == 1)
            res = (res * n) % K;
        pow = pow >> 1;
        n = (n * n) % K;
    }
    return res;
}
