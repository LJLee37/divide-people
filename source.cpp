#include <bits/stdc++.h>
#include <xlnt/xlnt.hpp>
using namespace std;
using namespace xlnt;

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
ExelData_t read_data(const string fileName)
{
    workbook wb;
    wb.load(fileName);
    auto reading = wb.active_sheet();
    ExelData_t retval;
    for (auto row : reading.rows())
    {
        vector<string> temp;
        for(auto cell : row)
        {
            temp.push_back(cell.to_string());
        }
        string hakbun = temp.front();
        reverse(temp.begin(), temp.end());
        temp.pop_back();
        vector<char> tempPri;
        for(auto i : temp)
            tempPri.push_back(i[0]);
        reverse(tempPri.begin(),tempPri.end());
        retval.push_back(make_pair(hakbun,tempPri));
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
    string fileName;
    cin >> fileName;
    ExelData_t rawData = read_data(fileName);
    show_preference_team(rawData);
    int maxTeamMates{max_teammates()};
    cout << "현재 팀 최대 인원수 : " << maxTeamMates;
}