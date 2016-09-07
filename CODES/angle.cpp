/* Kąt między wektorami w 3d */
long long int a[3],
			  b[3];

long double angle = acosl((a[0] * b[0] + a[1] * b[1] + a[2] * b[2]) / sqrtl(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]) / sqrtl(b[0] * b[0] + b[1] * b[1] + b[2] * b[2])) * 180 / M_PI;
