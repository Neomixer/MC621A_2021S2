typedef long long ll;

struct xtend {ll x, y, d;};
struct sol {ll x, k;};

xtend Euclid(ll a, ll b) {
	if (b == 0)
		return {1, 0, a};
	xtend aux = Euclid(b, a%b);
	return {aux.y, aux.x - a/b*aux.y, aux.d};
}

sol solveCongruence(ll a, ll b, ll m) { // ax = b (mod m)
	xtend aux = Euclid(a, m);
	return {b / aux.d * aux.x, m / aux.d};
}

sol CRT(ll a, ll m, ll b, ll n) { //generalized Chinese remainder theorem
	xtend aux = Euclid(m, n);
	ll lcm = m/g*n;
	ll ans; //((a*v*n + b*u*m)/g)%lcm;
	ll g = aux.d, u = aux.x, v = aux.y;
	if (a % g != b % g)
		return {0, 0};
	ans = n*((a*v) % m/g);
	ans = (ans + m*((b*u) % n/g)) % lcm;
	return {ans, lcm};
}

sol nCRT(ll[] a, ll[] m, ll n) { //CRT for system of n equations
	sol aux = CRT(a[0], m[0], a[1], m[1]);
	for (ll i = 2; i < n; i++)
		aux = CRT(aux.x, aux.k, a[i], m[i]);
	return aux;
}