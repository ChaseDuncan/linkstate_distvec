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
		

		/*Functions for testing*/
		void print_graph();

	private:
		unordered_map<int,  vector<pair<int, int> > > graph;

};


#endif /*linkstate.h*/
