#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void heapify(vector<long long>& pinakas, int n, int i, vector<int>& day, vector<int>& month, vector<int>& year);
void heapSort(vector<long long>& pinakas, int n, vector<int>& day, vector<int>& month, vector<int>& year);
void quickSort(vector<long long>& pinakas, int start, int finish, vector<int>& day, vector<int>& month, vector<int>& year);
int partition(vector<long long>& pinakas, int start, int finish, vector<int>& day, vector<int>& month, vector<int>& year);

int main(){

    ifstream file("file.csv"); //opening our file

    if (!file.is_open()){
        cout<<"Error opening file"<<endl;
        return 1;
    }

    int records=0, i=0;
    vector <int> day, month, year;
    vector <long long> cumulative;
    string line;

    while(getline(file,line)){
        vector<string> row;
        stringstream ss(line);
        string cell;

        while(getline(ss, cell, ',')) //splitting lines into rows
            row.push_back(cell);

        stringstream ssl(row[2]); //extracting the dates from the 3rd row
        string dayStr, monthStr, yearStr;
        getline(ssl,dayStr,'/');
        getline(ssl,monthStr,'/');
        getline(ssl,yearStr,'/');

        //storing dates as integers and cumulatives as long long numbers in vectors
        day.push_back(stoi(dayStr));
        month.push_back(stoi(monthStr));
        year.push_back(stoi(yearStr));
        cumulative.push_back(stoll(row[9]));
        records++;
    }

    //letting the user choose between the 2 algorithms
    int choice;

    do{
        cout<<"Press 1 to use Heap Sort or press 2 to use Quick Sort in order to sort cumulatives:"<<endl;
        cin>>choice;
    } while(choice!=1 && choice!=2);

    if(choice==1)
        heapSort(cumulative,records,day,month,year);
    else if(choice==2)
        quickSort(cumulative,0,records-1,day,month,year);

    for(i=0; i<records; i++)
        cout<<"Cumulative: "<<cumulative[i]<<" -> date: "<<day[i]<<"/"<<month[i]<<"/"<<year[i]<<"\t";

    if(choice==1)
        cout<<endl<<endl<<"Heap Sort was executed successfully";
    else
        cout<<endl<<endl<<"Quick Sort was executed successfully";

    return 0;
}

void heapify(vector<long long>& pinakas, int n, int i, vector<int>& day, vector<int>& month, vector<int>& year){
    int largest=i; //root
    int left=2*i+1; //left child
    int right=2*i+2; //right child

    if (left<n && pinakas[left]>pinakas[largest]) //check if left child is bigger than the root and if yes set it as largest
        largest=left;

    if (right<n && pinakas[right]>pinakas[largest]) //check if right child is bigger than the largest and if yes set itself as largest
        largest=right;

    if (largest!=i){
        swap(pinakas[i], pinakas[largest]); //if root is not the largest swap the two elements
        heapify(pinakas, n, largest, day, month, year); //recursive call of heapify function in order to heapify the new subtree
    }
}

void heapSort(vector<long long>& pinakas, int n, vector<int>& day, vector<int>& month, vector<int>& year){
    for(int i=n/2-1; i>=0; i--) //start from the last non-leaf node and move up to the root
        heapify(pinakas, n, i, day, month, year); //heapify

    for(int i=n-1; i>=0; i--){
        swap(pinakas[0], pinakas[i]);
        heapify(pinakas, i, 0, day, month, year);
    }
}

void quickSort(vector<long long>& pinakas, int start, int finish, vector<int>& day, vector<int>& month, vector<int>& year){
    int pos;

    if(start<finish){
        pos=partition(pinakas, start, finish, day, month, year);
        quickSort(pinakas, start, pos-1, day, month, year); //recursive call of quickSort applied on the left sub-vector
        quickSort(pinakas, pos+1, finish, day, month, year); //recursive call of quickSort applied on the right sub-vector
    }
}

int partition(vector<long long>& pinakas, int start, int finish, vector<int>& day, vector<int>& month, vector<int>& year){
    int i,j;
    long long pivot;

    pivot=pinakas[start]; //randomly choosing the first element of the vector "pinakas" as the pivot element of the algorithm
    i=start+1; //index that will run through the vector from left to right
    j=finish; //index that will run through the vector from right to left

    while(true){
        while(i<=finish && pinakas[i]<=pivot) //searching for a number bigger than pivot that's on the left of it
            i++;
        while(j>=start && pinakas[j]>pivot) //searching for a number smaller than pivot that's on the right of it
            j--;

        if(i<j)
            swap(pinakas[i], pinakas[j]); //if i is still on the left of j then swap the two numbers that were found from the searching above
        else{
            swap(pinakas[start], pinakas[j]); //else swap the "start" number with the number that j is pointing at and also return j
            return j;
        }
    }
}
