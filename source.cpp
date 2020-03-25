#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <xlnt/xlnt.hpp>
using namespace std;
using namespace xlnt;
#define DEBUGMODE false

//Data types
//Excel Data type
typedef vector<pair<string, vector<char>>> ExcelData_t;
typedef vector<pair<string, string>> FinalData_t;

//function prototypes
//read an excel file and returns ExcelData_t
ExcelData_t read_data(string& fileName, bool checkDuplicate = false);
//show preference of team
void show_preference_team(const ExcelData_t& sheetData);
//get max teammates
int max_teammates();
//spread people by their priority
FinalData_t spread_people(ExcelData_t& people, const int& maxNum);
//write FinalData_t to an excel file
void write_data(const string& fileName, const FinalData_t& finalData);
//show the result of spreading
void show_result(const FinalData_t& finalData);
//add people to noMoving
void add_noMoving(const ExcelData_t& people, const int& maxNum, vector<vector<string>>& teams, vector<string>& noMoving);
//return if nkoMoving contains person
bool is_noMoving(vector<string>& noMoving, string& person);

//function definitions
bool is_noMoving(vector<string>& noMoving, string& person)
{
    for ( auto i : noMoving)
        if(i == person)
            return true;
    return false;
}
ExcelData_t read_data(string& fileName, bool checkDuplicate = false)
{
    workbook wb;
    redo:
    try
    {
        wb.load(fileName);
    }
    catch (const std::exception& ex)
    {
        cout << ex.what() << endl;
        cout << "파일을 찾을 수 없습니다. 파일 이름을 다시 입력해주세요 : ";
        cin >> fileName;
        goto redo;
    }
    
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
    if(checkDuplicate)
    {
        sort( retval.begin(), retval.end() );
        retval.erase( unique( retval.begin(), retval.end() ), retval.end() );
        for (auto i : retval)
        {
            for (auto j : retval)
        {
            if(i.first == j.first)
                if(i != j)
                {
                    cout << "경고! 중복되는 학번이 있습니다. 분배 과정에서 프로그램이 멈출 가능성이 있습니다." << endl;
                    cout << "중복 학번 : " << i.first << endl;
                }
        }
    }}
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
FinalData_t spread_people(ExcelData_t& people, const int& maxNum)
{
    vector<vector<string>> teams;
    for (int i = 0; i < 5; i++)
        teams.push_back(vector<string>{});
    for (auto i : people)
    {
        auto j = i.second[0];
        teams[j - 'A'].push_back(i.first);
    }
    vector<string> noMoving;
    for(add_noMoving(people, maxNum, teams, noMoving); noMoving.size() < people.size(); add_noMoving(people, maxNum, teams, noMoving))
    {
        vector<int>leftSeats;
        for(auto i : teams)
            leftSeats.push_back(maxNum - i.size());
        ExcelData_t moving;
        for(auto i : people)
            if(!is_noMoving(noMoving, i.first))
                moving.push_back(i);
        for(auto i : moving)
        {
            bool moved = false;
            for(auto j : i.second)
            {
                if(moved)
                    break;
                if(leftSeats[(j - 'A')] > 0)
                {
                    for (auto k = 0; k < 5; k++)
                    {
                        auto temp = find(teams[k].begin(), teams[k].end(), i.first);
                        if((temp != teams[k].end()) || teams[k].back() == i.first)
                        {
                            teams[k].erase(temp);
                            break;
                        }
                    }
                    cout << i.first << endl;
                    teams[(j - 'A')].push_back(i.first);
                    moved = true;
                }
            }
        }
        cout << "one loop done" << endl;
    }
    for(auto i : teams)
        if(i.size() > maxNum)
        {
            cout << "분배가 불가능합니다." << endl;
            cout << "Abort" << endl;
            abort();
        }
    FinalData_t retval;
    for(auto i = 0; i < teams.size(); i++)
    {
        sort(teams[i].begin(),teams[i].end());
        for (auto j : teams[i])
            retval.push_back(make_pair(j, string{char(i + 'A')}));
    }
    return retval;
}
void write_data(const string& fileName, const FinalData_t& finalData)
{
    workbook wb;
    worksheet writing = wb.active_sheet();
    writing.title("Data");
    for (int i = 0; i < finalData.size(); i++)
    {
        writing.cell(cell_reference(1, i + 1)).value(finalData[i].first);
        writing.cell(cell_reference(2, i + 1)).value(finalData[i].second);
    }
    wb.save(fileName);
}
void show_result(const FinalData_t& finalData)
{
    for (auto i : finalData)
        cout << i.first << ": " << i.second << "조 입니다." << endl;
}
void add_noMoving(const ExcelData_t& people, const int& maxNum, vector<vector<string>>& teams, vector<string>& noMoving)
{
    for (auto i : teams)
    {
        if (i.size() <= maxNum)
            for (auto j : i)
                if(!is_noMoving(noMoving, j))
                {    
                    noMoving.push_back(j);
                }
        else
            for (auto j : i)
                for (auto k : people)
                    if(j == k.first)
                    {
                        if (k.second.size() == 1)
                        {
                            if(!is_noMoving(noMoving, j))
                            {    
                                noMoving.push_back(j);
                            }
                        }
                    
                        else 
                        {
                            bool tempFlag = false;
                            for (auto l : k.second)
                                if(teams[l].size() <= maxNum)
                                {
                                    tempFlag = true;
                                    break;
                                }
                            if(!tempFlag)
                            {
                                if(!is_noMoving(noMoving, j))
                                {    
                                    noMoving.push_back(j);
                                }
                            }
                        }
                    }
    }
}


//main
int main()
{
    cout << "조원 분배 프로그램입니다." << endl;
    cout << "엑셀 파일 이름을 입력하세요 : ";
    string fileName{"Test_case.xlsx"};
    #if DEBUGMODE == flase
    cin >> fileName;
    #endif
    cout << "파일에 중복이 있는지 체크하시겠습니까? (Y|n) : ";
    cout << "이대로 저장하시겠습니까? (Y|n) : ";
    char chkdpl;
    cin >> chkdpl;
    bool check = false;
    switch (chkdpl)
    {
    case 'Y':
    case 'y':
        cout << "중복이 있는지 확인합니다..." << endl;
        check = true;
        break;
    case 'N':
    case 'n':
        cout << "중복을 확인하지 않습니다. 중복이 있다면 프로그램이 분배과정에서 가만히 있습니다." << endl;
        break;
    default:
        cout << "잘못된 입력입니다. 중복을 확인하지 않습니다."<<endl;
        break;
    }
    ExcelData_t rawData = read_data(fileName, check);
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
        #if DEBUGMODE == true
        fileName = "a.xlsx";
        #else
        cin >> fileName;
        #endif
        write_data(fileName, finalData);
        cout << "저장을 완료했습니다." << endl;
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