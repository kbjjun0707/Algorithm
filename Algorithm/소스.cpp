#include <iostream>
#include <queue>
#include <vector>
#include <ctime>
#include <fstream>
using namespace std;

class Point {
public:
	Point(int x = 0, int y = 0) {
		this->x = x;
		this->y = y;
	}
	int x, y;
	Point operator+(const Point &a) {
		return Point(x + a.x, y + a.y);
	}
};

class BFS {
	int m_DirNum;
	Point m_DirCandidate[8] = {
		{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1,1}, {-1, 0}, {-1,-1}
	};
	Point *m_Dir = 0;
	Point m_Size;
	Point m_Start, m_End;

	std::queue<Point> m_Que;

	int **m_Map = 0;

	bool check(const Point &p1, const Point &p2) {
		if ((p2.x < 0 || p2.x >= m_Size.x || p2.y < 0 || p2.y >= m_Size.y) || (m_Map[p2.y][p2.x] == -1))
			return false;
		if ((m_Map[p2.y][p2.x] == 0) || (m_Map[p2.y][p2.x] >(m_Map[p1.y][p1.x] + 1))) {
			return true;
		}
		return false;
	}

	void search(Point &p) {
		for (int i = 0; i < 4; i++) {
			Point nextPoint = p + m_Dir[i];
			if (check(p, nextPoint)) {
				m_Map[nextPoint.y][nextPoint.x] = (m_Map[p.y][p.x] + 1);
				m_Que.push(nextPoint);
			}
		}
	}

public:
	/*
	@param row		미로의 세로 크기
	@param col		미로의 가로 크기
	@param dir		탐색 방향의 수 4(십자 칸) or 8(이웃한 모든 칸)
	@parma datas	미로 맵의 정보 ( (default nullptr)
	@parma sx sy	시작 점 (default 0, 0)
	@parma ex ex	도착 점 (default 0, 0)
	*/
	BFS(const int row, const int col, const int dir, int **datas = nullptr, const int sx = 0, const int sy = 0, const int ex = 0, const int ey = 0) {
		m_Size = { col, row };
		m_DirNum = dir;
		m_Dir = new Point[m_DirNum];
		for (int i = 0, ti = 0; i < m_DirNum; i++, ti += (8/m_DirNum)) {
			m_Dir[i] = m_DirCandidate[ti];
		}
		setStartPoint(sx, sy);
		setEndPoint(ex, ey);

		m_Map = new int*[row] { 0 };
		for (int i = 0; i < row; i++)
			m_Map[i] = new int[col] { 0 };

		if (datas != nullptr)
			setMap(datas);
	}

	~BFS() {
		for (int i = 0; i < (m_Size.y); i++)
			delete[] m_Map[i];
		delete[] m_Map;
		delete[] m_Dir;
	}

	void setStartPoint(const int x, const int y) {
		m_Start = { x,y };
	}

	void setEndPoint(const int x, const int y) {
		m_End = { x,y };
	}

	void setMap(int **datas) {
		if (datas == nullptr) {
			cout << "datas null" << endl;
			return;
		}
		for (int r = 0; r < m_Size.y; r++) {
			for (int c = 0; c < m_Size.x; c++) {
				m_Map[r][c] = datas[r][c];
			}
		}
	}

	void run() {
		if (m_Map == nullptr) {
			cout << "No map." << endl;
			return;
		}

		clock_t startTime = clock();
		m_Map[m_Start.y][m_Start.x] = 1;
		m_Que.push(m_Start);
		while (!m_Que.empty()) {
			//// printer
			//{
			//	system("cls");
			//	for (int r = 0; r < m_Size.y; r++) {
			//		for (int c = 0; c < m_Size.x; c++) {
			//			printf("%02d ", m_Map[r][c]);
			//		}
			//		cout << endl;
			//	}
			//	getchar();
			//}

			Point tmp = m_Que.front(); m_Que.pop();
			search(tmp);
			if (m_Map[m_End.y][m_End.x] > 0) break;
		}
		clock_t endTime = clock();
		cout << "Run for " << (double)((endTime - startTime) / 1000.0) << " second." << endl;
	}

	int getAriivalCount() {
		return m_Map[m_End.y][m_End.x];
	}

	int resultPrint() {
		int res = getAriivalCount();
		if (res > 0) {
			cout << "Arrive at destination " << res << " moves." << endl;
		} else if (res == 0) {
			cout << "No way." << endl;
		}
		return res;
	}
};

int **Data;

int main() {
	int szX, szY, stX, stY, enX, enY;
	char buf[100];
	
	FILE *fp;
	fopen_s(&fp, "input.txt", "r");

	if (fp == nullptr) {
		cout << "file open fail" << endl;
		return 0;
	}

	fgets(buf, 99, fp);
	fscanf_s(fp, " %d %d %d %d %d %d", &szX, &szY, &stX, &stY, &enX, &enY);

	Data = new int*[szY] { 0 };
	for (int i = 0; i < szY; i++)
		Data[i] = new int[szX] { 0 };


	for (int r = 0; r < szY; r++) {
		for (int c = 0; c < szX; c++) {
			fscanf_s(fp, "%d", &Data[r][c]);
		}
	}
	fclose(fp);


	
	BFS finder(szY, szX, 4, Data);
	finder.setStartPoint(stX, stY);
	finder.setEndPoint(enX, enY);
	finder.run();
	finder.resultPrint();

	return 0;
}
