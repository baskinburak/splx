#include "catch.hpp"

#include <splx/opt/BezierQPGenerator.hpp>

#include <stdexcept>
#include <iostream>

#include <Eigen/Dense>

TEST_CASE("BezierQPGenerator.bernsteinCoefficientMatrix test") {
    double double_eq_epsilon = 1e-15;

    splx::BezierQPGenerator<double, 3> generator(8, 0.35914595895879409);

    auto bern = generator.bernsteinCoefficientMatrixTest(1);
    Eigen::Matrix<double, 8, 8> mtr;
    mtr << -19.490682897543422,19.490682897543422,0,0,0,0,0,0,
            325.61718841079289,-651.2343768215859,325.61718841079289,0,0,0,0,0,
            -2266.6076304658627,6799.822891397589,-6799.8228913975881,2266.6076304658627,0,0,0,0,
            8414.8058244880795,-33659.223297952318,50488.834946928473,-33659.223297952318,8414.8058244880795,0,0,0,
            -17572.53342530342,87862.667126517103,-175725.33425303421,175725.33425303421,-87862.667126517103,17572.53342530342,0,0,
            19571.467239946949,-117428.80343968168,293572.00859920419,-391429.34479893895,293572.00859920419,-117428.80343968165,19571.467239946949,0,
            -9082.4109955586973,63576.876968910881,-190730.63090673267,317884.38484455441,-317884.38484455441,190730.63090673267,-63576.876968910881,9082.4109955586973,
            0,0,0,0,0,0,0,0;
    REQUIRE((bern.transpose()-mtr).squaredNorm() < double_eq_epsilon);

    bern = generator.bernsteinCoefficientMatrixTest(2);
    mtr << 325.61718841079289,-651.2343768215859,325.61718841079289,0,0,0,0,0,-4533.2152609317254,13599.645782795178,-13599.645782795176,4533.2152609317254,0,0,0,0,25244.417473464237,-100977.66989385695,151466.5048407854,-100977.66989385695,25244.417473464237,0,0,0,-70290.133701213679,351450.66850606841,-702901.33701213682,702901.33701213682,-351450.66850606841,70290.133701213679,0,0,97857.336199734738,-587144.01719840837,1467860.0429960208,-1957146.7239946947,1467860.0429960208,-587144.01719840826,97857.336199734738,0,-54494.465973352191,381461.26181346527,-1144383.7854403961,1907306.3090673264,-1907306.3090673264,1144383.7854403961,-381461.26181346527,54494.465973352191,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0;
    REQUIRE((bern.transpose()-mtr).squaredNorm() < double_eq_epsilon);

    bern = generator.bernsteinCoefficientMatrixTest(4);
    mtr << 50488.834946928473,-201955.33978771389,302933.00968157081,-201955.33978771389,50488.834946928473,0,0,0,-421740.8022072821,2108704.0110364105,-4217408.0220728209,4217408.0220728209,-2108704.0110364105,421740.8022072821,0,0,1174288.0343968167,-7045728.2063809,17614320.515952252,-23485760.687936336,17614320.515952252,-7045728.2063808991,1174288.0343968167,0,-1089889.3194670437,7629225.2362693055,-22887675.708807919,38146126.181346528,-38146126.181346528,22887675.708807919,-7629225.2362693055,1089889.3194670437,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0;

    REQUIRE((bern.transpose()-mtr).squaredNorm() < double_eq_epsilon);
}