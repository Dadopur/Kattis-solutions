#include <vector>
#include <iostream>
#include <tuple>
#include <algorithm>
//#include <limits>
#include <cfloat>
using namespace std;


bool interval_compare(tuple<double, double, int>& i1, tuple<double, double, int>& i2) {
    return (get<0>(i1) < get<0>(i2));
}

vector<int> count_intervals(tuple<double, double> interval, vector<tuple<double, double, int>> intervals) {
    // Initiate variables
    double primary_start = get<0>(interval);
    double primary_end = get<1>(interval);
    double start = primary_start;
    double end = start - 1;
    vector<int> indexes;

    // Sort interval with complexity O(NlogN) on start value
    sort(intervals.begin(), intervals.end(), interval_compare);

    //intervals.push_back(make_tuple(numeric_limits<double>::infinity(), numeric_limits<double>::infinity(), -1));
    intervals.push_back(make_tuple(DBL_MAX, DBL_MAX, -1));
    
    int index = -1;
    for (unsigned i = 0; i < intervals.size();) {
        tuple<double, double, int> new_interval = intervals[i];
        double new_start = get<0>(new_interval);
        double new_end = get<1>(new_interval);

        // Update end, is it a better interval or worse?
        if (new_start <= start) {
            if (new_end > end) {
                end = new_end;
                index = get<2>(new_interval);
            }
            i++;
        } else {
            // Current interval doesn't cover the start, need a new interval
            start = end;

            if (index != -1) { indexes.push_back(index);}
            // End of loop, impossible or done
            if (new_start > end || end >= primary_end) {
                break;
            }
        }
    }

    // Returns
    if (end < primary_end) {
        // Can't find valid intervals to cover interval.
        vector<int> invalid = {-1};
        return invalid;
    }
    
    return indexes;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    // Get interval to be checked
    double start_interval;
    double end_interval;
    while(cin >> start_interval >> end_interval) {
        tuple<double, double> interval = make_tuple(start_interval, end_interval);

        // Number of intervals that will be given. 
        int num_intervals;
        cin >> num_intervals;
        //cout <<"STARTING "<< num_intervals << endl;
        // Create intervals vector
        vector<tuple<double, double, int>> intervals;
        for (int i = 0; i < num_intervals; i++) {
            cin >> start_interval >> end_interval;
            intervals.push_back(make_tuple(start_interval, end_interval, i));
        }
        // Find number of counts
        vector<int> interval_count = count_intervals(interval, intervals);

        // Print results
        if (interval_count.size() != 0 && interval_count.front() == -1) {
            cout << "impossible\n";
        } else {
            cout << interval_count.size() << "\n";
            for(int count : interval_count) {
                cout << count << " ";
            }
            cout << "\n";
        }
    }
}