#ifndef DISTVEC_H
#define DISTVEC_H

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

class Distvec
{
	public:
		void make_graph_and_list_edges(string topofile);
		int parse_changes(string changesfile);	
		void print_graph();
		void print_edges();
		void populate_distances();
		void bellman_ford(int source);
		void output_to_file(ofstream & myfile, string message_file);
		void print_routing_table(ofstream & output);
		void print_message_path(ofstream & output, string message_file);
		void update_graph_and_edges(int ithchange);
		void delete_edge(int source, int dest);
		void update_edge(int source, int dest, int newcost);
	private:	
		/*source->cost*neighbor*/
		unordered_map<int, vector<pair<int, int> > > graph;	
		/*source->neighbor->newcost*/
		vector<tuple<int, int, int> > changes;
		/*source, dest, route*/
		unordered_map<int, unordered_map<int, vector<int> > > routing_table;
		/*src, cost, neighbor*/
		vector<tuple<int, int, int> > edges;
		/*source->dest->distance*predecessor*/
		unordered_map<int, unordered_map<int, pair<int, int> > * > distances;

	
};

#endif


