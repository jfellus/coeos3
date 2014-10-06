/*
 * geom.h
 *
 *  Created on: 2 oct. 2014
 *      Author: jfellus
 */

#ifndef GEOM_H_
#define GEOM_H_

#include <math.h>
#include <vector>
#include "utils.h"


#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

int sgn(double s);


class Vector2D;


class Vector2D {
public:
	double x,y;
public:
	Vector2D() {this->x = this->y = 0;}
	Vector2D(double x, double y) { this->x = x; this->y = y;}

	Vector2D operator*(double s)  {return Vector2D(x*s,y*s);}
	const Vector2D& operator*=(double s)  {x*=s;y*=s; return *this;}
	Vector2D operator/(double s)  {return Vector2D(x/s,y/s);}
	const Vector2D& operator/=(double s)  {x/=s;y/=s; return *this;}
	Vector2D operator+(const Vector2D& v) const {return Vector2D(x+v.x,y+v.y);}
	const Vector2D& operator+=(const Vector2D& v) {x+=v.x;y+=v.y; return *this;}
	Vector2D operator-(const Vector2D& v) const {return Vector2D(x-v.x,y-v.y);}
	const Vector2D& operator-=(const Vector2D& v) {x-=v.x;y-=v.y; return *this;}

	bool operator==(const Vector2D& v) {return x==v.x && y==v.y;}

	double dist_sq(const Vector2D& v) {return (v.x-x)*(v.x-x) + (v.y-y)*(v.y-y);}
	double dist(const Vector2D& v) {return sqrt(dist_sq(v));}
	double dot(const Vector2D& v) const {return x*v.x + y*v.y; }

	double norm_sq() const {return this->dot(*this);}
	double norm() const {return sqrt(norm_sq());}
	const Vector2D& normalize() {return *this/=norm();}
};


class Rectangle {
public:
	double x,y,w,h;
	bool empty = true;
public:
	Rectangle(double x, double y, double w, double h) {
		empty = false;
		this->x = x; this->y = y; this->w = w; this->h = h;
		correct();
	}
	Rectangle() {this->x = this->y = this->w = this->h = 0; empty = true;}
	Rectangle(const Rectangle& r) {
		x = r.x; y = r.y; w = r.w; h = r.h; empty = r.empty;
	}

	void copy(double* x, double* y, double* w, double* h) {*x = this->x; *y = this->y; *w = this->w; *h = this->h; empty = false;}

	bool contains(double x, double y) {
		if(empty) return false;
		return x>=this->x && x<=this->x+w && y>=this->y && y<=this->y+h;
	}

	void add(const Rectangle& r) {
		DBG("------" << (bool)r);
		if(!r) return;
		if(empty) {
			x = r.x; y = r.y; w = r.w; h=r.h;
			DBG("ok " << *this);
			empty = false;
			return;
		}
		if(r.x < x)  {w += x-r.x; x = r.x;}
		if(r.y < y)  {h += y-r.y; y = r.y;}
		if(r.x+r.w > x+w)  {w = r.x+r.w - x;}
		if(r.y+r.h > y+h)  {h = r.y+r.h - y;}
	}

	void add(const Vector2D& p) { add(p.x,p.y);}
	void add(double px, double py) {
		empty = false;
		if(px < x) {w += x-px; x = px;}
		if(py < y) {h += y-py; y = py;}
		if(px > x+w) {w = px - x;}
		if(py > y+h) {h = py - y;}
	}

	const Rectangle& augment(double amount) {
		if(empty) return *this;
		x-=amount; y-=amount; w+=amount*2; h+=amount*2;
		return *this;
	}

	const Rectangle& reverse() {
		x+=w; w = -w;
		return *this;
	}

	Vector2D center();

	bool is_in(const Rectangle& r) {
		if(r.empty || empty) return false;
		return x >= r.x && x+w <= r.x+r.w && y >= r.y && y+h <= r.y+r.h;
	}

	bool contains(Rectangle r) {
		return r.is_in(*this);
	}

	void correct() {
		if(empty) return;
		if(w<0) {x += w; w = -w;}
		if(h<0) {y += h; h = -h;}
	}

	bool is_empty() {return empty;}
	operator bool() const {return !empty;}

	Rectangle operator+(const Vector2D& v) {return Rectangle(x+v.x,y+v.y,w, h);	}

};
std::ostream& operator<<(std::ostream& os, const Rectangle& r);



class Line2D {
public:
	Vector2D a,b;
public:
	Line2D(const Vector2D& a, const Vector2D& b) {this->a=a;this->b=b;}
	Line2D(double x1, double y1, double x2, double y2) {
		a.x = x1; a.y = y1; b.x = x2; b.y = y2;
	}

	Vector2D normal() {
		return Vector2D(a.y-b.y, b.x-a.x).normalize();
	}

	double distance(const Vector2D& p) {
		Vector2D v = p - a;
		Vector2D s = b - a;
		double d = v.dot(s)/s.dot(s);
		if(d < 0 || d > 1) return -1;
		s *= d;
		s += a;
		return (p-s).norm();
	}

	bool hasPoint(Vector2D p) {
		double d = location(p);
		return d <= 1 && d >= 0;
	}

	double location(Vector2D p) {
		p -= a;
		Vector2D s = b - a;
		return p.dot(s)/s.dot(s);
	}

	Vector2D project(Vector2D p) {
		p -= a;
		Vector2D s = b - a;
		double d = p.dot(s)/s.dot(s);
		s *= d;
		return a+s;
	}
};


class Bezier {
public:
	int maxRecursion = 64;
	float flatnessTolerance = pow(2.0,-maxRecursion-1);

	double x1,x2,y1,y2,x3,y3,x4,y4;
public:
	Bezier(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
		this->x1 = x1; this->x2 = x2; this->x3 = x3; this->x4 = x4;
		this->y1 = y1; this->y2 = y2; this->y3 = y3; this->y4 = y4;
	}

	Vector2D start() {return Vector2D(x1,y1);}
	Vector2D end() {return Vector2D(x4,y4);}
	Vector2D start_ctrl() {return Vector2D(x2,y2);}
	Vector2D end_ctrl() {return Vector2D(x3,y3);}

	Vector2D pt(int i) {
		if(i==0) return start();
		if(i==1) return start_ctrl();
		if(i==2) return end_ctrl();
		if(i==3) return end();
		DBG("Wrong number" << i); *((int*)0)=0;
	}

	Rectangle get_bounds(); // Rough upper bound
	Rectangle get_precise_bounds(); // Uses cairo (...expensive)

	bool has_point(double x, double y, double tolerance);


	/** Calculates the distance that the point lies from the curve. */
	double distance(const Vector2D& point, double *location = 0) {
		std::vector<double> candidates;
		std::vector<Vector2D> w = convertToBezier(point);
		int degree = 3;
		int higherDegree = (2 * degree) - 1;
		int numSolutions = findRoots(w, higherDegree, candidates, 0);
		Vector2D v = point - start();
		double dist = v.norm_sq();
		double t = 0.0;

		for (int i = 0; i < numSolutions; i++) {
			v = point - bezier(pts(), degree, candidates[i], 0, 0);
			double newDist = v.norm_sq();
			if (newDist < dist) {
				dist = newDist;
				t = candidates[i];
			}
		}
		v = point - end();
		double newDist = v.norm_sq();
		if (newDist < dist) {
			dist = newDist;
			t = 1.0;
		}
		if(location) *location = t;
		return dist;
	}

	/** finds the nearest point on the curve to the given point. */
	Vector2D nearestPointOnCurve(const Vector2D& point, double* location = 0) {
		double loc;
		distance(point, &loc);
		if(location) *location = loc;
		return pointOnPath(loc);
	};

	std::vector<Vector2D> pts() {
		std::vector<Vector2D> v;
		v.push_back(Vector2D(x1,y1));
		v.push_back(Vector2D(x2,y2));
		v.push_back(Vector2D(x3,y3));
		v.push_back(Vector2D(x4,y4));
		return v;
	}

	std::vector<Vector2D> convertToBezier(const Vector2D& point) {
		int degree = 3;
		int higherDegree = (2 * degree) - 1;
		Vector2D c[degree];
		Vector2D d[degree-1];
		double cdTable[2][3];
		double z[3][4] =  { {1.0, 0.6, 0.3, 0.1}, {0.4, 0.6, 0.6, 0.4}, {0.1, 0.3, 0.6, 1.0}};
		std::vector<Vector2D> w;

		for (int i = 0; i <= degree; i++) c[i] = pt(i) - point;
		for (int i = 0; i <= degree - 1; i++) {
			d[i] = pt(i+1) - pt(i);
			d[i] *= 3;
		}
		for (int row = 0; row <= degree - 1; row++) {
			for (int column = 0; column <= degree; column++) {
				cdTable[row][column] = d[row].dot(c[column]);
			}
		}
		for (int i = 0; i <= higherDegree; i++) w.push_back(Vector2D((float)i/higherDegree, 0));
		int n = degree, m = degree-1;
		for (int k = 0; k <= n + m; k++) {
			int lb = MAX(0, k - m), ub = MIN(k, n);
			for (int i = lb; i <= ub; i++) {
				int j = k - i;
				w[i+j].y += cdTable[j][i] * z[j][i];
			}
		}
		return w;
	}


	/** counts how many roots there are. */
	int findRoots(const std::vector<Vector2D>& w, int degree, std::vector<double>& t, int depth) {
		switch (getCrossingCount(w, degree)) {
		case 0 : return 0;
		case 1 :
			if (depth >= maxRecursion) {	 t.push_back((w[0].x + w[degree].x) / 2.0);	 return 1;	 }
			if (isFlatEnough(w, degree))  {	 t.push_back(computeXIntercept(w, degree));	 return 1;	 }
			break;
		}

		std::vector<Vector2D> left;
		std::vector<Vector2D> right;
		bezier(w, degree, 0.5, &left, &right);

		std::vector<double> left_t;
		std::vector<double> right_t;
		int left_count  = findRoots(left,  degree, left_t, depth+1);
		int right_count = findRoots(right, degree, right_t, depth+1);

		for (int i = 0; i < left_count; i++) t.push_back(left_t[i]);
		for (int i = 0; i < right_count; i++) t.push_back(right_t[i]);
		return (left_count+right_count);
	}


	int getCrossingCount(std::vector<Vector2D> w, int degree) {
		int n_crossings = 0, sign, old_sign;
		sign = old_sign = sgn(w[0].y);
		for (int i = 1; i <= degree; i++) {
			sign = sgn(w[i].y);
			if (sign != old_sign) n_crossings++;
			old_sign = sign;
		}
		return n_crossings;
	}


	bool isFlatEnough(const std::vector<Vector2D>& w, int degree) {
		double  error, intercept_1, intercept_2, left_intercept, right_intercept,
		a, b, c, det, dInv, a1, b1, c1, a2, b2, c2;
		a = w[0].y - w[degree].y;
		b = w[degree].x - w[0].x;
		c = w[0].x * w[degree].y - w[degree].x * w[0].y;

		double max_distance_above = 0.0, max_distance_below = 0.0;

		for (int i = 1; i < degree; i++) {
			double value = a * w[i].x + b * w[i].y + c;
			if (value > max_distance_above)       max_distance_above = value;
			else if (value < max_distance_below)  max_distance_below = value;
		}

		a1 = 0.0; b1 = 1.0; c1 = 0.0; a2 = a; b2 = b;
		c2 = c - max_distance_above;
		det = a1 * b2 - a2 * b1;
		dInv = 1.0/det;
		intercept_1 = (b1 * c2 - b2 * c1) * dInv;
		a2 = a; b2 = b; c2 = c - max_distance_below;
		det = a1 * b2 - a2 * b1;
		dInv = 1.0/det;
		intercept_2 = (b1 * c2 - b2 * c1) * dInv;
		left_intercept = MIN(intercept_1, intercept_2);
		right_intercept = MAX(intercept_1, intercept_2);
		error = right_intercept - left_intercept;
		return (error < flatnessTolerance)? 1 : 0;
	}


	double computeXIntercept(const std::vector<Vector2D>& w, int degree) {
		double XLK = 1.0, YLK = 0.0,
				XNM = w[degree].x - w[0].x, YNM = w[degree].y - w[0].y,
				XMK = w[0].x - 0.0, YMK = w[0].y - 0.0,
				det = XNM*YLK - YNM*XLK, detInv = 1.0/det,
				S = (XNM*YMK - YNM*XMK) * detInv;
		return 0.0 + XLK * S;
	}

	/** left and right must be of size degree */
	Vector2D bezier(const std::vector<Vector2D>& w, int degree, double t, std::vector<Vector2D>* left, std::vector<Vector2D>* right) {
		Vector2D temp[degree+1][degree+1];
		for (int j = 0; j <= degree; j++) temp[0][j] = w[j];
		for (int i = 1; i <= degree; i++) {
			for (int j =0 ; j <= degree - i; j++) {
				temp[i][j].x = (1.0 - t) * temp[i-1][j].x + t * temp[i-1][j+1].x;
				temp[i][j].y = (1.0 - t) * temp[i-1][j].y + t * temp[i-1][j+1].y;
			}
		}
		if (left)
			for (int j = 0; j <= degree; j++) left->push_back(temp[j][0]);
		if (right)
			for (int j = 0; j <= degree; j++) right->push_back(temp[degree-j][j]);

		return (temp[degree][0]);
	}

	/**
	 * calculates a point on the curve, for a Bezier of order 3.
	 * @param location a decimal indicating the distance along the curve the point should be located at.  this is the distance along the curve as it travels, taking the way it bends into account.  should be a number from 0 to 1, inclusive.
	 */
	Vector2D pointOnPath_o2(double t) {
		double t2 = t*t;
		double mt = 1-t;
		double mt2 = mt*mt;
		return Vector2D(
				x1*mt2 + x2*2*mt*t + x3*t2,
				y1*mt2 + y2*2*mt*t + y3*t2
		);
	}

	/**
	 * calculates a point on the curve, for a Bezier of order 2.
	 * @param location a decimal indicating the distance along the curve the point should be located at.  this is the distance along the curve as it travels, taking the way it bends into account.  should be a number from 0 to 1, inclusive.
	 */
	Vector2D pointOnPath(double t) {
		double t2 = t*t;
		double t3 = t2*t;
		double mt = 1-t;
		double mt2 = mt*mt;
		double mt3 = mt2*mt;
		return Vector2D(
				x1*mt3 + x2*3*mt2*t + x3*3*mt*t2 + x4*t3,
				y1*mt3 + y2*3*mt2*t + y3*3*mt*t2 + y4*t3
		);
	}

	bool isPoint() {
		return start() == start_ctrl();
	}

	/**
	 * finds the point that is 'distance' along the path from 'location'.  this method returns both the x,y location of the point and also
	 * its 'location' (proportion of travel along the path); the method below - _pointAlongPathFrom - calls this method and just returns the
	 * point.
	 */
	Vector2D pointAlongPath(double location, double distance, double* newloc = 0) {

		if (isPoint()) {
			if(newloc) *newloc = location;
			return start();
		}

		Vector2D prev = pointOnPath(location);
		double tally = 0;
		double curLoc = location;
		int direction = distance > 0 ? 1 : -1;
		Vector2D cur;

		while (tally < fabs(distance)) {
			curLoc += (0.005 * direction);
			cur = pointOnPath(curLoc);
			tally += cur.dist(prev);
			prev = cur;
		}

		if(newloc) *newloc = curLoc;
		return cur;
	}

	double length() {
		if (isPoint()) return 0;

		Vector2D prev = start();
		double tally = 0, curLoc = 0;
		int direction = 1;
		Vector2D cur;

		while (curLoc < 1) {
			curLoc += (0.005 * direction);
			cur = pointOnPath(curLoc);
			tally += cur.dist(prev);
			prev = cur;
		}
		return tally;
	}

	/** finds the point that is 'distance' along the path from 'location' */
	Vector2D pointAlongPathFrom(double location, double distance) {
		return pointAlongPath(location, distance);
	}

	/** finds the location that is 'distance' along the path from 'location' */
	double locationAlongPathFrom(double location, double distance) {
		double l;
		pointAlongPath(location, distance, &l);
		return l;
	}

	/** returns the gradient of the curve at the given location, which is a decimal between 0 and 1 inclusive. */
	double gradientAtPoint(double location) {
		Vector2D p1 = pointOnPath(location), p2 = pointOnPath_o2(location);
		double dy = p2.y - p1.y, dx = p2.x - p1.x;
		return dy == 0 ? NAN : atan(dy / dx);
	}

	/**
		returns the gradient of the curve at the point which is 'distance' from the given location.
		if this point is greater than location 1, the gradient at location 1 is returned.
		if this point is less than location 0, the gradient at location 0 is returned.
	 */
	double gradientAtPointAlongPathFrom(double location, double distance) {
		double l = locationAlongPathFrom(location, distance);
		if (l > 1) l = 1;
		if (l < 0) l = 0;
		return gradientAtPoint(l);
	}

	/**
	 * calculates a line that is 'length' pixels long, perpendicular to, and centered on, the path at 'distance' pixels from the given location.
	 * if distance is not supplied, the perpendicular for the given location is computed (ie. we set distance to zero).
	 */
	Line2D normalToPathAt(double location, double length, double distance = 0) {
		double l = location;
		Vector2D p = distance ? pointAlongPath(location, distance, &l) : pointOnPath(location);
		double m = gradientAtPoint(l);
		double _theta2 = !isnan(m) ? atan(-1 / m) : NAN;
		double x = isnan(_theta2) ? length : length * cos(_theta2);
		double y = isnan(_theta2) ? 0 : length * sin(_theta2);
		return Line2D(p.x, p.y, p.x + x, p.y + y);
	}
};


#endif /* GEOM_H_ */
