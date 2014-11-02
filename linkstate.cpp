#include "linkstate.h"

using namespace std;

void Linkstate::make_graph(string topofile)
{
	ifstream infile(topofile);
	string line;
	int ints[3];//holds ints from line read in

	while(getline(infile, line))
	{
		istringstream iss(line);
		int number;
		int idx = 0;

		while(iss >> number)
		{
			ints[idx] = number;		
			idx++;
		}
			
		pair<int, int> edge(ints[2], ints[1]);
		graph[ints[0]].push_back(edge);	
	}
}

void Linkstate::find_state()
{
		
	
	
	
}

void Linkstate::find_shortest_path(int source)
{



}

void Linkstate::update_graph()
{
	
}
void Linkstate::parse_changes(string changesfile)
{
	ifstream infile(changesfile);
	string line;
	int ints[3];
	vector<tuple<int, int, int> > tempchanges;
	while(getline(infile, line))
	{
		istringstream iss(line);
		int number;
		ints[3];
		int idx=0;
		while(iss >> number)
		{
			ints[idx]=number;
			idx++;			
		}
		tuple<int,int,int>change(ints[0], ints[1], ints[2]);
		tempchanges.push_back(change);
	}
	//reverse the order, push into changes
	while(!tempchanges.empty()){
		tuple<int, int, int> temp=tempchanges.back();
		changes.push_back(temp);
		tempchanges.pop_back();
	}
}
void Linkstate::change_path_weight(int source, int neighbor, int weight)
{
	
}

void Linkstate::make_new_path(int source, int neighbor, int weight)
{
	
}
/*For testing purposes */
void Linkstate::print_graph()
{
	for(unordered_map<int,  vector<pair<int, int> > >::iterator it = graph.begin(); it != graph.end(); it++)
	{	
		int src = it->first;
		vector<pair<int, int> > vec = it->second;
		for(int i = 0; i < vec.size(); i ++)
		{
			int dest = vec[i].first;
			int wgt = vec[i].second;
			cout<< src << dest << wgt << endl;
		}
	}
}



int main(int argc, char *argv[])
{
	Linkstate ls;
	ls.make_graph(argv[1]);	
	ls.print_graph();
	ls.parse_changes(argv[2]);
}
