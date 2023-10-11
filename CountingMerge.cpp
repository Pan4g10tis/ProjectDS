#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

bool isLeapYear(int year);
int getDaysInMonth(int month, int year);
int daysBetweenDate(int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear);
void merge(vector<int> &numofdays, int left, int mid, int right, vector<int> &day, vector<int> &month, vector<int> &year, vector<long> &value);
void mergeSort(vector<int> &numofdays, int left, int right, vector<int> &day, vector<int> &month, vector<int> &year, vector<long> &value);
void countingSort(vector<int>& numofdays, int length, vector<int>& day, vector<int>& month, vector<int>& year, vector<long>& value);

int main(){

    ifstream file("file.csv");

    if (!file.is_open()){
        cout<<"Error opening file"<<endl;
        return 1;
    }

    int records=0, i=0;
    vector<int> day, month, year, numofdays;
    vector<long> value;

    string line;
    while(getline(file, line)){
        vector<string> row;
        stringstream ss(line);
        string cell;

        while(getline(ss, cell, ','))
            row.push_back(cell);

        //extracting day, month and year from the string
        stringstream ss1(row[2]);
        string dayStr, monthStr, yearStr;
        getline(ss1, dayStr, '/');
        getline(ss1, monthStr, '/');
        getline(ss1, yearStr, '/');

        day.push_back(stoi(dayStr));
        month.push_back(stoi(monthStr));
        year.push_back(stoi(yearStr));
        value.push_back(stol(row[8]));

        records ++;
    }

    for(i=0; i<records; i++)
        numofdays.push_back(daysBetweenDate(day[0], month[0], year[0], day[i], month[i], year[i]));

    int choice; //letting the user decide which algorithm he wants to use

    do{
        cout<<"Press 1 to use Merge Sort or press 2 to use Counting Sort in order to sort the dates: "<<endl;
        cin>>choice;
    } while(choice!=1 && choice!=2);

    if(choice==1)
        mergeSort(numofdays, 0, records-1, day, month, year, value);
    else
        countingSort(numofdays, records, day, month, year, value);

    for(i=0; i<records; i++)
        cout<<"Date: "<<day[i]<<"/"<<month[i]<<"/"<<year[i]<<" -> Value: "<<value[i]<<"\t";

    if(choice==1)
        cout<<endl<<endl<<"Merge Sort was executed successfully";
    else
        cout<<endl<<endl<<"Counting Sort was executed successfully";

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

void merge(vector<int> &numofdays, int left, int mid, int right, vector<int> &day, vector<int> &month, vector<int> &year, vector<long> &value){

    int n1=mid-left+1;
    int n2=right-mid;

    //Creating temporary vectors to store the elements of the two subarrays
    vector<int> leftnum, rightnum;
    vector<int> leftday, rightday;
    vector<int> leftmonth, rightmonth;
    vector<int> leftyear, rightyear;
    vector<long> leftvalue, rightvalue;

    //Copying the elements from the original arrays to the temporary arrays
    for(int i=0; i<n1; i++){
        leftnum.push_back(numofdays[left + i]);
        leftday.push_back(day[left + i]);
        leftmonth.push_back(month[left + i]);
        leftyear.push_back(year[left + i]);
        leftvalue.push_back(value[left + i]);
    }

    for(int j=0; j<n2; j++){
        rightnum.push_back(numofdays[mid + 1 + j]);
        rightday.push_back(day[mid + 1 + j]);
        rightmonth.push_back(month[mid + 1 + j]);
        rightyear.push_back(year[mid + 1 + j]);
        rightvalue.push_back(value[mid + 1 + j]);
    }

    int i=0,j=0,k=left;

    //Merging the temporary arrays back into the original arrays in sorted order
    while(i<n1 && j<n2){
        if(leftnum[i]==rightnum[j]){
            if(leftvalue[i]<=rightvalue[j]){
                numofdays[k]=leftnum[i];
                day[k]=leftday[i];
                month[k]=leftmonth[i];
                year[k]=leftyear[i];
                value[k]=leftvalue[i];
                i++;
            }
            else{
                numofdays[k]=rightnum[j];
                day[k]=rightday[j];
                month[k]=rightmonth[j];
                year[k]=rightyear[j];
                value[k]=rightvalue[j];
                j++;
            }
        }
        else{
            if(leftnum[i]<rightnum[j]){
                numofdays[k]=leftnum[i];
                day[k]=leftday[i];
                month[k]=leftmonth[i];
                year[k]=leftyear[i];
                value[k]=leftvalue[i];
                i++;
            }
            else{
                numofdays[k]=rightnum[j];
                day[k]=rightday[j];
                month[k]=rightmonth[j];
                year[k]=rightyear[j];
                value[k]=rightvalue[j];
                j++;
            }
        }
        k++;
    }

    //Copying the remaining elements of the left subarray (if there are any)
    while(i<n1){
        numofdays[k]=leftnum[i];
        day[k]=leftday[i];
        month[k]=leftmonth[i];
        year[k]=leftyear[i];
        value[k]=leftvalue[i];
        i++;
        k++;
    }

    //Copying the remaining elements of the right subarray (if there are any)
    while(j<n2){
        numofdays[k]=rightnum[j];
        day[k]=rightday[j];
        month[k]=rightmonth[j];
        year[k]=rightyear[j];
        value[k]=rightvalue[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int> &numofdays, int left, int right, vector<int> &day, vector<int> &month, vector<int> &year, vector<long> &value){
    if(left<right){
        int mid=left+(right-left)/2;

        //Recursive calling mergeSort to divide the array into subarrays
        mergeSort(numofdays, left, mid, day, month, year, value);
        mergeSort(numofdays, mid + 1, right, day, month, year, value);

        //Merging the sorted subarrays
        merge(numofdays, left, mid, right, day, month, year, value);
  }
}

void countingSort(vector<int>& numofdays, int length, vector<int>& day, vector<int>& month, vector<int>& year, vector<long>& value){

    int i, maxElement=0;

    for(i=0; i<length; i++){
        if(numofdays[i]>maxElement)
            maxElement = numofdays[i];
    }

    //Creating a counting array of size maxNumofDays + 1
    vector<int> counter;

    for(i=0; i<=maxElement+1; i++)
        counter.push_back(0);

    //Counting the number of appearances of each numofdays
    for(i=0; i<length; i++)
        counter[numofdays[i]]++;

    //Adding the count of the previous element to the count of the current element
    for(i=1; i<=maxElement; i++)
        counter[i]+=counter[i-1];

    //Creating temporary arrays to store the sorted elements
    vector<int> outputnum(length);
    vector<int> outputday(length), outputmonth(length), outputyear(length);
    vector<long> outputvalue(length);

    //Sorting the elements based on the count array
    for(i=length-1; i>=0; i--){
        outputnum[counter[numofdays[i]] - 1] = numofdays[i];
        outputday[counter[numofdays[i]] - 1] = day[i];
        outputmonth[counter[numofdays[i]] - 1] = month[i];
        outputyear[counter[numofdays[i]] - 1] = year[i];
        outputvalue[counter[numofdays[i]] - 1] = value[i];
        counter[numofdays[i]]--;
    }

    //Copying the sorted elements back to the original arrays
    for(i=0; i<length; i++){
        numofdays[i] = outputnum[i];
        day[i] = outputday[i];
        month[i] = outputmonth[i];
        year[i] = outputyear[i];
        value[i] = outputvalue[i];
    }
}
