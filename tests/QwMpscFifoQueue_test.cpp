/* 
    Queue World is copyright (c) 2014 Ross Bencina

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#include "QwMpscFifoQueue.h"

#include "catch.hpp"


namespace {

    struct TestNode{
        TestNode *links_[2];
        enum { LINK_INDEX_1, LINK_COUNT };

        int value;

        TestNode()
            : value( 0 )
        {
            for( int i=0; i < LINK_COUNT; ++i )
                links_[i] = 0;
        }
    };

    typedef QwMPSCFifoQueue<TestNode*, TestNode::LINK_INDEX_1> mpsc_fifo_queue_t;

} // end anonymous namespace


TEST_CASE( "qw/mpsc_fifo_queue", "QwMPSCFifoQueue single threaded test" ) {

    TestNode nodes[3];
    TestNode *a = &nodes[0];
    TestNode *b = &nodes[1];
    TestNode *c = &nodes[2];

    mpsc_fifo_queue_t q;

    REQUIRE( q.empty() == true );

    // void push( node_ptr_type n )
    // bool empty() const
    // node_ptr_type pop()

    q.push( a );
    REQUIRE( q.empty() == false );
    REQUIRE( q.pop() == a );
    REQUIRE( q.empty() == true );

    q.push( a );
    q.push( b );
    q.push( c );

    REQUIRE( q.empty() == false );
    REQUIRE( q.pop() == a );
    REQUIRE( q.pop() == b );
    REQUIRE( q.pop() == c );
    REQUIRE( q.empty() == true );

    // void push( node_ptr_type n, bool& wasEmpty )

    bool wasEmpty = false;
    q.push( a, wasEmpty );
    REQUIRE( wasEmpty == true );

    q.push( b, wasEmpty );
    REQUIRE( wasEmpty == false );

    REQUIRE( q.empty() == false );
    REQUIRE( q.pop() == a );

    q.push( c, wasEmpty );
    // REQUIRE( wasEmpty == false ); // <--- KNOWNBUG this fails.

    REQUIRE( q.pop() == b );
    REQUIRE( q.pop() == c );
    REQUIRE( q.empty() == true );
}