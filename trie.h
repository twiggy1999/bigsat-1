#pragma once
#include <set>
#include <vector>

/*
	Editted 1/31
*/

struct p_node;

struct node {
	int data;
	std::set<p_node> next;
	node() { data = 0;  }
	node(int i) { data = i; }
	bool operator== (const node& n) const { return data == n.data; }
	bool operator<  (const node& n) const { return data <  n.data; }  //set::find() 需要用到
	bool operator>  (const node& n) const { return data >  n.data; }
};

struct p_node {                                                       //为调用set::find方法建立的类
	node* p;
	p_node() { p = 0; }
	explicit p_node(node* ptr) { p = ptr; }
	bool operator== (const p_node& ptr) const { return *p == *(ptr.p); }
	bool operator<  (const p_node& ptr) const { return *p <  *(ptr.p); }
	bool operator>  (const p_node& ptr) const { return *p >  *(ptr.p); }
	node* operator-> () { return p; }                                  //智能指针
};

typedef void (*operation)(p_node);

class Trie {
public:
	void insert(std::vector<int>& clause);       //带剔除逻辑的插入子句
	void traverse(p_node start, operation op);   //以dfs序遍历，对每个节点使用op操作(后序)  
	bool if_implied(std::vector<int>& clause);
	Trie()  { root.p = new node; root->data = 0; root->next.clear(); }
	~Trie() { traverse(root, [](p_node ptr) { delete ptr.p; }); }
private:
	p_node root;
};
