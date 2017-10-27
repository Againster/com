#include <iostream>

namespace mine{
	template <class T>
	class auto_ptr
	{
	private:
		T* const m_ptr;
	public:
		auto_ptr(T* const ptr) : m_ptr(ptr) { }
		~auto_ptr() { delete (m_ptr); }

		operator T*() { return (m_ptr); }

		T* operator ->() { return (m_ptr); }
		T& operator *() { return (*m_ptr); }
	};
}
using namespace mine;
int main(){
	int* y = new int(7);
	auto_ptr<int> x(y);
	*x=8;// will call function "T& operator *() { return (*m_ptr); }".
	int *z=(int *)x;// will call function "operator T*() { return (m_ptr); }".
	return 0;
}