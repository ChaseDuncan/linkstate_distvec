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
			
		pair<int, int> edge_1(ints[2], ints[1]);
		graph[ints[0]].push_back(edge_1);	
		pair<int, int> edge_2(ints[2], ints[0]);
		graph[ints[1]].push_back(edge_2);
	}
}

void Linkstate::find_state()
{
	
	
	
}

void Linkstate::find_shortest_path(int source)
{
	unordered_map<int, bool> visited; //TRUE if visited, FALSE if not
	unordered_map<int, pair<int, int> > * dist=new unordered_map<int, pair<int, int> >;//copy over to &distances[source];
	pair<int, int> srcnode(0, source); //predecessor distance
	(*dist)[source]=srcnode;
	priority_queue<pair<int, int> > pq;
	pq.push(srcnode);
	for(auto val : graph){
		if(val.first!=source){
			pair<int, int> n(INT_MIN+1, val.first);
			pair<int, int>nn(INT_MAX, val.first);
			(*dist)[val.first]=nn;
			pq.push(n); //add all the nodes
		}
	}
	int predecessor=source;
	while(!pq.empty()){
		pair<int, int> curnode = pq.top();
		curnode.first=(-1)*curnode.first;
		pq.pop();
		int curnodeint=curnode.second;
		visited[curnodeint]=true;
		vector<pair<int, int> > * neighbors=&graph[curnodeint];
		for(int i=0;i<neighbors->size();i++){
			int neighborint=(*neighbors)[i].second;
			//default value of bool is false, so don't have to worry about new entry'
			if(!visited[neighborint]){
				int potentialnewdist=INT_MAX;
				if(curnode.first!=INT_MAX){
					potentialnewdist=curnode.first + (*neighbors)[i].first;
				}
				if (potentialnewdist < (*dist)[neighborint].first){
					(*dist)[neighborint].first=potentialnewdist;
					(*dist)[neighborint].second=curnodeint;
					pair<int, int> neighbor_despair((-1)*(*dist)[neighborint].first, neighborint);
					pq.push(neighbor_despair);
				}
			}
		}
	}
	if(distances[source]!=NULL){
		delete distances[source];
		distances[source]=NULL;
	}
	distances[source]=dist;
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
	unordered_map< int, pair<int, int> >* table = distances[source];
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

void Linkstate::print_routing_table()
{
	ofstream myfile;
	myfile.open("output.txt");

	//go through all nodes
	for(auto source:distances){
		int sourceint=source.first;
		//need this to put the nodes on
		priority_queue<int> pq;
		//go through all destination
		for(auto dest:(*source.second)){
			int destint=dest.first;
			pq.push((-1)*destint);
		}
		while(!pq.empty()){
			int destint=(-1)*pq.top();
			pq.pop();
			int curhop=destint;
			int prevcurhop=curhop;
			int totaldist=(*source.second)[destint].first;
			//go backwards
			while(curhop!=sourceint){
				prevcurhop=curhop;
				curhop=(*source.second)[curhop].second;
			}
			myfile<<destint<<" "<<prevcurhop<<" "<<totaldist<<"\n";
		}
	}
	myfile.close();
}


int main(int argc, char *argv[])
{
	Linkstate ls;
	ls.make_graph(argv[1]);	
	//ls.print_graph();
	ls.parse_changes(argv[2]);
	ls.find_shortest_path(2);
	//ls.print_routing_table(2);
	ls.print_routing_table();
}
