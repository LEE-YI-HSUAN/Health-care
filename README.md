# 資料結構作業-健保整理
## 主要目的：
計算個別病患與各疾病(ICD)的住院總天數
## 說明：
健保資料庫住院檔記錄著病患的 ID 代碼、生日、性別、住院日期、出院日期、 ICD 等等。所謂 ICD 即是國際疾病傷害及死因分類標準，是世界衛生組織依據疾病的某些特徵，依照規則將疾病分門別類，並用編碼的方法來表示的系統。在做某類疾病患者的住院天數分析時，可利用住院檔的資料進行統計。 
## 特別狀況：
在ICD相同時，住院日期可能會有交錯、包含的可能性。 
- 交錯(此例為 45 天) 
:::info
C,20200601,20200630 C,20200615,20200715
:::
- 包含於(此例為 31 天) 
:::info
D,20200801,20200831 
D,20200810,20200820 
:::
### 範例：
- ID 為 b2bf3ca6546d 病人從 20081213 入住醫院並於 20090105 出院，並且ICD 碼為 1889 與 V1052 兩項，此紀錄住院天數為 24 日。 

## 輸入輸出說明：
### 輸入格式：
- 欄位依序【病患 ID、住院日期、出院日期、ICD_1、ICD_2、ICD_3】
- 病患ID：字串
- 住院和出院日期格式：YYYYMMDD
- ICD_1、ICD_2、ICD_3 為三項當次住院醫生診斷的 ICD 碼
- ICD 碼可以為空值，但每筆紀錄一定會有一項 ICD 碼。 
	- 舉例： `A1,,` 
- 出院日期可能為空值，如遇到出院日期為空值時，則假設出院日期為當月最後一天。 
	- 舉例：`id,20200513,,A1,,`
	- 代表該病患入院日期為 20200513,出院日期為 20200531 

### 輸出格式：
- 依序為【病患 ID、ICD、總天數】
- 依照各病患 ID、ICD 來升冪排序輸出(以 string 判斷大小方式)

## 範例：
- Input: 
	- `ah18n4sj1g7r,20070501,20070515,V440,,`
	- `ah18n4sj1g7r,20070516,20070518,V440,25000,`
	- `b2bf3ca6546d,20081213,20090105,1889,V1052,` 
- Output: (STDOUT) 
	- ah18n4sj1g7r,25000,3 
	- ah18n4sj1g7r,V440,18
	- b2bf3ca6546d,1889,24 
	- b2bf3ca6546d,V1052,24
## 程式實作：
自己寫link list 
### 架構
- id：
```cpp=
class listNode_id {
public:
  string id;
  linkedList_icd icd;
  listNode_id* next;
  listNode_id() :id("0"), icd(), next(0) {};
  listNode_id(string a, linkedList_icd b) :id(a), icd(b), next(0)  {};
};

class linkedList_id {
public:
	listNode_id* first;
	linkedList_id() :first(0) {};
	//從後面放入
	void Push_Back(string x, linkedList_icd y);
	//判斷傳入id是否有已有資料，有回傳true，沒有false
	bool find_id(string id);
	//將icd加到對應的id下
	void add_icd(string id, linkedList_icd icd);
	//回傳id數量
	int id_size();
	//回傳icd數量
	int icd_size(listNode_id* a);
	//進行id排序
	void sort_id();
	//進行icd排序
	void sort_icd();
};
```

- ICD：
```cpp=
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
	void Push_Back(string x, string y, string k);
	//出院空白日期
	void blank_Date();
	//合併日期
	void merge();
	//刪除重複資料
	void d_icd();
	//計算天數
	void cal_date(string id);
	
	friend class linkedList_id;
};
```
