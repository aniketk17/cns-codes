#include <bits/stdc++.h>
#include <unistd.h>
#include <cmath>
using namespace std;

int N;
void transmission(int &total_frame, int &sent_cnt)
{
    int i = 1;
    while (i <= total_frame)
    {
        int z = 0;
        int k = i; 
        while (k < i + N && k <= total_frame)
        {
            cout << "Frame " << k << " sent" << endl;
            sent_cnt++;
            k++;
        }

        k = i;
        sleep(1);   
        while (k < i + N && k <= total_frame)
        {
            int f = rand() % 4;
            if (f)
            {
                cout << "Acknowledgment for Frame " << k << endl;
                z++;
            }
            else
            {
                cout << "Timeout!! Frame Number : " << k << " Not Received" << endl;
                break;
            }

            k++;
        }

        sleep(2);
        
        cout << "\n";
        if (z > 0)
        {
            cout << "Window slided" << endl;
        }
        i = i + z;
    }
}

int main()
{
    int total_frame=0, sent_cnt = 0, m=2;

    cout<<"\nenter m:";
    cin>>m;
    cout<<"Enter the msg len: ";
    cin>>total_frame;
    
    N = pow(2, m) - 1;
    cout << "Calculated Window Size (2^m - 1): " << N << endl;

    transmission(total_frame, sent_cnt);
    cout << "Total number of frames which were sent and resent are: " << sent_cnt << endl; 

    return 0;
}