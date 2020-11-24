#include <assert.h>
using namespace std;

#include "bus.hpp"

int main() {
    BusSystem* bs = new BusSystem();
    // Normal
    assert(bs->query("SQ 12") == "1");
    assert(bs->query("INS A 0-A 001 900") == "1");
    assert(bs->query("INS A 1-A 101 1000") == "2");
    assert(bs->query("INS A 2-A 201 1100") == "3");
    assert(bs->query("INS A 3-A 301 1200") == "4");
    assert(bs->query("INS A 4-A 401 1300") == "5");
    assert(bs->query("INS A 5-A 1 501 1400") == "6");
    assert(bs->query("INS A 6-A 1 601 1500") == "7");
    assert(bs->query("INS A 7-A 1 701 1600") == "8");
    assert(bs->query("INS A 8-A 1 801 1700") == "9");
    assert(bs->query("INS A 9-A 1 901 1800") == "10");
    assert(bs->query("INS A 10-A 1 1001 1900") == "11");

    assert(bs->query("CS A 801") == "9");
    assert(bs->query("CS A 801 1") == "4");
    assert(bs->query("CS A 801 0") == "5");

    assert(bs->query("CS A 1000") == "9");
    assert(bs->query("CS A 1000 0") == "4");
    assert(bs->query("CS A 1000 1") == "5");
    //  spam the command with CODE that match no routes.
        for (int i = 0; i < 100000; ++i) {
        string cmd = string("CS ") + to_string(i) + string(" 0");
        assert(bs->query(cmd) == "0");
    }

    delete bs;
}