#include <bits/stdc++.h>
#include "libbuild/install/include/OpenXLSX/OpenXLSX.h"
using namespace std;
using namespace OpenXLSX;

//Data types
//Exel Data type
typedef vector<pair<string, vector<char>>> ExelData_t;

//function prototypes
//read an exel file and returns ExelData_t
ExelData_t read_data(const string fileName, const string sheetName);
//show preference of team
void show_preference_team(ExelData_t& sheetData);
//get max teammates
int max_teammates();

//function definitions
ExelData_t read_data(const string fileName, const string sheetName)
{
    XLDocument reading;
    reading.OpenDocument(fileName);
    ExelData_t retval;
    auto currentSheet = reading.Workbook().Worksheet(sheetName);
    for (int i = 1; currentSheet.Cell('A' + to_string(i)).Value().AsString() != "NA"; i++)
    {
        vector<char> tempPri;
        for(char j = 'B'; currentSheet.Cell(j + to_string(i)).Value().AsString() != "NA"; j++)
            tempPri.push_back(currentSheet.Cell(j + to_string(i)).Value().Get<char>());
        retval.push_back(make_pair(currentSheet.Cell('A' + to_string(i)).Value().AsString(),tempPri));
    }
    return retval;
}
void show_preference_team(ExelData_t& sheetData)
{
    cout << "제 1지망 선택 인원수" << endl;
    vector<int> number(5);
    for (int i = 0; i < sheetData.size(); i++)
        number[sheetData[i].second[0] - 'A']++;
    for (char i = 'A'; i <= 'E'; i++)
        cout << i << ": " << number[i - 'A'] << endl;
    cout << endl;
}
int max_teammates()
{
    cout << "각 조당 최대 인원수를 설정해주세요 : ";
    string temp;
    cin >> temp;
    int retval{stoi(temp)};
    return retval;
}

//main
int main()
{
    cout << "조원 분배 프로그램입니다." << endl;
    cout << "엑셀 파일 이름을 입력하세요 : ";
    string fileName, sheetName;
    cin >> fileName;
    cout << "타겟 시트 이름을 입력하세요 : ";
    cin >> sheetName;
    ExelData_t rawData = read_data(fileName, sheetName);
    show_preference_team(rawData);
    int maxTeamMates{max_teammates()};
    cout << "현재 팀 최대 인원수 : " << maxTeamMates;
}