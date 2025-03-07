#include <iostream>
#include <vector>
#include <unordered_set>
#include <cctype>
#include <map>
using namespace std;

// Function to generate the Playfair cipher matrix
typedef vector<vector<char>> Matrix;

Matrix generateMatrix(const string& key, int size) {
    unordered_set<char> used;
    Matrix matrix(size, vector<char>(size));
    string refinedKey;
    
    // Prepare alphabet (A-Z, optionally digits if size > 5)
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (size > 5) alphabet += "0123456789";
    
    // Process keyword
    for (char c : key) {
        c = toupper(c);
        if (used.find(c) == used.end() && (alphabet.find(c) != string::npos)) {
            refinedKey += c;
            used.insert(c);
        }
    }
    
    // Fill remaining alphabet
    for (char c : alphabet) {
        if (used.find(c) == used.end()) {
            refinedKey += c;
            used.insert(c);
        }
    }
    
    // Fill the matrix
    int idx = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = refinedKey[idx++];
        }
    }
    return matrix;
}

// Function to find the position of a letter in the matrix
void findPosition(const Matrix& matrix, char c, int& row, int& col) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == c) {
                row = i;
                col = j;
                return;
            }
        }
    }
}

// Function to prepare the plaintext (convert to uppercase, remove spaces, insert padding character for duplicate pairs)
string prepareText(string text, char padding, map<int, char>& spaceMap) {
    string result;
    for (size_t i = 0; i < text.length(); i++) {
        if (isalpha(text[i]) || isdigit(text[i])) {
            result += toupper(text[i]);
        } else if (text[i] == ' ') {
            spaceMap[result.length()] = ' ';
        }
    }
    
    for (size_t i = 0; i < result.length(); i += 2) {
        if (i + 1 < result.length() && result[i] == result[i + 1]) {
            result.insert(i + 1, 1, padding);
        }
    }
    if (result.length() % 2 != 0) result += padding;
    return result;
}

// Playfair encryption/decryption function
string playfairCipher(const string& text, const Matrix& matrix, bool encrypt) {
    string result;
    int size = matrix.size();
    
    for (size_t i = 0; i < text.length(); i += 2) {
        int row1, col1, row2, col2;
        findPosition(matrix, text[i], row1, col1);
        findPosition(matrix, text[i + 1], row2, col2);
        
        if (row1 == row2) {
            col1 = (col1 + (encrypt ? 1 : size - 1)) % size;
            col2 = (col2 + (encrypt ? 1 : size - 1)) % size;
        } else if (col1 == col2) {
            row1 = (row1 + (encrypt ? 1 : size - 1)) % size;
            row2 = (row2 + (encrypt ? 1 : size - 1)) % size;
        } else {
            swap(col1, col2);
        }
        result += matrix[row1][col1];
        result += matrix[row2][col2];
    }
    return result;
}

// Function to restore spaces in decrypted text
string restoreSpaces(const string& text, const map<int, char>& spaceMap) {
    string result;
    for (size_t i = 0; i < text.length(); i++) {
        if (spaceMap.find(i) != spaceMap.end()) {
            result += ' ';
        }
        result += text[i];
    }
    return result;
}

int main() {
    int size;
    string key, text;
    char padding;
    
    cout << "Enter matrix size (5-9): ";
    cin >> size;
    while (size < 5 || size > 9) {
        cout << "Invalid size! Enter a value between 5 and 9: ";
        cin >> size;
    }
    
    cout << "Enter keyword: ";
    cin >> key;
    cin.ignore();
    
    cout << "Enter padding character (e.g., X or Q): ";
    cin >> padding;
    cin.ignore();
    
    Matrix matrix = generateMatrix(key, size);
    
    cout << "Enter text: ";
    getline(cin, text);
    
    map<int, char> spaceMap;
    string preparedText = prepareText(text, padding, spaceMap);
    
    string encrypted = playfairCipher(preparedText, matrix, true);
    string decrypted = playfairCipher(encrypted, matrix, false);
    decrypted = restoreSpaces(decrypted, spaceMap);
    
    cout << "\nGenerated Playfair Matrix (" << size << "x" << size << "):\n";
    for (const auto& row : matrix) {
        for (char c : row) cout << "| " << c << " ";
        cout << "|\n";
    }
    
    cout << "\nPrepared Text: " << preparedText;
    cout << "\nEncrypted Text: " << encrypted;
    cout << "\nDecrypted Text: " << decrypted << "\n";
    return 0;
}
