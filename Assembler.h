#ifndef Assembler_H_
#define Assembler_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
using namespace std;

const int StartAddress = 0;

class Assembler
{
private:
	vector<string> opcodeR;
	vector<string> opcodeI;
	vector<string> opcodeJ;
	unordered_map<string, string> opcodeMap;
	unordered_map<string, string> regMap;
	unordered_map<string, string> funcMap;
	unordered_map<string, string> shamtMap;
	unordered_map<string, int> labelMap;
	int PC;
	ofstream out;

public:
	Assembler(const string name);
	virtual ~Assembler();
	bool readLine(ifstream &inFile);
	bool readRInst(const string &str);
	bool readIInst(const string &str);
	bool readJInst(const string &str);
	void readLabel(const string &str);
	bool preTreatment(ifstream &inFile);
	string toBinary(int x);
	string toBinary_shamt(int x);
	string toBinary_address(int x);
	int HextoDec(const string &s);
};

#endif