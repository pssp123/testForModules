//RSA密码
/*理解算法最重要，最好自己动手实现试试看，可以使用MFC写一个简单的交互界面*/

#include <iostream>
#include "main.hpp"
using namespace std;
//RSA算法所需参数

//小素数表
static RandNumber g_Rnd;
RandNumber::RandNumber(__uint64_t s)
{
    if(!s)
    {
        randSeed= (__uint64_t)time(NULL);
    }
    else
    {
        randSeed=s;
    }
}
__uint64_t RandNumber::Random(__uint64_t n)
{
    randSeed=multiplier * randSeed + adder;
    return randSeed % n;
}

//模乘运算，返回值 x=a*b mod n
inline __uint64_t MulMod(__uint64_t a, __uint64_t b, __uint64_t n)
{
    return a * b % n;
}

//模幂运算，返回值 x=base^pow mod n
__uint64_t PowMod(__uint64_t &base, __uint64_t &pow, __uint64_t &n)
{
    __uint64_t    a=base, b=pow, c=1;
    while(b)
    {
        while(!(b & 1))
        {
            b>>=1;            //a=a * a % n;    //函数看起来可以处理64位的整数，但由于这里a*a在a>=2^32时已经造成了溢出，因此实际处理范围没有64位
            a=MulMod(a, a, n);
        }        b--;        //c=a * c % n;        //这里也会溢出，若把64位整数拆为两个32位整数不知是否可以解决这个问题。
        c=MulMod(a, c, n);
    }    return c;
}
/*
Rabin-Miller素数测试，通过测试返回1，否则返回0。
n是待测素数。
*/
long RabinMillerKnl(__uint64_t &n)
{
    __uint64_t    b, m, j, v, i;
    m=n - 1;
    j=0;    //计算出m、j，使得n-1=m*2^j，其中m是正奇数，j是非负整数
    while(!(m & 1))
    {
        ++j;
        m>>=1;
    }    //随机取一个b，2<=b<n-1
    b=2 + g_Rnd.Random(n - 3);    //算v=b^m mod n
    v=PowMod(b, m, n);    //如果v==1，通过测试
    if(v == 1)
    {
        return 1;
    }    //令i=1
    i=1;    //v=n-1，通过测试
    while(v != n - 1)
    {
        //i==l，非素数，结束
        if(i == j)
        {
            return 0;
        }        //v=v^2 mod n，i=i+1
        __uint64_t xxx;
        __uint64_t xxxx = 2;
        xxx = xxxx;
        v = PowMod(v, xxx, n);
        ++i;        //循环到5
    }    return 1;
}
/*
Rabin-Miller素数测试，循环调用核心loop次
全部通过返回1，否则返回0
*/
long RabinMiller(__uint64_t &n, long loop)
{
    //用小素数筛选一次，提高效率
    for(long i=0; i < g_PrimeCount; i++)
    {
        if(n % g_PrimeTable[i] == 0)
        {
            return 0;
        }
    }    //循环调用Rabin-Miller测试loop次，使得非素数通过测试的概率降为(1/4)^loop
    for(long i=0; i < loop; i++)
    {
        if(!RabinMillerKnl(n))
        {
            return 0;
        }
    }    return 1;
}
/*
随机生成一个bits位(二进制位)的素数，最多32位
*/
__uint64_t RandomPrime(char bits)
{
    __uint64_t    base;
    do
    {
        base= (unsigned long)1 << (bits - 1);   //保证最高位是1
        base+=g_Rnd.Random(base);               //再加上一个随机数
        base|=1;    //保证最低位是1，即保证是奇数
    } while(!RabinMiller(base, 30));    //进行拉宾－米勒测试30次
    return base;    //全部通过认为是素数
}
/*
欧几里得法求最大公约数
*/
__uint64_t EuclidGcd(__uint64_t &p, __uint64_t &q)
{
    __uint64_t    a=p > q ? p : q;
    __uint64_t    b=p < q ? p : q;
    __uint64_t    t;
    if(p == q)
    {
        return p;   //两数相等，最大公约数就是本身
    }
    else
    {
        while(b)    //辗转相除法，gcd(a,b)=gcd(b,a-qb)
        {
            a=a % b;
            t=a;
            a=b;
            b=t;
        }        return a;
    }
}
/*
Stein法求最大公约数
*/
__uint64_t SteinGcd(__uint64_t &p, __uint64_t &q)
{
    __uint64_t    a=p > q ? p : q;
    __uint64_t    b=p < q ? p : q;
    __uint64_t    t, r=1;
    if(p == q)
    {
        return p;           //两数相等，最大公约数就是本身
    }
    else
    {
        while((!(a & 1)) && (!(b & 1)))
        {
            r<<=1;          //a、b为偶数时，gcd(a,b)=2*gcd(a/2,b/2)
            a>>=1;
            b>>=1;
        }
        if(!(a & 1))
        {
            t=a;            //a为偶数，交换a，b
            a=b;
            b=t;
        }
        do
        {
            while(!(b & 1))
            {
                b>>=1;      //b为偶数，a为奇数时，gcd(b,a)=gcd(b/2,a)
            }            if(b < a)
            {
                t=a;        //b小于a，交换a，b
                a=b;
                b=t;
            }            b=(b - a) >> 1; //b、a都是奇数，gcd(b,a)=gcd((b-a)/2,a)
        } while(b);
        return r * a;
    }
}
/*
已知a、b，求x，满足a*x =1 (mod b)
相当于求解a*x-b*y=1的最小整数解
*/
__uint64_t Euclid(__uint64_t &a, __uint64_t &b)
{
    __uint64_t m, e, i, j, x, y;
    long xx, yy;
    m=b;e=a;x=0;y=1;xx=1;yy=1;
    while(e)
    {
        i=m / e;j=m % e;
        m=e;e=j;j=y;y*=i;
        if(xx == yy)
        {
            if(x > y)
                y=x - y;
            else{
                y-=x;
                yy=0;
            }
        }
        else
        {
            y+=x;
            xx=1 - xx;
            yy=1 - yy;
        }        x=j;
    }
    if(xx == 0)
        x=b - x;
    return x;
}
/*
随机产生一个RSA加密参数
*/
RSA_PARAM RsaGetParam(void)
{
    RSA_PARAM           Rsa={ 0 };
    __uint64_t    t;
    Rsa.p=RandomPrime(16);          //随机生成两个素数
    Rsa.q=RandomPrime(16);
    Rsa.n=Rsa.p * Rsa.q;
    Rsa.f=(Rsa.p - 1) * (Rsa.q - 1);
    do
    {
        Rsa.e=g_Rnd.Random(65536);  //小于2^16，65536=2^16
        Rsa.e|=1;                   //保证最低位是1，即保证是奇数，因f一定是偶数，要互素，只能是奇数
    } while(SteinGcd(Rsa.e, Rsa.f) != 1);
    Rsa.d=Euclid(Rsa.e, Rsa.f);
    Rsa.s=0;
    t=Rsa.n >> 1;
    while(t)
    {
        Rsa.s++;                    //s=log2(n)
        t>>=1;
    }
    return Rsa;
}
/*
拉宾－米勒测试
*/
void TestRM(void)
{
    unsigned long   k=0;
    cout << "拉宾－米勒测试\n" << endl;
    for(__uint64_t i=4197900001; i < 4198000000; i+=2)
    {
        if(RabinMiller(i, 30))
        {
            k++;
            cout << i << endl;
        }
    }    cout << "Total: " << k << endl;
}
/*
 RSA加密解密
*/
void TestRSA(void) {
    cout << "请输入待加密的内容（支持字母、汉字、以及其他符号和下划线）：\n";
    RSA_PARAM r;
    string in_1;
    fflush(stdin);
    char pSrc[100];
    scanf("%s", pSrc);

    const unsigned long n = sizeof(pSrc);
    unsigned char *q, pDec[n];
    __uint64_t pEnc[n];
    r = RsaGetParam();
    cout << "---------------------------------\n";
    cout << "p=" << r.p << endl;
    cout << "q=" << r.q << endl;
    cout << "f=(p-1)*(q-1)=" << r.f << endl;
    cout << "n=p*q=" << r.n << endl;
    cout << "e=" << r.e << endl;
    cout << "d=" << r.d << endl;
    cout << "s=" << r.s << endl;
    cout << "---------------------------------\n";

    q = (unsigned char *) pSrc;
    cout << "Encode:\n";
    for (unsigned long i = 0; i < n; i++) {
        __uint64_t xxx;
        __uint64_t xxxx = q[i];
        xxx = xxxx;
        pEnc[i] = PowMod(xxx, r.e, r.n);
        cout << hex << pEnc[i] << " ";
    }
    cout << "\n\n";

    cout << "Decode:\n";
    for (unsigned long i = 0; i < n; i++) {
        pDec[i] = PowMod(pEnc[i], r.d, r.n);
        cout << hex << (unsigned long) pDec[i] << " ";
    }
    cout << "\n\n";

    cout << "解密后的文档：\n";
    cout << (char *) pDec << endl;
}
int main(void) {
    cout << "Start~!\n\n";
    char inorder;
    cout << "如果想进行下一个加密过程，请输入n键，退出请输入q键：\n\n";
    while (cin >> inorder && inorder == 'n') {
        TestRSA();
        cout << "如果想进行下一个加密过程，请输入n键，退出请输入q键：\n\n";
    }
    if (inorder == 'q')
        cout << "谢谢使用~！下次再见 > . < ~\n";
    return 0;
}
