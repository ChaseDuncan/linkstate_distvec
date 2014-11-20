#include "distvec.h"

using namespace std;

void Distvec::make_graph_and_list_edges(string topofile)
{
	ifstream infile(topofile);
	string line;
	int ints[3];

	while(getline(infile, line))
	{
		istringstream iss(line);
		int number;
		int idx=0;
		
		while(iss >> number)
		{
			ints[idx] = number;
			idx++;
		}

		pair<int, int> edge_1(ints[2], ints[1]);
		tuple<int, int, int> edge_1a(ints[0], ints[2], ints[1]);
		graph[ints[0]].push_back(edge_1);
		pair<int, int> edge_2(ints[2], ints[0]);
		tuple<int, int, int> edge_2a(ints[1], ints[2], ints[0]);
		graph[ints[1]].push_back(edge_2);
		edges.push_back(edge_1a);
		edges.push_back(edge_2a);
	}
}

int Distvec::parse_changes(string changesfile)
{
	int num_changes=0;
	ifstream infile(changesfile);
	string line;
	int ints[3];
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
		tuple<int, int, int> change(ints[0], ints[1], ints[2]);
		changes.push_back(change);
		num_changes+=1;
	}
	return num_changes;
}

void Distvec::bellman_ford(int source)
{
	//cout<<"beginning of bellman"<<endl;
	unordered_map<int, pair<int, int> > * dist = new unordered_map<int, pair<int, int> >;
	//go through graph and initialize vertices
	int vertices_size=0;
	for(auto val : graph){
		if(val.first==source){
			pair<int, int> src(0, val.first); //dist, predecessor
			(*dist)[val.first]=src;
		}
		else{
			pair<int, int> inf(INT_MAX, -1);
			(*dist)[val.first]=inf;
		}
		vertices_size+=1;
	}
	//"relax the edges"
	for(int i=0;i<vertices_size;i++){
		for(int j=0;j<edges.size();j++){
			int cur_src=get<0>(edges[j]);
			int cur_cost=get<1>(edges[j]);
			int cur_dest=get<2>(edges[j]);
			//if a better path
			if((*dist)[cur_src].first != INT_MAX && (*dist)[cur_src].first+cur_cost<(*dist)[cur_dest].first){
				(*dist)[cur_dest].first=(*dist)[cur_src].first+cur_cost;
				//update predecessor
				(*dist)[cur_dest].second=cur_src;
			}
		}
	}
	if(distances[source]!=NULL){
		delete distances[source];
		distances[source]=NULL;
	}
	distances[source]=dist;
	/*for(auto & it: *dist){
		cout<<"in bellman ford. Source: "<<source<<" dest: "<<it.first<<" distances: "<<it.second.first<<" predecessor: "<<it.second.second<<endl;
	}*/
	//cout<<"end of bellman"<<endl;
}
void Distvec::populate_distances()
{
	//put in priorirty queue so we can go through in the right order
	priority_queue<int> pq;
	for(auto node:graph){
		pq.push((node.first));
	}
	while(!pq.empty()){
		int curnode=pq.top();
		pq.pop();
		bellman_ford(curnode);
	}
}
void Distvec::print_message_path(ofstream & myfile, string message_file)
{
	ifstream infile(message_file);
	string line;
	while(getline(infile, line)){
		size_t srcidx=line.find(" ");
		string srcstr=line.substr(0, srcidx);
		int srcint=atoi(srcstr.c_str());
		size_t destidx=line.find(" ", srcidx+1); //have to add one otherwise we're on the same space
		string deststr=line.substr(srcidx+1, destidx-1);
		int destint=atoi(deststr.c_str());
		string message=line.substr(destidx+1);
		myfile<<"from "<<srcint<<" to "<<destint<<" hops";
		vector<int> route = routing_table[srcint][destint];
		for(int i=0;i<route.size()-1;i++){
			myfile<<" "<<route[i];
		}
		myfile<<" message "<<message<<"\n";
	}
	
}
void Distvec::output_to_file(ofstream & myfile, string message_file)
{
	print_routing_table(myfile);
	print_message_path(myfile,message_file);
	
}

void Distvec::print_routing_table(ofstream & myfile)
{
	for(auto source:distances){
		int sourceint=source.first;
		//need this so we can go through the nodes in the right order
		//even though this isn't dijkstrajs, just using the priority queue
		//since the map is unordered'
		priority_queue<int> pq;
		//go through all the destination 
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
			//fill out the routing table, backtrace
			stack<int> backward_route;
			while(curhop!=sourceint){
				backward_route.push(curhop);
				prevcurhop=curhop;
				curhop=(*source.second)[curhop].second;
				//cout<<"curhop: "<<curhop<<" prevcurhop: "<<prevcurhop<<" sourceint: "<<sourceint<<" destint: "<<destint<<endl;
			}
			//clear vector as this might be the second call after changes
			routing_table[sourceint][destint].clear();
			//push on the source as that's the "first hop"
			routing_table[sourceint][destint].push_back(sourceint);
			while(!backward_route.empty()){
				int nextnode=backward_route.top();
				backward_route.pop();
				routing_table[sourceint][destint].push_back(nextnode);
			}
			myfile<<destint<<" "<<prevcurhop<<" "<<totaldist<<"\n";
		}
		myfile<<"\n"<<endl;
	}
}
void Distvec::update_graph_and_edges(int ithchange)
{
	int source=get<0>(changes[ithchange]);
	int dest=get<1>(changes[ithchange]);
	int newcost=get<2>(changes[ithchange]);
	vector<pair<int, int> > vec=graph[source];
	//deleted a node
	if(newcost==-999){
		for(int i=0;i<(graph[source]).size();i++){
			//found edge, delete it
			if(graph[source][i].second==dest){
				graph[source].erase((graph[source]).begin()+i);
			}
		}
		//do the same for the other edge
		for(int i=0;i<graph[dest].size();i++){
			if(graph[dest][i].second==source){
				graph[dest].erase((graph[dest]).begin()+i);
			}
		}
		//for deubugging
		/*cout<<"in update graph";
		for(int i=0;i<edges.size();i++){
			int src=get<0>(edges[i]);
			int dest=get<2>(edges[i]);
			cout<<"src: "<<src<<" dest: "<<dest<<endl;
		}*/
		delete_edge(source, dest);
	}
	//insert or update weight
	else{
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
		else if(found){
			for(int i=0;i<graph[dest].size();i++){
				if(graph[dest][i].second==source){
					graph[dest][i].second=newcost;
				}
			}
		}
		update_edge(source, dest, newcost);	
	}
}
void Distvec::update_edge(int source, int dest, int newcost)
{
	bool found=false;
	for(int i=0;i<edges.size();i++){
		int cursrc=get<0>(edges[i]);
		int curdest=get<2>(edges[i]);
		if((cursrc==source && curdest==dest)||(cursrc==dest && curdest==source)){
			found=true;
			get<1>(edges[i])=newcost;
		}
	}
	//if adding new edge
	if(!found){
		tuple<int, int, int> a(source, newcost, dest);
		tuple<int, int, int> b(dest, newcost, source);
		edges.push_back(a);
		edges.push_back(b);
	}
}
void Distvec::delete_edge(int source, int dest)
{
	priority_queue<int> tobedeleted;
	//since the graph is undirected, we'll have to delete both sets of edges
	for(int i=0;i<edges.size();i++){
		int cursrc=get<0>(edges[i]);
		int curdest=get<2>(edges[i]);
		if((cursrc==source && curdest==dest) || (cursrc==dest && curdest==source)){
			tobedeleted.push(i);
			//edges.erase((edges.begin()+i));	
		}
		//cout<<"at idx ten: "<<get<0>(edges[10])<<" second: "<<get<2>(edges[10])<<endl;
	}
	while(!tobedeleted.empty()){
		int idx=tobedeleted.top();
		tobedeleted.pop();
		edges.erase((edges.begin()+idx));
	}
}
/*just for testing purposes*/
void Distvec::print_graph()
{
	for(unordered_map<int, vector<pair<int, int> > >::iterator it = graph.begin(); it != graph.end(); it++)
	{
		int src = it->first;
		vector<pair<int, int> > vec = it->second;
		for(int i=0;i<vec.size();i++)
		{
				int dest = vec[i].first;
				int wgt = vec[i].second;
				cout<< src<<" "<<dest << " "<< wgt<<endl;
		}
	}
}
/*just for testing purposes*/
void Distvec::print_edges()
{	
	for(int i=0;i<edges.size();i++)
	{
		int src=get<0>(edges[i]);
		int cost=get<1>(edges[i]);
		int dest=get<2>(edges[i]);
		cout<<"source: "<<src<<" cost: "<<cost<<" dest: "<<dest<<endl;
	}
}
int main(int argc, char *argv[])
{
	Distvec dv;
	ofstream output;
	output.open("output.txt");
	dv.make_graph_and_list_edges(argv[1]);
	//cout<<"print graph:"<<endl;
	//dv.print_graph();
	int num_changes=dv.parse_changes(argv[3]);
	dv.populate_distances();
	dv.output_to_file(output, argv[2]);
	//do everything again for each change
	for(int i=0;i<num_changes;i++){
		cout<<"print graph: "<<endl;
		dv.print_graph();
		//cout<<"print edges: "<<endl;
		//dv.print_edges();
		dv.update_graph_and_edges(i);
		dv.populate_distances();
		dv.output_to_file(output, argv[2]);
	}
	//cout<<"final graph: "<<endl;
	//dv.print_graph();
	//cout<<"edges: "<<endl;
	//dv.print_edges();
	output.close();
	
}
