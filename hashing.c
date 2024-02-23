#include <stdint.h>
//#include "hashing.h"

///////////////////////////////////////////// from Dominik
// source: https://github.com/dominikkempa/lz77-to-slp/blob/main/src/karp_rabin_hashing.cpp

uint64_t mod_mersenne(uint64_t a, uint64_t p, uint64_t mersenne_prime_exponent) {
    // We are guaranteed that a < 2^(2k)
    // because a < 2^64 <= 2^(2k).
    a = (a & p) + (a >> mersenne_prime_exponent);
    a = (a & p) + (a >> mersenne_prime_exponent);
    return a == p ? 0 : a;
}


uint64_t mul_mod_mersenne(
    const uint64_t a,
    const uint64_t b,
    const uint64_t p,
    const uint64_t mersenne_prime_exponent) {
  const uint32_t k = mersenne_prime_exponent;
  __extension__ const unsigned __int128 ab =
    (unsigned __int128)a *
    (unsigned __int128)b;
  uint64_t lo = (uint64_t)ab;
  const uint64_t hi = (ab >> 64);
  lo = (lo & p) + ((lo >> k) + (hi << (64 - k)));
  lo = (lo & p) + (lo >> k);
  return lo == p ? 0 : lo;
}

// takes O(log n) time
uint64_t pow_mod_mersenne(
    const uint64_t a,
    const uint64_t m,
    const uint64_t p,
    const uint64_t mersenne_prime_exponent) {
  uint64_t pow = mod_mersenne(a, p, mersenne_prime_exponent);
  uint64_t ret = mod_mersenne(1, p, mersenne_prime_exponent);
  uint64_t n = m;
  while (n > 0) {
    //std::cout << n << std::endl;
    if (n & 1)
      ret = mul_mod_mersenne(ret, pow, p, mersenne_prime_exponent);
    pow = mul_mod_mersenne(pow, pow, p, mersenne_prime_exponent);
    n >>= 1;
  }
  return ret;
}

uint64_t concat(
    const uint64_t left_hash,
    const uint64_t right_hash,
    const uint64_t right_len,
    const uint64_t base,
    const uint64_t p,
    const uint64_t mersenne_prime_exponent) {
  return mod_mersenne(
    mul_mod_mersenne(pow_mod_mersenne(base, right_len, p, mersenne_prime_exponent),
        left_hash,
        p,
        mersenne_prime_exponent
    ) + right_hash,
    p,
    mersenne_prime_exponent
 );
}
/////////////////////////////////////////////