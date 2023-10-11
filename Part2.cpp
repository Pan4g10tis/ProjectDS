#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <cmath>
#include <algorithm>
#include <utility>

using namespace std;

class Node{
    public:
    int year, month, day, value, height;
    Node *left;
    Node *right;
};

class HashTable{
    public:

    int length;
    vector<list<pair<string,long long int>>> table; //Creation of vector of lists


    HashTable(int tableSize){  //The size of the vector is inserted
        length=tableSize;
        table.resize(length);
    }

    int hashFunction(string date){ //The Hash key is calculated by adding together the ascii values of the string date

        long long int sum=0;
        for (int i=0; i<10; i++)
            sum += (long long int)date[i];

        return sum % length;
    }



    void insert(string date, long long int value){ //The pair of a date and its corresponding cumulative is added in the vector of lists
        int index=hashFunction(date);
        table[index].push_back(make_pair(date, value));
    }


    long long int search(string date){ //By comparing the dates the first cumulative value of they date that is inserted is returned to the user
        int index = hashFunction(date);

        for(auto& pair:table[index]){
            if(pair.first==date)
                return pair.second;
        }

        cout<<"The date no longer exists"<<endl; //if the date is not found print this message
        return -1;
    }

    void edit(string date, long long int new_value){ // The date and cumulative requested are found and we override the old cumulative with the new (new_value)
        long long int j=new_value;
        int index=hashFunction(date);

        for (auto& pair:table[index]){
                if (pair.first==date){
                    pair.second=j;
                    cout<<"Edit succesfull"<<endl;
                    return;
                }
        }
    }

    void remove(string date){ //The date and cumulative requested are found and the list is being erased from the vector
        int index=hashFunction(date);
        table.erase(table.begin()+index);
        cout<<"Deletion successfull"<<endl;
        return;
    }
};

int getHeight(Node *N);
int compDate(int year1, int month1, int day1, int year2, int month2, int day2);
Node *newNode(int day, int month, int year, int value);
Node *rightRotate(Node *y);
Node *leftRotate(Node *x);
int getBalance(Node *N);
Node *insertNodeDate(Node *node, int day, int month, int year, int value);
Node *insertNodeValue(Node *node, int day, int month, int year, int value);
Node *nodeWithMinValue(Node *node);
Node *nodeWithMaxValue(Node *node);
Node *deleteNode(Node *root, int day, int month, int year);
int searchNode(Node* root, int day, int month, int year);
void printInOrder(Node *root);
Node *modifyNode(Node *root, int day, int month, int year, int newValue);

int main(){
    ifstream file("file.csv");

    if (!file.is_open()){
        cout << "Error opening file" << endl;
        return 1;
    }

    int records=0, action=0, modify, decision1, decision2;
    string searchDate;

    Node *root=NULL;
    Node *searchResult=NULL;

    string line;

    //Letting the user choose between a BST and a Chain Hashing Table
    do{
        cout<<"Press 1 to use a BST or press 2 to use a Chain Hashing Table:"<<endl;
        cin>>decision1;
    }while(decision1!=1 && decision1!=2);

    if(decision1==1){
        //Letting the user choose between a date-based BST and a value-based BST
        do{
            cout<<"Press 1 to use a BST based on the dates or press 2 to use a BST based on the values:"<<endl;
            cin>>decision2;
        }while(decision2!=1 && decision2!=2);

        if(decision2==1){
            while (getline(file, line)){
                vector<string> row;
                stringstream ss(line);
                string cell;

                while (getline(ss, cell, ','))
                    row.push_back(cell);

                vector<int> datenvalue;

                stringstream ss1(row[2]);
                string dayStr, monthStr, yearStr;
                getline(ss1, dayStr, '/');
                getline(ss1, monthStr, '/');
                getline(ss1, yearStr, '/');

                root=insertNodeDate(root, stoi(dayStr), stoi(monthStr), stoi(yearStr), stoi(row[8]));

                records ++;
            }

            do{
                do{
                    cout<<"Press 1 for in-order print"<<endl<<"or"<<endl;
                    cout<<"Press 2 to search for the value of a specific date"<<endl<<"or"<<endl;
                    cout<<"Press 3 to edit the value of a specific date"<<endl<<"or"<<endl;
                    cout<<"Press 4 to delete a specific date"<<endl<<"or"<<endl;
                    cout<<"Press 5 to exit program"<<endl;
                    cin>>action;
                }while(action<1 || action>5);

                switch(action){
                    case 1:
                        {
                        printInOrder(root);
                        cout<<endl<<"--------------------"<<endl;
                        break;
                        }
                    case 2:
                        {
                        cout<<"Input date (dd/mm/yyyy format):"<<endl;
                        cin>>searchDate;
                        stringstream ss(searchDate);
                        string dayStr, monthStr, yearStr;
                        getline(ss, dayStr, '/');
                        getline(ss, monthStr, '/');
                        getline(ss, yearStr, '/');
                        int result=searchNode(root, stoi(dayStr), stoi(monthStr), stoi(yearStr));
                        cout<<"Value: "<<result<<endl<<"--------------------"<<endl;
                        break;
                        }
                    case 3:
                        {
                        cout<<"Input date (dd/mm/yyyy format):"<<endl;
                        cin>>searchDate;
                        stringstream ss(searchDate);
                        string dayStr, monthStr, yearStr;
                        getline(ss, dayStr, '/');
                        getline(ss, monthStr, '/');
                        getline(ss, yearStr, '/');
                        cout<<"Input new value:"<<endl;
                        cin>>modify;
                        root=modifyNode(root, stoi(dayStr), stoi(monthStr), stoi(yearStr), modify);
                        cout<<"Value changed to "<<modify<<" successfully"<<endl<<"--------------------"<<endl;
                        break;
                        }
                    case 4:
                        {
                        cout<<"Input date (dd/mm/yyyy format):"<<endl;
                        cin>>searchDate;
                        stringstream ss(searchDate);
                        string dayStr, monthStr, yearStr;
                        getline(ss, dayStr, '/');
                        getline(ss, monthStr, '/');
                        getline(ss, yearStr, '/');
                        root=deleteNode(root, stoi(dayStr), stoi(monthStr), stoi(yearStr));
                        cout<<"Date deleted successfully"<<endl<<"--------------------"<<endl;
                        break;
                        }
                }
            }while(action!=5);

            cout<<"Exited program"<<endl;
        }
        else{
            while (getline(file, line)) {
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

                root=insertNodeValue(root, stoi(dayStr), stoi(monthStr), stoi(yearStr), stoi(row[8]));

                records ++;
            }

            do{
                cout<<"Press 1 to find out the date with the lowest value or 2 for the date with the highest value: "<<endl;
                cin>>action;
            }while(action!=1 && action!=2);

            switch(action){
                case 1:
                    {
                    searchResult=nodeWithMinValue(root);
                    cout<<"The date with the lowest value is: "<<searchResult->day<<"/"<<searchResult->month<<"/"<<searchResult->year<<endl;
                    break;
                    }
                case 2:
                    {
                    searchResult=nodeWithMaxValue(root);
                    cout<<"The date with the highest value is: "<<searchResult->day<<"/"<<searchResult->month<<"/"<<searchResult->year<<endl;
                    break;
                    }
            }
        }
    }
    else{
        //A Chain Hashing Table was chosen
        vector<string> date;
        vector<long long int> value;

        while(getline(file,line)){
            vector<string> row;
            stringstream ss (line);
            string cell;

            while (getline(ss,cell, ','))
                row.push_back(cell);

            date.push_back(row[2]);
            value.push_back(stoll(row[8]));
            records++;
        }

        vector<pair<string,long long int>> combined; //Creating a vector that has as members both dates and cumulatives

        for(int i=0;i<records;i++)
            combined.push_back(make_pair(date[i],value[i]));

        HashTable myTable(2541);//2541 is the number of all the dates without including any repetitions

        for(int i=0;i<records;i++)//The pairs of dates and cumulatives are inserted in the HashTable
            myTable.insert(combined[i].first,combined[i].second);

        string given_date;
        int choice,c;

        do{
            do{
                cout<<"Press 1 to search for the value of a specific date"<<endl<<"or"<<endl;
                cout<<"Press 2 to edit the value of a specific date"<<endl<<"or"<<endl;
                cout<<"Press 3 to delete a specific date"<<endl<<"or"<<endl;
                cout<<"Press 4 to exit"<<endl;
                cin>>choice;
            }while(choice!=1 && choice!=2 && choice!=3 && choice!=4);

            switch(choice){
                case 1:
                    {
                    cout<<"Give a date (dd/mm/yyyy format) you want a value for: "<<endl;
                    cin>>given_date;
                    long long int c=myTable.search(given_date);
                    cout<<"Value: "<<c<<endl;
                    break;
                    }
                case 2:
                    {
                    long long int given_value;
                    cout<<"Give a date (dd/mm/yyyy format) for which you want to change the value"<<endl;
                    cin>>given_date;
                    cout<<"Give the new value"<<endl;
                    cin>>given_value;
                    myTable.edit(given_date,given_value);
                    c=myTable.search(given_date);
                    cout<<"New value: "<<c<<endl;
                    break;
                    }
                case 3:
                    {
                    cout<<"Give a date (dd/mm/yyyy format) you want to remove"<<endl;
                    cin>>given_date;
                    myTable.remove(given_date);
                    c=myTable.search(given_date);
                    break;
                }
            }
        }while(choice!=4);

        cout<<"Exited program"<<endl;
        return 0;
    }

    return 0;
}

int getHeight(Node *N){
    if(N==NULL)
        return 0;

    return N->height;
}

int max(int a, int b){
    return (a>b)?a:b;
}

int compDate(int year1, int month1, int day1, int year2, int month2, int day2){
    if(year1==year2 && month1==month2 && day1==day2)
        return 0;
    if(year1<year2 || (year1==year2 && month1<month2) || (year1==year2 && month1==month2 && day1<day2))
        return 1;
    return -1;
}

Node *newNode(int day, int month, int year, int value){
    Node *node=new Node();
    node->year=year;
    node->month=month;
    node->day=day;
    node->value=value;
    node->left=NULL;
    node->right=NULL;
    node->height=1;
    return (node);
}

Node *rightRotate(Node *y){
    Node *x=y->left;
    Node *T2=x->right;
    x->right=y;
    y->left=T2;
    y->height=max(getHeight(y->left), getHeight(y->right))+1;
    x->height=max(getHeight(x->left), getHeight(x->right))+1;
    return x;
}

Node *leftRotate(Node *x){
    Node *y=x->right;
    Node *T2=y->left;
    y->left=x;
    x->right=T2;
    x->height=max(getHeight(x->left), getHeight(x->right))+1;
    y->height=max(getHeight(y->left), getHeight(y->right))+1;
    return y;
}

int getBalance(Node *N){
    if (N==NULL)
        return 0;

    return getHeight(N->left)-getHeight(N->right);
}

Node *insertNodeDate(Node *node, int day, int month, int year, int value){
    if(node==NULL)
        return (newNode(day, month, year, value));

    //Compare the dates to determine the insertion position
    if(compDate(year, month, day, node->year, node->month, node->day)==1)
        node->left=insertNodeDate(node->left, day, month, year, value);
    else if(compDate(year, month, day, node->year, node->month, node->day)==-1)
        node->right=insertNodeDate(node->right, day, month, year, value);
    else
        return node; //Do not insert the date if it already exists in the tree

    //Update the height of the current node
    node->height=1+max(getHeight(node->left),  getHeight(node->right));
    int balanceFactor=getBalance(node); //Check the balance factor of the current node

    //Make rotations if necessary to fix the balance
    if(balanceFactor>1){
        if(compDate(year, month, day, node->left->year, node->left->month, node->left->day)==1)
            return rightRotate(node);
        else if(compDate(year, month, day, node->left->year, node->left->month, node->left->day)==-1){
            node->left=leftRotate(node->left);
            return rightRotate(node);
        }
    }

    if(balanceFactor<-1){
        if(compDate(year, month, day, node->right->year, node->right->month, node->right->day)==-1)
            return leftRotate(node);
        else if(compDate(year, month, day, node->right->year, node->right->month, node->right->day)==1){
            node->right=rightRotate(node->right);
            return leftRotate(node);
        }
    }

    return node;
}

Node *insertNodeValue(Node *node, int day, int month, int year, int value){

    if(node==NULL)
        return (newNode(day, month, year, value));

    //Compare the values to determine the insertion position
    if(value<node->value)
        node->left=insertNodeValue(node->left, day, month, year, value);
    else if(value>node->value)
        node->right=insertNodeValue(node->right, day, month, year, value);
    else
        return node; //Do not insert the value if it already exists in the tree

    //Update the height of the current node
    node->height=1+max(getHeight(node->left), getHeight(node->right));
    int balanceFactor=getBalance(node); //Check the balance factor of the current node

    //Make rotations if necessary to fix the balance
    if(balanceFactor>1){
        if(value<node->left->value)
            return rightRotate(node);
        else if(value>node->left->value){
            node->left=leftRotate(node->left);
            return rightRotate(node);
        }
    }

    if(balanceFactor<-1){
        if(value>node->right->value)
            return leftRotate(node);
        else if(value<node->right->value){
            node->right=rightRotate(node->right);
            return leftRotate(node);
        }
    }

    return node;
}

Node *nodeWithMinValue(Node *node){
    Node *current=node;
    while(current->left!=NULL)
        current=current->left;

    return current;
}

Node *nodeWithMaxValue(Node *node){
    Node *current=node;
    while (current->right!=NULL)
        current=current->right;

    return current;
}

Node *deleteNode(Node *root, int day, int month, int year){
    if(root==NULL)
        return root;

    if(compDate(year, month, day, root->year, root->month, root->day)==1)
        root->left=deleteNode(root->left, day, month, year); //Recursively visit the left subtree to find the node to delete
    else if(compDate(year, month, day, root->year, root->month, root->day)==-1)
        root->right=deleteNode(root->right, day, month, year); //Recursively visit the right subtree to find the node to delete
    else{
        //wanted node is found
        if((root->left==NULL) || (root->right==NULL)){
            //node has max 1 child
            Node *temp=root->left?root->left:root->right;

            if(temp==NULL){
                //node has no child
                temp=root;
                root=NULL;
            }
            else
                *root=*temp;

            free(temp);
        }
        else{
            //node has 2 children
            Node *temp=nodeWithMinValue(root->right);
            root->year=temp->year;
            root->month=temp -> month;
            root->day=temp->day;
            root->right=deleteNode(root->right, temp->day, temp->month, temp->year);
        }
    }

    if(root==NULL)
        return root;

    root->height=1+max(getHeight(root->left), getHeight(root->right));
    int balanceFactor=getBalance(root);

    //Make rotations if necessary to fix the balance
    if(balanceFactor>1){
        if(getBalance(root->left)>=0)
            return rightRotate(root);
        else{
            root->left=leftRotate(root->left);
            return rightRotate(root);
        }
    }

    if(balanceFactor<-1){
        if(getBalance(root->right)<=0)
            return leftRotate(root);
        else{
            root->right=rightRotate(root->right);
            return leftRotate(root);
        }
    }

    return root;
}

int searchNode(Node* root, int day, int month, int year){
    int result;

    if(compDate(year, month, day, root->year, root->month, root->day)==0)
        result=root->value; //Node with the given date is found
    else if(compDate(year, month, day, root->year, root->month, root->day)==1)
        result=searchNode(root->left, day, month, year); //Recursively search in the left subtree
    else if(compDate(year, month, day, root->year, root->month, root->day)==-1)
        result=searchNode(root->right, day, month, year); //Recursively search in the right subtree

    return result;
}

void printInOrder(Node *root){
    if(root!=nullptr){
        printInOrder(root->left); //Recursively visit the left subtree
        cout<<" "<<root->value; //Print the value of the current node
        printInOrder(root->right); //Recursively visit the left subtree
    }
}

Node *modifyNode(Node *root, int day, int month, int year, int newValue){
    if(compDate(year, month, day, root->year, root->month, root->day)==0)
        root->value=newValue; //Node with the given date is found so modify it's value
    else if(compDate(year, month, day, root->year, root->month, root->day)==1)
        root=modifyNode(root->left, day, month, year, newValue); //Recursively call modifyNode for the left subtree
    else if(compDate(year, month, day, root->year, root->month, root->day)==-1)
        root=modifyNode(root->right, day, month, year, newValue); //Recursively call modifyNode for the right subtree

    root->height=1+max(getHeight(root->left), getHeight(root->right));
    int balanceFactor=getBalance(root);

    if(balanceFactor>1){
        if(compDate(year, month, day, root->left->year, root->left->month, root->left->day)==1)
            return rightRotate(root);
        else if(compDate(year, month, day, root->left->year, root->left->month, root->left->day)==-1){
            root->left=leftRotate(root->left);
            return rightRotate(root);
        }
    }

    if(balanceFactor<-1){
        if(compDate(year, month, day, root->right->year, root->right->month, root->right->day)==-1)
            return leftRotate(root);
        else if(compDate(year, month, day, root->right->year, root->right->month, root->right->day)==1){
            root->right=rightRotate(root->right);
            return leftRotate(root);
        }
    }

    return root;
}
