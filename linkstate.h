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

using namespace std;

class Linkstate
{
	public:
		void make_graph(string topofile);	
		void find_state()	

		/*Functions for testing*/
		void print_graph();

	private:
		/*source->neighbor->cost*/
		unordered_map<int, vector<pair<int, int> > > graph;
		/*source-> dest-> (predecessor*distance) */
		unordered_map< int, unordered_map<int, vector<pair<int, int> > > > distances;

};


#endif /*linkstate.h*/
