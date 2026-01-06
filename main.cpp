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

// This is parent class or common class in all classes. It takes inputs, printing gantt chart and matrices for each process
class Scheduler {
    public:
        int n;
        int quantum;
        int total_tat, total_wt;
        bool isPriority; // extra input for priority and round robin so seperating them
        bool isRoundRobin;
        vector<int> arrTime, burstTime, priority;
        vector<pair<int, int>> gantt_chart; // {PID, StartTime}
        vector<int> ct, tat, wt, resTime; // other array for completion time, turn around time, etc.

        Scheduler(bool priority, bool roundRobin) {
            // stating whether it is prioriy or round robin or any other algorithm
            this->isPriority = priority;
            this->isRoundRobin = roundRobin;
            total_wt = 0;
            total_tat = 0;
        }

        void takeInput() {
            cout << "Enter number of processes:";
            cin >> n;
            if(n <= 0) return;
            if(this->isRoundRobin) {
                cout << "Enter time quantum:" << endl;
                cin >> quantum;
            }

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

            if(this->isPriority) {
                // taking priorities of each process as input if it is a prioriry based algorithm
                cout << "Enter priority of all processes:" << endl;
                for(int i = 0; i < n; i++) {
                    int pr;
                    cin >> pr;
                    priority.push_back(pr);
                }
            }
        }

        void printMatrices() {
            if (this->isPriority) cout << "Process\t\tArrival\t\tBurst\t\tPriority\tCompletion\tTurnaround\tWaiting\n";
            else cout << "Process\t\tArrival\t\tBurst\t\tCompletion\tTurnaround\tWaiting\n";
            for(int i = 0; i < n; i++) {
                cout << "P" << i << "\t\t";
                cout << this->arrTime[i] << "\t\t";
                cout << this->burstTime[i] << "\t\t";
                if(this->isPriority) cout << this->priority[i] << "\t\t";
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


        void printGanttChart() {
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
        }

        bool isInputValid() {
            // checking whether user entered valid input or not
            // priority can be -ve also so no need to check for it
            if(n <= 0) return false;
            if(this->isRoundRobin) {
                if(quantum <= 0) return false;
            }
            for(int i = 0; i < n; i++) {
                if(arrTime[i] < 0 || burstTime[i] < 0) return false;
            }
            return true;
        }
};


// comparator for heap of priority algorithm
class ComparePriority {
    public:
    bool operator() (vector<int> &a, vector<int>& b) {
        if(a[1] == b[1]) { // same priority then sort by arrival time
            return a[2] > b[2];
        } else {
            return a[1] < b[1];
        }
    }
};

// This class involves functionalities for priority algorithm(non-preemptive)
class Priority : public Scheduler {
    public:
        Priority(bool priority, bool roundRobin) : Scheduler(priority, roundRobin) {
            // calling parent class constuctor
        }

        void calculate() {
            ct.resize(n);
            tat.resize(n);
            wt.resize(n);
            resTime.resize(n, -1);
            vector<int> is_completed(n, 0);
            vector<int> isInQueue(n, 0); // use to avoid re-insertion of a process in priority queue
            int time = 0;
            int completed_count = 0;

            // using heap to get values in O(logn) time
            priority_queue<vector<int>, vector<vector<int>>, ComparePriority> pq;
            for(int i = 0; i < n; i++) {
                if(arrTime[i] == 0) {
                    pq.push({i, priority[i], arrTime[i]});
                    isInQueue[i] = 1;
                }
            }

            while(completed_count < n) {
                int best_job_idx = -1;
                if(!pq.empty()) {
                    // using priority queue to get next job in O(logn) time instead of iterating a loop over all the processes
                    best_job_idx = pq.top()[0];
                    pq.pop();
                    isInQueue[best_job_idx] = 0; // we are popping this process so this will not be in queue
                }

                if(best_job_idx != -1) {
                    // A process is selected
                    int process = best_job_idx;
                    int at = arrTime[process];
                    int bt = burstTime[process];

                    gantt_chart.push_back({process, time});

                    if(resTime[process] == -1)
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
                else {
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
                for(int i = 0; i < n; i++) {
                    if(arrTime[i] <= time && is_completed[i] == 0 && isInQueue[i] == 0) {
                        pq.push({i, priority[i], arrTime[i]});
                        isInQueue[i] = 1;
                    }
                }
            }
            // for end time
            gantt_chart.push_back({-99, time});
        }
};

class PriorityPreemptive : public Scheduler {
    public:
        PriorityPreemptive(bool priority, bool roundRobin) : Scheduler(priority, roundRobin) {
            // calling parent class constructor
        }

        void calculate() {
            vector<int> remainingBT = burstTime;
            ct.resize(n);
            tat.resize(n);
            wt.resize(n);
            resTime.resize(n, -1);
            vector<int> isInQueue(n, 0);
            int currentTime = 0;
            int completed_count = 0;
            
            // using heap to get values in O(logn) time
            priority_queue<vector<int>, vector<vector<int>>, ComparePriority> pq;

            for(int i = 0; i < n; i++) {
                if(arrTime[i] == 0 && remainingBT[i] > 0) {
                    pq.push({i, priority[i], arrTime[i]});
                    isInQueue[i] = 1;
                }
            }

            while(completed_count < n) {
                int highestPriorityJob = -1;
                if(!pq.empty()) {
                    // using priority queue to get next job in O(logn) time instead of iterating a loop over all the processes
                    highestPriorityJob = pq.top()[0];
                    pq.pop();
                    isInQueue[highestPriorityJob] = 0; // we are popping this process so this will not be in queue
                }

                if(highestPriorityJob != -1) {
                    if(resTime[highestPriorityJob] == -1) {
                        resTime[highestPriorityJob] = currentTime - arrTime[highestPriorityJob];
                    }
                    remainingBT[highestPriorityJob]--;

                    // avoiding continues insertion of a process in gantt chart
                    if(gantt_chart.empty() || gantt_chart.back().first != highestPriorityJob) {
                        gantt_chart.push_back({highestPriorityJob, currentTime});
                    }
                    currentTime++;

                    if(remainingBT[highestPriorityJob] == 0) {
                        // process is completed
                        completed_count++;
                        ct[highestPriorityJob] = currentTime;
                        tat[highestPriorityJob] = ct[highestPriorityJob] - arrTime[highestPriorityJob];
                        wt[highestPriorityJob] = tat[highestPriorityJob] - burstTime[highestPriorityJob];
                        total_tat += tat[highestPriorityJob];
                        total_wt += wt[highestPriorityJob];
                    } else {
                        // re-insert it to heap
                        int idx = highestPriorityJob; // using idx for simplicity
                        pq.push({idx, priority[idx], arrTime[idx]});
                        isInQueue[idx] = 1;
                    }
                }
                else {               
                    // avoiding continues inserting of IDLE
                    if(gantt_chart.empty() || gantt_chart.back().first != -1) {
                        gantt_chart.push_back({ -1, currentTime });
                    }
                    int nextTime = 1e9;
                    for(int i = 0; i < n; i++) {
                        if(remainingBT[i] > 0 && arrTime[i] >= currentTime) {
                            nextTime = min(nextTime, arrTime[i]);
                        }
                    }
                    if(nextTime == 1e9) break; // no process left
                    currentTime = nextTime;
                }

                for(int i = 0; i < n; i++) {
                    if(arrTime[i] <= currentTime && remainingBT[i] > 0 && isInQueue[i] == 0) {
                        pq.push({i, priority[i], arrTime[i]});
                        isInQueue[i] = 1;
                    }
                }
            }
            // for end time
            gantt_chart.push_back({-99, currentTime});
        }
};


// comparator for shortest job first
class CompareSJS {
    public:
    bool operator() (vector<int>& a, vector<int>& b) {
        if(a[1] == b[1]) { // if same burst time then short by arrival time
            return a[2] > b[2];
        } else {
            return a[1] > b[1];
        }
    }
};

class SJF : public Scheduler {
    public:
        SJF(bool priority, bool roundRobin) : Scheduler(priority, roundRobin) {
            // calling parent class constructor
        }

        void calculate() {
            ct.resize(n);
            tat.resize(n);
            wt.resize(n);
            resTime.resize(n, -1);
            vector<int> isCompleted(n, 0);
            vector<int> isInQueue(n, 0); // to avoid repeatative insertion in heap
            int time = 0;
            int completed_count = 0;

            priority_queue<vector<int>, vector<vector<int>>, CompareSJS> pq;
            for(int i = 0; i < n; i++) {
                if(arrTime[i] == 0) {
                    pq.push({i, burstTime[i], arrTime[i]});
                    isInQueue[i] = 1;
                }
            }

            while(completed_count < n) {
                // finding a job with minimum burst time and valid arrival time
                int best_job_idx = -1;
                if(!pq.empty()) {
                    best_job_idx = pq.top()[0];
                    pq.pop();
                    isInQueue[best_job_idx] = 0;
                }

                if(best_job_idx != -1) {
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
                    isCompleted[process] = 1;
                    completed_count++;
                }
                else {
                    // no job is ready, CPU is empty
                    int next_arrival = 1e9;
                    for(int i = 0; i < n; i++) {
                        if(isCompleted[i] == 0) {
                            next_arrival = min(next_arrival, arrTime[i]);
                        }
                    }
                    gantt_chart.push_back({-1, time});
                    time = next_arrival;
                }
                for(int i = 0; i < n; i++) {
                    if(isCompleted[i] == 0 && isInQueue[i] == 0) {
                        pq.push({i, burstTime[i], arrTime[i]});
                        isInQueue[i] = 1;
                    }
                }
            }
            // for end time
            gantt_chart.push_back({-99, time});
        }
};

class SJFPreemptive : public Scheduler {
    public:
        SJFPreemptive(bool priority, bool roundRobin) : Scheduler(priority, roundRobin) {
            // calling parent class constructor
        }

        void calculate() {
            ct.resize(n);
            tat.resize(n);
            wt.resize(n);
            resTime.resize(n, -1);
            vector<int> remainingBT = burstTime;
            vector<int> isInQueue(n, 0); // to avoid repeatative insertion in heap
            int currentTime = 0;
            int completed_count = 0;
            
            priority_queue<vector<int>, vector<vector<int>>, CompareSJS> pq;
            for(int i = 0; i < n; i++) {
                if(arrTime[i] == 0) {
                    pq.push({i, burstTime[i], arrTime[i]});
                    isInQueue[i] = 1;
                }
            }

            while(completed_count < n) {
                int shortestJob = -1;
                if(!pq.empty()) {
                    shortestJob = pq.top()[0];
                    pq.pop();
                    isInQueue[shortestJob] = 0;
                }

                if(shortestJob != -1) {
                    // updating response time
                    if(resTime[shortestJob] == -1) {
                        resTime[shortestJob] = currentTime - arrTime[shortestJob];
                    }
                    // decreasing burst time by 1
                    remainingBT[shortestJob]--;
                    // checking if the process is just before this if yes then don't add it again
                    if(gantt_chart.empty() || gantt_chart.back().first != shortestJob) {
                        gantt_chart.push_back({shortestJob, currentTime});
                    }

                    currentTime++; // incrementing time only by 1
                    if(remainingBT[shortestJob] == 0) {
                        // process has been completed
                        completed_count++;
                        ct[shortestJob] = currentTime;
                        tat[shortestJob] = ct[shortestJob] - arrTime[shortestJob];
                        wt[shortestJob] = tat[shortestJob] - burstTime[shortestJob];
                        total_tat += tat[shortestJob];
                        total_wt += wt[shortestJob];
                    } else {
                        // push again to heap
                        pq.push({shortestJob, burstTime[shortestJob], arrTime[shortestJob]});
                        isInQueue[shortestJob] = 1;
                    }
                }
                else {
                    // do not repeat IDLE -> the gap is more than one
                    if(gantt_chart.size() == 0 || gantt_chart.back().first != -1) {
                        gantt_chart.push_back({-1, currentTime});
                    }
                    // jumping to next arrival time
                    int nextArrival = 1e9;
                    for(int i = 0; i < n; i++) {
                        if(arrTime[i] > currentTime) nextArrival = min(nextArrival, arrTime[i]);
                    }
                    currentTime = nextArrival;
                }
                for(int i = 0; i < n; i++) {
                    if(remainingBT[i] > 0 && isInQueue[i] == 0) {
                        pq.push({i, burstTime[i], arrTime[i]});
                        isInQueue[i] = 1;
                    }
                }
            }
            // for end time
            gantt_chart.push_back({-99, currentTime});
        }
};

class FCFS : public Scheduler{
    public:
        FCFS(bool priority, bool roundRobin) : Scheduler(priority, roundRobin) {
            // calling parent class constructor
        }

        void calculate() {
            ct.resize(n);
            tat.resize(n);
            wt.resize(n);
            resTime.resize(n, -1);

            // combining both arrival time and burst time in one array
            vector<vector<int>> detail;
            for(int i = 0; i < n; i++) {
                detail.push_back({i, arrTime[i], burstTime[i]});
            }
            // sorting it by arrival time with tie-breaker -> process id
            sort(detail.begin(), detail.end(), sortByArrival);

            int time = 0;
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
        }
};

class RoundRobin : public Scheduler {
    public:
        RoundRobin(bool priority, bool roundRobin) : Scheduler(priority, roundRobin) {
            // calling parent class constuctor
        }

        void calculate() {
            queue<int> readyQueue;
            vector<int> remainingBT = burstTime;
            ct.resize(n);
            tat.resize(n);
            wt.resize(n);
            resTime.resize(n, -1);

            vector<vector<int>> processes;
            for(int i = 0; i < n; i++) {
                processes.push_back({arrTime[i], burstTime[i], i});
            }
            // sorting according to arrival time
            sort(processes.begin(), processes.end());

            int currentTime = 0;
            int completed_count = 0;
            int pid = 0; // pointer to push all processes in ready queue one by one

            while(completed_count < n) {
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
                        if(gantt_chart.empty() || gantt_chart.back().first != -1) {
                            gantt_chart.push_back({-1, idleStartTime}); // push IDLE block with its start time
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
                if(gantt_chart.empty() || gantt_chart.back().first != curr_pid) {
                    gantt_chart.push_back({curr_pid, currentTime});
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
                    completed_count++;
                    ct[curr_pid] = currentTime;
                    tat[curr_pid] = ct[curr_pid] - arrTime[curr_pid];
                    wt[curr_pid] = tat[curr_pid] - burstTime[curr_pid];
                    total_tat += tat[curr_pid];
                    total_wt += wt[curr_pid];
                }
            }
            // for printing end time
            gantt_chart.push_back({-99, currentTime});
        }
};

void shortestJobFirst() {
    SJF process(false, false);
    process.takeInput();
    if(!process.isInputValid()) {
        cout << "Wrong Input!";
        return;
    }
    process.calculate();
    process.printGanttChart();
    process.printMatrices();
}

void shortestJobFirstPreemptive() {
    SJFPreemptive process(false, false);
    process.takeInput();
    if(!process.isInputValid()) {
        cout << "Wrong Input!";
        return;
    }
    process.calculate();
    process.printGanttChart();
    process.printMatrices();
}

void firstComeFirstServe() {
    FCFS process(false, false);
    process.takeInput();
    if(!process.isInputValid()) {
        cout << "Wrong Input!";
        return;
    }
    process.calculate();
    process.printGanttChart();
    process.printMatrices();
}

void priorityScheduling() {
    Priority process(true, false);
    process.takeInput();
    if(!process.isInputValid()) {
        cout << "Wrong Input!";
        return;
    }
    process.calculate();
    process.printGanttChart();
    process.printMatrices();
}

void prioritySchedulingPreemptive() {
    PriorityPreemptive process(true, false);
    process.takeInput();
    if(!process.isInputValid()) {
        cout << "Wrong Input!";
        return;
    }
    process.calculate();
    process.printGanttChart();
    process.printMatrices();
}

void roundRobin() {
    RoundRobin process(false, true);
    process.takeInput();
    if(!process.isInputValid()) {
        cout << "Wrong Input!";
        return;
    }
    process.calculate();
    process.printGanttChart();
    process.printMatrices();
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
    switch(choice) {
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
    default:
        cout << "Wrong choice!";
        break;
    }

    return 0;
}
