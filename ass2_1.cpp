#include <iostream>
#include <queue>
#include <fstream>
#include <algorithm>
#include <map>

using namespace std;

class Process{
public:
    string name;
    int arrivalTime;
    int burst;
    int priority;

public:
    Process() : name(""), arrivalTime(0), burst(0), priority(0) {}
    bool operator<(const Process& a, const Process& b){
        return a.burst < b.burst;
    }
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
    vector<string> chart; 
    sort(p.begin(), p.end(), [&](const Process& a, const Process& b){return a.arrivalTime < b.arrivalTime;});
    map<string, pair<int, int>> t;
    int time = 0;
   
    for(int i = 0; i < p.size(); ++i){
        chart.push_back(to_string(time));
        chart.push_back(p[i].name);
        time += p[i].burst; 
        t[p[i].name].first = time - p[i].arrivalTime;
        t[p[i].name].second = t[p[i].name].first - p[i].burst;
    }
    chart.push_back(to_string(time));
    freopen("FCFS.txt", "w", stdout);
    cout << "Scheduling chart: ";
    cout << chart[0];
    for(int i = 1; i < chart.size(); ++i)
        cout << "~" << chart[i];
    cout << '\n';
    int totalTT = 0;
    int totalWT = 0;
    for(auto m : t){
        totalTT += m.second.first;
        totalWT += m.second.second;
        cout << m.first << ":\t TT = " << m.second.first << ' ' << "WT = " << m.second.second << '\n';
    }
    cout << "Average:\t TT = " << 1.0 * totalTT / p.size() << "\tWT = " << 1.0 * totalWT / p.size();
}

void SRTN(vector<Process> p){
    priority_queue<Process, vector<Process>> pq;
    sort(p.begin(), p.end(), [&](const Process& a, const Process& b){return a.arrivalTime > b.arrivalTime;});
    vector<string> chart;
    map<string, pair<int, int>> t;
    Process cur;
    int time = 0;
    for(int i = 0; ;++i){
        chart.push_back(to_string(time));
        if(i == p.back().arrivalTime){
            pq.push(p.back());
            p.pop_back();
        }
        if(p.size() == 0 && pq.empty())
            break;
        Process nxt;
        if(!pq.empty()){
            nxt = pq.top();
            pq.pop();
        }
        if(cur.name == "" || cur.burst > nxt.burst)
            cur = nxt;
        chart.push_back(cur.name);
    }
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
