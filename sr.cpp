#include<iostream>
#include<cstdlib>
#include<ctime>
#include<cmath>

using namespace std;

int receiver(int);
int simulate(int);
int negack(int);

int main() {
    srand(time(0));

    int tmp1, tmp2 = 0, tmp3 = 0, tmp4 = 0, tmp5 = 0;
    int i, frame, morePacket, m;
    int windowsize;

    m=4;

    windowsize = pow(2, m - 1);

    cout << "Window size calculated using Selective Repeat formula is: " << windowsize << endl;
    
    cout<<"Enter the message: ";
    string msg;
    cin.ignore();
    getline(cin,msg);
    frame=msg.size();

    morePacket = frame;

    while(morePacket > 0) {
        tmp1 = simulate(windowsize);
        windowsize -= tmp1;
        tmp4 += tmp1;
        
        if(tmp4 > frame)
            tmp4 = frame;
            
        for(i = frame - morePacket; i < tmp4; i++) {
            cout << "\nSending Frame " << i;
        }
            
        tmp2 = receiver(tmp1);
        tmp3 += tmp2;
        
        if(tmp3 > frame)
            tmp3 = frame;

        tmp2 = negack(tmp1);
        tmp5 += tmp2;
        
        if(tmp5 != 0) {
            cout << "\nNo acknowledgement for the frame " << tmp5;
            cout << "\nRetransmitting frame " << tmp5;
        }
        
        morePacket -= tmp1;
        
        windowsize = pow(2, m - 1);
    }

    cout << "\nAll packets are successfully transmitted." << endl;

    return 0;
}

int receiver(int tmp1) {
    int i = rand() % tmp1;
    return i;
}

int negack(int tmp1) {
    int i = rand() % tmp1;
    return i;
}

int simulate(int windowsize) {
    int tmp1 = rand();
    
    if(tmp1 == 0)
        tmp1 = simulate(windowsize);
    
    int i = tmp1 % windowsize;
    
    if(i == 0)
        return windowsize;
    else
        return i;
}