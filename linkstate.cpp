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

void Linkstate::output_to_file(ofstream & output, string message_file)
{
	print_routing_table(output);
	print_message_path(output, message_file);
}

void Linkstate::print_message_path(ofstream & myfile, string message_file)
{
	ifstream infile(message_file);
	string line;
	while(getline(infile, line)){
		size_t srcidx=line.find(" ");
		string srcstr=line.substr(0, srcidx);
		int srcint=atoi(srcstr.c_str());
		size_t destidx=line.find(" ", srcidx+1); //have to add one otherwise find same space
		string deststr=line.substr(srcidx+1, destidx-1);
		int destint=atoi(deststr.c_str());
		string message=line.substr(destidx+1);
		myfile<<"from "<<srcint<<" to "<<destint<<" hops";
		vector<int> route=routing_table[srcint][destint];
		for(int i=0;i<route.size()-1;i++){
			myfile<<" "<<route[i];
		}
		myfile<<" message "<<message<<"\n";
	}

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

void Linkstate::update_graph(int ithchange)
{
	int source=get<0>(changes[ithchange]);
	int dest=get<1>(changes[ithchange]);
	int newcost=get<2>(changes[ithchange]);
	vector<pair<int, int> > vec = graph[source];
	//deleted a node
	if(newcost==-999){
		for(int i=0;i<(graph[source]).size();i++){
			//found edge, delete it
			if(graph[source][i].second==dest){
				cout<<"found edge between: "<<source<<" dest: "<<graph[source][i].first<<" updating cost: "<<graph[source][i].second<<endl;	
				graph[source].erase((graph[source]).begin()+i);
			}
		}
		//have to do the same for other edge
		for(int i=0;i<graph[dest].size();i++){
			if(graph[dest][i].second==source){
				graph[dest].erase((graph[dest]).begin()+i);
			}
		}
	}
	//either insert a new edge or update weight
	if(newcost!=-999){
		bool found=false;
		for(int i=0;i<vec.size();i++){
			//found edge, update it
			if(graph[source][i].second==dest){
				found=true;
				graph[source][i].second=newcost;
			}
		}
		if(!found){
			pair<int, int> p(newcost, dest);
			graph[source].push_back(p);
			pair<int, int> p2(newcost, source);
			graph[dest].push_back(p2);
		}
		//have to do the same for other edge
		else if(found){
			for(int i=0;i<graph[dest].size();i++){
				if(graph[dest][i].second==source){
					graph[dest][i].second=newcost;
				}
			}
		}	
	}
}


int  Linkstate::parse_changes(string changesfile)
{
	int num_changes=0;
	ifstream infile(changesfile);
	string line;
	int ints[3];
	//vector<tuple<int, int, int> > tempchanges;
	queue<tuple<int, int, int> >tempchanges;
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
		tempchanges.push(change);
	}
	//reverse the order, push into changes
	while(!tempchanges.empty()){
		num_changes+=1;
		tuple<int, int, int> temp=tempchanges.front();
		changes.push_back(temp);
		tempchanges.pop();
	}
	return num_changes;
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
		for(int i = 0; i < vec.size(); i++)
		{
			int dest = vec[i].first;
			int wgt = vec[i].second;
			cout<< src <<" "<<dest <<" "<< wgt << endl;
		}
	}
}

void Linkstate::print_routing_table(ofstream & myfile)
{
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
			//for filling out the routing table
			stack<int> backward_route;
			//go backwards
			while(curhop!=sourceint){
				backward_route.push(curhop);
				prevcurhop=curhop;
				curhop=(*source.second)[curhop].second;
			}
			//clear the vector as this might be the second call, for changes
			routing_table[sourceint][destint].clear();
			//push on the source as that's the "first hop"'
			routing_table[sourceint][destint].push_back(sourceint);
			while(!backward_route.empty()){
				int nextnode=backward_route.top();
				cout<<"sourceint: "<<sourceint<<" nextnode: "<<nextnode<<endl;
				backward_route.pop();
				routing_table[sourceint][destint].push_back(nextnode);
			}
			myfile<<destint<<" "<<prevcurhop<<" "<<totaldist<<"\n";
		}
		myfile<<"\n"<<endl;
	}
}

void Linkstate::populate_distances()
{
	//put in priority queue just so we can have correct output
	priority_queue<int> pq;
	for(auto node : graph){
		pq.push((node.first));
	}
	while(!pq.empty()){
		int curnode=(pq.top());
		pq.pop();
		find_shortest_path(curnode);
	}
}
int main(int argc, char *argv[])
{
	Linkstate ls;
	ofstream output;
	output.open("output.txt");
	ls.make_graph(argv[1]);	
	ls.print_graph();
	int num_changes=ls.parse_changes(argv[2]);
	ls.populate_distances();
	ls.output_to_file(output, argv[3]);
	//do all again for each change
	for(int i=0;i<num_changes;i++){
		cout<<"after "<<i<<"th change: "<<endl;
		ls.print_graph();
		ls.update_graph(i);
		ls.populate_distances();
		ls.output_to_file(output, argv[3]);
	}
	output.close();
}
