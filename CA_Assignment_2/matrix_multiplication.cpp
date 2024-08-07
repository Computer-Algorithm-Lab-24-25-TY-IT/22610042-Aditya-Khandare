#include <iostream>
#include <vector>
#include <limits>

using namespace std;

// Function to multiply two matrices
vector<vector<int>> multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int rowsA = A.size();
    int colsA = A[0].size();
    int rowsB = B.size();
    int colsB = B[0].size();

    if (colsA != rowsB) {
        throw invalid_argument("Number of columns in A must be equal to number of rows in B.");
    }

    vector<vector<int>> C(rowsA, vector<int>(colsB, 0));

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

// Function to recursively multiply matrices based on the optimal split
vector<vector<int>> matrixChainMultiply(const vector<vector<vector<int>>>& matrices, const vector<vector<int>>& s, int i, int j) {
    if (i == j) {
        return matrices[i];
    }

    vector<vector<int>> left = matrixChainMultiply(matrices, s, i, s[i][j]);
    vector<vector<int>> right = matrixChainMultiply(matrices, s, s[i][j] + 1, j);
    return multiplyMatrices(left, right);
}

// Function to find the minimum number of scalar multiplications needed to multiply the chain of matrices
pair<vector<vector<int>>, vector<vector<int>>> matrixChainMultiplication(const vector<int>& p) {
    int n = p.size() - 1;
    vector<vector<int>> dp(n, vector<int>(n, numeric_limits<int>::max()));
    vector<vector<int>> s(n, vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
        dp[i][i] = 0;
    }

    for (int l = 2; l <= n; ++l) {
        for (int i = 0; i < n - l + 1; ++i) {
            int j = i + l - 1;
            for (int k = i; k < j; ++k) {
                int q = dp[i][k] + dp[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
                if (q < dp[i][j]) {
                    dp[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }
    return {dp, s};
}

// Function to print a matrix
void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

// Function to print the optimal parenthesization of the matrix chain
void printOptimalParenthesization(const vector<vector<int>>& s, int i, int j, char& name) {
    if (i == j) {
        cout << name++;
        return;
    }
    cout << "(";
    printOptimalParenthesization(s, i, s[i][j], name);
    printOptimalParenthesization(s, s[i][j] + 1, j, name);
    cout << ")";
}

int main() {
    int n;
    cout << "Enter the number of matrices: ";
    cin >> n;

    vector<int> p(n + 1);
    cout << "Enter the dimensions of the matrices (p[i] and p[i+1] for matrix i):" << endl;
    for (int i = 0; i <= n; ++i) {
        cin >> p[i];
    }

    vector<vector<vector<int>>> matrices(n);
    for (int i = 0; i < n; ++i) {
        cout << "Enter the elements of matrix " << i + 1 << " (" << p[i] << "x" << p[i + 1] << "):" << endl;
        matrices[i].resize(p[i], vector<int>(p[i + 1]));
        for (int r = 0; r < p[i]; ++r) {
            for (int c = 0; c < p[i + 1]; ++c) {
                cin >> matrices[i][r][c];
            }
        }
    }

    auto [dp, s] = matrixChainMultiplication(p);

    cout << "Minimum number of multiplications is: " << dp[0][n - 1] << endl;

    cout << "Optimal parenthesization is: ";
    char name = 'A';
    printOptimalParenthesization(s, 0, n - 1, name);
    cout << endl;

    try {
        vector<vector<int>> result = matrixChainMultiply(matrices, s, 0, n - 1);
        cout << "Resultant matrix is:" << endl;
        printMatrix(result);
    } catch (const invalid_argument& e) {
        cerr << e.what() << endl;
    }

    return 0;
}
