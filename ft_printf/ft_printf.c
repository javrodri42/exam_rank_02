
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

static int ft_strlen(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return (i);
}

static int ft_count(long long n, int base_len)
{
	int i = 1;
	while (n >= base_len)
	{
		n /= base_len;
		i++;
	}
	return (i);
}

static void	ft_putnbr(long long n, int base_len, char *base)
{
	if (n >= base_len)
		ft_putnbr(n / base_len, base_len, base);
	write(1, &base[n % base_len], 1);
}

int	ft_printf(const char *format, ...)
{
	va_list ap;
	int res = 0, len = 0, prec = 0, flag_prec = 0, neg = 0, zeros = 0, spaces = 0, width = 0, base_len = 0;
	long num = 0;
	char *base, *str, *s;
	va_start(ap, format);
	str = (char *)format;
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			len = 0, prec = 0, flag_prec = 0, neg = 0, zeros = 0, spaces = 0, width = 0, base_len = 0;
			while (*str <= '9' && *str >= '0')
			{
				width = width * 10 + *str - '0';
				str++;
			}
			if (*str == '.')
			{
				str++;
				flag_prec = 1;
				while (*str <= '9' && *str >= '0')
				{
					prec = prec * 10 + *str - '0';
					str++;
				}
			}
			if (*str == 's')
			{
				s = va_arg(ap, char *);
				if (!s)
					s = "(null)";
				len = ft_strlen(s);
			}
			if (*str == 'd')
			{
				num = va_arg(ap, int);
				base_len = 10;
				base = "0123456789";
				if (num < 0)
				{
					neg = 1;
					num *= -1;
				}
				len = ft_count(num, base_len) + neg;
			}
			if (*str == 'x')
			{
				num = va_arg(ap, unsigned);
				base_len = 16;
				base = "0123456789abcdef";
				len = ft_count(num, base_len);
			}
			if (flag_prec == 1 && prec > len && *str != 's')
				zeros = prec - len + neg;
			else if (flag_prec == 1 && prec < len && *str == 's')
				len = prec;
			else if (flag_prec == 1 && prec == 0 && (num == 0 || *str == 's'))
				len = 0;
			spaces = width - len - zeros;
			while (spaces-- > 0)
				res += write(1, " ", 1);
			if (neg)
				write(1, "-", 1);
			while (zeros-- > 0)
				res += write(1, "0", 1);
			if (*str == 's')
				write(1, s, len);
			else if (len > 0)
				ft_putnbr(num, base_len, base);
			res += len;
		}
		else
			res += write(1, str, 1);
		str++;
	}
	va_end(ap);
	return (res);
}