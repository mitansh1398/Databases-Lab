#include <bits/stdc++.h>

using namespace std;


/*this class contains declarations of all the data structures that are used to perform the given tasks*/

class Transactions{


public:
	void readInput();   // to read the given input file
	void PossibleChains();  // to find all possible chains
	void isRecoverable();  // to check whether schedule is recoverable or not
	void isCascadeless();  // to check whether schedule is cascadeless or not
    void isSerializable();
    void populateGraph();
    bool checkCycleDFS(int x);
    void printOrder();
    void isStrict();

private:
	vector<pair<int, pair<char,int> > > transaction; 
	// it stores all the trasaction details
    unordered_map<int,int> commit;
    // it stores the position of commit operation of a trasaction 
    vector<pair<int,vector<int> > > chain;
    // to store all the possible chains
    vector< pair<int,pair<int,int> > > firstReadArray; 
    // it represents a vector where each elements is a tuple containing tid of write operation, position of first read in a chain and its tid  
    vector< pair<int,pair<int,int> > > firstWriteArray;
    map<int, set<int> > graph;
    map<int, int> color;
    vector<int> serialOrder;
};

void Transactions::readInput(){
	/*
        -->this function read the given input file and store all the transactions details in d.s. 'transactions'
         
	*/
	int Tid, DataItem=0;
    char Opr;
    while(cin>>Tid){
        cin>>Opr;
        if(Opr!='C')
            cin>>DataItem;
        else{
            commit[Tid]=transaction.size();
            // cout<<"commit "<<Tid<<" "<<commit[Tid]<<endl;
        }
        // Not covering if input format is wrong.
        transaction.push_back({Tid,{Opr,DataItem}});
    }
}


void Transactions::PossibleChains(){
	/*
	    --> this method find all the possible chains and store it in d.s. 'chain'
	    --> each chain represents collection of read operations on a dataitem that is written by a transaction before those read operations
        
	*/
	vector<int> temp;
	pair<int,int> firstRead;
	for(int i=0; i<transaction.size(); i++){
        if(transaction[i].second.first!='W')
            continue;

        int DataItem=transaction[i].second.second;

        for(int j=i+1; j<transaction.size(); j++){
            if(transaction[j].second.second!=DataItem||transaction[i].first==transaction[j].first)
                continue;
            if(transaction[j].second.first=='R'){
            	if(temp.size()==0)
            		firstRead={transaction[j].first,j};
            	temp.push_back(transaction[j].first);
            }
            if(transaction[j].second.first=='W'){
                firstWriteArray.push_back({transaction[i].first, {transaction[j].first, j}});
                break;
            }

        }
        if(temp.size()){
            chain.push_back({transaction[i].first,temp});
            firstReadArray.push_back({transaction[i].first,{firstRead}});
        }
        temp.clear();
    }
}


void Transactions::isRecoverable(){
	/*
         --> this method checks whether the given schedule is recoverable or not
	*/
    cout<<"Recoverable: ";
	for(int i=0; i<chain.size(); i++){
        int x = commit[chain[i].first];
        for(int j=0; j<chain[i].second.size(); j++){
            if(x>=commit[chain[i].second[j]]){
            	cout<<"No"<<endl;
            	cout<<"Transaction "<<chain[i].second[j]<<" is reading DataItem written by Transaction "<<chain[i].first<<endl;
            	cout<<"And Transaction "<<chain[i].first<<" commits after Transaction "<<chain[i].second[j]<<endl;
            	return;
            }
        }
    }
    cout<<"Yes"<<endl;
}


void Transactions::isCascadeless(){
	/*
         --> this method checks whether the given schedule is cascadeless or not
	*/
    cout<<"Cascadeless: ";
	for(int i=0; i<firstReadArray.size(); i++){
        int x = commit[firstReadArray[i].first];
        // cout<<x<<" "<<firstReadArray[i].second.second<<" "<<firstReadArray[i].first<<endl;
        if(x>=firstReadArray[i].second.second){
        	cout<<"No"<<endl;
        	cout<<"Transaction "<<firstReadArray[i].second.first<<" is reading DataItem written by Transaction "<<firstReadArray[i].first<<endl;
        	cout<<"And Transaction "<<firstReadArray[i].first<<" commits after reading by Transaction "<<firstReadArray[i].second.first<<endl;
        	return;
        }
    }
    cout<<"Yes"<<endl;
}

void Transactions::populateGraph(){
    for(int i=0; i<transaction.size(); i++){
        set<int> s;
        graph[transaction[i].first] = s;
    }
    for(int i=0; i<transaction.size(); i++){
        color[transaction[i].first] = 0;
        if(transaction[i].second.first=='C'){
            continue;
        }
        for(int j=i+1; j<transaction.size(); j++){
            if(transaction[j].second.first=='C'){
                continue;
            }
            if(transaction[i].second.second==transaction[j].second.second){
                if(transaction[i].first!=transaction[j].first){
                    if(transaction[i].second.first=='W'||transaction[j].second.first=='W'){
                        // cout<<transaction[i].first<<" "<<transaction[j].first<<endl;
                        // cout<<transaction[i].second.first<<" "<<transaction[j].second.first<<endl;
                        // cout<<transaction[i].second.second<<" "<<transaction[j].second.second<<endl;
                        graph[transaction[i].first].emplace(transaction[j].first);
                        // cout<<"inserted"<<endl;
                    }
                }
            }
        }
    }
}

bool Transactions::checkCycleDFS(int x){
    color[x] = 1;
    // cout<<x<<endl;
    for(auto it:graph[x]){
        if(color[it]==0){
            bool check = checkCycleDFS(it);
            if(!check){
                return false;
            }
        } else if(color[it]==1){
            return false;
        }
    }
    color[x] = 2;
    serialOrder.push_back(x);
    return true;
}

void Transactions::isSerializable(){ 
    populateGraph();
    bool check = true;
    for(auto it:graph){
        // cout<<it.first<<" "<<endl;
        if(color[it.first]==0){
            check = checkCycleDFS(it.first);
            if(!check){
                break;
            }
        }
    }
    if(check){
        cout<<"Serializable: Yes"<<endl;
        cout<<"Serial Order: ";
        for(int i=serialOrder.size()-1; i>=0; i--){
            cout<<serialOrder[i]<<" ";
        }
        cout<<endl;
    } else {
        cout<<"Serializable: No"<<endl;
    }
}

void Transactions::isStrict(){
    cout<<"Strict: ";
    for(int i=0; i<firstReadArray.size(); i++){
        int x = commit[firstReadArray[i].first];
        // cout<<x<<" "<<firstReadArray[i].second.second<<" "<<firstReadArray[i].first<<endl;
        if(x>=firstReadArray[i].second.second){
        	cout<<"No"<<endl;
        	cout<<"Transaction "<<firstReadArray[i].second.first<<" is reading DataItem written by Transaction "<<firstReadArray[i].first<<endl;
        	cout<<"And Transaction "<<firstReadArray[i].first<<" commits after reading by Transaction "<<firstReadArray[i].second.first<<endl;
        	return;
        }
    }
    for(int i=0; i<firstWriteArray.size(); i++){
        int x = commit[firstWriteArray[i].first];
        // cout<<x<<" "<<firstReadArray[i].second.second<<" "<<firstReadArray[i].first<<endl;
        if(x>=firstWriteArray[i].second.second){
        	cout<<"No"<<endl;
        	cout<<"Transaction "<<firstWriteArray[i].second.first<<" is writing DataItem written by Transaction "<<firstWriteArray[i].first<<endl;
        	cout<<"And Transaction "<<firstWriteArray[i].first<<" commits after reading by Transaction "<<firstWriteArray[i].second.first<<endl;
        	return;
        }
    }
    cout<<"Yes"<<endl;
}

int main(){

	Transactions obj;

	obj.readInput();   // to read input file

	obj.PossibleChains(); //to find all possible chains

	/*Each chain represents collection of read operation on a dataitem that is written by a transaction before those read operations*/

	obj.isRecoverable();  // to check whether schedule is recoverable or not

	obj.isCascadeless(); // to check whether schedule is cascadeless or not 

	obj.isStrict();

	obj.isSerializable();

	return 0;

}