
// 迭代
int factorial_iter(int n)
{
    int ret = 1;
    for (int i = 2; i <= n; i++)
	ret *= i;
    return ret;
}

// 递归
int factorial_recur(int n)
{
    if (n == 1)
	return 1;
    else
	return n * factorial_recur(n - 1);
}

int main()
{
	printf("%d\n", factorial_iter(10));
	printf("%d\n", factorial_recur(10));
}
