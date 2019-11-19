# include <iostream>
# include <vector>
# include <string>
# include <set>
# include <queue>
# include <utility>
# include <algorithm>
using namespace std;

//Class Definitions--------------------------------------------------------------------------

class lan{
	public:
		char id;
		int DP;
		vector<int> adj_bridges;
		vector<int> hosts;
		lan()
		{
			id = '\0';
			DP = -1;
		}
};



class bridge{
	public:
		int id;
		int root;
		int root_distance;
		vector<char> adj_lans;
		//vector<ftable> forwarding_table;
		pair<char,int> RP;
		bridge()
		{
			id = -1;
			root = id;
			RP = make_pair('\0',-1);
			root_distance = -1;
		}
};



class message{
	public:
		int root;
		int dist;
		bridge source;
		int destination;
		char lan;
		message()
		{
			root = -1;
			dist = -1;
			destination = -1;
			lan = '\0';
		}
};

struct messageComparer
{
    bool operator()(message const &j1, message const &j2)
    {
    	if(j1.destination < j2.destination) return true;
    	else return false;
        //compare j1 and j2 and return true or false
    }
};

class traces{
	public:
		int time;
		int bridge;
		char status;
		message m;
};
/*
class data_packet{
public:
	int source;
	int destination;
	char prev;
	int bridge;
};*/

//--------------------------------------------------------------------------------------------

//Helper Functions-----------------------------------------------------------------------------

void print_bridge_network(vector<bridge> network)
{
	for(int i=0; i<network.size();i++)
	{
		cout<<network[i].id<<"\t";
		for(int j=0; j<network[i].adj_lans.size();j++)
			cout<<network[i].adj_lans[j]<<" ";
		cout<<endl;
	}
}

void print_lan_network(vector<lan> network)
{
	for(int i=0; i<network.size();i++)
	{
		cout<<network[i].id<<"\t";
		for(int j=0; j<network[i].adj_bridges.size();j++)
			cout<<network[i].adj_bridges[j]<<" ";
		cout<<network[i].DP<<endl;
	}
}

typedef set<message,messageComparer> messageSet;

messageSet SendMessage(message m,vector<bridge> bridge_network, vector<lan> lan_network)
{
	messageSet messages;
	int root = m.root;
	int d = m.dist;
	bridge source = m.source;
	for(int i=0; i<bridge_network.size();i++)	//in bridge n/w
	{
		printf("hey1");
		if(source.id == bridge_network[i].id)	//if
		{
			printf("hey2");
			for(int j=0; j<bridge_network[i].adj_lans.size(); j++)
			{
				printf("hey3");
				for(int k=0; k<lan_network.size(); k++)
				{
					printf("hey4");
					if(bridge_network[i].adj_lans[j] == lan_network[k].id)
					{
						printf("hey5");
						for(int p=0; p<lan_network[k].adj_bridges.size();p++)
						{
							printf("hey6");
							if(lan_network[k].adj_bridges[p] != source.id)
							{
								printf("hey7");
								message ms;
								ms.root = root;
								ms.dist = d;
								ms.source = source;
								ms.destination = lan_network[k].adj_bridges[p];
								ms.lan = lan_network[k].id;
								messages.insert(ms);
							}
						}
							
					}
				}
			}
		} 
			
	}
	return messages;
}

message UpdateConfig(message m,vector<bridge>& bridge_network)
{
	int root = m.root;
	int d = m.dist;
	bridge source = m.source;
	int destination = m.destination;
	char lan = m.lan;

	message return_message;

	for(int i=0; i<bridge_network.size();i++)
	{
		if(destination == bridge_network[i].id)
		{
			bridge b = bridge_network[i];
			if(root < b.root)
			{   
                cout<<"updating the table";
				return_message.root = root;
				return_message.dist = d+1;
				return_message.source=bridge_network[i];
				bridge_network[i].root = root;
				bridge_network[i].RP = make_pair(lan,source.id);
				bridge_network[i].root_distance = d+1;
			}
			else if(root == b.root && d+1 < bridge_network[i].root_distance)
			{
                cout<<"updating the table ";
				return_message.root = root;
				return_message.dist = d+1;
				return_message.source=bridge_network[i];
				bridge_network[i].root = root;
				bridge_network[i].RP = make_pair(lan,source.id);
				bridge_network[i].root_distance = d+1;
			}
			else if (root == b.root && d+1 == bridge_network[i].root_distance && source.id<bridge_network[i].RP.second)
			{
                cout<<"updating the table";
				return_message.root = root;
				return_message.dist = d+1;
				return_message.source=bridge_network[i];
				bridge_network[i].root = root;				
				bridge_network[i].RP = make_pair(lan,source.id);
				bridge_network[i].root_distance = d+1;
			}
			else
			{
				return_message.root = -1;
				return_message.dist = d+1;
				return_message.source=bridge_network[i];
			}
		}
	}

	return return_message;
}

/*lan lookIntoTable(vector<ftable> forwarding_table, int d)
{
	lan l;
	l.id='\0';
	for(int i=0; i<forwarding_table.size();i++)
	{
		ftable f = forwarding_table[i];
		if(f.host == d) l = f.fport;
	}
	return l;
}*/
//--------------------------------------------------------------------------------------------------------

int main(){
	int tr=1;
	int n;
	cin >> n;
	vector<bridge> bridge_network;
	set<char> lan_set;
	
	// Initializing the bridges
	for(int i=0; i<=n; i++)
	{
		printf("hey1");
		bridge b;
		string line;
		getline(cin,line);  // getting line by line i/p
		if(i!=0)
		{
			for(int j=0; j<line.size(); j++)
			{
				if(j==1)
				{
					printf("hey2");
					if(line[j+1]!=':') // check for colon
					{
						printf("hey4");	
						b.id = 10*((int) line[j] -48) + ((int) line[j+1] -48);
						b.root = b.id;
						j++;
					}
					else			//set id
					{
						printf("hey5");
						b.id = (int) line[j] - 48;
						b.root = b.id;
					}
				}
				if(j!=0 & j!=1 & j!=2)
				{
					printf("hey3");
					if(line[j] != ' ' and line[j]!=':') 
					{
						printf("hey6");
						b.adj_lans.push_back(line[j]);		// push for adjacent lans
						lan_set.insert(line[j]);			// push to make a lan set
					}
				}
			}
			sort(b.adj_lans.begin(),b.adj_lans.end());
			bridge_network.push_back(b);		// make a bridge network
		}
	}

	cout<<"Initial Bridge Setup"<<endl;
	print_bridge_network(bridge_network); //printing bridge network

	cout<<"Inputting bridges done, now"<<endl;

	vector<lan> lan_network;

	while(!lan_set.empty())  //creating a lan network
	{
		char c = *lan_set.begin();
		printf("%c",c);
		lan l;
		l.id = c;
		for(int i=0; i<bridge_network.size();i++)
			for(int j=0; j<bridge_network[i].adj_lans.size(); j++)
				if (bridge_network[i].adj_lans[j] == c) l.adj_bridges.push_back(bridge_network[i].id);
		lan_network.push_back(l);  //pushing in lan network
		lan_set.erase(lan_set.begin());	//removing from lan set
	}

	print_lan_network(lan_network); //print lan network

	// Implementing spanning tree protocol

	queue<message> spawned,received;  // making a spawned and recieved 
	vector<class traces> traces_queue;
	int timestamp = 0,initial=1;

	for(int i=0; i<bridge_network.size();i++)  //initialize config msg
	{
		message m;
		m.root = bridge_network[i].id;
		m.dist=0;
		m.source=bridge_network[i];
		spawned.push(m);
	}

	while(!spawned.empty())
	{
		if(initial != 1)
		{
			while(!spawned.empty())
			{
				spawned.pop();
				//cout<<"spawned emptied"<<endl;
			}
		}

		while(!received.empty())
		{
			message m = received.front();
			cout<<"Plucked out from received"<<endl;
			message to_be_published = UpdateConfig(m, bridge_network);
			if(to_be_published.root != -1) 
			{
				spawned.push(to_be_published);
				cout<<"pushed in spawned"<<endl;
			}
			traces t;
			t.time = timestamp;
			t.bridge = m.destination;
			t.status = 'r';
			t.m = m;
			if(tr==1) cout<<t.time<<" B"<<t.bridge<<" "<<t.status<<" (B"<<t.m.root<<" "<<t.m.dist<<" B"<<t.m.source.id<<")\n";
			traces_queue.push_back(t);
			received.pop();
			cout<<"receive trace added"<<endl;
		}

		queue<message> temp;
		while(!spawned.empty())
		{
			message m = spawned.front();
			cout<<"Plucked out from spawned"<<endl;
			messageSet received_by_set = SendMessage(m, bridge_network, lan_network);
			spawned.pop();
			while(!received_by_set.empty())
			{
				received.push(*received_by_set.begin());
				received_by_set.erase(received_by_set.begin());
			}
			traces t;
			t.time = timestamp;
			t.bridge = m.source.id;
			t.status = 's';
			t.m = m;
			traces_queue.push_back(t);
			if(tr==1) cout<<t.time<<" B"<<t.bridge<<" "<<t.status<<" (B"<<t.m.root<<" "<<t.m.dist<<" B"<<t.m.source.id<<")\n";
			temp.push(m);
		}

		while(!temp.empty())
		{
			spawned.push(temp.front());
			temp.pop();
		}

		timestamp++;
		initial = 0;

	}

	for(int i=0; i<lan_network.size(); i++)
	{
		lan_network[i].DP = *min_element(begin(lan_network[i].adj_bridges),end(lan_network[i].adj_bridges));
	}

	cout<<endl<<"DP for lans"<<endl;
	print_lan_network(lan_network);

	for(int i=0; i<bridge_network.size(); i++)
	{
		cout<<"B"<<bridge_network[i].id<<":";
		for(int j=0; j<bridge_network[i].adj_lans.size(); j++)
		{
			int flag=0;
			char c = bridge_network[i].adj_lans[j];
			cout<<" "<<c<<"-";
			if(c == bridge_network[i].RP.first) {cout<<"RP";flag=1;}
			for(int k=0;k<lan_network.size();k++)
			{
				if(c==lan_network[k].id && bridge_network[i].id==lan_network[k].DP && flag==0)
					{cout<<"DP";flag=1;break;}
			}
			if(flag==0) 
			{
				cout<<"NP";
				bridge_network[i].adj_lans.erase(remove(bridge_network[i].adj_lans.begin(), bridge_network[i].adj_lans.end(), c), bridge_network[i].adj_lans.end());
				j--;
			}
		}
		cout<<endl;
	}

	cout<<"Updating Lan network"<<endl<<endl;

	for(int i=0; i<lan_network.size();i++)
	{
		char c = lan_network[i].id;
		vector<int> l;
		for(int i=0; i<bridge_network.size();i++)
			for(int j=0; j<bridge_network[i].adj_lans.size(); j++)
				if (bridge_network[i].adj_lans[j] == c) l.push_back(bridge_network[i].id);
		lan_network[i].adj_bridges = l;
	}
	cout<<"Final Bridge Setup"<<endl;
	print_bridge_network(bridge_network); 
}
