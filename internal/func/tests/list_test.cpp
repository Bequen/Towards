#include "list.hpp"

#include <assert.h>

int main() {
	int *pArray = (int*)malloc(10 * sizeof(int));
	List<int> l = List<int>(10, pArray);

	return 0;
}
