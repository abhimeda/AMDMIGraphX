#include <iostream>
#include <vector>
#include <rtg/literal.hpp>
#include <rtg/operators.hpp>
#include <rtg/cpu/cpu_target.hpp>
#include "test.hpp"
#include "verify.hpp"

void exp_test()
{
    rtg::program p;
    rtg::shape s{rtg::shape::float_type, {3}};
    auto l = p.add_literal(rtg::literal{s, {-1, 0, 1}});
    p.add_instruction(rtg::exp{}, l);
    p.compile(rtg::cpu::cpu_target{});
    auto result = p.eval({});
    std::vector<float> results_vector(3);
    result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
    std::vector<float> gold = {0.36787944f, 1.f, 2.71828183f};
    EXPECT(test::verify_range(results_vector, gold));
}

void sin_test()
{
    rtg::program p;
    rtg::shape s{rtg::shape::float_type, {3}};
    auto l = p.add_literal(rtg::literal{s, {-1, 0, 1}});
    p.add_instruction(rtg::sin{}, l);
    p.compile(rtg::cpu::cpu_target{});
    auto result = p.eval({});
    std::vector<float> results_vector(3);
    result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
    std::vector<float> gold = {-0.84147098f, 0.f, 0.84147098f};
    EXPECT(test::verify_range(results_vector, gold));
}

void cos_test()
{
    rtg::program p;
    rtg::shape s{rtg::shape::float_type, {3}};
    auto l = p.add_literal(rtg::literal{s, {-1, 0, 1}});
    p.add_instruction(rtg::cos{}, l);
    p.compile(rtg::cpu::cpu_target{});
    auto result = p.eval({});
    std::vector<float> results_vector(3);
    result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
    std::vector<float> gold = {0.54030231f, 1.f, 0.54030231f};
    EXPECT(test::verify_range(results_vector, gold));
}

void tan_test()
{
    rtg::program p;
    rtg::shape s{rtg::shape::float_type, {3}};
    auto l = p.add_literal(rtg::literal{s, {-1, 0, 1}});
    p.add_instruction(rtg::tan{}, l);
    p.compile(rtg::cpu::cpu_target{});
    auto result = p.eval({});
    std::vector<float> results_vector(3);
    result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
    std::vector<float> gold = {-1.55740772f, 0.0f, 1.55740772f};
    EXPECT(test::verify_range(results_vector, gold));
}

void reshape_test()
{
    rtg::shape a_shape{rtg::shape::float_type, {24, 1, 1, 1}};
    std::vector<float> data(24);
    std::iota(data.begin(), data.end(), -3);
    {
        rtg::program p;
        auto l                         = p.add_literal(rtg::literal{a_shape, data});
        std::vector<int64_t> new_shape = {8, 3, 1, 1};
        p.add_instruction(rtg::reshape{new_shape}, l);
        p.compile(rtg::cpu::cpu_target{});
        auto result = p.eval({});
        std::vector<float> results_vector(3);
        result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
        EXPECT(test::verify_range(results_vector, data));
    }
    {
        rtg::program p;
        auto l                         = p.add_literal(rtg::literal{a_shape, data});
        std::vector<int64_t> new_shape = {1, 3, 4, 2};
        p.add_instruction(rtg::reshape{new_shape}, l);
        p.compile(rtg::cpu::cpu_target{});
        auto result = p.eval({});
        std::vector<float> results_vector(3);
        result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
        EXPECT(test::verify_range(results_vector, data));
    }
    {
        rtg::program p;
        auto l                         = p.add_literal(rtg::literal{a_shape, data});
        std::vector<int64_t> new_shape = {1, 3, 4, 2};
        p.add_instruction(rtg::reshape{new_shape}, l);
        p.compile(rtg::cpu::cpu_target{});
        auto result = p.eval({});
        std::vector<float> results_vector(3);
        result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
        EXPECT(test::verify_range(results_vector, data));
    }
}

// std::cout << std::abs(results_vector[i]-gold[i])  << std::endl;
void gemm_test()
{
    rtg::program p;
    std::vector<float> a = {-0.00925222, 0.56250403, 0.70107397,  0.75402161,  -0.505885,
                            1.33628943,  -0.11413,   -0.31270559, 1.59336732,  -0.19361027,
                            -0.91620867, 0.40108416, -0.06969921, 0.68483471,  -0.39906632,
                            -1.66423624, 0.69040076, -1.31490171, -0.11282616, -0.79391814};
    std::vector<float> b = {6.09568541e-01,
                            -6.10527007e-01,
                            3.66646462e-01,
                            1.18951101e-01,
                            5.58777432e-01,
                            -3.21296298e-01,
                            -5.95997198e-01,
                            -5.01425721e-01,
                            -2.84606807e-01,
                            -5.73673557e-01,
                            -8.99430260e-01,
                            -4.25103093e-01,
                            1.53027987e+00,
                            -3.81407415e-04,
                            -3.29650255e-01};
    std::vector<float> c = {-1.56327541e+00,
                            -7.09570140e-01,
                            -5.37424982e-01,
                            -2.22994831e-01,
                            -2.15586437e+00,
                            2.09177941e-03,
                            -1.47279677e+00,
                            2.02627040e-01,
                            -6.04527691e-01,
                            -1.29885596e+00,
                            2.16294914e+00,
                            -1.48101497e-01};
    rtg::shape a_shape{rtg::shape::float_type, {4, 5}};
    auto al = p.add_literal(rtg::literal{a_shape, a});
    rtg::shape b_shape{rtg::shape::float_type, {5, 3}};
    auto bl = p.add_literal(rtg::literal{b_shape, b});
    p.add_instruction(rtg::gemm{}, al, bl);
    p.compile(rtg::cpu::cpu_target{});
    auto result = p.eval({});
    std::vector<float> results_vector(12);
    result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
    float tol = 1e-6;
    for(int i = 0; i < results_vector.size(); i++)
    {
        EXPECT(std::abs(results_vector[i] - c[i]) < tol);
    }
}

void softmax_test()
{
    rtg::program p;
    std::vector<float> a = {
        -5.61869681e-01, 9.07827199e-01,  1.29255986e+00,  3.18533443e-02,  -1.22183852e-03,
        -2.83830553e-01, -1.03245842e+00, -9.28322077e-01, -8.82696748e-01, 1.11327164e-01,
        -9.20038462e-01, 8.47388089e-01,  2.51734018e-01,  1.50563884e+00,  2.23056650e+00,
        -6.17576987e-02, -1.00264274e-01, -6.10369384e-01, 1.17537189e+00,  -2.51560897e-01,
        -8.50333512e-01, -8.03578615e-01, -6.51194930e-01, -2.58137047e-01, 4.65528190e-01,
        3.23284641e-02,  -1.54700470e+00, 1.38096774e+00,  5.39869189e-01,  -7.56884992e-01,
        1.81503093e+00,  -2.11269641e+00, 1.92466557e+00,  1.77230799e+00,  2.21660900e+00,
        1.56777036e+00,  -2.08995026e-03, 3.50566894e-01,  -1.15042710e+00, -1.18577778e+00,
        8.90633047e-01,  -6.63949102e-02, 1.44661188e+00,  1.59215283e+00,  -2.56262213e-01,
        9.39079225e-01,  4.07298543e-02,  3.86590779e-01,  6.09607756e-01,  8.22331488e-01,
        -2.82126725e-01, -9.49052632e-01, -4.24012303e-01, -5.32990396e-01, -3.18386006e+00,
        3.27092171e-01,  -1.33315325e+00, 3.62459183e-01,  3.74710828e-01,  -1.30302286e+00,
        1.79680198e-01,  -4.51832324e-01, 4.34282750e-01,  -7.09520102e-01, 6.20333970e-01,
        -1.28712380e+00, 2.04130828e-01,  -7.70607769e-01, 1.61889160e+00,  -1.50951004e+00,
        -4.10505563e-01, -3.56566496e-02, -1.29747534e+00, -1.49967879e-01, 7.77626812e-01,
        -8.28408226e-02, 2.73412596e-02,  5.79780899e-03,  9.87900198e-02,  -7.95276761e-01,
        -1.38536084e+00, -6.63573861e-01, 3.89783204e-01,  -1.30670881e+00, -7.62425125e-01,
        -4.04883057e-01, 6.24344349e-01,  3.68128955e-01,  -1.01577950e+00, -3.06715906e-01,
        5.67961395e-01,  2.98198581e-01,  -1.63613629e+00, -3.75131965e-01, -6.75393403e-01,
        2.59172034e+00,  6.75538957e-01,  9.07939598e-02,  1.92257717e-01,  -1.21592450e+00,
        -2.73682117e-01, 1.25232983e+00,  -1.39969170e+00, -1.91483587e-01, 2.57732719e-01,
        3.10056299e-01,  1.41833842e+00,  -1.81386679e-01, 3.92868072e-01,  -8.14771175e-01,
        2.02392387e+00,  -9.42091495e-02, -3.77683818e-01, 2.05638766e+00,  2.93796062e-01,
        -6.02131486e-01, 2.70461679e-01,  -8.92358482e-01, 1.04388881e+00,  2.66154885e-01};

    std::vector<float> s = {
        0.30191708, 0.59879845, 0.50029165, 0.24915339, 0.36823985, 0.13190967, 0.0349741,
        0.18750034, 0.21905553, 0.27000085, 0.0547399,  0.56318235, 0.47422904, 0.78964758,
        0.91381913, 0.44601166, 0.47902739, 0.13120073, 0.4449684,  0.18766427, 0.15753111,
        0.07844277, 0.05120674, 0.36648798, 0.14637007, 0.13152322, 0.01560997, 0.29065287,
        0.49196178, 0.10550152, 0.81890774, 0.06369215, 0.62972021, 0.74931765, 0.67285055,
        0.35034987, 0.28612873, 0.31931475, 0.04220394, 0.16093165, 0.22390974, 0.11915915,
        0.3115395,  0.35899726, 0.22190949, 0.57518375, 0.13888834, 0.7753762,  0.4642328,
        0.57055861, 0.21954368, 0.34515455, 0.09486015, 0.40631217, 0.01842281, 0.48770609,
        0.06652815, 0.36023033, 0.42343026, 0.24226256, 0.17348589, 0.44066274, 0.6865865,
        0.17296699, 0.46923906, 0.06921105, 0.3570261,  0.4125829,  0.73165393, 0.15302512,
        0.29499072, 0.33932695, 0.30852377, 0.40762195, 0.40170741, 0.36259529, 0.60848355,
        0.42618036, 0.31721094, 0.02960522, 0.28256637, 0.24389413, 0.2725659,  0.10663581,
        0.27622163, 0.28264219, 0.53652936, 0.09476089, 0.40890986, 0.34848392, 0.32572666,
        0.53076893, 0.11529481, 0.29117745, 0.14625968, 0.8756339,  0.49818122, 0.10656087,
        0.1813329,  0.17664003, 0.21410346, 0.80408043, 0.02315119, 0.27155462, 0.32804728,
        0.13268511, 0.61795473, 0.49703068, 0.41696799, 0.10175809, 0.71028161, 0.29929739,
        0.17377149, 0.76075399, 0.20071237, 0.32632929, 0.36892858, 0.09416146, 0.26656723,
        0.42914796};

    rtg::shape a_shape{rtg::shape::float_type, {5, 3, 4, 2}};
    auto al = p.add_literal(rtg::literal{a_shape, a});
    p.add_instruction(rtg::softmax{}, al);
    p.compile(rtg::cpu::cpu_target{});
    auto result = p.eval({});
    std::vector<float> results_vector(120);
    result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
    EXPECT(test::verify_range(results_vector, s));
}

void conv2d_test()
{
    rtg::program p;
    std::vector<float> a = {
        2.71567607,  -0.9960829,  0.91671127,  0.28140706,  0.63235772,  0.08077253,  0.80927712,
        -0.59108931, -1.05421555, -2.76622486, -0.85044265, -0.52049929, 0.67726439,  -0.65290606,
        0.02345525,  -0.33579525, 0.38901961,  1.05473483,  -1.31188095, 1.8963089,   -0.07265259,
        0.947339,    0.41949373,  -0.70814759, 0.25892952,  1.07311416,  1.2571274,   -0.62318051,
        -0.19951548, -0.94232577, -0.29393643, 0.42292568,  -0.80230367, 1.40909171,  0.63617158,
        0.13900366,  1.09253144,  -0.15265895, 1.54781747,  0.72780299,  1.09189606,  -0.38068101,
        0.97057933,  -0.58958799, 1.56188643,  0.21474874,  0.58725154,  -1.27097559, -0.03024297,
        1.09437096,  -0.4897908,  0.34838957,  -1.31042492, -1.69069934, 0.86956722,  -0.40457946,
        0.46691212,  1.29273605,  0.26464137,  0.22073045,  -1.02178168, 0.22163901,  -1.84387338,
        0.75522131,  -0.45775682, -0.42241111, -1.50944722, 1.07256448,  -1.95876884, -0.28106022,
        0.3341668,   2.13129425,  -1.14728117, -1.06555498, -0.298444,   -0.88322699, -0.65866792,
        -2.06007552, 0.01374334,  0.45612028,  0.52715492,  1.01914406,  -1.72659791, 0.80650896,
        0.16860051,  2.24112225,  -0.78620857, 0.36566174,  -0.07020134, -0.47976932, -0.68230027,
        -0.94711417, -0.54506505, 1.66504931,  -0.71860826, 0.61132306};

    std::vector<float> c = {
        2.82721668e-02,  6.44195229e-02,  1.53499246e-02,  1.72468081e-01,  -6.33238107e-02,
        9.49496776e-02,  1.40258059e-01,  -7.92879611e-02, -1.29301161e-01, 3.11307609e-03,
        -1.90624535e-01, 1.13238767e-01,  -2.80647576e-02, 3.12882811e-02,  -3.52091640e-02,
        3.33581865e-02,  6.43158704e-02,  7.40238279e-02,  -1.00106120e-01, -9.56912562e-02,
        1.44342467e-01,  9.40258950e-02,  6.36333972e-02,  1.66158378e-03,  -8.91554281e-02,
        2.58734226e-02,  1.70919895e-02,  1.78214177e-01,  8.84564668e-02,  8.98126513e-02,
        -1.63809001e-01, 1.37802169e-01,  1.66439757e-01,  -1.45631135e-02, 1.88469887e-04,
        4.76950556e-02,  -1.91969007e-01, -1.76233292e-01, -7.70473927e-02, 1.14828631e-01,
        1.76608220e-01,  -1.50728196e-01, 1.99946314e-02,  -5.88052124e-02, 1.31612435e-01,
        1.61106288e-02,  -1.35080189e-01, 1.49512306e-01,  3.86456847e-02,  1.29330024e-01,
        -3.22975963e-02, -5.60784787e-02, -5.41997552e-02, 4.78562862e-02};

    std::vector<float> s = {0.27039781,
                            0.19105849,
                            -0.06339942,
                            -0.65087199,
                            0.40867025,
                            0.05063812,
                            -0.14907975,
                            0.49018705,
                            -0.49197209,
                            0.33236548,
                            -0.39374301,
                            0.16012701,
                            0.06574871,
                            0.71606487,
                            -0.55201721,
                            -0.46427044};
    rtg::shape a_shape{rtg::shape::float_type, {2, 3, 4, 4}};
    auto al = p.add_literal(rtg::literal{a_shape, a});

    rtg::shape c_shape{rtg::shape::float_type, {2, 3, 3, 3}};
    auto cl = p.add_literal(rtg::literal{c_shape, c});

    p.add_instruction(rtg::convolution{}, al, cl);
    p.compile(rtg::cpu::cpu_target{});
    auto result = p.eval({});

    std::vector<float> results_vector(16);
    result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
    EXPECT(test::verify_range(results_vector, s));
}

void conv2d_padding_test()
{
    rtg::program p;
    std::vector<float> a = {
        2.71567607,  -0.9960829,  0.91671127,  0.28140706,  0.63235772,  0.08077253,  0.80927712,
        -0.59108931, -1.05421555, -2.76622486, -0.85044265, -0.52049929, 0.67726439,  -0.65290606,
        0.02345525,  -0.33579525, 0.38901961,  1.05473483,  -1.31188095, 1.8963089,   -0.07265259,
        0.947339,    0.41949373,  -0.70814759, 0.25892952,  1.07311416,  1.2571274,   -0.62318051,
        -0.19951548, -0.94232577, -0.29393643, 0.42292568,  -0.80230367, 1.40909171,  0.63617158,
        0.13900366,  1.09253144,  -0.15265895, 1.54781747,  0.72780299,  1.09189606,  -0.38068101,
        0.97057933,  -0.58958799, 1.56188643,  0.21474874,  0.58725154,  -1.27097559, -0.03024297,
        1.09437096,  -0.4897908,  0.34838957,  -1.31042492, -1.69069934, 0.86956722,  -0.40457946,
        0.46691212,  1.29273605,  0.26464137,  0.22073045,  -1.02178168, 0.22163901,  -1.84387338,
        0.75522131,  -0.45775682, -0.42241111, -1.50944722, 1.07256448,  -1.95876884, -0.28106022,
        0.3341668,   2.13129425,  -1.14728117, -1.06555498, -0.298444,   -0.88322699, -0.65866792,
        -2.06007552, 0.01374334,  0.45612028,  0.52715492,  1.01914406,  -1.72659791, 0.80650896,
        0.16860051,  2.24112225,  -0.78620857, 0.36566174,  -0.07020134, -0.47976932, -0.68230027,
        -0.94711417, -0.54506505, 1.66504931,  -0.71860826, 0.61132306};

    std::vector<float> c = {
        -0.16115488, -0.09800646, -0.05412646, 0.10475694,  0.00555485,  -0.12667653, 0.0458357,
        -0.02656217, -0.16338061, 0.15037455,  0.0102711,   0.01303349,  0.05242859,  0.02034754,
        0.04751867,  -0.17038961, -0.1434752,  -0.10770349, 0.05676742,  -0.15838449, 0.10128359,
        -0.18958683, 0.11954515,  0.10758857,  -0.01058291, -0.12797487, 0.08971019,  0.18793164,
        -0.00881396, -0.06588994, -0.13321903, -0.03300409, 0.01439607,  0.07618178,  -0.11556662,
        0.00764295,  0.12956454,  -0.08937147, -0.12763587, 0.04674943,  0.05765297,  0.11336918,
        0.14747436,  -0.06199479, -0.01166052, -0.12432006, -0.04494537, -0.17581205, 0.09475745,
        0.1149437,   -0.1014564,  0.0274073,   -0.01323579, -0.11092556};

    std::vector<float> s = {
        -0.0201216,  0.40407312,  -0.39005592, -0.0631946,  0.37963012,  -0.64611685, 0.1349397,
        -0.54113752, 0.28533003,  0.27667275,  -0.16442731, -0.181494,   0.30564839,  0.58744538,
        0.32015014,  0.24969585,  -0.27367792, -0.53308117, 0.41236052,  0.26136363,  -0.01489828,
        0.57652152,  -0.38506854, 0.119615,    0.0437076,   0.04779706,  0.57887721,  0.23126155,
        0.05695833,  -0.68200272, 0.02063358,  -0.10267162, 0.8062973,   -0.38149622, -0.40134856,
        -0.03353126, 0.38991132,  -0.3478111,  0.03661491,  0.25783631,  0.62772679,  -0.1961118,
        0.76423508,  -0.36241418, -0.20994355, -0.12368261, -0.9406727,  0.02340185,  -0.08793129,
        -0.02471633, -0.58163726, -0.02211772, -0.42014724, 0.77525634,  0.504951,    -0.20537445,
        -0.20369984, -0.83037728, -1.40423918, -0.46160448, -0.22944322, 0.36074194,  0.49579027,
        0.46527559};

    rtg::shape a_shape{rtg::shape::float_type, {2, 3, 4, 4}};
    auto al = p.add_literal(rtg::literal{a_shape, a});

    rtg::shape c_shape{rtg::shape::float_type, {2, 3, 3, 3}};
    auto cl = p.add_literal(rtg::literal{c_shape, c});

    p.add_instruction(rtg::convolution{{{1, 1}}, {{1, 1}}}, al, cl);
    p.compile(rtg::cpu::cpu_target{});
    auto result = p.eval({});

    std::vector<float> results_vector(64);
    result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
    EXPECT(test::verify_range(results_vector, s));
}

void conv2d_padding_stride_test()
{
    rtg::program p;
    std::vector<float> a = {
        2.71567607,  -0.9960829,  0.91671127,  0.28140706,  0.63235772,  0.08077253,  0.80927712,
        -0.59108931, -1.05421555, -2.76622486, -0.85044265, -0.52049929, 0.67726439,  -0.65290606,
        0.02345525,  -0.33579525, 0.38901961,  1.05473483,  -1.31188095, 1.8963089,   -0.07265259,
        0.947339,    0.41949373,  -0.70814759, 0.25892952,  1.07311416,  1.2571274,   -0.62318051,
        -0.19951548, -0.94232577, -0.29393643, 0.42292568,  -0.80230367, 1.40909171,  0.63617158,
        0.13900366,  1.09253144,  -0.15265895, 1.54781747,  0.72780299,  1.09189606,  -0.38068101,
        0.97057933,  -0.58958799, 1.56188643,  0.21474874,  0.58725154,  -1.27097559, -0.03024297,
        1.09437096,  -0.4897908,  0.34838957,  -1.31042492, -1.69069934, 0.86956722,  -0.40457946,
        0.46691212,  1.29273605,  0.26464137,  0.22073045,  -1.02178168, 0.22163901,  -1.84387338,
        0.75522131,  -0.45775682, -0.42241111, -1.50944722, 1.07256448,  -1.95876884, -0.28106022,
        0.3341668,   2.13129425,  -1.14728117, -1.06555498, -0.298444,   -0.88322699, -0.65866792,
        -2.06007552, 0.01374334,  0.45612028,  0.52715492,  1.01914406,  -1.72659791, 0.80650896,
        0.16860051,  2.24112225,  -0.78620857, 0.36566174,  -0.07020134, -0.47976932, -0.68230027,
        -0.94711417, -0.54506505, 1.66504931,  -0.71860826, 0.61132306};

    std::vector<float> c = {
        -0.14601797, -0.13000923, 0.06521662,  0.06178288,  -0.11083675, 0.10154136,  0.09990512,
        0.06030385,  -0.11374587, -0.17523311, -0.14344215, 0.17802463,  0.06300922,  -0.15325832,
        0.07066704,  0.05166031,  0.00615084,  -0.02606523, 0.08083995,  -0.17913306, 0.0624622,
        0.0735731,   -0.04198661, -0.0164391,  -0.06374192, 0.16569914,  0.10681538,  0.07370754,
        0.02802075,  0.00282027,  0.15104802,  -0.11084409, -0.00197773, 0.07924436,  0.03528272,
        0.04765259,  -0.15896152, 0.07917164,  0.12125669,  -0.1154705,  -0.11999125, 0.12749968,
        -0.06269585, 0.18658121,  -0.03944227, 0.0111798,   -0.17731084, 0.11789055,  -0.09982193,
        0.08142821,  0.0729029,   0.11303909,  0.12735154,  0.03885292};

    std::vector<float> s = {-0.20817225,
                            0.87965256,
                            0.14958936,
                            -1.24887264,
                            -0.06540672,
                            0.20778663,
                            0.40456355,
                            -0.99900877,
                            0.4917807,
                            0.1994698,
                            0.64205718,
                            0.37798831,
                            -0.25315839,
                            0.44276932,
                            -0.16138598,
                            0.79344082};

    rtg::shape a_shape{rtg::shape::float_type, {2, 3, 4, 4}};
    auto al = p.add_literal(rtg::literal{a_shape, a});

    rtg::shape c_shape{rtg::shape::float_type, {2, 3, 3, 3}};
    auto cl = p.add_literal(rtg::literal{c_shape, c});

    p.add_instruction(rtg::convolution{{{1, 1}}, {{2, 2}}}, al, cl);
    p.compile(rtg::cpu::cpu_target{});
    auto result = p.eval({});

    std::vector<float> results_vector(16);
    result.visit([&](auto output) { results_vector.assign(output.begin(), output.end()); });
    EXPECT(test::verify_range(results_vector, s));
}

int main()
{
    exp_test();
    sin_test();
    cos_test();
    tan_test();
    gemm_test();
    reshape_test();
    softmax_test();
    conv2d_test();
    conv2d_padding_test();
    conv2d_padding_stride_test();
}