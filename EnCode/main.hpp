//
// Created by pierre on 18-5-16.
//

#ifndef ENCODE_MAIN_HPP
#define ENCODE_MAIN_HPP
struct RSA_PARAM
{
    __uint64_t    p,q;   //两个素数，不参与加密解密运算
    __uint64_t    f;      //f=(p-1)*(q-1)，不参与加密解密运算
    __uint64_t    n,e;   //公匙，n=p*q，gcd(e,f)=1
    __uint64_t    d;      //私匙，e*d=1 (mod f)，gcd(n,d)=1
    __uint64_t    s;      //块长，满足2^s<=n的最大的s，即log2(n)
};

const static long       g_PrimeTable[]=
{
    3,5,7,11,13,17,19,23,29,31,37,41,43,
    47,53,59,61,67,71,73,79,83,89,97
};

const static long g_PrimeCount=sizeof(g_PrimeTable) / sizeof(long);const __uint64_t  multiplier=12747293821;
const __uint64_t adder=1343545677842234541;//随机数类

class RandNumber
{
private:
    __uint64_t    randSeed;
public:
    RandNumber(__uint64_t s=0);
    __uint64_t    Random(__uint64_t n);
};
#endif //ENCODE_MAIN_HPP
