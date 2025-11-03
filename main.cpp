#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

// functions for sorting
bool sortByArrival(const vector<int>& a, const vector<int>& b) {
    // tie-breaker :- process id
    if(a[1] == b[1]) return a[0] < b[0];
    return a[1] < b[1];
}

bool sortByBurstTime(const vector<int>& a, const vector<int>& b) {
    // tie-breaker :- FCFS
    if(a[2] == b[2]) return sortByArrival(a, b);
    else return a[2] < b[2];
}

bool sortByPriority(const vector<int>& a, const vector<int>& b) {
    // tie-breaker :- FCFS (higher number = higher priority)
    if(a[3] == b[3]) return sortByArrival(a, b);
    else return a[3] > b[3];
}

void priorityScheduling() {
    int n;
    vector<int> arrTime;
    vector<int> burstTime;
    vector<int> priority;

    cout << "Enter number of processes:";
    cin >> n;
    if(n <= 0) return;

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

    vector<pair<int, int>> gantt_chart; // {PID, StartTime}
    vector<int> ct(n);
    vector<int> tat(n);
    vector<int> wt(n);
    vector<int> resTime(n);
    vector<int> is_completed(n, 0);
    int time = 0;
    int total_wt = 0, total_tat = 0;
    int completed_count = 0;

    while(completed_count < n) {
        int best_job_idx = -1;
        int max_priority = -1;
        int min_arrival = 1e9;

        for(int i = 0; i < n; i++) {
            if(arrTime[i] <= time && is_completed[i] == 0) {
                if(priority[i] > max_priority) {
                    max_priority = priority[i];
                    best_job_idx = i;
                    min_arrival = arrTime[i];
                }
                else if(priority[i] == max_priority) { // same priority -> tie breaker
                    if(arrTime[i] < min_arrival) {
                        best_job_idx = i;
                        min_arrival = arrTime[i];
                    }
                }
            }
        }

        if(best_job_idx == -1) {
            // no job is ready, CPU is empty
            int next_arrival = 1e9;
            for(int i = 0; i < n; i++) {
                if(is_completed[i] == 0) {
                    next_arrival = min(next_arrival, arrTime[i]);
                }
            }
            gantt_chart.push_back({-1, time});
            time = next_arrival;
        }
        else {
            // A process is selected
            int process = best_job_idx;
            int at = arrTime[process];
            int bt = burstTime[process];

            gantt_chart.push_back({process, time});

            resTime[process] = time - at;
            time += bt;
            ct[process] = time;
            tat[process] = ct[process] - at;
            wt[process] = tat[process] - bt;
            total_tat += tat[process];
            total_wt += wt[process];
            is_completed[process] = 1;
            completed_count++;
        }
    }
    // for end time
    gantt_chart.push_back({-99, time});

    // printing gantt chart
    cout << "\nGantt Chart :-" << endl;
    int gantt_size = gantt_chart.size();
    for(int i = 0; i < gantt_size-1; i++) {
        cout << "--------";
    }
    cout << endl;

    for(int i = 0; i < gantt_size-1; i++) {
        int pid = gantt_chart[i].first;
        if(pid == -1) {
            cout << "| IDLE  ";
        }
        else {
            cout << "|  P" << pid+1 << "   ";
        }
    }
    cout << "|" << endl;

    for(int i = 0; i < gantt_size-1; i++) {
        cout << "--------";
    }
    cout << endl;
    
    if(!gantt_chart.empty()){
        cout << gantt_chart[0].second;
    }
    for(int i = 0; i < gantt_size-1; i++) {
        int completion = gantt_chart[i+1].second;
        // here we are giving less space for a double digit number to make a balance
        if(completion > 9) {
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
    for(int i = 0; i < n; i++) {
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
    if(n <= 0) return;

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

    vector<pair<int, int>> gantt_chart; // {PID, StartTime}
    vector<int> ct(n);
    vector<int> tat(n);
    vector<int> wt(n);
    vector<int> resTime(n);
    vector<int> is_completed(n, 0);
    int time = 0;
    int total_wt = 0, total_tat = 0;
    int completed_count = 0;

    while(completed_count < n) {
        // finding a job with minimum burst time and valid arrival time
        int best_job_idx = -1;
        int min_burst = 1e9;
        int min_arrival = 1e9;

        for(int i = 0; i < n; i++) {
            if(arrTime[i] <= time && is_completed[i] == 0) {
                if(burstTime[i] < min_burst) {
                    min_burst = burstTime[i];
                    best_job_idx = i;
                    min_arrival = arrTime[i];
                }
                else if(burstTime[i] == min_burst) { // tie-breaker -> FCFS
                    if(arrTime[i] < min_arrival) {
                        best_job_idx = i;
                        min_arrival = arrTime[i];
                    }
                }
            }
        }

        if(best_job_idx == -1) {
            // no job is ready, CPU is empty
            int next_arrival = 1e9;
            for(int i = 0; i < n; i++) {
                if(is_completed[i] == 0) {
                    next_arrival = min(next_arrival, arrTime[i]);
                }
            }
            gantt_chart.push_back({-1, time});
            time = next_arrival;
        }
        else {
            // A process is selected
            int process = best_job_idx;
            int at = arrTime[process];
            int bt = burstTime[process];
            
            gantt_chart.push_back({process, time});

            resTime[process] = time - at;
            time += bt;
            ct[process] = time;
            tat[process] = ct[process] - at;
            wt[process] = tat[process] - bt;
            total_tat += tat[process];
            total_wt += wt[process];
            is_completed[process] = 1;
            completed_count++;
        }
    }
    // for end time
    gantt_chart.push_back({-99, time});

    // printing gantt chart
    cout << "\nGantt Chart :-" << endl;
    int gantt_size = gantt_chart.size();
    for(int i = 0; i < gantt_size-1; i++) {
        cout << "--------";
    }
    cout << endl;

    for(int i = 0; i < gantt_size-1; i++) {
        int pid = gantt_chart[i].first;
        if(pid == -1) {
            cout << "| IDLE  ";
        }
        else {
            cout << "|  P" << pid+1 << "   ";
        }
    }
    cout << "|" << endl;

    for(int i = 0; i < gantt_size-1; i++) {
        cout << "--------";
    }
    cout << endl;
    
    if(!gantt_chart.empty()){
        cout << gantt_chart[0].second;
    }
    for(int i = 0; i < gantt_size-1; i++) {
        int completion = gantt_chart[i+1].second;
        // here we are giving less space for a double digit number to make a balance
        if(completion > 9) {
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
    for(int i = 0; i < n; i++) {
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

void firstComeFirstServe() {
    int n;
    vector<int> arrTime;
    vector<int> burstTime;

    cout << "Enter number of processes:";
    cin >> n;
    if(n <= 0) return;

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
    // sorting it by arrival time with tie-breaker -> process id
    sort(detail.begin(), detail.end(), sortByArrival);

    vector<pair<int, int>> gantt_chart; // {PID, StartTime}
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
            gantt_chart.push_back({-1, time}); // -1 indicates cpu is free
            time = at;
        }

        // pushing to gantt chart
        gantt_chart.push_back({process, time});

        // calculating all values
        resTime[process] = time - at;
        time += bt;
        ct[process] = time;
        tat[process] = ct[process] - at;
        wt[process] = tat[process] - bt;
        total_tat += tat[process];
        total_wt += wt[process];
    }
    // for end time
    gantt_chart.push_back({-1, time});

    // printing gantt chart
    cout << "\nGantt Chart :-" << endl;
    for(int i = 0; i < gantt_chart.size()-1; i++) {
        cout << "--------";
    }
    cout << endl;

    for(int i = 0; i < gantt_chart.size()-1; i++) {
        int pid = gantt_chart[i].first;
        if(pid == -1) {
            cout << "| IDLE  ";
        }
        else {
            cout << "|  P" << pid+1 << "   ";
        }
    }
    cout << "|" << endl;

    for(int i = 0; i < gantt_chart.size()-1; i++) {
        cout << "--------";
    }
    cout << endl;
    
    if(!gantt_chart.empty()){
        cout << gantt_chart[0].second;
    }

    for(int i = 0; i < gantt_chart.size()-1; i++) {
        int completion = gantt_chart[i+1].second;
        // here we are giving less space for a double digit number to make a balance
        if(completion > 9) {
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
    for(int i = 0; i < n; i++) {
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
    if(n <= 0) return;

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

    while(completed < n) {
        int shortestJob = -1;
        int minBurst = 1e9;

        for(int i = 0; i < n; i++) {
            if(arrTime[i] <= currentTime && remainingBT[i] > 0) {
                if(remainingBT[i] < minBurst) {
                    minBurst = remainingBT[i];
                    shortestJob = i;
                }
                else if(remainingBT[i] == minBurst) { // tie breaker -> FCFS
                    if(arrTime[i] < arrTime[shortestJob]) {
                        shortestJob = i;
                    }
                }
            }
        }

        if(shortestJob == -1) {
            // do not repeat IDLE -> the gap is more than one
            if(gantt.size() == 0 || gantt.back().first != -1) {
                gantt.push_back({-1, currentTime});
            }
            currentTime++;
        }
        else {
            // updating response time
            if(resTime[shortestJob] == -1) {
                resTime[shortestJob] = currentTime - arrTime[shortestJob];
            }
            // decreasing burst time by 1
            remainingBT[shortestJob]--;
            // checking if the process is just before this if yes then don't add it again
            if(gantt.empty() || gantt.back().first != shortestJob) {
                gantt.push_back({shortestJob, currentTime});
            }

            currentTime++; // incrementing time only by 1
            if(remainingBT[shortestJob] == 0) {
                // process has been completed
                completed++;
                ct[shortestJob] = currentTime;
                tat[shortestJob] = ct[shortestJob] - arrTime[shortestJob];
                wt[shortestJob] = tat[shortestJob] - burstTime[shortestJob];
                total_tat += tat[shortestJob];
                total_wt += wt[shortestJob];
            }
        }
    }
    // for end time
    gantt.push_back({-99, currentTime});

    // printing gantt chart
    cout << "\nGantt Chart:-" << endl;
    int gantt_size = gantt.size();
    for(int i = 0; i < gantt_size-1; i++) {
        cout << "--------";
    }
    cout << endl;

    for(int i = 0; i < gantt_size-1; i++) {
        if(gantt[i].first == -1) {
            cout << "| IDLE  ";
        }
        else {
            cout << "|  P" << gantt[i].first+1 << "   ";
        }
    }
    cout << "|" << endl;

    for(int i = 0; i < gantt_size-1; i++) {
        cout << "--------";
    }
    cout << endl;

    // if gantt chart is not empty then only print first here
    if(!gantt.empty()) {
        cout << gantt[0].second;
    }
    for(int i = 0; i < gantt_size-1; i++) {
        int completion = gantt[i+1].second;
        // less space for 2 digit number than 1 digit number
        if(completion > 9) {
            cout << "      " << completion;
        }
        else {
            cout << "       " << completion;
        }
    }
    cout << endl << endl;

    // printing values in table form
    cout << "SJF (Preemptive) Results:" << endl;
    cout << "Process\t\tArrival\t\tBurst\t\tCompletion\tTurnaround\tWaiting" << endl;
    for(int i = 0; i < n; i++) {
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
    if(n <= 0) return;

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

    while(completed < n) {
        int highestPriorityJob = -1;
        int maxPriority = -1;

        for(int i = 0; i < n; i++) {
            if(arrTime[i] <= currentTime && remainingBT[i] > 0) {
                if(priority[i] > maxPriority) {
                    maxPriority = priority[i];
                    highestPriorityJob = i;
                }
                else if(priority[i] == maxPriority) { // tie-breaker -> FCFS
                    if(arrTime[i] < arrTime[highestPriorityJob]) {
                        highestPriorityJob = i;
                    }
                }
            }
        }

        if(highestPriorityJob == -1) {
            // avoiding continues inserting of IDLE
            if(gantt.empty() || gantt.back().first != -1) {
                gantt.push_back({ -1, currentTime });
            }
            currentTime++;
        }
        else {
            if(resTime[highestPriorityJob] == -1) {
                resTime[highestPriorityJob] = currentTime - arrTime[highestPriorityJob];
            }
            remainingBT[highestPriorityJob]--;

            // avoiding continues insertion of a process in gantt chart
            if(gantt.empty() || gantt.back().first != highestPriorityJob) {
                gantt.push_back({highestPriorityJob, currentTime});
            }
            currentTime++;

            if(remainingBT[highestPriorityJob] == 0) {
                // process is completed
                completed++;
                ct[highestPriorityJob] = currentTime;
                tat[highestPriorityJob] = ct[highestPriorityJob] - arrTime[highestPriorityJob];
                wt[highestPriorityJob] = tat[highestPriorityJob] - burstTime[highestPriorityJob];
                total_tat += tat[highestPriorityJob];
                total_wt += wt[highestPriorityJob];
            }
        }
    }
    // for end time
    gantt.push_back({-99, currentTime});

    // printing gantt chart
    cout << "\nGantt Chart :-" << endl;
    int gantt_size = gantt.size();
    for(int i = 0; i < gantt_size-1; i++) {
        cout << "--------";
    }
    cout << endl;

    for(int i = 0; i < gantt_size-1; i++) {
        if(gantt[i].first == -1) {
            cout << "| IDLE  ";
        }
        else {
            cout << "|  P" << gantt[i].first+1 << "   ";
        }
    }
    cout << "|" << endl;

    for(int i = 0; i < gantt_size-1; i++) {
        cout << "--------";
    }
    cout << endl;

    // printing the first process if gantt is not empty
    if(!gantt.empty()) {
        cout << gantt[0].second;
    }
    for(int i = 0; i < gantt_size-1; i++) {
        int completion = gantt[i+1].second;
        if(completion > 9) {
            cout << "      " << completion;
        } else {
            cout << "       " << completion;
        }
    }
    cout << endl << endl;

    // printing values in table form
    cout << "Priority (Preemptive) Results:" << endl;
    cout << "Process\t\tArrival\t\tBurst\t\tPriority\tCompletion\tTurnaround\tWaiting" << endl;
    for(int i = 0; i < n; i++) {
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
    if(n <= 0) return;

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
    cout << "Enter quantum time:";
    cin >> quantum;

    vector<vector<int>> processes;
    for(int i = 0; i < n; i++) {
        processes.push_back({arrTime[i], burstTime[i], i});
    }
    // sorting according to arrival time
    sort(processes.begin(), processes.end());

    queue<int> readyQueue;
    vector<int> remainingBT = burstTime;
    vector<int> ct(n);
    vector<int> tat(n);
    vector<int> wt(n);
    vector<int> resTime(n, -1);

    int currentTime = 0;
    int completed = 0;
    int pid = 0; // pointer to push all processes in ready queue one by one
    int total_wt = 0, total_tat = 0;
    vector<pair<int, int>> gantt;

    while(completed < n) {
        // adding all process with arrTime <= currTime
        while(pid < n && processes[pid][0] <= currentTime) {
            readyQueue.push(processes[pid][2]);
            pid++;
        }

        if(readyQueue.empty()) {
            // no process is arrived at this time
            if(pid < n) {
                int idleStartTime = currentTime;
                currentTime = processes[pid][0]; // move time to next process arrival
                // avoiding repeatation of IDLE time
                if(gantt.empty() || gantt.back().first != -1) {
                    gantt.push_back({-1, idleStartTime}); // push IDLE block with its start time
                }
            }
            else {
                // if all processes are processed
                break;
            }
            continue; // continue to next loop to re-check ready queue
        }

        // taking the front process
        int curr_pid = readyQueue.front();
        readyQueue.pop();

        // avoiding continuous repeatation of same process
        if(gantt.empty() || gantt.back().first != curr_pid) {
            gantt.push_back({curr_pid, currentTime});
        }
        // updating response time
        if(resTime[curr_pid] == -1) {
            resTime[curr_pid] = currentTime - arrTime[curr_pid];
        }

        // substracting quantum for the burst time
        int subtract_BT = min(quantum, remainingBT[curr_pid]);
        currentTime += subtract_BT;
        remainingBT[curr_pid] -= subtract_BT;

        // pushing all the processes that are arrived now
        while(pid < n && processes[pid][0] <= currentTime) {
            readyQueue.push(processes[pid][2]);
            pid++;
        }

        if(remainingBT[curr_pid] > 0) {
            readyQueue.push(curr_pid); // adding back to queue if the process is not finished
        }
        else {
            completed++;
            ct[curr_pid] = currentTime;
            tat[curr_pid] = ct[curr_pid] - arrTime[curr_pid];
            wt[curr_pid] = tat[curr_pid] - burstTime[curr_pid];
            total_tat += tat[curr_pid];
            total_wt += wt[curr_pid];
        }
    }
    // for printing end time
    gantt.push_back({-99, currentTime});

    // printing gantt chart
    cout << "\nGantt Chart :-" << endl;
    int gantt_size = gantt.size();
    for(int i = 0; i < gantt_size-1; i++) {
        cout << "--------";
    }
    cout << endl;

    for(int i = 0; i < gantt_size-1; i++) {
        if(gantt[i].first == -1) cout << "| IDLE  ";
        else cout << "|  P" << gantt[i].first+1 << "   ";
    }
    cout << "|" << endl;

    for(int i = 0; i < gantt_size-1; i++) {
        cout << "--------";
    }
    cout << endl;

    if(!gantt.empty()) {
        cout << gantt[0].second;
    }
    for(int i = 0; i < gantt_size-1; i++) {
        int completion = gantt[i+1].second;
        if(completion > 9) {
            cout << "      " << completion;
        } else {
            cout << "       " << completion;
        }
    }
    cout << endl << endl;

    // printing values in table form
    cout << "Round Robin Results:" << endl;
    cout << "Process\t\tArrival\t\tBurst\t\tCompletion\tTurnaround\tWaiting" << endl;
    for(int i = 0; i < n; i++) {
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
        firstComeFirstServe();
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
