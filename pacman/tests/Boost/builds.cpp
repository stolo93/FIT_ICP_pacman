#include "boost/lockfree/queue.hpp"


int main()
{
    auto queue = boost::lockfree::queue<int>(1);
    int return_value = 1;

    if (! queue.push(0)) { return 1; }

    queue.pop(return_value);

    return return_value;
}