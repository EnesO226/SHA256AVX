#include <iostream>
#include <immintrin.h>




__m256i  K0x428a2f98, K0x71374491, K0xb5c0fbcf, K0xe9b5dba5, K0x3956c25b, K0x59f111f1, K0x923f82a4, K0xab1c5ed5,
         K0xd807aa98, K0x12835b01, K0x243185be, K0x550c7dc3, K0x72be5d74, K0x80deb1fe, K0x9bdc06a7, K0xc19bf174,
         K0xe49b69c1, K0xefbe4786, K0x0fc19dc6, K0x240ca1cc, K0x2de92c6f, K0x4a7484aa, K0x5cb0a9dc, K0x76f988da,
         K0x983e5152, K0xa831c66d, K0xb00327c8, K0xbf597fc7, K0xc6e00bf3, K0xd5a79147, K0x06ca6351, K0x14292967,
         K0x27b70a85, K0x2e1b2138, K0x4d2c6dfc, K0x53380d13, K0x650a7354, K0x766a0abb, K0x81c2c92e, K0x92722c85,
         K0xa2bfe8a1, K0xa81a664b, K0xc24b8b70, K0xc76c51a3, K0xd192e819, K0xd6990624, K0xf40e3585, K0x106aa070,
         K0x19a4c116, K0x1e376c08, K0x2748774c, K0x34b0bcb5, K0x391c0cb3, K0x4ed8aa4a, K0x5b9cca4f, K0x682e6ff3,
         K0x748f82ee, K0x78a5636f, K0x84c87814, K0x8cc70208, K0x90befffa, K0xa4506ceb, K0xbef9a3f7, K0xc67178f2;

__m256i H0x6a09e667, H0xbb67ae85, H0x3c6ef372, H0xa54ff53a, H0x510e527f, H0x9b05688c, H0x1f83d9ab, H0x5be0cd19;
__m256i allones;

#define majority(a, b, c) _mm256_xor_si256(_mm256_xor_si256(_mm256_and_si256(a, b), _mm256_and_si256(b, c)), _mm256_and_si256(a, c))
#define choose(e, f, g) _mm256_xor_si256(_mm256_and_si256(e, f), _mm256_and_si256(_mm256_xor_si256(e, allones) , g))

#define ROR32_AVX(x, n) _mm256_or_si256(_mm256_srli_epi32((x), (n)), _mm256_slli_epi32((x), 32 - (n)))

#define sigma0(x) _mm256_xor_si256(_mm256_xor_si256(ROR32_AVX((x), 2), ROR32_AVX((x), 13)), ROR32_AVX((x), 22))
#define sigma1(x) _mm256_xor_si256(_mm256_xor_si256(ROR32_AVX((x), 6), ROR32_AVX((x), 11)), ROR32_AVX((x), 25))

#define step(maj, ch, s0, s1, az, bz, cz, dz, ez, fz, gz, hz, k, m) \
    ez = _mm256_add_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_add_epi32(ch(e, f, g), s1(e)), hz), k), m), dz); \
    az = _mm256_add_epi32(_mm256_add_epi32(_mm256_add_epi32(_mm256_add_epi32(ch(e, f, g), s1(e)), hz), k), m); \
    az = _mm256_add_epi32(az, _mm256_add_epi32(maj(a, b, c), s0(a)));


__m256i m[64];

void round() 
{
    __m256i a = _mm256_setr_epi32(0x6a09e667, 0x6a09e667, 0x6a09e667, 0x6a09e667, 0x6a09e667, 0x6a09e667, 0x6a09e667, 0x6a09e667);
    __m256i b = _mm256_setr_epi32(0xbb67ae85, 0xbb67ae85, 0xbb67ae85, 0xbb67ae85, 0xbb67ae85, 0xbb67ae85, 0xbb67ae85, 0xbb67ae85);
    __m256i c = _mm256_setr_epi32(0x3c6ef372, 0x3c6ef372, 0x3c6ef372, 0x3c6ef372, 0x3c6ef372, 0x3c6ef372, 0x3c6ef372, 0x3c6ef372);
    __m256i d = _mm256_setr_epi32(0xa54ff53a, 0xa54ff53a, 0xa54ff53a, 0xa54ff53a, 0xa54ff53a, 0xa54ff53a, 0xa54ff53a, 0xa54ff53a);
    __m256i e = _mm256_setr_epi32(0x510e527f, 0x510e527f, 0x510e527f, 0x510e527f, 0x510e527f, 0x510e527f, 0x510e527f, 0x510e527f);
    __m256i f = _mm256_setr_epi32(0x9b05688c, 0x9b05688c, 0x9b05688c, 0x9b05688c, 0x9b05688c, 0x9b05688c, 0x9b05688c, 0x9b05688c);
    __m256i g = _mm256_setr_epi32(0x1f83d9ab, 0x1f83d9ab, 0x1f83d9ab, 0x1f83d9ab, 0x1f83d9ab, 0x1f83d9ab, 0x1f83d9ab, 0x1f83d9ab);
    __m256i h = _mm256_setr_epi32(0x5be0cd19, 0x5be0cd19, 0x5be0cd19, 0x5be0cd19, 0x5be0cd19, 0x5be0cd19, 0x5be0cd19, 0x5be0cd19);

    step(majority, choose, sigma0, sigma1, a, b, c, d, e, f, g, h, K0x428a2f98, m[0]);
    step(majority, choose, sigma0, sigma1, h, a, b, c, d, e, f, g, K0x71374491, m[1]);
    step(majority, choose, sigma0, sigma1, g, h, a, b, c, d, e, f, K0xb5c0fbcf, m[2]);
    step(majority, choose, sigma0, sigma1, f, g, h, a, b, c, d, e, K0xe9b5dba5, m[3]);
    step(majority, choose, sigma0, sigma1, e, f, g, h, a, b, c, d, K0x3956c25b, m[4]);
    step(majority, choose, sigma0, sigma1, d, e, f, g, h, a, b, c, K0x59f111f1, m[5]);
    step(majority, choose, sigma0, sigma1, c, d, e, f, g, h, a, b, K0x923f82a4, m[6]);
    step(majority, choose, sigma0, sigma1, b, c, d, e, f, g, h, a, K0xab1c5ed5, m[7]);
    step(majority, choose, sigma0, sigma1, a, b, c, d, e, f, g, h, K0xd807aa98, m[8]);
    step(majority, choose, sigma0, sigma1, h, a, b, c, d, e, f, g, K0x12835b01, m[9]);
    step(majority, choose, sigma0, sigma1, g, h, a, b, c, d, e, f, K0x243185be, m[10]);
    step(majority, choose, sigma0, sigma1, f, g, h, a, b, c, d, e, K0x550c7dc3, m[11]);
    step(majority, choose, sigma0, sigma1, e, f, g, h, a, b, c, d, K0x72be5d74, m[12]);
    step(majority, choose, sigma0, sigma1, d, e, f, g, h, a, b, c, K0x80deb1fe, m[13]);
    step(majority, choose, sigma0, sigma1, c, d, e, f, g, h, a, b, K0x9bdc06a7, m[14]);
    step(majority, choose, sigma0, sigma1, b, c, d, e, f, g, h, a, K0xc19bf174, m[15]);
    step(majority, choose, sigma0, sigma1, a, b, c, d, e, f, g, h, K0xe49b69c1, m[16]);
    step(majority, choose, sigma0, sigma1, h, a, b, c, d, e, f, g, K0xefbe4786, m[17]);
    step(majority, choose, sigma0, sigma1, g, h, a, b, c, d, e, f, K0x0fc19dc6, m[18]);
    step(majority, choose, sigma0, sigma1, f, g, h, a, b, c, d, e, K0x240ca1cc, m[19]);
    step(majority, choose, sigma0, sigma1, e, f, g, h, a, b, c, d, K0x2de92c6f, m[20]);
    step(majority, choose, sigma0, sigma1, d, e, f, g, h, a, b, c, K0x4a7484aa, m[21]);
    step(majority, choose, sigma0, sigma1, c, d, e, f, g, h, a, b, K0x5cb0a9dc, m[22]);
    step(majority, choose, sigma0, sigma1, b, c, d, e, f, g, h, a, K0x76f988da, m[23]);
    step(majority, choose, sigma0, sigma1, a, b, c, d, e, f, g, h, K0x983e5152, m[24]);
    step(majority, choose, sigma0, sigma1, h, a, b, c, d, e, f, g, K0xa831c66d, m[25]);
    step(majority, choose, sigma0, sigma1, g, h, a, b, c, d, e, f, K0xb00327c8, m[26]);
    step(majority, choose, sigma0, sigma1, f, g, h, a, b, c, d, e, K0xbf597fc7, m[27]);
    step(majority, choose, sigma0, sigma1, e, f, g, h, a, b, c, d, K0xc6e00bf3, m[28]);
    step(majority, choose, sigma0, sigma1, d, e, f, g, h, a, b, c, K0xd5a79147, m[29]);
    step(majority, choose, sigma0, sigma1, c, d, e, f, g, h, a, b, K0x06ca6351, m[30]);
    step(majority, choose, sigma0, sigma1, b, c, d, e, f, g, h, a, K0x14292967, m[31]);
    step(majority, choose, sigma0, sigma1, a, b, c, d, e, f, g, h, K0x27b70a85, m[32]);
    step(majority, choose, sigma0, sigma1, h, a, b, c, d, e, f, g, K0x2e1b2138, m[33]);
    step(majority, choose, sigma0, sigma1, g, h, a, b, c, d, e, f, K0x4d2c6dfc, m[34]);
    step(majority, choose, sigma0, sigma1, f, g, h, a, b, c, d, e, K0x53380d13, m[35]);
    step(majority, choose, sigma0, sigma1, e, f, g, h, a, b, c, d, K0x650a7354, m[36]);
    step(majority, choose, sigma0, sigma1, d, e, f, g, h, a, b, c, K0x766a0abb, m[37]);
    step(majority, choose, sigma0, sigma1, c, d, e, f, g, h, a, b, K0x81c2c92e, m[38]);
    step(majority, choose, sigma0, sigma1, b, c, d, e, f, g, h, a, K0x92722c85, m[39]);
    step(majority, choose, sigma0, sigma1, a, b, c, d, e, f, g, h, K0xa2bfe8a1, m[40]);
    step(majority, choose, sigma0, sigma1, h, a, b, c, d, e, f, g, K0xa81a664b, m[41]);
    step(majority, choose, sigma0, sigma1, g, h, a, b, c, d, e, f, K0xc24b8b70, m[42]);
    step(majority, choose, sigma0, sigma1, f, g, h, a, b, c, d, e, K0xc76c51a3, m[43]);
    step(majority, choose, sigma0, sigma1, e, f, g, h, a, b, c, d, K0xd192e819, m[44]);
    step(majority, choose, sigma0, sigma1, d, e, f, g, h, a, b, c, K0xd6990624, m[45]);
    step(majority, choose, sigma0, sigma1, c, d, e, f, g, h, a, b, K0xf40e3585, m[46]);
    step(majority, choose, sigma0, sigma1, b, c, d, e, f, g, h, a, K0x106aa070, m[47]);
    step(majority, choose, sigma0, sigma1, a, b, c, d, e, f, g, h, K0x19a4c116, m[48]);
    step(majority, choose, sigma0, sigma1, h, a, b, c, d, e, f, g, K0x1e376c08, m[49]);
    step(majority, choose, sigma0, sigma1, g, h, a, b, c, d, e, f, K0x2748774c, m[50]);
    step(majority, choose, sigma0, sigma1, f, g, h, a, b, c, d, e, K0x34b0bcb5, m[51]);
    step(majority, choose, sigma0, sigma1, e, f, g, h, a, b, c, d, K0x391c0cb3, m[52]);
    step(majority, choose, sigma0, sigma1, d, e, f, g, h, a, b, c, K0x4ed8aa4a, m[53]);
    step(majority, choose, sigma0, sigma1, c, d, e, f, g, h, a, b, K0x5b9cca4f, m[54]);
    step(majority, choose, sigma0, sigma1, b, c, d, e, f, g, h, a, K0x682e6ff3, m[55]);
    step(majority, choose, sigma0, sigma1, a, b, c, d, e, f, g, h, K0x748f82ee, m[56]);
    step(majority, choose, sigma0, sigma1, h, a, b, c, d, e, f, g, K0x78a5636f, m[57]);
    step(majority, choose, sigma0, sigma1, g, h, a, b, c, d, e, f, K0x84c87814, m[58]);
    step(majority, choose, sigma0, sigma1, f, g, h, a, b, c, d, e, K0x8cc70208, m[59]);
    step(majority, choose, sigma0, sigma1, e, f, g, h, a, b, c, d, K0x90befffa, m[60]);
    step(majority, choose, sigma0, sigma1, d, e, f, g, h, a, b, c, K0xa4506ceb, m[61]);
    step(majority, choose, sigma0, sigma1, c, d, e, f, g, h, a, b, K0xbef9a3f7, m[62]);
    step(majority, choose, sigma0, sigma1, b, c, d, e, f, g, h, a, K0xc67178f2, m[63]);


    H0x6a09e667 = _mm256_add_epi32(H0x6a09e667, a);
    H0xbb67ae85 = _mm256_add_epi32(H0xbb67ae85, b);
    H0x3c6ef372 = _mm256_add_epi32(H0x3c6ef372, c);
    H0xa54ff53a = _mm256_add_epi32(H0xa54ff53a, d);
    H0x510e527f = _mm256_add_epi32(H0x510e527f, e);
    H0x9b05688c = _mm256_add_epi32(H0x9b05688c, f);
    H0x1f83d9ab = _mm256_add_epi32(H0x1f83d9ab, g);
    H0x5be0cd19 = _mm256_add_epi32(H0x5be0cd19, h);


    unsigned int aa = _mm256_extract_epi32(H0x6a09e667, 0);
    unsigned int bb = _mm256_extract_epi32(H0xbb67ae85, 0);
    unsigned int cc = _mm256_extract_epi32(H0x3c6ef372, 0);
    unsigned int dd = _mm256_extract_epi32(H0xa54ff53a, 0);
    unsigned int ee = _mm256_extract_epi32(H0x510e527f, 0);
    unsigned int ff = _mm256_extract_epi32(H0x9b05688c, 0);
    unsigned int gg = _mm256_extract_epi32(H0x1f83d9ab, 0);
    unsigned int hh = _mm256_extract_epi32(H0x5be0cd19, 0);

    printf("%x%x%x%x%x%x%x%x\n", aa, bb, cc, dd, ee, ff, gg, hh);
}
    


void InitAllOnes() {
    allones = _mm256_setr_epi32(0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff);
}

void InitH() 
{
    H0x6a09e667 = _mm256_setr_epi32(0x6a09e667, 0x6a09e667, 0x6a09e667, 0x6a09e667, 0x6a09e667, 0x6a09e667, 0x6a09e667, 0x6a09e667);
    H0xbb67ae85 = _mm256_setr_epi32(0xbb67ae85, 0xbb67ae85, 0xbb67ae85, 0xbb67ae85, 0xbb67ae85, 0xbb67ae85, 0xbb67ae85, 0xbb67ae85);
    H0x3c6ef372 = _mm256_setr_epi32(0x3c6ef372, 0x3c6ef372, 0x3c6ef372, 0x3c6ef372, 0x3c6ef372, 0x3c6ef372, 0x3c6ef372, 0x3c6ef372);
    H0xa54ff53a = _mm256_setr_epi32(0xa54ff53a, 0xa54ff53a, 0xa54ff53a, 0xa54ff53a, 0xa54ff53a, 0xa54ff53a, 0xa54ff53a, 0xa54ff53a);
    H0x510e527f = _mm256_setr_epi32(0x510e527f, 0x510e527f, 0x510e527f, 0x510e527f, 0x510e527f, 0x510e527f, 0x510e527f, 0x510e527f);
    H0x9b05688c = _mm256_setr_epi32(0x9b05688c, 0x9b05688c, 0x9b05688c, 0x9b05688c, 0x9b05688c, 0x9b05688c, 0x9b05688c, 0x9b05688c);
    H0x1f83d9ab = _mm256_setr_epi32(0x1f83d9ab, 0x1f83d9ab, 0x1f83d9ab, 0x1f83d9ab, 0x1f83d9ab, 0x1f83d9ab, 0x1f83d9ab, 0x1f83d9ab);
    H0x5be0cd19 = _mm256_setr_epi32(0x5be0cd19, 0x5be0cd19, 0x5be0cd19, 0x5be0cd19, 0x5be0cd19, 0x5be0cd19, 0x5be0cd19, 0x5be0cd19);
}

void InitK()
{
    K0x428a2f98 = _mm256_setr_epi32(0x428a2f98, 0x428a2f98, 0x428a2f98, 0x428a2f98, 0x428a2f98, 0x428a2f98, 0x428a2f98, 0x428a2f98);
    K0x71374491 = _mm256_setr_epi32(0x71374491, 0x71374491, 0x71374491, 0x71374491, 0x71374491, 0x71374491, 0x71374491, 0x71374491);
    K0xb5c0fbcf = _mm256_setr_epi32(0xb5c0fbcf, 0xb5c0fbcf, 0xb5c0fbcf, 0xb5c0fbcf, 0xb5c0fbcf, 0xb5c0fbcf, 0xb5c0fbcf, 0xb5c0fbcf);
    K0xe9b5dba5 = _mm256_setr_epi32(0xe9b5dba5, 0xe9b5dba5, 0xe9b5dba5, 0xe9b5dba5, 0xe9b5dba5, 0xe9b5dba5, 0xe9b5dba5, 0xe9b5dba5);
    K0x3956c25b = _mm256_setr_epi32(0x3956c25b, 0x3956c25b, 0x3956c25b, 0x3956c25b, 0x3956c25b, 0x3956c25b, 0x3956c25b, 0x3956c25b);
    K0x59f111f1 = _mm256_setr_epi32(0x59f111f1, 0x59f111f1, 0x59f111f1, 0x59f111f1, 0x59f111f1, 0x59f111f1, 0x59f111f1, 0x59f111f1);
    K0x923f82a4 = _mm256_setr_epi32(0x923f82a4, 0x923f82a4, 0x923f82a4, 0x923f82a4, 0x923f82a4, 0x923f82a4, 0x923f82a4, 0x923f82a4);
    K0xab1c5ed5 = _mm256_setr_epi32(0xab1c5ed5, 0xab1c5ed5, 0xab1c5ed5, 0xab1c5ed5, 0xab1c5ed5, 0xab1c5ed5, 0xab1c5ed5, 0xab1c5ed5);
    K0xd807aa98 = _mm256_setr_epi32(0xd807aa98, 0xd807aa98, 0xd807aa98, 0xd807aa98, 0xd807aa98, 0xd807aa98, 0xd807aa98, 0xd807aa98);
    K0x12835b01 = _mm256_setr_epi32(0x12835b01, 0x12835b01, 0x12835b01, 0x12835b01, 0x12835b01, 0x12835b01, 0x12835b01, 0x12835b01);
    K0x243185be = _mm256_setr_epi32(0x243185be, 0x243185be, 0x243185be, 0x243185be, 0x243185be, 0x243185be, 0x243185be, 0x243185be);
    K0x550c7dc3 = _mm256_setr_epi32(0x550c7dc3, 0x550c7dc3, 0x550c7dc3, 0x550c7dc3, 0x550c7dc3, 0x550c7dc3, 0x550c7dc3, 0x550c7dc3);
    K0x72be5d74 = _mm256_setr_epi32(0x72be5d74, 0x72be5d74, 0x72be5d74, 0x72be5d74, 0x72be5d74, 0x72be5d74, 0x72be5d74, 0x72be5d74);
    K0x80deb1fe = _mm256_setr_epi32(0x80deb1fe, 0x80deb1fe, 0x80deb1fe, 0x80deb1fe, 0x80deb1fe, 0x80deb1fe, 0x80deb1fe, 0x80deb1fe);
    K0x9bdc06a7 = _mm256_setr_epi32(0x9bdc06a7, 0x9bdc06a7, 0x9bdc06a7, 0x9bdc06a7, 0x9bdc06a7, 0x9bdc06a7, 0x9bdc06a7, 0x9bdc06a7);
    K0xc19bf174 = _mm256_setr_epi32(0xc19bf174, 0xc19bf174, 0xc19bf174, 0xc19bf174, 0xc19bf174, 0xc19bf174, 0xc19bf174, 0xc19bf174);
    K0xe49b69c1 = _mm256_setr_epi32(0xe49b69c1, 0xe49b69c1, 0xe49b69c1, 0xe49b69c1, 0xe49b69c1, 0xe49b69c1, 0xe49b69c1, 0xe49b69c1);
    K0xefbe4786 = _mm256_setr_epi32(0xefbe4786, 0xefbe4786, 0xefbe4786, 0xefbe4786, 0xefbe4786, 0xefbe4786, 0xefbe4786, 0xefbe4786);
    K0x0fc19dc6 = _mm256_setr_epi32(0x0fc19dc6, 0x0fc19dc6, 0x0fc19dc6, 0x0fc19dc6, 0x0fc19dc6, 0x0fc19dc6, 0x0fc19dc6, 0x0fc19dc6);
    K0x240ca1cc = _mm256_setr_epi32(0x240ca1cc, 0x240ca1cc, 0x240ca1cc, 0x240ca1cc, 0x240ca1cc, 0x240ca1cc, 0x240ca1cc, 0x240ca1cc);
    K0x2de92c6f = _mm256_setr_epi32(0x2de92c6f, 0x2de92c6f, 0x2de92c6f, 0x2de92c6f, 0x2de92c6f, 0x2de92c6f, 0x2de92c6f, 0x2de92c6f);
    K0x4a7484aa = _mm256_setr_epi32(0x4a7484aa, 0x4a7484aa, 0x4a7484aa, 0x4a7484aa, 0x4a7484aa, 0x4a7484aa, 0x4a7484aa, 0x4a7484aa);
    K0x5cb0a9dc = _mm256_setr_epi32(0x5cb0a9dc, 0x5cb0a9dc, 0x5cb0a9dc, 0x5cb0a9dc, 0x5cb0a9dc, 0x5cb0a9dc, 0x5cb0a9dc, 0x5cb0a9dc);
    K0x76f988da = _mm256_setr_epi32(0x76f988da, 0x76f988da, 0x76f988da, 0x76f988da, 0x76f988da, 0x76f988da, 0x76f988da, 0x76f988da);
    K0x983e5152 = _mm256_setr_epi32(0x983e5152, 0x983e5152, 0x983e5152, 0x983e5152, 0x983e5152, 0x983e5152, 0x983e5152, 0x983e5152);
    K0xa831c66d = _mm256_setr_epi32(0xa831c66d, 0xa831c66d, 0xa831c66d, 0xa831c66d, 0xa831c66d, 0xa831c66d, 0xa831c66d, 0xa831c66d);
    K0xb00327c8 = _mm256_setr_epi32(0xb00327c8, 0xb00327c8, 0xb00327c8, 0xb00327c8, 0xb00327c8, 0xb00327c8, 0xb00327c8, 0xb00327c8);
    K0xbf597fc7 = _mm256_setr_epi32(0xbf597fc7, 0xbf597fc7, 0xbf597fc7, 0xbf597fc7, 0xbf597fc7, 0xbf597fc7, 0xbf597fc7, 0xbf597fc7);
    K0xc6e00bf3 = _mm256_setr_epi32(0xc6e00bf3, 0xc6e00bf3, 0xc6e00bf3, 0xc6e00bf3, 0xc6e00bf3, 0xc6e00bf3, 0xc6e00bf3, 0xc6e00bf3);
    K0xd5a79147 = _mm256_setr_epi32(0xd5a79147, 0xd5a79147, 0xd5a79147, 0xd5a79147, 0xd5a79147, 0xd5a79147, 0xd5a79147, 0xd5a79147);
    K0x06ca6351 = _mm256_setr_epi32(0x06ca6351, 0x06ca6351, 0x06ca6351, 0x06ca6351, 0x06ca6351, 0x06ca6351, 0x06ca6351, 0x06ca6351);
    K0x14292967 = _mm256_setr_epi32(0x14292967, 0x14292967, 0x14292967, 0x14292967, 0x14292967, 0x14292967, 0x14292967, 0x14292967);
    K0x27b70a85 = _mm256_setr_epi32(0x27b70a85, 0x27b70a85, 0x27b70a85, 0x27b70a85, 0x27b70a85, 0x27b70a85, 0x27b70a85, 0x27b70a85);
    K0x2e1b2138 = _mm256_setr_epi32(0x2e1b2138, 0x2e1b2138, 0x2e1b2138, 0x2e1b2138, 0x2e1b2138, 0x2e1b2138, 0x2e1b2138, 0x2e1b2138);
    K0x4d2c6dfc = _mm256_setr_epi32(0x4d2c6dfc, 0x4d2c6dfc, 0x4d2c6dfc, 0x4d2c6dfc, 0x4d2c6dfc, 0x4d2c6dfc, 0x4d2c6dfc, 0x4d2c6dfc);
    K0x53380d13 = _mm256_setr_epi32(0x53380d13, 0x53380d13, 0x53380d13, 0x53380d13, 0x53380d13, 0x53380d13, 0x53380d13, 0x53380d13);
    K0x650a7354 = _mm256_setr_epi32(0x650a7354, 0x650a7354, 0x650a7354, 0x650a7354, 0x650a7354, 0x650a7354, 0x650a7354, 0x650a7354);
    K0x766a0abb = _mm256_setr_epi32(0x766a0abb, 0x766a0abb, 0x766a0abb, 0x766a0abb, 0x766a0abb, 0x766a0abb, 0x766a0abb, 0x766a0abb);
    K0x81c2c92e = _mm256_setr_epi32(0x81c2c92e, 0x81c2c92e, 0x81c2c92e, 0x81c2c92e, 0x81c2c92e, 0x81c2c92e, 0x81c2c92e, 0x81c2c92e);
    K0x92722c85 = _mm256_setr_epi32(0x92722c85, 0x92722c85, 0x92722c85, 0x92722c85, 0x92722c85, 0x92722c85, 0x92722c85, 0x92722c85);
    K0xa2bfe8a1 = _mm256_setr_epi32(0xa2bfe8a1, 0xa2bfe8a1, 0xa2bfe8a1, 0xa2bfe8a1, 0xa2bfe8a1, 0xa2bfe8a1, 0xa2bfe8a1, 0xa2bfe8a1);
    K0xa81a664b = _mm256_setr_epi32(0xa81a664b, 0xa81a664b, 0xa81a664b, 0xa81a664b, 0xa81a664b, 0xa81a664b, 0xa81a664b, 0xa81a664b);
    K0xc24b8b70 = _mm256_setr_epi32(0xc24b8b70, 0xc24b8b70, 0xc24b8b70, 0xc24b8b70, 0xc24b8b70, 0xc24b8b70, 0xc24b8b70, 0xc24b8b70);
    K0xc76c51a3 = _mm256_setr_epi32(0xc76c51a3, 0xc76c51a3, 0xc76c51a3, 0xc76c51a3, 0xc76c51a3, 0xc76c51a3, 0xc76c51a3, 0xc76c51a3);
    K0xd192e819 = _mm256_setr_epi32(0xd192e819, 0xd192e819, 0xd192e819, 0xd192e819, 0xd192e819, 0xd192e819, 0xd192e819, 0xd192e819);
    K0xd6990624 = _mm256_setr_epi32(0xd6990624, 0xd6990624, 0xd6990624, 0xd6990624, 0xd6990624, 0xd6990624, 0xd6990624, 0xd6990624);
    K0xf40e3585 = _mm256_setr_epi32(0xf40e3585, 0xf40e3585, 0xf40e3585, 0xf40e3585, 0xf40e3585, 0xf40e3585, 0xf40e3585, 0xf40e3585);
    K0x106aa070 = _mm256_setr_epi32(0x106aa070, 0x106aa070, 0x106aa070, 0x106aa070, 0x106aa070, 0x106aa070, 0x106aa070, 0x106aa070);
    K0x19a4c116 = _mm256_setr_epi32(0x19a4c116, 0x19a4c116, 0x19a4c116, 0x19a4c116, 0x19a4c116, 0x19a4c116, 0x19a4c116, 0x19a4c116);
    K0x1e376c08 = _mm256_setr_epi32(0x1e376c08, 0x1e376c08, 0x1e376c08, 0x1e376c08, 0x1e376c08, 0x1e376c08, 0x1e376c08, 0x1e376c08);
    K0x2748774c = _mm256_setr_epi32(0x2748774c, 0x2748774c, 0x2748774c, 0x2748774c, 0x2748774c, 0x2748774c, 0x2748774c, 0x2748774c);
    K0x34b0bcb5 = _mm256_setr_epi32(0x34b0bcb5, 0x34b0bcb5, 0x34b0bcb5, 0x34b0bcb5, 0x34b0bcb5, 0x34b0bcb5, 0x34b0bcb5, 0x34b0bcb5);
    K0x391c0cb3 = _mm256_setr_epi32(0x391c0cb3, 0x391c0cb3, 0x391c0cb3, 0x391c0cb3, 0x391c0cb3, 0x391c0cb3, 0x391c0cb3, 0x391c0cb3);
    K0x4ed8aa4a = _mm256_setr_epi32(0x4ed8aa4a, 0x4ed8aa4a, 0x4ed8aa4a, 0x4ed8aa4a, 0x4ed8aa4a, 0x4ed8aa4a, 0x4ed8aa4a, 0x4ed8aa4a);
    K0x5b9cca4f = _mm256_setr_epi32(0x5b9cca4f, 0x5b9cca4f, 0x5b9cca4f, 0x5b9cca4f, 0x5b9cca4f, 0x5b9cca4f, 0x5b9cca4f, 0x5b9cca4f);
    K0x682e6ff3 = _mm256_setr_epi32(0x682e6ff3, 0x682e6ff3, 0x682e6ff3, 0x682e6ff3, 0x682e6ff3, 0x682e6ff3, 0x682e6ff3, 0x682e6ff3);
    K0x748f82ee = _mm256_setr_epi32(0x748f82ee, 0x748f82ee, 0x748f82ee, 0x748f82ee, 0x748f82ee, 0x748f82ee, 0x748f82ee, 0x748f82ee);
    K0x78a5636f = _mm256_setr_epi32(0x78a5636f, 0x78a5636f, 0x78a5636f, 0x78a5636f, 0x78a5636f, 0x78a5636f, 0x78a5636f, 0x78a5636f);
    K0x84c87814 = _mm256_setr_epi32(0x84c87814, 0x84c87814, 0x84c87814, 0x84c87814, 0x84c87814, 0x84c87814, 0x84c87814, 0x84c87814);
    K0x8cc70208 = _mm256_setr_epi32(0x8cc70208, 0x8cc70208, 0x8cc70208, 0x8cc70208, 0x8cc70208, 0x8cc70208, 0x8cc70208, 0x8cc70208);
    K0x90befffa = _mm256_setr_epi32(0x90befffa, 0x90befffa, 0x90befffa, 0x90befffa, 0x90befffa, 0x90befffa, 0x90befffa, 0x90befffa);
    K0xa4506ceb = _mm256_setr_epi32(0xa4506ceb, 0xa4506ceb, 0xa4506ceb, 0xa4506ceb, 0xa4506ceb, 0xa4506ceb, 0xa4506ceb, 0xa4506ceb);
    K0xbef9a3f7 = _mm256_setr_epi32(0xbef9a3f7, 0xbef9a3f7, 0xbef9a3f7, 0xbef9a3f7, 0xbef9a3f7, 0xbef9a3f7, 0xbef9a3f7, 0xbef9a3f7);
    K0xc67178f2 = _mm256_setr_epi32(0xc67178f2, 0xc67178f2, 0xc67178f2, 0xc67178f2, 0xc67178f2, 0xc67178f2, 0xc67178f2, 0xc67178f2);

}


int main()
{
    InitK();
    InitH();
    InitAllOnes();
    for (int i = 0; i < 64; i++) {
        m[i] = _mm256_setr_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
    }

    round();
    std::cout << "Initialized constants...\n";
    system("pause");
}