#include <iostream>
using namespace std;

class Cache { //**BasicCache** 2개의 정수를 검색하고 덧셈하는 프로그램

private:
	int HDD[5000],  //하드 디스크(5000*4바이트): 유일한 값 5000개 저장
		RAM[500] = { 0 },  //램(500*4바이트)
		L1[5] = { 0 },  //캐시1(5*4바이트)
		L2[20] = { 0 },  //캐시2(20*4바이트)
		L3[200] = { 0 },  //캐시3(200*4바이트)
		R1 = 0, //레지스터1(1*4바이트)
		R2 = 0, //레지스터2(1*4바이트)
		h = 0, r = 0, l1 = 0, l2 = 0, l3 = 0; //배열들 시작 인덱스
	bool check; //접근 성공 실패를 나타냄 
	bool isRegister1 = 1; //레지스터1이면 1, 레지스터2면 0
	double hit1 = 0, hit2 = 0, hit3 = 0; //캐시1,2,3 적중 횟수 
	double acc_time = 0; //접근 시간 

public:
	Cache() {
		for (int i = 0; i < 5000; i++) HDD[i] = i + 1;//HDD의 내용 1~5000
	}
	void reset_acc_time() {
		acc_time = 0;
	}
	bool check_cache1(int data) // 캐시1(L1)에 입력받은 데이터가 있는지 검사
	{
		for (int i = 0; i < 5; i++) {
			if (L1[i] == data) {
				check = true;
				cout << "!!Cache1 Hit!!" << endl;
				hit1++;
				break;
			}
			else check = false;
		}
		acc_time += 0.1;
		return check;
	}

	bool check_cache2(int data) // 캐시2(L2)에 입력받은 데이터가 있는지 검사
	{
		for (int i = 0; i < 20; i++) {
			if (L2[i] == data) {
				check = true;
				cout << "!!Cache2 Hit!!" << endl;
				hit2++;
				break;
			}
			else check = false;
		}
		acc_time += 0.1;
		return check;
	}

	bool check_cache3(int data) // 캐시3(L3)에 입력받은 데이터가 있는지 검사
	{
		for (int i = 0; i < 200; i++) {
			if (L3[i] == data) {
				check = true;
				cout << "!!Cache3 Hit!!" << endl;
				hit3++;
				break;
			}
			else check = false;
		}
		acc_time += 0.1;
		return check;
	}

	bool check_ram(int data) // 램에 입력받은 데이터가 있는지 검사
	{
		for (int i = 0; i < 500; i++) {
			if (RAM[i] == data) {
				check = true;
				cout << "data in RAM!" << endl;
				break;
			}
			else check = false;
		}
		acc_time += 1;
		return check;
	}

	bool check_hdd(int data) // 하드디스크에 입력받은 데이터가 있는지 검사
	{
		for (int i = 0; i < 5000; i++) {
			if (HDD[i] == data) {
				check = true;
				cout << "Data in Hdd!" << endl;
				break;
			}
		}
		acc_time += 3;
		return check;
	}


	//********레지스터에 넣을 데이터 찾기********

	void in_cache1(int data, bool isRegister1)
	{
		if (check_cache1(data)) { //캐시1에 데이터가 있으면 가져와서 레지스터에 저장
			if (isRegister1)
				R1 = data;
			else
				R2 = data;
			//캐시 적중
		}
		else in_cache2(data, isRegister1); //캐시1에 데이터가 없으면 캐시2를 검사하여 캐시에 저장하는 함수 호출
	}

	void in_cache2(int data, bool isRegister1)
	{
		if (check_cache2(data)) { //캐시2에 데이터가 있으면 가져와서 레지스터에 저장
			if (l1 >= 5)
				l1 = 0;
			L1[l1] = data;

			if (isRegister1)
				R1 = data;
			else
				R2 = data;

			l1++;
			//캐시 적중
		}
		else in_cache3(data, isRegister1); //캐시2에 데이터가 없으면 캐시3을 검사하여 캐시에 저장하는 함수 호출
	}


	void in_cache3(int data, bool isRegister1)
	{
		if (check_cache3(data)) { //캐시3에 데이터가 있으면 가져와서 레지스터에 저장
			if (l1 >= 5)
				l1 = 0;
			L1[l1] = data;

			if (l2 >= 20)
				l2 = 0;
			L2[l2] = data;

			if (isRegister1)
				R1 = data;
			else
				R2 = data;

			l1++; l2++;
			//캐시 적중
		}
		else in_ram(data, isRegister1); //캐시3에 데이터가 없으면 RAM을 검사하여 RAM에 저장하는 함수 호출
	}

	void in_ram(int data, bool isRegister1)
	{
		if (check_ram(data)) { //램을 검사해서 데이터가 있으면 가져와서 캐시에 저장 -> 레지스터에 저장
			if (l1 >= 5)
				l1 = 0;
			L1[l1] = data;

			if (l2 >= 20)
				l2 = 0;
			L2[l2] = data;

			if (l3 >= 200)
				l3 = 0;
			L3[l3] = data;

			if (isRegister1)
				R1 = data;
			else
				R2 = data;

			l1++; l2++; l3++;
		}
		else in_hdd(data, isRegister1); //램에 데이터가 없으면 하드디스크를 검사하여 램에 저장하는 함수 호출
	}

	void in_hdd(int data, bool isRegister1)
	{
		if (check_hdd(data)) { //하드디스크를 검사해서 데이터가 있으면 가져와서 램에 저장 -> 캐쉬에 저장 -> 레지스터에 저장
			if (r >= 500) {
				r = 0;

				if (l1 >= 5) { // l1 인덱스 초과
					l1 = 0;

					if (l2 >= 20) { //l2 인덱스 초과
						l2 = 0;

						if (l3 >= 200) // l3 인덱스 초과
							l3 = 0;
					}
					else if (l2 < 20) { //l2 인덱스 초과 아님
						if (l3 >= 200) // l3 인덱스 초과
							l3 = 0;
					}
				}

				else if (l1 < 5) { // l1 인덱스 초과 아님
					if (l2 >= 20) { // l2 인덱스 초과
						l2 = 0;

						if (l3 >= 200) //l3 인덱스 초과
							l3 = 0;
					}
					else if (l2 < 20) { //l2 인덱스 초과 아님
						if (l3 >= 200) //l3 인덱스 초과
							l3 = 0;
					}
				}
			}
			else if (r < 500) {
				if (l1 >= 5) { // l1 인덱스 초과
					l1 = 0;

					if (l2 >= 20) { //l2 인덱스 초과
						l2 = 0;

						if (l3 >= 200) // l3 인덱스 초과
							l3 = 0;
					}
					else if (l2 < 19) { //l2 인덱스 초과 아님
						if (l3 >= 200) // l3 인덱스 초과
							l3 = 0;
					}
				}

				else if (l1 < 5) { // l1 인덱스 초과 아님
					if (l2 >= 20) { // l2 인덱스 초과
						l2 = 0;

						if (l3 >= 200) //l3 인덱스 초과
							l3 = 0;
					}
					else if (l2 < 20) { //l2 인덱스 초과 아님
						if (l3 >= 200) //l3 인덱스 초과
							l3 = 0;
					}
				}
			}

			RAM[r] = data;
			L1[l1] = data;
			L2[l2] = data;
			L3[l3] = data;

			if (isRegister1)
				R1 = data;
			else
				R2 = data;

			r++; l1++; l2++; l3++;
			// 캐시실패, 메모리 실패
		}
		else error(); //하드디스크에도 원하는 데이터가 없으면 오류 문구 출력
	}


	void error()
	{
		cout << "**접근 실패! 데이터 존재하지 않음!**" << endl;
		exit(0);//종료
	}

	void show_hit(double count)
	{
		cout << "전체 메모리의 참조 횟수: " << count << endl;
		cout << "캐시 메모리1의 적중 횟수: " << hit1 << endl;
		cout << "캐시 메모리1 Hit율: " << (hit1 / count) * 100 << "%" << endl; // 히트율 = (적중 횟수)/(참조 횟수) 
		cout << "캐시 메모리1 Miss율: " << 100 - (hit1 / count) * 100 << "%" << endl << endl;

		cout << "캐시 메모리2의 적중 횟수: " << hit2 << endl;
		cout << "캐시 메모리2 Hit율: " << (hit2 / count) * 100 << "%" << endl; // 히트율 = (적중 횟수)/(참조 횟수) 
		cout << "캐시 메모리1 Miss율: " << 100 - (hit2 / count) * 100 << "%" << endl << endl;

		cout << "캐시 메모리3의 적중 횟수: " << hit3 << endl;
		cout << "캐시 메모리3 Hit율: " << (hit3 / count) * 100 << "%" << endl; // 히트율 = (적중 횟수)/(참조 횟수) 
		cout << "캐시 메모리1 Miss율: " << 100 - (hit3 / count) * 100 << "%" << endl << endl;
	}

	void show()
	{
		/*cout << "<하드디스크>";
		for (int i = 0; i < 5000; i++)//HDD 값 출력
		{
			if (i % 16 == 0) cout << endl;
			cout << HDD[i] << " ";
		}*/
		cout << endl;
		cout << "<메인메모리>" << endl;
		for (int i = 0; i < 500; i++) cout << RAM[i] << " "; //RAM의 내용 출력 

		cout << endl;
		cout << "<캐시메모리3>" << endl;
		for (int i = 0; i < 200; i++) cout << L3[i] << " "; //Cache3의 내용 출력 

		cout << endl;
		cout << "<캐시메모리2>" << endl;
		for (int i = 0; i < 20; i++) cout << L2[i] << " "; //Cache2의 내용 출력 

		cout << endl;
		cout << "<캐시메모리1>" << endl;
		for (int i = 0; i < 5; i++) cout << L1[i] << " "; //Cache1의 내용 출력 

		cout << endl;
		cout << "<레지스터1>" << endl; //연산 결과값 출력 
		cout << R1 << endl;

		cout << "<레지스터2>" << endl; //연산 결과값 출력 
		cout << R2 << endl;

		cout << "데이터 접근 시간: " << acc_time << "초" << endl;
	}

	void show_add_result() {
		cout << "두 정수의 덧셈 결과는 " << R1 + R2 << " 이다." << endl;
	}
};

int main()
{
	Cache c1;
	int input1, input2, count = 0;

	while (true) {
		cout << "읽고싶은 첫번째 데이터를 입력하시오(1이상의 정수)" << endl;
		cin >> input1;

		cout << endl;
		count++;
		c1.reset_acc_time();
		c1.in_cache1(input1, 1);
		c1.show();
		c1.show_hit(count);

		cout << endl;


		cout << "읽고싶은 두번째 데이터를 입력하시오(1이상의 정수)" << endl;
		cin >> input2;

		cout << endl;
		count++;
		c1.reset_acc_time();
		c1.in_cache1(input2, 0);
		c1.show();
		c1.show_hit(count);

		cout << endl;

		c1.show_add_result();

		cout << endl;
	}

	system("pause");
	return 0;
}
