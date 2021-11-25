#include <utility>
using namespace std;

typedef pair<int,int> ii;

int f(int x);

//in repository
ii floydCycleFinding(int x0) { // function int f(int x) is defined earlier
	//finding k*mu
	int tortoise = f(x0), hare = f(f(x0));
	while (tortoise != hare) {tortoise = f(tortoise); hare = f(f(hare));}
	//finding mu (start of cycle)
	int mu = 0; hare = x0;
	while (tortoise != hare) {tortoise = f(tortoise); hare = f(hare); mu++;}
	//finding lambda (cycle)
	int lambda = 1; hare = f(tortoise);
	while (tortoise != hare) {hare = f(hare); lambda++;}
	return make_pair(mu, lambda);
}