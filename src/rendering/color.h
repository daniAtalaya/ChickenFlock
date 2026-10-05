#pragma once
class Color {
	public:
		Color(const int r, const int g, const int b, const int a) : r(r), g(g), b(b), a(a) {}
		Color() {}

		int r = 0;
		int g = 0;
		int b = 0;
		int a = 0;
};