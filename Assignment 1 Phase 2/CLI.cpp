#include "bus.hpp"

using namespace std;

int main() {
    BusSystem bs;

    while (1) {
        string cmd;
        getline(cin, cmd);
        string res = bs.query(cmd);

        cout << res << '\n';
    }     
}
