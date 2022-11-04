#ifndef A2D_FE_BASIS_H
#define A2D_FE_BASIS_H

#include "a2dmatops2d.h"
#include "a2dmatops3d.h"
#include "a2dobjs.h"
#include "multiphysics/fespace.h"

namespace A2D {

/*
  The Basis class type is a class with all const static member data and
  static member functions.

  The basis classes provide:

  1. static const ndof: The number of degrees of freedom for the basis function.

  2. static const ncomp: The number of components in the function space output.

  3. static function interp: This function evaluates a function space object at
  a quadrature point. This function computes both values and derivatives that
  may be used in the weak form of the governing equations. This can be written
  as

  u = N(pt) * dof

  where u may be the solution as well as the derivatives of the solution, the
  divergence or curl. Here pt is a quadrature point in the reference element
  space.

  4 .static function add: This function adds the residual contribution from the
  corresponding function space object to the degrees of freedom. This can be
  written as

  dof += N(pt)^T * u

  where the dimension of u and N are the same as above.

  5. stride: Indicates that the basis functions are arranged in a special
  structure.

  6. basis: This function provides the full interpolation matrix that takes the
  degrees of freedom and computes the solution. To save space, the structure of
  the matrix may be leveraged for computational efficiency. For instance when
  the static member stride = 2, then the full interpolation matrix N is

  dof = [ u1    v1    u2     v2    u3    v3    u4    v4  ]

  u   = [ N1    0     N2     0     N3    0     N4    0   ]
  u,x = [ N1,x  0     N2,x   0     N3,x  0     N4,x  0   ]
  u,y = [ N1,y  0     N2,y   0     N3,y  0     N4,y  0   ]
  u,z = [ N1,z  0     N2,z   0     N3,y  0     N4,z  0   ]
  u   = [ 0     N1    0      N2   0      N3    0     N4  ]
  u,x = [ 0     N1,x  0      N2,x 0      N3,x  0     N4,x]
  u,y = [ 0     N1,y  0      N2,y 0      N3,y  0     N4,y]
  u,z = [ 0     N1,z  0      N2,z 0      N3,y  0     N4,z]

  Note this may be stored as a 4 x 4 matrix, rather than an 8 x 8 matrix.

  In this case, a call to the function basis returns:

  N =
  [ N1,  N1,x  N1,y  N1,z
    N2,  N2,x  N2,y  N2,z
    N3,  N3,x  N3,y  N3,z
    N4,  N4,x  N4,y  N4,z ]

  Note that the matrix is in column-major order, not row major order!
  This enables more efficient memory access.

  For an H(div) space in 2D, this would look something like this:

  dof = [ dof1   dof2   dof3   dof4   dof5]

  u   = [ N1    N4    N7    N10    N13 ]
  v   = [ N2    N5    N8    N11    N14 ]
  div = [ N3    N6    N9    N12    N15 ]

  where N1 through N15 are each separate functions.

  In this case, stride = 1.

  Note: The entries in the matrix must be consistent in ordering with the
  set_value()/get_value() calls from the function space objects.
*/

/*
  Lagrange basis for a triangle
*/
template <typename T>
class LagrangeTri0Scalar {
 public:
  static const A2D::index_t ndof = 1;
  static const A2D::index_t ncomp = A2D::L2ScalarSpace<T, 2>::ncomp;

  template <class Quadrature, A2D::index_t offset, class SolnType>
  static void interp(A2D::index_t n, const SolnType sol,
                     A2D::L2ScalarSpace<T, 2>& out) {
    T& u = out.get_value();
    u = sol[offset];
  }

  template <class Quadrature, A2D::index_t offset, class SolnType>
  static void add(A2D::index_t n, const A2D::L2ScalarSpace<T, 2>& in,
                  SolnType res) {
    const T& u = in.get_value();
    res[offset] += u;
  }

  // Set the matrix stride
  static const A2D::index_t stride = 1;

  // Set the basis size
  static const A2D::index_t basis_size = 1;

  // Set the derived quantities - number of dof for each stride
  static const A2D::index_t ndof_per_stride = ndof / stride;

  // Number of components per stride
  static const A2D::index_t ncomp_per_stride = ncomp / stride;

  template <class Quadrature, class BasisType>
  static void basis(A2D::index_t n, BasisType N) {
    N[0] = 1.0;
  }
};

/*
  Lagrange basis for a triangle, TODO: finish the implementation
*/
template <typename T, A2D::index_t C>
class LagrangeTri0 {
 public:
  static const A2D::index_t ndof = C;
  static const A2D::index_t ncomp = A2D::L2Space<T, C, 2>::ncomp;

  template <class Quadrature, A2D::index_t offset, class SolnType>
  static void interp(A2D::index_t n, const SolnType sol,
                     A2D::L2Space<T, C, 2>& out) {
    A2D::Vec<T, 2>& u = out.get_value();
    u(0) = sol[offset];
    u(1) = sol[offset + 1];
  }

  template <class Quadrature, A2D::index_t offset, class SolnType>
  static void add(A2D::index_t n, const A2D::L2Space<T, C, 2>& in,
                  SolnType res) {
    const A2D::Vec<T, 2>& u = in.get_value();
    res[offset] += u(0);
    res[offset + 1] += u(1);
  }

  // Set the matrix stride
  static const A2D::index_t stride = 1;

  template <class Quadrature, class BasisType>
  static void basis(A2D::index_t n, BasisType N) {
    N[0] = 1.0;
  }
};

/*
  Lagrange basis for a triangle
*/
template <typename T, A2D::index_t C>
class LagrangeTri1 {
 public:
  static const A2D::index_t ndof = 3 * C;
  static const A2D::index_t ncomp = A2D::H1Space<T, C, 2>::ncomp;

  template <class Quadrature, A2D::index_t offset, class SolnType>
  static void interp(A2D::index_t n, const SolnType sol,
                     A2D::H1Space<T, C, 2>& out) {
    double pt[2];
    Quadrature::get_point(n, pt);

    // Compute the shape functions
    double N[3];
    N[0] = 1.0 - pt[0] - pt[1];
    N[1] = pt[0];
    N[2] = pt[1];

    A2D::Vec<T, C>& u = out.get_value();
    A2D::Mat<T, C, 2>& grad = out.get_grad();

    for (A2D::index_t i = 0; i < C; i++) {
      u(i) = N[0] * sol[i + offset] + N[1] * sol[C + i + offset] +
             N[2] * sol[2 * C + i + offset];
      grad(i, 0) = sol[C + i + offset] - sol[i + offset];
      grad(i, 1) = sol[2 * C + i + offset] - sol[i + offset];
    }
  }

  template <class Quadrature, A2D::index_t offset, class SolnType>
  static void add(A2D::index_t n, const A2D::H1Space<T, C, 2>& in,
                  SolnType res) {
    double pt[2];
    Quadrature::get_point(n, pt);

    // Compute the shape functions
    double N[3];
    N[0] = 1.0 - pt[0] - pt[1];
    N[1] = pt[0];
    N[2] = pt[1];

    const A2D::Vec<T, C>& u = in.get_value();
    const A2D::Mat<T, C, 2>& grad = in.get_grad();

    for (A2D::index_t i = 0; i < C; i++) {
      res[i + offset] += N[0] * u(i) - grad(i, 0) - grad(i, 1);
      res[C + i + offset] += N[1] * u(i) + grad(i, 0);
      res[2 * C + i + offset] += N[2] * u(i) + grad(i, 1);
    }
  }

  // Set the matrix stride
  static const A2D::index_t stride = C;

  // Set the basis size
  static const A2D::index_t basis_size = 9;

  // Set the derived quantities - number of dof for each stride
  static const A2D::index_t ndof_per_stride = ndof / stride;

  // Number of components per stride
  static const A2D::index_t ncomp_per_stride = ncomp / stride;

  // Compute the full matrix of basis functions
  template <class Quadrature, class BasisType>
  static void basis(A2D::index_t n, BasisType N) {
    double pt[2];
    Quadrature::get_point(n, pt);

    N[0] = 1.0 - pt[0] - pt[1];
    N[1] = -1.0;
    N[2] = -1.0;

    N[3] = pt[0];
    N[4] = 1.0;
    N[5] = 0.0;

    N[6] = pt[1];
    N[7] = 0.0;
    N[8] = 1.0;
  }
};

/*
  Raviart-Thomas element for H(div) in 2D
*/
template <typename T>
class RT2DTri1 {
 public:
  static const A2D::index_t ndof = 3;
  static const A2D::index_t ncomp = A2D::Hdiv2DSpace<T>::ncomp;

  template <class Quadrature, A2D::index_t offset, class SolnType>
  static void interp(A2D::index_t n, const SolnType sol,
                     A2D::Hdiv2DSpace<T>& out) {
    double pt[2];
    Quadrature::get_point(n, pt);

    A2D::Vec<T, 2>& u = out.get_value();
    T& div = out.get_div();

    u(0) = pt[0] * sol[offset] + (pt[0] - 1.0) * sol[1 + offset] +
           pt[0] * sol[2 + offset];
    u(1) = pt[1] * sol[offset] + pt[1] * sol[1 + offset] +
           (pt[1] - 1.0) * sol[2 + offset];
    div = 2.0 * (sol[offset] + sol[1 + offset] + sol[2 + offset]);
  }

  template <class Quadrature, A2D::index_t offset, class SolnType>
  static void add(A2D::index_t n, const A2D::Hdiv2DSpace<T>& in, SolnType res) {
    double pt[2];
    Quadrature::get_point(n, pt);

    const A2D::Vec<T, 2>& u = in.get_value();
    const T& div = in.get_div();

    res[offset] += pt[0] * u(0) + pt[1] * u(1) + 2.0 * div;
    res[1 + offset] += (pt[0] - 1.0) * u(0) + pt[1] * u(1) + 2.0 * div;
    res[2 + offset] += pt[0] * u(0) + (pt[1] - 1.0) * u(1) + 2.0 * div;
  }

  // Set the matrix stride
  static const A2D::index_t stride = 1;

  // Set the matrix size
  static const A2D::index_t basis_size = 9;

  // Set the derived quantities - number of dof for each stride
  static const A2D::index_t ndof_per_stride = ndof / stride;

  // Number of components per stride
  static const A2D::index_t ncomp_per_stride = ncomp / stride;

  // Compute the full matrix of basis functions
  template <class Quadrature, class BasisType>
  static void basis(A2D::index_t n, BasisType N) {
    double pt[2];
    Quadrature::get_point(n, pt);

    N[0] = pt[0];
    N[1] = pt[1];
    N[2] = 2.0;

    N[3] = (pt[0] - 1.0);
    N[4] = pt[1];
    N[5] = 2.0;

    N[6] = pt[0];
    N[7] = (pt[1] - 1.0);
    N[8] = 2.0;
  }
};

/*
  Template pattern for computing the number of dof in a basis
*/
template <class... Basis>
struct __count_basis_dof;

template <>
struct __count_basis_dof<> {
  static const A2D::index_t ndof = 0;
};

template <class First, class... Remain>
struct __count_basis_dof<First, Remain...> {
  static const A2D::index_t ndof =
      First::ndof + __count_basis_dof<Remain...>::ndof;
};

/*
  Template pattern for computing the total number of components
*/
template <class... Basis>
struct __count_basis_ncomp;

template <>
struct __count_basis_ncomp<> {
  static const A2D::index_t ncomp = 0;
};

template <class First, class... Remain>
struct __count_basis_ncomp<First, Remain...> {
  static const A2D::index_t ncomp =
      First::ncomp + __count_basis_ncomp<Remain...>::ncomp;
};

/*
  Template pattern for computing the size of the interpolation matrices
*/
template <class... Basis>
struct __count_basis_size;

template <>
struct __count_basis_size<> {
  static const A2D::index_t basis_size = 0;
};

template <class First, class... Remain>
struct __count_basis_size<First, Remain...> {
  static const A2D::index_t basis_size =
      First::basis_size + __count_basis_size<Remain...>::basis_size;
};

/*
  The finite element basis class.

  This class stores a collection of basis function objects
*/
template <typename T, class... Basis>
class FEBasis {
 public:
  typedef std::tuple<Basis...> BasisSpace;

  /**
   * @brief Number of basis function objects
   */
  static constexpr A2D::index_t nbasis =
      std::tuple_size<std::tuple<Basis...>>();

  /**
   * @brief Count the total number of components from all the basis functions
   */
  static constexpr A2D::index_t ncomp = __count_basis_ncomp<Basis...>::ncomp;

  /**
   * @brief Count up the total number of degrees of freedom for this set of
   * basis functions
   */
  static constexpr A2D::index_t ndof = __count_basis_dof<Basis...>::ndof;

  /**
   * @brief Count up the total basis size required to store all of the
   * interpolation matrices for this set of basis functions
   */
  static constexpr A2D::index_t basis_size =
      __count_basis_size<Basis...>::basis_size;

  /**
   * @brief Get the number of degrees of freedom associated with the given basis
   */
  template <A2D::index_t index>
  static constexpr A2D::index_t get_ndof() {
    return std::tuple_element<index, BasisSpace>::type::ndof;
  }

  /**
   * @brief Get the cumulative number of degrees of associated with the basis
   * before the given basis
   */
  template <A2D::index_t index>
  static constexpr A2D::index_t get_dof_offset() {
    return get_dof_offset_<0, index, Basis...>();
  }

  /**
   * @brief Get the basis offset
   */
  template <A2D::index_t index>
  static constexpr A2D::index_t get_basis_size_offset() {
    return get_basis_size_offset_<0, index, Basis...>();
  }

  /**
   * @brief Get the component offset for the given basis
   */
  template <A2D::index_t index>
  static constexpr A2D::index_t get_comp_offset() {
    return get_comp_offset_<0, index, Basis...>();
  }

  /**
   * @brief Interpolate using the basis functions at the quadrature point
   *
   * @param pt The interpolation point index
   * @param dof The degree of freedom object
   * @param s The output finite element space object
   */
  template <class Quadrature, class FEDof, class FiniteElementSpace>
  static void interp(A2D::index_t pt, const FEDof& dof, FiniteElementSpace& s) {
    interp_<Quadrature, FEDof, FiniteElementSpace, 0, Basis...>(pt, dof, s);
  }

  /**
   * @brief Add values to the degree of freedom using the interpolation
   * functions
   *
   * @param pt The interpolation point index
   * @param s The finite element space objec
   * @param dof The degree of freedom object that values are added to
   */
  template <class Quadrature, class FiniteElementSpace, class FEDof>
  static void add(A2D::index_t pt, const FiniteElementSpace& s, FEDof& dof) {
    add_<Quadrature, FiniteElementSpace, FEDof, 0, Basis...>(pt, s, dof);
  }

  /**
   * @brief Interpolate with the basis functions. This can be used to check for
   * consistency
   *
   * @param pt The interpolation point index
   * @param dof The degree of freedom object
   * @param s The output finite element space object
   */
  template <class Quadrature, class FEDof, class FiniteElementSpace>
  static void interp_basis(A2D::index_t pt, const FEDof& dof,
                           FiniteElementSpace& s) {
    // Evaluate the basis functions
    double N[basis_size];
    eval_basis<Quadrature, Basis...>(pt, N);

    interp_basis_<FEDof, FiniteElementSpace, 0, Basis...>(N, dof, s);
  }

  /**
   * @brief Add values to the degree of freedom using the interpolation
   * functions
   *
   * @param pt The interpolation point index
   * @param s The finite element space objec
   * @param dof The degree of freedom object that values are added to
   */
  template <class Quadrature, class FiniteElementSpace, class FEDof>
  static void add_basis(A2D::index_t pt, const FiniteElementSpace& s,
                        FEDof& dof) {
    // Evaluate the basis functions
    double N[basis_size];
    eval_basis<Quadrature, Basis...>(pt, N);

    add_basis_<FiniteElementSpace, FEDof, 0, Basis...>(N, s, dof);
  }

  template <class Quadrature, class QMat, class Mat>
  static void add_outer(A2D::index_t pt, const QMat& jac, Mat& mat) {
    // Evaluate the basis functions
    double N[basis_size];
    eval_basis<Quadrature, Basis...>(pt, N);

    // Add the outer-product
    add_outer_<QMat, Mat, 0, Basis...>(N, jac, mat);
  }

 private:
  template <class Quadrature, class FEDof, class FiniteElementSpace,
            A2D::index_t index, class First, class... Remain>
  static void interp_(const A2D::index_t pt, const FEDof& dof,
                      FiniteElementSpace& s) {
    // Interpolate
    First::template interp<Quadrature, get_dof_offset<index>()>(
        pt, dof, s.template get<index>());

    // Do the next solution space, if any...
    interp_<Quadrature, FEDof, FiniteElementSpace, index + 1, Remain...>(
        pt, dof, s);
  }

  template <class Quadrature, class FEDof, class FiniteElementSpace,
            A2D::index_t index>
  static void interp_(const A2D::index_t pt, const FEDof& dof,
                      FiniteElementSpace& s) {}

  template <class Quadrature, class FiniteElementSpace, class FEDof,
            A2D::index_t index, class First, class... Remain>
  static void add_(const A2D::index_t pt, const FiniteElementSpace& s,
                   FEDof& dof) {
    // Add the interpolation
    First::template add<Quadrature, get_dof_offset<index>()>(
        pt, s.template get<index>(), dof);

    // Do the next solution space, if any...
    add_<Quadrature, FiniteElementSpace, FEDof, index + 1, Remain...>(pt, s,
                                                                      dof);
  }

  template <class Quadrature, class FiniteElementSpace, class FEDof,
            A2D::index_t index>
  static void add_(const A2D::index_t pt, const FiniteElementSpace& s,
                   FEDof& dof) {}

  template <class Quadrature, class First, class... Remain>
  static void eval_basis(const A2D::index_t pt, double N[]) {
    First::template basis<Quadrature>(pt, N);

    eval_basis<Quadrature, Remain...>(pt, &N[First::basis_size]);
  }

  template <class Quadrature>
  static void eval_basis(const A2D::index_t pt, double N[]) {}

  // Get the offset recursively
  template <A2D::index_t r, A2D::index_t index, class First, class... Remain>
  static constexpr A2D::index_t get_dof_offset_() {
    if (r == index) {
      return 0;
    }
    return First::ndof + get_dof_offset_<r + 1, index, Remain...>();
  }

  template <A2D::index_t r, A2D::index_t index>
  static constexpr A2D::index_t get_dof_offset_() {
    return 0;
  }

  // Get the component offset recursively
  template <A2D::index_t r, A2D::index_t index, class First, class... Remain>
  static constexpr A2D::index_t get_comp_offset_() {
    if (r == index) {
      return 0;
    }
    return First::ncomp + get_comp_offset_<r + 1, index, Remain...>();
  }

  template <A2D::index_t r, A2D::index_t index>
  static constexpr A2D::index_t get_comp_offset_() {
    return 0;
  }

  // Get the basis size offset recursively
  template <A2D::index_t r, A2D::index_t index, class First, class... Remain>
  static constexpr A2D::index_t get_basis_size_offset_() {
    if (r == index) {
      return 0;
    }
    return First::basis_size +
           get_basis_size_offset_<r + 1, index, Remain...>();
  }

  template <A2D::index_t r, A2D::index_t index>
  static constexpr A2D::index_t get_basis_size_offset_() {
    return 0;
  }

  // Interpolate with the basis functions evaluated
  template <class FEDof, class FiniteElementSpace, A2D::index_t index,
            class First, class... Remain>
  static void interp_basis_(const double N[], const FEDof& dof,
                            FiniteElementSpace& s) {
    T value[First::ncomp];
    for (A2D::index_t icomp = 0; icomp < First::ncomp; icomp++) {
      value[icomp] = 0.0;
    }

    A2D::index_t idof = get_dof_offset<index>();
    for (A2D::index_t idx = 0; idx < First::ndof_per_stride; idx++) {
      T* v = value;
      for (A2D::index_t istride = 0; istride < First::stride;
           istride++, idof++) {
        for (A2D::index_t icomp = 0; icomp < First::ncomp_per_stride; icomp++) {
          v[0] += N[icomp] * dof[idof];
          v++;
        }
      }
      N += First::ncomp_per_stride;
    }

    for (A2D::index_t icomp = 0; icomp < First::ncomp; icomp++) {
      s[get_comp_offset<index>() + icomp] = value[icomp];
    }

    interp_basis_<FEDof, FiniteElementSpace, index + 1, Remain...>(N, dof, s);
  }

  template <class FEDof, class FiniteElementSpace, A2D::index_t index>
  static void interp_basis_(const double N[], const FEDof& dof,
                            FiniteElementSpace& s) {}

  // Interpolate with the basis functions evaluated
  template <class FiniteElementSpace, class FEDof, A2D::index_t index,
            class First, class... Remain>
  static void add_basis_(const double N[], const FiniteElementSpace& s,
                         FEDof& dof) {
    T values[First::ncomp];
    for (A2D::index_t icomp = 0; icomp < First::ncomp; icomp++) {
      values[icomp] = s[get_comp_offset<index>() + icomp];
    }

    A2D::index_t idof = get_dof_offset<index>();
    for (A2D::index_t idx = 0; idx < First::ndof_per_stride; idx++) {
      T* v = values;
      for (A2D::index_t istride = 0; istride < First::stride;
           istride++, idof++) {
        for (A2D::index_t icomp = 0; icomp < First::ncomp_per_stride; icomp++) {
          dof[idof] += N[icomp] * v[0];
          v++;
        }
      }
      N += First::ncomp_per_stride;
    }

    add_basis_<FiniteElementSpace, FEDof, index + 1, Remain...>(N, s, dof);
  }

  template <class FiniteElementSpace, class FEDof, A2D::index_t index>
  static void add_basis_(const double N[], const FiniteElementSpace& s,
                         FEDof& dof) {}

  template <class QMat, class Mat, A2D::index_t index, class First,
            class... Remain>
  static void add_outer_(const double N0[], const QMat& jac, Mat& mat) {
    const double* N = &N0[get_basis_size_offset<index>()];

    A2D::index_t idof = get_dof_offset<index>();
    for (A2D::index_t idx = 0; idx < First::ndof_per_stride; idx++) {
      for (A2D::index_t istride = 0; istride < First::stride;
           istride++, idof++) {
        // Compute the values in a row vector values = N_{i}^{T} * jac
        T values[ncomp];
        for (A2D::index_t jcomp = 0; jcomp < ncomp; jcomp++) {
          values[jcomp] = 0.0;

          const A2D::index_t offset =
              istride * First::ncomp_per_stride + get_comp_offset<index>();
          for (A2D::index_t icomp = 0; icomp < First::ncomp_per_stride;
               icomp++) {
            values[jcomp] += jac(icomp + offset, jcomp) * N[icomp];
          }
        }

        // Make the call to complete the product with the row
        add_outer_row_<Mat, 0, Basis...>(idof, N0, values, mat);
      }

      N += First::ncomp_per_stride;
    }

    add_outer_<QMat, Mat, index + 1, Remain...>(N0, jac, mat);
  }

  template <class QMat, class Mat, A2D::index_t index>
  static void add_outer_(const double N0[], const QMat& jac, Mat& mat) {}

  template <class Mat, A2D::index_t index, class First, class... Remain>
  static void add_outer_row_(const A2D::index_t idof, const double N[],
                             const T values[], Mat& mat) {
    A2D::index_t jdof = get_dof_offset<index>();
    for (A2D::index_t idx = 0; idx < First::ndof_per_stride; idx++) {
      const T* v = &values[get_comp_offset<index>()];

      for (A2D::index_t jstride = 0; jstride < First::stride;
           jstride++, jdof++) {
        T val = 0.0;
        for (A2D::index_t jcomp = 0; jcomp < First::ncomp_per_stride; jcomp++) {
          val += N[jcomp] * v[0];
          v++;
        }

        mat(idof, jdof) += val;
      }

      N += First::ncomp_per_stride;
    }

    add_outer_row_<Mat, index + 1, Remain...>(idof, N, values, mat);
  }

  template <class Mat, A2D::index_t index>
  static void add_outer_row_(const A2D::index_t idof, const T values[],
                             const double N[], Mat& mat) {}
};

}  // namespace A2D

#endif  // A2D_FE_BASIS_H