
#include <map>
#include "skiplist.h"
#include <time.h>
using namespace std;
using namespace ystl;
int main(){
	const int insert_count = 10000;
	SkipList<int, int> skl;
	map<int, int> m;
	//clock_t start1 = clock();

	//cout << skl.header->getKey() << endl;
	//cout << skl.header->forward[0]->getKey() << endl;
	for (int i = 1; i != insert_count; ++i){
		skl.insert(i, i+10);
	}
	/*clock_t ends1 = clock();

	clock_t start2 = clock();*/
	for (int i = 0; i != insert_count; ++i){
		m[i] = i+10;
	}
	//clock_t ends2 = clock();
	Node<int,int>* node;
	clock_t start1 = clock();

	//cout << skl.header->getKey() << endl;
	//cout << skl.header->forward[0]->getKey() << endl;
	for (int i = 1; i != insert_count; ++i){
		node = skl.search(i);
	}
	clock_t ends1 = clock();
	int t = 0;
	clock_t start2 = clock();
	for (int i = 0; i != insert_count; ++i){
		t = m[i];
	}
	clock_t ends2 = clock();
	cout << "skiplist insert time: " << (double)(ends1 - start1) / CLOCKS_PER_SEC << endl;
	cout << "rbtree insert time: " << (double)(ends2 - start2) / CLOCKS_PER_SEC << endl;
	//std::vector<int, allocator<int>> vec;
	//for (int i = 0; i < 10; i++){
	//	vec.push_back(i);
	//	std::cout << vec[i] << " ";
	//}
	//list<int> lst;
	//for (int i = 0; i < 10; i++){
	//	lst.push_back(i);
	//}

	//list<int>::iterator it = lst.begin();
	//while (it != lst.end()){
	//	std::cout << *it << " ";
	//	++it;
	//}
	return 0;
}