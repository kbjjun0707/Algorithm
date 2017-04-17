#include <vector>
#include <algorithm>

#include <iostream>
using namespace std;

struct Point2f {
	float x, y;
	Point2f(const Point2f &src) {
		this->x = src.x; this->y = src.y;
	}
	Point2f(float x = 0, float y = 0) {
		this->x = x; this->y = y;
	}
	Point2f operator-(const Point2f &b) {
		return Point2f(this->x - b.x, this->y - b.y);
	}
	void operator=(const Point2f &b) {
		this->x = b.x; this->y = b.y;
	}
};

bool LeftBottomComp(const Point2f &a, const Point2f &b) {
	if (a.x == b.x)
		return a.y < b.y;
	return a.x < b.x;
}

Point2f g_stdPt;
bool AngleComp(const Point2f &a, const Point2f &b) {
	if (((a.y - g_stdPt.y) / (a.x - g_stdPt.x)) == ((b.y - g_stdPt.y) / (b.x - g_stdPt.x))) {
		return (((a.y - g_stdPt.y)*(a.y - g_stdPt.y) + (a.x - g_stdPt.x)*(a.x - g_stdPt.x)) <
			((b.y - g_stdPt.y)*(b.y - g_stdPt.y) + (b.x - g_stdPt.x)*(b.x - g_stdPt.x)));
	}
	return (((a.y - g_stdPt.y) / (a.x - g_stdPt.x)) < ((b.y - g_stdPt.y) / (b.x - g_stdPt.x)));
}

// u, v는 벡터
int det(const Point2f &u, const Point2f &v) {
	return ((u.x * v.y) - (u.y * v.x));
}

// det < 0 이면 ccw, det > 0 cw, det == 0 평행
int det(Point2f &a, Point2f &b, Point2f &c) {
	return det(Point2f(b - a), Point2f(c - b));
}

void convexHull(const std::vector<Point2f> &p_Datas, std::vector<Point2f> &dst, std::vector<Point2f> &noncoverage = std::vector<Point2f>()) {
	dst.clear();
	noncoverage.clear();
	int size = p_Datas.size();
	if (size <= 3) {
		for (auto it : p_Datas)
			dst.push_back(it);
		return;
	}

	std::vector<Point2f> Datas;
	for (auto it : p_Datas)
		Datas.push_back(it);

	std::sort(Datas.begin(), Datas.end(), LeftBottomComp);
	g_stdPt = Datas[0];		// 각도를 구할기준점 설정
	std::sort(Datas.begin()+1, Datas.end(), AngleComp);

	Point2f p1 = Datas[0], p2 = Datas[1], p3;
	dst.push_back(p1);
	dst.push_back(p2);

	// i == 정렬된 배열의 인덱스
	// top == convexhull로 판단, 저장하는 배열의 맨 위 인덱스
	int i = 2, top = 1;
	while (i < size) {
		p1 = dst[top - 1];
		p2 = dst[top - 0];
		p3 = Datas[i];
		if (det(p1, p2, p3) >= 0) {
			dst.push_back(p3);
			i++;
			top++;
		} else {
			noncoverage.push_back(dst.back());
			dst.pop_back();
			top--;
		}
	}
}

int main() {
	FILE *fp;
	fopen_s(&fp, "input.txt", "r");

	int num = 0;
	float a, b;
	fscanf_s(fp, "%d", &num);

	std::vector<Point2f> datas;		// 점들 저장소
	for (int i = 0; i < num; i++) {
		fscanf_s(fp, "%f %f", &a, &b);
		datas.push_back(Point2f(a, b));
	}
	fclose(fp);


	std::vector<Point2f> convexhullPoints;
	convexHull(datas, convexhullPoints);
	
	for (auto it : convexhullPoints) {
		cout << it.x << ", " << it.y << endl;
	}


	return 0;
}