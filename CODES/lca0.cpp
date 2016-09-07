/* 2011
 * Maciej Szeptuch
 * II UWr
 * ----------
 *  LCA z RMQ.
 */
#include<cstdio>
#include<vector>
//#define DEBUG(args...) fprintf(stderr, args)
#define DEBUG(args...)

const int MAX_VERTS = 600000;

struct iTree
{
	static const int MAX_SIZE = 1048576 * 2;
	int data[2 * MAX_SIZE];
	unsigned int size;

	inline const unsigned int insert(const int &value); // UWAGA: statyczne drzewo, bez update po każdym dodaniu!
	inline void propagate(void);
	inline const int getMin(const int &start, const int &end, const int &iStart = 0, const int &iEnd = MAX_SIZE - 1, const int &pos = 1);
};

int verts,
	trip,
	first, second,
	act, next, lca,
	depth[MAX_VERTS + 1],
	repr[MAX_VERTS + 1],
	order[MAX_VERTS + 1], ord,
	orderToVert[2 * MAX_VERTS + 1];
long long int result;

std::vector<int> graph[MAX_VERTS + 1];

iTree tree;

inline static int MIN(const int &a, const int &b){return a<b?a:b;}
inline static void preprocess(const int &v, const int &f = -1, const int &d = 0);
inline static int findLCA(const int &a, const int &b);

int main(void)
{
	scanf("%d", &verts);
	for(int e = 1; e < verts; ++ e)
	{
		scanf("%d %d", &first, &second);
		-- second;
		-- first;
		graph[first].push_back(second);
		graph[second].push_back(first);
	}

	preprocess(0);
	tree.propagate();
	scanf("%d", &trip);
	for(int t = 0; t < trip; ++ t)
	{
		scanf("%d", &next);
		-- next;
		lca = findLCA(act, next);
		result += depth[act] + depth[next] - 2 * depth[lca];
		act = next;
	}

	printf("%lld\n", result);
	return 0;
}

inline static void preprocess(const int &v, const int &f, const int &d)
{
	depth[v] = d;
	repr[v] = tree.insert(order[v] = ord ++);
	orderToVert[ord - 1] = v;
	for(unsigned int n = 0; n < graph[v].size(); ++ n)
		if(graph[v][n] != f)
		{
			preprocess(graph[v][n], v, d + 1);
			tree.insert(order[v]);
		}
}

inline static int findLCA(const int &a, const int &b)
{
	if(repr[a] > repr[b])
		return findLCA(b, a);

	return orderToVert[tree.getMin(repr[a], repr[b])];
}

inline const unsigned int iTree::insert(const int &value)
{
	this->data[MAX_SIZE + this->size] = value;
	return this->size ++;
}

inline void iTree::propagate(void)
{
	for(int v = MAX_SIZE - 1; v; -- v)
		this->data[v] = MIN(this->data[2 * v], this->data[2 * v + 1]);
}

inline const int iTree::getMin(const int &start, const int &end, const int &iStart, const int &iEnd, const int &pos)
{
	if(start == iStart && end == iEnd)
		return this->data[pos];

	const int iMiddle = (iStart + iEnd) / 2;
	if(end <= iMiddle)
		return this->getMin(start, end, iStart, iMiddle, pos * 2);

	if(start > iMiddle)
		return this->getMin(start, end, iMiddle + 1, iEnd, pos * 2 + 1);

	return MIN(
		this->getMin(start, iMiddle, iStart, iMiddle, pos * 2),
		this->getMin(iMiddle + 1, end, iMiddle + 1, iEnd, pos * 2 + 1));
}
