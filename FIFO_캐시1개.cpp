#include <iostream>
using namespace std;

class Cache { //**BasicCache** 2���� ������ �˻��ϰ� �����ϴ� ���α׷�, ĳ��1��

private:
	int HDD[5000],  //�ϵ� ��ũ(5000*4����Ʈ): ������ �� 5000�� ����
		RAM[500] = { 0 },  //��(500*4����Ʈ)
		L1[5] = { 0 },  //ĳ��1(5*4����Ʈ)
		R1 = 0, //��������1(1*4����Ʈ)
		R2 = 0, //��������2(1*4����Ʈ)
		h = 0, r = 0, l1 = 0, l2 = 0, l3 = 0; //�迭�� ���� �ε���
	bool check; //���� ���� ���и� ��Ÿ�� 
	bool isRegister1 = 1; //��������1�̸� 1, ��������2�� 0
	double hit1 = 0, hit2 = 0, hit3 = 0; //ĳ��1,2,3 ���� Ƚ�� 
	double acc_time = 0; //���� �ð� 

public:
	Cache() {
		for (int i = 0; i < 5000; i++) HDD[i] = i + 1;//HDD�� ���� 1~5000
	}
	void reset_acc_time() {
		acc_time = 0;
	}
	bool check_cache1(int data) // ĳ��1(L1)�� �Է¹��� �����Ͱ� �ִ��� �˻�
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

	bool check_ram(int data) // ���� �Է¹��� �����Ͱ� �ִ��� �˻�
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

	bool check_hdd(int data) // �ϵ��ũ�� �Է¹��� �����Ͱ� �ִ��� �˻�
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


	//********�������Ϳ� ���� ������ ã��********

	void in_cache1(int data, bool isRegister1)
	{
		if (check_cache1(data)) { //ĳ�ÿ� �����Ͱ� ������ �����ͼ� ��������1�� ����
			if (isRegister1)
				R1 = data;
			else
				R2 = data;
			//ĳ�� ����
		}
		else in_ram(data, isRegister1); //ĳ�ÿ� �����Ͱ� ������ RAM�� �˻��Ͽ� RAM�� �����ϴ� �Լ� ȣ��
	}

	void in_ram(int data, bool isRegister1)
	{
		if (check_ram(data)) { //���� �˻��ؼ� �����Ͱ� ������ �����ͼ� ĳ�ÿ� ���� -> �������Ϳ� ����
			if (l1 >= 5)
				l1 = 0;
			L1[l1] = data;


			if (isRegister1)
				R1 = data;
			else
				R2 = data;

			l1++;
		}
		else in_hdd(data, isRegister1); //���� �����Ͱ� ������ �ϵ��ũ�� �˻��Ͽ� ���� �����ϴ� �Լ� ȣ��
	}

	void in_hdd(int data, bool isRegister1)
	{
		if (check_hdd(data)) { //�ϵ��ũ�� �˻��ؼ� �����Ͱ� ������ �����ͼ� ���� ���� -> ĳ���� ���� -> �������Ϳ� ����
			if (r >= 500) {
				r = 0;

				if (l1 >= 5) { // l1 �ε��� �ʰ�
					l1 = 0;
				}
			}
			else if (r < 500) {
				if (l1 >= 5) { // l1 �ε��� �ʰ�
					l1 = 0;
				}
			}

			RAM[r] = data;
			L1[l1] = data;

			if (isRegister1)
				R1 = data;
			else
				R2 = data;

			r++; l1++;
			// ĳ�ý���, �޸� ����
		}
		else error(); //�ϵ��ũ���� ���ϴ� �����Ͱ� ������ ���� ���� ���
	}


	void error()
	{
		cout << "**���� ����! ������ �������� ����!**" << endl;
		exit(0);//����
	}

	void show_hit(double count)
	{
		cout << "��ü �޸��� ���� Ƚ��: " << count << endl;
		cout << "ĳ�� �޸��� ���� Ƚ��: " << hit1 << endl;
		cout << "ĳ�� �޸� Hit��: " << (hit1 / count) * 100 << "%" << endl; // ��Ʈ�� = (���� Ƚ��)/(���� Ƚ��) 
		cout << "ĳ�� �޸� Miss��: " << 100 - (hit1 / count) * 100 << "%" << endl << endl;

	}

	void show()
	{
		/*cout << "<�ϵ��ũ>";
		for (int i = 0; i < 5000; i++)//HDD �� ���
		{
			if (i % 16 == 0) cout << endl;
			cout << HDD[i] << " ";
		}*/
		cout << endl;
		cout << "<���θ޸�>" << endl;
		for (int i = 0; i < 500; i++) cout << RAM[i] << " "; //RAM�� ���� ��� 

		cout << endl;
		cout << "<ĳ�ø޸�>" << endl;
		for (int i = 0; i < 5; i++) cout << L1[i] << " "; //Cache1�� ���� ��� 

		cout << endl;
		cout << "<��������1>" << endl; //���� ����� ��� 
		cout << R1 << endl;

		cout << "<��������2>" << endl; //���� ����� ��� 
		cout << R2 << endl;

		cout << "������ ���� �ð�: " << acc_time << "��" << endl;
	}

	void show_add_result() {
		cout << "�� ������ ���� ����� " << R1 + R2 << " �̴�." << endl;
	}
};

int main()
{
	Cache c1;
	int input1, input2, count = 0;

	while (true) {
		cout << "�а���� ù��° �����͸� �Է��Ͻÿ�(1�̻��� ����)" << endl;
		cin >> input1;

		cout << endl;
		count++;
		c1.reset_acc_time();
		c1.in_cache1(input1, 1);
		c1.show();
		c1.show_hit(count);

		cout << endl;


		cout << "�а���� �ι�° �����͸� �Է��Ͻÿ�(1�̻��� ����)" << endl;
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
