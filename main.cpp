
#include <iostream>

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/breadth_first_search.hpp>

//#include <boost/random/linear_congruential.hpp>

using namespace std;
using namespace boost;

class cMyVertex
{
};
class cMyEdge
{
public:
    double myWeight;
};

typedef adjacency_list<vecS, vecS, directedS,
        cMyVertex, cMyEdge> graph_t;

void add_edge( int a, int b, double w, graph_t& g )
{
    g[add_edge( a, b, g ).first].myWeight = w;
}

void AddNew(  vector< vector<int> >& negcycs, vector<int>& path )
{
    for( auto& c : negcycs )
    {
        bool same = true;
        for( auto v : path )
        {
            if ( find( c.begin(), c.end(), v ) == c.end() )
            {
                same = false;
                break;
            }
        }
        if( same )
            return;
    }

    path.push_back( path[0] );
    negcycs.push_back( path );
}

vector< vector<int> > negcycs( graph_t& g )
{
    vector< vector<int> > vnegcycs;
    for( int start = 0; start < (int)num_vertices(g); start++ )
    {
        // check that a cycle can start and finish on this vertex
        if( ! out_degree( start, g) )
            continue;
        int in_degree = 0;
        auto es = edges(g);
        for (auto eit = es.first; eit != es.second; ++eit)
            if( (int)boost::target(*eit, g) == start )
                in_degree++;
        if( ! in_degree )
            continue;

        //cout << "start " << start << "\n";

        std::vector<int> pred( num_vertices(g),-1);
        pred[start] = start;
        breadth_first_search(
            g,
            start,
            boost::visitor(
                boost::make_bfs_visitor(
                    boost::record_predecessors(
                        pred.data(),
                        boost::on_tree_edge()))));

        for (auto eit = es.first; eit != es.second; ++eit)
        {
            // check for back edge
            if( (int)boost::target(*eit, g) != start )
                continue;
            if( pred[source(*eit, g)] == -1)
                continue;

            // total edge weights in cycle
            double pw = 0;
            vector<int> path;
            int v = source(*eit, g);
            path.push_back( v );
            while( 1 )
            {
                pw += g[edge(pred[v],v,g).first].myWeight;
                v = pred[v];
                path.push_back( v );
                if( v == start )
                {
                    pw += g[edge(source(*eit, g),v,g).first].myWeight;
                    break;
                }
            }

            // check that sum of path weights is negative
            if( pw >= 0 )
                continue;

            // save if unique cycle
            AddNew( vnegcycs, path );

        }
    }
    return vnegcycs;
}

int
main(int, char *[])
{
    vector< vector<int> > vnegcycs;

    // construct graph with 1 negative cycle
    graph_t g1(6);
    add_edge( 0, 1, -1, g1 );
    add_edge( 1, 2, -2, g1 );
    add_edge( 2, 0, -3, g1 );
    add_edge( 2, 3,  1, g1 );
    add_edge( 3, 4,  1, g1 );
    add_edge( 4, 5,  1, g1 );
    add_edge( 5, 3,  1, g1 );

    vnegcycs = negcycs( g1 );

    for( auto& c : vnegcycs )
    {
        for( int v : c )
            cout << v << " ";
        cout << "\n";
    }

    if( (int)vnegcycs.size() != 1 )
        cout << "FAILED!!!\n";

    // construct graph with 2 negative cycles
    graph_t g2(6);
    add_edge( 0, 1, -1, g2 );
    add_edge( 1, 2, -2, g2 );
    add_edge( 2, 0, -3, g2 );
    add_edge( 2, 3,  1, g2 );
    add_edge( 3, 4, -1, g2 );
    add_edge( 4, 5, -1, g2 );
    add_edge( 5, 3, -1, g2 );

    vnegcycs = negcycs( g2 );

    for( auto& c : vnegcycs )
    {
        for( int v : c )
            cout << v << " ";
        cout << "\n";
    }

    if( (int)vnegcycs.size() != 2 )
        cout << "FAILED!!!\n";
}

