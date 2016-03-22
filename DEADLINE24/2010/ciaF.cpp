/* 2010
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>

class Matrix
{
	public:
		long long int a,
			b,
			c,
			d,
			MOD;

		Matrix(long long int A = 0, long long int B = 0, long long int C = 0, long long int D = 0, long long int mod = 1000000000);
		Matrix operator*(const Matrix &N) const;
		void print(void);
};

Matrix fastPower(Matrix M, long long int n, long long int &mod);

long long int tests,
	modulo,
	st,
	nd,
	need;

int main(void)
{
	scanf("%lld %lld", &modulo, &tests);
	for(long long int t = 0; t < tests; ++ t)
	{
		scanf("%lld %lld %lld", &st, &nd, &need);
		printf("%lld\n", (fastPower(Matrix(1, 2, 1, 0, modulo), need, modulo) * Matrix(nd, 0, st, 0, modulo)).c % modulo);
	}

	return 0;
}

Matrix::Matrix(long long int A, long long int B, long long int C, long long int D, long long int mod)
{
	a = A;
	b = B;
	c = C;
	d = D;
	MOD = mod;
	return;
}

Matrix Matrix::operator*(const Matrix &N) const
{
	return Matrix(
		((long long int)a * N.a + (long long int)b * N.c) % MOD,
		((long long int)a * N.b + (long long int)b * N.d) % MOD,
		((long long int)c * N.a + (long long int)d * N.c) % MOD,
		((long long int)c * N.b + (long long int)d * N.d) % MOD
	);
}

void Matrix::print(void)
{
	printf("[%lld %lld]\n[%lld %lld]\n\n", a, b,c, d);
	return;
}

Matrix fastPower(Matrix M, long long int n, long long int &mod)
{
	if(n == 0)
		return Matrix(1, 0, 0, 1, mod);

	if(n % 2)
		return fastPower(M, n - 1, mod) * M;

	Matrix half = fastPower(M, n / 2, mod);
	return half * half;
}
