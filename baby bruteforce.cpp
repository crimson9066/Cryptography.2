#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

// Function to decrypt text using a substitution key
string decryptText(const string& text, const string& key) {
    string decrypted;
    for (char c : text) {
        if (isalpha(c)) {
            decrypted += key[toupper(c) - 'A'];
        } else {
            decrypted += c; // Preserve spaces and punctuation
        }
    }
    return decrypted;
}

// Function to generate all permutations of the alphabet
void generatePermutations(string& key, vector<string>& permutations) {
    do {
        permutations.push_back(key);
    } while (next_permutation(key.begin(), key.end()));
}

// Function to brute-force the cipher by trying all key permutations
void bruteForceAttack(const string& encryptedText) {
    string key = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    vector<string> permutations;

    // Generate all permutations of the alphabet
    generatePermutations(key, permutations);

    // Try each key permutation and decrypt the message
    int count = 0;
    for (const string& perm : permutations) {
        string decryptedText = decryptText(encryptedText, perm);
        cout << "Decrypted Text with Key " << perm << ":\n" << decryptedText << "\n\n";
        count++;
    }
    cout << "Total keys tested: " << count << endl;
}

int main() {
    srand(time(0)); // Seed the random number generator

    string encryptedText;
    cout << "Enter the encrypted text: ";
    getline(cin, encryptedText);

    // Perform brute-force attack on the encrypted message
    bruteForceAttack(encryptedText);

    return 0;
}
