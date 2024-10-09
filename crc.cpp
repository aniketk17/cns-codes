#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// Function to convert a character to its binary representation
void charToBinary(char ch, int binary[]) {
    int a = int(ch);
    for (int i = 7; i >= 0; i--) {
        binary[i] = a % 2;
        a /= 2;
    }
}

// Function to perform CRC division
void performCRC(int data[], int dataLength, int divisor[], int divisorLength, int remainder[]) {
    int temp[50];
    for (int i = 0; i < dataLength; i++) {
        temp[i] = data[i];
    }

    for (int i = 0; i < dataLength - divisorLength + 1; i++) {
        if (temp[i] == 1) {
            for (int j = 0; j < divisorLength; j++) {
                temp[i + j] = temp[i + j] ^ divisor[j];
            }
        }
    }

    for (int i = 0; i < divisorLength - 1; i++) {
        remainder[i] = temp[dataLength - divisorLength + 1 + i];
    }
}

// Function to induce an error at a random position
void induceError(int data[], int length) {
    int errorBit = rand() % length;
    data[errorBit] = data[errorBit] ^ 1;
}

// Function to process a character with CRC
bool processCharacter(char ch) {
    cout << "\nCharacter: " << ch << " (Ascii: " << int(ch) << ")";
    int binary[8];
    charToBinary(ch, binary);

    cout << "\nCharacter: " << ch << " (Binary: ";
    for (int i = 0; i < 8; i++) {
        cout << binary[i];
    }
    cout << ")" << endl;

    int divisorLength = 4;
    int divisor[4] = {1, 1, 0, 1}; // Example divisor (13 in binary)
    int remainder[3] = {0, 0, 0};

    int data[11];
    for (int i = 0; i < 8; i++) {
        data[i] = binary[i];
    }
    for (int i = 8; i < 11; i++) {
        data[i] = 0; // Append 0's for CRC calculation
    }

    performCRC(data, 11, divisor, divisorLength, remainder);

    cout << "CRC Remainder: ";
    for (int i = 0; i < 3; i++) {
        cout << remainder[i];
    }
    cout << endl;

    // Replace the trailing 0s with the remainder to form the new dividend
    for (int i = 0; i < 3; i++) {
        data[8 + i] = remainder[i];
    }

    cout << "New Dividend (after appending CRC): ";
    for (int i = 0; i < 11; i++) {
        cout << data[i];
    }
    cout << endl;

    // Ask the user if they want to induce an error
    char induceErrorChoice;
    cout << "Do you want to induce an error? (y/n): ";
    cin >> induceErrorChoice;

    if (induceErrorChoice == 'y' || induceErrorChoice == 'Y') {
        induceError(data, 11);
    }

    int receivedRemainder[3] = {0, 0, 0};
    performCRC(data, 11, divisor, divisorLength, receivedRemainder);

    bool errorDetected = false;
    for (int i = 0; i < 3; i++) {
        if (receivedRemainder[i] != 0) {
            errorDetected = true;
            break;
        }
    }

    if (!errorDetected) {
        cout << "Syndrome=";
        for (int i = 0; i < 3; i++) {
            cout << receivedRemainder[i];
        }
        cout << endl;
        cout << "As syndrome=000, thus No error detected. Message: (" << ch << ") transmitted correctly "<< endl;
        return true; // No error detected, return true
    } else {
        cout << "Syndrome=";
        for (int i = 0; i < 3; i++) {
            cout << receivedRemainder[i];
        }
        cout << endl;
        cout << "As syndrome!=000, thus Error detected. Code word discarded." << endl;
        return false; // Error detected, return false
    }
}

int main() {
    srand(time(0)); // Seed the random number generator
    string input;
    cout << "Enter a string: ";
    cin >> input;

    bool allCorrect = true; // To track if any errors were detected
    string receivedWord = ""; // To store the correctly received characters

    for (char ch : input) {
        if (processCharacter(ch)) {
            // cout << "Transmitted Frame: " << ch << endl;
            receivedWord += ch; // Append character if no error detected
        } else {
            allCorrect = false; // Mark as false if an error was detected
        }
    }

    if (allCorrect) {
        cout << "Received Word: " << input << endl;
        cout << "Message transmitted successfully without errors." << endl; // Transfer message
    } else {
        cout << "Received Word: " << receivedWord << endl; // Show correctly received characters
        cout << "Some code words were discarded due to errors." << endl;
    }

    return 0;
}
