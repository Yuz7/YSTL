#include <cstdlib>
#include <new>

namespace ystl{
	/*class malloc_alloc{
	private:
		static void *oom_malloc(size_t);
		static void *oom_realloc(void *, size_t);
		static void(*malloc_alloc_oom_handler) ();

	public:
		static void* allocate(size_t n)
		{
			void *result = malloc(n);
			if (result == 0) result = oom_malloc(n);
			return result;
		}

		static void deallocate(void *p, size_t)
		{
			free(p);
		}

		static void * reallocate(void *p, size_t, size_t new_sz)
		{
			void * result = realloc(p, new_sz);
			if (result == 0) result = oom_realloc(p, new_sz);
			return result;
		}

		static void(*set_malloc_handler(void(*f)()))(){
			void(*old)() = malloc_alloc_oom_handler;
			malloc_alloc_oom_handler = f;
			return (old);
		}
	};
	
	void(*malloc_alloc::malloc_alloc_oom_handler)() = 0;
	void *malloc_alloc::oom_malloc(size_t n)
	{
		void(*my_malloc_handler) ();
		void *result;

		for (;;){
			my_malloc_handler = malloc_alloc_oom_handler;
			if (my_malloc_handler == 0) { _THROW_BAD_ALLOC; }
			(*my_malloc_handler)();
			result = malloc(n);
			if (result) return (result);
		}
	}
	void * malloc_alloc::oom_realloc(void *p, size_t n)
	{
		void(*my_malloc_handler)();
		void *result;

		for (;;)
		{
			my_malloc_handler = malloc_alloc_oom_handler;
			if (my_malloc_handler == 0) { _THROW_BAD_ALLOC; }
			(*my_malloc_handler)();
			result = realloc(p, n);
			if (result) return(result);
		}
	}*/

	enum { Align = 8 };
	enum { MAXBYTES = 128 };
	enum { NFREELISTS = MAXBYTES / Align };
	class alloc
	{
	private:
		static size_t ROUND_UP(size_t bytes){
			return (bytes + Align - 1) &~ (Align - 1);
		}

	private:
		union obj{
			union obj * next;
			char client[1];
		};
	private:
		static obj* volatile freelist[NFREELISTS];
		static size_t FREELIST_INDEX(size_t bytes){
			return (bytes + Align - 1) / (Align - 1);
		}
		static void *refill(size_t n);
		static char *chunk_alloc(size_t size, int &nobjs);

		static char *start_free;
		static char *end_free;
		static size_t heap_size;

	public:
		static void * allocate(size_t n){
			obj* volatile *my_free_list;
			obj* result;
			if (n > (size_t)MAXBYTES) return malloc(n);/* (malloc_alloc::allocate(n))*/
			my_free_list = freelist + FREELIST_INDEX(n);
			result = *my_free_list;
			if (result == 0){
				void *r = refill(ROUND_UP(n));
				return r;
			}
			*my_free_list = result->next;
			return (result);
		}

		static void deallocate(void *p, size_t n){
			obj *q = (obj *)p;
			obj * volatile * my_free_list;

			if (n > (size_t)MAXBYTES){
				free(p);
				return;
			}

			my_free_list = freelist + FREELIST_INDEX(n);
			q->next = *my_free_list;
			*my_free_list = q;
		}

		static void * reallocate(void *p, size_t old_sz, size_t new_sz);
	};
	
	char* alloc::start_free = 0;
	char* alloc::end_free = 0;
	size_t alloc::heap_size = 0;

	alloc::obj* volatile alloc::freelist[NFREELISTS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	void* alloc::refill(size_t n){
		int nobjs = 20;
		
		char* chunk = chunk_alloc(n, nobjs);
		
		obj* volatile * my_free_list;
		obj* result;
		obj* current_obj, *next_obj;
		int i;

		if (nobjs == 1) return (chunk);
		my_free_list = freelist + FREELIST_INDEX(n);

		result = (obj *)chunk;
		*my_free_list = next_obj = (obj *)(chunk + n);

		for (i = 1;; i++){
			current_obj = next_obj;
			next_obj = (obj *)((char *)next_obj + n);
			if (nobjs - 1 == i){
				current_obj->next = 0;
				break;
			}
			else {
				current_obj->next = next_obj;
			}
		}
		return (result);
	}

	char* alloc::chunk_alloc(size_t size, int& nobjs)
	{
		char* result;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;

		if (bytes_left >= total_bytes){
			result = start_free;
			start_free += total_bytes;
			return(result);
		}
		else if (bytes_left >= size) {
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return(result);
		}
		else{
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			if (bytes_left > 0) {
				obj * volatile * my_free_list = freelist + FREELIST_INDEX(bytes_left);
				((obj *)start_free)->next = *my_free_list;
				*my_free_list = (obj *)start_free;
			}

			start_free = (char *)malloc(bytes_to_get);
			if (start_free == 0){
				int i;
				obj* volatile * my_free_list, *p;
				for (i = size; i <= MAXBYTES; i += Align){
					my_free_list = freelist + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p!=0){
						*my_free_list = p->next;
						start_free = (char *)p;
						end_free = start_free + i;
						return (chunk_alloc(size, nobjs));
					}
				}
				end_free = 0;
				/*void *result = malloc(n);
				if (result == 0) result = oom_malloc(n);
				return result;*/
				start_free = (char *) malloc(bytes_to_get);
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return (chunk_alloc(size, nobjs));
		}
	}

}