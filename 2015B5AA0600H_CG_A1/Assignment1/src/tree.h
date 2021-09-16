#include <vector>

using namespace std;

typedef struct node {
	int val;					//information
	struct node* llink;		//pointer to left subtree
	struct node* rlink;//pointer to right subtree
	struct node* father;
	int status;
	
	int level;
	int x;						//x coordinate of node
	int y;						//y coordinate of node
	int offset;					//distance to each son
	bool thread;				//Threading active or not
}node;

typedef struct node_offset {
	node* adr;	//address
	int offset;		//offset from root of subtree
	int level;		//tree level
}node_offset;

class tree
{
public:
	
	node* root;
	tree(int n);
	void Construct_Balanced_Tree(vector <int>&, int, int, int);
	void Construct_unBalanced_Tree(vector<int>& a, int n);
	int calcHeight(node* N);
	void initializer(vector <int>&, int n);
	void drawTree(int cx, int cy, node* N);
	void Tidier();
	void printTree(node* N);
};
