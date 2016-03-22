/* 2009
Maciej Szeptuch
XIV LO Wrocław
*/

#include <cstdio>
#include <vector>
#include <cstdlib>
#include <list>

using namespace std;

int tests,
	rooms,
	corrs;

bool flag,
	 visited [ 100010 ];

pair < int, bool > graph [ 100010 ] [ 4 ];
pair < int, int > act;

list < pair < int, int > > bad;

void dfs ( int last, int vert );

int main ( void )
{
	scanf ( "%d", & tests );

	for ( int t = 0; t < tests; ++ t )
	{
		for ( int i = 0; i < 100010; ++ i )
		{
			graph [ i ] [ 0 ] = graph [ i ] [ 1 ] = graph [ i ] [ 2 ] = graph [ i ] [ 3 ] = make_pair ( 0, false );
			visited [ i ] = false;
		}

		scanf ( "%d", & rooms );

		for ( int r = 0; r < rooms; ++ r )
		{
			scanf ( "%d", & corrs );
			graph [ r ] [ 3 ] . first = corrs;

			for ( int c = 0; c < corrs; ++ c )
			{
				scanf ( "%d",  & graph [ r ] [ c ] . first );
				-- graph [ r ] [ c ] . first;
			}
		}

		dfs ( 0, 0 );
		printf ( "%d\n", rooms );
		for ( int  r = 0; r < rooms; ++ r )
		{
			printf ( "%d ", graph [ r ] [ 3 ] . first );
			for ( int c = 0; c < graph [ r ] [ 3 ] . first; ++ c )
				printf ( "%d ", graph [ r ] [ c ] . first + 1 );

			printf ( "\n" );
		}
	}
	return 0;
}

void dfs ( int last, int vert )
{
	int father = -1;

	for ( int c = 0; c < graph [ vert ] [ 3 ] . first; ++ c )
		if ( graph [ vert ] [ c ] . first == last )
			father = c;

	if ( father != -1 && ! visited [ vert ] )
	{
		swap ( graph [ vert ] [ father ], graph [ vert ] [ graph [ vert ] [ 3 ] . first - 1 ] );
		father = graph [ vert ] [ 3 ] . first - 1;
	}

	visited [ vert ] = true;

	if ( graph [ vert ] [ ( father + 1 ) % graph [ vert ] [ 3 ] . first ] . second )
		return;

	graph [ vert ] [ ( father + 1 ) % graph [ vert ] [ 3 ] . first ] . second = true;

	dfs ( vert, graph [ vert ] [ ( father + 1 ) % graph [ vert ] [ 3 ] . first ] . first );

	father = -1;

	for ( int c = 0; c < graph [ vert ] [ 3 ] . first; ++ c )
		if ( ! visited [ graph [ vert ] [ c ] . first ]  )
			father = c;

	if ( father == -1 )
		return;

	if ( father == 0 )
		swap ( graph [ vert ] [ 1 % graph [ vert ] [ 3 ] . first ], graph [ vert ] [ 2 % graph [ vert ] [ 3 ] . first ] );

	else
		swap ( graph [ vert ] [ 0 ], graph [ vert ] [ father ] );

	graph [ vert ] [ 0 ] . second = true;
	dfs ( vert, graph [ vert ] [ 0 ] . first );
}

