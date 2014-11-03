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
			
		pair<int, int> edge_1(ints[1], ints[2]);
		graph[ints[0]].push_back(edge_1);	
		pair<int, int> edge_2(ints[0], ints[2]);
		graph[ints[1]].push_back(edge_2);
	}
}

void Linkstate::find_state()
{
	
	
	
}

void Linkstate::find_shortest_path(int source)
{	
	/*For sytantic simplicity we get references to 
	  the dictionary and the vector that we need*/
	unordered_map<int, pair<int, int> >* map = &distances[source];	
	vector<pair<int, int> >* neighbors = &graph[source];
	
	//put source in the map
	pair<int, int> src(source, 0);
	(*map)[source] = src;

	priority_queue<tuple<int, int, int> > pq;
	int predecessor = source;	

	//initialize priority queue
	cout<< "size of neighbors is "<< (*neighbors).size()<<endl;
	for(int i = 0; i < (*neighbors).size(); i++)
	{
		//grab the next neighbor
		pair<int, int> next = (*neighbors)[i];
		//make a new pair of (weight*node) to and add it to the prioity queue
		cout<< "curr_node: "<< next.first<< " weight: "<< next.second<< " pred: "<< predecessor<< endl;

		tuple<int, int, int> np(next.second,  next.first, predecessor);		
		pq.push(np);
	}	
	
	//main loop
	while(!pq.empty())
	{
		//get next best (weight*node*predecessor)
		tuple<int, int, int> best = pq.top();		
		pq.pop();

		int curr_node = get<1>(best);
		int curr_weight = get<0>(best);
		int curr_pred = get<2>(best);
		
		/*check to see if current node being considerd has already logged*/
		if((*map).find(curr_node)==(*map).end())
		{
			pair<int, int>	temp(curr_pred, curr_weight);
			(*map)[curr_node] = temp;
		}	

		//grab the vector for neighbors of best
		vector<pair<int, int> >* best_neighbors = &graph[curr_node];
		predecessor = curr_node;

		//add best's neighbors to the priority queue
		for(int i = 0; i < (*best_neighbors).size(); i++)
		{
			//grab the next neighbor
			pair<int, int> next = (*best_neighbors)[i];
			cout<< "curr_node: "<< next.first<< " weight: "<< next.second<< " pred: "<< predecessor<< endl;
			//make a new pair of (weight*node) to and add it to the prioity queue
			
			if((*map).find(curr_node)==(*map).end())
			{
				tuple<int, int, int> np(next.second,  next.first, predecessor);		
				pq.push(np);
			}
		}	
	}
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

void Linkstate::print_routing_table(int source)
{
	unordered_map< int, pair<int, int> >* table = &distances[source];
	for(unordered_map<int,  pair<int, int> >::iterator it = (*table).begin(); it != (*table).end(); it++)
	{
		pair<int, int> data = it->second;
		int node = it->first;
		int pred = data.first;
		int distance = data.second;
		int next_hop = node;
		
		while(pred != source)	
		{
			next_hop = pred;
			pred = (*table)[pred].first;	
		}

		cout<< node<< " "<< next_hop<< " "<< distance<< endl;			
	}



}



int main(int argc, char *argv[])
{
	Linkstate ls;
	ls.make_graph(argv[1]);	
	ls.print_graph();
	ls.parse_changes(argv[2]);
	ls.find_shortest_path(2);
	ls.print_routing_table(2);
}
