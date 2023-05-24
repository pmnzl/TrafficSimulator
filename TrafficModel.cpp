#include "TrafficModel.h"
#include <algorithm> 
#include <iostream>
using namespace std;

TrafficModel::TrafficModel() { }
TrafficModel::~TrafficModel(){ }

void TrafficModel::set_commands(vector<string> commands)
{
	this->commands = commands;
}

/* A helper function. 
 * Given a specific id as the input, it searches the lane change command stored in
 * "commands". Returns the value of the turn-light in the command for this car id.
 * 0 = no light, 1 = left light, 2 = right light.
 */
int TrafficModel::get_lane_change_command(int id)
{
	int count = commands.size();
	for (int i = 0; i < count; i++){
		size_t found = commands[i].find(',');
		string iter_id = commands[i].substr(0, found);
		string light = commands[i].substr(found+1, commands[i].size()-1);
		stringstream ss1(iter_id);
		int id_value;
		ss1 >> id_value;
		if (id_value == id){
			stringstream ss2(light);
			int light_value;
			ss2 >> light_value;
			return light_value;
		}
	}
	return 0;
}

/*
 * The function that updates the vehicle positions and states.
 */
void TrafficModel::update()
{
	Car* current = NULL;
	Car* temp = NULL;
	int signal = 0;
	bool move = false;
	vector<int> complete;

	for (unsigned int i = 0; i < platoons.size(); i++){
		current = platoons[i].get_tail();

		while (current != NULL)
		{
			temp = current->get_prev(); //used as address changes in the loop causing errors
			//check if vehicle has already been moved this tick
			move = (complete.end() != find(complete.begin(), complete.end(), current->get_id()));
			signal = get_lane_change_command(current->get_id()); //get signal from a specified car

			switch (signal)
			{
				case 0 /*drive straight*/:
					if (move == false)
					{
						platoons[i].moveFoward(current);
						move = true;
					}
					break;
				
				case 1 /*turn left*/:
					if (i != 0 && move == false) //checking for invalid moves
					{
						if (platoons[i-1].free(current) == true)
						{
							platoons[i].remove(current);
							platoons[i-1].insert(current);
							move = true;
						}
					}
					break;

				case 2 /*turn right*/:
					if ((i != platoons.size() - 1) && move == false) //checking for invalid moves
					{
						if (platoons[i+1].free(current) == true)
						{
							platoons[i].remove(current);
							platoons[i+1].insert(current);
							move = true;
						}
					}
					break;
				
				default:
					break;
			}

			if (move == false) //if all cases fail try move the car forward
			{
				platoons[i].moveFoward(current);
			}

			complete.push_back(current->get_id()); //add vehicle id to complete
			current = temp; //point back to the correct address
		}
	}
	complete.clear(); //clears vector for next tick
}


/*
 * Initialization based on the input information
 */
void TrafficModel::initialize(vector<string> info)
{
	int lane_count = info.size();
	for (int i = 0; i < lane_count; i++){
		Platoon p = Platoon(info[i]);
		platoons.push_back(p);
	}
}

// Returns all the vehicle states in the system
vector<string> TrafficModel::get_system_state()
{
	vector<string> output;
	int size = platoons.size();
	for (int i = 0; i < size; i++){
		// get the last vehicle in the platoon
		Car* temp = platoons[i].get_tail();
		string s = "";
		ostringstream out;
		while (temp != NULL){
			out << ";(" << temp->get_id() << "," << i << "," << temp->get_position() << \
					 "," << get_lane_change_command(temp->get_id()) << ")";
			temp = temp->get_prev();
		}

		output.push_back(out.str());
	}
	return output;
}