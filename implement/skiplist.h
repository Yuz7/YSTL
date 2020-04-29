#include "random.h"
#include <cstddef>
#include <cassert>
#include <ctime>
#include <iostream>
namespace ystl{
template<typename K,typename V>
class Node{
	friend class skiplist;
public:
	Node(){}
	Node(const K k, const V v){
		key = k;
		value = v;
	}
	~Node(){
		delete[]forward;
	}

	K getKey()const{ return key; }
	V getValue()const{ return value; }

public:
	K key;
	V value;
	Node<K, V> **forward;
	int nodeLevel;
};

template<typename K, typename V>
class SkipList{
public:
	typedef Node<K, V>				linknode;
	typedef Node<K, V>*				linktype;
public:
	SkipList() : rnd(0x12345678){
		createlist(0x7fffffff);
	}
	~SkipList(){
		freelist();
	}
public:
	void createlist(K footerKey){
		createNode(0, footer);
		footer->key = footerKey;
		this->level = 0;
		createNode(MAXLEVEL, header);
		for (int i = 0; i < MAXLEVEL; ++i){
			header->forward[i] = footer;
		}
		nodecnt = 0;
	}

	void freelist(){
		linktype p = header;
		linktype q;
		while (p != NULL){
			q = p->forward[0];
			delete p;
			p = q;
		}
		delete p;
	}

public:
	int level;
	static const int MAXLEVEL = 16;
	Random rnd;

	size_t nodecnt;
	
	linktype header;
	linktype footer;
	Node<K, V>* node;

public:
	int getRandomlevel(){ 
		int lvl = static_cast<int>(rnd.Uniform(MAXLEVEL));
		if (lvl == 0) {
			lvl = 1;
		}
		return level;
	}
	void createNode(int level, linktype&node){
		node = new linknode(NULL, NULL);
		node->forward = new linktype[level + 1];
		node->nodeLevel = level;
		assert(node != NULL);
	}
	void createNode(int level, linktype &node, K key, V val){
		node = new linknode(key,val);
		if (level >= 0){
			node->forward = new linktype[level+1];
		}
		node->nodeLevel = level;
		assert(node != NULL);
	}
public:
	linktype search(const K key) const{
		linktype node = header;
		for (int i = level; i >= 0; --i){
			while ((node->forward[i])->key < key){
				node = node->forward[i];
			}
		}
		node = node->forward[0];
		if (node->key == key){
			return node;
		}
		else{
			return nullptr;
		}
	}
	bool insert(K key, V val){

		linktype updates[MAXLEVEL];
		linktype node = header;

		for (int i = level; i >= 0; --i){
			//linktype next = node->forward[i];
			while ((node->forward[i])->key < key){
				node = node->forward[i];
				//next = node->forward[i];
			}
			updates[i] = node;
		}
		node = node->forward[0];
		if (node->key == key){
			return false;
		}
		int nodelevel = getRandomlevel();
		if (nodelevel > level){
			nodelevel = ++level;
			updates[nodelevel] = header;
		} 

		//std::cout << " debug2 ";
		linktype newNode;
		createNode(nodelevel, newNode, key, val);
		//std::cout << " debug3 ";
		//std::cout << "newnode: " << newNode->getKey() << endl;
		for (int i = nodelevel; i >= 0; --i){
			//std::cout << " debug4 "<<endl;
			node = updates[i];
			//std::cout << "nodekey: " << node->forward[nodelevel]->getKey() << endl;
			//std::cout << " debug5 " << endl;
			newNode->forward[i] = node->forward[i];
			node->forward[i] = newNode;
		}
		
		++nodecnt;

		return true;
	}
	bool remove(K key, V val){
		linktype updates[MAXLEVEL];
		linktype node = header;
		for (int i = level; i >= 0; --i){
			while ((node->forward[i])->key < key) {
				node = node->forward[i];
			}
			updates[i] = node;
		}
		node = node->forward[0];
		if (node->key != key){
			return false;
		}
		for (int i = 0; i <= level; i++){
			if (updates[i]->forward[i] != node){
				break;
			}
			updates[i]->forward[i] = node->forward[i];
		}
		delete node;
		while (level > 0 && header->forward[level] == footer) {
			--level;
		}
		--nodecnt;
		return true;
	}
};

}