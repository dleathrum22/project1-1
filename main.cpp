#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <queue>
#include <deque>
#include <vector>

// GLOBAL SIMULATION VARIABLES
	int n = 0;
	int m = 1;
	int t_cs = 6;
	int t_slice = 94;

class Process {
public:
	Process() {
		ID = ""; 
		arrival = 0, burst = 0, numburst = 0, IO = 0, wait = 0;
	}
	Process(std::string id, int arr, int b, int num, int io){
		ID = id;
		arrival = arr;
		burst = b;
		numburst = num;
		IO = io;
		wait = io;
		isWait = 0;
		arrived = 0;
		finished = 0;
		waitTime = 0;
		endTime = 0;
		timer = 0;
	}
	std::string ID;
	int arrival, burst, numburst, IO, wait, isWait, arrived, finished, waitTime, endTime, timer;

	void setID(std::string id) {ID = id;}
	void setArr(int arr){arrival = arr;}
	void setBurst(int b){burst = b;}
	void setNum(int num){numburst = num;}
	void setIo(int io) {IO = io;}
	void countBurst() { numburst--; }
	void setWaitIO() { wait = IO; }
};

std::string printQ( std::queue<Process> q ) {
	//std::cout << "in printQ" << std::endl;
	std::string storage;
	while( !(q.empty()) ) {
		storage += " ";
		storage += q.front().ID;
		q.pop();
	}
	if( storage == "" ) { storage = " <empty>"; }
	return storage;
}


void readFile(std::string filename, std::vector<Process> &processes){
	// std::cout << "Started readFile\n";
	std::ifstream input(filename.c_str());
	std::string line;
	while(getline(input,line)){
		if(line[0] == '#' || line == ""){ continue; }
		std::string id;
		int arr = 0, b = 0, num = 0, io = 0, count = 0, temp = 0, lasti = 0;
		for(int i = 0; i < line.size(); i++){
			// if(line[i] == '|'){
			// 	count++;
			// }else{
			// 	char temp = line[i];
			// 	switch(count){
			// 		case 0: id += line[i];
			// 		case 1: arr = (arr*10) + atoi(temp) ;
			// 		case 2: b = (b*10) + temp;
			// 		case 3: num = (num*10) + temp;
			// 		case 4: io = (io*10) + temp;
			// 	}
			// }
			// std::cout << "i " << i << std::endl;
			if(line[i] == '|'){
				switch(count){
					case 0: id = line.substr(0, 1);
							// std::cout << "id " << id << std::endl;
							break;
					case 1: arr = atoi(line.substr(lasti+1, i-1).c_str());
							// std::cout << line.substr(lasti, i-1).c_str() << std::endl;
							break;
					case 2: b = atoi(line.substr(lasti+1, i-1).c_str());
							// std::cout << b << std::endl;
							break;
					case 3: num = atoi(line.substr(lasti+1, i-1).c_str());
							io = atoi(line.substr(i+1,line.size()-1).c_str());
							// std::cout << num << std::endl;
							break;
				}
				count++;
				lasti = i;
			}else{
				// std::cout << "count " << count << std::endl;
				// switch(count){
				// 	case 0: id = line.substr(lasti, i);
				// 			std::cout << "id " << id << std::endl;
				// 			break;
				// 	case 1: arr = std::atoi(line.substr(lasti, i));
				// 			std::cout << arr << std::endl;
				// 			break;
				// 	case 2: b = std::atoi(line.substr(lasti, i));
				// 			std::cout << b << std::endl;
				// 			break;
				// 	case 3: num = std::atoi(line.substr(lasti, i));
				// 			std::cout << num << std::endl;
				// 			break;
				// 	case 4: io = std::atoi(line.substr(lasti, i));
				// 			std::cout << io << std::endl;
				// 			break;
				// }
			}
		}
		// std::cout << id << " " << arr << " " << b << " " << num << " " << io << std::endl;
		Process T(id, arr, b, num, io);
		processes.push_back(T);
	}
	n = processes.size();
	input.close();
}

/*void FCFS(std::string filename){
	std::vector<Process> processes;
	int finished = 0;
	int t = 0;
	readFile(filename, processes);
	std::queue<Process> ready;
	std::queue<Process> running;

	std::cout << "time " << t << "ms: Simulator started for FCFS [Q <empty>]" << std::endl;
	// std::cout << "test\n";
	for(int i = 0; i < processes.size(); i++){
		// std::cout << processes[i].arrival << std::endl;
		if(processes[i].arrival == 0){
			ready.push(processes[i]);
			processes[i].arrived = 1;
			std::cout << "time " << t << "ms: Proccess " << processes[i].ID << " arrived and added to the ready queue [Q " << ready.back().ID << "]" << std::endl;
			// std::cout << i << std::endl;
		}

	}

	// std::cout << "rocketleague\n";
	while(finished < processes.size()){
		if(running.empty() && !ready.empty()){
			running.push(ready.front());
			ready.pop();
			// std::cout << running.front().ID << " " << ready.size() << std::endl;
			std::cout << "time " << t+3 << "ms: Proccess " << running.front().ID << " started using the CPU [Q " << ready.back().ID << "]" << std::endl;
			t+=running.front().burst+6;
			// std::cout << "test" <<std::endl;
			for(int i=0; i< processes.size();++i)
			{
				if(processes[i].arrival > t-6)
				{
					processes[i].arrival-=t;
					// std::cout << "arr " << processes[i].arrival << std::endl;
				}
				else if (processes[i].arrival <=t && processes[i].arrived == 0 && processes[i].finished == 0)
				{
					// std::cout << "ready " << processes[i].ID << std::endl;
					processes[i].arrived = 1;
					ready.push(processes[i]);
					// std::cout << ready.size() << std::endl;
					std::cout << "time " << t-running.front().burst + processes[i].arrival - 6 << "ms: Proccess " << processes[i].ID << " arrived and added to the ready queue [Q " << ready.back().ID << "]" << std::endl;
				}
			}

			// t+=6;
			std::cout << "time " << t-3 << "ms: Proccess " << running.front().ID << " completed a CPU burst; " << running.front().numburst << " bursts to go [Q " << ready.back().ID << "]" << std::endl;


			//BLOCK PROCESSES HERE
			for(int i = 0; i < processes.size(); ++i){
				// std::cout << "aaa " << processes[i].isWait << std::endl;
				if(processes[i].isWait == 1){
					processes[i].wait -= running.front().burst;
					// std::cout << "TEST" << std::endl;
				}
				if(processes[i].wait <= 0 && processes[i].isWait == 1 && processes[i].finished == 0){
					processes[i].isWait = 0;
					processes[i].wait = processes[i].IO;
					std::cout << "time " << t-3 << "ms: Proccess " << processes[i].ID << " completed I/O; added to ready queue [Q " << ready.back().ID << "]" << std::endl;
					ready.push(processes[i]);
					// std::cout << ready.size() << std::endl;
				}
			}

			if(running.front().numburst <= 0){
				std::cout << "time " << t-3 << "ms: Proccess " << running.front().ID << " terminated [Q <empty>]" << std::endl;
				finished++;
				running.front().finished = 1;
				running.pop();
			}else{
				running.front().countBurst();
				std::cout << "time " << t-3 << "ms: Proccess " << running.front().ID << " switching out of CPU; will block on I/O until time " << t+running.front().IO << " ms [Q " << running.back().ID << "]" << std::endl;
				running.front().isWait = 1;
				running.pop();
			}

		}else if(running.empty() && ready.empty()){
			int lowest = processes[0].wait;
			int pos = 0;
			for(int i = 1; i < processes.size(); i++){
				if(processes[i].wait < lowest && processes[i].finished == 0){
					lowest = processes[i].wait;
					pos = i;
				}
			}
			t += lowest;
			processes[pos].isWait = 0;
			processes[pos].wait = processes[pos].IO;
			ready.push(processes[pos]);
			// std::cout << ready.size() << std::endl;
		}
	}
	// std::cout << "FCFS took " << (float)t/CLOCKS_PER_SEC << " seconds" << std::endl;
} */

void FCFS(std::string filename) {
	std::vector<Process> processes;
	std::vector<Process> blocked;
	std::vector<Process>::iterator iterator;
	int finished = 0;
	int total = 0;
	int t = 0;
	int swi1 = 0;
	int swi2 = 0;
	std::string str;
	int whileBurst = 6;
	readFile(filename, processes);
	std::queue<Process> ready;
	std::queue<Process> running;

	std::cout << "time " << t << "ms: Simulator started for FCFS [Q <empty>]" << std::endl;
	for(int i = 0; i < processes.size(); i++){
		total += processes[i].numburst;
		//std::cout << processes[i].ID << " numburst:  " << processes[i].numburst << std::endl;	
	}
	//std::cout << "total:  " << total << std::endl;

	while( finished < total ) {
		for( int j = 0; j < processes.size(); ++j ) {
			if ( processes[j].arrival == t ) {
				ready.push(processes[j]);
				//std::cout << "ready back:  " << ready.back().ID << std::endl;
				//std::cout << "how about the front:  " << ready.front().ID << std::endl;
				processes[j].arrived = 1;
				str = printQ( ready );
				std::cout << "time " << t << "ms: Process " << processes[j].ID << " has arrived and added to ready queue [Q" << str << "]" << std::endl;
			}
		}

		//std::cout << "first on ready:  " << ready.front().ID << std::endl;

		if( (swi1 <= 0) ) {
			if ( !(running.empty()) ){
				running.front().timer -= 1;
				if( running.front().timer == 0 ) {
					swi1 += 3;
					running.front().numburst -= 1;
					if ( running.front().numburst == 0 ) {
						str = printQ( ready );
						std::cout << "time " << t << "ms: Process "<< running.front().ID << " terminated [Q" << str << "]" << std::endl;
						//std::cout << "time " << t << "ms: Process " running.front()>ID << " "erminated
						//std::cout << "Finished " << running.front().ID << " at time " << t << std::endl;
					}
					else {
						//ready.push( running.front() );
						iterator = blocked.end();
						blocked.insert(iterator, running.front() );
						blocked.back().setWaitIO();
						int till = t + running.front().wait;
						str = printQ( ready );
						std::cout << "time " << t << "ms: Process "<< running.front().ID << " completed a CPU burst; " << running.front().numburst << " bursts to go [Q" << str << "]" << std::endl;
						std::cout << "time " << t << "ms: Process " << running.front().ID << " switching out of CPU; will block in I/O until time " << till+3 << "ms [Q" << str << "]" << std::endl;
						//std::cout << "Finished one burst of " << running.front().ID << " with " << running.front().numburst << " left at time " << t << std::endl;
					}
					++finished;
					running.pop();
					//std::cout << "finished:  " << finished << std::endl;
				}

			}

			//std::cout << "ready size:  " << ready.size() << std::endl;
			if( running.empty() && !(ready.empty()) ) {
				// /std::cout << "time " << t << "ms: Process " << ready.front().ID << " started using the CPU [Q" << str << "]" << std::endl;
				//std::cout << "Switching the active process to " << ready.front().ID << " at time " << t << std::endl;
				swi1 += 3;
				//std::cout << "ready front:  " << ready.front().ID << std::endl;
				running.push( ready.front() );
				//std::cout << "running front:  " << running.front().ID << std::endl;
				str = printQ( ready );
				std::string temp = ready.front().ID;
				ready.pop();
				str = printQ( ready );
				std::cout << "time " << t+swi1 << "ms: Process " << temp << " started using the CPU [Q" << str << "]" << std::endl;
				// ready.pop();
				running.front().timer = running.front().burst;
				if( whileBurst > 0 ) {
					--whileBurst;
				}
				whileBurst = 6;
			}
			
			// for( int k = 0; k < processes.size(); ++k ) {
			// 	if( (processes[k].arrived == 1) && (processes[k].ID != running.front().ID) ) {

			// 	}
			// }

			iterator = blocked.begin();
			//std::cout << "entering blocked checker" << std::endl;
			if( !(blocked.empty()) ) {
				for( int u = 0; u < blocked.size(); ++u ) {
					blocked[u].wait -= 1;
					if( blocked[u].wait == 0 ) {
						ready.push( blocked[u] );
						str = printQ( ready );
						std::cout << "time " << t << "ms: Process " << blocked[u].ID << " completed I/O; added to ready queue [Q" << str << "]" << std::endl;
						blocked.erase( iterator );
					}
					iterator++;
				}
			}
		}else{
			// std::cout << swi1 << std::endl;
			swi1--;
		}

		/*if ( !(ready.empty()) ) {
			std::vector<Process> it;
  			std::vector<Process>::iterator iter;
			while( !(ready.empty()) ) {
        		iter = it.insert( it.begin(), ready.front() );
        		ready.pop();
			}	
			for( iter = it.begin(); iter != it.end(); ++iter ) {
				(*iter).waitTime += 1;
			}
			while( !(it.empty()) ) {
				ready.push(it.front());
				it.pop_back();
			}
		}*/
		// --swi1;
		// --swi2;
		++t;
		//std::cout << "running:  " << running.front().ID << " at time " << t << std::endl;
		//std::cout << "the time has now reached:  " << t << std::endl;
	}
	t += 2;
	std::cout << "time " << t << "ms: Simulator ended for FCFS" << std::endl;
}


int main(int argc, char** argv){
	std::string filename = argv[1];

	for(int i = 0; i < 3; i++){
		switch(i){
			case 0: FCFS(filename);
			// case 1: SRT(filename);
			// case 2: RR(filename);

		}
	}
	return EXIT_SUCCESS;
}
