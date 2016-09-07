/***************************************** LCA ************************************/
static inline void dfs(int v, int f)
{
        father[v][0] = f;
        path[v] = path[f] + MAX(height[f] - height[v], 0);
        revpath[v] = revpath[f] + MAX(height[v] - height[f], 0);
        for(int l = 1; l < 20; ++ l)
           father[v][l] = father[father[v][l - 1]][l - 1];

        start[v] = Time ++;
        for(unsigned int n = 0; n < graph[v].size(); ++ n)
                if(graph[v][n] != f)
                        dfs(graph[v][n], v);

        end[v] = Time ++;

}

static inline int findLCA(int a, int b)
{
        if(inInterval(a, b))
                return b;

        if(inInterval(b, a))
                return a;

        int l = 0;
        while(!inInterval(a, b))
        {
                l = 0;
                while(!inInterval(a, father[b][++ l]));
                b = father[b][l - 1];
        }

        return b;
}

static inline bool inInterval(int v, int i)
{
        return start[i] <= start[v] && start[v] <= end[i];
}
