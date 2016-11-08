#pragma once

template<typename R, typename... Args>
struct D
{
	int a;
	float b;
	R (*func)(Args... args);
};

int f3(int a, float b);
int f4(int a, float b);

//D<int, int, float> d[] =
//{
//	{1, 1.1f, f1},
//	{2, 2.2f, f2},
//};
//
int f1(int a, float b)
{
	return 1;
}
int f2(int a, float b)
{
	return 1;
}