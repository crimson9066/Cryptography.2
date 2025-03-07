# 1.Substitution Cipher Decryption Tool in C++
A C++ tool to decrypt substitution ciphers using frequency analysis, hill climbing with simulated annealing, and user-guided adjustments.

# Features
- Automatically analyzes letter frequency to generate an initial decryption key.
- Hill Climbing with Simulated Annealing which Optimizes the decryption key using a heuristic approach.
- Allows users to manually refine the substitution key for better accuracy.
- Uses English trigram frequencies to evaluate decrypted text.
- Loads trigram frequencies from a file for accurate scoring.

# 2.Substitution Cipher Decryption Tool in C++

A C++ tool to decrypt substitution ciphers using a brute-force approach by testing all possible key permutations.

# Features
- Attempts to break a monoalphabetic substitution cipher by testing all 26! possible key permutations.
- Decrypts the encrypted message using each key permutation and outputs all possible decrypted texts.
- The tool ensures that spaces and punctuation marks in the encrypted message are retained during the decryption process.
- Displays the decrypted message for each key permutation, making it easy to identify the correct one.

