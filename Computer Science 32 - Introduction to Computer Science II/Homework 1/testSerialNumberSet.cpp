#include "SerialNumberSet.h"
#include "Set.h"
#include <cassert>

 int main()
{
	 SerialNumberSet u;
	 assert(u.size() == 0);

	 SerialNumberSet u2;
	 u2.add(4);
	 u2.add(5);
	 u2.add(6);

	 u = u2;

	 u.print();
	 u2.print();

	 SerialNumberSet u3(u);
	 assert(u3.size() == 3); 
	 u3.print();




	return 0;
}
