#include "map.h"
#include <cstdlib>
#include <algorithm>



//**************MAP Class*************

Map::Map(){}
Map::Map(ifstream& file){
	file >> row >> col;
	map = new string[row];
	for (int i = 0; i < row; i++){
		file >> map[i];
	}
}

//���� ����ϴ� �Լ�
void Map::printMap(){
	for (int i = 0; i < row; i++){
		cout << map[i] << endl;
	}
}




//�������� ã�� string�� �ʿ� ����ִ� �Լ�
void Map::putStr(Quiz& q, string str){
	int r = q.row, c = q.col;
	if (q.horizon){
		for (int i = 0; i < q.len; i++){
			map[r][c + i] = str[i];
		}
	}
	else{
		for (int i = 0; i < q.len; i++){
			map[r + i][c] = str[i];
		}
	}
}

//��� �ش��ϴ� ���� ��Ʈ���� �ʿ��� ����� �Լ�
string Map::makeStr(Quiz& q){
	string str;
	int r = q.row, c = q.col;
	if (q.horizon){
		for (int i = 0; i < q.len; i++){
			str += map[r][c + i];
		}
	}
	else{
		for (int i = 0; i < q.len; i++){
			str += map[r + i][c];
		}
	}

	return str;
}


//**************QUIZ Class*************

Quiz::Quiz(){}
Quiz::Quiz(int r, int c, bool hori){
	row = r - 1;
	col = c - 1;
	len = 0;
	fill = false;
	horizon = hori;
}

//�� ������ ���̸� ���ϴ� �Լ�
void Quiz::setLen(const Map& m){
	int idx = 0;
	if (horizon){
		idx = col;
		while (m.map[row][idx] != '*'){
			len++, idx++;
			if (idx == m.col) break;
		}
	}
	else{
		idx = row;
		while (m.map[idx][col] != '*'){
			len++, idx++;
			if (idx == m.row) break;
		}
	}

	//cout << row << ' ' << col << ' ' << horizon << ' ' << len << endl;
}

//�������� ã�� �ܾ �ʿ� �� �� �ִ����� Ȯ��
bool Quiz::matchStr(string origin, string str){
	bool match = true;
	if (horizon){
		for (int i = 0; i < str.length(); i++){
			//�� ���� ĭ�� ��ĭ(_)�� �ƴϰ� �ִ� ���ڿ� ������ �ʴٸ� Ʋ�� �ܾ�
			if (origin[i] != '_' && origin[i] != str[i]){
				match = false;
				break;
			}
		}
	}
	else{
		for (int i = 0; i < str.length(); i++){
			//�� ���� ĭ�� ��ĭ(_)�� �ƴϰ� �ִ� ���ڿ� ������ �ʴٸ� Ʋ�� �ܾ�
			if (origin[i] != '_' && origin[i] != str[i]){
				match = false;
				break;
			}
		}
	}
	return match;
}

//**************QUIZSET Class*************

QuizSet::QuizSet(){}
QuizSet::QuizSet(ifstream& file, const Map& m, int h, int v){
	hori = h;
	vert = v;
	quizSet = new Quiz[hori + vert];
	int r, c;
	for (int i = 0; i < hori + vert; i++){
		file >> r >> c;
		quizSet[i] = (i < hori ? Quiz(r, c, true) : Quiz(r, c, false));
		quizSet[i].setLen(m);
	}
	qsort(quizSet, hori + vert, sizeof(Quiz), cmp);
	setConnected();
	
}

//��� ��� Ǯ�������� üũ�ϴ� �Լ�
bool QuizSet::quizComplete(){
	bool check = true;
	for (int i = 0; i < hori + vert; i++){
		if (!(check &= quizSet[i].fill))
			break;
	}
	return check;
}

//�������� Ǯ ��� �����ϴ� �Լ�
int QuizSet::selectNextQuiz(int index){
	int idx = -1, t;
	//����� ���� �߿��� ���� ���� ����
	for (int i = 0; i < quizSet[index].connected.size(); i++){
		t = quizSet[index].connected[i];
		if (!quizSet[t].fill){
			idx = t;
			break;
		}
	}
	//����� ��� ������ Ǯ�ȴٸ� ��ü ���� �� ���� ���̰� �� ���� ����
	if (idx == -1){
		for (int i = 0; i < hori + vert; i++){
			if (!quizSet[i].fill){
				idx = i;
				break;
			}
		}
	}
	return idx;
}

//���� ������� ��̸�ũ�� ����� �Լ� 
void QuizSet::setConnected(){
	for (int i = 0; i < hori + vert; i++){
		int r = quizSet[i].row, c = quizSet[i].col, l = quizSet[i].len;
		if (quizSet[i].horizon){
			for (int j = 0; j < hori + vert; j++){
				if (!quizSet[j].horizon && quizSet[j].row <= r && r <= quizSet[j].row + quizSet[j].len && c <= quizSet[j].col && quizSet[j].col <= c + l)
					quizSet[i].connected.push_back(j);
			}
		}
		else{
			for (int j = 0; j < hori + vert; j++){
				if (quizSet[j].horizon && quizSet[j].col <= c && c <= quizSet[j].col + quizSet[j].len && r <= quizSet[j].row && quizSet[j].row <= r + l)
					quizSet[i].connected.push_back(j);
			}
		}
		//sort(quizSet[i].connected.begin(), quizSet[i].connected.end(), cmp);
	}
}

int cmp(const void* a, const void* b){
	Quiz* qa = (Quiz*)a;
	Quiz* qb = (Quiz*)b;
	if (qa->len > qb->len)
		return -1;
	else if (qa->len < qb->len)
		return 1;
	else
		return 0;
}
