#ifndef _Disassembler_H_
#define _Disassembler_H_
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

const int startAddress = 0;
class Disassembler
{
private:
    unordered_map<string, string> funcMap; //for R type. func to opcode.
    unordered_map<string, string> IMap; //for I type. first six bits to opcode
    unordered_map<string, string> JMap; //for J type. first six bits to opcode
    unordered_map<int, string> labelMap;
    unordered_map<string, string> regMap;
    int PC;
    int labelCounter;
    ofstream out;

public:
    Disassembler(const string name);
    bool preTreatment(ifstream &inFile);
    bool readLine(ifstream &inFile);
    bool readRInst(const string &str);
    bool readIInst(const string &str);
    bool readJInst(const string &str);
    void createLabel(const string &str);
    virtual ~Disassembler();
    void trim(string &s); //delete all the space
    int toDec(const string binary); //transmit 2'component into dec
};

#endif
