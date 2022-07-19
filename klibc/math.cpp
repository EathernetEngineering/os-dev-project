#include "klibc/math.hpp"

//const uint64_t g_taylor_series_iterations = 8;

// -----------------------------------------------

int abs(int n)
{
	return n < 0 ? (n * -1) : n;
}

long int labs(long int n)
{
	return n < 0 ? (n * -1) : n;
}

long long int llabs(long long int n)
{
	return n < 0 ? (n * -1) : n;
}

//double abs(double n)
//{
//	return n < 0.0 ? (n * -1.0) : n;
//}

//float absf(float n)
//{
//	return n < 0.0f ? (n * -1.0f) : n;
//}

//long double absl(long double n)
//{
//	return n < (long double)0.0 ? (n * (long double)-1.0) : n;
//}

// -----------------------------------------------

//double exp(double n)
//{
//	double result = n;
//	double x_n = n;
//	for (uint64_t i = 0; i < g_taylor_series_iterations; i++)
//	{
//		for (uint64_t j = 0; j < i; j++)
//		{
//			x_n *= n;
//		}
//		x_n = n;
//		result += (x_n / factorial(i));
//	}
//	return result;
//}

//float expf(float n)
//{
//	UNUSED(n);
//	return 0;
//}

//long double expl(long double n)
//{
//	UNUSED(n);
//	return 0;
//}

// -----------------------------------------------

//double ln(double n)
//{
//	UNUSED(n);
//	return 0;
//}

//float lnf(float n)
//{
//	UNUSED(n);
//	return 0;
//}

//long double lnl(long double n)
//{
//	UNUSED(n);
//	return 0;
//}

// -----------------------------------------------

//double pow(double base, double x)
//{
//	UNUSED(base);
//	UNUSED(x);
//	return 0;
//}

//float powf(float base, float x)
//{
//	UNUSED(base);
//	UNUSED(x);
//	return 0;
//}

//long double powl(long double base, long double x)
//{
//	UNUSED(base);
//	UNUSED(x);
//	return 0;
//}

// -----------------------------------------------

int factorial(int n)
{
	int r = 1;
	for (; n > 0; n--) r *= n;
	return r;
}

long int factoriall(long int n)
{
	long int r = 1;
	for (; n > 0; n--) r *= n;
	return r;
}

unsigned int factorialu(unsigned int n)
{
	unsigned int r = 1;
	for (; n > 0; n--) r *= n;
	return r;
}

long unsigned int factorialul(long unsigned int n)
{
	long unsigned int r = 1;
	for (; n > 0; n--) r *= n;
	return r;
}

// -----------------------------------------------

