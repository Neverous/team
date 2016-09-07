/* Otoczka wypukła algorytmem Jarvisa */
std::pair<int, int> ref,
					end;

inline static const long long int cross(const std::pair<int, int> &A, const std::pair<int, int> &B, const std::pair<int, int> &C);
inline static void convexHull(std::vector<std::pair<int, int> > point, std::vector<std::pair<int, int> > &convex);

inline static void convexHull(std::vector<std::pair<int, int> > point, std::vector<std::pair<int, int> > &convex)
{
	/* FIND LEFTMOST */
	ref = point[0];
	for(long unsigned int p = 0; p < point.size(); ++ p)
		if(point[p] < ref)
			ref = point[p];

	convex.clear();
	do
	{
		while(convex.size() > 1 && !cross(convex[convex.size() - 2], convex.back(), ref))
			convex.pop_back();

		convex.push_back(ref);
		end = point[0];
		for(long unsigned int p = 1; p < point.size(); ++ p)
			if(end == ref || cross(convex.back(), end, point[p]) < 0)
				end = point[p];

		ref = end;
	}
	while(ref != convex.front());

	return;
}

inline static const long long int cross(const std::pair<int, int> &A, const std::pair<int, int> &B, const std::pair<int, int> &C)
{
	return (long long int)(B.first - A.first) * (C.second - A.second) - (B.second - A.second) * (C.first - A.first);
}
