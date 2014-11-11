/***********************************************************
    random.c -- ����2��ʬ��
             -- �����ʬ��
             -- ����ʬ��
             -- �ؿ�ʬ��
             -- ����ʬ��
             -- �١���ʬ��
             -- �߾�ʬ��
             -- �������ƥ��å�ʬ��
             -- �磻�֥�ʬ��
             -- Cauchy (��������) ʬ��
             -- Fʬ��
             -- tʬ��
***********************************************************/

/***** ������� (������Ʊˡ) ******************************/

#include <limits.h>
static unsigned long seed = 1;  /* Ǥ�� */

void init_rnd(unsigned long x)
{
    seed = x;
}

unsigned long irnd(void)
{
    seed = seed * 1566083941UL + 1;
    return seed;
}

double rnd(void)  /* 0 <= rnd() < 1 */
{
    return (1.0 / (ULONG_MAX + 1.0)) * irnd();
}

/**********************************************************/

#include <math.h>
#define PI 3.141592653589793238
#define E  2.718281828459045235

double triangular_rnd(void)  /* ����ʬ�� */
{
    return rnd() - rnd();
}

double power_rnd1(int n)  /* �߾�ʬ�� 1 */
{
    int i;
    double p, r;

    p = rnd();
    for (i = 0; i < n; i++)
        if ((r = rnd()) > p) p = r;
    return p;
}

double power_rnd(double n)  /* �߾�ʬ�� 2 */
{
    return pow(rnd(), 1.0 / (n + 1));
}

double exp_rnd(void)  /* �ؿ�ʬ�� */
{
    return -log(1 - rnd());
}

double Cauchy_rnd1(void)  /* Cauchy (��������) ʬ�� 1 */
{
    return tan(PI * rnd());
}

double Cauchy_rnd(void)  /* Cauchy (��������) ʬ�� 2 */
{
    double x, y;

    do {
        x = 1 - rnd();  y = 2 * rnd() - 1;
    } while (x * x + y * y > 1);
    return y / x;
}

double nrnd1(void)  /* ����ʬ�� 1 */
{
    return rnd() + rnd() + rnd() + rnd() + rnd() + rnd()
         + rnd() + rnd() + rnd() + rnd() + rnd() + rnd() - 6;
}

double nrnd2(void)  /* ����ʬ�� 2 */
{
    static int sw = 0;
    static double t, u;

    if (sw == 0) {
        sw = 1;
        t = sqrt(-2 * log(1 - rnd()));  u = 2 * PI * rnd();
        return t * cos(u);
    } else {
        sw = 0;
        return t * sin(u);
    }
}

double nrnd(void)  /* ����ʬ�� 3 */
{
    static int sw = 0;
    static double r1, r2, s;

    if (sw == 0) {
        sw = 1;
        do {
            r1 = 2 * rnd() - 1;
            r2 = 2 * rnd() - 1;
            s = r1 * r1 + r2 * r2;
        } while (s > 1 || s == 0);
        s = sqrt(-2 * log(s) / s);
        return r1 * s;
    } else {
        sw = 0;
        return r2 * s;
    }
}

double gamma_rnd1(int two_a)  /* �����ʬ�� */
{
    int i;
    double x, r;

    x = 1;
    for (i = two_a / 2; i > 0; i--) x *= rnd();
    x = -log(x);
    if ((two_a & 1) != 0) {  /* if two_a is odd */
        r = nrnd();  x += 0.5 * r * r;
    }
    return x;
}

double gamma_rnd(double a)  /* �����ʬ��, a > 0 */
{
    double t, u, x, y;

    if (a > 1) {
        t = sqrt(2 * a - 1);
        do {
            do {
                /* ���Σ��Ԥ� y = tan(PI * rnd()) ��Ʊ�� */
                do {
                    x = 1 - rnd();  y = 2 * rnd() - 1;
                } while (x * x + y * y > 1);
                y /= x;
                x = t * y + a - 1;
            } while (x <= 0);
            u = (a - 1) * log(x / (a - 1)) - t * y;
        } while (u < -50 || rnd() > (1 + y * y) * exp(u));
    } else {
        t = E / (a + E);
        do {
            if (rnd() < t) {
                x = pow(rnd(), 1 / a);  y = exp(-x);
            } else {
                x = 1 - log(rnd());  y = pow(x, a - 1);
            }
        } while (rnd() >= y);
    }
    return x;
}

double beta_rnd1(double a, double b)  /* �١���ʬ�� 1 */
{
    double x, y;

    do {
        x = pow(rnd(), 1 / a);  y = pow(rnd(), 1 / b);
    } while (x + y > 1);
    return x / (x + y);
}

double beta_rnd(double a, double b)  /* �١���ʬ�� 2 */
{
    double temp;

    temp = gamma_rnd(a);
    return temp / (temp + gamma_rnd(b));
}

double chisq_rnd1(int n)  /* ����2��ʬ�� */
{
    int i;
    double s, t;

    s = 0;
    for (i = 0; i < n; i++) {  t = nrnd();  s += t * t;  }
    return s;
}

double chisq_rnd(double n)  /* ����2��ʬ�� */
{
    return 2 * gamma_rnd(0.5 * n);
}

double F_rnd(double n1, double n2)  /* Fʬ�� */
{
    return (chisq_rnd(n1) * n2) / (chisq_rnd(n2) * n1);
}

double t_rnd(double n)  /* tʬ�� */
{
    double a, b, c;

    if (n <= 2) return nrnd() / sqrt(chisq_rnd(n) / n);
    do {
        a = nrnd();
        b = a * a / (n - 2);
        c = log(1 - rnd()) / (1 - 0.5 * n);
    } while (exp(-b-c) > 1 - b);
    return a / sqrt((1 - 2.0 / n) * (1 - b));
}

double logistic_rnd(void)  /* �������ƥ��å�ʬ�� */
{
    double r;

    r = rnd();
    return log(r / (1 - r));
}

double Weibull_rnd(double alpha)  /* Weibull (�磻�֥�) ʬ�� */
{
    return pow(-log(1 - rnd()), 1 / alpha);
}

/**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int i, choice, ix, n, histo[20];
    double a, b, x, s1, s2;

    init_rnd((unsigned long) time(NULL));  /* ����� */

    printf("***** ��˥塼 *****\n");
    printf("  1: ����ʬ��\n");
    printf("  2: �߾�ʬ�� 1\n");
    printf("  3: �߾�ʬ�� 2\n");
    printf("  4: �ؿ�ʬ��\n");
    printf("  5: ��������ʬ�� 1\n");
    printf("  6: ��������ʬ�� 2\n");
    printf("  7: ����ʬ�� 1\n");
    printf("  8: ����ʬ�� 2\n");
    printf("  9: ����ʬ�� 3\n");
    printf(" 10: �����ʬ�� 1\n");
    printf(" 11: �����ʬ�� 2\n");
    printf(" 12: �١���ʬ�� 1\n");
    printf(" 13: �١���ʬ�� 2\n");
    printf(" 14: ��������ʬ�� 1\n");
    printf(" 15: ��������ʬ�� 2\n");
    printf(" 16: Fʬ��\n");
    printf(" 17: tʬ��\n");
    printf(" 18: �������ƥ��å�ʬ��\n");
    printf(" 19: �磻�֥�ʬ��\n");
    printf("? ");  scanf("%d", &choice);

    switch(choice) {
    case 2: case 3: case 10: case 11: case 14: case 15: case 17: case 19:
        printf("���� (1��)? ");  scanf("%lf", &a);  break;
    case 12: case 13: case 16:
        printf("���� (2��)? ");  scanf("%lf%lf", &a, &b);  break;
    }
    printf("�Ŀ�? ");  scanf("%d", &n);

    for (i = 0; i < 20; i++) histo[i] = 0;
    s1 = s2 = 0;
    for (i = 0; i < n; i++) {
        switch(choice) {
        case 1: x = triangular_rnd();  break;
        case 2: x = power_rnd1(a);  break;
        case 3: x = power_rnd(a);  break;
        case 4: x = exp_rnd();  break;
        case 5: x = Cauchy_rnd1();  break;
        case 6: x = Cauchy_rnd();  break;
        case 7: x = nrnd1();  break;
        case 8: x = nrnd2();  break;
        case 9: x = nrnd();  break;
        case 10: x = gamma_rnd1(a);  break;
        case 11: x = gamma_rnd(a);  break;
        case 12: x = beta_rnd1(a, b);  break;
        case 13: x = beta_rnd(a, b);  break;
        case 14: x = chisq_rnd1(a);  break;
        case 15: x = chisq_rnd(a);  break;
        case 16: x = F_rnd(a, b);  break;
        case 17: x = t_rnd(a);  break;
        case 18: x = logistic_rnd();  break;
        case 19: x = Weibull_rnd(a);  break;
        }
        ix = (int) floor(2 * x) + 10;
        if (ix >= 0 && ix < 20) histo[ix]++;
        s1 += x;  s2 += x * x;
    }

    for (i = 0; i < 20; i++)
        printf("%4.1f -- %4.1f: %5.1f%%\n",
            0.5 * (i - 10), 0.5 * (i - 9), 100.0 * histo[i] / n);
    s1 /= n;  s2 = sqrt((s2 - n * s1 * s1) / (n - 1));
    printf("ʿ�� %g  ɸ���к� %g\n", s1, s2);

    return EXIT_SUCCESS;
}