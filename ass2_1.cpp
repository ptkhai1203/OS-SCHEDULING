#include <iostream>
#include <queue>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <sstream>
using namespace std;

class Process {
public:
    string name;
    double arrivalTime;
    double burst;
    int priority;

public:
    Process(): name(""), arrivalTime(0), burst(0), priority(0) {}
	bool operator()(Process const& a, Process const& b) {
		return a.burst > b.burst;
	}
};

vector<Process> readFile(string filename, double& quantum){
    ifstream fi(filename);
    int nProcess;
    fi >> nProcess >> quantum;
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
    map<string, pair<double, double>> t;
    double time = 0;
   
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
    double totalTT = 0;
    double totalWT = 0;
    for(auto m : t){
        totalTT += m.second.first;
        totalWT += m.second.second;
        cout << m.first << ": \t TT = " << m.second.first << ' ' << "WT = " << m.second.second << '\n';
    }
    cout << "Average:\t TT = " << totalTT / p.size() << "\tWT = " << totalWT / p.size();
	fclose(stdout);
}

void SRTN(vector<Process> p){
    vector<Process> pp = p;
    priority_queue<Process, vector<Process>, Process> pq;
    sort(p.begin(), p.end(), [&](const Process& a, const Process& b){return a.arrivalTime > b.arrivalTime;});
    vector<string> chart;
    map<string, double> TT;
    map<string, double> WT;
    Process cur = p.back();
    p.pop_back();
    double time = 0;
    chart.push_back(to_string(time));
    while(!pq.empty() || p.size() != 0 || cur.burst > 0){
        if(!pq.empty()){
            if(pq.top().burst < cur.burst){
                if(cur.burst > 0)
                    pq.push(cur);
                chart.push_back(cur.name);
                chart.push_back(to_string(time));
                cur = pq.top();
                pq.pop();
            }
        }
        cur.burst--;
        ++time;
        if(cur.burst == 0){
            TT[cur.name] = time - cur.arrivalTime;
            chart.push_back(cur.name);
            chart.push_back(to_string(time));
            if(!pq.empty()){
                cur = pq.top();
                pq.pop();
            }
        } 
        if(time == p.back().arrivalTime){
            pq.push(p.back());
            p.pop_back();
        }
    } 
    double totalTT = 0;
    double totalWT = 0;
    for(auto _p : pp){
        WT[_p.name] = TT[_p.name] - _p.burst;
        totalTT += TT[_p.name];
        totalWT += WT[_p.name];
    }
    freopen("SRTN.txt", "w", stdout);
    cout << "Scheduling chart: 0";
    for(int i = 1; i < chart.size(); ++i)
        cout << "~" << chart[i];
    cout << '\n';
    for(auto m : TT){
        cout << m.first << ": \t TT = "  << m.second << " WT = " << WT[m.first] << '\n';
    }
    cout << "Average: \t TT = " << totalTT / pp.size() << "\t WT = " << totalWT / pp.size();
	fclose(stdout);
}

void SJF(vector<Process> p) {
    ofstream out("SJF.txt");
    double totalTime = 0, totalTT = 0, totalWT = 0;
	int count = p.size();
    sort(p.begin(), p.end(), [](const Process& a, const Process& b){ return a.arrivalTime < b.arrivalTime; });
    vector<Process> temp;
    temp.push_back(p[0]);
    p.erase(p.begin());
    map<string, double> TT, WT;
    out << "Scheduling chart: 0";
    while (p.size() || temp.size()) {
        totalTime += temp[0].burst;
        TT.insert({temp[0].name, totalTime});
        WT.insert({temp[0].name, totalTime - temp[0].burst});
        out << " ~" << temp[0].name << "~ " << totalTime;
        temp.erase(temp.begin());
        for (int i = 0; i < p.size(); i++) {
            if (totalTime >= p[i].arrivalTime) {
                temp.push_back(p[i]);
                p.erase(p.begin() + i);
                i--;
            }
        }
        sort(temp.begin(), temp.end(), [](const Process& a, const Process& b){ return a.burst < b.burst; });
    }
    out << endl;
    for (auto x : TT) {
        totalTT += x.second;
        totalWT += WT[x.first];
        out << x.first << ":\t\tTT = " << x.second << "\tWT = " << WT[x.first] << endl; 
    }
    out << "Average:\t\t\tTT = " << totalTT / count << "\t\tWT = " << totalWT / count;
    out.close();
}

void PreemptivePriority(vector<Process> p) {
	priority_queue<int> process;
	vector<string> processName;
	vector<double> arrivalTime;
	vector<double> cpuBurst;
	vector<int> priority;
	for (int i = 0; i < (int)p.size(); i++) {
		processName.push_back(p[i].name);
		arrivalTime.push_back(p[i].arrivalTime);
		cpuBurst.push_back(p[i].burst);
		priority.push_back(p[i].priority * -1);
	}
	vector<double> oldBurst = cpuBurst;
	vector<double> oldArrival = arrivalTime;
	double currentTime = 0;
	double* WT = new double[p.size()]();
	double* TT = new double[p.size()]();
	string oldProcess = "";
	stringstream ss;
	double sumTime = 0;
	for (int i = 0; i < (int)p.size(); i++) {
		sumTime += cpuBurst[i];
	}
	while (currentTime <= sumTime) {
		int indexProcess = -1;
		while (true) {
			auto push = find(arrivalTime.begin(), arrivalTime.end(), currentTime);
			if (push != arrivalTime.end()) {
				int temp = distance(arrivalTime.begin(), push);
				arrivalTime[temp] = -1;
				process.push(priority[temp]);
			}
			else
				break;
		}
		if (process.size() == 0) {
			currentTime++;
			continue;
		}
		int ProcessInProcess = process.top();
		process.pop();
		auto it = find(priority.begin(), priority.end(), ProcessInProcess);
		if (it != priority.end())
			indexProcess = distance(priority.begin(), it);
		string Name = processName[indexProcess];
		if (oldProcess != Name) {
			ss << currentTime << " ~ " << Name << " ~ ";
			oldProcess = Name;
		}
		currentTime += 1;
		cpuBurst[indexProcess] -= 1;
		if (cpuBurst[indexProcess] == 0) {
			priority[indexProcess] = INT_MAX;
			TT[indexProcess] = currentTime - oldArrival[indexProcess];
			WT[indexProcess] = TT[indexProcess] - oldBurst[indexProcess];
		}
		else {
			process.push(priority[indexProcess]);
		}
		if (currentTime == sumTime)
			ss << currentTime;
	}
	double AVG_WT = 0;
	double AVG_TT = 0;
	for (int i = 0; i < (int)p.size(); i++) {
		AVG_WT += WT[i];
		AVG_TT += TT[i];
	}
	AVG_WT /= p.size();
	AVG_TT /= p.size();
	string chart = ss.str();
	ofstream out;
	out.open("Priority (Preemptive).txt", ios::out);
	out << "Scheduling chart: " << endl;
	out << "\t" << chart << endl;
	for (int i = 0; i < (int)p.size(); i++) {
		out << processName[i] << ":\t" << "TT = " << TT[i] << " WT = " << WT[i] << endl;
	}
	out << "Average:	TT = " << AVG_TT << " WT = " << AVG_WT;
	out.close();
}

void RR(vector<Process> p, int quantum){
	stringstream ss;
	queue<string> process;
	vector<string> processName;
	vector<double> arrivalTime;
	vector<double> cpuBurst;
	for (int i = 0; i < (int)p.size(); i++) {
		processName.push_back(p[i].name);
		arrivalTime.push_back(p[i].arrivalTime);
		cpuBurst.push_back(p[i].burst);
	}
	vector<double> oldBurst = cpuBurst;
	vector<double> oldArrival = arrivalTime;
	int currentTime = 0;
	double* WT = new double[p.size()]();
	double* TT = new double[p.size()]();
	string oldProcess = "";
	double sumTime = 0;
	for (int i = 0; i < (int)p.size(); i++) {
		sumTime += cpuBurst[i];
	}
	while (true) {
		auto start = find(arrivalTime.begin(), arrivalTime.end(), 0);
		if (start == arrivalTime.end())
			break;
		arrivalTime[distance(arrivalTime.begin(), start)] = -1;
		process.push(processName[distance(arrivalTime.begin(), start)]);
	}
	while (currentTime <= sumTime) {
		int indexProcess = -1;
		for (int i = currentTime + 1; i <= currentTime + quantum; i++) {
			bool flag = true;
			while (flag) {
				auto it = find(arrivalTime.begin(), arrivalTime.end(), i);
				if (it != arrivalTime.end()) {
					int temp = distance(arrivalTime.begin(), it);
					arrivalTime[temp] = -1;
					process.push(processName[temp]);
				}
				else
					flag = false;
			}
		}
		if (process.size() == 0) {
			currentTime++;
			continue;
		}
		string ProcessInProcess = process.front();
		process.pop();
		auto it = find(processName.begin(), processName.end(), ProcessInProcess);
		if (it != processName.end())
			indexProcess = distance(processName.begin(), it);
		if (oldProcess != ProcessInProcess) {
			ss << currentTime << " ~ " << ProcessInProcess << " ~ ";
			oldProcess = ProcessInProcess;
		}
		if (cpuBurst[indexProcess] > quantum) {
			currentTime += quantum;
			cpuBurst[indexProcess] -= quantum;
		}
		else {
			currentTime += cpuBurst[indexProcess];
			cpuBurst[indexProcess] = 0;
			TT[indexProcess] = currentTime - oldArrival[indexProcess];
			WT[indexProcess] = TT[indexProcess] - oldBurst[indexProcess];
		}
		if (cpuBurst[indexProcess] > 0) {
			process.push(ProcessInProcess);
		}
		if (currentTime == sumTime)
			ss << currentTime;
	}
	double AVG_WT = 0;
	double AVG_TT = 0;
	for (int i = 0; i < (int)p.size(); i++) {
		AVG_WT += WT[i];
		AVG_TT += TT[i];
	}
	AVG_WT /= p.size();
	AVG_TT /= p.size();
	string chart = ss.str();
	ofstream out;
	out.open("RR.txt", ios::out);
	out << "Scheduling chart: " << endl;
	out << "\t" << chart << endl;
	for (int i = 0; i < (int)p.size(); i++) {
		out << processName[i] << ":\t" << "TT = " << TT[i] << " WT = " << WT[i] << endl;
	}
	out << "Average:	TT = " << AVG_TT << " WT = " << AVG_WT;
	out.close();
}

void NonpreemptivePriority(vector<Process> p) {
    ofstream out("Priority (Nonpreemptive).txt");
    double totalTime = 0, totalTT = 0, totalWT = 0;
	int count = p.size();
    sort(p.begin(), p.end(), [](const Process& a, const Process& b){ return a.arrivalTime < b.arrivalTime; });
    vector<Process> temp;
    temp.push_back(p[0]);
    p.erase(p.begin());
    map<string, double> TT, WT;
    out << "Scheduling chart: 0";
    while (p.size() || temp.size()) {
        totalTime += temp[0].burst;
        TT.insert({temp[0].name, totalTime});
        WT.insert({temp[0].name, totalTime - temp[0].burst});
        out << " ~" << temp[0].name << "~ " << totalTime;
        temp.erase(temp.begin());
        for (int i = 0; i < p.size(); i++) {
            if (totalTime >= p[i].arrivalTime) {
                temp.push_back(p[i]);
                p.erase(p.begin() + i);
                i--;
            }
        }
        sort(temp.begin(), temp.end(), [](const Process& a, const Process& b){ return a.priority < b.priority; });
    }
    out << endl;
    for (auto x : TT) {
        totalTT += x.second;
        totalWT += WT[x.first];
        out << x.first << ":\t\tTT = " << x.second << "\tWT = " << WT[x.first] << endl; 
    }
    out << "Average:\t\t\tTT = " << totalTT / count << "\t\tWT = " << totalWT / count;
    out.close();
}

typedef void (*schedule_ptr)(vector<Process>);

schedule_ptr schedule_methods[] = {
    FCFS, SRTN, SJF, PreemptivePriority, NonpreemptivePriority
};

int main(){

    double q;
    vector<Process> p = readFile("Input.txt", q);
    RR(p,q);
    PreemptivePriority(p);
    
    for(int i = 0; i < 5; ++i)
        schedule_methods[i](p);

    return 0;
}
