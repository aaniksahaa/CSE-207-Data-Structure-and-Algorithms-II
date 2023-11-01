
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

	void set_min_node()  // O(logn)
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

	void adjust_heap()  // O(logn) // Combines trees of duplicate order to maintain binomial heap structure
	{
		int n = roots.size();

		typename list<Node<T>*>::iterator it_first, it_second, it_third;

		if(n <= 1) 
		{
			return; // already adjusted
		}
		else if(n == 2)
		{
			it_first = it_second = roots.begin();

			it_second++;

			if((*it_first)->order == (*it_second)->order)
			{
				*it_first = combine_trees(*it_first, *it_second);
				roots.erase(it_second);
			}

			return;
		}
		else  // n >= 3, So we can safely start with three pointers
		{
			// Maintaing three iterators for three pointer

			it_first = it_second = it_third = roots.begin();

			it_second++;

			it_third++; it_third++;

			while(it_first != roots.end() && it_second != roots.end())
			{
				if(it_third == roots.end()) // two elements left, do like before
				{
					if((*it_first)->order == (*it_second)->order)
					{
						*it_first = combine_trees(*it_first, *it_second);
						roots.erase(it_second);
					}
					return;
				}
				else
				{
					if((*it_first)->order != (*it_second)->order)
					{
						it_first++;
						it_second++;
						it_third++;
					}
					else
					{
						if((*it_second)->order != (*it_third)->order) // only first 2 equal, like 2,2,5
						{
							*it_first = combine_trees(*it_first, *it_second);
							it_second = roots.erase(it_second);
							it_third++;
						}
						else  // all 3 equal, like 2,2,2
						{
							it_first++;
							it_second++;
							it_third++;
						}
					}
				}
			}
		}
	}

	void union_heap(BHeap H)  // prepares the roots list ( containing duplicate orders ) by combining root sorted by order
	{
		if(H.roots.empty())
		{
			return;
		}

		list<Node<T>*>new_roots;

		auto roots2 = H.roots;

		typename list<Node<T>*>::iterator it_first, it_second;

		it_first = roots.begin();
		it_second = roots2.begin();

		// Just like two pointer merging of sorted arrays

		while(it_first != roots.end() && it_second != roots2.end())
		{
			if((*it_first)->order <= (*it_second)->order)
			{
				new_roots.push_back(*it_first);
				it_first++;
			}
			else
			{
				new_roots.push_back(*it_second);
				it_second++;
			}
		}
		while(it_first != roots.end())
		{
			new_roots.push_back(*it_first);
			it_first++;
		}
		while(it_second != roots2.end())
		{
			new_roots.push_back(*it_second);
			it_second++;
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

	T extract_min()  // O(logn)
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