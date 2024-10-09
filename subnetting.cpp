#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <unistd.h>
using namespace std;

string givenIpStr = "";
int numberOfSubnets = 0;
vector<int> givenIpVector;
vector<int> mySubnetMask = {255, 255, 255, 0}, subnetMaskVector;
vector<int> myIpVector;

string vectorToString(const vector<int>& v) {
    string as = to_string(v[0]);
    for (int i = 1; i < v.size(); i++) {
        as += "." + to_string(v[i]);
    }
    return as;
}

vector<int> splitString(const string& s) {
    vector<int> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, '.')) {
        tokens.push_back(stoi(token));
    }
    return tokens;
}

// Calculate subnet mask for the number of subnets
vector<int> calculateSubnetMask(int numberOfSubnets) {
    int bitsToSet = ceil(log2(numberOfSubnets));
    int subnetBits = 24 + bitsToSet;  // Assuming starting from /24 (255.255.255.0)
    int networkBits = 32 - subnetBits;

    vector<int> mask(4, 0);
    for (int i = 0; i < 4; i++) {
        if (subnetBits >= 8) {
            mask[i] = 255;
            subnetBits -= 8;
        } else if (subnetBits > 0) {
            mask[i] = (256 - (1 << (8 - subnetBits)));
            subnetBits = 0;
        }
    }

    return mask;
}

// Bitwise AND function
vector<int> bitwiseAnd(const vector<int>& v1, const vector<int>& v2) {
    vector<int> result;
    for (int i = 0; i < v1.size(); i++) {
        result.push_back(v1[i] & v2[i]);
    }
    return result;
}

// Convert vector to integer
int vectorToInteger(const vector<int>& ip) {
    return (ip[0] << 24) | (ip[1] << 16) | (ip[2] << 8) | ip[3];
}

// Convert integer to vector
vector<int> integerToVector(int ipInt) {
    return { (ipInt >> 24) & 255, (ipInt >> 16) & 255, (ipInt >> 8) & 255, ipInt & 255 };
}

// Print subnet ranges based on subnet mask
void printSubnetRanges(const vector<int>& subnetMaskVector) {
    int numberOfHostsPerSubnet = ~vectorToInteger(subnetMaskVector) + 1;

    vector<int> networkAddress = bitwiseAnd(myIpVector, subnetMaskVector);
    int networkBaseInt = vectorToInteger(networkAddress);

    for (int i = 0; i < numberOfSubnets; i++) {
        int subnetBaseInt = networkBaseInt + i * numberOfHostsPerSubnet;
        vector<int> subnetBase = integerToVector(subnetBaseInt);

        vector<int> firstAddress = subnetBase;
        firstAddress[3] += 1; // First usable address

        vector<int> lastAddress = integerToVector(subnetBaseInt + numberOfHostsPerSubnet - 2); // Last usable address

        cout << "Subnet " << (i + 1) << ": " 
             << "Subnet ID: " << vectorToString(subnetBase)
             << ", Allocation Range: " 
             << vectorToString(firstAddress) << " to "
             << vectorToString(lastAddress) << endl;
    }
}

// Execute ping command
void executeCommand() {
    vector<int> myNetworkAddress = bitwiseAnd(myIpVector, subnetMaskVector);
    vector<int> givenNetworkAddress = bitwiseAnd(givenIpVector, subnetMaskVector);

    if (myNetworkAddress == givenNetworkAddress) {
        string cmd = "ping -c 3 " + givenIpStr;
        system(cmd.c_str());
    } else {
        cout << "-----Out of my subnet---------" << ", I shouldn't ping" << endl;
    }
}

// Detect the IP of the local machine
vector<int> detectMyIP() {
    char buffer[128];
    string result = "";

    FILE* pipe = popen("hostname -I", "r");
    if (!pipe) {
        cerr << "Failed to run command" << endl;
        return {};
    }

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }

    pclose(pipe);
    string firstIp = result.substr(0, result.find(" "));
    return splitString(firstIp);
}

int main() {
    while (true) {
        myIpVector = detectMyIP();
        
        cout << "My IP is: " << vectorToString(myIpVector) << endl;
        
        cout << "Enter IP: ";
        cin >> givenIpStr;
        givenIpVector = splitString(givenIpStr);
        
        cout << "Enter number of subnets: ";
        cin >> numberOfSubnets;

        subnetMaskVector = calculateSubnetMask(numberOfSubnets);
        cout << "The subnet mask is: " << vectorToString(subnetMaskVector) << endl;

        printSubnetRanges(subnetMaskVector);
        executeCommand();
    }
    return 0;
}
