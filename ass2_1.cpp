#include <iostream>
#include <queue>
#include <fstream>
#include <algorithm>

using namespace std;

class Process{
public:
    string name;
    int arrivalTime;
    int burst;
    int priority;
};

vector<Process> readFile(string filename, int& quanTum){
    ifstream fi(filename);
    int nProcess;
    fi >> nProcess >> quanTum;
    vector<Process> p(nProcess);
    for(int i = 0; i < nProcess; ++i){
        fi >> p[i].name;
        fi >> p[(p[i].name[p[i].name.length() - 1] - '0') - 1].arrivalTime >> p[(p[i].name[p[i].name.length() - 1] - '0') - 1].burst >> p[(p[i].name[p[i].name.length() - 1] - '0') - 1].priority;
    }
    return p;
}

void FCFS(vector<Process> p){
    queue<Process> q;
    sort(p.begin(), p.end(), [&](const Process& a, const Process& b){return a.arrivalTime < b.arrivalTime;});
    for(int i = 0; i < p.size(); ++i)
        cout << p[i].name << '\n';
    for(auto pro : p)
        q.push(pro);

}

void SJF(vector<Process> p) {
    int totalTime = 0;
    sort(p.begin(), p.end(), [](const Process& a, const Process& b){ return a.burst < b.burst; });
    vector<int> TT(p.size()), WT(p.size());
    cout << "Scheduling chart: 0";
    for (int i = 0; i < p.size(); i++) {
        
    }
    // 0 ~P1~ 4 ~P2~ 8 ~P3~ 11 ~P1~ 15 ~P2~ 16 ~P1~ 32
}

void PreemptivePriority(vector<Process> p) {

}

int main(){

    int q;
    vector<Process> p = readFile("Input.txt", q);
    
    cout << q << '\n';
    
    FCFS(p);

    return 0;
}
