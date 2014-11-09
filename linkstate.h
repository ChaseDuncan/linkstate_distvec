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
		void print_routing_table(int source);

	private:
		/*source->cost*neighbor*/
		unordered_map<int, vector<pair<int, int> > > graph;
		/*source-> dest-> (distance, predecessor) */
		unordered_map< int, unordered_map<int, pair<int, int> >* > distances;
		/*source->neighbor->newcost*/
		vector<tuple<int, int, int> > changes;

};


#endif /*linkstate.h*/
/*
1  function Dijkstra(Graph, source):
2      dist[source] := 0                     // Initializations
3      for each vertex v in Graph:           
4          if v ≠ source
5              dist[v] := infinity           // Unknown distance from source to v
6              previous[v] := undefined      // Predecessor of v
7          end if
8          Q.add_with_priority(v,dist[v])
9      end for 
10
11
12     while Q is not empty:                 // The main loop
13         u := Q.extract_min()              // Remove and return best vertex
14         mark u as scanned
15         for each neighbor v of u:
16             if v is not yet scanned:
17                 alt = dist[u] + length(u, v) 
18                 if alt < dist[v]
19                     dist[v] := alt
20                     previous[v] := u
21                     Q.decrease_priority(v,alt)
22                 end if
23             end if
24         end for
25     end while
26     return previous[]*/
