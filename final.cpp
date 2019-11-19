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
		pair<char,int> RP;
		bridge()
		{
			id = -1;
			root = id;
			RP = make_pair('\0',-1);
			root_distance = -1;
		}
};


class config_msg{
	public:
		int root;
		int dist;
		bridge source;
		int destination;
		char lan;
		config_msg()
		{
			root = -1;
			dist = -1;
			destination = -1;
			lan = '\0';
		}
};




struct compare_func
{
    bool operator()(config_msg const &j1, config_msg const &j2)
    {
    	if(j1.destination < j2.destination) return true;
    	else return false;
        //compare j1 and j2 and return true or false
    }
};

class netw_info{
	public:
		int time;
		int bridge;
		char status;
		config_msg m;
};


//--------------------------------------------------------------------------------------------

//Helper Functions-----------------------------------------------------------------------------

void print_bridge_network(vector<bridge> network)
{
	cout<<"Bridge NO.\t"<<"Adjacent Lans"<<endl;
	for(int i=0; i<network.size();i++)
	{
		cout<<network[i].id<<"\t\t";
		for(int j=0; j<network[i].adj_lans.size();j++)
			cout<<network[i].adj_lans[j]<<" ";
		cout<<endl;
	}
}

void print_lan_network(vector<lan> network)
{
	cout<<"Lan No.\t\t"<<"Adj Bridges\t"<<"Designated Bridge"<<endl;
	for(int i=0; i<network.size();i++)
	{
		cout<<network[i].id<<"\t\t";
		for(int j=0; j<network[i].adj_bridges.size();j++)
			cout<<network[i].adj_bridges[j]<<" ";
		cout<<"\t\t"<<network[i].DP<<endl;
	}
}

typedef set<config_msg,compare_func> messageSet;

messageSet SendMessage(config_msg m,vector<bridge> bridge_network, vector<lan> lan_network)
{
	messageSet messages;
	int root = m.root;
	int d = m.dist;
	bridge source = m.source;
	for(int i=0; i<bridge_network.size();i++)	//in bridge n/w
	{
		if(source.id == bridge_network[i].id)	//if
		{
			for(int j=0; j<bridge_network[i].adj_lans.size(); j++)
			{
				for(int k=0; k<lan_network.size(); k++)
				{
					if(bridge_network[i].adj_lans[j] == lan_network[k].id)
					{
						for(int p=0; p<lan_network[k].adj_bridges.size();p++)
						{
							if(lan_network[k].adj_bridges[p] != source.id)
							{
								config_msg ms;
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

config_msg UpdateConfig(config_msg m,vector<bridge>& bridge_network)
{
	int root = m.root;
	int d = m.dist;
	bridge source = m.source;
	int destination = m.destination;
	char lan = m.lan;

	config_msg return_message;

	for(int i=0; i<bridge_network.size();i++)
	{
		if(destination == bridge_network[i].id)
		{
			bridge b = bridge_network[i];
			if(root < b.root)
			{   
                //cout<<"updating the table";
				return_message.root = root;
				return_message.dist = d+1;
				return_message.source=bridge_network[i];
				bridge_network[i].root = root;
				bridge_network[i].RP = make_pair(lan,source.id);
				bridge_network[i].root_distance = d+1;
			}
			else if(root == b.root && d+1 < bridge_network[i].root_distance)
			{
                //cout<<"updating the table ";
				return_message.root = root;
				return_message.dist = d+1;
				return_message.source=bridge_network[i];
				bridge_network[i].root = root;
				bridge_network[i].RP = make_pair(lan,source.id);
				bridge_network[i].root_distance = d+1;
			}
			else if (root == b.root && d+1 == bridge_network[i].root_distance && source.id<bridge_network[i].RP.second)
			{
                //cout<<"updating the table";
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
		bridge b;
		string line;
		getline(cin,line);  // getting line by line i/p
		if(i!=0)
		{
			for(int j=0; j<line.size(); j++)
			{
				if(j==1)
				{
					if(line[j+1]!=':') // check for colon
					{
						b.id = 10*((int) line[j] -48) + ((int) line[j+1] -48);
						b.root = b.id;
						j++;
					}
					else			//set id
					{
						b.id = (int) line[j] - 48;
						b.root = b.id;
					}
				}
				if(j!=0 & j!=1 & j!=2)
				{
					if(line[j] != ' ' and line[j]!=':') 
					{
						b.adj_lans.push_back(line[j]);		// push for adjacent lans
						lan_set.insert(line[j]);			// push to make a lan set
					}
				}
			}
			sort(b.adj_lans.begin(),b.adj_lans.end());
			bridge_network.push_back(b);		// make a bridge network
		}
	}

	cout<<"-----------------------------Bridge Representation-------------------------------"<<endl;
	print_bridge_network(bridge_network); //printing bridge network

	cout<<"-----------------------------LAN Representation-------------------------------"<<endl;

	vector<lan> lan_network;

	while(!lan_set.empty())  //creating a lan network
	{
		char c = *lan_set.begin();
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

	queue<config_msg> sender_queue,reciever_queue;  // making a sender_queue and recieved 
	vector<class netw_info> netw_info_queue;
	int timestamp = 0,initial=1;

	for(int i=0; i<bridge_network.size();i++)  //initialize config msg
	{
		config_msg m;
		m.root = bridge_network[i].id;
		m.dist=0;
		m.source=bridge_network[i];
		sender_queue.push(m);
	}

	while(!sender_queue.empty())
	{
		cout<<endl<<"time\t"<<"Bridge\t"<<"status\t"<<"sent/recieved msg"<<endl;
		if(initial != 1)
		{
			while(!sender_queue.empty())
			{
				sender_queue.pop();
			}
		}
		while(!reciever_queue.empty())
		{
			config_msg m = reciever_queue.front();
			config_msg to_be_published = UpdateConfig(m, bridge_network);
			if(to_be_published.root != -1) 
			{
				sender_queue.push(to_be_published);
				cout<<"Compare and updated"<<endl;
			}
			netw_info t;
			t.time = timestamp;
			t.bridge = m.destination;
			t.status = 'r';
			t.m = m;
			if(tr==1) cout<<t.time<<"\tB"<<t.bridge<<"\t"<<t.status<<"\t(B"<<t.m.root<<","<<t.m.dist<<",B"<<t.m.source.id<<")\n";
			netw_info_queue.push_back(t);
			reciever_queue.pop();
		}

		queue<config_msg> temp;

		while(!sender_queue.empty())
		{
			config_msg m = sender_queue.front();
			messageSet reciever_queue_by_set = SendMessage(m, bridge_network, lan_network);
			sender_queue.pop();
			while(!reciever_queue_by_set.empty())
			{
				reciever_queue.push(*reciever_queue_by_set.begin());
				reciever_queue_by_set.erase(reciever_queue_by_set.begin());
			}
			netw_info t;
			t.time = timestamp;
			t.bridge = m.source.id;
			t.status = 's';
			t.m = m;
			netw_info_queue.push_back(t);
			if(tr==1)
			{
				cout<<t.time<<"\tB"<<t.bridge<<"\t"<<t.status<<"\t(B"<<t.m.root<<","<<t.m.dist<<",B"<<t.m.source.id<<")\n";
			}
			temp.push(m);
		}

		while(!temp.empty())
		{
			sender_queue.push(temp.front());
			temp.pop();
		}

		timestamp++;
		initial = 0;

	}

	for(int i=0; i<lan_network.size(); i++)  //deciding the designated ports
	{
		lan_network[i].DP = *min_element(begin(lan_network[i].adj_bridges),end(lan_network[i].adj_bridges));
	}

	cout<<endl<<"-----------------------------DP for LAN-------------------------------"<<endl;
	print_lan_network(lan_network);
	cout<<endl;

	cout<<"-----------------------------Updating LANs-------------------------------"<<endl<<endl;
	
	for(int i=0; i<lan_network.size();i++)
	{
		char c = lan_network[i].id;
		vector<int> l;
		for(int i=0; i<bridge_network.size();i++)
			for(int j=0; j<bridge_network[i].adj_lans.size(); j++)
				if (bridge_network[i].adj_lans[j] == c) l.push_back(bridge_network[i].id);
		lan_network[i].adj_bridges = l;
	}
	cout<<"-----------------------------Final Bridge Representation-------------------------------"<<endl;
	print_bridge_network(bridge_network); 
}
