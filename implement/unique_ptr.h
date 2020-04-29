
template <T>
class unique_ptr
{
public:
	unique_ptr(T* p = nullptr) :ptr(p){};
	unique_ptr(const unique_ptr& rhs) = delete;
	unique_ptr& operator=(const unique& rhs) = delete;
	~unique_ptr(){
		if (ptr) delete ptr;
	}

	T* operator*(){ return ptr };
	T& operator->(){ return *ptr };

	void reset(T* q = nullptr){
		if (q != ptr){
			if (ptr) delete ptr;
			ptr = q;
		}
	}

	T* release(){
		T* res = ptr;
		ptr = nullptr;
		return res;
	}
private:
	T* ptr;
};
