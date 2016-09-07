/* Otoczka wypukła algorytmem A. M. Andrew "Monotone Chain" */

inline static const long long int cross(const std::pair<int, int> &A, const std::pair<int, int> &B, const std::pair<int, int> &C);
inline static void convexHull(std::vector<std::pair<int, int> > point, std::vector<std::pair<int, int> > &convex);

inline static void convexHull(std::vector<std::pair<int, int> > point, std::vector<std::pair<int, int> > &convex)
{
	std::sort(point.begin(), point.end());
	std::vector<int> lower,
					 upper;

	/* LOWER HULL */
	for(long unsigned int p = 0; p < point.size(); ++ p)
	{
		while(lower.size() > 1 && cross(point[lower[lower.size() - 2]], point[lower.back()], point[p]) <= 0)
			lower.pop_back();

		lower.push_back(p);
	}

	/* UPPER HULL */
	for(long int p = point.size() - 1; p >= 0; -- p)
	{
		while(upper.size() > 1 && cross(point[upper[upper.size() - 2]], point[upper.back()], point[p]) <= 0)
			upper.pop_back();

		upper.push_back(p);
	}

	convex.clear();
	for(unsigned int l = 0; l < lower.size(); ++ l)
		convex.push_back(point[lower[l]]);

	for(unsigned int u = 1; u + 1 < upper.size(); ++ u)
		convex.push_back(point[upper[u]]);

	return;
}

inline static const long long int cross(const std::pair<int, int> &A, const std::pair<int, int> &B, const std::pair<int, int> &C)
{
	return (long long int)(B.first - A.first) * (C.second - A.second) - (long long int)(B.second - A.second) * (C.first - A.first);
}
