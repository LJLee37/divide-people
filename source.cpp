#include <bits/stdc++.h>
#include <xlnt/xlnt.hpp>
using namespace std;
using namespace xlnt;

//Data types
//Excel Data type
typedef vector<pair<string, vector<char>>> ExcelData_t;
typedef vector<pair<string, char>> FinalData_t;

//function prototypes
//read an excel file and returns ExcelData_t
ExcelData_t read_data(const string& fileName);
//show preference of team
void show_preference_team(const ExcelData_t& sheetData);
//get max teammates
int max_teammates();
//spread people by their priority
FinalData_t spread_people(ExcelData_t people, const int& maxNum);
//write FinalData_t to an excel file
void write_data(const string& fileName, const FinalData_t& finalData);
//show the result of spreading
void show_result(const FinalData_t& finalData);

//function definitions
ExcelData_t read_data(const string& fileName)
{
    workbook wb;
    wb.load(fileName);
    auto reading = wb.active_sheet();
    ExcelData_t retval;
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
void show_preference_team(const ExcelData_t& sheetData)
{
    cout << "제 1지망 선택 인원수" << endl;
    vector<int> number(5);
    for (auto i : sheetData)
        number[i.second[0] - 'A']++;
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
FinalData_t spread_people(ExcelData_t people, const int& maxNum)
{
    vector<vector<string>> teams ;
    for (int i = 0; i < 5; i++)
        teams.push_back(vector<string>{});
    for (auto i : people)
    {
        auto j = i.second[0];
        teams[j - 'A'].push_back(i.first);
    }
    vector<int> overNumTeamIndex;
    for(auto i = 0; i < teams.size(); i++)
        if(teams[i].size() > maxNum)
            overNumTeamIndex.push_back(i);
    while(!overNumTeamIndex.empty())
    {
        //
    }
    FinalData_t retval;
    for(auto i = 0; i < teams.size(); i++)
    {
        sort(teams[i].begin(),teams[i].end());
        for (auto j : teams[i])
            retval.push_back(make_pair(j, i + 'A'));
    }
    return retval;
}
void write_data(const string& fileName, const FinalData_t& finalData)
{
    workbook wb;
    worksheet writing = wb.active_sheet();
    writing.title("Data");

}
void show_result(const FinalData_t& finalData)
{
    //
}

//main
int main()
{
    cout << "조원 분배 프로그램입니다." << endl;
    cout << "엑셀 파일 이름을 입력하세요 : ";
    string fileName;
    cin >> fileName;
    ExcelData_t rawData = read_data(fileName);
    show_preference_team(rawData);
    int maxTeamMates{max_teammates()};
    cout << "현재 팀 최대 인원수 : " << maxTeamMates << endl;
    cout << "경고! 분배가 불가능할 경우 프로그램이 종료될 때까지 분배를 시도합니다." << endl;
    cout << "시간이 지나도 분배가 되지 않는 경우 프로그램을 종료하고 팀 최대 인원수를 늘려서 다시 시도하십시오." << endl;
    cout << "분배중..." << endl;
    FinalData_t finalData{spread_people(rawData, maxTeamMates)};
    cout << "분배 성공!" << endl;
    cout << "분배 결과를 출력합니다..." << endl;
    show_result(finalData);
    cout << "이대로 저장하시겠습니까? (Y|n) : ";
    char sw;
    cin >> sw;
    switch (sw)
    {
    case 'Y':
    case 'y':
        cout << "저장할 파일 이름을 입력하세요 : ";
        cin >> fileName;
        write_data(fileName, finalData);
        cout << "저장을 완료했습니다. ";
        break;
    case 'N':
    case 'n':
        cout << "저장 없이 ";
        break;
    default:
        cout << "잘못된 입력입니다. ";
        break;
    }
    cout << "종료합니다." << endl;
    return 0;
}