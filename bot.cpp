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
  /* input canvas */

  POINT tl, br;

  cout << "top left? (press Q to set)\n";
  wait();
  GetCursorPos(&tl);
  cout << "bottom right? (press Q to set)\n";
  Sleep(300);
  wait();
  GetCursorPos(&br);

  int w = br.x - tl.x;
  int h = br.y - tl.y;
  cout << "canvas: " << w << " " << h << "\n";

  /* input image */

  bitmap_image image("img.bmp");
  if (!image){
    cout << "make sure there's an image named img.bmp in this folder\n";
    return 0;
  }
  int iw = image.width(), ih = image.height();
  cout << "image: " << iw << " " << ih << "\n";

  /* scale image + greyscale it */

  int rh = 1.0 * ih / iw * w;
  int rw = w;

  if (rh > h) {
    rh = h;
    rw = 1.0 * iw / ih * h;
  }

  cout << "rh, rw = " << rh << ", " << rw << "\n";

  int y0 = h / 2. - rw / 2.;
  int scy = max(1, rh / sz);
  int scx = max(1, rw / sz);

  vector<vector<double>> im(rw, vector<double>(rh));
  forn(y, rh) forn(x, rw) {

    double sr = 0, sg = 0, sb = 0, cnt = 0;
    rgb_t c;
    for(int i = 1.0 * x * iw / rw; i < 1.0 * (x + 1) * iw / rw; i++)
      for(int j = 1.0 * y * ih / rh; j < 1.0 * (y + 1) * ih / rh; j++) {
        image.get_pixel(i, j, c);
        sr += c.red;
        sg += c.green;
        sb += c.blue;
        cnt++;
      }

    sr /= cnt, sg /= cnt, sb /= cnt;
    im[x][y] = 0.21 * sr + 0.72 * sg + 0.07 * sb;
  }

  double dx = 1.0 * rw / sz;
  double dy = 1.0 * rh / sz;

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
  }

  /* dither */
  bitmap_image dithered(sz, sz);

  forn(y, sz) forn(x, sz) {

    double old = fim[x][y];
    double nrw = round(old / 255) * 255;
    fim[x][y] = nrw;
    int err = old - nrw;

    if (x + 1 < sz) fim[x + 1][y] += err * 7 / 16;
    if (x - 1 >= 0 && y + 1 < sz) fim[x - 1][y + 1] += err * 3 / 16;
    if (y + 1 < sz) fim[x][y + 1] += err * 5 / 16;
    if (x + 1 < sz && y + 1 < sz) fim[x + 1][y + 1] += err * 1 / 16;
    dithered.set_pixel(x, y, fim[x][y], fim[x][y], fim[x][y]);
  }

  dithered.save_image("dithered.bmp");

  /* draw */

  forn(y, sz) forn(x, sz) {
    if(GetKeyState('E') & 0x100) return 0;

    int c = round(fim[x][y] / 255) * 255;
    if (c) continue;

    SetCursorPos(x * (2 * rw / sz) / 2 + tl.x, y0 + y * (2 * rh / sz) / 2 + tl.y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, x * (2 * rw / sz) / 2 + tl.x, y0 + y * (2 * rh / sz) / 2 + tl.y, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, x * (2 * rw / sz) / 2 + tl.x, y0 + y * (2 * rh / sz) / 2 + tl.y, 0, 0);
  }

}
