#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

bool isLeapYear(int year);
int getDaysInMonth(int month, int year);
int daysBetweenDate(int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear);
void binarySearch(vector<int>& day, vector<int>& month, vector<int>& year, vector<long>& value, vector<long long>& cumulative, int start, int finish, int searchday, int searchmonth, int searchyear);
void interpolationSearch(vector<int>& day, vector<int>& month, vector<int>& year, vector<long>& value, vector<long long>& cumulative, int start, int finish, int searchday, int searchmonth, int searchyear);

int main(){

    ifstream file("file.csv");

    if (!file.is_open()){
        cout<<"Error opening file"<<endl;
        return 1;
    }

    int records=0, i=0;
    vector<int> day, month, year;
    vector<long> value;
    vector<long long> cumulative;
    string  searchDate;

    string line;
    while(getline(file, line)){
        vector<string> row;
        stringstream ss(line);
        string cell;

        while (getline(ss, cell, ','))
            row.push_back(cell);

        stringstream ss1(row[2]);
        string dayStr, monthStr, yearStr;
        getline(ss1, dayStr, '/');
        getline(ss1, monthStr, '/');
        getline(ss1, yearStr, '/');

        day.push_back(stoi(dayStr));
        month.push_back(stoi(monthStr));
        year.push_back(stoi(yearStr));
        value.push_back(stol(row[8]));
        cumulative.push_back(stoll(row[9]));

        records ++;
    }

    cout<<"Input date in format dd/mm/yyyy:"<<endl;
    cin>>searchDate;

    stringstream ss(searchDate);
    string dayStr, monthStr, yearStr;
    getline(ss, dayStr, '/');
    getline(ss, monthStr, '/');
    getline(ss, yearStr, '/');

    //Letting the user choose between binary search and interpolation search
    int choice;

    do{
        cout<<"Press 1 to use Binary Search or press 2 to use Interpolation Search"<<endl<<"in order to find out the value and the cumulative of a record with the above date:"<<endl;
        cin>>choice;
    } while(choice!=1 && choice!=2);

    if(choice==1)
        binarySearch(day, month, year, value, cumulative, 0, records-1, stoi(dayStr), stoi(monthStr), stoi(yearStr));
    else
        interpolationSearch(day, month, year, value, cumulative, 0, records-1, stoi(dayStr), stoi(monthStr), stoi(yearStr));

    if(choice==1)
        cout<<endl<<endl<<"Binary Search was executed successfully";
    else
        cout<<endl<<endl<<"Interpolation Search was executed successfully";

    return 0;
}

bool isLeapYear(int year){
    return(year%4==0 && year%100!=0) || (year%400==0);
}

int getDaysInMonth(int month, int year){
    if (month==2)
        return isLeapYear(year)?29:28;
    else if (month==4 || month==6 || month==9 || month==11)
        return 30;
    else
        return 31;
}

int daysBetweenDate(int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear){
    int days=0;

    for(int year=0; year<startYear; year++)
        days-=isLeapYear(year)?366:365;

    for(int month=1; month<startMonth; month++)
        days-=getDaysInMonth(month, endYear);

    days-=startDay;

    for(int year=0; year<endYear; year++)
        days+=isLeapYear(year)?366:365;

    for(int month=1; month<endMonth; month++)
        days += getDaysInMonth(month, endYear);

    days+=endDay;

    int daysBetween=days;

    return daysBetween;
}

void binarySearch(vector<int>& day, vector<int>& month, vector<int>& year, vector<long>& value, vector<long long>& cumulative, int start, int finish, int searchday, int searchmonth, int searchyear){

    int low=start, high=finish, mid;

    do{
        mid=low+(high-low)/2;

        if(year[mid]==searchyear && month[mid]==searchmonth && day[mid]==searchday){
            cout<<"Value: "<<value[mid]<<", Cumulative: "<<cumulative[mid]<<endl;
            break;
        }
        else{
            if(year[mid]<searchyear || (year[mid]==searchyear && month[mid]<searchmonth) || (year[mid]==searchyear && month[mid]==searchmonth && day[mid]<searchday))
                low=mid+1; //search the right half
            else
                high=mid-1; //search the left half
        }

    }while(low<=high);
}

void interpolationSearch(vector<int>& day, vector<int>& month, vector<int>& year, vector<long>& value, vector<long long>& cumulative, int start, int finish, int searchday, int searchmonth, int searchyear){

    int lo=start, hi=finish, mid;

    do{
        //Calculate the interpolated mid-point
        mid=lo+(((hi-lo)/(daysBetweenDate(day[lo], month[lo], year[lo], day[hi], month[hi], year[hi]))) * (daysBetweenDate(day[lo], month[lo], year[lo], searchday, searchmonth, searchyear)));

        if(year[mid]==searchyear && month[mid]==searchmonth && day[mid]==searchday){
            cout<<"Value: "<<value[mid]<<", Cumulative: "<<cumulative[mid]<<endl;
            break;
        }
        else{
            if(year[mid]<searchyear || (year[mid]==searchyear && month[mid]<searchmonth) || (year[mid]==searchyear && month[mid]==searchmonth && day[mid]<searchday))
                lo=mid+1; //search the right part
            else
                hi=mid-1; //search the left part
        }

    }while(lo<=hi);
}
