#include <iostream>
#include <map>
#include <climits>

void get_problem_values(int &num_of_states, int &num_of_datacenters){
	std::cout << "Insert number of states" << std::endl;
	std::cin >> num_of_states;
	std::cout << "Insert number of datacenters" << std::endl;
	std::cin >> num_of_datacenters;
}

void fill_distances_matrix(int num_of_states, std::map<int, std::map<int, int> > &distances){
	for(int i = 1; i <= num_of_states; ++i){
		for(int j = i + 1; j <= num_of_states; ++j){
			int current_distance;
			std::cout << "insert distance between state " << i << " and " << j << std::endl;
			std::cin >> current_distance;
			distances[i][j] = current_distance;
			distances[j][i] = current_distance;
		}
		/* distance from a state to itself is 0 */
		distances[i][i] = 0;
	}
}

int calculate_current_latency(int num_of_states, const std::map<int, int> &datacenters, const std::map<int, std::map<int, int> > &distances){
	int total_latency = 0;
	/* for each state, check the minimum distance to each datacenter */
	for(int i = 1; i <= num_of_states; ++i){
		int this_state_lat = INT_MAX;
		for(auto it = datacenters.begin(); it != datacenters.end(); ++it){
			int read_lat = distances.at(i).at(it->second);
			if (read_lat < this_state_lat){
				this_state_lat = read_lat;
			}
		}
		total_latency += this_state_lat;
	}
	return total_latency;
}

void manage_manually_allocated_dataceneters(int num_of_states, int &num_of_manually_allocated_datacenters, std::map<int, int> &datacenters, std::map<int, std::map<int, int> > &distances){
	std::cout << "How many datacenters are manually allocated?" << std::endl;
	std::cin >> num_of_manually_allocated_datacenters;

	for (int i = 1; i <= num_of_manually_allocated_datacenters; ++i){
		int state;
		std::cout << "Insert number of state where datacenter " << i << " should be." << std::endl;
		std::cin >> state;
		datacenters[i] = state;
		/* display current latency */
		std::cout << "latency after datacenter " << i << " was allocated: " << calculate_current_latency(num_of_states, datacenters, distances) << std::endl;
	}
}

bool state_already_has_datacenter(int state, const std::map<int, int> &datacenters){
	for (auto it = datacenters.begin(); it != datacenters.end(); ++it){
		if (it->second == state){
			return true;
		}
	}
	return false;
}

int locate_new_datacenter(int num_of_datacenter, int num_of_states, std::map<int, int> &datacenters, const std::map<int, std::map<int, int> > &distances){
	/* returns number of state which minimizes latency */
	int current_lat = INT_MAX;
	int state_that_minimizes_lat;
	for(int i = 1; i <= num_of_states; ++i){
		if (!state_already_has_datacenter(i, datacenters)){
			datacenters[num_of_datacenter] = i;
			if (calculate_current_latency(num_of_states, datacenters, distances) < current_lat){
				state_that_minimizes_lat = i;
			}
		}
	}
	return state_that_minimizes_lat;
}

void locate_remaining_datacenters(int num_of_states, int num_of_datacenters, int num_of_manually_allocated_datacenters, std::map<int, int> &datacenters, std::map<int, std::map<int, int> > &distances){
	for (int i = num_of_manually_allocated_datacenters + 1; i <= num_of_datacenters; ++i){
		datacenters[i] = locate_new_datacenter(i, num_of_states, datacenters, distances);
		/* display current latency */
		std::cout << "latency after datacenter " << i << " located: " << calculate_current_latency(num_of_states, datacenters, distances) << std::endl;
	}
}

void inform_datacenter_positions(const std::map<int, int> &datacenters){
	for (auto it = datacenters.begin(); it != datacenters.end(); ++it){
		std::cout << "datacenter\t" << it->first << "\tlocated in state\t" << it->second << std::endl;
	}
}

int main(){
	int num_of_states;
	int num_of_datacenters;

	/* A map is used to store de distance matrix. 
	 * States are identified with numbers from 1 to n, 
	 * where n is the number of states to consider. 
	 * Distance must be an integer number */
	std::map<int, std::map<int, int> > distances;

	/* A map is used to link each datacenter to a state. 
	 * K = datacenter.
	 * V = state.  */
	std::map<int, int> datacenters;

	get_problem_values(num_of_states, num_of_datacenters);

	fill_distances_matrix(num_of_states, distances);

	int num_of_manually_allocated_datacenters;
	manage_manually_allocated_dataceneters(num_of_states, num_of_manually_allocated_datacenters, datacenters, distances);

	locate_remaining_datacenters(num_of_states, num_of_datacenters, num_of_manually_allocated_datacenters, datacenters, distances);

	/* inform results */
	std::cout << "*** Final datacenter positions ****" << std::endl;
	inform_datacenter_positions(datacenters);
	return 0;
}
