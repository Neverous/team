/* Otoczka wypukła algorytmem Grahama */

std::pair<int, int> ref;

inline static const long long int cross(const std::pair<int, int> &A, const std::pair<int, int> &B, const std::pair<int, int> &C);
inline static const bool specialSort(const std::pair<int, int> &A, const std::pair<int, int> &B);
inline static void convexHull(std::vector<std::pair<int, int> > point, std::vector<std::pair<int, int> > &convex);

inline static void convexHull(std::vector<std::pair<int, int> > point, std::vector<std::pair<int, int> > &convex)
{
	ref = point[0];
	for(int p = 0; p < points; ++ p)
		if(point[p] < ref)
			ref = point[p];

	sort(point.begin(), point.end(), specialSort);

	for(int p = 0; p < points; ++ p)
	{
		while(convex.size() > 1 && cross(convex[convex.size() - 2], convex.back(), point[p]) <= 0)
			convex.pop_back();

		convex.push_back(point[p]);
	}

	return;
}

inline static const bool specialSort(const std::pair<int, int> &A, const std::pair<int, int> &B)
{
	long long int side = cross(ref, A, B);

	if(side > 0)
		return true;

	if(side == 0)
		return A < B;

	return false;
}

inline static const long long int cross(const std::pair<int, int> &A, const std::pair<int, int> &B, const std::pair<int, int> &C)
{
	return (long long int)(B.first - A.first) * (C.second - A.second) - (long long int)(B.second - A.second) * (C.first - A.first);
}
