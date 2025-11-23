#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
using namespace std;

// ---------- Linked List for Appliances ----------
struct Appliance {
    string name;
    float unitsPerHour;
    float hoursPerDay;
    Appliance* next;
};

Appliance* createApplianceList() {
    Appliance* head = nullptr;

    auto add = [&](const string& name, float uph, float hrs) {
        Appliance* node = new Appliance{name, uph, hrs, head};
        head = node;
    };

    add("Iron", 1.0, 0.5);
    add("Washing Machine", 0.5, 1);
    add("AC", 1.5, 5);
    add("TV", 0.1, 4);
    add("Refrigerator", 0.15, 24);
    add("Light", 0.01, 6);
    add("Fan", 0.05, 8);

    return head;
}

float calculateApplianceUsage(Appliance* head, ofstream& out) {
    float total = 0;
    out << "Appliance,Units/Day,Cost/Day\n";
    while (head) {
        float units = head->unitsPerHour * head->hoursPerDay;
        float cost = units <= 1 ? units * 5 : (units <= 3 ? units * 7 : units * 10);
        out << head->name << "," << units << ",Rs" << cost << "\n";
        total += units;
        head = head->next;
    }
    return total;
}

// ---------- Sliding Window Max 7-day Usage ----------
int slidingWindowMax(const vector<int>& arr, int k) {
    int maxSum = 0, sum = 0;
    for (int i = 0; i < k; ++i) sum += arr[i];
    maxSum = sum;
    for (int i = k; i < arr.size(); ++i) {
        sum += arr[i] - arr[i - k];
        maxSum = max(maxSum, sum);
    }
    return maxSum;
}

int main() {
    ifstream in("data/usage.txt");
    ofstream out("data/bill_data.csv");

    if (!in || !out) {
        cerr << "❌ Cannot open file. Make sure usage.txt and data folder exist.\n";
        return 1;
    }

    string line;
    getline(in, line);
    stringstream ss(line);
    vector<int> usage;
    string temp;
    while (getline(ss, temp, ',')) {
        usage.push_back(stoi(temp));
    }

    // Total Bill Calculation (Slab)
    float totalBill = 0;
    out << "===== Daily Usage and Bill =====\n";
    out << "Day,Units,Bill\n";
    for (size_t i = 0; i < usage.size(); ++i) {
        int u = usage[i];
        float cost = u <= 100 ? u * 5 : (u <= 200 ? u * 7 : u * 10);
        totalBill += cost;
        out << (i + 1) << "," << u << ",Rs" << cost << "\n";
    }

    out << "\nTotal Units: " << accumulate(usage.begin(), usage.end(), 0) << "\n";
    out << "Total Bill: Rs" << totalBill << "\n";

    if (usage.size() >= 7) {
        int maxWeek = slidingWindowMax(usage, 7);
        out << "Max 7-day Usage: " << maxWeek << " units\n";
    }

    // Appliance-based breakdown
    out << "\n===== Appliance-wise Usage =====\n";
    Appliance* appliances = createApplianceList();
    float totalApplianceUnits = calculateApplianceUsage(appliances, out);
    out << "Total Estimated Appliance Usage/Day: " << totalApplianceUnits << " units\n";

    in.close();
    out.close();

    cout << "✅ Bill saved to 'data/bill_data.csv'. Open in Excel or Google Sheets for charts!\n";
    return 0;
}




