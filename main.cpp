#include <graphics.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI acos(-1)

struct CzfLine {
	int x1;
	int y1;
	int x2;
	int y2;
	char angle[30] = "";
	color_t color = EGEARGB(0xFF, 0x80, 0xFF, 0xFF);;
};

CzfLine line1;

struct CzfCircle {
	int x;
	int y;
	int r = 0;
};
CzfCircle circle1;


//mouse status
bool isMouseMoving = false;
bool isChoisen = false;
bool isZooming = false;
int xMouse, yMouse;
int xLast, yLast;
int xFirst, yFirst;

//redraw
bool isRedraw = false;

void setup() {
	initgraph(640, 480, INIT_RENDERMANUAL);
	setbkcolor(WHITE);
	setcolor(WHITE);
	setfont(20, 0, "楷体");
	setbkmode(TRANSPARENT);
	//开启抗锯齿
	ege_enable_aa(true);
	//xor方式栅格画图
	setwritemode(R2_XORPEN);
	
	HWND hwnd = getHWnd(); // 
	SetWindowText(hwnd, TEXT("鼠标画图"));
}

void show() {
	if (isRedraw) {
		cleardevice();
		setlinestyle(DOTTED_LINE, 0, 1);
		line(circle1.x, 0, circle1.x, 480);
		line(0, circle1.y, 640, circle1.y);

		setlinestyle(SOLID_LINE, 0, 2);
		circle(circle1.x, circle1.y, circle1.r);

		line(line1.x1, line1.y1, line1.x2, line1.y2);

		setcolor(BLUE);

		xyprintf(line1.x1, line1.y1, line1.angle);
		isRedraw = false;
	}
}
void update() {
	while (mousemsg()) {
		mouse_msg msg = getmouse();
		//鼠标左键点击位置记录
		if (msg.is_move() && (isMouseMoving || isChoisen || isZooming)) {
			if (isMouseMoving) {
				//根据基点和当前位置画线
				setfillcolor(WHITE);
				bar(xFirst + 1, yFirst + 1, xFirst + 130, yFirst + 50);
				float angle = atan2((msg.y - yFirst), (msg.x - xFirst));
				float theta = angle * (180 / PI);

				setcolor(YELLOW);
				setlinestyle(DOTTED_LINE, 0, 3);
				line(xFirst, yFirst, xLast, yLast);

				line(xFirst, yFirst, msg.x, msg.y);

				setcolor(LIGHTGREEN);
				setlinestyle(SOLID_LINE, 0, 2);
				line(xLast, yLast - 10, xLast, yLast + 10);
				line(xLast - 10, yLast, xLast + 10, yLast);
				line(msg.x, msg.y - 10, msg.x, msg.y + 10);
				line(msg.x - 10, msg.y, msg.x + 10, msg.y);
				setcolor(YELLOW);


				//根据基点和当前位置画圆
				int x1 = msg.x - xFirst;
				int y1 = msg.y - yFirst;
				int x2 = xLast - xFirst;
				int y2 = yLast - yFirst;
				double dr1 = sqrt(x1 * x1 + y1 * y1);
				double dr2 = sqrt(x2 * x2 + y2 * y2);

				sprintf(line1.angle, "%6.2f/%.f", theta, dr1);
				setcolor(BLUE);
				xyprintf(xFirst, yFirst, line1.angle);
				setcolor(YELLOW);
				setlinestyle(DOTTED_LINE, 0, 3);
				circle(xFirst, yFirst, dr2);
				circle(xFirst, yFirst, dr1);

				//操作后，改变基点
				xLast = msg.x;
				yLast = msg.y;
			}

			if (isChoisen) {
				setcolor(YELLOW);
				setlinestyle(DOTTED_LINE, 0, 3);
				line(circle1.x, circle1.y, xLast, yLast);
				line(circle1.x, circle1.y, msg.x, msg.y);

				circle(xLast, yLast, circle1.r);
				circle(msg.x, msg.y, circle1.r);

				//操作后，改变基点
				xLast = msg.x;
				yLast = msg.y;
			}

			if (isZooming) {
				setcolor(YELLOW);
				setlinestyle(SOLID_LINE, 0, 3);

				int x1 = msg.x - circle1.x;
				int y1 = msg.y - circle1.y;
				int x2 = xLast - circle1.x;
				int y2 = yLast - circle1.y;
				double dr1 = sqrt(x1 * x1 + y1 * y1);
				double dr2 = sqrt(x2 * x2 + y2 * y2);

				circle(circle1.x, circle1.y, dr2);
				circle(circle1.x, circle1.y, dr1);

				//操作后，改变基点
				xLast = msg.x;
				yLast = msg.y;
			}

		} else if (msg.is_left()) {
			if (msg.is_down()) {
				//设置基点为点击位置
				xMouse = msg.x;
				yMouse = msg.y;
				xLast = msg.x;
				yLast = msg.y;
				xFirst = msg.x;
				yFirst = msg.y;

				int dist = (xFirst - circle1.x) * (xFirst - circle1.x) + (yFirst - circle1.y) * (yFirst - circle1.y);
				if (dist < (circle1.r - 5) * (circle1.r - 5)) {
					setcolor(YELLOW);
					setlinestyle(DOTTED_LINE, 0, 3);
					line(circle1.x, circle1.y, xLast, yLast);
					circle(xFirst, yFirst, circle1.r);
					isChoisen = true;
					break;
				} else if (dist < (circle1.r + 5) * (circle1.r + 5)) {

					isZooming = true;
					break;
				} else {
					setcolor(LIGHTGREEN);
					setlinestyle(SOLID_LINE, 0, 2);
					line(xLast, yLast - 10, xLast, yLast + 10);
					line(xLast - 10, yLast, xLast + 10, yLast);

					isMouseMoving = true;
				}

			} else {
				if (isMouseMoving) {
					//鼠标按键抬起
					isMouseMoving = false;
					line1.x1 = xFirst;
					line1.y1 = yFirst;
					line1.x2 = msg.x;
					line1.y2 = msg.y;

					float angle = atan2((msg.y - yFirst), (msg.x - xFirst));
					float theta = angle * (180 / PI);
					sprintf(line1.angle, "%6.2f", theta);

					circle1.x = xFirst;
					circle1.y = yFirst;
					int x1 = msg.x - xFirst;
					int y1 = msg.y - yFirst;
					double dr = sqrt(x1 * x1 + y1 * y1);
					circle1.r = dr;

					isRedraw = true;
				}
				if (isChoisen) {
					isChoisen = false;

					int dx = msg.x - circle1.x;
					int dy = msg.y - circle1.y;

					circle1.x = msg.x;
					circle1.y = msg.y;

					line1.x1 = msg.x;
					line1.y1 = msg.y;
					line1.x2 = line1.x2 + dx;
					line1.y2 = line1.y2 + dy;

					isRedraw = true;
				}
				if (isZooming) {
					isZooming = false;
					int x1 = msg.x - circle1.x;
					int y1 = msg.y - circle1.y;

					double dr = sqrt(x1 * x1 + y1 * y1);
					double ddr =  dr - circle1.r;
					circle1.r = dr;

					float theta = atof(line1.angle);
					float angle = theta * PI / 180;

					line1.x2 = line1.x2 + ddr*cos(angle);
					line1.y2 = line1.y2 + ddr*sin(angle);
					isRedraw = true;
				}
			}
		}
	}
}
int main() {
	setup();
	for (; is_run(); delay_fps(60)) {
		show();
		update();
	}
	closegraph();
	return 0;
}
