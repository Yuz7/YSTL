#include "yalloc.h"
#include <cassert>

namespace ystl{
	template<class T>
	class allocator{
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

	public:
		static T *allocate(){
			return static_cast<T*>(alloc::allocate(sizeof(T)));
		}
		static T *allocate(size_t n){
			if (n == 0) return 0;
			return static_cast<T*>(alloc::allocate(sizeof(T) * n));
		}
		static void deallocate(T *ptr){ 
			alloc::deallocate(static_cast<void*>(ptr),sizeof(T)); 
		}
		static void deallocate(T *ptr, size_t n){
			if(0!=n) alloc::deallocate(static_cast<void*>(ptr),n*sizeof(T));
		}
	};
}