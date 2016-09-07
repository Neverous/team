/* Współliniowość 2D */

inline static const bool arePointsCollinear(const long double &x1, const long double &y1, const long double &x2, const long double &y2, const long double &x3, const long double &y3)
{
	/*
	 * | x1 y1 1 |
	 * | x2 y2 1 | = 0
	 * | x3 y3 1 |
	 *
	 */
	long double val = y1 * x3 + y2 * x1 + y3 * x2 - y1 * x2 - y2 * x3 - y3 * x1;
	return val < 0.000001L && val > -0.000001L;
}
