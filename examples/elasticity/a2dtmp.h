/*
  0: (0, 0)  1: (0, 1)  2: (0, 2)
  3: (1, 0)  4: (1, 1)  5: (1, 2)
  6: (2, 0)  7: (2, 1)  8: (2, 2)
*/

template<typename T, int M, int N>
class Mat {
public:
  Mat(){}
  Mat( const T* vals ){
    for ( int i = 0; i < M * N; i++ ){
      A[i] = vals[i];
    }
  }
  template<class MatType>
  Mat( const MatType& mat ){
    for ( int i = 0; i < M; i++ ){
      for ( int j = 0; j < N; j++ ){
        A[M*i + j] = mat(i, j);
      }
    }
  }

  ///// Will this work?
  template<class MatType, class... IdxType>
  Mat( const MatType& mat, IdxType... idx ){
    for ( int i = 0; i < M; i++ ){
      for ( int j = 0; j < N; j++ ){
        A[M*i + j] = mat(idx..., i, j);
      }
    }
  }

  template<class IdxType>
  T& operator()( const IdxType i, const IdxType j ){
    return A[M*i + j];
  }

  template<class IdxType>
  const T& operator()( const IdxType i, const IdxType j ) const {
    return A[M*i + j];
  }

  T A[M * N];
};

template<typename T, int N>
class SymmMat {
public:
  static const int MAT_SIZE = (N * (N + 1))/2;

  SymmMat(){}
  SymmMat( const T* vals ){
    for ( int i = 0; i < MAT_SIZE; i++ ){
      A[i] = vals[i];
    }
  }

  template<class IdxType>
  T& operator()( const IdxType i, const IdxType j ){
    if (i >= j){
      return A[j + i*(i + 1)/2];
    }
    else {
      return A[i + j*(j + 1)/2];
    }
  }

  template<class IdxType>
  const T& operator()( const IdxType i, const IdxType j ) const {
    if (i >= j){
      return A[j + i*(i + 1)/2];
    }
    else {
      return A[i + j*(j + 1)/2];
    }
  }

  T A[MAT_SIZE];
};

template<class AMatType, class BMatType, class CMatType>
inline void Symm3x3SymmMultCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) = (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) = (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) = (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) = (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) = (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) = (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) = (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) = (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void Symm3x3SymmMultScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = scale * (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) = scale * (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) = scale * (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) = scale * (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) = scale * (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) = scale * (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) = scale * (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) = scale * (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) = scale * (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Symm3x3SymmMultAddCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) += (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) += (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) += (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) += (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) += (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) += (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) += (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) += (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Symm3x3SymmMultSubCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) -= (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) -= (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) -= (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) -= (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) -= (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) -= (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) -= (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) -= (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) -= (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void Symm3x3SymmMultAddScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += scale * (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) += scale * (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) += scale * (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) += scale * (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) += scale * (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) += scale * (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) += scale * (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) += scale * (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) += scale * (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Symm3x3MatMultCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) = (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) = (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) = (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) = (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) = (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) = (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) = (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) = (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void Symm3x3MatMultScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = scale * (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) = scale * (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) = scale * (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) = scale * (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) = scale * (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) = scale * (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) = scale * (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) = scale * (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) = scale * (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Symm3x3MatMultAddCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) += (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) += (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) += (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) += (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) += (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) += (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) += (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) += (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Symm3x3MatMultSubCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) -= (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) -= (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) -= (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) -= (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) -= (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) -= (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) -= (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) -= (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) -= (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void Symm3x3MatMultAddScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += scale * (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) += scale * (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) += scale * (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) += scale * (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) += scale * (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) += scale * (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) += scale * (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) += scale * (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) += scale * (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Symm3x3MatTransMultCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = (A(0, 0) * B(0, 0) + A(0, 1) * B(0, 1) + A(0, 2) * B(0, 2));
  C(0, 1) = (A(0, 0) * B(1, 0) + A(0, 1) * B(1, 1) + A(0, 2) * B(1, 2));
  C(0, 2) = (A(0, 0) * B(2, 0) + A(0, 1) * B(2, 1) + A(0, 2) * B(2, 2));
  C(1, 0) = (A(1, 0) * B(0, 0) + A(1, 1) * B(0, 1) + A(1, 2) * B(0, 2));
  C(1, 1) = (A(1, 0) * B(1, 0) + A(1, 1) * B(1, 1) + A(1, 2) * B(1, 2));
  C(1, 2) = (A(1, 0) * B(2, 0) + A(1, 1) * B(2, 1) + A(1, 2) * B(2, 2));
  C(2, 0) = (A(2, 0) * B(0, 0) + A(2, 1) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) = (A(2, 0) * B(1, 0) + A(2, 1) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) = (A(2, 0) * B(2, 0) + A(2, 1) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void Symm3x3MatTransMultScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = scale * (A(0, 0) * B(0, 0) + A(0, 1) * B(0, 1) + A(0, 2) * B(0, 2));
  C(0, 1) = scale * (A(0, 0) * B(1, 0) + A(0, 1) * B(1, 1) + A(0, 2) * B(1, 2));
  C(0, 2) = scale * (A(0, 0) * B(2, 0) + A(0, 1) * B(2, 1) + A(0, 2) * B(2, 2));
  C(1, 0) = scale * (A(1, 0) * B(0, 0) + A(1, 1) * B(0, 1) + A(1, 2) * B(0, 2));
  C(1, 1) = scale * (A(1, 0) * B(1, 0) + A(1, 1) * B(1, 1) + A(1, 2) * B(1, 2));
  C(1, 2) = scale * (A(1, 0) * B(2, 0) + A(1, 1) * B(2, 1) + A(1, 2) * B(2, 2));
  C(2, 0) = scale * (A(2, 0) * B(0, 0) + A(2, 1) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) = scale * (A(2, 0) * B(1, 0) + A(2, 1) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) = scale * (A(2, 0) * B(2, 0) + A(2, 1) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Symm3x3MatTransMultAddCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += (A(0, 0) * B(0, 0) + A(0, 1) * B(0, 1) + A(0, 2) * B(0, 2));
  C(0, 1) += (A(0, 0) * B(1, 0) + A(0, 1) * B(1, 1) + A(0, 2) * B(1, 2));
  C(0, 2) += (A(0, 0) * B(2, 0) + A(0, 1) * B(2, 1) + A(0, 2) * B(2, 2));
  C(1, 0) += (A(1, 0) * B(0, 0) + A(1, 1) * B(0, 1) + A(1, 2) * B(0, 2));
  C(1, 1) += (A(1, 0) * B(1, 0) + A(1, 1) * B(1, 1) + A(1, 2) * B(1, 2));
  C(1, 2) += (A(1, 0) * B(2, 0) + A(1, 1) * B(2, 1) + A(1, 2) * B(2, 2));
  C(2, 0) += (A(2, 0) * B(0, 0) + A(2, 1) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) += (A(2, 0) * B(1, 0) + A(2, 1) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) += (A(2, 0) * B(2, 0) + A(2, 1) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Symm3x3MatTransMultSubCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) -= (A(0, 0) * B(0, 0) + A(0, 1) * B(0, 1) + A(0, 2) * B(0, 2));
  C(0, 1) -= (A(0, 0) * B(1, 0) + A(0, 1) * B(1, 1) + A(0, 2) * B(1, 2));
  C(0, 2) -= (A(0, 0) * B(2, 0) + A(0, 1) * B(2, 1) + A(0, 2) * B(2, 2));
  C(1, 0) -= (A(1, 0) * B(0, 0) + A(1, 1) * B(0, 1) + A(1, 2) * B(0, 2));
  C(1, 1) -= (A(1, 0) * B(1, 0) + A(1, 1) * B(1, 1) + A(1, 2) * B(1, 2));
  C(1, 2) -= (A(1, 0) * B(2, 0) + A(1, 1) * B(2, 1) + A(1, 2) * B(2, 2));
  C(2, 0) -= (A(2, 0) * B(0, 0) + A(2, 1) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) -= (A(2, 0) * B(1, 0) + A(2, 1) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) -= (A(2, 0) * B(2, 0) + A(2, 1) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void Symm3x3MatTransMultAddScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += scale * (A(0, 0) * B(0, 0) + A(0, 1) * B(0, 1) + A(0, 2) * B(0, 2));
  C(0, 1) += scale * (A(0, 0) * B(1, 0) + A(0, 1) * B(1, 1) + A(0, 2) * B(1, 2));
  C(0, 2) += scale * (A(0, 0) * B(2, 0) + A(0, 1) * B(2, 1) + A(0, 2) * B(2, 2));
  C(1, 0) += scale * (A(1, 0) * B(0, 0) + A(1, 1) * B(0, 1) + A(1, 2) * B(0, 2));
  C(1, 1) += scale * (A(1, 0) * B(1, 0) + A(1, 1) * B(1, 1) + A(1, 2) * B(1, 2));
  C(1, 2) += scale * (A(1, 0) * B(2, 0) + A(1, 1) * B(2, 1) + A(1, 2) * B(2, 2));
  C(2, 0) += scale * (A(2, 0) * B(0, 0) + A(2, 1) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) += scale * (A(2, 0) * B(1, 0) + A(2, 1) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) += scale * (A(2, 0) * B(2, 0) + A(2, 1) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Mat3x3SymmMultCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) = (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) = (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) = (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) = (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) = (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) = (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) = (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) = (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void Mat3x3SymmMultScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = scale * (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) = scale * (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) = scale * (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) = scale * (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) = scale * (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) = scale * (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) = scale * (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) = scale * (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) = scale * (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Mat3x3SymmMultAddCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) += (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) += (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) += (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) += (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) += (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) += (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) += (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) += (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Mat3x3SymmMultSubCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) -= (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) -= (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) -= (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) -= (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) -= (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) -= (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) -= (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) -= (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) -= (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void Mat3x3SymmMultAddScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += scale * (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) += scale * (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) += scale * (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) += scale * (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) += scale * (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) += scale * (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) += scale * (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) += scale * (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) += scale * (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3SymmMultCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = (A(0, 0) * B(0, 0) + A(1, 0) * B(1, 0) + A(2, 0) * B(2, 0));
  C(0, 1) = (A(0, 0) * B(0, 1) + A(1, 0) * B(1, 1) + A(2, 0) * B(2, 1));
  C(0, 2) = (A(0, 0) * B(0, 2) + A(1, 0) * B(1, 2) + A(2, 0) * B(2, 2));
  C(1, 0) = (A(0, 1) * B(0, 0) + A(1, 1) * B(1, 0) + A(2, 1) * B(2, 0));
  C(1, 1) = (A(0, 1) * B(0, 1) + A(1, 1) * B(1, 1) + A(2, 1) * B(2, 1));
  C(1, 2) = (A(0, 1) * B(0, 2) + A(1, 1) * B(1, 2) + A(2, 1) * B(2, 2));
  C(2, 0) = (A(0, 2) * B(0, 0) + A(1, 2) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) = (A(0, 2) * B(0, 1) + A(1, 2) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) = (A(0, 2) * B(0, 2) + A(1, 2) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3SymmMultScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = scale * (A(0, 0) * B(0, 0) + A(1, 0) * B(1, 0) + A(2, 0) * B(2, 0));
  C(0, 1) = scale * (A(0, 0) * B(0, 1) + A(1, 0) * B(1, 1) + A(2, 0) * B(2, 1));
  C(0, 2) = scale * (A(0, 0) * B(0, 2) + A(1, 0) * B(1, 2) + A(2, 0) * B(2, 2));
  C(1, 0) = scale * (A(0, 1) * B(0, 0) + A(1, 1) * B(1, 0) + A(2, 1) * B(2, 0));
  C(1, 1) = scale * (A(0, 1) * B(0, 1) + A(1, 1) * B(1, 1) + A(2, 1) * B(2, 1));
  C(1, 2) = scale * (A(0, 1) * B(0, 2) + A(1, 1) * B(1, 2) + A(2, 1) * B(2, 2));
  C(2, 0) = scale * (A(0, 2) * B(0, 0) + A(1, 2) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) = scale * (A(0, 2) * B(0, 1) + A(1, 2) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) = scale * (A(0, 2) * B(0, 2) + A(1, 2) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3SymmMultAddCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += (A(0, 0) * B(0, 0) + A(1, 0) * B(1, 0) + A(2, 0) * B(2, 0));
  C(0, 1) += (A(0, 0) * B(0, 1) + A(1, 0) * B(1, 1) + A(2, 0) * B(2, 1));
  C(0, 2) += (A(0, 0) * B(0, 2) + A(1, 0) * B(1, 2) + A(2, 0) * B(2, 2));
  C(1, 0) += (A(0, 1) * B(0, 0) + A(1, 1) * B(1, 0) + A(2, 1) * B(2, 0));
  C(1, 1) += (A(0, 1) * B(0, 1) + A(1, 1) * B(1, 1) + A(2, 1) * B(2, 1));
  C(1, 2) += (A(0, 1) * B(0, 2) + A(1, 1) * B(1, 2) + A(2, 1) * B(2, 2));
  C(2, 0) += (A(0, 2) * B(0, 0) + A(1, 2) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) += (A(0, 2) * B(0, 1) + A(1, 2) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) += (A(0, 2) * B(0, 2) + A(1, 2) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3SymmMultSubCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) -= (A(0, 0) * B(0, 0) + A(1, 0) * B(1, 0) + A(2, 0) * B(2, 0));
  C(0, 1) -= (A(0, 0) * B(0, 1) + A(1, 0) * B(1, 1) + A(2, 0) * B(2, 1));
  C(0, 2) -= (A(0, 0) * B(0, 2) + A(1, 0) * B(1, 2) + A(2, 0) * B(2, 2));
  C(1, 0) -= (A(0, 1) * B(0, 0) + A(1, 1) * B(1, 0) + A(2, 1) * B(2, 0));
  C(1, 1) -= (A(0, 1) * B(0, 1) + A(1, 1) * B(1, 1) + A(2, 1) * B(2, 1));
  C(1, 2) -= (A(0, 1) * B(0, 2) + A(1, 1) * B(1, 2) + A(2, 1) * B(2, 2));
  C(2, 0) -= (A(0, 2) * B(0, 0) + A(1, 2) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) -= (A(0, 2) * B(0, 1) + A(1, 2) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) -= (A(0, 2) * B(0, 2) + A(1, 2) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3SymmMultAddScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += scale * (A(0, 0) * B(0, 0) + A(1, 0) * B(1, 0) + A(2, 0) * B(2, 0));
  C(0, 1) += scale * (A(0, 0) * B(0, 1) + A(1, 0) * B(1, 1) + A(2, 0) * B(2, 1));
  C(0, 2) += scale * (A(0, 0) * B(0, 2) + A(1, 0) * B(1, 2) + A(2, 0) * B(2, 2));
  C(1, 0) += scale * (A(0, 1) * B(0, 0) + A(1, 1) * B(1, 0) + A(2, 1) * B(2, 0));
  C(1, 1) += scale * (A(0, 1) * B(0, 1) + A(1, 1) * B(1, 1) + A(2, 1) * B(2, 1));
  C(1, 2) += scale * (A(0, 1) * B(0, 2) + A(1, 1) * B(1, 2) + A(2, 1) * B(2, 2));
  C(2, 0) += scale * (A(0, 2) * B(0, 0) + A(1, 2) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) += scale * (A(0, 2) * B(0, 1) + A(1, 2) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) += scale * (A(0, 2) * B(0, 2) + A(1, 2) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Mat3x3MatMultCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) = (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) = (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) = (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) = (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) = (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) = (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) = (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) = (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void Mat3x3MatMultScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = scale * (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) = scale * (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) = scale * (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) = scale * (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) = scale * (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) = scale * (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) = scale * (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) = scale * (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) = scale * (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Mat3x3MatMultAddCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) += (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) += (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) += (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) += (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) += (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) += (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) += (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) += (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Mat3x3MatMultSubCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) -= (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) -= (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) -= (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) -= (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) -= (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) -= (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) -= (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) -= (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) -= (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void Mat3x3MatMultAddScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += scale * (A(0, 0) * B(0, 0) + A(0, 1) * B(1, 0) + A(0, 2) * B(2, 0));
  C(0, 1) += scale * (A(0, 0) * B(0, 1) + A(0, 1) * B(1, 1) + A(0, 2) * B(2, 1));
  C(0, 2) += scale * (A(0, 0) * B(0, 2) + A(0, 1) * B(1, 2) + A(0, 2) * B(2, 2));
  C(1, 0) += scale * (A(1, 0) * B(0, 0) + A(1, 1) * B(1, 0) + A(1, 2) * B(2, 0));
  C(1, 1) += scale * (A(1, 0) * B(0, 1) + A(1, 1) * B(1, 1) + A(1, 2) * B(2, 1));
  C(1, 2) += scale * (A(1, 0) * B(0, 2) + A(1, 1) * B(1, 2) + A(1, 2) * B(2, 2));
  C(2, 0) += scale * (A(2, 0) * B(0, 0) + A(2, 1) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) += scale * (A(2, 0) * B(0, 1) + A(2, 1) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) += scale * (A(2, 0) * B(0, 2) + A(2, 1) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Mat3x3MatTransMultCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = (A(0, 0) * B(0, 0) + A(0, 1) * B(0, 1) + A(0, 2) * B(0, 2));
  C(0, 1) = (A(0, 0) * B(1, 0) + A(0, 1) * B(1, 1) + A(0, 2) * B(1, 2));
  C(0, 2) = (A(0, 0) * B(2, 0) + A(0, 1) * B(2, 1) + A(0, 2) * B(2, 2));
  C(1, 0) = (A(1, 0) * B(0, 0) + A(1, 1) * B(0, 1) + A(1, 2) * B(0, 2));
  C(1, 1) = (A(1, 0) * B(1, 0) + A(1, 1) * B(1, 1) + A(1, 2) * B(1, 2));
  C(1, 2) = (A(1, 0) * B(2, 0) + A(1, 1) * B(2, 1) + A(1, 2) * B(2, 2));
  C(2, 0) = (A(2, 0) * B(0, 0) + A(2, 1) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) = (A(2, 0) * B(1, 0) + A(2, 1) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) = (A(2, 0) * B(2, 0) + A(2, 1) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void Mat3x3MatTransMultScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = scale * (A(0, 0) * B(0, 0) + A(0, 1) * B(0, 1) + A(0, 2) * B(0, 2));
  C(0, 1) = scale * (A(0, 0) * B(1, 0) + A(0, 1) * B(1, 1) + A(0, 2) * B(1, 2));
  C(0, 2) = scale * (A(0, 0) * B(2, 0) + A(0, 1) * B(2, 1) + A(0, 2) * B(2, 2));
  C(1, 0) = scale * (A(1, 0) * B(0, 0) + A(1, 1) * B(0, 1) + A(1, 2) * B(0, 2));
  C(1, 1) = scale * (A(1, 0) * B(1, 0) + A(1, 1) * B(1, 1) + A(1, 2) * B(1, 2));
  C(1, 2) = scale * (A(1, 0) * B(2, 0) + A(1, 1) * B(2, 1) + A(1, 2) * B(2, 2));
  C(2, 0) = scale * (A(2, 0) * B(0, 0) + A(2, 1) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) = scale * (A(2, 0) * B(1, 0) + A(2, 1) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) = scale * (A(2, 0) * B(2, 0) + A(2, 1) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Mat3x3MatTransMultAddCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += (A(0, 0) * B(0, 0) + A(0, 1) * B(0, 1) + A(0, 2) * B(0, 2));
  C(0, 1) += (A(0, 0) * B(1, 0) + A(0, 1) * B(1, 1) + A(0, 2) * B(1, 2));
  C(0, 2) += (A(0, 0) * B(2, 0) + A(0, 1) * B(2, 1) + A(0, 2) * B(2, 2));
  C(1, 0) += (A(1, 0) * B(0, 0) + A(1, 1) * B(0, 1) + A(1, 2) * B(0, 2));
  C(1, 1) += (A(1, 0) * B(1, 0) + A(1, 1) * B(1, 1) + A(1, 2) * B(1, 2));
  C(1, 2) += (A(1, 0) * B(2, 0) + A(1, 1) * B(2, 1) + A(1, 2) * B(2, 2));
  C(2, 0) += (A(2, 0) * B(0, 0) + A(2, 1) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) += (A(2, 0) * B(1, 0) + A(2, 1) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) += (A(2, 0) * B(2, 0) + A(2, 1) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void Mat3x3MatTransMultSubCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) -= (A(0, 0) * B(0, 0) + A(0, 1) * B(0, 1) + A(0, 2) * B(0, 2));
  C(0, 1) -= (A(0, 0) * B(1, 0) + A(0, 1) * B(1, 1) + A(0, 2) * B(1, 2));
  C(0, 2) -= (A(0, 0) * B(2, 0) + A(0, 1) * B(2, 1) + A(0, 2) * B(2, 2));
  C(1, 0) -= (A(1, 0) * B(0, 0) + A(1, 1) * B(0, 1) + A(1, 2) * B(0, 2));
  C(1, 1) -= (A(1, 0) * B(1, 0) + A(1, 1) * B(1, 1) + A(1, 2) * B(1, 2));
  C(1, 2) -= (A(1, 0) * B(2, 0) + A(1, 1) * B(2, 1) + A(1, 2) * B(2, 2));
  C(2, 0) -= (A(2, 0) * B(0, 0) + A(2, 1) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) -= (A(2, 0) * B(1, 0) + A(2, 1) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) -= (A(2, 0) * B(2, 0) + A(2, 1) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void Mat3x3MatTransMultAddScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += scale * (A(0, 0) * B(0, 0) + A(0, 1) * B(0, 1) + A(0, 2) * B(0, 2));
  C(0, 1) += scale * (A(0, 0) * B(1, 0) + A(0, 1) * B(1, 1) + A(0, 2) * B(1, 2));
  C(0, 2) += scale * (A(0, 0) * B(2, 0) + A(0, 1) * B(2, 1) + A(0, 2) * B(2, 2));
  C(1, 0) += scale * (A(1, 0) * B(0, 0) + A(1, 1) * B(0, 1) + A(1, 2) * B(0, 2));
  C(1, 1) += scale * (A(1, 0) * B(1, 0) + A(1, 1) * B(1, 1) + A(1, 2) * B(1, 2));
  C(1, 2) += scale * (A(1, 0) * B(2, 0) + A(1, 1) * B(2, 1) + A(1, 2) * B(2, 2));
  C(2, 0) += scale * (A(2, 0) * B(0, 0) + A(2, 1) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) += scale * (A(2, 0) * B(1, 0) + A(2, 1) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) += scale * (A(2, 0) * B(2, 0) + A(2, 1) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3MatMultCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = (A(0, 0) * B(0, 0) + A(1, 0) * B(1, 0) + A(2, 0) * B(2, 0));
  C(0, 1) = (A(0, 0) * B(0, 1) + A(1, 0) * B(1, 1) + A(2, 0) * B(2, 1));
  C(0, 2) = (A(0, 0) * B(0, 2) + A(1, 0) * B(1, 2) + A(2, 0) * B(2, 2));
  C(1, 0) = (A(0, 1) * B(0, 0) + A(1, 1) * B(1, 0) + A(2, 1) * B(2, 0));
  C(1, 1) = (A(0, 1) * B(0, 1) + A(1, 1) * B(1, 1) + A(2, 1) * B(2, 1));
  C(1, 2) = (A(0, 1) * B(0, 2) + A(1, 1) * B(1, 2) + A(2, 1) * B(2, 2));
  C(2, 0) = (A(0, 2) * B(0, 0) + A(1, 2) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) = (A(0, 2) * B(0, 1) + A(1, 2) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) = (A(0, 2) * B(0, 2) + A(1, 2) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3MatMultScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = scale * (A(0, 0) * B(0, 0) + A(1, 0) * B(1, 0) + A(2, 0) * B(2, 0));
  C(0, 1) = scale * (A(0, 0) * B(0, 1) + A(1, 0) * B(1, 1) + A(2, 0) * B(2, 1));
  C(0, 2) = scale * (A(0, 0) * B(0, 2) + A(1, 0) * B(1, 2) + A(2, 0) * B(2, 2));
  C(1, 0) = scale * (A(0, 1) * B(0, 0) + A(1, 1) * B(1, 0) + A(2, 1) * B(2, 0));
  C(1, 1) = scale * (A(0, 1) * B(0, 1) + A(1, 1) * B(1, 1) + A(2, 1) * B(2, 1));
  C(1, 2) = scale * (A(0, 1) * B(0, 2) + A(1, 1) * B(1, 2) + A(2, 1) * B(2, 2));
  C(2, 0) = scale * (A(0, 2) * B(0, 0) + A(1, 2) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) = scale * (A(0, 2) * B(0, 1) + A(1, 2) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) = scale * (A(0, 2) * B(0, 2) + A(1, 2) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3MatMultAddCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += (A(0, 0) * B(0, 0) + A(1, 0) * B(1, 0) + A(2, 0) * B(2, 0));
  C(0, 1) += (A(0, 0) * B(0, 1) + A(1, 0) * B(1, 1) + A(2, 0) * B(2, 1));
  C(0, 2) += (A(0, 0) * B(0, 2) + A(1, 0) * B(1, 2) + A(2, 0) * B(2, 2));
  C(1, 0) += (A(0, 1) * B(0, 0) + A(1, 1) * B(1, 0) + A(2, 1) * B(2, 0));
  C(1, 1) += (A(0, 1) * B(0, 1) + A(1, 1) * B(1, 1) + A(2, 1) * B(2, 1));
  C(1, 2) += (A(0, 1) * B(0, 2) + A(1, 1) * B(1, 2) + A(2, 1) * B(2, 2));
  C(2, 0) += (A(0, 2) * B(0, 0) + A(1, 2) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) += (A(0, 2) * B(0, 1) + A(1, 2) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) += (A(0, 2) * B(0, 2) + A(1, 2) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3MatMultSubCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) -= (A(0, 0) * B(0, 0) + A(1, 0) * B(1, 0) + A(2, 0) * B(2, 0));
  C(0, 1) -= (A(0, 0) * B(0, 1) + A(1, 0) * B(1, 1) + A(2, 0) * B(2, 1));
  C(0, 2) -= (A(0, 0) * B(0, 2) + A(1, 0) * B(1, 2) + A(2, 0) * B(2, 2));
  C(1, 0) -= (A(0, 1) * B(0, 0) + A(1, 1) * B(1, 0) + A(2, 1) * B(2, 0));
  C(1, 1) -= (A(0, 1) * B(0, 1) + A(1, 1) * B(1, 1) + A(2, 1) * B(2, 1));
  C(1, 2) -= (A(0, 1) * B(0, 2) + A(1, 1) * B(1, 2) + A(2, 1) * B(2, 2));
  C(2, 0) -= (A(0, 2) * B(0, 0) + A(1, 2) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) -= (A(0, 2) * B(0, 1) + A(1, 2) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) -= (A(0, 2) * B(0, 2) + A(1, 2) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3MatMultAddScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += scale * (A(0, 0) * B(0, 0) + A(1, 0) * B(1, 0) + A(2, 0) * B(2, 0));
  C(0, 1) += scale * (A(0, 0) * B(0, 1) + A(1, 0) * B(1, 1) + A(2, 0) * B(2, 1));
  C(0, 2) += scale * (A(0, 0) * B(0, 2) + A(1, 0) * B(1, 2) + A(2, 0) * B(2, 2));
  C(1, 0) += scale * (A(0, 1) * B(0, 0) + A(1, 1) * B(1, 0) + A(2, 1) * B(2, 0));
  C(1, 1) += scale * (A(0, 1) * B(0, 1) + A(1, 1) * B(1, 1) + A(2, 1) * B(2, 1));
  C(1, 2) += scale * (A(0, 1) * B(0, 2) + A(1, 1) * B(1, 2) + A(2, 1) * B(2, 2));
  C(2, 0) += scale * (A(0, 2) * B(0, 0) + A(1, 2) * B(1, 0) + A(2, 2) * B(2, 0));
  C(2, 1) += scale * (A(0, 2) * B(0, 1) + A(1, 2) * B(1, 1) + A(2, 2) * B(2, 1));
  C(2, 2) += scale * (A(0, 2) * B(0, 2) + A(1, 2) * B(1, 2) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3MatTransMultCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = (A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2));
  C(0, 1) = (A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2));
  C(0, 2) = (A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2));
  C(1, 0) = (A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2));
  C(1, 1) = (A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2));
  C(1, 2) = (A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2));
  C(2, 0) = (A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) = (A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) = (A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3MatTransMultScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) = scale * (A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2));
  C(0, 1) = scale * (A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2));
  C(0, 2) = scale * (A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2));
  C(1, 0) = scale * (A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2));
  C(1, 1) = scale * (A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2));
  C(1, 2) = scale * (A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2));
  C(2, 0) = scale * (A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) = scale * (A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) = scale * (A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3MatTransMultAddCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += (A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2));
  C(0, 1) += (A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2));
  C(0, 2) += (A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2));
  C(1, 0) += (A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2));
  C(1, 1) += (A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2));
  C(1, 2) += (A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2));
  C(2, 0) += (A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) += (A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) += (A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3MatTransMultSubCore( const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) -= (A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2));
  C(0, 1) -= (A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2));
  C(0, 2) -= (A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2));
  C(1, 0) -= (A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2));
  C(1, 1) -= (A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2));
  C(1, 2) -= (A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2));
  C(2, 0) -= (A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) -= (A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) -= (A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<typename T, class AMatType, class BMatType, class CMatType>
inline void MatTrans3x3MatTransMultAddScaleCore( T scale, const AMatType& A, const BMatType& B, CMatType& C ){
  C(0, 0) += scale * (A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2));
  C(0, 1) += scale * (A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2));
  C(0, 2) += scale * (A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2));
  C(1, 0) += scale * (A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2));
  C(1, 1) += scale * (A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2));
  C(1, 2) += scale * (A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2));
  C(2, 0) += scale * (A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2));
  C(2, 1) += scale * (A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2));
  C(2, 2) += scale * (A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1) + A(2, 2) * B(2, 2));
}

template<typename T, class MatType>
inline T Mat3x3DetCore( const T A[] ){
  return (A(2, 2) * (A(0, 0) * A(1, 1) - A(1, 0) * A(0, 1)) -
          A(2, 1) * (A(0, 0) * A(1, 2) - A(1, 0) * A(0, 2)) +
          A(2, 0) * (A(0, 1) * A(1, 2) - A(0, 2) * A(1, 1)));
}

template<typename T, class AMatType, class DMatType>
inline T Mat3x3DetDerivForwardCore( const AMatType A,
                                    DMatType& Ad ){
  return (Ad(0, 0) * (A(2, 2) * A(1, 1) - A(2, 1) * A(1, 2)) +
          Ad(0, 1) * (A(2, 0) * A(1, 2) - A(2, 2) * A(1, 0)) +
          Ad(0, 2) * (A(2, 1) * A(1, 0) - A(2, 0) * A(1, 1)) +
          Ad(1, 0) * (A(2, 1) * A(0, 2) - A(2, 2) * A(0, 1)) +
          Ad(1, 1) * (A(2, 2) * A(0, 0) - A(2, 0) * A(0, 2)) +
          Ad(1, 2) * (A(2, 0) * A(0, 1) - A(2, 1) * A(0, 0)) +
          Ad(2, 0) * (A(0, 1) * A(1, 2) - A(0, 2) * A(1, 1)) +
          Ad(2, 1) * (A(1, 0) * A(0, 2) - A(0, 0) * A(1, 2)) +
          Ad(2, 2) * (A(0, 0) * A(1, 1) - A(1, 0) * A(0, 1)));
}

template<typename T, class AMatType, class DMatType>
inline void Mat3x3DetDerivReverseCore( const T detd,
                                       const AMatType& A,
                                       DMatType& Ad ){
  Ad(0, 0) += (A(2, 2) * A(1, 1) - A(2, 1) * A(1, 2)) * detd;
  Ad(0, 1) += (A(2, 0) * A(1, 2) - A(2, 2) * A(1, 0)) * detd;
  Ad(0, 2) += (A(2, 1) * A(1, 0) - A(2, 0) * A(1, 1)) * detd;
  Ad(1, 0) += (A(2, 1) * A(0, 2) - A(2, 2) * A(0, 1)) * detd;
  Ad(1, 1) += (A(2, 2) * A(0, 0) - A(2, 0) * A(0, 2)) * detd;
  Ad(1, 2) += (A(2, 0) * A(0, 1) - A(2, 1) * A(0, 0)) * detd;
  Ad(2, 0) += (A(0, 1) * A(1, 2) - A(0, 2) * A(1, 1)) * detd;
  Ad(2, 1) += (A(1, 0) * A(0, 2) - A(0, 0) * A(1, 2)) * detd;
  Ad(2, 2) += (A(0, 0) * A(1, 1) - A(1, 0) * A(0, 1)) * detd;
}

template<typename T, class AMatType, class AinvMatType>
inline T Mat3x3InverseCore( const AMatType& A, AinvMatType& Ainv ){
  T det = (A(2, 2) * (A(0, 0) * A(1, 1) - A(1, 0) * A(0, 1)) -
           A(2, 1) * (A(0, 0) * A(1, 2) - A(1, 0) * A(0, 2)) +
           A(2, 0) * (A(0, 1) * A(1, 2) - A(0, 2) * A(1, 1)));
  T detinv = 1.0/det;

  Ainv(0, 0) = (A(1, 1) * A(2, 2) - A(1, 2) * A(2, 1)) * detinv;
  Ainv(0, 1) =-(A(0, 1) * A(2, 2) - A(0, 2) * A(2, 1)) * detinv;
  Ainv(0, 2) = (A(0, 1) * A(1, 2) - A(0, 2) * A(1, 1)) * detinv;

  Ainv(1, 0) =-(A(1, 0) * A(2, 2) - A(1, 2) * A(2, 0)) * detinv;
  Ainv(1, 1) = (A(0, 0) * A(2, 2) - A(0, 2) * A(2, 0)) * detinv;
  Ainv(1, 2) =-(A(0, 0) * A(1, 2) - A(0, 2) * A(1, 0)) * detinv;

  Ainv(2, 0) = (A(1, 0) * A(2, 1) - A(1, 1) * A(2, 0)) * detinv;
  Ainv(2, 1) =-(A(0, 0) * A(2, 1) - A(0, 1) * A(2, 0)) * detinv;
  Ainv(2, 2) = (A(0, 0) * A(1, 1) - A(0, 1) * A(1, 0)) * detinv;

  return det;
}

template<typename T, class AinvMatType, class AdMatType, class BdMatType>
inline void Mat3x3InverseDerivForwardCore( const AinvMatType Ainv,
                                           const AdMatType Ad,
                                           BdMatType& Bd ){
  Mat<T, 3, 3> tmp;
  Mat3x3MatMultCore(Ainv, Ad, tmp);
  Mat3x3MatMultScaleCore(T(-1.0), tmp, Ainv, Bd);
}

template<typename T, class AinvMatType, class BdMatType, class AdMatType>
inline void Mat3x3InverseDerivReverseCore( const AinvMatType& Ainv,
                                           const BdMatType& Bd,
                                           AdMatType& Ad ){
  Mat<T, 3, 3> tmp;
  MatTrans3x3MatMultCore(Ainv, Bd, tmp);
  Mat3x3MatTransMultAddScaleCore(T(-1.0), tmp, Ainv, Ad);
}

template<typename T, class SMatType, class RMatType>
inline T Symm3x3MatMultTraceCore( const SMatType& S,
                                  const RMatType& R ){
  return ((S(0, 0) * R(0, 0) + S(1, 1) * R(1, 1) + S(2, 2) * R(2, 2)) +
          2.0 * (S(0, 1) * R(0, 1) + S(0, 2) * R(0, 2) + S(1, 2) * R(1, 2)));
}

template<typename T, class SMatType, class RMatType>
inline void Symm3x3MatMultTraceReverseCore( const T scale,
                                            const SMatType& S,
                                            RMatType& Rd ){
  Rd(0, 0) += scale * S(0, 0);
  Rd(1, 1) += scale * S(1, 1);
  Rd(2, 2) += scale * S(2, 2);
  Rd(0, 1) += 2.0 * scale * S(0, 1);
  Rd(0, 2) += 2.0 * scale * S(0, 2);
  Rd(1, 2) += 2.0 * scale * S(1, 2);
}

template<class UxMatType, class EMatType>
inline void Mat3x3LinearGreenStrainCore( const UxMatType& Ux,
                                         EMatType& E ){
  // E = 0.5*(Ux + Ux^{T});
  E(0, 0) = Ux(0, 0);
  E(1, 1) = Ux(1, 1);
  E(2, 2) = Ux(2, 2);
  E(0, 1) = 0.5*(Ux(0, 1) + Ux(1, 0));
  E(0, 2) = 0.5*(Ux(0, 2) + Ux(2, 0));
  E(1, 2) = 0.5*(Ux(1, 2) + Ux(2, 1));
}

template<class EMatType, class UxMatType>
inline void Mat3x3LinearGreenStrainReverseCore( const EMatType& Ed,
                                                UxMatType& Uxd ){
  Uxd(0, 0) += Ed(0, 0);
  Uxd(0, 1) += Ed(0, 1);
  Uxd(0, 2) += Ed(0, 2);
  Uxd(1, 0) += Ed(0, 1);
  Uxd(1, 1) += Ed(1, 1);
  Uxd(1, 2) += Ed(1, 2);
  Uxd(2, 0) += Ed(0, 2);
  Uxd(2, 1) += Ed(1, 2);
  Uxd(2, 2) += Ed(2, 2);
}

template<class UxMatType, class EMatType>
inline void Mat3x3GreenStrainCore( const UxMatType& Ux,
                                   EMatType& E ){
  // E = 0.5*(Ux + Ux^{T} + Ux^{T} * Ux)
  E(0, 0) = Ux(0, 0) + 0.5*(Ux(0, 0) * Ux(0, 0) + Ux(1, 0) * Ux(1, 0) + Ux(2, 0) * Ux(2, 0));
  E(1, 1) = Ux(1, 1) + 0.5*(Ux(0, 1) * Ux(0, 1) + Ux(1, 1) * Ux(1, 1) + Ux(2, 1) * Ux(2, 1));
  E(2, 2) = Ux(2, 2) + 0.5*(Ux(0, 2) * Ux(0, 2) + Ux(1, 2) * Ux(1, 2) + Ux(2, 2) * Ux(2, 2));
  E(0, 1) = 0.5*(Ux(0, 1) + Ux(1, 0) + Ux(0, 0) * Ux(0, 1) + Ux(1, 0) * Ux(1, 1) + Ux(2, 0) * Ux(2, 1));
  E(0, 2) = 0.5*(Ux(0, 2) + Ux(2, 0) + Ux(0, 0) * Ux(0, 2) + Ux(1, 0) * Ux(1, 2) + Ux(2, 0) * Ux(2, 2));
  E(1, 3) = 0.5*(Ux(1, 2) + Ux(2, 1) + Ux(0, 1) * Ux(0, 2) + Ux(1, 1) * Ux(1, 2) + Ux(2, 1) * Ux(2, 2));
}

template<class UxMatType, class UxdMatType, class EMatType>
inline void Mat3x3GreenStrainForwardCore( const UxMatType& Ux,
                                          const UxdMatType& Uxd,
                                          EMatType& Ed ){
  Ed(0, 0) = Uxd(0, 0) + Ux(0, 0) * Uxd(0, 0) + Ux(1, 0) * Uxd(1, 0) + Ux(2, 0) * Uxd(2, 0);
  Ed(1, 1) = Uxd(1, 1) + Ux(0, 1) * Uxd(0, 1) + Ux(1, 1) * Uxd(1, 1) + Ux(2, 1) * Uxd(2, 1);
  Ed(2, 2) = Uxd(2, 2) + Ux(0, 2) * Uxd(0, 2) + Ux(1, 2) * Uxd(1, 2) + Ux(2, 2) * Uxd(2, 2);

  Ed(0, 1) = 0.5*(Uxd(0, 1) + Uxd(1, 0) +
               Ux(0, 0) * Uxd(0, 1) + Ux(1, 0) * Uxd(1, 1) + Ux(2, 0) * Uxd(2, 1) +
               Uxd(0, 0) * Ux(0, 1) + Uxd(1, 0) * Ux(1, 1) + Uxd(2, 0) * Ux(2, 1));
  Ed(0, 2) = 0.5*(Uxd(0, 2) + Uxd(2, 0) +
               Ux(0, 0) * Uxd(0, 2) + Ux(1, 0) * Uxd(1, 2) + Ux(2, 0) * Uxd(2, 2) +
               Uxd(0, 0) * Ux(0, 2) + Uxd(1, 0) * Ux(1, 2) + Uxd(2, 0) * Ux(2, 2));
  Ed(1, 2) = 0.5*(Uxd(1, 2) + Uxd(2, 1) +
               Ux(0, 1) * Uxd(0, 2) + Ux(1, 1) * Uxd(1, 2) + Ux(2, 1) * Uxd(2, 2) +
               Uxd(0, 1) * Ux(0, 2) + Uxd(1, 1) * Ux(1, 2) + Uxd(2, 1) * Ux(2, 2));
}

template<typename T, class UxMatType, class EdMatType, class UxdMatType>
inline void Mat3x3GreenStrainReverseCore( const UxMatType& Ux,
                                          const EdMatType& Ed,
                                          UxdMatType& Uxd ){
  T ux0 = Ux(0, 0) + 1.0;
  T ux4 = Ux(1, 1) + 1.0;
  T ux8 = Ux(2, 2) + 1.0;
  T e1 = 0.5 * Ed(0, 1);
  T e2 = 0.5 * Ed(0, 2);
  T e4 = 0.5 * Ed(1, 2);

  // Uxd = (I + Ux) * E
  Uxd(0, 0) += ux0 * Ed(0, 0) + Ux(0, 1) * e1 + Ux(0, 2) * e2;
  Uxd(0, 1) += ux0 * e1 + Ux(0, 1) * Ed(1, 1) + Ux(0, 2) * e4;
  Uxd(0, 2) += ux0 * e2 + Ux(0, 1) * e4 + Ux(0, 2) * Ed(2, 2);
  Uxd(1, 0) += Ux(1, 0) * Ed(0, 0) + ux4 * e1 + Ux(1, 2) * e2;
  Uxd(1, 1) += Ux(1, 0) * e1 + ux4 * Ed(1, 1) + Ux(1, 2) * e4;
  Uxd(1, 2) += Ux(1, 0) * e2 + ux4 * e4 + Ux(1, 2) * Ed(2, 2);
  Uxd(2, 0) += Ux(2, 0) * Ed(0, 0) + Ux(2, 1) * e1 + ux8 * e2;
  Uxd(2, 1) += Ux(2, 0) * e1 + Ux(2, 1) * Ed(1, 1) + ux8 * e4;
  Uxd(2, 2) += Ux(2, 0) * e2 + Ux(2, 1) * e4 + ux8 * Ed(2, 2);
}

template<typename T, class EMatType, class SMatType>
inline void Symm3x3IsotropicConstitutiveCore( const T mu,
                                              const T lambda,
                                              const EMatType& E,
                                              SMatType S[] ){
  T tr = lambda * (E(0, 0) + E(1, 1) + E(2, 2));
  T mu2 = 2.0 * mu;
  S(0, 0) = mu2 * E(0, 0) + tr;
  S(0, 1) = mu2 * E(0, 1);
  S(0, 2) = mu2 * E(0, 2);
  S(1, 1) = mu2 * E(1, 1) + tr;
  S(1, 2) = mu2 * E(1, 2);
  S(2, 2) = mu2 * E(2, 2) + tr;
}

template<typename T, class SdMatType, class EdMatType>
inline void Symm3x3IsotropicConstitutiveReverseCore( const T mu,
                                                     const T lambda,
                                                     const SdMatType& Sd,
                                                     EdMatType& Ed ){
  T tr = lambda * (Sd(0, 0) + Sd(1, 1) + Sd(2, 2));
  T mu2 = 2.0 * mu;
  Ed(0, 0) += mu2 * Sd(0, 0) + tr;
  Ed(0, 1) += mu2 * Sd(0, 1);
  Ed(0, 2) += mu2 * Sd(0, 2);
  Ed(1, 1) += mu2 * Sd(1, 1) + tr;
  Ed(1, 2) += mu2 * Sd(1, 2);
  Ed(2, 2) += mu2 * Sd(2, 2) + tr;
}

/*
template<class UxMatType, class ESymmMatType>
class Mat3x3GreenStrain {
public:
  Mat3x3GreenStrain( const UxMatType& Ux, ESymmMatType& E ){
    Mat3x3GreenStrainCore(Ux, E);
  }
};

template<class UxMatType, class ESymmMatType>
class ADMat3x3GreenStrain {
public:
  ADMat3x3GreenStrain( const UxMatType& Ux, ESymmMatType& E ){
    Mat3x3GreenStrainCore(Ux, E);
  }
};
*/