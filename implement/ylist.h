#include "yallocator.h"
#include "yconstruct.h"
namespace ystl{

template<class T>
struct node{
	T data;
	node* pre;
	node* next;
	bool operator==(const node& n){
		return data == n.data && pre == n.pre && next == n.next;
	}
};

template<class T>
struct list_iterator{
	typedef node<T>* link_type;
	typedef list_iterator<T> self;
	typedef size_t size_type;
	link_type node;

	list_iterator(link_type x = nullptr) : node(x){}
	list_iterator(){}
	list_iterator(const list_iterator& rhs) :node(rhs.node) {}

	bool operator==(const list_iterator<T>& it) const{ return node == it.node; }
	bool operator!=(const list_iterator<T>& it) const{ return node != it.node; }
	T& operator*() const { return node->data; }
	T* operator->() const { return &(operator*()); }

	self& operator++() {
		node = (link_type)(node->next);
		return *this;
	}
	self operator++(int){
		self tmp = *this;
		++*this;
		return tmp;
	}
	self& operator--() {
		node = (link_type)(node->pre);
		return *this;
	}
	self operator--(int){
		self tmp = *this;
		--*this;
		return tmp;
	}
};

template<class T>
class list{
public:
	typedef T value_type;
	typedef	node<T> listnode;
	typedef size_t size_type;
	typedef list_iterator<T> iterator;
	typedef const list_iterator<T> const_iterator;
	typedef T& reference;
	typedef listnode* link_type;
public:
	iterator begin()	{ return pnode->next; }
	iterator end()		{ return pnode; }
protected:
	link_type pnode;

	typedef allocator<listnode> node_allocator;
	link_type create_node(const T& x) {
		link_type p = node_allocator::allocate();
		construct(&p->data, x);
		return p;
	}

	void destroy_node(link_type p){
		destroy(&p->data);
		node_allocator::deallocate(p);
	}
public:
	//copy,cstor,dcstor
	list(){
		pnode = node_allocator::allocate();
		pnode->next = pnode;
		pnode->pre = pnode;
	}
	explicit list(size_type n);
	list(size_type n, const value_type& value);

public:
	//data access
	reference front() { return *begin(); }
	reference back() { return *(--end()); }

public:
	// capcity
	bool empty() const { return pnode->next == pnode; }
	size_type size() {
		size_type ret = 0;
		iterator first = begin();
		iterator last = end();
		while (first != last){
			++first;
			ret++;
		}
		return ret;
	};

public:
	//modifiers
	void clear(){ 
		link_type cur = (link_type)pnode->next;
		while (cur != pnode){
			link_type tmp = cur;
			cur = (link_type)cur->next;
			destroy_node(tmp);
		}
		pnode->next = pnode;
		pnode->pre = pnode;
	}

	void push_back(const value_type& val) { insert(end(), val); }

	iterator insert(iterator pos, const T& val){
		link_type tmp = create_node(val);
		tmp->next = pos.node;
		tmp->pre = pos.node->pre;
		tmp->pre->next = tmp;
		pos.node->pre = tmp;
		return tmp;
	}

	void pop_back(){
		iterator tmp = end();
		erase(--tmp);
	}

	void push_front(const value_type& val) { insert(begin(), val); }
	void pop_front(){ erase(begin())};

	iterator erase(const_iterator pos){ 
		link_type next_node = link_type(pos.node->next);
		link_type pre_node = link_type(pos.node->pre);
		pre_node->next = next_node;
		next_node->pre = pre_node;
		destroy(pos.node);
		return iterator(next_node);
	}
	iterator erase(const_iterator first, const_iterator last);

public:
	//operations
	void merge(list<T>& other);
	void splice(const_iterator pos, list<T>& other);
	void splice(const_iterator pos, list<T>& other, const_iterator first, const_iterator last);
	void sort();
	void unique(){ 
		iterator first = begin();
		iterator last = end();
		if (first == last) return;
		iterator next = first;
		while (++next != last){
			if (*next == *first)
				erase(next);
			else first == next;
			next = first;
		}
	}
	void reverse();
};
}