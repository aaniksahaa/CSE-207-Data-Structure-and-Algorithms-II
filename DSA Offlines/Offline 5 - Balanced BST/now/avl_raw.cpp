
#include <bits/stdc++.h>
using namespace std;

/*** Definitions ***/

#define gap " "
#define br "\n"
#define pb push_back
#define all(a) a.begin(),a.end()
#define int long long int
#define fo(i,n) for(int i=0;i<n;i++)
#define pii pair<int,int>
#define double long double

/*** Constants ***/

const int inf = LLONG_MAX - 100;

/*** Globals ***/

int debug_insert, debug_delete, debug_node;

// Data structure for AVL Node

template<class T>
class AVLNode{
public:

	T value;
	AVLNode<T>*left;
	AVLNode<T>*right;
	AVLNode<T>*parent;
	int height; 

	AVLNode(T item, AVLNode<T>*l = NULL, AVLNode<T>*r = NULL, AVLNode<T>*p = NULL, int h = 0)
	{
		value = item;
		left = l;
		right = r;
		parent = p;
		height = h;
	}
	bool is_leaf()
	{
		return left == NULL && right == NULL;
	}
	void update_height()
	{
		if(is_leaf())
		{
			height = 0;
		}
		else
		{
			int left_height  = (left == NULL) ? 0 : left->height;
			int right_height = (right == NULL) ? 0 : right->height;
			
			height =  1 + max(left_height, right_height);
		}

		if(debug_node) cout<<"updated height of "<<value<<" = "<<height<<endl;
		
		return;
	}
	int get_balance_factor()
	{
		int left_height  = (left == NULL) ? 0 : 1 + left->height;
		int right_height = (right == NULL) ? 0 : 1 + right->height;

		if(debug_node) cout<<"Left height of "<<value<<" = "<<left_height<<endl;
		if(debug_node) cout<<"Right height of "<<value<<" = "<<right_height<<endl;

		return left_height - right_height;
	}
	bool is_balanced()
	{
		int balance_factor = get_balance_factor();
		return abs(balance_factor) <= 1;
	}
};

template<class T>
AVLNode<T>* left_rotate( AVLNode<T>*p )
{
	if(p->right == NULL)
	{
		return p;           // Return unchanged as left-rotate can't be done without right child
	}
	AVLNode<T>*q = p->right;

	p->right = q->left;
	if(q->left != NULL)
	{
		q->left->parent = p;
	}

	q->parent = p->parent;
	if(p->parent != NULL)
	{
		if(p == p->parent->left)
		{
			p->parent->left = q;
		}
		else
		{
			p->parent->right = q;
		}
	}
	q->left = p;
	p->parent = q;

	p->update_height();
	q->update_height();

	return q;

}

template<class T>
AVLNode<T>* right_rotate( AVLNode<T>*p )
{
	if(p->left == NULL)
	{
		return p;       // Return unchanged as right-rotate can't be done without left child
	}
	AVLNode<T>*q = p->left;
	p->left = q->right;
	if(q->right != NULL)
	{
		q->right->parent = p;
	}
	q->parent = p->parent;
	if(p->parent != NULL)
	{
		if(p == p->parent->left)
		{
			p->parent->left = q;
		}
		else
		{
			p->parent->right = q;
		}
	}
	q->right = p;
	p->parent = q;

	p->update_height();
	q->update_height();

	return q;
}

// Make the subtree balanced by rotations and return new root of the subtree
template<class T>
AVLNode<T>*restructure(AVLNode<T>*root)
{
	int balance_factor = root->get_balance_factor();

	if(debug_insert || debug_delete) cout<<"balance factor of "<<root->value<<" = "<<balance_factor<<endl;

	if( balance_factor > 1 )   // Current node is Left Heavy
	{
		int left_balance_factor = root->left->get_balance_factor();

		if(left_balance_factor >= 0)  // Left-Left Case
		{
			if(debug_insert || debug_delete) cout<<"left-left"<<endl;
			return right_rotate(root);
		}
		else  // Left-Right Case
		{
			if(debug_insert || debug_delete) cout<<"left-right"<<endl;
			root->left = left_rotate(root->left);
			return right_rotate(root);
		}
	}
	else if( balance_factor < -1 )   // Current node is Right Heavy
	{
		int right_balance_factor = root->right->get_balance_factor();

		if(right_balance_factor <= 0)  // Right-Right Case
		{
			if(debug_insert || debug_delete) cout<<"right-right"<<endl;
			return left_rotate(root);
		}
		else  // Right-Left Case
		{
			if(debug_insert || debug_delete) cout<<"right-left"<<endl;
			root->right = right_rotate(root->right);
			return left_rotate(root);
		}
	}

	return root; 

}

template<class T>
class AVL{
public:
	AVLNode<T>*rt;


	AVL()
	{
		rt = NULL;
	}

	AVLNode<T>* insertHelp(AVLNode<T>*root, T item)
	{
		if(root == NULL)
		{
			if(debug_insert) cout<<"reached empty, inserting node "<<item<<" here..."<<endl;
			AVLNode<T>*node = new AVLNode<T>(item, NULL, NULL, NULL, 0); // creating new node
			return node;
		}
		else if(item < root->value)
		{
			if(debug_insert) cout<<"going left from "<<root->value<<endl;
			AVLNode<T>*node = insertHelp(root->left, item);
			root->left = node;
			if(node != NULL)
			{
				node->parent = root;
			}
			if(debug_insert) cout<<"returned to "<<root->value<<endl;
		}
		else if(item > root->value)
		{
			if(debug_insert) cout<<"going right from "<<root->value<<endl;
			AVLNode<T>*node = insertHelp(root->right, item);
			root->right = node;
			if(node != NULL)
			{
				node->parent = root;
			}
			if(debug_insert) cout<<"returned to "<<root->value<<endl;
		}
		else
		{
			cout<<"Sorry, Value already exists!"<<endl;
			return root;
		}

		root->update_height(); // due to inserting at left or right, this node's height may have changed 

		int balance_factor = root->get_balance_factor();

		if(debug_insert) cout<<"balance factor of "<<root->value<<" = "<<balance_factor<<endl; 

		if( abs(balance_factor) > 1 ) // if unbalanced
		{
			root = restructure(root);
		}

		return root;  // if current node already balanced
	}
	AVLNode<T>* minHelp(AVLNode<T>*root)
	{
		if(root == NULL)
		{
			return NULL;
		}
		if(root->left != NULL)
		{
			return minHelp(root->left);
		}
		else
		{
			return root;
		}
	}
	AVLNode<T>* delHelp(AVLNode<T>*root, T item)
	{
		if(root==NULL)
		{
			if(debug_delete) cout<<"Item to be deleted is not found"<<endl;
			return NULL;
		}
		else if(item < root->value)
		{
			if(debug_delete) cout<<"going left from "<<root->value<<endl;
			AVLNode<T>*node = delHelp(root->left, item);
			root->left = node;
			if(node != NULL)
			{
				node->parent = root;
			}
			if(debug_delete) cout<<"returned to "<<root->value<<endl;
		}
		else if(item > root->value)
		{
			if(debug_delete) cout<<"going right from "<<root->value<<endl;
			AVLNode<T>*node = delHelp(root->right, item);
			root->right = node;
			if(node != NULL)
			{
				node->parent = root;
			}
			if(debug_delete) cout<<"returned to "<<root->value<<endl;
		}
		else  // this node has to be deleted
		{
			if(debug_delete) cout<<"item found"<<endl;
			AVLNode<T>*temp = root;

			if(root->left == NULL && root->right == NULL)
			{
				delete root;
				return NULL;
			}
			else if(root->left == NULL)
			{
				root = root->right;
				delete temp;
			}
			else if(root->right == NULL)
			{
				root = root->left;
				delete temp;
			}
			else
			{
				AVLNode<T>* inorder_successor = minHelp(root->right);
				root->value = inorder_successor->value;
				//root->right = delMinHelp(root->right);
				root->right = delHelp(root->right, inorder_successor->value);
			}
		}

		root->update_height(); // due to inserting at left or right, this node's height may have changed 

		int balance_factor = root->get_balance_factor();

		if(debug_delete) cout<<"balance factor of "<<root->value<<" = "<<balance_factor<<endl; 

		if( abs(balance_factor) > 1 ) // if unbalanced
		{
			root = restructure(root);
		}

		return root;  // if current node already balanced

	}
	bool findHelp(AVLNode<T>*root, T item)
	{
		if(root == NULL)
		{
			return false;  // not found
		}
		else if(item < root->value)
		{
			return findHelp(root->left, item);
		}
		else if(item > root->value)
		{
			return findHelp(root->right, item);
		}
		else
		{
			return true; // found
		}
	}
	void inorderHelp(AVLNode<T>*root)
	{
		if(root == NULL)
		{
			return;
		}
		inorderHelp(root->left);
		cout<<root->value<<" ";
		inorderHelp(root->right);
	}
	void printHeightsHelp(AVLNode<T>*root)
	{
		if(root == NULL)
		{
			return;
		}
		printHeightsHelp(root->left);
		cout<<root->value<<" --> "<<root->height<<endl;
		printHeightsHelp(root->right);
	}
	void printHelp(AVLNode<T>*root)
	{
		if(root == NULL)
		{
			return;
		}
		cout<<root->value;
		if(root->is_leaf())
		{
			return;
		}
		cout<<"(";
		printHelp(root->left);
		cout<<",";
		printHelp(root->right);
		cout<<")";
	}

	void insert(T item)
	{
		if(debug_insert) cout<<endl<<item<<" is to be inserted"<<endl<<endl;
		rt = insertHelp(rt,item);
	}
	void del(T item)
	{
		if(debug_delete) cout<<endl<<item<<" is to be deleted"<<endl<<endl;
		rt = delHelp(rt,item);
	}
	bool find(T item)
	{
		return findHelp(rt, item);
	}
	void inorder()
	{
		inorderHelp(rt);
		cout<<endl;
	}
	void printHeights()
	{
		cout<<endl<<"Heights of all nodes: "<<endl<<endl;
		printHeightsHelp(rt);
		cout<<endl;
	}
	void print()
	{
		printHelp(rt);
		cout<<endl;
	}
};

double get_duration_ms(const std::chrono::steady_clock::time_point& start,
                                               const std::chrono::steady_clock::time_point& end) {
     auto duration_ms = std::chrono::duration<double, std::milli>(end - start).count();
	 return duration_ms;
}

int32_t main()
{
	freopen("in.txt", "r", stdin);
    freopen("out_avl.txt", "w", stdout);

	double insert_time = 0.0, delete_time = 0.0, search_time = 0.0, trav_time = 0.0, total_time = 0.0;

	int n,p;
	AVL<int>A;

	string s;

	int num = 0;

	while(cin>>s)
	{
		debug_insert = debug_delete = debug_node = 0;
		num++;
		if(s == "F")
		{
			cin>>p;
			auto start = std::chrono::steady_clock::now();
			bool found = A.find(p);
			auto end = std::chrono::steady_clock::now();
			if(!found)
			{
				cout<<"not ";
			}
			cout<<"found"<<endl;

			search_time += get_duration_ms(start,end);
		}
		else if(s == "I")
		{
			cin>>p;
			auto start = std::chrono::steady_clock::now();
			A.insert(p);
			auto end = std::chrono::steady_clock::now();
			A.print();

			insert_time += get_duration_ms(start,end);

		}
		else if(s == "T")
		{
			auto start = std::chrono::steady_clock::now();
			A.inorder();
			auto end = std::chrono::steady_clock::now();

			trav_time += get_duration_ms(start,end);

		}
		else if(s == "D")
		{
			cin>>p;
			auto start = std::chrono::steady_clock::now();
			A.del(p);
			auto end = std::chrono::steady_clock::now();
			A.print();

			delete_time += get_duration_ms(start,end);
		}
	}

	fclose(stdout);

	freopen("report_avl.txt","w",stdout);

	int precision = 8;

	total_time = insert_time + delete_time + search_time + trav_time;

	cout<<"operation time(ms)"<<endl;
	cout<<"insert "<<setprecision(precision)<<insert_time<<endl;
	cout<<"delete "<<setprecision(precision)<<delete_time<<endl;
	cout<<"search "<<setprecision(precision)<<search_time<<endl; 
	cout<<"trav "<<setprecision(precision)<<trav_time<<endl;
	cout<<"total "<<setprecision(precision)<<total_time<<endl;

	fclose(stdout);
}

/*

F 1
I 1
I 2
I 3
I 4
I 5
I 6
I 7
I 8
I 10
T
D 5
D 7
F 7
D 6
D 4
F 1
T

*/


