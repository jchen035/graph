#include<iostream>
#include<fstream>
#include<string>

using namespace std;
#define INF 100000000

class Graph;
class Edge;
class Vertex
{
private:
	int index;
	int distance;
	Edge *head; //linked list of edges
	Vertex *source;
	Vertex *next;
	bool visited;
public:
	Vertex()
	{
		head=NULL;
		source=NULL;
		distance=INF;
		visited=false;
	}
	friend class Graph;	
};




class Edge
{
private:
	int weight;
	Vertex *start,*finish;
	Edge *next;
public:	
	friend class  Graph;
};


class Graph
{
private:
	int size;
	Vertex **vertexes;
	Vertex *vertexQueue;
	void insertVertex(Vertex *&current,Vertex *newVertex);
	void removeVertex(Vertex *&current,Vertex *newVertex);
	void printPath(Vertex *current);
public:
	Graph(int size);
	void addEdge(int start,int finish,int weight);
	void enqueueVertex(Vertex *vertex);
	void updatePriority(Vertex *vertex);
	void dijkstra();
};


void Graph::enqueueVertex(Vertex *vertex)
{
	insertVertex(vertexQueue,vertex);
}


void Graph::updatePriority(Vertex *vertex)
{
	removeVertex(vertexQueue,vertex);
	insertVertex(vertexQueue,vertex);
}


void Graph::insertVertex(Vertex *&current,Vertex *newVertex)
{
	if(current==NULL)
	{
		current=newVertex;
		newVertex->next=NULL;
	}
	else if(current->distance>newVertex->distance)
	{
		newVertex->next=current;
		current=newVertex;
	}
	else
	{
		insertVertex(current->next,newVertex);
	}
}

void Graph::removeVertex(Vertex *&current,Vertex *newVertex)
{
	if(current==NULL)
	{
		return;
	}
	else if(current==newVertex)
	{
		current=current->next;
	}
	else
	{
		removeVertex(current->next,newVertex);
	}
}


Graph::Graph(int size)
{
	vertexes=new Vertex*[size];
	this->size=size;
	vertexQueue=NULL;
	for(int i=0;i<size;i++)
	{
		vertexes[i]=new Vertex();
		vertexes[i]->index=i;		
	}
}



void Graph::addEdge(int start,int finish,int weight)
{
	Edge *newEdge=new Edge();
	newEdge->finish=vertexes[finish];
	newEdge->start=vertexes[start];
	newEdge->next=newEdge->start->head;
	newEdge->start->head=newEdge;
	newEdge->weight=weight;
}

void Graph::printPath(Vertex *current)
{
	if(current->source!=NULL)
	{
		printPath(current->source);
	}
	cout<<(char)('A'+current->index)<<" ";
}

void Graph::dijkstra()
{
	vertexes[0]->distance=0;
	enqueueVertex(vertexes[0]);
	while(vertexQueue!=NULL)
	{
		Vertex *current=vertexQueue;
		current->visited=true;
		cout<<(char)('A'+current->index)<<" distance: "<< current->distance<<" path : ";
		printPath(current);
		cout<<endl;
		vertexQueue=vertexQueue->next;
		Edge *currentEdge=current->head;
		while(currentEdge!=NULL)
		{
			if(currentEdge->finish->visited)
			{
				//skip
			}
			else if(currentEdge->finish->distance==INF)
			{
				currentEdge->finish->distance=current->distance+currentEdge->weight;
				currentEdge->finish->source=current;
				insertVertex(vertexQueue,currentEdge->finish);				
				
			}
			else
			{
				if(currentEdge->finish->distance>current->distance+currentEdge->weight)
				{
					currentEdge->finish->distance=current->distance+currentEdge->weight;
					updatePriority(currentEdge->finish);
					currentEdge->finish->source=current;
				}
			}
			currentEdge=currentEdge->next;
		}
	}
}


int main()
{
	ifstream inputFile;
	string fileName;
	cout<<"Enter file name:";
	getline(cin,fileName);
	inputFile.open(fileName.c_str());
	if(inputFile.fail())
	{
		cout<<"Can't open "<<fileName<<endl;
	}
	else
	{
		Graph graph(28);
		string source,destination;
		inputFile>>source;
		while(!inputFile.eof())
		{
			if(source.size()!=1||(source.at(0)<'A'||source.at(0)>'Z'))
			{
				cout<<"Invalid name "<<source<<endl;
				system("pause");
				return -1;
			}
			inputFile>>destination;
			if(destination.size()!=1||(destination.at(0)<'A'||destination.at(0)>'Z'))
			{
				cout<<"Invalid name "<<destination<<endl;
				system("pause");
				return -1;
			}
			int weight;
			inputFile>>weight;
			graph.addEdge(((int)source.at(0))-((int)'A'),  ((int)destination.at(0))-((int)'A'),weight);
			inputFile>>source;
		}
		graph.dijkstra();
	}
	system("pause");
	return 0;
}
