#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>

using namespace std;

// functions for sorting
static bool sortByArrival(const vector<int>& a, const vector<int>& b) {
    // tie-breaker :- process id
    if(a[1] == b[1]) return a[0] < b[0];
    return a[1] < b[1];
}

static bool sortByBurstTime(const vector<int>& a, const vector<int>& b) {
    // tie-breaker :- FCFS
    if(a[2] == b[2]) return sortByArrival(a, b);
    else return a[2] < b[2];
}

static bool sortByPriority(const vector<int>& a, const vector<int>& b) {
    // tie-breaker :- FCFS (higher number = higher priority)
    if(a[3] == b[3]) return sortByArrival(a, b);
    else return a[3] > b[3];
}

// forward declarations for the functions
void shortestJobFirstPreemptive();
void prioritySchedulingPreemptive();
void roundRobin();

void priorityScheduling() {
    int n;
    vector<int> arrTime;
    vector<int> burstTime;
    vector<int> priority;

    cout << "Enter number of processes:";
    cin >> n;
    cout << "Enter arrival time of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int time;
        cin >> time;
        arrTime.push_back(time);
    }
    cout << "Enter burst time of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int time;
        cin >> time;
        burstTime.push_back(time);
    }
    cout << "Enter priority of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int pr;
        cin >> pr;
        priority.push_back(pr);
    }

    // combining details into one vector
    vector<vector<int>> detail;
    for(int i = 0; i < n; i++) {
        detail.push_back({i, arrTime[i], burstTime[i], priority[i]});
    }
    // sorting it by priority
    sort(detail.begin(), detail.end(), sortByPriority);
    
    vector<int> gantt;
    vector<int> ct(n);
    vector<int> tat(n);
    vector<int> wt(n);
    vector<int> resTime(n);
    int time = 0;
    int total_wt = 0, total_tat = 0;

    // calculating all the values
    for(int i = 0; i < n; i++) {
        int process = detail[i][0];
        int at = detail[i][1];
        int bt = detail[i][2];

        // handling case for an gap between processes
        if(time < at) {
            time = at;
        }

        // pushing to gantt chart
        gantt.push_back(process);

        // calculating all values
        resTime[process] = time - at;
        time += bt;
        ct[process] = time;
        tat[process] = ct[process] - at;
        wt[process] = tat[process] - bt;
        total_tat += tat[process];
        total_wt += wt[process];
    }

    // printing gantt chart
    cout << "\nGantt Chart :-" << endl;
    for (int i = 0; i < n; i++) {
        cout << "-------";
    }
    cout << endl;

    for (int i = 0; i < n; i++) {
        cout << "|  P" << detail[i][0] << "   ";
    }
    cout << "|" << endl;

    for (int i = 0; i < n; i++) {
        cout << "-------";
    }
    cout << endl;
    cout << 0;
    for (int i = 0; i < n; i++) {
        int completion = ct[detail[i][0]];
        // here we are giving less space for a double digit number to make a balance
        if (completion > 9) {
            cout << "      " << completion;
        }
        else {
            cout << "       " << completion;
        }
    }
    cout << endl << endl;

    // printing values in table form
    cout << "Priority (Non-Preemptive) Results:" << endl;
    cout << "Process\t\tArrival\t\tBurst\t\tPriority\tCompletion\tTurnaround\tWaiting" << endl;
    for (int i = 0; i < n; i++) {
        cout << "P" << i << "\t\t";
        cout << arrTime[i] << "\t\t";
        cout << burstTime[i] << "\t\t";
        cout << priority[i] << "\t\t";
        cout << ct[i] << "\t\t";
        cout << tat[i] << "\t\t";
        cout << wt[i] << endl;
    }
    cout << endl;

    double avg_tat = double(total_tat) / n;
    double avg_wt = double(total_wt) / n;
    cout << "Avg. turn around time: " << avg_tat << endl;
    cout << "Avg. waiting time: " << avg_wt << endl;
}

void shortestJobFirst() {
    int n;
    vector<int> arrTime;
    vector<int> burstTime;

    cout << "Enter number of processes:";
    cin >> n;
    cout << "Enter arrival time of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int time;
        cin >> time;
        arrTime.push_back(time);
    }
    cout << "Enter burst time of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int time;
        cin >> time;
        burstTime.push_back(time);
    }

    // combining both arrival time and burst time in one array
    vector<vector<int>> detail;
    for(int i = 0; i < n; i++) {
        detail.push_back({i, arrTime[i], burstTime[i]});
    }
    // sorting it by burst time
    sort(detail.begin(), detail.end(), sortByBurstTime);

    vector<int> gantt;
    vector<int> ct(n);
    vector<int> tat(n);
    vector<int> wt(n);
    vector<int> resTime(n);
    int time = 0;
    int total_wt = 0, total_tat = 0;

    // calculating all the values
    for(int i = 0; i < n; i++) {
        int process = detail[i][0];
        int at = detail[i][1];
        int bt = detail[i][2];

        // handling case for an gap between processes
        if(time < at) {
            time = at;
        }

        // pushing to gantt chart
        gantt.push_back(process);

        // calculating all values
        resTime[process] = time - at;
        time += bt;
        ct[process] = time;
        tat[process] = ct[process] - at;
        wt[process] = tat[process] - bt;
        total_tat += tat[process];
        total_wt += wt[process];
    }

    // printing gantt chart
    cout << "\nGantt Chart :-" << endl;
    for (int i = 0; i < n; i++) {
        cout << "-------";
    }
    cout << endl;

    for (int i = 0; i < n; i++) {
        cout << "|  P" << detail[i][0] << "   ";
    }
    cout << "|" << endl;

    for (int i = 0; i < n; i++) {
        cout << "-------";
    }
    cout << endl;
    cout << 0;
    for (int i = 0; i < n; i++) {
        int completion = ct[detail[i][0]];
        // here we are giving less space for a double digit number to make a balance
        if (completion > 9) {
            cout << "      " << completion;
        }
        else {
            cout << "       " << completion;
        }
    }
    cout << endl << endl;

    // printing values in table form
    cout << "SJF (Non-Preemptive) Results:" << endl;
    cout << "Process\t\tArrival\t\tBurst\t\tCompletion\tTurnaround\tWaiting" << endl;
    for (int i = 0; i < n; i++) {
        cout << "P" << i << "\t\t";
        cout << arrTime[i] << "\t\t";
        cout << burstTime[i] << "\t\t";
        cout << ct[i] << "\t\t";
        cout << tat[i] << "\t\t";
        cout << wt[i] << endl;
    }
    cout << endl;

    double avg_tat = double(total_tat) / n;
    double avg_wt = double(total_wt) / n;
    cout << "Avg. turn around time: " << avg_tat << endl;
    cout << "Avg. waiting time: " << avg_wt << endl;
}

void fcfs() {
    int n;
    vector<int> arrTime;
    vector<int> burstTime;

    cout << "Enter number of processes:";
    cin >> n;
    cout << "Enter arrival time of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int time;
        cin >> time;
        arrTime.push_back(time);
    }
    cout << "Enter burst time of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int time;
        cin >> time;
        burstTime.push_back(time);
    }

    // combining both arrival time and burst time in one array
    vector<vector<int>> detail;
    for(int i = 0; i < n; i++) {
        detail.push_back({i, arrTime[i], burstTime[i]});
    }
    // sorting it by arrival time
    sort(detail.begin(), detail.end(), sortByArrival);

    vector<int> gantt;
    vector<int> ct(n);
    vector<int> tat(n);
    vector<int> wt(n);
    vector<int> resTime(n);
    int time = 0;
    int total_wt = 0, total_tat = 0;

    // calculating all the values
    for(int i = 0; i < n; i++) {
        int process = detail[i][0];
        int at = detail[i][1];
        int bt = detail[i][2];

        // handling case for an gap between processes
        if(time < at) {
            time = at;
        }

        // pushing to gantt chart
        gantt.push_back(process);

        // calculating all values
        resTime[process] = time - at;
        time += bt;
        ct[process] = time;
        tat[process] = ct[process] - at;
        wt[process] = tat[process] - bt;
        total_tat += tat[process];
        total_wt += wt[process];
    }

    // printing gantt chart
    cout << "\nGantt Chart :-" << endl;
    for (int i = 0; i < n; i++) {
        cout << "-------";
    }
    cout << endl;

    for (int i = 0; i < n; i++) {
        cout << "|  P" << detail[i][0] << "   ";
    }
    cout << "|" << endl;

    for (int i = 0; i < n; i++) {
        cout << "-------";
    }
    cout << endl;
    cout << 0;
    for (int i = 0; i < n; i++) {
        int completion = ct[detail[i][0]];
        // here we are giving less space for a double digit number to make a balance
        if (completion > 9) {
            cout << "      " << completion;
        }
        else {
            cout << "       " << completion;
        }
    }
    cout << endl << endl;

    // printing values in table form
    cout << "FCFS Results:" << endl;
    cout << "Process\t\tArrival\t\tBurst\t\tCompletion\tTurnaround\tWaiting" << endl;
    for (int i = 0; i < n; i++) {
        cout << "P" << i << "\t\t";
        cout << arrTime[i] << "\t\t";
        cout << burstTime[i] << "\t\t";
        cout << ct[i] << "\t\t";
        cout << tat[i] << "\t\t";
        cout << wt[i] << endl;
    }
    cout << endl;

    double avg_tat = double(total_tat) / n;
    double avg_wt = double(total_wt) / n;
    cout << "Avg. turn around time: " << avg_tat << endl;
    cout << "Avg. waiting time: " << avg_wt << endl;
}

void shortestJobFirstPreemptive() {
    int n;
    vector<int> arrTime;
    vector<int> burstTime;

    cout << "Enter number of processes:";
    cin >> n;
    cout << "Enter arrival time of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int time;
        cin >> time;
        arrTime.push_back(time);
    }
    cout << "Enter burst time of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int time;
        cin >> time;
        burstTime.push_back(time);
    }

    vector<int> remainingBT = burstTime;
    vector<int> ct(n);
    vector<int> tat(n);
    vector<int> wt(n);
    vector<int> resTime(n, -1);

    vector<pair<int, int>> gantt; // {process_id, start_time}
    int currentTime = 0;
    int completed = 0;
    int total_wt = 0, total_tat = 0;

    while(completed != n) {
        int shortestJob = -1;
        int minBurst = 1e9; // a big number

        for(int i = 0; i < n; i++) {
            if(arrTime[i] <= currentTime && remainingBT[i] > 0) {
                if(remainingBT[i] < minBurst) {
                    minBurst = remainingBT[i];
                    shortestJob = i;
                }
                else if(remainingBT[i] == minBurst) {
                    if(arrTime[i] < arrTime[shortestJob]) {
                        shortestJob = i;
                    }
                }
            }
        }

        if(shortestJob == -1) {
            currentTime++;
        } else {
            if(resTime[shortestJob] == -1) {
                resTime[shortestJob] = currentTime - arrTime[shortestJob];
            }

            remainingBT[shortestJob]--;
            
            if(gantt.empty() || gantt.back().first != shortestJob) {
                gantt.push_back({shortestJob, currentTime});
            }

            currentTime++;

            if(remainingBT[shortestJob] == 0) {
                completed++;
                ct[shortestJob] = currentTime;
                tat[shortestJob] = ct[shortestJob] - arrTime[shortestJob];
                wt[shortestJob] = tat[shortestJob] - burstTime[shortestJob];
                total_tat += tat[shortestJob];
                total_wt += wt[shortestJob];
            }
        }
    }
    
    gantt.push_back({-1, currentTime});

    // printing gantt chart
    cout << "\nGantt Chart :-" << endl;
    size_t gantt_size = gantt.size() - 1;
    for (size_t i = 0; i < gantt_size; i++) {
        cout << "-------";
    }
    cout << endl;

    for (size_t i = 0; i < gantt_size; i++) {
        cout << "|  P" << gantt[i].first << "   ";
    }
    cout << "|" << endl;

    for (size_t i = 0; i < gantt_size; i++) {
        cout << "-------";
    }
    cout << endl;
    
    // handles case where chart is empty
    if(!gantt.empty()){
      cout << gantt[0].second;
    }
    
    for (size_t i = 0; i < gantt_size; i++) {
        int completion = gantt[i+1].second;
        if (completion > 9) {
            cout << "      " << completion;
        } else {
            cout << "       " << completion;
        }
    }
    cout << endl << endl;

    // printing values in table form
    cout << "SJF (Preemptive) Results:" << endl;
    cout << "Process\t\tArrival\t\tBurst\t\tCompletion\tTurnaround\tWaiting" << endl;
    for (int i = 0; i < n; i++) {
        cout << "P" << i << "\t\t";
        cout << arrTime[i] << "\t\t";
        cout << burstTime[i] << "\t\t";
        cout << ct[i] << "\t\t";
        cout << tat[i] << "\t\t";
        cout << wt[i] << endl;
    }
    cout << endl;

    double avg_tat = double(total_tat) / n;
    double avg_wt = double(total_wt) / n;
    cout << "Avg. turn around time: " << avg_tat << endl;
    cout << "Avg. waiting time: " << avg_wt << endl;
}

void prioritySchedulingPreemptive() {
    int n;
    vector<int> arrTime;
    vector<int> burstTime;
    vector<int> priority;

    cout << "Enter number of processes:";
    cin >> n;
    cout << "Enter arrival time of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int time;
        cin >> time;
        arrTime.push_back(time);
    }
    cout << "Enter burst time of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int time;
        cin >> time;
        burstTime.push_back(time);
    }
    cout << "Enter priority of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int pr;
        cin >> pr;
        priority.push_back(pr);
    }

    vector<int> remainingBT = burstTime;
    vector<int> ct(n);
    vector<int> tat(n);
    vector<int> wt(n);
    vector<int> resTime(n, -1);

    vector<pair<int, int>> gantt;
    int currentTime = 0;
    int completed = 0;
    int total_wt = 0, total_tat = 0;

    while(completed != n) {
        int highestPriorityJob = -1;
        int maxPriority = -1;

        for(int i = 0; i < n; i++) {
            if(arrTime[i] <= currentTime && remainingBT[i] > 0) {
                if(priority[i] > maxPriority) {
                    maxPriority = priority[i];
                    highestPriorityJob = i;
                }
                else if(priority[i] == maxPriority) {
                    if(arrTime[i] < arrTime[highestPriorityJob]) {
                        highestPriorityJob = i;
                    }
                }
            }
        }

        if(highestPriorityJob == -1) {
            currentTime++;
        } else {
            if(resTime[highestPriorityJob] == -1) {
                resTime[highestPriorityJob] = currentTime - arrTime[highestPriorityJob];
            }

            remainingBT[highestPriorityJob]--;
            
            if(gantt.empty() || gantt.back().first != highestPriorityJob) {
                gantt.push_back({highestPriorityJob, currentTime});
            }

            currentTime++;

            if(remainingBT[highestPriorityJob] == 0) {
                completed++;
                ct[highestPriorityJob] = currentTime;
                tat[highestPriorityJob] = ct[highestPriorityJob] - arrTime[highestPriorityJob];
                wt[highestPriorityJob] = tat[highestPriorityJob] - burstTime[highestPriorityJob];
                total_tat += tat[highestPriorityJob];
                total_wt += wt[highestPriorityJob];
            }
        }
    }
    
    gantt.push_back({-1, currentTime});

    // printing gantt chart
    cout << "\nGantt Chart :-" << endl;
    size_t gantt_size = gantt.size() - 1;
    for (size_t i = 0; i < gantt_size; i++) {
        cout << "-------";
    }
    cout << endl;

    for (size_t i = 0; i < gantt_size; i++) {
        cout << "|  P" << gantt[i].first << "   ";
    }
    cout << "|" << endl;

    for (size_t i = 0; i < gantt_size; i++) {
        cout << "-------";
    }
    cout << endl;
    
    if(!gantt.empty()){
      cout << gantt[0].second;
    }
    
    for (size_t i = 0; i < gantt_size; i++) {
        int completion = gantt[i+1].second;
        if (completion > 9) {
            cout << "      " << completion;
        } else {
            cout << "       " << completion;
        }
    }
    cout << endl << endl;

    // printing values in table form
    cout << "Priority (Preemptive) Results:" << endl;
    cout << "Process\t\tArrival\t\tBurst\t\tPriority\tCompletion\tTurnaround\tWaiting" << endl;
    for (int i = 0; i < n; i++) {
        cout << "P" << i << "\t\t";
        cout << arrTime[i] << "\t\t";
        cout << burstTime[i] << "\t\t";
        cout << priority[i] << "\t\t";
        cout << ct[i] << "\t\t";
        cout << tat[i] << "\t\t";
        cout << wt[i] << endl;
    }
    cout << endl;

    double avg_tat = double(total_tat) / n;
    double avg_wt = double(total_wt) / n;
    cout << "Avg. turn around time: " << avg_tat << endl;
    cout << "Avg. waiting time: " << avg_wt << endl;
}

void roundRobin() {
    int n;
    vector<int> arrTime;
    vector<int> burstTime;
    int quantum;

    cout << "Enter number of processes:";
    cin >> n;
    cout << "Enter arrival time of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int time;
        cin >> time;
        arrTime.push_back(time);
    }
    cout << "Enter burst time of all processes:" << endl;
    for(int i = 0; i < n; i++) {
        int time;
        cin >> time;
        burstTime.push_back(time);
    }
    cout << "Enter time quantum:";
    cin >> quantum;

    vector<vector<int>> processes;
    for(int i = 0; i < n; i++) {
        processes.push_back({arrTime[i], burstTime[i], i});
    }
    sort(processes.begin(), processes.end());

    queue<int> readyQueue;
    vector<int> remainingBT = burstTime;
    vector<int> ct(n);
    vector<int> tat(n);
    vector<int> wt(n);
    vector<int> resTime(n, -1);
    
    int currentTime = 0;
    int completed = 0;
    int process_ptr = 0;
    int total_wt = 0, total_tat = 0;
    vector<pair<int, int>> gantt;
    
    while(completed < n) {
        while(process_ptr < n && processes[process_ptr][0] <= currentTime) {
            readyQueue.push(processes[process_ptr][2]);
            process_ptr++;
        }
        
        if(readyQueue.empty()) {
            if (process_ptr < n) {
                 if(gantt.empty() || gantt.back().first != -2) {
                     gantt.push_back({-2, currentTime});
                 }
                currentTime = processes[process_ptr][0];
            } else {
                // all processes are done
                break;
            }
            continue;
        }
        
        int pid = readyQueue.front();
        readyQueue.pop();
        
        if (!gantt.empty() && gantt.back().first == -2) {
            gantt.back() = {pid, gantt.back().second};
        } else if (gantt.empty() || gantt.back().first != pid) {
             gantt.push_back({pid, currentTime});
        }
        
        if(resTime[pid] == -1) {
            resTime[pid] = currentTime - arrTime[pid];
        }
        
        int exec_time = min(quantum, remainingBT[pid]);
        currentTime += exec_time;
        remainingBT[pid] -= exec_time;
        
        while(process_ptr < n && processes[process_ptr][0] <= currentTime) {
            readyQueue.push(processes[process_ptr][2]);
            process_ptr++;
        }
        
        if(remainingBT[pid] > 0) {
            readyQueue.push(pid);
        } else {
            completed++;
            ct[pid] = currentTime;
            tat[pid] = ct[pid] - arrTime[pid];
            wt[pid] = tat[pid] - burstTime[pid];
            total_tat += tat[pid];
            total_wt += wt[pid];
        }
    }
    
    gantt.push_back({-1, currentTime});
    
    // printing gantt chart
    cout << "\nGantt Chart :-" << endl;
    size_t gantt_size = gantt.size() - 1;
    for (size_t i = 0; i < gantt_size; i++) {
        cout << "-------";
    }
    cout << endl;

    for (size_t i = 0; i < gantt_size; i++) {
        if(gantt[i].first == -2) cout << "| IDLE  ";
        else cout << "|  P" << gantt[i].first << "   ";
    }
    cout << "|" << endl;

    for (size_t i = 0; i < gantt_size; i++) {
        cout << "-------";
    }
    cout << endl;
    
    if(!gantt.empty()){
      cout << gantt[0].second;
    }

    for (size_t i = 0; i < gantt_size; i++) {
        int completion = gantt[i+1].second;
        if (completion > 9) {
            cout << "      " << completion;
        } else {
            cout << "       " << completion;
        }
    }
    cout << endl << endl;

    // printing values in table form
    cout << "Round Robin Results:" << endl;
    cout << "Process\t\tArrival\t\tBurst\t\tCompletion\tTurnaround\tWaiting" << endl;
    for (int i = 0; i < n; i++) {
        cout << "P" << i << "\t\t";
        cout << arrTime[i] << "\t\t";
        cout << burstTime[i] << "\t\t";
        cout << ct[i] << "\t\t";
        cout << tat[i] << "\t\t";
        cout << wt[i] << endl;
    }
    cout << endl;

    double avg_tat = double(total_tat) / n;
    double avg_wt = double(total_wt) / n;
    cout << "Avg. turn around time: " << avg_tat << endl;
    cout << "Avg. waiting time: " << avg_wt << endl;
}

int main() {
    int choice;
    cout << "Choose an Algorithm to perform:" << endl;
    cout << "1. First Come First Serve(FCFS):" << endl;
    cout << "2. Shortest Job First(non-preemptive):" << endl;
    cout << "3. Shortest Job First(preemptive):" << endl;
    cout << "4. Priority Based Scheduling(non-preemptive):" << endl;
    cout << "5. Priority Based Scheduling(preemptive):" << endl;
    cout << "6. Round Robin:" << endl;
    cin >> choice;
    switch (choice) {
        case 1:
            fcfs();
        break;
        case 2:
            shortestJobFirst();
        break;
        case 3:
            shortestJobFirstPreemptive();
        break;
        case 4:
            priorityScheduling();
        break;
        case 5:
            prioritySchedulingPreemptive();
        break;
        case 6:
            roundRobin();
        break;
    }

    return 0;
}