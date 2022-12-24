// lab4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <regex>
#include <sstream>
#include <cstring>
#include <vector>
#include <ctime>
#include <windows.h>
using namespace std;


vector<string> split(string& s, char sep)
{
    vector<string> args;
    istringstream ss{ s };
    for (string arg; getline(ss, arg, sep); )
        args.push_back(arg);
    return args;
}

int getResult(int count, int value, int plus) {
    int res = 0;
    for (int i = 0; i < count; i++) {
        res += rand() % value + 1;
    }
    res += plus;
    // cout << "   =====" << endl;
    return res;
}

int dice(string& s) {
    
    char delimeterDifDices = ',';
    char delimeterDiceSpecies = 'd';
    char delimeterDicePlus = '+';
    char delimeterDiceMinus = '-';
    bool increment;
    int result = 0;
    // убрать пробелы из строки
    s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
    // делим на дайсы
    vector<string> resDifDices = split(s, delimeterDifDices);
    for (auto it = resDifDices.begin(); it != resDifDices.end(); it++) {
        int count, value, plus = 0;
        vector<string> resDiceSpecies = split(*it, delimeterDiceSpecies);
        count = stoi(resDiceSpecies[0]);
        if (strstr(resDiceSpecies[1].c_str(), "+")) {
            vector<string> resDicePlus = split(resDiceSpecies[1], delimeterDicePlus);
            value = stoi(resDicePlus[0]);
            plus = stoi(resDicePlus[1]);
        }
        else if (strstr(resDiceSpecies[1].c_str(), "-")) {
            vector<string> resDiceMinus = split(resDiceSpecies[1], delimeterDiceMinus);
            value = stoi(resDiceMinus[0]);
            plus = stoi(resDiceMinus[1]);
            plus *= -1;

        }
        else value = stoi(resDiceSpecies[1]);
        int resThisDice = getResult(count, value, plus);
        result += resThisDice;
    }
    return result;
}

int main()
{
    srand(time(NULL));
    string str = "4d6";
    int testsNum = 1000000;
    const int size = 21;
    int results[size];
    for (int i = 0; i < size; i++) 
        results[i] = 0;
    for (int i = 0; i < testsNum; i++) {
        int res = dice(str);
        results[res - 4]++;
    }
    for (int i = 0; i < size; i++) {
        cout << i + 4 << ": " << results[i] << endl;
    }
}
