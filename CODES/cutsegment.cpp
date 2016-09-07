/* Sprawdź czy dane odcinki się przecinają. */

inline static const long long int cross(const long long int &xA, const long long int &yA, const long long int &xB, const long long int &yB, const long long int &x, const long long int &y)
{
	return (xB - xA) * (y - yA) - (yB - yA) * (x - xA);
}

cross(x1, y1, x2, y2, x3, y3) * cross(x1, y1, x2, y2, x4, y4) <= 0 && cross(x3, y3, x4, y4, x1, y1) * cross(x3, y3, x4, y4, x2, y2) <= 0 ?"Tak":"Nie";
