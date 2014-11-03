#ifndef LINKSTATE_H
#define LINKSTATE_H

#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <utility>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <tuple>

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
		void print_routing_table(int source);

	private:
		/*source->neighbor->cost*/
		unordered_map<int, vector<pair<int, int> > > graph;
		/*source-> dest-> (predecessor*distance) */
		unordered_map< int, unordered_map<int, pair<int, int> > > distances;
		/*source->neighbor->newcost*/
		vector<tuple<int, int, int> > changes; //hello

};


#endif /*linkstate.h*/
