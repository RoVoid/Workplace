#include <iostream>
#include <queue>

#define uint unsigned

using std::cout;
using std::endl;
using std::nothrow;

struct Node{
	int value;
	bool isBlack;
	Node *left, *right, *parent;
};

struct Pair{
	uint level;
	Node* node;
};

class RBTree {
	Node* root = nullptr;
	
	Node* grandparent(Node *n){ return n && (n->parent) ? n->parent->parent : nullptr; }

	Node* uncle(Node* n) {
		Node *g = grandparent(n);
		if (!g) return nullptr;
		return n->parent == g->left ? g->right : g->left;
	}	
	
	void rotate_left(Node *n)
	{
		Node *pivot = n->right;
		
		pivot->parent = n->parent;
		if (n->parent) {
			if (n->parent->left==n) n->parent->left = pivot;
			else n->parent->right = pivot;
		}		
		
		n->right = pivot->left;
		if (pivot->left) pivot->left->parent = n;

		n->parent = pivot;
		pivot->left = n;
		
		if(n == root) root = pivot;
	}

	void rotate_right(Node *n)
	{
		Node *pivot = n->left;
		
		pivot->parent = n->parent;
		if (n->parent) {
			if (n->parent->left==n) n->parent->left = pivot;
			else n->parent->right = pivot;
		}		
		
		n->left = pivot->right;
		if (pivot->right) pivot->right->parent = n;

		n->parent = pivot;
		pivot->right = n;
		
		if(n == root) root = pivot;
	}
	
	void insert_case1(Node *n) {
		if (n->parent) insert_case2(n);
		else n->isBlack = true;
	}
	
	void insert_case2(Node *n) {
		if (n->parent->isBlack) return;
		insert_case3(n);
	}
	
	void insert_case3(Node *n){
		Node *u = uncle(n), *g;
		
		if(!u || u->isBlack) {
			insert_case4(n);
			return;
		}

		n->parent->isBlack = true;
		u->isBlack = true;
		g = grandparent(n);
		g->isBlack = false;
		insert_case1(g);
	}

	void insert_case4(Node *n) {
		Node *g = grandparent(n);

		if ((n == n->parent->right) && (n->parent == g->left)) {
			rotate_left(n->parent);
			n = n->left;
		} else if ((n == n->parent->left) && (n->parent == g->right)) {
			rotate_right(n->parent);
			n = n->right;
		}
		
		insert_case5(n);
	}
	
	void insert_case5(Node *n){
		Node *g = grandparent(n);

		n->parent->isBlack = true;
		g->isBlack = false;
		if ((n == n->parent->left) && (n->parent == g->left)) rotate_right(g);
		else rotate_left(g);
	}
	
	public:
	void push(const int& value){
		Node *node = new(nothrow) Node{value, false, nullptr, nullptr, nullptr};
		if(!node) return;
		if(!root){
			root = node;
			insert_case1(node);
			return;
		}
		Node* curr = root;
		while(1){
			if(curr->value > value){
				if(!curr->left) {
					node->parent = curr;
					curr->left = node;
					break;
				}
				curr = curr->left;
				continue;
			}
			if(!curr->right) {
				node->parent = curr;
				curr->right = node;
				break;
			}
			curr = curr->right;
		}
		insert_case1(node);
	}
	void show(){
		std::queue<Pair> pairs;
		cout<<"\\/\\/\\/\\/\\/"<<endl;
		if(root){
			Pair pair;
			uint level = 0;
			pairs.push({0, root});
			while(!pairs.empty()){
				pair = pairs.front();
				pairs.pop();
				if(level != pair.level){
					cout<<endl;
					++level;
				}
				cout<<pair.node->value<<(pair.node->isBlack ? '+' : '-')<<' ';
				if(pair.node->left) pairs.push({pair.level + 1, pair.node->left});
				if(pair.node->right) pairs.push({pair.level + 1, pair.node->right});
			}
		}
		cout<<endl<<"/\\/\\/\\/\\/\\"<<endl;
	}
};

int main(){
	RBTree t;
	t.push(5);
	t.push(1);
	t.push(6);
	t.show();
	
	t.push(4);
	t.show();
	
	t.push(3);
	t.show();
	
	t.push(0);
	t.push(4);
	t.push(4);
	t.push(4);
	t.show();
	
	t.push(0);
	t.show();
	return 0;
}
