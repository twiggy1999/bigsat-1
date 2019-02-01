#include "All.h"
#include "trie.h"

typedef std::set<p_node> Set;
typedef Set::iterator Iterator;

void Trie::traverse(p_node now, operation x) {
	for (Iterator it = now->next.begin(); it != now->next.end(); it++) traverse(*it, x);  //后序遍历
	x(now);
}

static bool set_find(Set& x, Iterator& ans, int target) {                                 //返回是否找到
	node s(target);
	p_node tmp(&s);
	ans = x.find(tmp);
	if (ans != x.end()) return true;
	return false;
}

static bool implied(p_node start, std::vector<int>& x, int now) {                         //start是从哪个节点开始插，vector是待插句子，now是当前第几个lit待插(从0开始)
	Iterator it = start->next.begin();
	int i = 0;                                                                            //在当前层向下走的位置（如果存在的话）
	for (; i != x.size(); i++) if (set_find(start->next, it, x[i])) {                     //在Trie树当前层找到了待插lit，往下走
		if (it->p->next.size() == 0 || implied(*it, x, i + 1)) return true;               //待插句子被包含
	}
	return false;
}

bool Trie::if_implied(std::vector<int>& x) {                                              //start是从哪个节点开始插，vector是待插句子，now是当前第几个lit待插(从0开始)
	return implied(root, x, 0);
}

static void insert_intern(p_node start, std::vector<int>& x, int now) {
	Iterator it;
	if (now == x.size()) return;
	if (set_find(start->next, it, x[now])) {
		insert_intern(*it, x, now + 1);
	}
	else {
		p_node tmp;
		tmp.p = new node(x[now]);
		start->next.insert(tmp);
		insert_intern(tmp, x, now + 1);
	}
}

void Trie::insert(std::vector<int>& x) {
	insert_intern(root, x, 0);
}
