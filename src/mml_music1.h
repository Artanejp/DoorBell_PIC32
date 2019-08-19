#pragma once
/*
 * MML SYNTAX from "FM-7 F-BASIC Grammer book (???)" 
 * By FUJITSU LIMITED, Apr. 1984, at PAGE 3-137 to 3-144.
 * EXCEPTS:
 * Sx[,y] : Set envelope type [x] to param[y]. Y = 1228000 / (expect_freq * 256)
 * type [x] : bit8..
 * Xxxxxx : Set noise frequency [Hz].If 0 or minus, Unset noise.
 */

const char *test_mml1 = "T150V8S16,1800\
r1o5d+8g+8g8d+2^d+6r2..r12\
>b8<e8d+8>b2^b6r1r12\
<g8g+8a+8d+4<c+4>b2a+2^a+8<c+8d+8e8>g+4<a+4g1>d+8g+8g8d+2^d+6r2..r12\
>b8<e8d+8>b2^b6r1r12\
<a+8d+8g4d+8g+8a+8b2a+4<d8d+8>g+1^g+1a+4b8d+4r4.a+4b8<c+4>b4a+8d+4.f+1.^f+8b4<c+8>g+4r4.\
b4<c+8e4d+4^d+16c+2.^c+16d+8>a+1^a+8b4g+8r4\
<e4>b4b8g+8r4<e8r4d+2..>b8r1<c+4>b8r4g+4f4.r4.\
f8g+8a+4a+8a+8r4b4a+4a+8a+8r4b4a+4a+8a+4a+4b8a+1^a+8r1d+8g+8g8d+2^d+6r2..r12\
>b8<e8d+8>b2^b6r1r12\
<g8g+8a+8d+4<c+4>b2a+2^a+8<c+8d+8e8>g+4<a+4g1>d+8g+8g8d+2^d+6r2..r12\
>b8<e8d+8>b2^b6r1r12\
<a+8d+8g4d+8g+8a+8b2a+4<d8d+8>g+1^g+1a+4b8d+4r4.a+4b8<c+4>b4a+8d+4.f+1.^f+8b4<c+8>g+4r4.\
b4<c+8e4d+4^d+16c+2.^c+16d+8>a+1^a+8b4g+8r4<e4>b4b8g+8r4<e8r4d+2..>b8r1\
<c+4>b8r4g+4f4.r4.f8g+8a+4a+8a+8r4b4a+4a+8a+8r4b4a+4a+8a+4a+4b8a+1^a+8\
";
#if 1
const char *test_mml2 = "T150V3S16,400\
r2o3f4f+4\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
e8e16e16e8e16e16e8e16e16e8e16e16\
e8e16e16e8e16e16e8e16e16e8e16e16\
d+8d+16d+16d+8d+16d+16d+8d+16d+16d+8d+16d+16\
b8b16b16b8b16b16a+8a+16a+16a+8a+16a+16\
f+8f+16f+16f+8f+16f+16f+8f+16f+16f+8f+16f+16\
d+8d+16d+16d+8d+16d+16d+8d+16d+16d+8d+16d+16\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
e8e16e16e8e16e16e8e16e16e8e16e16\
e8e16e16e8e16e16e8e16e16e8e16e16\
d+8d+16d+16d+8d+16d+16d+8d+16d+16d+8d+16d+16\
b8b16b16b8b16b16a+8a+16a+16a+8a+16a+16\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+8.\
g+16g+16g+8g+16g+16g+8g+8g8g8\
f+8f+8f+8f+16f+16f+8f+8f+8f+16f+8.\
f+8f+8f+16f+16f+8f+8f+8f+8\
f+8f+8f+8f+16f+16f+8f+8f+8f+16f+8.\
f+8f+8f+16f+16f+8f+8f+8f+8\
g+8g+8g+8g+16g+16g+8g+8g+8g+4\
g+8g+8g+16g+16g+8g+8g+8g+8\
e8e8e8e8e8e8e8f+4\
f+8f+8f+8f+8f+8f+8f+8\
e8e8e8e8e8e8e8e8\
e8e8e8e8e8e8e8e8\
f+8f+8f+8f+8f+8f+8f+8f+8\
f+8f+8f+8f+8f+8f+8f+8f+8\
c+8c+8c+8c+8c+8c+8c+8c+8\
c+8c+8c+8c+8c+8c+8c+8d+4\
d+8d+8r4d+4d+4\
d+8d+8r4d+4r4.\
d+8r4d+4d+1^d+8\
r2f4f+4\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
e8e16e16e8e16e16e8e16e16e8e16e16\
e8e16e16e8e16e16e8e16e16e8e16e16\
d+8d+16d+16d+8d+16d+16d+8d+16d+16d+8d+16d+16\
b8b16b16b8b16b16a+8a+16a+16a+8a+16a+16\
f+8f+16f+16f+8f+16f+16f+8f+16f+16f+8f+16f+16\
d+8d+16d+16d+8d+16d+16d+8d+16d+16d+8d+16d+16\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
e8e16e16e8e16e16e8e16e16e8e16e16\
e8e16e16e8e16e16e8e16e16e8e16e16\
d+8d+16d+16d+8d+16d+16d+8d+16d+16d+8d+16d+16\
b8b16b16b8b16b16a+8a+16a+16a+8a+16a+16\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+8.\
g+16g+16g+8g+16g+16g+8g+8g8g8\
f+8f+8f+8f+16f+16f+8f+8f+8f+16f+8.\
f+8f+8f+16f+16f+8f+8f+8f+8\
f+8f+8f+8f+16f+16f+8f+8f+8f+16f+8.\
f+8f+8f+16f+16f+8f+8f+8f+8\
g+8g+8g+8g+16g+16g+8g+8g+8g+4\
g+8g+8g+16g+16g+8g+8g+8g+8\
e8e8e8e8e8e8e8f+4\
f+8f+8f+8f+8f+8f+8f+8\
e8e8e8e8e8e8e8e8\
e8e8e8e8e8e8e8e8\
f+8f+8f+8f+8f+8f+8f+8f+8\
f+8f+8f+8f+8f+8f+8f+8f+8\
c+8c+8c+8c+8c+8c+8c+8c+8\
c+8c+8c+8c+8c+8c+8c+8d+4\
d+8d+8r4d+4d+4\
d+8d+8r4d+4r4.\
d+8r4d+4d+1^d+8\
";

const char *test_mml3 = "T150V4\
r1\
o5d+8r4d+1^d+4\
d+4.\
d+8r4>b1^b4.\
r4\
a+1\
b2<c+2\
c+1\
>a+1\
<d+8r4d+1^d+4\
d+8r4\
>b8r4b1^b4\
b4.\
a+1\
b2a+2\
<d+1^d+1\
a+4a+8a+8r2\
a+4a+8a+4a+4.\
f+4.f+2.\
f+4f+2^f+8\
d+4d+8d+8d+2\
d+4d+8d+4d+4.\
g+2.g+8a+1^a+8\
g+4g+8r4g+4g+4\
g+8g+8r4g+4.\
f+2..f+2.\
f+4.\
g+4g+8r4g+4g+2.\
g+8g+8g+4\
a+8a+8r4a+4a+4\
a+8a+8r4a+4a+4\
a+8a+8r4.a+8a+1^a+8\
r1\
d+8r4d+1^d+4\
d+4.\
d+8r4>b1^b4.\
r4\
a+1\
b2<c+2\
c+1\
>a+1\
<d+8r4d+1^d+4\
d+8r4\
>b8r4b1^b4\
b4.\
a+1\
b2a+2\
<d+1^d+1\
a+4a+8a+8r2\
a+4a+8a+4a+4.\
f+4.f+2.\
f+4f+2^f+8\
d+4d+8d+8d+2\
d+4d+8d+4d+4.\
g+2.g+8a+1^a+8\
g+4g+8r4g+4g+4\
g+8g+8r4g+4.\
f+2..f+2.\
f+4.\
g+4g+8r4g+4g+2.\
g+8g+8g+4\
a+8a+8r4a+4a+4\
a+8a+8r4a+4a+4\
a+8a+8r4.a+8a+1^a+8\
";
#else
const char *test_mml2 = "T150V0";
const char *test_mml3 = "T150V0";
#endif