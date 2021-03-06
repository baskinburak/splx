#ifndef SPLX_QPOPERATIONS_HPP
#define SPLX_QPOPERATIONS_HPP
#include <qp_wrappers/problem.hpp>
#include <splx/curve/ParametricCurve.hpp>
#include <absl/strings/str_cat.h>
#include <splx/types.hpp>
#include <memory>

namespace splx {

template<typename T, unsigned int DIM>
class QPOperations {
public:
    using Row = splx::Row<T>;
    using Matrix = splx::Matrix<T>;
    using VectorDIM = splx::VectorDIM<T, DIM>;
    using Vector = splx::Vector<T>;
    using Index = splx::Index;
    using _ParametricCurve = ParametricCurve<T, DIM>;
    using ControlPoints = typename _ParametricCurve::ControlPoints;
    using AlignedBox = splx::AlignedBox<T, DIM>;
    using Hyperplane = splx::Hyperplane<T, DIM>;
    using Constraint = splx::Constraint<T>;


    QPOperations(Index dvar_count, T a) : m_ndecisionvars(dvar_count), m_a(a) {
        this->maxParameterCheck(a);
    }

    virtual ~QPOperations() {

    }

    /**
    * the row that must be multiplied with the decision variables to get the
    * dimension d of the k^th derivative of the curve at parameter u
    */
    virtual Row evalBasisRow(unsigned int d, T u, unsigned int k) const = 0;

    /**
    * returns Q and c that adds the cost
    * lambda * \int_{0}^{m_a} ||df^k(u)/du^k||_2^2 du
    * as \frac{1}{2} p^T Q p + p^Tc where p denotes the decision variables
    */
    virtual std::pair<Matrix, Vector> integratedSquaredDerivativeCost(
        unsigned int k, T lambda) const = 0;

    /**
    *   returns Q and c that adds the cost lambda * ||f^k(u)-target||^2
    *   as \frac{1}{2} p^T Q p + p^Tc where p denotes the decision variables
    */
    virtual std::pair<Matrix, Vector> evalCost(T u, unsigned int k, 
            const VectorDIM& target, T lambda) const = 0;

    /**
     *   returns set of constraints that enforces f^k(u) = target
     *   each returned constraint it a 3 tuple where
     *   1st element is row that must be dot producted
     *   with decision variables, 2nd variable is the
     *   lower bound the third variable is the upper bound
     *
     *   soft convertible variables sets whether the constraint
     *   can be converted to a soft constraint if desired
    */
    virtual std::vector<Constraint> evalConstraint(
                    T u, unsigned int k, const VectorDIM& target,
                    bool soft_convertible = false,
                    T soft_convertible_weight = T(1)) const = 0;

    /*
     * given hyperplane hp(x) = a_1x_1+a_2x_2+...+a_nx_n + d = 0
     * such that n=(a_1, ..., a_n) is the normal,
     * returns set of constraints that enforces hp(f(u)) <= 0 for all u
     *
     *   soft convertible variables sets whether the constraint
     *   can be converted to a soft constraint if desired
    */
    virtual std::vector<Constraint> hyperplaneConstraintAll(
                    const Hyperplane& hp,
                    bool soft_convertible = false,
                    T soft_convertible_weight = T(1)) const = 0;

    /*
     * given hyperplane hp(x) = a_1x_1+a_2x_2+...+a_nx_n + d = 0
     * such that n=(a_1, ..., a_n) is the normal,
     * returns set of constraints that enforces hp(f(u)) <= 0 at u
     *
     *   soft convertible variables sets whether the constraint
     *   can be converted to a soft constraint if desired
    */
    virtual std::vector<Constraint> hyperplaneConstraintAt(
                T u, const Hyperplane& hp,
                bool soft_convertible = false,
                T soft_convertible_weight = T(1)) const = 0;

    /*
    * returns lbx and ubx such that when lbx <= p <= ubx,
    * than f(u) is in the given bounding box
    */
    virtual std::pair<Vector, Vector> boundingBoxConstraint(
            const AlignedBox& bbox) const = 0;

    /*
    * Given solution vector convert it to the corresponding parametric curve
    */
    virtual std::shared_ptr<_ParametricCurve> extractCurve(
                const Vector& soln) const = 0;

    /*
     * Get decision variables for the curve that is equivalent to the segment
     * from 'from' to 'to'.
     */
    virtual Vector getDVarsForSegment(
            const VectorDIM& from, const VectorDIM& to) const = 0;

    Index numDecisionVariables() const { return m_ndecisionvars; }
    void numDecisionVariables(Index ndvar) { m_ndecisionvars = ndvar; }

    T maxParameter() const { return m_a; }
    void maxParameter(T a) { 
        this->maxParameterCheck(a);
        m_a = a; 
    }

protected:
    void maxParameterCheck(T maxParam) const {
        if(maxParam < 0) {
            throw std::domain_error(
                absl::StrCat(
                    "max parameter can't be negative. given: ", 
                    maxParam
                )
            );
        }
    }

    void parameterCheck(T param) const {
        if(param > this->maxParameter()) {
            throw std::domain_error(
                absl::StrCat(
                    "param can't be more than maxParam. given param: ", 
                    param,
                    ", max param: ",
                    this->maxParameter()
                )
            );
        } else if(param < 0) {
            throw std::domain_error(
                absl::StrCat(
                    "param can't be negative. given param: ",
                    param
                )
            );
        }
    }

    void dimensionIndexCheck(unsigned int d) const {
        if(d >= DIM) {
            throw std::domain_error(
                absl::StrCat(
                    "dimension index must be less than dimension. dimension: ",
                    DIM,
                    " given dimension index: ",
                    d
                )
            );
        }
    }

private:
    Index m_ndecisionvars;
    T m_a;
};


} // end namespace splx


#endif // SPLX_QPOPERATIONS_HPP