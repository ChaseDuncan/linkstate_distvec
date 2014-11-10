#ifndef LINKSTATE_H
#define LINKSTATE_H

#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <functional>
#include <utility>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <tuple>
#include <limits.h>

using namespace std;

class Linkstate
{
	public:
		void make_graph(string topofile);	
		void find_state();
		void find_shortest_path(int source);	
		void update_graph();
		void change_path_weight(int source, int neighbor, int weight);
		void make_new_path(int source, int neighbor, int weight);
		void parse_changes(string changesfile);
		/*Functions for testing*/
		void print_graph();
		//void print_routing_table(int source);
		void print_routing_table(ofstream & myfile);
		void output_to_file(string message_file);
		void print_message_path(ofstream & myfile, string message_file);
		void populate_distances();

	private:
		/*source->cost*neighbor*/
		unordered_map<int, vector<pair<int, int> > > graph;
		/*source-> dest-> (distance, predecessor) */
		unordered_map< int, unordered_map<int, pair<int, int> >* > distances;
		/*source->neighbor->newcost*/
		vector<tuple<int, int, int> > changes;
		/*source, dest, route*/
		unordered_map<int, unordered_map<int, vector<int> > > routing_table;

};


#endif /*linkstate.h*/

