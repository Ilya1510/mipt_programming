#include "rational.h"
#include "finite.h"
#include <iostream>
#include <stdio.h>
#include <vector>

typedef unsigned uint;

template <unsigned N, typename Field = Rational>
class SquareMatrix;

template <typename Field>
class RowMatrix {
    Field* row_;
public:
    RowMatrix(Field* a) : row_(a) {}
    const Field& operator[](unsigned j) const {
        return row_[j];
    }
    Field& operator[](unsigned j) {
        return row_[j];
    }
};

template <unsigned M, unsigned N, typename Field = Rational>
class Matrix {
private:
    Field matrix_[M][N];
    const std::pair<Matrix, Matrix> gaussMethod(bool needInvert) const {
        unsigned currentHeight = 0;
        int changeSignDet = 0;
        Matrix diag = *this;
        Matrix inverted(1);
        for (uint i = 0; i < N; ++i) { //choose stolbik
            //diag.print();
            //std::cout << "!!!\n";
            bool findNotNullElement = true;
            if (currentHeight < M && !diag.matrix_[currentHeight][i]) {
                findNotNullElement = false;
                for (uint j = currentHeight + 1; j < M; ++j) {
                    if (diag.matrix_[j][i]) {
                        std::swap(diag.matrix_[currentHeight], diag.matrix_[j]);
                        changeSignDet ^= (j - currentHeight) % 2;
                        if (needInvert) {
                            std::swap(inverted.matrix_[currentHeight], inverted.matrix_[j]); 
                        }
                        findNotNullElement = true;
                        break;
                    }
                }
            }
            if (!findNotNullElement) {
                continue;
            }
            for (uint j = 0; j < M; ++j) {
                if (j != currentHeight) {
                    Field coeff = diag.matrix_[j][i] / diag.matrix_[currentHeight][i];
                    //matrix_[j] -= matrix_[currentHeight] * coeff
                    for (uint t = 0; t < N; ++t) {
                        diag.matrix_[j][t] -= diag.matrix_[currentHeight][t] * coeff;
                    }
                    if (needInvert) {
                        for (uint t = 0; t < N; ++t) {
                            inverted.matrix_[j][t] -= inverted.matrix_[currentHeight][t] * coeff;
                        }
                    }
                }
            }
            ++currentHeight;
        }

        if (changeSignDet) {
            for (uint t = 0; t < N; ++t) {
                diag.matrix_[currentHeight - 1][t] *= -1;
            }
            if (needInvert) {
                for (uint t = 0; t < N; ++t) {
                    inverted.matrix_[currentHeight - 1][t] *= -1;
                }
            }
        }
        return std::make_pair(diag, inverted);
    }
public:
    static const unsigned WIDTH = N;
    static const unsigned HEIGHT = M;

    Matrix() {}
    Matrix(const std::vector <std::vector<Field> >& a) {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                matrix_[i][j] = a[i][j];
            }
        }
    }
    Matrix(const std::vector<std::vector<int> >& a) {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                matrix_[i][j] = Field(a[i][j]);
            }
        }
    }
    Matrix(const Matrix& a) {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                matrix_[i][j] = a.matrix_[i][j];
            }
        }
    }
    explicit Matrix(const Field& a) {
        for (uint i = 0; i < N && i < M; ++i) {
            matrix_[i][i] = a;
        }
    }
    //fill matrix from matrix less size
    template<unsigned H, unsigned W>
    Matrix(const Matrix<H, W, Field>& a, unsigned up, unsigned left) {
        for (uint i = 0; i < M && i + up < H; ++i) {
            for (uint j = 0; j < N && j + left < W; ++j) {
                matrix_[i][j] = a[i + up][j + left];
            }
        }
    }
    Matrix& operator=(const Matrix& a) {
        if (this != &a) {
            for (uint i = 0; i < M; ++i) {
                for (uint j = 0; j < N; ++j) {
                    matrix_[i][j] = a.matrix_[i][j];
                }
            }
        }
        return *this;
    }
    const Field trace() const {
        static_assert(N == M, "try get trace from not square matrix");
        Field res = 0;
        for (uint i = 0; i < N; ++i) {
            res += matrix_[i][i];
        }
        return res;
    }

    Matrix& operator+=(const Matrix& a) {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                matrix_[i][j] += a.matrix_[i][j];
            }
        }
        return *this;
    }
    Matrix& operator-=(const Matrix& a) {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                matrix_[i][j] -= a.matrix_[i][j];
            }
        }
        return *this;
    }
    Matrix& operator*=(const Matrix& a) {
        static_assert(M == N, "matrix are not square in *=");
        return *this = *this * a;
    }
    Matrix& operator*=(const Field& k) {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                matrix_[i][j] *= k;
            }
        }
        return *this;
    }
    const Matrix<N, M, Field> transposed() const {
        Matrix<N, M, Field> res;
        for (uint i = 0; i < N; ++i) {
            for (uint j = 0; j < M; ++j) {
                res[i][j] = matrix_[j][i];
            }
        }
        return res;
    }
    uint rank() const {
        Matrix diag = gaussMethod(false).first;
        unsigned currentHeight = 0;
        for (uint i = 0; i < N; ++i) {
            if (diag[currentHeight][i]) {
                ++currentHeight;
            }
        }
        return currentHeight;
    }
    const Field det() const {
        static_assert(M == N, "try get det from not square matrix");
        Matrix diag = gaussMethod(false).first;
        unsigned currentHeight = 0;
        Field res = 1;
        for (uint i = 0; i < N; ++i) {
            if (currentHeight < M && diag[currentHeight][i]) {
                res *= diag[currentHeight][i];
                ++currentHeight;
            }
        }
        return res;
    }
    const Matrix inverted() const {
        static_assert(M == N, "try get inverted from not square matrix");
        std::pair<Matrix, Matrix> gaussMatrix = gaussMethod(true);
        Matrix& diag = gaussMatrix.first;
        Matrix& inverted = gaussMatrix.second;
        unsigned currentHeight = 0;
        for (uint i = 0; i < N; ++i) {
            if (currentHeight < M && diag[currentHeight][i]) {
                for (uint t = 0; t < N; ++t) {
                    inverted.matrix_[currentHeight][t] /= diag[currentHeight][i];
                }
                ++currentHeight;
            }
        }
        return inverted;
    }
    void invert() {
        static_assert(M == N, "try invert not square matrix");
        *this = inverted();
    }
    const std::vector<Field> getRow(unsigned i) const {
        std::vector<Field> res;
        for (uint j = 0; j < N; ++j) {
            res.push_back(matrix_[i][j]);
        }
        return res;
    }
    const std::vector<Field> getColumn(unsigned j) const {
        std::vector<Field> res;
        for (uint i = 0; i < M; ++i) {
            res.push_back(matrix_[i][j]);
        }
        return res;
    }
    const RowMatrix<const Field> operator[](unsigned i) const {
        return RowMatrix<const Field>(matrix_[i]);
    }
    RowMatrix<Field> operator[](unsigned i) {
        return RowMatrix<Field>(matrix_[i]);
    }
    void print() const {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                std::cout << matrix_[i][j].toString() << " ";
            }
            std::cout << std::endl;
        }
    }
    template <unsigned H, unsigned W>
    void fillFromMatrix(const Matrix<H, W, Field>& a, unsigned up, unsigned left) {
        for (uint i = 0; i < H; ++i) {
            for (uint j = 0; j < W; ++j) {
                matrix_[i + up][j + left] = a[i][j];
            }
        }
    }
    template <unsigned N, class Field = Rational>
    friend class SquareMatrix;
};

template <unsigned M, unsigned N, typename Field = Rational>
const Matrix<M, N, Field> operator+(const Matrix<M, N, Field>& a, const Matrix<M, N, Field>& b) {
    Matrix<M, N, Field> tmp = a;
    tmp += b;
    return tmp;
}
template <unsigned M, unsigned N, typename Field = Rational>
const Matrix<M, N, Field> operator-(const Matrix<M, N, Field>& a, const Matrix<M, N, Field>& b) {
    Matrix<M, N, Field> tmp = a;
    tmp -= b;
    return tmp;
}
template <unsigned M, unsigned N, typename Field = Rational>
const Matrix<M, N, Field> operator*(const Matrix<M, N, Field>& a, const Field& k) {
    Matrix<M, N, Field> tmp = a;
    tmp *= k;
    return tmp;
}
#include <algorithm>
template <unsigned M, unsigned N, unsigned K, typename Field = Rational>
const Matrix<M, K, Field> operator*(const Matrix<M, N, Field>& a, const Matrix<N, K, Field>& b) {
    //naive algorithm
    if (M <= 1 || N <= 1 || K <= 1) {
        Matrix<M, K, Field> res;
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < K; ++j) {
                for (uint t = 0; t < N; ++t) {
                    res[i][j] += a[i][t] * b[t][j];
                }
            }
        }
        return res;
    }
    //make matrix with n, m, k : 2; Supplemented
    const unsigned nUp = N + N % 2;
    const unsigned mUp = M + M % 2;
    const unsigned kUp = K + K % 2;
    Matrix<mUp, nUp, Field> aSupplemented(a, 0, 0);
    Matrix<nUp, kUp, Field> bSupplemented(b, 0, 0);

    //Cut into pieces of the matrix
    const unsigned nSmall = nUp / 2;
    const unsigned mSmall = mUp / 2;
    const unsigned kSmall = kUp / 2;
    Matrix<mSmall, nSmall, Field> a11(aSupplemented, 0, 0);
    Matrix<mSmall, nSmall, Field> a12(aSupplemented, 0, nUp / 2);
    Matrix<mSmall, nSmall, Field> a21(aSupplemented, mUp / 2, 0);
    Matrix<mSmall, nSmall, Field> a22(aSupplemented, mUp / 2, nUp / 2);

    Matrix<nSmall, kSmall, Field> b11(bSupplemented, 0, 0);
    Matrix<nSmall, kSmall, Field> b12(bSupplemented, 0, kUp / 2);
    Matrix<nSmall, kSmall, Field> b21(bSupplemented, nUp / 2, 0);
    Matrix<nSmall, kSmall, Field> b22(bSupplemented, nUp / 2, kUp / 2);

    //Strassen algorithm
    Matrix<mSmall, kSmall, Field> P1 = a11 * (b21 - b22);
    Matrix<mSmall, kSmall, Field> P2 = (a11 + a12) * b22;
    Matrix<mSmall, kSmall, Field> P3 = (a21 + a22) * b11;
    Matrix<mSmall, kSmall, Field> P4 = a22 * (b21 - b11);
    Matrix<mSmall, kSmall, Field> P5 = (a11 + a22) * (b11 + b22);
    Matrix<mSmall, kSmall, Field> P6 = (a12 - a22) * (b21 + b22);
    Matrix<mSmall, kSmall, Field> P7 = (a11 - a21) * (b11 + b12);

    Matrix<mUp, kUp, Field> res;
    res.fillFromMatrix(P5 + P4 - P2 + P6, 0, 0);
    res.fillFromMatrix(P1 + P2, 0, kSmall);
    res.fillFromMatrix(P3 + P4, mSmall, 0);
    res.fillFromMatrix(P1 + P5 - P3 - P7, mSmall, kSmall);

    Matrix<M, K, Field> ans(res, 0, 0);
    return ans;
}

template <unsigned N, typename Field = Rational>
class SquareMatrix : public Matrix<N, N, Field> {
public:
    SquareMatrix(const Matrix<N, N, Field>& a) : Matrix(a) {}
    SquareMatrix() {
        for (uint i = 0; i < N; ++i) {
            for (uint j = 0; j < N; ++j) {
                matrix_[i][j] = i == j ? Field(1) : Field(0);
            }
        }
    }
    SquareMatrix(const std::vector<std::vector<Rational> >& a)
            :Matrix(a) {}
    SquareMatrix(const std::vector<std::vector<int> >& a)
            :Matrix(a) {}

};