/* Note: 
    * You change anything in this answer 
    * such that your implementation must have BusSystem class with method query.
    * This query method should return the output of each instruction
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

#include <exception>
#include <sstream>
#include <unordered_map>
#include <list>

using namespace std;

int string2int(const string& s) {
    size_t sz;
    int i = 0;
    i = stoi(s, &sz);

    if (sz != s.size())
        throw invalid_argument("The whole string is not a number");
    return i;
}

/*
*   TokenList translates command to a list of tokens, stores each token as strings.
*   Responsible for:
*       Checking whether the syntax is valid in term of whitespaces format. The valid format is:
*           No spaces at the beginning and the end of the command
*           Between 2 arguments, there is no more than 1 whitespace.
*       The rest of checking for syntax validity is upto individual command execution method.
*/
class TokenList {
private:
    vector<string> tokens;

    static bool isWhitespace(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

public:
    TokenList() {
    } 

    /*
    *   Throws invalid_argument exception if the syntax is not valid.
    */
    TokenList(string cmd) {
        if (isWhitespace(cmd[0]) || isWhitespace(cmd.back()))
            throw invalid_argument("White space(s) at beginning or the end");
        for (int i = 0; i + 1 < (int)cmd.size(); ++i) 
            if (isWhitespace(cmd[i]) && isWhitespace(cmd[i + 1]))
                throw invalid_argument("2 consecutive white spaces");

        stringstream ss(cmd);
        while (1) {
            if (ss.eof()) 
                break;

            string t;
            ss >> t;
            tokens.push_back(t);
        }
    } 

    string& operator[](int index) {
        return tokens[index];
    }

    int size() {
        return tokens.size();
    }
};

class BusSystem {
private:
    struct Trip {
        string busID;
        int start, finish;
        bool dir;

        Trip() {
        }

        Trip(string _busID, int _start = 0, int _finish = 0, bool _dir = 0)
        : busID(_busID), start(_start), finish(_finish), dir(_dir) {

        }
    };

private:
    static const int MIN_TIME = -1;
    static const int MAX_TIME = 1e9;

    TokenList tkl;

    int maxNBus;
    unordered_map<string, list<Trip>> routes;

    /*  
    *   Set the maximum number of trips on a route.
    *   Should be only executed successfully once.
    * 
    *   There is always an SQ query and the start of the test.
    *   SQ has exactly 1 argument.
    *   All argument has the right type.
    *   
    *   return 1 if SQ succeeds, else return -1.
    */
    string SQ() {
        if (tkl.size() != 2)
            throw invalid_argument("SQ: The number of arguments is not 1");

        int n = string2int(tkl[1]);
        maxNBus = n;
        return "1";
    }


    /* 
    *   Insert a trip to a route.
    * 
    *   INS only succeeds when:
    *       There is no bus on the routes with the same startTime, dir.
    *       There is no trip with the same bus have 
    *       Has 4 or 5 arguments.
    *       All argument has the right type.
    *       If a bus a
    *    
    *   Default value of direction is 0.
    *   A bus only run on one single route.
    *   One bus can have mutiple trips on the same route.
    * 
    *   Return the total number of trips on the route if INS succeeds, else return -1. 
    */
    bool overlap(int x, int y, int u, int v) {
        return (x <= u && u <= y) || (x <= v && v <= y); 
    }
    string INS() {
        // Syntax check.
        if (tkl.size() < 5 || tkl.size() > 6)
            throw invalid_argument("INS: The number of arguments is not 4 or 5");

        string routeID = tkl[1];
        string busID = tkl[2];
        int dir = 0;
        int startTime = 0, finishTime = 0;
        if (tkl.size() == 5) {
            startTime = string2int(tkl[3]);
            finishTime = string2int(tkl[4]);
        }
        else {
            dir = string2int(tkl[3]);
            startTime = string2int(tkl[4]);
            finishTime = string2int(tkl[5]);
        }
        if ((int)routeID.size() > 5 || (int)busID.size() > 10)
            throw invalid_argument("INS: CODE has size greater than 5 or LP has size greater than 10");
        if (startTime >= finishTime)
            throw invalid_argument("INS: TIMEA is not smaller than TIMEB");
        if (dir > 1 || dir < 0)
            throw invalid_argument("INS: CASE is not a binary value");

        // Check whether the trip can be inserted or not.
        if ((int)routes[routeID].size() == maxNBus)
            return "-1";

        int lastFinishTime = -1;
        for (const auto& trip: routes[routeID])
            if (trip.busID == busID)
                lastFinishTime = max(lastFinishTime, trip.finish);
        if (startTime <= lastFinishTime)
            return "-1";

        for (const auto& trip:routes[routeID]) 
            if (trip.dir == dir && trip.start == startTime)
                return "-1";

        // Insert the trip to the route.
        routes[routeID].emplace_back(busID, startTime, finishTime, dir);
        return to_string(routes[routeID].size());
    }

    /* 
    *   Delete trip(s) from a route with hint if provided.
    * 
    *   DEL only succeeds when:
    *       Has 1 or 2 or 3 argument(s).
    *       All argument has the right type.
    *    
    *   In case no time argument is provided, delete every trip in the route.
    *   In case only startTime is provided, delete every trip has startTime <= trip.start.
    *   In case bot startTime and finishTime is provided, delete every trip 
    *   has startTime <= trip.start <= finishTime.
    * 
    *   Return the total number of deleted trip if DEL succeeds, else return -1.
    */
    string DEL() {
        // Syntax check.
        if (tkl.size() < 2 || tkl.size() > 4)
            throw invalid_argument("DEL: The number of arguments is not 1 or 2 or 3");

        string routeID = tkl[1];
        int startTime = MIN_TIME;
        int finishTime = MAX_TIME;
        if (tkl.size() > 2) 
            finishTime = startTime = string2int(tkl[2]);
        if (tkl.size() > 3) 
            finishTime = string2int(tkl[3]);
        if ((int)routeID.size() > 5)
            throw invalid_argument("DEL: CODE has size greater than 5");
        if (startTime > finishTime)
            throw invalid_argument("DEL: TIMEA is greater than TIMEB");
    
        // If the route does not exists, return 0.
        if (!routes.count(routeID))
            return "0";

        // Delete all the trip in the route with trip.start in [startTime..finishTime].
        int cDeleted = 0;
        auto it = routes[routeID].begin();
        while (it != routes[routeID].end()) 
            if (startTime <= it->start && it->start <= finishTime) {
                it = routes[routeID].erase(it);
                ++cDeleted;
            }
            else 
                ++it;

        return to_string(cDeleted);
    }

    /*
    *   Count the number of trip that has started but has not finished at the provided time
    *   with hint if provided.
    * 
    *   CS only succeeds when:
    *       Has 2 or 3 arguments.
    *       All argument has the right type.
    *   
    *   A trip is said to have started at the time t if trip.start >= t.
    *   A trip is said to have finished at the time t if trip.end < t.
    *   So a trip is said to be going at the time t if trip.start <= t <= trip.finish.
    * 
    *   If direction is specified, limit only to the trips go on that direction, else count both 
    *   directions.
    *   Return the number of trip fits the description on success, else return -1.
    */
    string CS() {
        // Syntax check
        if (tkl.size() < 3 || tkl.size() > 4)
            throw invalid_argument("CS: The number of arguments is not 2 or 3");

        string routeID = tkl[1];
        int time = string2int(tkl[2]);
        int dir = -1;
        if (tkl.size() > 3) {
            dir = string2int(tkl[3]);
            if (dir > 1 || dir < 0)
                throw invalid_argument("CS: CASE is not a binary value");
        }
        if ((int)routeID.size() > 5)
            throw invalid_argument("CS: CODE has size greater than 5");

        // If the route does not exists, return 0.
        if (!routes.count(routeID))
            return "0";

        int res = 0;
        for (const auto& trip: routes[routeID])
            res += trip.start <= time && time <= trip.finish && (dir < 0 || dir == trip.dir);
        return to_string(res);
    }

    /*
    *   Count the number of trip that has finished at the provided time with hint if provided.
    * 
    *   CE only succeeds when:
    *       has 2 or 3 arguments.
    *       all argument has the right type.
    *   
    *   A trip is said to have finished at the time t if trip.finish < t.
    * 
    *   If direction is specified, limit only to the trips go on that direction, else count both 
    *   directions.
    *   Return the number of trip fits the description on success, else return -1.
    */
    string CE() {
        // Syntax check
        if (tkl.size() < 3 || tkl.size() > 4)
            throw invalid_argument("CE: the number of arguments is not 2 or 3");

        string routeID = tkl[1];
        int time = string2int(tkl[2]);
        int dir = -1;
        if (tkl.size() > 3) {
            dir = string2int(tkl[3]);
            if (dir > 1 || dir < 0)
                throw invalid_argument("CE: CASE is not a binary value");
        }
        if ((int)routeID.size() > 5)
            throw invalid_argument("CE: CODE has size greater than 5");
            
        // If the route does not exists, return 0.
        if (!routes.count(routeID))
            return "0";

       int res = 0;
        for (const auto& trip: routes[routeID])
            res += time > trip.finish && (dir < 0 || dir == trip.dir);
        return to_string(res);
    }

    /*
    *   Find the bus that has started and its start time is closest to the time specified with hint
    *   if provided.
    * 
    *   GS only succeeds when:
    *       has 2 or 3 arguments.
    *       all argument has the right type.
    *   
    *   If direction is specified, limit only to the trips go on that direction, else search on
    *   both directions.
    *   Return the ID of the bus fits the description on success, else return -1.
    *   In case mutiple results found, prioritize the trip with trip = 0. 
    */
    bool betterGS(const Trip& a, const Trip& b, int time) {
        if (abs(a.start - time) != abs(b.start - time))
            return abs(a.start - time) < abs(b.start - time);
        return a.dir == 0;
    }
    string GS() {
        // Syntax check
        if (tkl.size() < 3 || tkl.size() > 4)
            throw invalid_argument("GS: the number of arguments is not 2 or 3");

        string routeID = tkl[1];
        int time = string2int(tkl[2]);
        int dir = -1;
        if (tkl.size() > 3) {
            dir = string2int(tkl[3]);
            if (dir > 1 || dir < 0)
                throw invalid_argument("GS: CASE is not a binary value");
        }
        if ((int)routeID.size() > 5)
            throw invalid_argument("GS: CODE has size greater than 5");

        // If the route does not exists, return -1.
        if (!routes.count(routeID))
            return "-1";

        Trip res = Trip("-1", MIN_TIME, MIN_TIME);
        for (const auto& trip: routes[routeID])
            if (time >= trip.start && (dir < 0 || dir == trip.dir))
                if (betterGS(trip, res, time))
                    res = trip;
        return res.busID;
    }

    /*
    *   Find the bus that has finished with its finish time closest to the time specified with
    *   hint if provided.
    * 
    *   GE only succeeds when:
    *       has 2 or 3 arguments.
    *       all argument has the right type.
    *   
    *   If direction is specified, limit only to the trips go on that direction, else search both 
    *   directions.
    *   Return the ID of the bus fits the description on success, else return -1.
    *   In case mutiple result found, prioritize the bus with trip = 0, the prioritize the bus
    *   that starts later.
    */
    bool betterGE(const Trip& a, const Trip& b, int time) {
        if (abs(a.finish - time) != abs(b.finish - time))
            return abs(a.finish - time) < abs(b.finish - time);
        if (a.dir != b.dir)
            return a.dir == 0;
        return a.start > b.start;
    }
    string GE() {
        // Syntax check
        if (tkl.size() < 3 || tkl.size() > 4)
            throw invalid_argument("GE: the number of arguments is not 2 or 3");

        string routeID = tkl[1];
        int time = string2int(tkl[2]);
        int dir = -1;
        if (tkl.size() > 3) {
            dir = string2int(tkl[3]);
            if (dir > 1 || dir < 0)
                throw invalid_argument("GE: CASE is not a binary value");
        }
        if ((int)routeID.size() > 5)
            throw invalid_argument("GE: CODE has size greater than 5");

        // If the route does not exists, return -1.
        if (!routes.count(routeID))
            return "-1";

        Trip res = Trip("-1", MIN_TIME, MIN_TIME);
        for (const auto& trip: routes[routeID])
            if (time > trip.finish && (dir < 0 || dir == trip.dir))
                if (betterGE(trip, res, time))
                    res = trip;
        return res.busID;
    }

public:
    string query(string instruction) {
        try {
            tkl = TokenList(instruction);

            if (tkl[0] == "SQ")
                return SQ();
            if (tkl[0] == "INS")
                return INS();
            if (tkl[0] == "DEL")
                return DEL();
            if (tkl[0] == "CS")
                return CS();
            if (tkl[0] == "CE")
                return CE();
            if (tkl[0] == "GS")
                return GS();
            if (tkl[0] == "GE")
                return GE();
        }
        // catch (const invalid_argument& e) {
        //     cerr << e.what() << '\n';
        //     return "-1";
        // }
        // catch (const char* e) {
        //     cerr << e << '\n';
        //     return "-1";
        // }
        catch (...) {
            return "-1";
        }

        // No matching command
        return "-1";
    }

    string toString() {
        stringstream ss;
        ss << maxNBus;
        for (const auto& route: routes) {
            ss << '\n' << route.first << ' ';
            for (const auto& trip: route.second)
                ss << trip.busID << '|' << trip.start << '|' << trip.finish << '|' << trip.dir << ' ';
        }

        return ss.str();
    }
};
