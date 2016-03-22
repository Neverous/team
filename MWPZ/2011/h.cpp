/* 2011
 * Maciej Szeptuch
 * II UWr
 * ----------
 */
#include<cstdio>
#include<vector>
#include<algorithm>
#include<cmath>

int points,
	tests;
long double res,
	 		act;
std::vector<std::pair<int, int> > point,
								  convex;

std::pair<int, int> ref;
 long double MAX( long double a,  long double b){return a>b?a:b;}
 long double MIN( long double a,  long double b){return a<b?a:b;}

 bool specialSort( std::pair<int, int> A,  std::pair<int, int> B);
 int cross( std::pair<int, int> A,  std::pair<int, int> B,  std::pair<int, int> C);
  void convexHull(std::vector<std::pair<int, int> > point, std::vector<std::pair<int, int> > &convex);
 long double dist( std::pair<int, int> A,  std::pair<int, int> B);

int main(void)
{
	scanf("%d", &tests);
	for(int t = 0; t < tests; ++ t)
	{
		scanf("%d", &points);
		point.resize(points);
		for(int p = 0; p < points; ++ p)
			scanf("%d %d", &point[p].first, &point[p].second);

		if(points < 3)
		{
			puts("0.00");
			continue;
		}

		convexHull(point, convex);
		res = 1000000000L;
		for(unsigned int c = 0; c < convex.size(); ++ c)
		{
			act = 0;
			for(unsigned int p = 0; p < convex.size(); ++ p)
				act = MAX(act, 1.0 * cross(convex[c], convex[(c + 1) % convex.size()], convex[p]) * 1.0 / dist(convex[c], convex[(c + 1) % convex.size()]));

			res = MIN(res, act);
		}

		printf("%.2Lf\n", res);
	}

	return 0;
}

 long double dist( std::pair<int, int> A,  std::pair<int, int> B)
{
	return sqrtl((A.first - B.first) * (A.first - B.first) + (A.second - B.second) * (A.second - B.second));
}

  void convexHull(std::vector<std::pair<int, int> > point, std::vector<std::pair<int, int> > &convex)
{
	ref = point[0];
	for(int p = 0; p < points; ++ p)
		if(point[p] < ref)
			ref = point[p];

	sort(point.begin(), point.end(), specialSort);
	convex.clear();
	for(int p = 0; p < points; ++ p)
	{
		while(convex.size() > 1 && cross(convex[convex.size() - 2], convex.back(), point[p]) <= 0)
			convex.pop_back();

		convex.push_back(point[p]);
	}

	return;
}

 bool specialSort( std::pair<int, int> A,  std::pair<int, int> B)
{
	long long int side = cross(ref, A, B);

	if(side > 0)
		return true;

	if(side == 0)
		return A < B;

	return false;
}

 int cross( std::pair<int, int> A,  std::pair<int, int> B,  std::pair<int, int> C)
{
	return (B.first - A.first) * (C.second - A.second) - (B.second - A.second) * (C.first - A.first);
}
