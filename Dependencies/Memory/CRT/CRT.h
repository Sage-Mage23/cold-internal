#pragma once
#include <Windows.h>
#include <winternl.h>
#include <TlHelp32.h>

#include "../../Importer/lazy.h"

namespace NoCRT
{
	__forceinline wchar_t* to_string(int n)
	{
		int m = n;
		wchar_t s[100]; //max
		wchar_t ss[100]; //max
		int i = 0, j = 0;
		if (n < 0)
		{
			m = 0 - m;
			j = 1;
			ss[0] = '-';
		}
		while (m > 0)
		{
			s[i++] = m % 10 + '0';
			m /= 10;
		}
		s[i] = '\0';
		i = i - 1;
		while (i >= 0)
		{
			ss[j++] = s[i--];
		}
		ss[j] = '\0';
		return ss;
	}

	wchar_t* c_wcscpy(wchar_t* dest, const wchar_t* src)
	{
		wchar_t* dest_iter = dest;
		while (*src)
		{
			*dest_iter = *src;
			++dest_iter;
			++src;
		}
		*dest_iter = L'\0';
		return dest;
	}

	wchar_t* c_wcscat(wchar_t* dest, const wchar_t* src)
	{
		wchar_t* dest_iter = dest;
		while (*dest_iter)
		{
			++dest_iter;
		}

		while (*src)
		{
			*dest_iter = *src;
			++dest_iter;
			++src;
		}
		*dest_iter = L'\0';
		return dest;
	}

	int c_strlen(const char* string)
	{
		int cnt = 0;
		if (string)
		{
			for (; *string != 0; ++string) ++cnt;
		}
		return cnt;
	}

	__forceinline int __strlen(const char* str)
	{
		const char* s;
		for (s = str; *s; ++s);
		return (s - str);
	}

	int __fabs(int _X)
	{
		int s = _X >> 31;
		return (_X ^ s) - s;
	}

	double __sqrtf(double Number)
	{
		if (Number < 0)
		{
			return (-(double)(((double)(1e+300 * 1e+300)) * 0.0));
		}

		if (Number <= 1.192092896e-07)
		{
			return 0.0;
		}

		double MySqrt = Number;

		while (__fabs(MySqrt - Number / MySqrt) > 1.192092896e-07 * MySqrt)
		{
			MySqrt = (MySqrt + Number / MySqrt) / 2.0;
		}

		return MySqrt;
	}

	__forceinline int __wcslen(const wchar_t* s)
	{
		int len = 0;

		while (s[len] != L'\0')
		{
			if (s[++len] == L'\0')
				return len;
			if (s[++len] == L'\0')
				return len;
			if (s[++len] == L'\0')
				return len;
			++len;
		}

		return len;
	}

	__forceinline int __wcslen2(wchar_t* str)
	{
		int counter = 0;
		if (!str)
			return 0;
		for (; *str != '\0'; ++str)
			++counter;
		return counter;
	}

	__forceinline int __wcsicmp_insensitive(wchar_t* cs, wchar_t* ct)
	{
		auto len_cs = __wcslen2(cs);
		auto len_ct = __wcslen2(ct);

		if (len_cs < len_ct)
			return false;

		for (size_t i = 0; i <= len_cs - len_ct; i++)
		{
			bool match = true;

			for (size_t j = 0; j < len_ct; j++)
			{
				// Convert characters to lowercase manually
				wchar_t csChar = (cs[i + j] >= L'A' && cs[i + j] <= L'Z') ? (cs[i + j] + L'a' - L'A') : cs[i + j];
				wchar_t ctChar = (ct[j] >= L'A' && ct[j] <= L'Z') ? (ct[j] + L'a' - L'A') : ct[j];

				if (csChar != ctChar)
				{
					match = false;
					break;
				}
			}

			if (match)
				return true;
		}

		return false;
	}

	bool __isxdigit(char c)
	{
		return (c >= '0' && c <= '9') ||
			(c >= 'a' && c <= 'f') ||
			(c >= 'A' && c <= 'F');
	}

	unsigned long __strtoul(const char* str, char** endptr, int base = 16)
	{
		unsigned long result = 0;
		const char* ptr = str;

		while (__isxdigit(*ptr))
		{
			result *= base;
			if (*ptr >= '0' && *ptr <= '9')
			{
				result += *ptr - '0';
			}
			else if (*ptr >= 'a' && *ptr <= 'f')
			{
				result += *ptr - 'a' + 10;
			}
			else if (*ptr >= 'A' && *ptr <= 'F')
			{
				result += *ptr - 'A' + 10;
			}
			++ptr;
		}

		if (endptr)
		{
			*endptr = const_cast<char*>(ptr);
		}

		return result;
	}

	inline float __fabs(double x)
	{
		if (x < 0.0)
		{
			return -x;
		}

		return x;
	}

	__forceinline void* __memcpy(void* dest, const void* src, unsigned long long count)
	{
		char* char_dest = (char*)dest;
		char* char_src = (char*)src;
		if ((char_dest <= char_src) || (char_dest >= (char_src + count)))
		{
			while (count > 0)
			{
				*char_dest = *char_src;
				char_dest++;
				char_src++;
				count--;
			}
		}
		else
		{
			char_dest = (char*)dest + count - 1;
			char_src = (char*)src + count - 1;
			while (count > 0)
			{
				*char_dest = *char_src;
				char_dest--;
				char_src--;
				count--;
			}
		}

		return dest;
	}

	__forceinline void* __memset(void* dst0, int c0, unsigned int len)
	{
		unsigned int i;
		unsigned int fill;
		unsigned int chunks = len / sizeof(fill);
		char* char_dest = (char*)dst0;
		unsigned int* uint_dest = (unsigned int*)dst0;
		fill = (c0 << 24) + (c0 << 16) + (c0 << 8) + c0;

		for (i = len; i > chunks * sizeof(fill); i--)
		{
			char_dest[i - 1] = c0;
		}

		for (i = chunks; i > 0; i--)
		{
			uint_dest[i - 1] = fill;
		}

		return dst0;
	}

	__forceinline unsigned int __strncmp(const char* s1, const char* s2, size_t n)
	{
		if (n == 0)
			return (0);
		do
		{
			if (*s1 != *s2++)
				return (*(unsigned char*)s1 - *(unsigned char*)--s2);
			if (*s1++ == 0)
				break;
		} while (--n != 0);
		return (0);
	}

	__forceinline char* __strstr(const char* s, const char* find)
	{
		char c, sc;
		size_t len;
		if ((c = *find++) != 0)
		{
			len = __strlen(find);
			do
			{
				do
				{
					if ((sc = *s++) == 0)
					{
						return (0);
					}
				} while (sc != c);
			} while (__strncmp(s, find, len) != 0);
			s--;
		}
		return ((char*)s);
	}

	__forceinline unsigned int __rand()
	{
		int start_range = 10;
		int end_range = 30;

		static unsigned int rand = 0xACE1U;

		if (start_range == end_range)
		{
			return start_range;
		}

		rand += 0x3AD;
		rand %= (end_range - start_range + 1);

		while (rand < start_range)
		{
			rand = rand + (end_range - start_range + 1);
		}

		return rand;
	}

	float c_powf(float base, float exponent) {
		float result = 1;
		for (int i = 0; i < exponent; i++) {
			result *= base;
		}
		return result;
	}

	double c_factorial(int n) {
		double result = 1.0;
		for (int i = 2; i <= n; ++i) {
			result *= i;
		}
		return result;
	}

	double c_power(double base, int exponent) {
		double result = 1.0;
		for (int i = 0; i < exponent; ++i) {
			result *= base;
		}
		return result;
	}

	double c_sin(double angle) {
		double sinValue = 0.0;
		int iterations = 10;

		int sign = 1;
		for (int n = 1; n <= iterations; ++n) {
			double term = c_power(angle, 2 * n - 1) / c_factorial(2 * n - 1);
			sinValue += sign * term;
			sign *= -1;
		}

		return sinValue;
	}

	double c_cos(double angle) {
		double cosValue = 0.0;
		int iterations = 10;

		int sign = 1;
		for (int n = 0; n < iterations; ++n) {
			double term = c_power(angle, 2 * n) / c_factorial(2 * n);
			cosValue += sign * term;
			sign *= -1;
		}

		return cosValue;
	}

	static double 	sin_off_tbl[] = { 0.0, -M_PI / 2., 0, -M_PI / 2. };
	static double 	sin_sign_tbl[] = { 1,-1,-1,1 };

	static double 	cos_off_tbl[] = { 0.0, -M_PI / 2., 0, -M_PI / 2. };
	static double 	cos_sign_tbl[] = { 1,-1,-1,1 };

	double sinddd(double x)
	{
		int quadrant;
		double x2, result;

		/* Calculate the quadrant */
		quadrant = (int)(x * (2. / M_PI));

		/* Get offset inside quadrant */
		x = x - quadrant * (M_PI / 2.);

		/* Normalize quadrant to [0..3] */
		quadrant = (quadrant - 1) & 0x3;

		/* Fixup value for the generic function */
		x += sin_off_tbl[quadrant];

		/* Calculate the negative of the square of x */
		x2 = -(x * x);

		result = 0;

#if (PRECISION >= 10)
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13 * 14 * 15 * 16 * 17 * 18 * 19 * 20);
		result *= x2;
#endif
#if (PRECISION >= 9)
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13 * 14 * 15 * 16 * 17 * 18);
		result *= x2;
#endif
#if (PRECISION >= 8)
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13 * 14 * 15 * 16);
		result *= x2;
#endif
#if (PRECISION >= 7)
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13 * 14);
		result *= x2;
#endif
#if (PRECISION >= 6)
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12);
		result *= x2;
#endif
#if (PRECISION >= 5)
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10);
		result *= x2;
#endif
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8);
		result *= x2;

		result += 1. / (1. * 2 * 3 * 4 * 5 * 6);
		result *= x2;

		result += 1. / (1. * 2 * 3 * 4);
		result *= x2;

		result += 1. / (1. * 2);
		result *= x2;

		result += 1;

		/* Apply correct sign */
		result *= sin_sign_tbl[quadrant];

		return result;
	}

	double cosddd(double x)
	{

		int quadrant;
		double x2, result;

		/* Calculate the quadrant */
		quadrant = (int)(x * (2. / M_PI));

		/* Get offset inside quadrant */
		x = x - quadrant * (M_PI / 2.);

		/* Normalize quadrant to [0..3] */
		quadrant = quadrant & 0x3;

		/* Fixup value for the generic function */
		x += cos_off_tbl[quadrant];

		/* Calculate the negative of the square of x */
		x2 = -(x * x);

		result = 0;

#if (PRECISION >= 10)
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13 * 14 * 15 * 16 * 17 * 18 * 19 * 20);
		result *= x2;
#endif
#if (PRECISION >= 9)
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13 * 14 * 15 * 16 * 17 * 18);
		result *= x2;
#endif
#if (PRECISION >= 8)
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13 * 14 * 15 * 16);
		result *= x2;
#endif
#if (PRECISION >= 7)
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13 * 14);
		result *= x2;
#endif
#if (PRECISION >= 6)
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12);
		result *= x2;
#endif
#if (PRECISION >= 5)
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10);
		result *= x2;
#endif
		result += 1. / (1. * 2 * 3 * 4 * 5 * 6 * 7 * 8);
		result *= x2;

		result += 1. / (1. * 2 * 3 * 4 * 5 * 6);
		result *= x2;

		result += 1. / (1. * 2 * 3 * 4);
		result *= x2;

		result += 1. / (1. * 2);
		result *= x2;

		result += 1;

		result *= cos_sign_tbl[quadrant];

		return result;
	}

	float c_sinf(float _X)
	{
		return float(sinddd((double)_X));
	}

	float c_cosf(float _X)
	{
		return float(cosddd((double)_X));
	}

	float c_tanf(float _X)
	{
		return (c_sinf(_X) / c_cosf(_X));
	}

	double c_atan(double x) {
		if (x == 0.0) {
			return 0.0;
		}

		bool isNegative = false;
		if (x < 0.0) {
			x = -x;
			isNegative = true;
		}

		double result = x;
		double term = x;
		double divisor = 1.0;
		double divisorSquared = 1.0;

		for (int i = 1; i < 1000; i++) {
			divisorSquared += 2.0;
			divisor *= divisorSquared;

			term *= -x * x;
			double currentTerm = term / divisor;

			result += currentTerm;

			if (currentTerm == 0.0) {
				break;
			}
		}

		if (isNegative) {
			result = -result;
		}

		return result;
	}

	int c_fabs(int _X)
	{
		int s = _X >> 31;
		return (_X ^ s) - s;
	}

	double c_sqrtf(double Number)
	{
		if (Number < 0)
		{
			return (-(double)(((double)(1e+300 * 1e+300)) * 0.0));
		}

		if (Number <= 1.192092896e-07)
		{
			return 0.0;
		}

		double MySqrt = Number;

		while (c_fabs(MySqrt - Number / MySqrt) > 1.192092896e-07 * MySqrt)
		{
			MySqrt = (MySqrt + Number / MySqrt) / 2.0;
		}

		return MySqrt;
	}

	double c_atan2(double y, double x) {
		if (x == 0.0) {
			if (y > 0.0) {
				return 3.14159265358979323846 / 2.0;
			}
			else if (y < 0.0) {
				return -3.14159265358979323846 / 2.0;
			}
			else {
				return 0.0;
			}
		}

		double result = c_atan(y / x);

		if (x < 0.0) {
			result += (y >= 0.0) ? 3.14159265358979323846 : -3.14159265358979323846;
		}

		return result;
	}

	double c_acos(double x) {
		if (x > 1.0 || x < -1.0) {
			return 0.0;
		}

		double result = c_atan2(c_sqrtf(1 - x * x), x);
		return result;
	}
}
