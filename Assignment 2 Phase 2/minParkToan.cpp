#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <random>
using namespace std;

class Interval
{
public:
    int start;
    int end;

    Interval(int start = 0, int end = 0)
    {
        this->start = start;
        this->end = end;
    }
};

int minPark(vector<Interval> lInterval)
{
//     std::multimap<int, int> treeIn;
//     for (int i = 0; i < (int)lInterval.size(); i++)
//     {
//         treeIn.insert({lInterval[i].start, 0});
//     }
//     std::multimap<int, int> treeOut;
//     for (int i = 0; i < (int)lInterval.size(); i++)
//     {
//         treeOut.insert({lInterval[i].end, 0});
//     }
//     std::vector<int> vIn;
//     for (auto &node : treeIn)
//     {
//         vIn.push_back(node.first);
//         int size = (int)vIn.size();
//     }
//     std::vector<int> vOut;
//     for (auto &node : treeOut)
//     {
//         vOut.push_back(node.first);
//         int size = (int)vOut.size();
//     }
//     int idxStart = 0;
//     int idxEnd = 0;
//     int cnt = 0;
//     int max = 0;
//     while (idxStart < (int)vIn.size())
//     {
//         //cout << "Compare: " << vIn[idxStart] << " " << vOut[idxEnd] << endl;
//         if (vIn[idxStart] < vOut[idxEnd])
//         {
//             //cout << "Coming at " << vIn[idxStart] << endl;
//             cnt++;
//             idxStart++;
//         }
//         else if (vIn[idxStart] > vOut[idxEnd])
//         {
//             //cout << "Going at " << vOut[idxStart] << endl;
//             cnt--;
//             idxEnd++;
//         }
//         else if (vIn[idxStart] == vOut[idxEnd])
//         {
//             //cout << "No Change at " << vIn[idxStart] << endl;
//             idxStart++;
//             idxEnd++;
//         }
//         max = std::max(max, cnt);
//         if (idxStart == (int)vIn.size())
//         {
//             break;
//         }
//     }
//     return max;
// }

       /*-------------------------------------------------------------------------------*/

    sort(lInterval.begin(), lInterval.end(), [](const Interval &a, const Interval &b) {
        if (a.start != b.start)
            return a.start < b.start;
        else
            return a.end < b.end;
    });

    vector<int> heap;
    heap.reserve(lInterval.size());
    int res = 0;
    for (int i = 0; i < (int)(lInterval.size()); ++i)
    {
        int s = lInterval[i].start;
        int t = lInterval[i].end;

        while (!heap.empty() && heap[0] < s)
        {
            swap(heap[0], heap[heap.size() - 1]);
            heap.pop_back();

            size_t u = 0;
            while (1)
            {
                size_t v = 2 * u + 1;
                if (v >= heap.size())
                    break;
                if (v + 1 < heap.size() && heap[v + 1] < heap[v])
                    ++v;

                if (heap[u] < heap[v])
                    break;
                swap(heap[u], heap[v]);
                u = v;
            }
        }

        heap.push_back(t);
        res = max(res, (int)heap.size());
    }

    return res;
}

void printOutput(vector<Interval> &lInterval)
{
    cout << "Minimum Bus Parking Plot: " << minPark(lInterval) << endl;
    //    for (auto& i : lInterval)
    //    {
    //        cout << i.start << " " << i.end << endl;
    //    }
    cout << "-----------------------------------" << endl;
}

void TestCase()
{
    /*Simple Test Case*/
    {
        vector<Interval> lInterval;
        lInterval.push_back(Interval(2, 6));
        printOutput(lInterval);
        lInterval.push_back(Interval(8, 12));
        printOutput(lInterval);
        lInterval.push_back(Interval(4, 9));
        printOutput(lInterval);
        lInterval.push_back(Interval(0, 6));
        printOutput(lInterval);
        lInterval.push_back(Interval(5, 9));
        printOutput(lInterval);
        lInterval.push_back(Interval(5, 10));
        printOutput(lInterval);
    }

    {
        mt19937 rng;
        int n = 1000;
        vector<Interval> lInterval;
        for (int i = 0; i < n; ++i)
        {
            int s = rng() % n;
            int t = rng() % n;
            if (s == t)
                ++t;
            if (s > t)
                swap(s, t);

            //cout << "s, t: " << s << " " << t << endl;
            lInterval.push_back(Interval(s, t));
            printOutput(lInterval);
        }
    }
}

int main()
{
    freopen("outputToan.txt", "w", stdout);
    TestCase();
}