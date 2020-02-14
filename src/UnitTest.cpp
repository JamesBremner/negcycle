
#include <iostream>
#include <chrono>

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/breadth_first_search.hpp>

#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/random/linear_congruential.hpp>

using namespace std;
using namespace boost;

#include "negcyc.h"

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
typedef erdos_renyi_iterator<minstd_rand, graph_t> ERGen;

void add_edge( int a, int b, double w, graph_t& g )
{
    g[add_edge( a, b, g ).first].myWeight = w;
}

int
main(int, char *[])
{
    vector< vector<int> > vnegcycs;
    cNegCyc<graph_t> NC;

    // construct graph with 1 negative cycle
    graph_t g1(6);
    add_edge( 0, 1, -1, g1 );
    add_edge( 1, 2, -2, g1 );
    add_edge( 2, 0, -3, g1 );
    add_edge( 2, 3,  1, g1 );
    add_edge( 3, 4,  1, g1 );
    add_edge( 4, 5,  1, g1 );
    add_edge( 5, 3,  1, g1 );

    vnegcycs = NC.find( g1 );

    for( auto& c : vnegcycs )
    {
        for( int v : c )
            cout << v << " ";
        cout << "\n";
    }

    if( (int)vnegcycs.size() != 1 ) {
        cout << "FAILED!!!\n";
        exit(1);
    }

    // construct graph with 2 negative cycles
    graph_t g2(6);
    add_edge( 0, 1, -1, g2 );
    add_edge( 1, 2, -2, g2 );
    add_edge( 2, 0, -3, g2 );
    add_edge( 2, 3,  1, g2 );
    add_edge( 3, 4, -1, g2 );
    add_edge( 4, 5, -1, g2 );
    add_edge( 5, 3, -1, g2 );

    vnegcycs = NC.find( g2 );

    for( auto& c : vnegcycs )
    {
        for( int v : c )
            cout << v << " ";
        cout << "\n";
    }

    if( (int)vnegcycs.size() != 2 ) {
        cout << "FAILED!!!\n";
        exit(1);
    }

        // construct graph with 1 negative cycles
    graph_t g3(6);
    add_edge( 0, 1, -1, g3 );
    add_edge( 1, 2, -2, g3 );
    add_edge( 2, 0, -3, g3 );
    add_edge( 2, 3,  1, g3 );
    add_edge( 3, 4, -1, g3 );
    add_edge( 4, 5, -1, g3 );
    add_edge( 5, 3, 10, g3 );   // back edge makes this a +ve cycle

    vnegcycs = NC.find( g3   );

    for( auto& c : vnegcycs )
    {
        for( int v : c )
            cout << v << " ";
        cout << "\n";
    }

    if( (int)vnegcycs.size() != 1 ) {
        cout << "FAILED!!!\n";
        exit(1);
    }

            // construct graph with 1 negative cycles
    graph_t g4(6);
    add_edge( 0, 1, -1, g4 );
    add_edge( 1, 2, -2, g4 );
    add_edge( 2, 0, -3, g4 );
    add_edge( 2, 3,  1, g4 );
    add_edge( 3, 4, 10, g4 );
    add_edge( 4, 5, -1, g4 );
    add_edge( 5, 3, -1, g4 );

    vnegcycs = NC.find( g3   );

    for( auto& c : vnegcycs )
    {
        for( int v : c )
            cout << v << " ";
        cout << "\n";
    }

    if( (int)vnegcycs.size() != 1 ) {
        cout << "FAILED!!!\n";
        exit(1);
    }

    cout << "Unit tests passed\n";

}

