#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

struct Person {
    string name;
    string parent;
    int birth;
    string death;
    string religion;
    string gender;
};

int n;
vector < Person > persons;
vector < vector < int > > descendants;
vector < int > visited;
vector < int > ans;

bool operator < (const Person& lhs, const Person& rhs) {
    if (lhs.gender != rhs.gender) {
        if (lhs.gender == "M") {
            return true;
        } else {
            return false;
        }        
    }
    
    return lhs.birth < rhs.birth;
}

bool cmp(int lhs, int rhs) {
    return persons[lhs] < persons[rhs];
}

int getNumberFromName(string name) {
    for (int i = 0; i < n; ++i) {
        if (persons[i].name == name) {
            return i;
        }
    }
    
    return -1;
}

bool include(int i) {
    if (persons[i].death != "-") {
        return false;    
    }
    if (persons[i].religion == "Catholic") {
        return false;   
    }
    return true;
}

void dfs(int v) {
    for (const auto& child : descendants[v]) {
        if (!visited[child]) {
            visited[child] = 1;
            ans.push_back(child);
            dfs(child);
        }
    }
}

int main() {
    cin >> n;
    persons.resize(n);
    descendants.resize(n);
    int k = 0;
    for (auto& person : persons) {
        cin >> person.name >> person.parent >> person.birth >> person.death >> person.religion >> person.gender;
        
        /*
        cerr << k << " -> ";
        cerr << person.name << " ";
        cerr << person.parent << " ";
        cerr << person.birth << " ";
        cerr << person.death << " ";
        cerr << person.religion << " ";
        cerr << person.gender << endl;
        
        ++k;
        //*/
    }
    
    int first = 0;    
    for (int i = 0; i < n; ++i) {
        if (persons[i].parent != "-") {
            descendants[getNumberFromName(persons[i].parent)].push_back(i);
        } else {
            first = i;    
        }
    }
    
    for (auto& children : descendants) {
        sort(children.begin(), children.end(), cmp);
    }    
    
    visited.assign(n, 0);
    
    visited[first] = 1;
    ans.push_back(first);
    dfs(first);
    
    for (const auto& person : ans) {
        if (include(person)) {
            cout << persons[person].name << endl;
        }
    }
}