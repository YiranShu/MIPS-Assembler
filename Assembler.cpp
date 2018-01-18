#include "Assembler.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdlib.h>
using namespace std;

Assembler::Assembler(const string name): out(name)
{

    PC = StartAddress;

    //R type
    opcodeMap["add"] = "000000";
    opcodeMap["addu"] = "000000";
    opcodeMap["sub"] = "000000";
    opcodeMap["subu"] = "000000";
    opcodeMap["and"] = "000000";
    opcodeMap["or"] = "000000";
    opcodeMap["xor"] = "000000";
    opcodeMap["nor"] = "000000";
    opcodeMap["slt"] = "000000";
    opcodeMap["sltu"] = "000000";
    opcodeMap["sll"] = "000000";
    opcodeMap["srl"] = "000000";
    opcodeMap["sra"] = "000000";
    opcodeMap["mult"] = "000000";
    opcodeMap["multu"] = "000000";
    opcodeMap["div"] = "000000";
    opcodeMap["divu"] = "000000";
    opcodeMap["jr"] = "000000";
    opcodeMap["jalr"] = "000000";

    //I type
    opcodeMap["lw"] = "100011";
    opcodeMap["lb"] = "100000";
    opcodeMap["lbu"] = "100100";
    opcodeMap["lh"] = "100001";
    opcodeMap["lhu"] = "100101";
    opcodeMap["sw"] = "101011"; //H2b
    opcodeMap["sb"] = "101000";
    opcodeMap["sh"] = "101001";
    opcodeMap["addi"] = "001000";
    opcodeMap["addiu"] = "001001";
    opcodeMap["andi"] = "001100"; //Hc
    opcodeMap["ori"] = "001101"; //Hd
    opcodeMap["xori"] = "001110";
    opcodeMap["lui"] = "001111";
    opcodeMap["slti"] = "001010";
    opcodeMap["sltiu"] = "001011";
    opcodeMap["beq"] = "000100";
    opcodeMap["bne"] = "000101";
    opcodeMap["blez"] = "000110";
    opcodeMap["bgtz"] = "000111";
    //opcodeMap["bltz"] = "000001";
    //opcodeMap["bgez"] = "000001";

    //J type
    opcodeMap["j"] = "000010";
    opcodeMap["jal"] = "000011";

    funcMap["add"] = "100000";
    funcMap["addu"] = "100001";
    funcMap["sub"] = "100010";
    funcMap["subu"] = "100011";
    funcMap["and"] = "100100";
    funcMap["or"] = "100101";
    funcMap["xor"] = "100110";
    funcMap["nor"] = "100111";
    funcMap["slt"] = "101010";
    funcMap["sltu"] = "101011";
    funcMap["sll"] = "000000";
    funcMap["srl"] = "000010";
    funcMap["sra"] = "000011";
    funcMap["mult"] = "011000";
    funcMap["multu"] = "011001";
    funcMap["div"] = "011010";
    funcMap["divu"] = "011011";
    funcMap["jr"] = "001000";
    funcMap["jalr"] = "001001";

    //funcMap["bltz"] = "000000";
    //funcMap["bgez"] = "000001";

    opcodeR.push_back("add");
    opcodeR.push_back("addu");
    opcodeR.push_back("sub");
    opcodeR.push_back("subu");
    opcodeR.push_back("and");
    opcodeR.push_back("or");
    opcodeR.push_back("xor");
    opcodeR.push_back("nor");
    opcodeR.push_back("slt");
    opcodeR.push_back("sltu");
    opcodeR.push_back("sll");
    opcodeR.push_back("srl");
    opcodeR.push_back("sra");
    opcodeR.push_back("mult");
    opcodeR.push_back("multu");
    opcodeR.push_back("div");
    opcodeR.push_back("divu");
    opcodeR.push_back("jr");
    opcodeR.push_back("jalr");

    opcodeI.push_back("lw");
    opcodeI.push_back("lb");
    opcodeI.push_back("lbu");
    opcodeI.push_back("lh");
    opcodeI.push_back("lhu");
    opcodeI.push_back("sw");
    opcodeI.push_back("sb");
    opcodeI.push_back("sh");
    opcodeI.push_back("addi");
    opcodeI.push_back("addiu");
    opcodeI.push_back("andi");
    opcodeI.push_back("ori");
    opcodeI.push_back("xori");
    opcodeI.push_back("lui");
    opcodeI.push_back("slti");
    opcodeI.push_back("sltiu");
    opcodeI.push_back("beq");
    opcodeI.push_back("bne");
    opcodeI.push_back("blez");
    opcodeI.push_back("bgtz");
    //opcodeI.push_back("bgez");
    //opcodeI.push_back("bltz");

    opcodeJ.push_back("j");
    opcodeJ.push_back("jal");

    //reg
    regMap["r0"] = "00000";
    regMap["r1"] = "00001";
    regMap["r2"] = "00010";
    regMap["r3"] = "00011";
    regMap["r4"] = "00100";
    regMap["r5"] = "00101";
    regMap["r6"] = "00110";
    regMap["r7"] = "00111";
    regMap["r8"] = "01000";
    regMap["r9"] = "01001";
    regMap["r10"] = "01010";
    regMap["r11"] = "01011";
    regMap["r12"] = "01100";
    regMap["r13"] = "01101";
    regMap["r14"] = "01110";
    regMap["r15"] = "01111";
    regMap["r16"] = "10000";
    regMap["r17"] = "10001";
    regMap["r18"] = "10010";
    regMap["r19"] = "10011";
    regMap["r20"] = "10100";
    regMap["r21"] = "10101";
    regMap["r22"] = "10110";
    regMap["r23"] = "10111";
    regMap["r24"] = "11000";
    regMap["r25"] = "11001";
    regMap["r26"] = "11010";
    regMap["r27"] = "11011";
    regMap["r28"] = "11100";
    regMap["r29"] = "11101";
    regMap["r30"] = "11110";
    regMap["r31"] = "11111";

    //regR
    regMap["R0"] = "00000";
    regMap["R1"] = "00001";
    regMap["R2"] = "00010";
    regMap["R3"] = "00011";
    regMap["R4"] = "00100";
    regMap["R5"] = "00101";
    regMap["R6"] = "00110";
    regMap["R7"] = "00111";
    regMap["R8"] = "01000";
    regMap["R9"] = "01001";
    regMap["R10"] = "01010";
    regMap["R11"] = "01011";
    regMap["R12"] = "01100";
    regMap["R13"] = "01101";
    regMap["R14"] = "01110";
    regMap["R15"] = "01111";
    regMap["R16"] = "10000";
    regMap["R17"] = "10001";
    regMap["R18"] = "10010";
    regMap["R19"] = "10011";
    regMap["R20"] = "10100";
    regMap["R21"] = "10101";
    regMap["R22"] = "10110";
    regMap["R23"] = "10111";
    regMap["R24"] = "11000";
    regMap["R25"] = "11001";
    regMap["R26"] = "11010";
    regMap["R27"] = "11011";
    regMap["R28"] = "11100";
    regMap["R29"] = "11101";
    regMap["R30"] = "11110";
    regMap["R31"] = "11111";

    //MIPS standard
    regMap["$zero"] = "00000";
    regMap["$at"] = "00001";
    regMap["$v0"] = "00010";
    regMap["$v1"] = "00011";
    regMap["$a0"] = "00100";
    regMap["$a1"] = "00101";
    regMap["$a2"] = "00110";
    regMap["$a3"] = "00111";
    regMap["$t0"] = "01000";
    regMap["$t1"] = "01001";
    regMap["$t2"] = "01010";
    regMap["$t3"] = "01011";
    regMap["$t4"] = "01100";
    regMap["$t5"] = "01101";
    regMap["$t6"] = "01110";
    regMap["$t7"] = "01111";
    regMap["$s0"] = "10000";
    regMap["$s1"] = "10001";
    regMap["$s2"] = "10010";
    regMap["$s3"] = "10011";
    regMap["$s4"] = "10100";
    regMap["$s5"] = "10101";
    regMap["$s6"] = "10110";
    regMap["$s7"] = "10111";
    regMap["$t8"] = "11000";
    regMap["$t9"] = "11001";
    regMap["$k0"] = "11010";
    regMap["$k1"] = "11011";
    regMap["$gp"] = "11100";
    regMap["$sp"] = "11101";
    regMap["$fp"] = "11110";
    regMap["$ra"] = "11111";
}

bool Assembler::preTreatment(ifstream &inFile)
{
    string::iterator it;
    string str;
    string buff;
    //cout << "find" << endl;

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
            }//clear the notation
             //now buff is a pure string. eg: "add $s1, $s2, $s3" or a label or an empty string
             //read the opcode
            str.clear();
            for (unsigned int i = 0; i < buff.size(); i++)
            {
                if (buff[i] == ' ')
                    continue;        //clear the space
                while (i < buff.size() && buff[i] != ' ')
                {
                    str += buff[i]; //?
                    i++;
                }


                //itVec = find(opcodeR.begin(), opcodeR.end(), str);

                if (find(opcodeR.begin(), opcodeR.end(), str) != opcodeR.end())
                {
                    PC += 4;
                    //readRInst(buff);
                }
                else if (find(opcodeI.begin(), opcodeI.end(), str) != opcodeI.end())
                {
                    PC += 4;
                    //readIInst(buff);
                }
                else if (find(opcodeJ.begin(), opcodeJ.end(), str) != opcodeJ.end())
                {
                    PC += 4;
                    //readJInst(buff);
                }
                else if (find(str.begin(), str.end(), ':') != str.end())
                {
                    readLabel(buff);
                }
                else
                {
                    //inFile.close();
                    return false;
                }
                break;

            }
        }
    }

    //inFile.close();
    PC = StartAddress;
    cout << "successful to close" << endl;
    return true;
}

bool Assembler::readLine(ifstream &inFile)
{
    string buff;
    string str;
    string::iterator it;
    bool flag = true;

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
            }//clear the notation
            //now buff is a pure string. eg: "add $s1, $s2, $s3" or a label or an empty string
            //read the opcode
            str.clear();
            for (unsigned int i = 0; i < buff.size(); i++)
            {
                if (buff[i] == ' ')
                    continue;        //clear the space
                while (i < buff.size() && buff[i] != ' ')
                {
                    str += buff[i]; //?
                    i++;
                }


                //itVec = find(opcodeR.begin(), opcodeR.end(), str);

                if (find(opcodeR.begin(), opcodeR.end(), str) != opcodeR.end())
                {
                    PC += 4;
                    flag = readRInst(buff);
                }
                else if (find(opcodeI.begin(), opcodeI.end(), str) != opcodeI.end())
                {
                    PC += 4;
                    flag = readIInst(buff);
                }
                else if (find(opcodeJ.begin(), opcodeJ.end(), str) != opcodeJ.end())
                {
                    PC += 4;
                    flag = readJInst(buff);
                }
                else if (find(str.begin(), str.end(), ':') != str.end());
                else
                {
                    //inFile.close();
                    return false;
                }
                break;

            }
        }


    }

    //inFile.close();
    return flag;
}

bool Assembler::readRInst(const string &str)
{
    unsigned int i = 0;
    string temp;
    string func;
    string shamt;
    string rs;
    string rt;
    string rd;

    temp.clear();
    while (str[i] == ' ')
        i++;
    while (i < str.size() && str[i] != ' ')
    {
        temp += str[i]; //?
        i++;
    }//get the opcode

    func = funcMap[temp]; //get the function
    out << opcodeMap[temp] << " ";//print the opcode

    if (temp != "sll" && temp != "srl" && temp != "sra" && temp != "jr" && temp != "jalr")//normal case
    {
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ',')
        {
            temp += str[i]; //?
            i++;
        }//get rd
        if (regMap.find(temp) != regMap.end())//valid rd
            rd = regMap[temp];
        else
            return false; //invalid rd

        i++;
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ',')
        {
            temp += str[i]; //?
            i++;
        }//get rs
        if (regMap.find(temp) != regMap.end())//valid rs
            rs = regMap[temp];
        else
            return false; //invalid rs

        i++;
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ' ')
        {
            temp += str[i]; //?
            i++;
        }//get rt
        if (regMap.find(temp) != regMap.end())//valid rt
            rt = regMap[temp];
        else
            return false; //invalid rt
        out << rs << " " << rt << " " << rd << " " << "00000 " << func << endl;
    }


    else if (temp == "sll" || temp == "srl" || temp == "sra")
    {
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ',')
        {
            temp += str[i]; //?
            i++;
        }//get rd
        if (regMap.find(temp) != regMap.end())//valid rd
            rd = regMap[temp];
        else
            return false; //invalid rd

        i++;
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ',')
        {
            temp += str[i]; //?
            i++;
        }//get rt
        if (regMap.find(temp) != regMap.end())//valid rt
            rt = regMap[temp];
        else
            return false; //invalid rt

        i++;
        shamt.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ' ')
        {
            shamt += str[i]; //?
            i++;
        }//get shamt
        if (shamt[0] == '0' && (shamt[1] == 'x' || shamt[1] == 'X'))
        {
            int imm = HextoDec(shamt);
            shamt = toBinary_shamt(imm);
        }
        else
        {
            int imm = atoi(shamt.c_str());
            shamt = toBinary_shamt(imm);
        }
        out << "00000 " << rt << " " << rd << " " << shamt << " " << func << endl;
    }

    else if (temp == "jr")
    {
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ' ')
        {
            temp += str[i]; //?
            i++;
        }//get rs
        if (regMap.find(temp) != regMap.end())//valid rs
            rs = regMap[temp];
        else
            return false; //invalid rs
        out << rs << " " << "00000 " << "00000 " << "00000 " << func << endl;
    }

    else if (temp == "jalr")
    {
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ',')
        {
            temp += str[i]; //?
            i++;
        }//get rd
        if (regMap.find(temp) != regMap.end())//valid rd
            rd = regMap[temp];
        else
            return false; //invalid rd

        i++;
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ' ')
        {
            temp += str[i]; //?
            i++;
        }//get rs
        if (regMap.find(temp) != regMap.end())//valid rs
            rs = regMap[temp];
        else
            return false; //invalid rs

        out << rs << " " << "00000" << " " << rd << " " << "00000 " << func << endl;
    }

    return true;
}

bool Assembler::readIInst(const string &str)
{
    string temp;
    string rs;
    string rt;
    string imm;
    unsigned int i = 0;

    temp.clear();
    while (str[i] == ' ')
        i++;
    while (i < str.size() && str[i] != ' ')
    {
        temp += str[i]; //?
        i++;
    }//get the opcode
    out << opcodeMap[temp] << " ";//print the opcode


    if (temp != "lw" && temp != "lb" && temp != "lh" && temp != "lbu" && temp != "lhu"
        && temp != "sw" && temp != "sb" && temp != "sh" && temp != "lui"
        && temp != "beq" && temp != "bne" && temp != "blez" && temp != "bgtz") //normal case
    {
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ',')
        {
            temp += str[i]; //?
            i++;
        }//get rt
        if (regMap.find(temp) != regMap.end())//valid rt
            rt = regMap[temp];
        else
            return false; //invalid rt

        i++; //skip ","
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ',')
        {
            temp += str[i]; //?
            i++;
        }//get rs
        if (regMap.find(temp) != regMap.end())//valid rs
            rs = regMap[temp];
        else
            return false; //invalid rs

        i++;
        imm.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ' ')
        {
            imm += str[i]; //?
            i++;
        }//get immediate number
        if (imm[0] == '0' && (imm[1] == 'x' || imm[1] == 'X'))
        {
            int num = HextoDec(imm);
            imm = toBinary(num);
        }
        else
        {
            int num = atoi(imm.c_str());
            imm = toBinary(num);
        }
        out << rs << " " << rt << " " << imm << endl;
    }

    else if (temp == "lw" || temp == "lh" || temp == "lb" || temp == "lhu" || temp == "lbu"
        || temp == "sw" || temp == "sb" || temp == "sh")
    {
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ',')
        {
            temp += str[i]; //?
            i++;
        }//get rt
        if (regMap.find(temp) != regMap.end())//valid rt
            rt = regMap[temp];
        else
            return false; //invalid rt

        i++; //skip ","
        imm.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != '(')
        {
            imm += str[i]; //?
            i++;
        }//get immediate number
        if (!imm.empty())
        {
            if (imm[0] == '0' && (imm[1] == 'x' || imm[1] == 'X'))
            {
                int num = HextoDec(imm);
                imm = toBinary(num);
            }
            else
            {
                int num = atoi(imm.c_str());
                imm = toBinary(num);
            }
        }
        else
            imm = "0000000000000000";

        i++; //skip "("
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ')')
        {
            temp += str[i]; //?
            i++;
        }//get rs
        if (regMap.find(temp) != regMap.end())//valid rs
            rs = regMap[temp];
        else
            return false; //invalid rs

        out << rs << " " << rt << " " << imm << endl;
    }

    else if (temp == "lui")
    {
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ',')
        {
            temp += str[i]; //?
            i++;
        }//get rt
        if (regMap.find(temp) != regMap.end())//valid rt
            rt = regMap[temp];
        else
            return false; //invalid rt

        i++; //skip ","
        imm.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ' ')
        {
            imm += str[i]; //?
            i++;
        }//get immediate number
        if (imm[0] == '0' && (imm[1] == 'x' || imm[1] == 'X'))
        {
            int num = HextoDec(imm);
            imm = toBinary(num);
        }
        else
        {
            int num = atoi(imm.c_str());
            imm = toBinary(num);
        }

        out << "00000 " << rt << " " << imm << endl;
    }

    else if (temp == "beq" || temp == "bne")
    {
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ',')
        {
            temp += str[i]; //?
            i++;
        }//get rt
        if (regMap.find(temp) != regMap.end())//valid rt
            rt = regMap[temp];
        else
            return false; //invalid rt

        i++; //skip ","
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ',')
        {
            temp += str[i]; //?
            i++;
        }//get rs
        if (regMap.find(temp) != regMap.end())//valid rs
            rs = regMap[temp];
        else
            return false; //invalid rs

        i++; //skip ","
        imm.clear();
        while (str[i] == ' ')
        {
            i++;
        }
        if (str[i] <= '9' && str[i] >= '0') //digit
        {
            while (i < str.size() && str[i] != ' ')
            {
                imm += str[i];
                i++;
            }
            if (imm[0] == '0' && (imm[1] == 'x' || imm[1] == 'X'))
            {
                int num = HextoDec(imm);
                imm = toBinary(num);
            }
            else
            {
                int num = atoi(imm.c_str());
                imm = toBinary(num);
            }
        }
        else //label
        {
            while (i < str.size() && str[i] != ' ')
            {
                imm += str[i];
                i++;
            }
            if (labelMap.find(imm) != labelMap.end())
            {
                int num = (labelMap[imm] - PC) / 4;
                imm = toBinary(num);
            }
            else
                return false;
        }

        out << rs << " " << rt << " " << imm << endl;
    }

    else if (temp == "blez" || temp == "bgtz")
    {
        temp.clear();
        while (str[i] == ' ')
            i++;
        while (i < str.size() && str[i] != ',')
        {
            temp += str[i]; //?
            i++;
        }//get rs
        if (regMap.find(temp) != regMap.end())//valid rs
            rs = regMap[temp];
        else
            return false; //invalid rs

        i++; //skip ","
        imm.clear();
        while (str[i] == ' ')
        {
            i++;
        }
        if (str[i] <= '9' && str[i] >= '0') //digit
        {
            while (i < str.size() && str[i] != ' ')
            {
                imm += str[i];
                i++;
            }
            if (imm[0] == '0' && (imm[1] == 'x' || imm[1] == 'X'))
            {
                int num = HextoDec(imm);
                imm = toBinary(num);
            }
            else
            {
                int num = atoi(imm.c_str());
                imm = toBinary(num);
            }
        }
        else //label
        {
            while (i < str.size() && str[i] != ' ')
            {
                imm += str[i];
                i++;
            }
            if (labelMap.find(imm) != labelMap.end())
            {
                int num = (labelMap[imm] - PC) / 4;
                imm = toBinary(num);
            }
            else
                return false;
        }

        out << rs << " " << "00000" << " " << imm << endl;
    }
    return true;
}

bool Assembler::readJInst(const string &str)
{
    string temp;
    string imm;
    unsigned int i = 0;

    temp.clear();
    while (str[i] == ' ')
        i++;
    while (i < str.size() && str[i] != ' ')
    {
        temp += str[i]; //?
        i++;
    }//get the opcode
    out << opcodeMap[temp] << " ";//print the opcode

    imm.clear();
    while (str[i] == ' ')
    {
        i++;
    }
    if (str[i] >= '0' && str[i] <= '9') //digit
    {
        while (i < str.size() && str[i] != ' ')
        {
            imm += str[i];
            i++;
        }
        if (imm[0] == '0' && (imm[1] == 'x' || imm[1] == 'X'))
        {
            int num = HextoDec(imm);
            imm = toBinary_address(num);
        }
        else
        {
            int num = atoi(imm.c_str());
            imm = toBinary_address(num);
        }
    }
    else //label
    {
        while (i < str.size() && str[i] != ' ')
        {
            imm += str[i];
            i++;
        }
        if (labelMap.find(imm) != labelMap.end())
        {
            imm = toBinary_address(labelMap[imm] / 4);
        }
        else
            return false;
    }
    out << imm << endl;

    return true;
}

void Assembler::readLabel(const string &str)//build the relation between label and PC
{
    string label;
    int i = 0;

    label.clear();
    while (str[i] == ' ')
        i++;
    for (; str[i] != ':'; i++)
    {
        label += str[i];
    }
    labelMap[label] = PC;
}

string Assembler::toBinary(int x)
{
    string result;
    short int i;

    for (i = 0; i < 16; i++)
    {
        if ((x & 1 << (15 - i)) == 0)
            result += '0';
        else
            result += '1';
    }
    return result;
}

string Assembler::toBinary_shamt(int x)
{
    string result;
    short int i;

    for (i = 0; i < 5; i++)
    {
        if ((x & 1 << (4 - i)) == 0)
            result += '0';
        else
            result += '1';
    }
    return result;
}

string Assembler::toBinary_address(int x)
{
    string result;
    short int i;

    for (i = 0; i < 26; i++)
    {
        if ((x & 1 << (25 - i)) == 0)
            result += '0';
        else
            result += '1';
    }
    return result;
}

int Assembler::HextoDec(const string &s)
{
    int i = 0, count, sum;
    string temp;

    temp = s.substr(2, s.length() - 2);
    count = temp.length();
    sum = 0;
    for (i = count - 1; i >= 0; i--)//
    {
        if (temp[i] >= '0' && temp[i] <= '9') //
        {
            sum += (temp[i] - 48) * pow(16, count - i - 1);
        }
        else if (temp[i] >= 'A' && temp[i] <= 'F')//
        {
            sum += (temp[i] - 55) * pow(16, count - i - 1);
        }
        else if (temp[i] >= 'a' && temp[i] <= 'f')
        {
            sum += (temp[i] - 87) * pow(16, count - i - 1);
        }
    }
    return sum;
}

Assembler::~Assembler()
{
    out.close();
}
