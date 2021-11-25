#include <cmath>
#include <vector>
using namespace std;

#define EPS (1e-9)

//struct and related functions in repository
struct point { double x ,y;	// only used if more precision is needed
	point() { x = y = 0.0; }	// default constructor
	point(double _x, double _y) : x(_x), y(_y) {} // user−defined
	bool operator < (point other) const {
		if(fabs(x - other.x) > EPS)
			return x < other.x;
		else if(fabs(y - other.y) > EPS)
			return y < other.y;
		return 0; }
	bool operator == (point other) const {
		return (fabs(x - other.x) < EPS && (fabs(y - other.y) < EPS));}};

double dist(point p1, point p2) {	//Euclidean distance
	// hypot(dx, dy) returns sqrt(dx * dx + dy * dy)
	return hypot(p1.x - p2.x, p1.y - p2.y); }

//struct and related functions in repository
struct line { double a, b, c; };

//answer stored in the third parameter
void pointsToLine(point p1, point p2, line &l) {
	if(fabs(p1.x - p2.x) < EPS) { // vertical line is fine
		l.a = 1.0;	l.b = 0.0;	l.c = -p1.x; // default values
	}else{
		l.a = -(double)(p1.y-p2.y)/(p1.x-p2.x);
		l.b = 1.0;	// IMPORTANT: we fix the value of b to 1.0
		l.c = -(double)(l.a*p1.x) - p1.y;
	}
}


bool areParallel(line l1, line l2) {
	return (fabs(l1.a-l2.a) < EPS) && (fabs(l1.a-l2.a) < EPS); }

bool areSame(line l1, line l2) {
	return areParallel(l1, l2) && (fabs(l1.c-l2.c) < EPS); }

// returns true(+intersection point) if two lines are intersect
bool areIntersect(line l1, line l2, point &p){
	if(areParallel(l1,l2)) return false;// no intersection
	// solv system of 2 linear algebraic equations with 2  unknowns
	p.x=(l2.b*l1.c-l1.b*l2.c)/(l2.a*l1.b-l1.a*l2.b);
	// special case: test for vertical line to avoid division by zero
	if(fabs(l1.b)>EPS)p.y=-(l1.a*p.x+l1.c);
	else	p.y=-(l2.a*p.x+l2.c);
	return true; }

//struct and related functions in repository
struct vec { double x, y; // name: vec is different from STL vector
	vec(double _x, double _y):x(_x),y(_y) {} };

vec toVec(point a, point b) {
	return vec(b.x - a.x, b.y - a.y);}

vec scale(vec v, double s){ // nonnegative s = [<1 .. 1 .. >1]
	return vec(v.x*s, v.y*s);}							// shorter.same.longer

point translate(point p, vec v){// translate p according to v
	return point(p.x+v.x, p.y+v.y);}

double cross(vec a, vec b) { return a.x * b.y - a.y * b.x; }

//returns true if point r is on the left side of line pq
bool ccw(point p, point q, point r) {
	return cross(toVec(p, q), toVec(p, r)) > 0; }

//returns true if point r is on the same line as the line pq
bool collinear(point p, point q, point r) {
	return fabs(cross(toVec(p, q), toVec(p, r))) < EPS; }

double perimeter(const vector<point> &P) {
	double result = 0.0;
	for (int i = 0; i < (int)P.size()-1; i++)
		result += dist(P[i], P[i+1]);
	return result; }

double area(const vector<point> &P) {
	double result = 0.0, x1, y1, x2, y2;
	for (int i = 0; i < (int)P.size()-1; i++) {
		x1 = P[i].x;	x2 = P[i+1].x;
		y1 = P[i].y;	y2 = P[i+1].y;
		result += (x1 * y2 - x2 * y1);
	}
	return fabs(result) / 2.0; }

// line segment p−q intersect with line A−B.
point lineIntersectSeg(point p, point q, point A, point B){
	point s;
	line l1, l2;
	pointsToLine(p,q,l1);
	pointsToLine(A,B,l2);
	areIntersect(l1,l2,s);
	return s;
}

//cuts polygon Q along the line formed by point a −> point b
//(note: the last point must be the same as the first point)
vector<point> cutPolygon(point a,point b,const vector<point> &Q){
	vector<point> P;
	for(int i = 0; i<(int)Q.size();i++){
		double left1=cross(toVec(a,b),toVec(a,Q[i])),left2=0;
		if(i!=(int)Q.size()-1)
			left2=cross(toVec(a,b),toVec(a,Q[i+1]));
		if(left1>-EPS) // Q[i] is on the left of ab
			P.push_back(Q[i]);
		if(left1*left2<-EPS)//edge(Q[i],Q[i+1])crosses line ab
			P.push_back(lineIntersectSeg(Q[i],Q[i+1],a,b));
		}
	if(!P.empty()&&!(P.back()==P.front()))
		P.push_back(P.front());//make P’s first point = P’slastpoint
	return P;
}