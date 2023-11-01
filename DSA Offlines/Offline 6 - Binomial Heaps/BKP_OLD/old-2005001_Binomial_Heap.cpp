
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

int debug_insert, debug_delete, debug_node, debug_union;

// Data structure for AVL Node

template<class T>
class Node{
public:

	T value;
	Node<T>*left_child;
	Node<T>*right_sibling;
	Node<T>*parent;
	int order; 

	Node(T item, Node<T>*lc = NULL, Node<T>*rs = NULL, Node<T>*p = NULL, int o = 0)
	{
		value = item;
		left_child = lc;
		right_sibling = rs;
		parent = p;
		order = o;
	}

};

// Combine two binomial trees of same order given the roots of the trees

template<class T>
Node<T>* combine_trees(Node<T>*r1, Node<T>*r2)
{

	if(r1->order != r2->order )
	{
		// for debugging
		cout<<"Combining different ordered trees not allowed...\n";
	}

	if(r1->value > r2->value)
	{
		swap(r1,r2);
	}

	r2->right_sibling = r1->left_child;
	r2->parent = r1;
	r1->left_child = r2;
	r1->order++;

	return r1;
}

template<class T>
class BHeap{
public:

	list<Node<T>*>roots; // roots will be stored maintaining increasing order
	Node<T>*min_node;

	// Create empty binomial heap
	BHeap()
	{
		roots.clear();
		min_node = NULL;
	}

	// Create binomial heap from a single item
	BHeap(T item)
	{
		roots.clear();
		Node<T>*node = new Node<T>(item);
		roots.push_back(node);
		min_node = node;
	}

	BHeap(list<Node<T>*>roots)
	{
		this->roots = roots;
	}

	void set_min_node()
	{
		int curr_min = inf;
		if(roots.empty())
		{
			min_node = NULL;
		}
		else
		{
			for(auto root: roots)
			{
				if(root->value < curr_min)
				{
					curr_min = root->value;
					min_node = root;
				}
			}
		}
	}

	void adjust_heap()
	{
		int n = roots.size();

		typename list<Node<T>*>::iterator it1, it2, it3;

		if(n <= 1) 
		{
			return; // already adjusted
		}
		else if(n == 2)
		{
			it1 = it2 = roots.begin();
			it2++;

			if((*it1)->order == (*it2)->order)
			{
				*it1 = combine_trees(*it1, *it2);
				roots.erase(it2);
			}

			return;
		}
		else
		{
			it1 = it2 = it3 = roots.begin();

			it2++;

			it3 = it2;

			it3++;

			while(it1 != roots.end() && it2 != roots.end())
			{
				if(it3 == roots.end()) // two elements left, do like before
				{
					if((*it1)->order == (*it2)->order)
					{
						*it1 = combine_trees(*it1, *it2);
						roots.erase(it2);
					}
					return;
				}
				else
				{
					if((*it1)->order != (*it2)->order)
					{
						it1++;
						it2++;
						it3++;
					}
					else
					{
						if((*it2)->order != (*it3)->order) // only first 2 equal, like 2,2,5
						{
							*it1 = combine_trees(*it1, *it2);
							it2 = roots.erase(it2);
							it3++;
						}
						else  // all 3 equal, like 2,2,2
						{
							it1++;
							it2++;
							it3++;
						}
					}
				}
			}
		}
	}

	void union_heap(BHeap H)
	{
		if(H.roots.empty())
		{
			return;
		}

		list<Node<T>*>new_roots;

		auto roots2 = H.roots;

		typename list<Node<T>*>::iterator it1, it2;

		it1 = roots.begin();
		it2 = roots2.begin();

		// Just like two pointer merging of sorted arrays

		while(it1 != roots.end() && it2 != roots2.end())
		{
			if((*it1)->order <= (*it2)->order)
			{
				new_roots.push_back(*it1);
				it1++;
			}
			else
			{
				new_roots.push_back(*it2);
				it2++;
			}
		}
		while(it1 != roots.end())
		{
			new_roots.push_back(*it1);
			it1++;
		}
		while(it2 != roots2.end())
		{
			new_roots.push_back(*it2);
			it2++;
		}

		roots.clear();

		roots = new_roots;

		adjust_heap();

		set_min_node();

	}

	void insert(T item)  // O(logn)
	{
		BHeap H = BHeap(item);

		this->union_heap(H);

		set_min_node();
	}

	T find_min()  // O(1)
	{
		if(min_node == NULL)
		{
			cout<<"Sorry, Heap is empty !";
			T dummy;
			return dummy;
		}
		return min_node->value;
	}

	T extract_min()
	{
		if(min_node == NULL)
		{
			cout<<"Sorry, Heap is empty !";
			T dummy;
			return dummy;
		}

		auto it = roots.begin();

		while((*it) != min_node)
		{
			it++;
		}

		roots.erase(it);  // removing the minimum root from roots

		T ans = min_node->value;

		list<Node<T>*>split_roots;

		Node<T>*temp = min_node->left_child;

		while(temp != NULL)
		{
			auto temp2 = temp;

			temp = temp->right_sibling;

			temp2->right_sibling = NULL;
			split_roots.push_front(temp2);
		}

		BHeap H(split_roots);

		union_heap(H);

		set_min_node();

		return ans;
	}

	void print_tree(Node<T>*root)
	{
		cout<<"Binomial Tree, B"<<root->order<<endl;

		vector<Node<T>*>current_level;
		current_level.push_back(root);
		int level = 0;
		while(!current_level.empty())
		{
			vector<Node<T>*>next_level;
			cout<<"Level "<<level<<" : ";
			for(auto temp: current_level)
			{
				while(temp != NULL)
				{
					if(temp->left_child != NULL)
					{
						next_level.push_back(temp->left_child);
					}
					cout<<temp->value<<" ";
					temp = temp->right_sibling;
				}
			}
			cout<<endl;
			level++;
			current_level = next_level;
		}
	}

	void print()
	{
		cout<<"Printing Binomial Heap..."<<endl;
		for(auto root: roots)
		{
			print_tree(root);
		}
	}
};

void solve()
{

	BHeap<int>H;

	string line;
	
	while(getline(cin,line))
	{

		istringstream ist(line);

		string s;

		ist>>s;

		if(s == "I")
		{
			int p;
			ist>>p;
			H.insert(p);
		}
		else if(s == "F")
		{
			int ans = H.find_min();
			cout<<"Find-Min returned "<<ans<<endl;
		}
		else if(s == "E")
		{
			int ans = H.extract_min();
			cout<<"Extract-Min returned "<<ans<<endl;
		}
		else if(s == "U")
		{
			BHeap<int>H2;
			int p;
			while(ist>>p)
			{
				H2.insert(p);
			}
			H.union_heap(H2);
		}
		else if(s == "P")
		{
			H.print();
		}
	}
}

int32_t main()
{
	freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);
	solve();
	return 0;
}