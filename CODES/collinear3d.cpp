/* Współliniowość 3D */

inline static const bool arePointsCollinear(const long double &x1, const long double &y1, const long double &z1, const long double &x2, const long double &y2, const long double &z2, const long double &x3, const long double &y3, const long double &z3)
{
	/*
	 * | x3-x1 x2-x1 1 |
	 * | y3-y1 y2-y1 1 | = 0
	 * | z3-z1 z2-z1 1 |
	 *
	 */
	long double val = (x2 - x1) * (z3 - z1) + (y2 - y1) * (x3 - x1) + (z2 - z1) * (y3 - y1) - (x2 - x1) * (y3 - y1) - (y2 - y1) * (z3 - z1) - (z2 - z1) * (x3 - x1);
	return val < 0.000001L && val > -0.000001L;
}
