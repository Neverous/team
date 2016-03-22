/* 2011
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
#include<algorithm>
#include<vector>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

int points,
	height[1048576],
	leftHigh[1048576],
	rightHigh[1048576],
	res;

class Queue
{
	public:
		int data[1048576],
			d;
		void push(const int &v);
		int front(void);
};

Queue que;
std::vector<std::pair<int, int> > xx;

int main(void)
{
	scanf("%d", &points);
	for(int p = 0; p < points; ++ p)
		scanf("%d", &height[p]);

	for(int p = 0; p < points; ++ p)
	{
		que.push(p);
		leftHigh[p] = que.front();
	}

	que.d = 0;

	for(int p = points - 1; p >= 0; -- p)
	{
		que.push(p);
		rightHigh[p] = que.front();
	}

	for(int p = 0; p < points; ++ p)
	{
		if(leftHigh[p] != -1)
			xx.push_back(std::make_pair(leftHigh[p], p));

		if(rightHigh[p] != -1)
			xx.push_back(std::make_pair(p, rightHigh[p]));

		res += (leftHigh[p] != -1) + (rightHigh[p] != -1);
	}

	printf("%d\n", res);
	std::sort(xx.begin(), xx.end());
	for(int s = 0; s < 100 && s < xx.size(); ++ s)
		printf("%d %d\n", xx[s].first + 1, xx[s].second + 1);


	return 0;
}

void Queue::push(const int &v)
{
	while(this->d && height[this->data[this->d - 1]] <= height[v])
		-- this->d;

	this->data[this->d ++] = v;
	return;
}

int Queue::front(void)
{
	if(this->d < 2)
		return -1;

	return this->data[this->d - 2];
}
