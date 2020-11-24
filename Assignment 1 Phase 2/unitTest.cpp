/*
*   Nguyen Thanh Toan
*   Unit Tests for Assignment 1 Phase 2
*/
#include <assert.h>
using namespace std;

#include "bus.hpp"
// Include your class here

/*
*   Test guarantees:
*       1 bus only go on 1 route.
*       All time argument is an non negative integer with at most 9 digits which means it
*       is in range [0..1e9).
*   
*   Syntax error:
*       Invalid command keyword.
*       All form of invalid argument(s) for each command:
*           Have less or more argument(s) than expected.
*           CODE has size > 5.
*           LP has size > 10.
*           TIME, TIMEA, TIMEB, N is not an integer.
*           TIMEA > TIMEB 
*           CASE is not a binary value.
*           Whitespace(s) at the beginning or the end.
*           2 or more whitespaces in between arguments.
*
*   
*   Duration of a trip: [TIMEA..TIMEB].
*/


void exampleTest() {
    BusSystem* bs = new BusSystem();
    assert(bs->query("SQ 500") == "1");
    assert(bs->query("INS 50 50D1-23342 1234 5678") == "1");
    assert(bs->query("CS 50 2134") == "1");
    delete bs;
}

void invalidCommandTest() {
    BusSystem* bs = new BusSystem();
    assert(bs->query("INVLD_CMD 10000 10000") == "-1");
    delete bs;
}

void SQSyntaxTest() {
    BusSystem* bs = new BusSystem();
    //  0 argument
    assert(bs->query("SQ") == "-1");
    //  2 arguments
    assert(bs->query("SQ 123 10000") == "-1");
    //  3 arguments
    assert(bs->query("SQ 123 10000 10000") == "-1");
    //  N is not an integer
    assert(bs->query("SQ A123") == "-1");
    assert(bs->query("SQ 12A3") == "-1");
    assert(bs->query("SQ 123A") == "-1");
    //  Whitespaces at the begining
    assert(bs->query("   SQ 12") == "-1");
    //  Whitespaces at the end
    assert(bs->query("SQ 12   ") == "-1");
    //  2 or more consecutive whitespaces
    assert(bs->query("SQ    12") == "-1");
    delete bs;
}

void INSSyntaxTest() {
    BusSystem* bs = new BusSystem();
    //  3 arguments
    assert(bs->query("INS A 0-A 1234") == "-1");
    //  6 arguments
    assert(bs->query("INS A 0-A 1 1234 5678 10000") == "-1");
    //  CODE has size > 5
    assert(bs->query("INS ABCDEF 0-A 1 1234 5678") == "-1");
    //  LP has size > 10
    assert(bs->query("INS A 01234-ABCDE 1 1234 5678") == "-1");
    //  TIMEA is not an integer
    assert(bs->query("INS A 0-A 1 A1234 5678") == "-1");
    assert(bs->query("INS A 0-A 1 1A234 5678") == "-1");
    assert(bs->query("INS A 0-A 1 1234A 5678") == "-1");
    //  TIMEB is not an integer
    assert(bs->query("INS A 0-A 1 1234 A5678") == "-1");
    assert(bs->query("INS A 0-A 1 1234 56A78") == "-1");
    assert(bs->query("INS A 0-A 1 1234 5678A") == "-1");
    //  TIMEA > TIMEB
    assert(bs->query("INS A 0-A 1 5678 1234") == "-1");
    //  TIMEA = TIMEB
    assert(bs->query("INS A 0-A 1 1234 1234") == "-1");
    //  CASE is not a binary value
    assert(bs->query("INS A 0-A 1 1234 5678 1C2") == "-1");
    assert(bs->query("INS A 0-A 1 1234 5678 2") == "-1");
    //  Whitespaces at the begining
    assert(bs->query("   INS A 0-A 1 1234 5678") == "-1");
    //  Whitespaces at the end
    assert(bs->query("INS A 0-A 1 1234 5678   ") == "-1");
    //  2 or more consecutive whitespaces
    assert(bs->query("INS A 0-A    1 1234 5678") == "-1");
    delete bs;
}

void DELSyntaxTest() {
    BusSystem* bs = new BusSystem();
    //  0 arguments
    assert(bs->query("DEL") == "-1");
    //  3 arguments
    assert(bs->query("DEL A 1234 5678 10000") == "-1");
    //  CODE has size > 5
    assert(bs->query("DEL ABCDEF 1234") == "-1");
    //  TIMEA is not an integer
    assert(bs->query("DEL A A1234") == "-1");
    assert(bs->query("DEL A 12A34") == "-1");
    assert(bs->query("DEL A 1234A") == "-1");
    //  TIMEB is not an integer
    assert(bs->query("DEL A 1234 A5678") == "-1");
    assert(bs->query("DEL A 1234 56A78") == "-1");
    assert(bs->query("DEL A 1234 5678A") == "-1");
    //  TIMEA > TIMEB
    assert(bs->query("DEL A 5678 1234 ") == "-1");
    //  Whitespaces at the begining
    assert(bs->query("   DEL A 1234 5678") == "-1");
    //  Whitespaces at the end
    assert(bs->query("DEL A 1234 5678   ") == "-1");
    //  2 or more consecutive whitespaces
    assert(bs->query("DEL A    1234 5678") == "-1");
    delete bs;
}

void CSSyntaxTest() {
    BusSystem* bs = new BusSystem();
    //  1 arguments
    assert(bs->query("CS A") == "-1");
    //  4 arguments
    assert(bs->query("CS A 1234 0 10000") == "-1");
    //  CODE has size > 5
    assert(bs->query("CS ABCDEF 1234") == "-1");
    //  TIME is not an integer
    assert(bs->query("CS A A1234") == "-1");
    assert(bs->query("CS A 12A34") == "-1");
    assert(bs->query("CS A 1234A") == "-1");
    //  CASE is not binary value
    assert(bs->query("CS A 1234 1C2") == "-1");
    assert(bs->query("CS A 1234 2") == "-1");
    //  Whitespaces at the begining
    assert(bs->query("   CS A 1234 0") == "-1");
    //  Whitespaces at the end
    assert(bs->query("CS A 1234 0   ") == "-1");
    //  2 or more consecutive whitespaces
    assert(bs->query("CS A    1234 0") == "-1");
    delete bs;
}

void CESyntaxTest() {
    BusSystem* bs = new BusSystem();
    //  1 arguments
    assert(bs->query("CE A") == "-1");
    //  4 arguments
    assert(bs->query("CE A 1234 0 10000") == "-1");
    //  CODE has size > 5
    assert(bs->query("CE ABCDEF 1234") == "-1");
    //  TIME is not an integer
    assert(bs->query("CE A A1234") == "-1");
    assert(bs->query("CE A 12A34") == "-1");
    assert(bs->query("CE A 1234A") == "-1");
    //  CASE is not a binary value
    assert(bs->query("CE A 1234 1C2") == "-1");
    assert(bs->query("CE A 1234 2") == "-1");
    //  Whitespaces at the begining
    assert(bs->query("   CE A 1234") == "-1");
    //  Whitespaces at the end
    assert(bs->query("CE A 1234   ") == "-1");
    //  2 or more consecutive whitespaces
    assert(bs->query("CE A    1234") == "-1");
    delete bs;
}

void GSSyntaxTest() {
    BusSystem* bs = new BusSystem();
    //  1 arguments
    assert(bs->query("GS A") == "-1");
    //  4 arguments
    assert(bs->query("GS A 1234 0 10000") == "-1");
    //  CODE has size > 5
    assert(bs->query("GS ABCDEF 1234") == "-1");
    //  TIME is not an integer
    assert(bs->query("GS A A1234") == "-1");
    assert(bs->query("GS A 12A34") == "-1");
    assert(bs->query("GS A 1234A") == "-1");
    //  CASE is not a binary value
    assert(bs->query("GS A 1234 1C2") == "-1");
    assert(bs->query("GS A 1234 2") == "-1");
    //  Whitespaces at the begining
    assert(bs->query("   GS A 1234") == "-1");
    //  Whitespaces at the end
    assert(bs->query("GS A 1234   ") == "-1");
    //  2 or more consecutive whitespaces
    assert(bs->query("GS A    1234") == "-1");
    delete bs;
}

void GESyntaxTest() {
    BusSystem* bs = new BusSystem();
    //  1 arguments
    assert(bs->query("GE A") == "-1");
    //  4 arguments
    assert(bs->query("GE A 1234 0 10000") == "-1");
    //  CODE has size > 5
    assert(bs->query("GE ABCDEF 1234") == "-1");
    //  TIME is not an integer
    assert(bs->query("GE A A1234") == "-1");
    assert(bs->query("GE A 12A34") == "-1");
    assert(bs->query("GE A 1234A") == "-1");
    //  CASE is not a binary value
    assert(bs->query("GE A 1234 1C2") == "-1");
    assert(bs->query("GE A 1234 2") == "-1");
    //  Whitespaces at the begining
    assert(bs->query("   GE A 123") == "-1");
    //  Whitespaces at the end
    assert(bs->query("GE A 1234   ") == "-1");
    //  2 or more consecutive whitespaces
    assert(bs->query("GE A    1234") == "-1");
    delete bs;
}

void INSLogicTest() {
    BusSystem* bs = new BusSystem();

    assert(bs->query("SQ 6") == "1");
    // Normal 
    assert(bs->query("INS A 0-A 100 200") == "1");
    assert(bs->query("INS A 0-A 1 201 300") == "2");
    assert(bs->query("INS A 0-A 0 301 400") == "3");
    // Different LP, same TIMEA with bus already in schedule
    assert(bs->query("INS A 1-A 301 700") == "-1");
    assert(bs->query("INS A 1-A 1 301 700") == "4");
    // Same LP, Same TIMEA
    assert(bs->query("INS A 0-A 0 401 500") == "5");
    assert(bs->query("INS A 0-A 1 401 500") == "-1");
    // Same LP, overlap time with previous trip in schedule
    assert(bs->query("INS A 1-A 0 451 550") == "-1");
    assert(bs->query("INS A 1-A 1 451 550") == "-1");
    
    assert(bs->query("INS A 1-A 0 200 550") == "-1");
    assert(bs->query("INS A 1-A 1 200 550") == "-1");

    assert(bs->query("INS A 1-A 0 600 900") == "-1");
    assert(bs->query("INS A 1-A 1 600 900") == "-1");
    // Same LP, not overlapping but at the latter in TIMEA
    assert(bs->query("INS A 1-A 1 100 200") == "-1");
    // More than N
    assert(bs->query("INS A 1-A 1 1000 2000") == "6");
    assert(bs->query("INS A 1-A 1 3000 4000") == "-1");
    // Different CODE
    assert(bs->query("INS B 0-B 1 101 500") == "1");
    assert(bs->query("INS B 0-B 1 501 600") == "2");

    delete bs;
}

void DELLogicTest() {
    BusSystem* bs = new BusSystem();
    // Normal
    // Also check for number of trips after DEL by using INS
    assert(bs->query("SQ 6") == "1");
    assert(bs->query("INS A 0-A 1 100") == "1");
    assert(bs->query("INS A 0-A 101 200") == "2");
    assert(bs->query("INS A 0-A 201 300") == "3");
    assert(bs->query("INS A 0-A 301 400") == "4");
    assert(bs->query("INS A 0-A 401 500") == "5");
    assert(bs->query("INS A 0-A 501 600") == "6");
    assert(bs->query("INS B 0-B 1 100") == "1");

    assert(bs->query("DEL A 700 800") == "0");
    assert(bs->query("DEL A 700") == "0");

    assert(bs->query("DEL A 400 600") == "2");
    assert(bs->query("INS A 0-A 601 700") == "5");

    assert(bs->query("DEL A 300") == "0");
    assert(bs->query("DEL A 301") == "1");
    assert(bs->query("INS A 0-A 701 800") == "5");

    assert(bs->query("DEL A") == "5");
    assert(bs->query("INS A 0-A 1 100") == "1");
    assert(bs->query("INS A 0-A 801 900") == "2");

    assert(bs->query("INS B 0-B 501 600") == "2");
    // DEL on different CODE
    assert(bs->query("DEL B") == "2");
    //  spam the command with CODE that match no routes.
    for (int i = 0; i < 100000; ++i) {
        string cmd = string("DEL ") + to_string(i);
        assert(bs->query(cmd) == "0");
    }

    delete bs;
}

void CSLogicTest() {
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
    assert(bs->query("INS A 0-A 1 901 1800") == "10");
    assert(bs->query("INS A 1-A 1 1001 1900") == "11");

    assert(bs->query("CS A 801") == "9");
    assert(bs->query("CS A 801 0") == "5");
    assert(bs->query("CS A 801 1") == "4");

    assert(bs->query("CS A 1000") == "9");
    assert(bs->query("CS A 1000 0") == "4");
    assert(bs->query("CS A 1000 1") == "5");
    // Trivial Test
    assert(bs->query("CS A 2000") == "0");
    assert(bs->query("CS A 0") == "0");
    assert(bs->query("CS A 1") == "1");
    assert(bs->query("CS A 1 1") == "0");
    // With DEL
    assert(bs->query("DEL A 401") == "1");
    assert(bs->query("CS A 801") == "8");
    assert(bs->query("CS A 301") == "4");
    assert(bs->query("CS A 801 0") == "4");
    assert(bs->query("CS A 801 1") == "4");
    //  spam the command with CODE that match no routes.
    for (int i = 0; i < 100000; ++i) {
        string cmd = string("CS ") + to_string(i) + string(" 0");
        assert(bs->query(cmd) == "0");
    }

    delete bs;
}

void CELogicTest() {
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
    assert(bs->query("INS A 0-A 1 901 1800") == "10");
    assert(bs->query("INS A 1-A 1 1001 1900") == "11");

    assert(bs->query("CE A 1701") == "9");
    assert(bs->query("CE A 1701 0") == "5");
    assert(bs->query("CE A 1701 1") == "4");

    assert(bs->query("CE A 1600") == "7");
    assert(bs->query("CE A 1600 0") == "5");
    assert(bs->query("CE A 1600 1") == "2");
    // Trivial Test
    assert(bs->query("CE A 2000") == "11");
    assert(bs->query("CE A 0") == "0");
    assert(bs->query("CE A 900") == "0");
    assert(bs->query("CE A 901") == "1");
    assert(bs->query("CE A 901 1") == "0");
    // With DEL
    assert(bs->query("DEL A 401") == "1");
    assert(bs->query("CE A 1701") == "8");
    assert(bs->query("CE A 1201") == "4");
    assert(bs->query("CE A 1701 0") == "4");
    assert(bs->query("CE A 1701 1") == "4");
    //  spam the command with CODE that match no routes.
    for (int i = 0; i < 100000; ++i) {
        string cmd = string("CE ") + to_string(i) + string(" 0");
        assert(bs->query(cmd) == "0");
    }

    delete bs;
}

void GSLogicTest() {
    BusSystem* bs = new BusSystem();
    // Normal
    assert(bs->query("SQ 12") == "1");
    assert(bs->query("INS A 0-A 001 100") == "1");
    assert(bs->query("INS A 1-A 101 200") == "2");
    assert(bs->query("INS A 2-A 201 300") == "3");
    assert(bs->query("INS A 3-A 401 500") == "4");
    assert(bs->query("INS A 4-A 601 700") == "5");
    assert(bs->query("INS A 5-A 51 400") == "6");
    assert(bs->query("INS A 6-A 451 650") == "7");
    assert(bs->query("INS A 7-A 1 451 650") == "8");
    assert(bs->query("INS A 8-A 1 651 750") == "9");

    assert(bs->query("GS A 100") == "5-A");
    assert(bs->query("GS A 101") == "1-A");
    assert(bs->query("GS A 200") == "1-A");
    assert(bs->query("GS A 250") == "2-A");
    assert(bs->query("GS A 350") == "2-A");
    assert(bs->query("GS A 425") == "3-A");
    assert(bs->query("GS A 450") == "3-A");
    assert(bs->query("GS A 675") == "8-A");
    // CASE
    assert(bs->query("GS A 455") == "6-A");
    assert(bs->query("GS A 455 0") == "6-A");
    assert(bs->query("GS A 455 1") == "7-A");
    // Trivial Test
    assert(bs->query("GS A 0") == "-1");
    assert(bs->query("GS A 1000") == "8-A");
    // With DEL
    assert(bs->query("DEL A 51") == "1");
    assert(bs->query("GS A 100") == "0-A");
    assert(bs->query("DEL A 1") == "1");
    assert(bs->query("GS A 100") == "-1");
    //  spam the command with CODE that match no routes.
    for (int i = 0; i < 100000; ++i) {
        string cmd = string("GS ") + to_string(i) + string(" 0");
        assert(bs->query(cmd) == "-1");
    }

    delete bs;
}

void GELogicTest() {
    BusSystem* bs = new BusSystem();
    // Normal
    assert(bs->query("SQ 12") == "1");
    assert(bs->query("INS A 0-A 001 100") == "1");
    assert(bs->query("INS A 1-A 101 200") == "2");
    assert(bs->query("INS A 2-A 201 300") == "3");
    assert(bs->query("INS A 3-A 401 500") == "4");
    assert(bs->query("INS A 4-A 601 700") == "5");
    assert(bs->query("INS A 5-A 51 400") == "6");
    assert(bs->query("INS A 6-A 451 650") == "7");
    assert(bs->query("INS A 7-A 1 451 650") == "8");
    assert(bs->query("INS A 8-A 1 475 650") == "9");
    assert(bs->query("INS A 9-A 1 651 750") == "10");

    assert(bs->query("GE A 100") == "-1");
    assert(bs->query("GE A 101") == "0-A");
    assert(bs->query("GE A 200") == "0-A");
    assert(bs->query("GE A 250") == "1-A");
    assert(bs->query("GE A 350") == "2-A");
    assert(bs->query("GE A 450") == "5-A");
    // CASE
    assert(bs->query("GE A 675") == "6-A");
    assert(bs->query("GE A 675 0") == "6-A");
    assert(bs->query("GE A 675 1") == "8-A");
    // Trivial Test
    assert(bs->query("GE A 0") == "-1");
    assert(bs->query("GE A 1000") == "9-A");
    // With DEL
    assert(bs->query("DEL A 101") == "1");
    assert(bs->query("GE A 250") == "0-A");
    assert(bs->query("DEL A 1") == "1");
    assert(bs->query("GE A 250") == "-1");
    //  spam the command with CODE that match no routes.
    for (int i = 0; i < 100000; ++i) {
        string cmd = string("GE ") + to_string(i) + string(" 0");
        assert(bs->query(cmd) == "-1");
    }

    delete bs;
}

int main() {
    exampleTest();

    invalidCommandTest();
    SQSyntaxTest();
    INSSyntaxTest();
    DELSyntaxTest();
    CSSyntaxTest();
    CESyntaxTest();
    GSSyntaxTest();
    GESyntaxTest();

    INSLogicTest();
    DELLogicTest();
    CSLogicTest();
    CELogicTest();
    GSLogicTest();
    GELogicTest();
    
    cout << "All tests passed\n";
}
