#include <unordered_map>
using namespace std;

struct Doublelink{
	int freq;
	Doublelink* pre;
	Doublelink* next;
	Node* head;
	Node* tail;

	Doublelink(){
		freq = 1;
		head = new Node();
		tail = new Node();
	}
};

struct Node{
	int key;
	int value;	
	int fre = 1;
	Node* pre;
	Node* next;
	Doublelink doublylink;

	Node();

	Node(int key, int value) :key(key), value(value){};
};

class LFU{
private:
	int cap;
	int min;
	Doublelink* firstlink;
	Doublelink* lastlink;

public:
	LFU(int size) : cap(size){
		firstlink = new Doublelink();
		lastlink = new Doublelink();
		firstlink->next = lastlink;
		lastlink->pre = firstlink;

	}

};