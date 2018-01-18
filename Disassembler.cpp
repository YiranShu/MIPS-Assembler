#include "Disassembler.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
using namespace std;

Disassembler::Disassembler(const string name) : out(name), PC(startAddress), labelCounter(1)
{
    //for R type. func to opcode. //number: 21
    funcMap["100000"] = "add";
    funcMap["100001"] = "addu";
    funcMap["100010"] = "sub";
    funcMap["100011"] = "subu";
    funcMap["101010"] = "slt";
    funcMap["101011"] = "sltu";
    funcMap["100100"] = "and";
    funcMap["100101"] = "or";
    funcMap["100110"] = "xor";
    funcMap["100111"] = "nor";
    funcMap["000000"] = "sll";
    funcMap["000010"] = "srl";
    funcMap["000011"] = "sra";
    funcMap["011000"] = "mult";
    funcMap["011001"] = "multu";
    funcMap["011010"] = "div";
    funcMap["011011"] = "divu";
    //bltz
    //bgez
    funcMap["001000"] = "jr";
    funcMap["001001"] = "jalr";

    //for I type. first six bits to opcode number: 20
    IMap["100011"] = "lw";
    IMap["100000"] = "lb";
    IMap["100100"] = "lbu";
    IMap["100001"] = "lh";
    IMap["100101"] = "lhu";
    IMap["101011"] = "sw";
    IMap["101000"] = "sb";
    IMap["101001"] = "sh";
    IMap["001000"] = "addi";
    IMap["001001"] = "addiu";
    IMap["001100"] = "andi";
    IMap["001101"] = "ori";
    IMap["001110"] = "xori";
    IMap["001111"] = "lui";
    IMap["001010"] = "slti";
    IMap["001011"] = "sltiu";
    IMap["000100"] = "beq";
    IMap["000101"] = "bne";
    IMap["000110"] = "blez";
    IMap["000111"] = "bgtz";

    //for J type. first six bits to opcode number: 2
    JMap["000010"] = "j";
    JMap["000011"] = "jal";

    regMap["00000"] = "$zero";
    regMap["00001"] = "$at";
    regMap["00010"] = "$v0";
    regMap["00011"] = "$v1";
    regMap["00100"] = "$a0";
    regMap["00101"] = "$a1";
    regMap["00110"] = "$a2";
    regMap["00111"] = "$a3";
    regMap["01000"] = "$t0";
    regMap["01001"] = "$t1";
    regMap["01010"] = "$t2";
    regMap["01011"] = "$t3";
    regMap["01100"] = "$t4";
    regMap["01101"] = "$t5";
    regMap["01110"] = "$t6";
    regMap["01111"] = "$t7";
    regMap["10000"] = "$s0";
    regMap["10001"] = "$s1";
    regMap["10010"] = "$s2";
    regMap["10011"] = "$s3";
    regMap["10100"] = "$s4";
    regMap["10101"] = "$s5";
    regMap["10110"] = "$s6";
    regMap["10111"] = "$s7";
    regMap["11000"] = "$t8";
    regMap["11001"] = "$t9";
    regMap["11010"] = "$k0";
    regMap["11011"] = "$k1";
    regMap["11100"] = "$gp";
    regMap["11101"] = "$sp";
    regMap["11110"] = "$fp";
    regMap["11111"] = "$ra";
}

bool Disassembler::preTreatment(ifstream &inFile)
{
    string buff;
    string str;
    string::iterator it;

    while (!inFile.eof())
    {
        while (getline(inFile, buff))
        {
            it = find(buff.begin(), buff.end(), '#');
            while (it != buff.end())
            {
                it = buff.erase(it);
            }
            it = find(buff.begin(), buff.end(), '/');
            while (it != buff.end())
            {
                it = buff.erase(it);
            }
            trim(buff);
            //clear the notation and space
             //now buff is a pure string. eg: "00000000011000010001000000100000"
            str.clear();
            str = buff.substr(0, 6); //get opcode
            if (str == "000000") //R type
                PC += 4;
            else if (IMap.find(str) != IMap.end()) //I type
            {
                PC += 4;
                if (str == "000100" || str == "000101" || str == "000110" || str == "000111") //beq or bne or blez or bgtz
                    createLabel(buff);
            }
            else if (JMap.find(str) != JMap.end()) //J type
            {
                PC += 4;
                if (str == "000010" || str == "000011") //j or jal
                    createLabel(buff);
            }
            else
                return false;
        }
    }
    PC = startAddress;
    return true;
}

bool Disassembler::readLine(ifstream &inFile)
{
    string buff;
    string str;
    string::iterator it;
    bool flag;

    while (!inFile.eof())
    {
        while (getline(inFile, buff))
        {
            it = find(buff.begin(), buff.end(), '#');
            while (it != buff.end())
            {
                it = buff.erase(it);
            }
            it = find(buff.begin(), buff.end(), '/');
            while (it != buff.end())
            {
                it = buff.erase(it);
            }
            trim(buff);
            //clear the notation and space
            //now buff is a pure string. eg: "00000000011000010001000000100000"
            str.clear();
            str = buff.substr(0, 6); //get opcode
            if (str == "000000") //R type
            {
                PC += 4;
                flag = readRInst(buff);
            }

            else if (IMap.find(str) != IMap.end()) //I type
            {
                PC += 4;
                flag = readIInst(buff);
            }
            else if (JMap.find(str) != JMap.end()) //J type
            {
                PC += 4;
                flag = readJInst(buff);
            }
            else
                return false;
        }
    }
    return flag;
}

bool Disassembler::readRInst(const string &str)
{
    string temp;
    string shamt;
    string rs;
    string rt;
    string rd;
    int imm;

    if (labelMap.find(PC - 4) != labelMap.end())
        out << endl << labelMap[PC - 4] << ":" << endl;

    temp = str.substr(26, 6); //get func
    out << funcMap[temp] << " ";
    if (funcMap[temp] != "sll" && funcMap[temp] != "srl" && funcMap[temp] != "sra"
        && funcMap[temp] != "jr" && funcMap[temp] != "jalr")
    {//normal case
        if (regMap.find(str.substr(6, 5)) != regMap.end())
            rs = regMap[str.substr(6, 5)];//get rs
        else
            return false;
        if (regMap.find(str.substr(11, 5)) != regMap.end())
            rt = regMap[str.substr(11, 5)];//get rt
        else
            return false;
        if (regMap.find(str.substr(16, 5)) != regMap.end())
            rd = regMap[str.substr(16, 5)];//get rd
        else
            return false;
        out << rd << ", " << rs << ", " << rt << endl;
    }
    else if(funcMap[temp] == "sll" || funcMap[temp] == "srl" || funcMap[temp] == "sra")
    {
        if (regMap.find(str.substr(11, 5)) != regMap.end())
            rt = regMap[str.substr(11, 5)]; //get rt
        else
            return false;
        if (regMap.find(str.substr(16, 5)) != regMap.end())
            rd = regMap[str.substr(16, 5)]; //get rd
        else
            return false;
        shamt = str.substr(21, 5);
        imm = toDec(shamt);
        out << rd << ", " << rt << ", " << imm << endl;
    }
    else if(funcMap[temp] == "jr")
    {
        if (regMap.find(str.substr(6, 5)) != regMap.end())
            rs = regMap[str.substr(6, 5)];
        else
            return false;
        out << rs << endl;
    }
    else
    {
        if (regMap.find(str.substr(6, 5)) != regMap.end())
            rs = regMap[str.substr(6, 5)];
        else
            return false;
        if (regMap.find(str.substr(16, 5)) != regMap.end())
            rd = regMap[str.substr(16, 5)]; //get rd
        else
            return false;
        out << rd << "," << rs << endl;
    }
    return true;
}

bool Disassembler::readIInst(const string &str)
{
    string temp;
    string rs;
    string rt;
    string imm;
    string label;
    int num;

    if (labelMap.find(PC - 4) != labelMap.end())
        out << endl << labelMap[PC - 4] << ":" << endl;

    temp = str.substr(0, 6); //get opcode
    out << IMap[temp] << " ";
    if (IMap[temp] != "lw" && IMap[temp] != "lb" && IMap[temp] != "lh" && IMap[temp] != "lbu" && IMap[temp] != "lhu" &&
        IMap[temp] != "sw" && IMap[temp] != "sb" && IMap[temp] != "sh" &&
        IMap[temp] != "beq" && IMap[temp] != "bne" && IMap[temp] != "lui" &&
        IMap[temp] != "blez" && IMap[temp] != "bgtz")
    {
        if (regMap.find(str.substr(6, 5)) != regMap.end())
            rs = regMap[str.substr(6, 5)];
        else
            return false;
        if (regMap.find(str.substr(11, 5)) != regMap.end())
            rt = regMap[str.substr(11, 5)];
        else
            return false;
        imm = str.substr(16, 16); //get immediate number
        num = toDec(imm);
        out << rt << ", " << rs << ", " << num << endl;
    }

    else if (IMap[temp] == "lw" || IMap[temp] == "lb" || IMap[temp] == "lh" || IMap[temp] == "lbu" || IMap[temp] == "lhu" ||
        IMap[temp] == "sw" || IMap[temp] == "sb" || IMap[temp] == "sh")
    {
        if (regMap.find(str.substr(6, 5)) != regMap.end())
            rs = regMap[str.substr(6, 5)];
        else
            return false;
        if (regMap.find(str.substr(11, 5)) != regMap.end())
            rt = regMap[str.substr(11, 5)];
        else
            return false;
        imm = str.substr(16, 16); //get immediate number
        num = toDec(imm);
        out << rt << ", " << num << "(" << rs << ")" << endl;
    }

    else if (IMap[temp] == "lui")
    {
        if (regMap.find(str.substr(11, 5)) != regMap.end())
            rt = regMap[str.substr(11, 5)];
        else
            return false;
        imm = str.substr(16, 16); //get immediate number
        num = toDec(imm);
        out << rt << ", " << num << endl;
    }

    else if(IMap[temp] == "beq" || IMap[temp] == "bne")//beq or bne
    {
        if (regMap.find(str.substr(6, 5)) != regMap.end())
            rs = regMap[str.substr(6, 5)];
        else
            return false;
        if (regMap.find(str.substr(11, 5)) != regMap.end())
            rt = regMap[str.substr(11, 5)];
        else
            return false;
        imm = str.substr(16, 16); //get immediate number
        num = toDec(imm) * 4 + PC;
        label = labelMap[num];
        out << rt << ", " << rs << ", " << label << endl;
    }

    else//blez or bgtz
    {
        if (regMap.find(str.substr(6, 5)) != regMap.end())
            rs = regMap[str.substr(6, 5)];
        else
            return false;
        imm = str.substr(16, 16); //get immediate number
        num = toDec(imm) * 4 + PC;
        label = labelMap[num];
        out << rs << ", " << label << endl;
    }

    return true;
}

bool Disassembler::readJInst(const string &str)
{
    string opcode;
    string address;
    string label;
    int num;

    if (labelMap.find(PC - 4) != labelMap.end())
        out << endl << labelMap[PC - 4] << ":" << endl;

    opcode = JMap[str.substr(0, 6)];
    address = str.substr(6, 26);
    num = toDec(address) * 4;
    label = labelMap[num];
    out << opcode << " " << label << endl;
    return true;
}

void Disassembler::createLabel(const string &str)
{
    string opcode;
    string label;
    int imm;

    label = "Label" + to_string(labelCounter); //create the label
    labelCounter++;

    opcode = str.substr(0, 6); //get opcode
    if (IMap.find(opcode) != IMap.end())// beq or bne or blez or bgtz
    {
        imm = toDec(str.substr(16, 16));
        imm *= 4;
        imm += PC;
        labelMap[imm] = label;
    }

    else //j or jal
    {
        imm = toDec(str.substr(6, 26));
        imm *= 4;
        labelMap[imm] = label;
    }
}

Disassembler::~Disassembler()
{
    out.close();
}

void Disassembler::trim(string &s)
{
    int index = 0;

    if (!s.empty())
    {
        while ((index = s.find(' ', index)) != string::npos)
        {
            s.erase(index, 1);
        }
    }
}

int Disassembler::toDec(const string binary)
{
    int result = 0;
    int flag = 1;

    for (int i = binary.size() - 1; i > 0; --i)
    {
        if (binary[0] == '0')
        {
            result += (binary[i] - '0') * flag;
        }
        else
        {
            result += !(binary[i] - '0') * flag;
        }
        flag *= 2;
    }
    if (binary[0] == '1')
    {
        result += 1;
        result = -result;
    }
    return result;
}
