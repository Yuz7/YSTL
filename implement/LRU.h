#include <unordered_map>
using namespace std;

struct Node{
	int key;
	int val;
	Node* pre;
	Node* next;

	Node(int k, int v) :key(k), val(v), pre(nullptr), next(nullptr){};
};

class LRU{
private:
	int cap;
	Node* head;
	Node* tail;
	unordered_map<int, Node*> hash;

public:
	LRU(int size){
		cap = size;
		head = new Node();
		tail = new Node();
		head->next = tail;
		tail->pre = head;
		head->pre = nullptr;
		tail->next = nullptr;
	}

	int get(int key){
		if (hash.count(key) > 0){
			Node* node = hash[key];
			remove(node);
			tohead(node);
			return node->val;
		}
		return -1;
	}

	void put(int key,int value){
		if (hash.count(key) > 0){
			Node* node = hash[key];
			node->val = value;
			remove(node);
			tohead(node);
		}
		else{
			Node* pnode = new Node(key, value);
			if (hash.size() == cap){
				hash.erase(hash.find(tail->pre->key));
				remove(tail->pre);
			}
			tohead(pnode);
			hash[key] = pnode;
		}

	}

private:
	void remove(Node*node){
		node->pre->next = node->next;
		node->next->pre = node->pre;
	}

	void tohead(Node*node){
		head->next->pre = node;
		node->next = head->next;
		node->pre = head;
		head->next = node;
	}

};




