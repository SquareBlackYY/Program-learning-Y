#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <set>
#include <cstdio>
#include <cstdlib>      
#include <vector>       
#include <algorithm> 

using namespace std;

int main() {
			int a, b, c, d, e, f;
			a = 4;
			b = 1;
			c = 8;		
			d = 3;
			e = 1;
			f = 5;
			cout << a << " |  " << b << " |  " << c << " |  " << d << " |  " << e << " |  " << f << endl;
				int x, y;
				//y = (c * d - f * a) / (b * d - e * a);
				//x = (c - b * y) / a;
				int p = 1;
				do {
					x = (f - c + 26 * p) ;
					//cout << " p= " << p <<  endl;
					p++;

				} while (x%(d - a) != 0);
				x = x / (d - a);

				p = 0;
				do {
					y = c - a * x+ 26 * p;
					p++;
					//cout << " p= " << p <<  endl;
				} while (y <1);				
				//输出方程的解
				cout << " x=" << x  << endl;
				cout << " y=" << y << endl;

	system("pause");
	return 0;
}