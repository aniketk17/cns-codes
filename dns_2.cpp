#include <bits/stdc++.h>
using namespace std;
void nslookupIP(string &ip) {
    string command = "nslookup " + ip;
    cout << "\n"<<command << endl;
    system(command.c_str());
}
void nslookupURL(string &url) {
    string command = "nslookup " + url;
    cout << "\n" << command <<endl;
    system(command.c_str());
}

int main() {
    int choice;
    string input;

    int opt=1;
    while(opt==1)
    {
        cout << "1.Enter IP to CHECK IT'S URL \n2.ENTER URL TO CHECK THE IP ADDRESS:"<<endl;
        cin >> choice;
        if (choice == 1) {
            cout << "Enter IP address: ";
            cin>>input;
            nslookupIP(input);
        } else if (choice == 2) {
            std::cout << "Enter the URL: ";
            cin>>input;
            nslookupURL(input);
        } else {
            std::cout << "Invalid choice" << std::endl;
        }
        cout<<"\nEnter 1 to continue and O to Exit:";
        cin>>opt;
    }

    return 0;
}