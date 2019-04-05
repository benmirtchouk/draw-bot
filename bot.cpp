#include <Windows.h>
#include <bits/stdc++.h>
#include "bitmap_image.hpp"
#define all(a) a.begin(), a.end()
#define forn(i,n) for(int i = 0; i < (int) n; i++)
#define ios ios::sync_with_stdio(false); cin.tie(0); cout.tie(0)

using namespace std;
typedef int(__stdcall *MYPROC)(HDC hd, int x, int y);
typedef pair<int, int> pii;

void wait() {
  while(!(GetKeyState('Q') & 0x100));
}

int sz = 100;

int main() {
//  /* input color */
//
//  POINT black;
//  cout << "where is black?\n";
//  wait();
//  Sleep(100);
//  GetCursorPos(&black);

  /* input canvas */

  POINT tl, br;

  cout << "top left?\n";
  wait();
  GetCursorPos(&tl);
  cout << "bottom right?\n";
  Sleep(300);
  wait();
  GetCursorPos(&br);

  int w = br.x - tl.x;
  int h = br.y - tl.y;
  cout << "canvas: " << w << " " << h << "\n";

  /* input image */

  bitmap_image image("img.bmp");
  int iw = image.width(), ih = image.height();
  cout << "image: " << iw << " " << ih << "\n";

  /* scale image + greyscale it */

  cout << "height has to be " << (int)(1.0 * ih / iw * w) << "\n";
//  assert(1.0 * ih / iw * w <= h);

  int y0 = h / 2. - 0.5 * ih / iw * w;
  int nh = 1.0 * ih / iw * w;
  int scy = max(1, nh / sz);
  int scx = max(1, w / sz);

  cout << "y0, y1 = " << y0 << ", " << y0 + nh << "\n";

  vector<vector<double>> im(w, vector<double>(nh));
  bitmap_image grey_big(w, nh);

  forn(y, nh) forn(x, w) {

    double sr = 0, sg = 0, sb = 0, cnt = 0;
    rgb_t c;
    for(int i = 1.0 * x * iw / w; i < 1.0 * (x + 1) * iw / w; i++)
      for(int j = 1.0 * y * ih / nh; j < 1.0 * (y + 1) * ih / nh; j++) {
        image.get_pixel(i, j, c);
        sr += c.red;
        sg += c.green;
        sb += c.blue;
        cnt++;
      }

    sr /= cnt, sg /= cnt, sb /= cnt;
    im[x][y] = 0.21 * sr + 0.72 * sg + 0.07 * sb;
    grey_big.set_pixel(x, y, im[x][y], im[x][y], im[x][y]);
  }
  grey_big.save_image("grey_big.bmp");

  bitmap_image grey(sz, sz);
  double dx = 1.0*w / sz;
  double dy = 1.0*nh / sz;

  vector<vector<double>> fim(sz, vector<double>(sz));
  forn(x, sz) forn(y, sz) {

    double h = 0;
    int cnt = 0;

    for(int i = x * dx; i < (x + 1)*dx; i++)
      for(int j = y * dy; j < (y + 1)*dy; j++) {
        h += im[i][j];
        cnt++;
      }

    fim[x][y] = h / cnt;
    grey.set_pixel(x, y, fim[x][y], fim[x][y], fim[x][y]);
  }
  grey.save_image("grey.bmp");

  /* dither */
  bitmap_image dithered(sz, sz);

  forn(y, sz) forn(x, sz) {

    double old = fim[x][y];
    double nw = round(old / 255) * 255;
    fim[x][y] = nw;
    int err = old - nw;

    if (x + 1 < sz) fim[x + 1][y] += err * 7 / 16;
    if (x - 1 >= 0 && y + 1 < sz) fim[x - 1][y + 1] += err * 3 / 16;
    if (y + 1 < sz) fim[x][y + 1] += err * 5 / 16;
    if (x + 1 < sz && y + 1 < sz) fim[x + 1][y + 1] += err * 1 / 16;
    dithered.set_pixel(x, y, fim[x][y], fim[x][y], fim[x][y]);
  }

  dithered.save_image("dithered.bmp");

  /* draw */

  forn(y,100) forn(x,100){
      if(GetKeyState('E') & 0x100) return 0;

      int c = round(fim[x][y] / 255) * 255;
      if (c) continue;

      SetCursorPos(2*x + tl.x, y0 + 2*y + tl.y);
      mouse_event(MOUSEEVENTF_LEFTDOWN, 2*x + tl.x, y0 + 2*y + tl.y, 0, 0);
      mouse_event(MOUSEEVENTF_LEFTUP, 2*x + tl.x, y0 + 2*y + tl.y, 0, 0);
    }
}
