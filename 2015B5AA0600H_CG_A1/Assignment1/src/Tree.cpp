#include "tree.h"
#include "Utility.h"
#include <math.h>
#include<cstdlib>
#include<time.h>
#include<vector>
#include<iostream>
#include <chrono>


using namespace std;

#define X_separation 50
#define Y_separation 50

using namespace std;

struct timer
{
	void reset() { start = std::chrono::steady_clock::now(); }

	unsigned long long milliseconds_elapsed() const
	{
		const auto now = clock::now();
		using namespace std::chrono;
		return duration_cast<milliseconds>(now - start).count();
	}

	using clock = std::chrono::steady_clock;
	clock::time_point start = clock::now();
};

/*!This procedures forms the pre-order traversal of the tree
converting the relative coordinates to absolute coordinates*/
void relative_to_absolute(node* root, int col) {
	/*This procedures forms the pre-order traversal of the tree
	converting the relative coordinates to absolute coordinates*/
	if (root != NULL) {
		root->x = col;
		if (root->thread == true) {
			root->thread = false;
			root->llink = NULL;
			root->rlink = NULL;
		}
		relative_to_absolute(root->llink, col - root->offset);
		relative_to_absolute(root->rlink, col + root->offset);
	}
}

void tree::Construct_Balanced_Tree(vector <int>& a, int start, int finish, int beg = 0) //needs to be private
{
	if (start > finish) return;
	int mid = (start + finish) / 2;
	a[beg] = mid;
	//cout << "beg" << endl;
	//for (int i = start; i < finish; ++i) cout << a[i] << " ";
	//cout << endl;
	Construct_Balanced_Tree(a, start, mid - 1, beg + 1);
	Construct_Balanced_Tree(a, mid + 1, finish, mid - start + beg + 1);
	//cout << "end\n";
	//for (int i = start; i < finish; ++i) cout << a[i] << " ";
	//cout << endl;
}

void tree::Construct_unBalanced_Tree(vector <int>& a, int n) 
{
	srand(time(NULL));
	for (int i = 0; i < n; ++i)
		a[i] = rand() % (n - 0 + 1);

}

tree::tree(int n)
{
	node* N = new node;
	root = N;
	root->llink = NULL;
	root->rlink = NULL;
	root->thread = false;
	vector <int> arr(n);

	Construct_Balanced_Tree(arr, 0, n - 1);

	//Construct_unBalanced_Tree(arr, n);
	for (int i : arr)
		cout << i << endl;
	
	this->initializer(arr, n);
}

void tree::initializer(vector <int>& input, int n) 
{
	root->val = input[0];
	root->father = NULL;
	root->level = 0;
	for (int i = 1; i < n; ++i)
	{
		node* N = new node;
		N->llink = NULL;
		N->rlink = NULL;
		N->val = input[i];
		node* curr = this->root;
		node* parent = NULL;
		while (curr != NULL)
		{
			parent = curr;
			if (N->val >= curr->val)
				curr = (curr->rlink);
			else
				curr = (curr->llink);
		}
		if (N->val >= parent->val)
		{
			parent->rlink = N;
		}
		else
		{
			parent->llink = N;
		}
		N->father = parent;
		N->level = parent->level + 1;
	}
}

/*! Prints the tree in inorder
*/
void tree::printTree(node* N)
{
	if (N == NULL)
		return;
	printTree(N->llink);
	cout << N->val << " ";
	printTree(N->rlink);
}

/*! This function returns the height of the tree
*/
int tree::calcHeight(node* N = NULL)
{
	if (N == NULL) N = root;
	if (N->llink == NULL && N->rlink == NULL) return 0; //change return 0 to return 1 if height of single node tree is treated as 1
	if (N->llink == NULL) return 1 + calcHeight(N->rlink);
	if (N->rlink == NULL) return 1 + calcHeight(N->llink);
	return 1 + (calcHeight(N->llink) > calcHeight(N->rlink) ? calcHeight(N->llink) : calcHeight(N->rlink));
}

// This function calculates all offsets for the tidier algo

void setup_tidier(node* root, int level, node_offset& rightmost, node_offset& leftmost) {
	node* left;
	node* right;
	node_offset LR, LL, RR, RL;
	int CURsep, ROOTsep, LOffsum, ROffsum;
	if (root == NULL) {
		leftmost.level = -1;
		rightmost.level = -1;
	}
	else {
		root->y = level;
		left = root->llink;
		right = root->rlink;
		setup_tidier(left, level + 1, LR, LL);       //Position left subtree recursively 
		setup_tidier(right, level + 1, RR, RL);	  //Postion right subtree recursively
		if (left == NULL && right == NULL) {
			rightmost.adr = root;
			leftmost.adr = root;
			rightmost.level = level;
			leftmost.level = level;
			rightmost.offset = 0;
			leftmost.offset = 0;
			root->offset = 0;
		}
		else {

			//Set up for subtree pushing. Place roots of subtrees minimum distance apart
			CURsep = X_separation;
			ROOTsep = X_separation;
			LOffsum = 0;
			ROffsum = 0;
			/*Now consider each level  in turn until one subtree is exhausted
			pushing the subtrees apart when neccessary.*/
			while (left != NULL && right != NULL) {
				if (CURsep < X_separation) {
					ROOTsep = ROOTsep + (X_separation - CURsep);
					CURsep = X_separation;
				}
				//Advance left 
				if (left->rlink != NULL) {
					LOffsum = LOffsum + left->offset;
					CURsep = CURsep - left->offset;
					left = left->rlink;
				}
				else {
					LOffsum = LOffsum - left->offset;
					CURsep = CURsep + left->offset;
					left = left->llink;
				}
				//Advance right
				if (right->llink != NULL) {
					ROffsum = ROffsum - right->offset;
					CURsep = CURsep - right->offset;
					right = right->llink;
				}
				else {
					ROffsum = ROffsum + right->offset;
					CURsep = CURsep + right->offset;
					right = right->rlink;
				}
			}
			/*Set the offset in node root and include it in accumulated offsets in right and left*/
			root->offset = (ROOTsep + 1) / 2;
			LOffsum = LOffsum - root->offset;
			ROffsum = ROffsum + root->offset;
			/*Update node_offset descendents information*/
			if (RL.level > LL.level || root->llink == NULL) {
				leftmost = RL;
				leftmost.offset = leftmost.offset + root->offset;
			}
			else {
				leftmost = LL;
				leftmost.offset = leftmost.offset - root->offset;
			}
			if (LR.level > RR.level || root->rlink == NULL) {
				rightmost = LR;
				rightmost.offset = rightmost.offset - root->offset;
			}
			else {
				rightmost = RR;
				rightmost.offset = rightmost.offset + root->offset;
			}
			/*If subtrees of root were of uneven heights,check to see if threading is necessary
			At most one thread needs to be inserted*/
			if (left != NULL && left != root->llink) {
				(RR.adr)->thread = true;
				(RR.adr)->offset = abs((RR.offset + root->offset) - LOffsum);
				if ((LOffsum - root->offset) <= RR.offset) {
					(RR.adr)->llink = left;
				}
				else {
					(RR.adr)->rlink = left;
				}
			}
			else if (right != NULL && right != root->rlink) {
				(LL.adr)->thread = true;
				(LL.adr)->offset = abs((LL.offset - root->offset) - ROffsum);
				if ((ROffsum + root->offset) >= LL.offset) {
					(LL.adr)->rlink = right;
				}
				else {
					(LL.adr)->llink = right;
				}
			}
		}
	}
}

//This function takes in the values of x and y of the parent node and draws the node
//as well as the line connecting it with the parent node
//The function does this recursively for each child of the node
//No parameters need to be passed when calling it

void tree::drawTree(int cx = 0, int cy = 0, node* N = NULL)
{
	if (N == NULL) N = root;
	int dx = N->x ;
	int dy = (N->y) * Y_separation - 750;
	if (cx == 0 && cy == 0)
	{
		cx = dx;
		cy = dy;
	}
	if (N->llink != NULL) //draw the root -> llink subtree
		drawTree(dx, dy, N->llink);
	if (N->rlink != NULL) //draw the root -> rlink subtree
		drawTree(dx, dy, N->rlink);
	if (N->llink == NULL && N->rlink == NULL)//if node is a leaf draw a diamond
	{
		plotCircle(10, 1, dx, -dy);
		
	}
	else //if node is not a leaf then draw circle
	{
		
		plotCircle(10, 1, dx, -dy);
		
	}
	
	plotLine(dx, -dy, cx, -cy);
}
/*! This function sets up the initial conditions and calls the functions to implement tidier algorithm
	and draws the tree
*/
void tree::Tidier(void)
{
	
	
	clock_t start = clock();
	timer ti;
	auto t1 = std::chrono::high_resolution_clock::now();
	
	node_offset rightmost, leftmost;
	setup_tidier(root, 0, rightmost, leftmost);

	relative_to_absolute(root, 400);
	drawTree();
	printTree(root);
	clock_t end = clock();
	
	auto t2 = std::chrono::high_resolution_clock::now();
	
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	std::cout << "elapsed microseconds: "<< duration << std::endl;
}
