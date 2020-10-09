#include "unique_ptr.hpp"

using namespace std;

int main(int argc, char const* argv[]) {
    stl::unique_ptr<int[]> uptr(new int[4]);

    return 0;
}
