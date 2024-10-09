#include <iostream>
#include <unistd.h>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for seeding the random number generator
#include <cmath>    // for pow() function

using namespace std;

// Function for Go-Back-N ARQ protocol
void GoBackN(int total_frame, int window_size) {
    int sent_cnt = 0;
    int i = 1;

    while (i <= total_frame) {
        int z = 0;
        int k = i;

        // Sending frames
        while (k < i + window_size && k <= total_frame) {
            cout << "Frame " << k << " sent" << endl;
            sent_cnt++;
            k++;
        }

        k = i;
        sleep(1);  // Simulate transmission delay

        // Acknowledging frames
        while (k < i + window_size && k <= total_frame) {
            int f = rand() % 2;  // Randomly simulate ACK or timeout
            if (!f) {
                cout << "Acknowledgment for Frame " << k << endl;
                z++;
            } else {
                cout << "Timeout! Frame Number: " << k << " Not Received" << endl;
                break;
            }
            k++;
        }

        sleep(2);  // Simulate processing delay
        
        cout << "\n";
        if (z > 0) {
            cout << "Window slided" << endl;  // Window slides when frames are acknowledged
        }

        i = i + z;  // Slide the window
    }

    cout << "Total number of frames sent and resent: " << sent_cnt << endl;
}

// Function for Selective Repeat ARQ protocol
void SelectiveRepeat(int total_frame, int window_size) {
    int window[total_frame];
    for (int i = 0; i < total_frame; i++) {
        window[i] = i;
    }

    int send[window_size], receive[window_size], acked[total_frame];
    for (int i = 0; i < total_frame; i++) {
        acked[i] = -1;
    }

    for (int i = 0; i < window_size; i++) {
        send[i] = -1;
        receive[i] = -1;
    }

    int i = 0, k = 0, p = 0;
    while (i < total_frame) {
        int flag = 1, j;

        // Checking for unacknowledged frames (timeouts)
        for (j = 0; j < i && j < window_size; j++) {
            if (acked[j] == 0) {
                cout << "Timeout for frame " << j << ". Retransmitting frame " << j << endl;
                flag = 0;
                break;
            }
        }

        if (flag == 0) {
            int temp = i;
            i = j;
            cout << "Frame " << i << " is sent" << endl;
            receive[p] = window[i];
            cout << "Frame " << i << " is received" << endl;
            acked[i] = 1;
            cout << "Ack is sent for frame " << i << endl;
            i = temp;
            p = (p + 1) % window_size;
        } else {
            int rn = rand() % 5;
            if (rn == 1) {
                acked[i] = 0;
                cout << "Frame " << i << " is lost." << endl;
                send[k] = window[i];
                i++;
                k = (k + 1) % window_size;
            } else {
                send[k] = window[i];
                cout << "Frame " << i << " is sent" << endl;
                receive[p] = window[i];
                cout << "Frame " << i << " is received." << endl;
                acked[i] = 1;
                cout << "Ack is sent for frame " << i << endl;
                i++;
                k = (k + 1) % window_size;
                p = (p + 1) % window_size;
            }
        }

        cout << "Sender Window:" << endl;
        int temp = k;
        for (int l = 0; l < window_size; l++) {
            if (send[temp] != -1)
                cout << send[temp] << "  ";
            temp = (temp + 1) % window_size;
        }
        cout << endl;

        cout << "Receiver Window:" << endl;
        temp = p;
        for (int l = 0; l < window_size; l++) {
            if (receive[temp] != -1)
                cout << receive[temp] << " ";
            temp = (temp + 1) % window_size;
        }
        cout << endl;
        
        cout << "Window slided" << endl;  // Window slides after each successful frame reception and acknowledgment
        sleep(2);  // Simulate processing delay
    }
}

int main() {
    srand(time(0));  // Seed the random number generator
    int total_frame, n, window_size, choice;

    do {
        cout << "\n--- ARQ Protocol Simulation ---" << endl;
        cout << "1. Go-Back-N ARQ" << endl;
        cout << "2. Selective Repeat ARQ" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1 || choice == 2) {
            cout << "Enter the Total number of frames: ";
            cin >> total_frame;

            cout << "Enter n: ";
            cin >> n;

            // Calculate window size based on protocol
            if (choice == 1) {
                // Go-Back-N: Window Size = 2^n - 1
                window_size = pow(2, n) - 1;
                cout << "Calculated Window Size (Go-Back-N): " << window_size << endl;
                GoBackN(total_frame, window_size);
            } else if (choice == 2) {
                // Selective Repeat: Window Size = 2^(n-1)
                window_size = pow(2, n - 1);
                cout << "Calculated Window Size (Selective Repeat): " << window_size << endl;
                SelectiveRepeat(total_frame, window_size);
            }
        } else if (choice != 3) {
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 3);

    cout << "Exiting program..." << endl;
    return 0;
}
