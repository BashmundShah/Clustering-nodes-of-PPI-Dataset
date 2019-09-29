#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

using namespace std;

//Struct Declaration
struct edges
{
	int val=0; //shows connection 
	int weight=0; //weight of the edge
};

struct node
{
	bool cluster=false; //Check whether the given node is part of Cluster or not. Initially, false!
	int index=0; 
	int degree=0; 
	int weight=0; 
};

//Function Declaration
int getNoOfEdges();
int getNoOfNodes();

double density(int, int);
double clusterProperty(int, int, double);

void sortPriorityW(node *x,int size);
void sortPriorityD(node *x,int size);

int maxWeight(node *,int);
int maxDegree(node *,int);

//Driver Function
int main()
{
    int n=getNoOfNodes();
	int e=getNoOfEdges();
	
	node nodes[n]; //Nodes of the Graph
	node nodes_[n]; //Nodes of the Cluster also known as "Nodes Dash"
	
	edges G[n][n]; //Edges of the Graph	
	edges G_[n][n]; //Edges of the Cluster

	//Reading the File to get the Nodes and their Connections
	ifstream file("Temp1.txt");
	int x1,x2;
	float w;
	while(file>>x1)
	{
		file>>x2;
		file>>w;
		G[x1-1][x2-1].val=1;
		G[x1-1][x2-1].weight=w;
		G[x2-1][x1-1].val=1;
		G[x2-1][x1-1].weight=w;
	}
	file.close();
	
	//Assigning degree and weight of the nodes
	double d_=0.0,d=1.0,cp_=0.0,cp=1.0;
	cout<<"Enter a threshold density(0<=d<=1): ";
	cin>>d_;
	cout<<"Enter a threshold cluster property(0<=cp<=1) ";
	cin>>cp_;
	system("cls");
	int max_w,max_d,noOfFiles=1;
	do
	{
		//finding weight of edges and nodes.Assigning them to their respective nodes
		cout<<"Finding Weight ...\n";
		cout<<"Assigning degree and weight ...\n";
		cout<<"Creating Cluster...\n\n";
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
				G[i][j].weight=0;
				
				if(i==j)
				continue;
				
				for(int k=0;k<n;k++)
				{
					G[i][j].weight+=(G[i][k].val*G[k][j].val);
				}
				nodes[i].index=i;
				nodes[i].degree+=G[i][j].val;
				nodes[i].weight+=G[i][j].weight;
			}
		}
		int noOfNodes=0,noOfEdges=0;
		max_w=maxWeight(nodes,n);
		max_d=maxDegree(nodes,n);
	
		//Priority nodes
		node temp[n];	
		for(int i=0;i<n;i++)
		{
				temp[i].degree=nodes[i].degree;
				temp[i].weight=nodes[i].weight;
				temp[i].index=i;
		}
		if(max_w>0)
		{
			sortPriorityW(temp,n);
		}
		else if(max_d>0)
		{
			sortPriorityD(temp,n);
		}
		int index=temp[0].index;
		nodes[index].cluster=nodes_[index].cluster=true;
		noOfNodes=1;		
		for(int i=0;i<n;i++)
		{
			index=temp[i].index;
			for(int j=0;j<n;j++)
			{
				if(nodes[index].cluster==false)
				{
					nodes_[index].index=nodes_[index].degree=nodes_[index].weight=0;
					continue;
				}
				nodes[index].cluster=nodes_[index].cluster=true;
				nodes_[index]=nodes[index];				
				if(G[index][j].val==1 and nodes[j].cluster==false)
				{
					noOfNodes++;
					int e=0;
					nodes_[j]=nodes[j];
					nodes_[j].cluster=nodes[j].cluster=true;
					for(int k=0;k<n;k++)
					{
						if(G[j][k].val==1 and nodes_[k].cluster==true)
						{
							e++;
						}
					}
					noOfEdges+=e;
					d=density(noOfEdges,noOfNodes);
					cp=clusterProperty(e,noOfNodes-1,d);
					if(d<d_ or cp<cp_)
					{
						nodes_[j].weight=0;
						nodes_[j].degree=0;
						nodes_[j].index=0;
						nodes_[j].cluster=nodes[j].cluster=false;
						noOfEdges-=e;
						noOfNodes--;
					}
				}	
			}
		}
		string filename="\0";
		ofstream files;
		stringstream a;
		a<<noOfFiles;
		filename = "Clusters/Cluster#" + a.str();
	    filename += ".txt";
	    files.open(filename.c_str(), ios::out);
	    files<<"Cluster: \n";
	    bool c=false;
		for(int i=0;i<n;i++)
		{
			for(int j=i+1;j<n;j++)
			{
				if(G[i][j].val==1 and nodes[i].cluster==true and nodes[j].cluster==true) 
				{
					files<<i+1<<"\t"<<j+1<<"\t"<<G[i][j].weight<<endl;
					c=true;
				}
			}
		}
		for(int i=0;i<n;i++)
		{
			if(!c && nodes_[i].weight!=0 and nodes_[i].degree!=0)
			{
				files<<nodes_[i].index+1<<"\tNo Connection\n";
			}
		}
		files.close();
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
				if(nodes[i].cluster==true)
				{
					nodes[i].degree=nodes[i].index=nodes[i].weight=0;
					G[i][j].val=G[i][j].weight=0;
					G[j][i].val=G[j][i].weight=0;
				}
			}
		}
		for(int i=0;i<n;i++)
		{
			if(nodes[i].cluster==true)
			{
				nodes_[i].cluster=nodes[i].cluster=false;
			}
		}
		max_w=maxWeight(nodes,n);
		max_d=maxDegree(nodes,n);
		noOfFiles++;
	}
	while(max_w>0 or max_d>0);
	cout<<"Cluster(s) Created: "<<--noOfFiles<<endl;
	return 0;
}
//Functions Definitions
int maxWeight(node *n,int no)
{
	int max=n[0].weight;
	for(int i=0;i<no;i++)
	{
		if(max<n[i].weight)
		{
			max=n[i].weight;
		}
	}
	return max;
}
int maxDegree(node *n,int no)
{
	int max=n[0].degree,index=0;
	for(int i=0;i<no;i++)
	{
		if(max<=n[i].degree)
		{
			max=n[i].degree;
		}
	}
	return max;
}
int getNoOfNodes()
{
	ifstream file("Temp1.txt");
	int x1,x2,max=0;
	float w;
	while(file>>x1)
	{
		file>>x2;
		file>>w;
		if(x1>max)
		max=x1;
		if(x2>max)
		max=x2;
	}
	file.close();
	return max;
}
int getNoOfEdges()
{
	ifstream file;
	file.open("Temp1.txt");
	int e=0,i;
	float x;
	while(file>>i)
	{
		file>>i;
		file>>x;
		e++;
	}
	file.close();
	return e;
}
void sortPriorityW(node x[],int size)
{
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
		{
			if(x[i].weight>x[j].weight)
			{
				node temp;
				temp=x[i];
				x[i]=x[j];
				x[j]=temp;
			}
		}
	}
}
void sortPriorityD(node x[],int size)
{
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
		{
			if(x[i].degree>x[j].degree)
			{
				node temp;
				temp=x[i];
				x[i]=x[j];
				x[j]=temp;
			}
		}
	}
}
double clusterProperty(int Ec, int Nc, double d)
{
	if(Nc==0 or d==0)
	return 0.0;
	
	return Ec/(d*Nc);
}
double density(int e, int n)
{
	if(n<2)
	return 0;
	
	return (e*2.0)/(n*(n-1.0));
}
