/* AHO CORASIC */
struct TrieNode
{
	TrieNode *child[27];
	TrieNode *fail;
	int match;
};

inline static const int $(const char sign){return sign - 'a';}
inline static void insertWord(TrieNode *node, char *word, const int &match);
inline static void updateTrie(TrieNode *node);

int patterns,
	length[1001];
TrieNode root,
		 *pos = &root;
char pattern[1001],
	 text[1000001];
std::vector<std::pair<int, int> > result;

int main(void)
{
	scanf("%d", &patterns);
	for(int p = 0; p < patterns; ++ p)
	{
		scanf("%s", pattern);
		insertWord(&root, pattern, p + 1);
		while(pattern[++ length[p]]);
	}

	updateTrie(&root);
	scanf("%s", text);
	for(int t = 0; text[t]; ++ t)
	{
		while(pos->fail && !pos->child[$(text[t])])
			pos = pos->fail;

		if(pos->child[$(text[t])])
			pos = pos->child[$(text[t])];

		for(TrieNode *bak = pos; bak->fail; bak = bak->fail)
			if(bak->match)
				result.push_back(std::make_pair(2 + t - length[bak->match - 1], bak->match));
	}

	std::sort(result.begin(), result.end());
	for(int r = 0; r < result.size(); ++ r)
		printf("%d %d\n", result[r].second, result[r].first);

	return 0;
}

inline static void insertWord(TrieNode *node, char *word, const int &match)
{
	TrieNode *act = node;
	for(int w = 0; word[w]; ++ w)
	{
		if(!act->child[$(word[w])])
			act->child[$(word[w])] = new TrieNode();

		act = act->child[$(word[w])];
		act->fail = node;
	}

	act->match = match;
}

inline static void updateTrie(TrieNode *node)
{
	TrieNode *fail;
	std::queue<TrieNode *> que;
	que.push(node);

	while(!que.empty())
	{
		node = que.front();
		que.pop();

		for(int w = 0; w < 'z' - 'a' + 1; ++ w)
		{
			if(!node->child[w])
				continue;

			que.push(node->child[w]);
			fail = node->fail;
			while(fail && !fail->child[w])
				fail = fail->fail;

			if(fail)
				fail = fail->child[w];

			else
				fail = node;

			node->child[w]->fail = fail;
		}
	}

	return;
}

