#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Edge;

class Node {
public:
	int nodeId;
	char data;
	char pos;
	int x;
	int y;
	Edge* edge;
	Node* next;

	Node() : nodeId(0), data('\0'), pos('\0'), x(0), y(0), edge(nullptr), next(nullptr) {}
	Node(const int& _nodeId, const char& _data, const char& _pos, const int& _x, const int& _y) {
		nodeId = _nodeId;
		data = _data;
		pos = _pos;
		x = _x;
		y = _y;
		edge = nullptr;
		next = nullptr;
	}
	void setValues(const int& _nodeId, const char& _data, const int& _x, const int& _y) {
		nodeId = _nodeId;
		data = _data;
		x = _x;
		y = _y;

	}
};

struct Edge {
	Node* to;
	Node* from;
	int weight;
	Edge* next;

	Edge() : to(nullptr), from(nullptr), weight(0), next(nullptr) {}
	Edge(Node* _to, Node* _from, int _weight) : to(_to), from(_from), weight(_weight), next(nullptr) {}
	bool isEmpty()
	{
		if (to == nullptr && from == nullptr && weight == 0 && next == nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

void addNode(Node*& head, const int& nodeId, const char& data, const int& x, const int& y);
Node* findNode(Node* head, const int& nodeId);
void addEdge(Edge*& head, Node*& nodeHead, const int& toNodeId, const int& fromNodeId, const int& weight);
void readNodeFile(const string& filename, Node*& head);
void readEdgeFile(const string& filename, Node*& head, Edge*& edge);
bool checkEdge(Node* node1, Node* node2, Edge* head);
bool findEdges(const int& x1, const int& x2, const int& y1, const int& y2, Node* head, Edge* root);
void displayGraph3(Node* head);
int PlayGame(Node* head, Edge* root);
void play(int& level, int& score, string& name);

int main() {
	char val = '\0';
	string name;
	cout << "Welcome to the 'Treasure Hunt in a Maze'" << endl;
	cout << "Press P to play the game " << endl;
	cin >> val;
	if (val == 'p' || val == 'P') {
		cout << "Enter your name " << endl;
		cin >> name;
		ofstream fout;
		fout.open(name + "_game_stats.txt");
		fout.close();
	}
	else {
		cout << "Bye! Have a nice day :)" << endl;
		return 0;
	}

	int score = 0;
	int level = 1;
	
	while (level <= 4) {
		play(level, score, name);	
	}
	ofstream fout;
	fout.open(name + "_game_stats.txt");
	fout << "Here are your game statistics:" << endl;
	fout << "Player name: " << name << endl;
	fout << "Score: " << score << endl;
	cout << "Here are your game statistics:" << endl;
	cout << "Player name: " << name << endl;
	cout << "Score : " << score << endl;
	cout << "Thank you for playing our game!" << endl;
	fout.close();



	return 0;
}

void play(int& level, int& score, string& name) {
	if (level < 1 || level > 4) {
		return ;
	}

	string nodeName = "level" + to_string(level) + "Node.txt";
	string edgeName = "level" + to_string(level) + "Edge.txt";

	Node* head = nullptr;
	Edge* root = new Edge;
	readNodeFile(nodeName, head);
	readEdgeFile(edgeName, head, root);
	score = PlayGame(head, root);

	if (score != 0) {
		char input = '\0';
		cout << "Press N for next level " << endl;
		cout << "Press Q to quit game" << endl;
		cout << "Press R to restart game " << endl;
		cin >> input;
		if (input == 'n' || input == 'N') {
			level++;
		}
		else if (input == 'q' || input == 'Q') {

			ofstream fout;
			fout.open(name + "_game_stats.txt");
			fout << "Here are your game statistics:" << endl;
			fout << "Player name: " << name << endl;
			fout << "Score: " << score << endl;
			cout << "Here are your game statistics:" << endl;
			cout << "Player name: " << name << endl;
			cout << "Score : " << score << endl;
			cout << "Thank you for playing our game!" << endl;
			fout.close();
			exit(0);
			
		}
		else if (input == 'r' || input == 'R') {
			level = 1;
			score = 0;
		}
	}
	else {
		cout << "You failed to complete the level :(" << endl;
		cout << "If you want to play again Press R" << endl;
		char input = '\0';
		cin >> input;
		if (input == 'r' || input == 'R') {
			level = 1;
			score = 0;
		}
		else {
			cout << "Thank you for playing our game!" << endl;
			exit(0);
		}
	}
}




void addNode(Node*& head, const int& nodeId, const char& data, const int& x, const int& y) {
    Node* newNode = new Node();
    newNode->setValues(nodeId, data, x, y);
    newNode->next = head;
    head = newNode;
}

Node* findNode(Node* head, const int& nodeId) {
    Node* current = head;
    while (current != nullptr) {
        if (current->nodeId == nodeId) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void addEdge(Edge*& head, Node*& nodeHead, const int& toNodeId, const int& fromNodeId, const int& weight) {
    Node* toNode = findNode(nodeHead, toNodeId);
    Node* fromNode = findNode(nodeHead, fromNodeId);

    if (toNode && fromNode) {
        Edge* newEdge = new Edge(toNode, fromNode, weight);
        if (head->isEmpty()) {
            head = newEdge;
        }
        else {
            Edge* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newEdge;
        }
    }
    else {
        cerr << "Error: Invalid node IDs for edge creation." << endl;
    }
}

void readNodeFile(const string& filename, Node*& head) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    int nodeId, x, y;
    char data;
    while (file >> nodeId >> data >> x >> y) {
        addNode(head, nodeId, data, x, y);
    }
    file.close();
}

void readEdgeFile(const string& filename, Node*& head, Edge*& edge) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    int toNodeId, fromNodeId, weight;
    while (file >> toNodeId >> fromNodeId >> weight) {
        addEdge(edge, head, toNodeId, fromNodeId, weight);
    }
    file.close();
}


bool checkEdge(Node* node1, Node* node2, Edge* head) {
    Edge* temp = head;
    while (temp != nullptr) {
        if ((temp->to == node1 && temp->from == node2) || (temp->to == node2 && temp->from == node1)) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

bool findEdges(const int& x1, const int& x2, const int& y1, const int& y2, Node* head, Edge* root) {
    Node* node1 = nullptr;
    Node* node2 = nullptr;

    // Find the two nodes
    Node* current = head;
    while (current != nullptr) {
        if (current->x == x1 && current->y == y1) {
            node1 = current;
        }
        if (current->x == x2 && current->y == y2) {
            node2 = current;
        }
        current = current->next;
    }

    // Traverse the Edge list to find the edge between the two nodes
    Edge* temp = root;
    while (temp != nullptr) {
        if ((temp->to == node1 && temp->from == node2) || (temp->to == node2 && temp->from == node1)) {
            return true;
        }
        temp = temp->next;
    }

    return false;
}

void displayGraph3(Node* head) {
    Node* current = head;
    int maxX = 0, maxY = 0;
    while (current != nullptr) {
        if (current->x > maxX) maxX = current->x;
        if (current->y > maxY) maxY = current->y;
        current = current->next;
    }

    for (int y = 0; y <= maxY; y++) {
        for (int x = 0; x <= maxX; x++) {
            bool found = false;
            current = head;
            while (current != nullptr) {
                if (current->x == x && current->y == y) {
                    cout << current->data << " ";
                    found = true;
                    break;
                }
                current = current->next;
            }
            if (!found) cout << ". ";
        }
        cout << endl;
    }
}

int PlayGame(Node* head, Edge* root)
{
    int Score = 0;
    char input;
    bool Monster = false;
    bool Treasure = false;
    Node* current = head;
    int maxX = 0, maxY = 0;
    while (current != nullptr) {
        if (current->x > maxX) maxX = current->x;
        if (current->y > maxY) maxY = current->y;
        current = current->next;
    }
    Node* current2 = head;
    int x = 0, y = 0;
    int tempX = 0, tempY = 0;
    cout << "YOUR CURRENT POSITION = *" << endl;
    cout << "Monster = X" << endl;
    cout << "Treasure = &" << endl;
    do
    {

        current2 = head;
        tempX = x;
        tempY = y;
        displayGraph3(head);
        cout << "Enter W=UP | S=Down | A=Left | D=Right" << endl;
        cin >> input;
        if (input == 'W' || input == 'w')
        {
            y--;
        }
        else if (input == 'S' || input == 's')
        {
            y++;
        }
        else if (input == 'A' || input == 'a')
        {
            x--;
        }
        else if (input == 'D' || input == 'd')
        {
            x++;
        }
        if (findEdges(tempX, x, tempY, y, head, root) == true)
        {
            while (current2 != nullptr)
            {


                if (current2->x == x && current2->y == y)
                {
                    if (current2->data == '+')
                    {
                        current2->data = '*';
                        Score += 10;
                        break;
                    }
                    else if (current2->data == 'X')
                    {
                        Monster = true;
                        Score = 0;
                        break;
                    }
                    else if (current2->data == '&')
                    {
                        Treasure = true;
                        Score += 50;
                        break;
                    }
                }
                current2 = current2->next;
            }
        }
        else
        {
            cout << "No Path! Choose another path " << endl;
        }

        if (Monster == true)
        {
            cout << "Monster!" << endl;
            cout << "You lost. Better luck next time :(" << endl;
        }
        else if (Treasure == true)
        {
            cout << "Treasure Found!" << endl;
            cout << "You won! Congratulations :)" << endl;
        }

    } while (x <= maxX && y <= maxY && Treasure == false && Monster == false);
    cout << "TOTAL SCORE = " << Score << endl;

    return Score;

}
