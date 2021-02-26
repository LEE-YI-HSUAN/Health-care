// 1072019.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <time.h>
using namespace std;

//icd
class linkedList_icd;
//icd node
class listNode_icd {
public:
	string icd;
	string dateS;
	string dateE;

	listNode_icd* next;
	listNode_icd() :icd("0"), dateS("0"), dateE("0"), next(0) {};
	listNode_icd(string a, string b, string c) :icd(a), dateS(b), dateE(c), next(0) {};
};

class linkedList_icd {
private:
	listNode_icd* first;
public:
	linkedList_icd() :first(0) {};

	//從後面放入
	void Push_Back(string x, string y, string k) {
		listNode_icd* newNode = new listNode_icd(x, y, k);
		if (first == 0) {
			first = newNode;
			return;
		}

		listNode_icd* current = first;
		while (current->next != 0) {
			current = current->next;
		}

		current->next = newNode;
	}
	//出院空白日期
	void blank_Date() {
		listNode_icd* current = first;
		string month[13] = { "0","31","28","31","30","31","30","31","31","30","31","30","31" };

		for (; current != 0; current = current->next) {
			int sy = 0, ey = 0, sm = 0, em = 0;
			if (current->dateE == "") {
				for (int i = 0; i < current->dateS.length(); i++) {
					if (i == 6)
						break;
					current->dateE += current->dateS[i];
					if (i < 4) {
						sy *= 10; sy += current->dateS[i] - '0';
						ey *= 10; ey += current->dateE[i] - '0';
					}
					else if (i >= 4 && i < 6) {
						sm *= 10; sm += current->dateS[i] - '0';
						em *= 10; em += current->dateE[i] - '0';
					}

				}
				if ((sy % 4 == 0) && (sy % 100 != 0) || sy % 400 == 0)
					month[2] = "29";
				else
					month[2] = "28";
				current->dateE += month[sm];
			}

		}
	}
	//合併日期
	void merge() {
		listNode_icd* current = first;
		listNode_icd* temp = first;
		for (; current != 0; current = current->next) {
			for (temp = current->next; temp != 0; temp = temp->next) {
				//相同icd下dataS較小的在最前面
				if (temp->icd == current->icd && current->dateS > temp->dateS) {
					string t = temp->dateS;
					temp->dateS = current->dateS;
					current->dateS = t;
					t = temp->dateE;
					temp->dateE = current->dateE;
					current->dateE = t;
				}
				if ((temp->icd == current->icd && current->dateS <= temp->dateS) && temp->dateE <= current->dateE) {
					temp->dateE = current->dateE;
					temp->dateS = current->dateS;
				}
				else if (temp->icd == current->icd && temp->dateS <= current->dateE && temp->dateS >= current->dateS && temp->dateE >= current->dateE) {
					current->dateE = temp->dateE;
					temp->dateS = current->dateS;
				}
			}
		}
	}
	//刪除重複資料
	void d_icd() {
		listNode_icd* current = first;
		listNode_icd* temp = first;
		for (; current != 0; current = current->next) {
			listNode_icd* prev = current;
			for (temp = current->next; temp != 0; temp = temp->next) {
				if (temp->icd == current->icd && temp->dateS == current->dateS && temp->dateE == current->dateE) {
					prev->next = temp->next;
					delete temp;
					temp = prev;
				}
				else if (temp->icd == "") {
					prev->next = temp->next;
					delete temp;
					temp = prev;
				}
				else
					prev = prev->next;
			}
		}
	}
	//計算天數
	void cal_date(string id) {
		listNode_icd* current = first;

		int total = 0;
		for (; current != 0; current = current->next) {
			int month[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
			int sy = 0, ey = 0, sm = 0, em = 0, sd = 0, ed = 0;
			for (int i = 0; i < current->dateS.length(); i++) {
				if (i < 4) {
					sy *= 10; sy += current->dateS[i] - '0';
					ey *= 10; ey += current->dateE[i] - '0';
				}
				else if (i >= 4 && i < 6) {
					sm *= 10; sm += current->dateS[i] - '0';
					em *= 10; em += current->dateE[i] - '0';
				}
				else if (i >= 6 && i < current->dateS.length()) {
					sd *= 10; sd += current->dateS[i] - '0';
					ed *= 10; ed += current->dateE[i] - '0';
				}
			}
			//同一年
			if (sy == ey) {
				//閏年
				if ((sy % 4 == 0) && (sy % 100 != 0) || sy % 400 == 0)
					month[2] = 29;
				if (sm == em)
					total += ed - sd + 1;
				else {
					total += month[sm] - sd + 1;
					for (sm += 1; sm < em; sm++)
						total += month[sm];
					total += ed;
				}
			}
			//不同年
			else {
				if ((sy % 4 == 0) && (sy % 100 != 0) || sy % 400 == 0)
					month[2] = 29;
				int m = 12, d = 31;
				if (sm == m)
					total += d - sd + 1;
				else {
					total += month[sm] - sd + 1;
					for (sm += 1; sm < m; sm++)
						total += month[sm];
					total += d;
				}
				sy++;
				for (; sy < ey; sy++) {
					if ((sy % 4 == 0) && (sy % 100 != 0) || sy % 400 == 0)
						total += 366;
					else
						total += 365;
				}
				if ((sy % 4 == 0) && (sy % 100 != 0) || sy % 400 == 0)
					month[2] = 29;
				else
					month[2] = 28;
				m = 1, d = 1;
				if (em == m)
					total += ed - d + 1;
				else {
					total += month[m] - d + 1;
					for (m += 1; m < em; m++)
						total += month[m];
					total += ed;
				}
			}
			if (current->next == 0) {
				cout << id << "," << current->icd << "," << total << endl;
				break;
			}
			if (current->next->icd != current->icd) {
				cout << id << "," << current->icd << "," << total << endl;
				total = 0;
			}
		}
	}
	friend class linkedList_id;

};
//id-------------------------------------------------------------------------------------------
class linkedList_id;
//id node
class listNode_id {
public:
	string id;
	linkedList_icd icd;
	listNode_id* next;
	listNode_id() :id("0"), icd(), next(0) {};
	listNode_id(string a, linkedList_icd b) :id(a), icd(b), next(0) {};
};

class linkedList_id {
public:
	listNode_id* first;
	linkedList_id() :first(0) {};
	//從後面放入
	void Push_Back(string x, linkedList_icd y) {
		listNode_id* newNode = new listNode_id(x, y);

		if (first == 0) {
			first = newNode;
			return;
		}

		listNode_id* current = first;
		while (current->next != 0) {
			current = current->next;
		}

		current->next = newNode;
	}
	//判斷id是否有已有資料
	bool find_id(string id) {
		listNode_id* current = first;
		while (current != 0) {
			if (current->id == id)
				return true;
			current = current->next;
		}
		return false;
	}
	//將icd加到對應的id下
	void add_icd(string id, linkedList_icd icd) {
		listNode_id* current = first;
		listNode_icd* current_icd = icd.first;
		while (current != 0 && current_icd != 0) {
			if (current->id == id) {
				current->icd.Push_Back(current_icd->icd, current_icd->dateS, current_icd->dateE);
				current_icd = current_icd->next;
			}
			else
				current = current->next;
		}
	}
	//id大小
	int id_size() {
		listNode_id* s = first;
		int num = 0;
		while (s != 0) {
			num++;
			s = s->next;
		}
		return num;
	}
	//icd大小
	int icd_size(listNode_id* a) {
		listNode_icd* s = a->icd.first;
		int num = 0;
		while (s != 0) {
			num++;
			s = s->next;
		}
		return num;
	}
	//進行id排序
	void sort_id() {
		listNode_id* current = first;
		listNode_id* prev = first;
		listNode_id* temp = first;
		for (int i = id_size(); i > 0; i--) {
			current = first;
			prev = first;
			for (int j = 0; j < i - 1 && current->next; j++) {
				if (current->id > current->next->id) {
					temp = current->next;
					current->next = temp->next;
					temp->next = current;
					if (current == first) {
						first = temp;
						prev = temp;
					}
					else {
						prev->next = temp;
						prev = prev->next;
					}
				}
				else {
					current = current->next;
					if (j != 0) prev = prev->next;
				}
			}
		}
	}
	//進行icd排序
	void sort_icd() {
		listNode_id* a = first;
		for (int n = id_size(); n > 0; n--) {
			listNode_icd* current = a->icd.first;
			listNode_icd* prev = a->icd.first;
			listNode_icd* temp = a->icd.first;
			for (int i = icd_size(a); i > 0; i--) {
				current = a->icd.first;
				prev = a->icd.first;
				for (int j = 0; j < i - 1 && current->next; j++) {
					if (current->icd > current->next->icd) {
						temp = current->next;
						current->next = temp->next;
						temp->next = current;
						if (current == a->icd.first) {
							a->icd.first = temp;
							prev = temp;
						}
						else {
							prev->next = temp;
							prev = prev->next;
						}
					}
					else {
						current = current->next;
						if (j != 0) prev = prev->next;
					}
				}
			}
			a = a->next;
		}
	}
};
//----------------------------------------------------------------
//儲存現在讀取的這筆資料
class one_infor {
	string id;
	string dateS;
	string dateE;
	vector <string> icd;
public:
	//判斷應該存到哪一資料中
	void judgment_data(int index, string data) {
		if (index == 1)
			set_id(data);
		else if (index == 2)
			set_dateS(data);
		else if (index == 3)
			set_dateE(data);
		else {
			if (data == "")
				return;
			else
				set_icd(data);
		}
	}
	void set_id(string id) {
		if (id != " ")
			this->id = id;
	}
	void set_dateS(string date) {
		dateS = date;
	}
	void set_dateE(string date) {
		if (date[0] >= 65 && date[0] <= 90)
			set_icd(date);
		else
			dateE = date;
	}
	void set_icd(string icd) {
		this->icd.push_back(icd);
	}
	string get_id() {
		return id;
	}
	vector <string> get_icd() {
		return icd;
	}
	string get_dateS() {
		return dateS;
	}
	string get_dateE() {
		return dateE;
	}
};
//--------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	ifstream fin;
	char* filename = argv[1];
	fin.open(filename);

	linkedList_id all_ID;
	string str;
	//讀資料
	while (getline(fin, str)) {
		one_infor now;
		linkedList_icd icd;

		string s = "";
		int index = 0;
		str += ",";
		for (int i = 0; i < str.length(); i++) {
			if (str[i] != ',')
				s += str[i];
			else {
				index++;
				now.judgment_data(index, s);
				s = "";
			}
		}
		//將每個病例、病人對應分別放好
		for (int i = 0; i < now.get_icd().size(); i++)
			icd.Push_Back(now.get_icd()[i], now.get_dateS(), now.get_dateE());

		//判斷id是否已經有過病例，有就跳過，沒有就創造
		if (!all_ID.find_id(now.get_id()))
			all_ID.Push_Back(now.get_id(), icd);
		else
			all_ID.add_icd(now.get_id(), icd);
	}
	//資料排序+		dateE	""	std::string

	all_ID.sort_id();
	all_ID.sort_icd();

	listNode_id* temp = all_ID.first;
	//空白日期
	for (; temp != 0; temp = temp->next)
		temp->icd.blank_Date();
	//合併資料
	temp = all_ID.first;
	for (; temp != 0; temp = temp->next)
		temp->icd.merge();
	//刪除重複資料
	temp = all_ID.first;
	for (; temp != 0; temp = temp->next)
		temp->icd.d_icd();
	//計算天數
	temp = all_ID.first;
	for (; temp != 0; temp = temp->next)
		temp->icd.cal_date(temp->id);
	//system("pause");
	return 0;
}

// 執行程式: Ctrl + F5 或 [偵錯] > [啟動但不偵錯] 功能表
// 偵錯程式: F5 或 [偵錯] > [啟動偵錯] 功能表

// 開始使用的提示: 
//   1. 使用 [方案總管] 視窗，新增/管理檔案
//   2. 使用 [Team Explorer] 視窗，連線到原始檔控制
//   3. 使用 [輸出] 視窗，參閱組建輸出與其他訊息
//   4. 使用 [錯誤清單] 視窗，檢視錯誤
//   5. 前往 [專案] > [新增項目]，建立新的程式碼檔案，或是前往 [專案] > [新增現有項目]，將現有程式碼檔案新增至專案
//   6. 之後要再次開啟此專案時，請前往 [檔案] > [開啟] > [專案]，然後選取 .sln 檔案
