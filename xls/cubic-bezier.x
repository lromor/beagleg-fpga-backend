import std

// Sample a cubic polynomial f(x) with coefficients a, b, c with no constant.
// f(x) = a * x + b * x ** 2 + c * x ** 3
// The function parameters are represented as fixed point rational numbers.
// The parameters are passed with the following precisions:
// x : signed 1:0:16, 16 bits wide, max value (1 << 16) - 1
// a : signed 1:18:2, 16 + 2, a = C1 * 3.
// b : signed 1:20:2, 18 + 2, b = 3 * (C2 - 2 * C1)
// c : signed 1:19:2, 19, c = 3 * C1 - 3 * C2 + C3
// The values a, b, c are precomputed.
// Their values derives from the signed control points of width 16 of the bezier curve.
// To maintain the exact representation, a, b and c are extended to match
// the required width.
// where Qm.n means fixed point rational number with m integral bits, and n
// fractional bits.
// The total amount of of bits required is provided by the largest multiplication
// which is c * x ** 3. The amount of bits required is
// 23 + 51 = 74.
fn main(x: sN[17], a: sN[23], b: sN[23], c: sN[23]) ->sN[24] {
    let x2: sN[34] = std::smul(x, x); // 1:1:32
    let x3: sN[51] = std::smul(x2, x); // 1:2:48

    // Before summing it to the final value, we need shift it
    // left to match the fractional point. This is 50 - (16 + 2) = 32
    let p1: sN[40] = std::smul(a, x);
    let p1: sN[74] = p1 as sN[74];
    let p1: sN[74] = p1 << sN[74]: 32;

    // The fractional part is positioned at 2 + 32 = 34 index.
    // The shift is then 50 - (32 + 2) = 14;
    let p2: sN[57] = std::smul(b, x2);
    let p2: sN[74] = p2 as sN[74];
    let p2: sN[74] = p2 << sN[74]: 16;

    // This doesn't need any shift.
    let p3: sN[74] = std::smul(c, x3);
    (p1 + p2 + p3)[50:74] as sN[24]
}
