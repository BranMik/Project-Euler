
#include <iostream>
#include <iterator>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace chrono;

class Pattern{
private:
	vector<short> patternNums;
	size_t currentIndex = 0;
public:

	Pattern(){patternIsValid = false;}
	Pattern(vector<short>,short);
	bool patternIsValid;
	void advancePattern(){ if (++currentIndex == patternNums.size()) currentIndex = 0;}
	short getCurrentPatternValue(){return patternNums[currentIndex];}
	short getPatternSize(){return patternNums.size();}
};
Pattern::Pattern(vector<short> patternVec , short startingIndex){
	patternNums = patternVec;
	currentIndex = startingIndex;
	patternIsValid = true;
}

const uint RESULT_MODULO = 1000000000;
const short MAX_K = 50;
const short PATTERN_SEARCH_LIMIT_FOR_LEFT = 12;
const short PATTERN_SEARCH_LIMIT_FOR_RIGHT = 6;
const short PATTERN_SEARCH_REPEAT_TRESHOLD = 5;
uint64_t N;
short K;

void solveWithPatterns(Pattern , uint , uint[][MAX_K]);
pair<Pattern , uint> solveAndFindPattern(uint[][MAX_K]);
void updateSumsCounter(uint[][MAX_K], uint, bool);
Pattern searchPattern(vector<short>&);
uint optiPowerMod(int_fast64_t );

int main() {
    cout<<"N : ";
    cin>>N;
    cout<<"\nK : ";
    cin>>K;
    cout<<endl;
    auto start_time = high_resolution_clock::now();
    uint sumsCounter[2][MAX_K] = { 0 };
    sumsCounter[0][0] = 1;
    sumsCounter[1][0] = 1;
    pair<Pattern,uint> patternAndStart = solveAndFindPattern(sumsCounter);
    if(patternAndStart.first.patternIsValid){ //Pattern was found
    	solveWithPatterns(patternAndStart.first,patternAndStart.second,sumsCounter);
    }
    auto end_time = high_resolution_clock::now();
    cout << "Running time: " << duration_cast<microseconds>
    					 (end_time - start_time).count() / 1000.f << " ms" << endl;
    uint result = sumsCounter[0][0];
    cout << result;
    return 0;
}

pair<Pattern,uint> solveAndFindPattern(uint sumsCounter[][MAX_K]){
	short patternSearchTrigger = 50;
	vector<short> nPowMods;
	Pattern result = Pattern();
	bool currentSumArray = true;
	for(uint64_t n = 1 ; n <= N ; n++){
		uint num = optiPowerMod(n);
		nPowMods.push_back(num);
		updateSumsCounter(sumsCounter , num , currentSumArray);
		if(n > patternSearchTrigger){
			result = searchPattern(nPowMods);
			if(result.patternIsValid){
				copy(sumsCounter[currentSumArray] , sumsCounter[currentSumArray] + MAX_K , sumsCounter[!currentSumArray]);
				return pair<Pattern,int>(result,n + 1);
			}else {
				patternSearchTrigger *= 10;
			}
		}
		currentSumArray=!currentSumArray;
	}

	sumsCounter[!currentSumArray][0]--;
	copy(sumsCounter[!currentSumArray] , sumsCounter[!currentSumArray] + MAX_K , sumsCounter[currentSumArray]);
	return  pair<Pattern,int>(result , -1);
}

Pattern searchPattern(vector<short> &nPowMods){
    short start;
    short end;
    for(size_t left = 0 ; left < nPowMods.size() - PATTERN_SEARCH_LIMIT_FOR_LEFT ; left++){
		for(size_t right = left + PATTERN_SEARCH_REPEAT_TRESHOLD ; right < nPowMods.size() - PATTERN_SEARCH_LIMIT_FOR_RIGHT ; right++){
			bool numbersAreRepeating = equal(nPowMods.begin() + left , nPowMods.begin() + left + PATTERN_SEARCH_REPEAT_TRESHOLD , nPowMods.begin() + right);
			if(numbersAreRepeating){
				start = left;
				end = right;//EXCLUSIVE
				vector<short> patternNumbers(nPowMods.begin() + start , nPowMods.begin() + end);
				int patternStart = (nPowMods.size() - end) % (end - start);
				return Pattern(patternNumbers , patternStart);
			}
        }
    }
    return Pattern();
}

void solveWithPatterns(Pattern pattern , uint nStartAt , uint sumsCounter[][MAX_K]){

	//Variable that carry the index of array that is being updated in current iteration cycle. Boolean is used as either index 0 or 1.
    bool currentSumArray = true;
    uint num;
    for(uint64_t n = nStartAt ; n <= N ; n++){
        num = pattern.getCurrentPatternValue();
        updateSumsCounter(sumsCounter , num , currentSumArray);
        pattern.advancePattern();
        currentSumArray = !currentSumArray;
    }
    sumsCounter[!currentSumArray][0]--;
    copy(sumsCounter[!currentSumArray] , sumsCounter[!currentSumArray] + MAX_K , sumsCounter[currentSumArray]);
}

void updateSumsCounter(uint sumsCounter[][MAX_K] , uint num , bool currentSumArray){
	short updateFromSum = K - num;

	//The index of array updated in previous iteration cycle.
	bool previousSumArray = !currentSumArray;
	for(short sum = 0 ; sum < K ; sum++){
		
		//Removing the %K operation from equation and replacing it with this conditional improves code speed by 1/3, see Euler250_short for previous version
		if(updateFromSum == K)updateFromSum = 0; 
		sumsCounter[currentSumArray][sum] = (sumsCounter[previousSumArray][sum] + sumsCounter[previousSumArray][updateFromSum++]) % RESULT_MODULO;
	}
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
