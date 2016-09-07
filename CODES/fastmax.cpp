/* 2010
 * Maciej Szeptuch
 * XIV LO Wrocław
 */
#include<cstdio>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

class iTree
{
	private:
		int data[1 << 19];
		int SIZE;

	public:
		iTree(int siz = 1 << 18);
		inline void setValue(int id, int value);
		inline int getMax(int start, int end, int iStart = 0, int iEnd = -1,
				int pos = 1) const;

};

int size,
	queries,
	query,
	from,
	to;
iTree tree;

inline static int MAX(const int &a, const int &b);

int main(void)
{
	scanf("%d %d", &size, &queries);
	tree = iTree(size);
	for(int q = 0; q < queries; ++ q)
	{
		scanf("%d %d %d", &query, &from, &to);
		if(query)
			tree.setValue(from, to);

		else
			printf("%d\n", tree.getMax(from, to));

	}

	return 0;
}

iTree::iTree(int siz)
{
	this->SIZE = 1;
	while(this->SIZE < siz)
		this->SIZE *= 2;

	for(int s = 0; s < 2 * this->SIZE; ++ s)
		this->data[s] = 0;

	return;
}

inline void iTree::setValue(int id, int value)
{
	id += this->SIZE;
	this->data[id] = value;
	id /= 2;
	while(id)
	{
		this->data[id] = MAX(this->data[id * 2], this->data[id * 2 + 1]);
		id /= 2;
	}

	return;
}

inline int iTree::getMax(int start, int end,
		int iStart, int iEnd,
		int pos) const
{
	if(iEnd == -1)
		iEnd = this->SIZE - 1;

	if(start == iStart && end == iEnd)
		return this->data[pos];

	int iMiddle = (iStart + iEnd) / 2;
	if(end <= iMiddle)
		return this->getMax(start, end, iStart, iMiddle, pos * 2);

	if(start > iMiddle)
		return this->getMax(start, end, iMiddle + 1, iEnd, pos * 2 + 1);

	return MAX(this->getMax(start, iMiddle, iStart, iMiddle, pos * 2),
			this->getMax(iMiddle + 1, end, iMiddle + 1, iEnd, pos * 2 + 1));
}

inline static int MAX(const int &a, const int &b)
{
	return a>b?a:b;
}
