#include <iostream>
#include <Windows.h>
#include <conio.h>
#include<time.h>
#include<vector>
#include<string>
#include<algorithm>
using namespace  std;

enum Color { Black = 0, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White };

enum Direction { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, esc = 27 };
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);//получает дискриптор активного окна

void SetColor(Color text, Color background)
{
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void SetCursor(int x, int y)
{
	COORD myCoords = { x,y };//инициализируем передаваемыми значениями объект координат
	SetConsoleCursorPosition(hStdOut, myCoords);
}
struct Element
{
	int data;
	Element* next;//указатель на следующий элемент
};
class List
{
	//указатель на начало списка
	Element* Head;
	//указатель на конец списка
	Element* Tail;
	//количество элементов
	int count;
public:
	List()//конструктор по умолчанию - список пуст
	{
		count = 0;
		Head = Tail = nullptr;//оба указателя никуда не указывают
	}
	void addElement(int data)//метод для добавления элемента в хвост списка
	{
		//создаём новый динамический элемент
		Element* elem = new Element;
		elem->data = data;
		//если это первый добавляемый элемент - он и голова и хвост одновременно
		if (isEmpty())// count==0
		{
			Head = Tail = elem;
		}
		else
		{
			Tail->next = elem;//добавили новый элемент после хвостового
			Tail = elem;//пепебросили хвост на последний элемент
		}
		Tail->next = nullptr;//после хвоста никого нет
		count++;
	}
	//метод для проверки пуст ли список
	bool isEmpty()
	{
		return Head == nullptr;
	}
	//извлечение элемента с головы
	int getHead()
	{		//если был удалён последний элемент - сбросить голову и хвост в нул

		if (!isEmpty())
		{
			Element* temp = Head;//записываем головной элемент
			//перебрасываем голову на следующий элемент
			Head = Head->next;
			int t = temp->data;
			delete temp;
			count--;
			if (count == 0)
			{
				Head = Tail = nullptr;
			}
			return t;
		}
		return MININT;
	}

	int getElem(int n)//изъятие элемента с позиции
	{
		if (!isEmpty())
		{
			if (n == 0)//с головы
			{
				getHead();
			}

			else if (n == count - 1)//c хвоста
			{
				//cout << "TAIL\n";
				Element* temp;
				Element* move = Head;
				for (int i = 0; i < n - 1; i++)
				{
					move = move->next;
				}
				temp = move->next;//удаляемый элемент
				//temp->next = nullptr;
				int t = temp->data;

				//move->next->next;//аналог 98 строки
				//int t = move->next->next->data;
				count--;
				if (count == 0)
				{
					Head = Tail = nullptr;
				}
				else
				{
					Tail = move;
					Tail->next = nullptr;
				}
				delete temp;
				return t;
			}
			else if (n < count - 1 && n>0)//с середины
			{
				Element* temp, * x;
				Element* move = Head;
				for (int i = 0; i < n - 1; i++)
				{
					move = move->next;
				}
				temp = move->next;
				x = temp->next;//в Х записываем весь оствашийся список
				int t = temp->data;//в переменную записываем значение элемента которй удаляем
				delete temp;
				count--;
				if (count == 0)
				{
					Head = Tail = nullptr;
				}
				else
				{
					move->next = x;
				}
				return t;
			}
		}
		//если был удалён последний элемент - сбросить голову и хвост в нул

	}
	//показать содержимое списка
	void printList()
	{
		Element* move = Head;
		while (move != nullptr)
		{
			cout << move->data << "\t";
			move = move->next;//перебрасываем указатель на следующий элемент
		}
	}
	//перегрузить []
	int operator[](int n)
	{
		if (n < count)
		{
			Element* move = Head;
			for (int i = 0; i < n; i++)
			{
				move = move->next;
			}
			return move->data;
		}
		return -1;
	}
	//добавить элемент на указанную позицию
	void addElement(int n, int data)
	{
		//если мы добавляем в хвост, если мы добавляем в голову, если это первый из добавляемых элементов
		if (n == count || Head == nullptr)
		{
			addElement(data);
		}
		else if (n == 0)//добавление в начало
		{
			Element* newelem = new Element;
			newelem->data = data;

			newelem->next = Head;
			Head = newelem;
			count++;
		}
		else if (n < count)
		{
			Element* move = Head, * temp;
			Element* newelem = new Element;
			newelem->data = data;
			for (int i = 0; i < n - 1; i++)
			{
				move = move->next;
			}
			temp = move->next;//записали адрес следующего после добавлемого
			move->next = newelem;//создали связь между элементом, до которого мы дошли и новым элементом
			newelem->next = temp;//после добавленного элемента идут все остальные
			count++;
		}
	}
	//перегрузить =
	List operator = (List& right)
	{
		int temp;
		do
		{
			temp = this->getHead();
		} while (this->count != 0);
		for (size_t i = 0; i < right.count; i++)
		{
			this->addElement(i, right[i]);
		}
		return*this;
	}
	// перегрузить += конкотенация двух списков
	List operator += (List& right)
	{
		for (size_t i = 0; i < right.count; i++)
		{
			this->addElement(right[i]);
		}
		return*this;
	}
	//перегрузить !  инвертировать направление списка
	List operator ! ()
	{		
		List copy;		
		for (int i = count - 1; i >= 0; i--)
		{			
			copy.addElement(this->getElem(i));
		}
		for (size_t i = 0; i < copy.count; i++)
		{
			this->addElement(copy[i]);
		}
		return *this;
	}
	/*void operator! ()
	{
		for (size_t i = count - 1; i > 0; i--)
		{
			cout << getElem(i) << "\t";
		}
	}*/
	~List()
	{

	}
};



//деструктор
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));
	List mylist, mylist2;
	for (size_t i = 0; i < 4; i++)
	{
		mylist.addElement(rand() % 45);
	}
	for (size_t i = 0; i < 6; i++)
	{
		mylist2.addElement(rand() % 45);
	}
	cout << "Первый список: \n";
	mylist.printList();
	cout << "\n Второй список: \n";
	mylist2.printList();
	cout << endl;
	/*cout << "\n\n" << mylist.getHead() << "\t" << mylist.getHead() << "\n\n";
	mylist.printList();
	cout << "Элемент из списка: " << mylist[4] << "\n";*/
	cout << mylist.getElem(3);
	cout << "\nДобавили элемент 33 в хвост первого списка\n";
	mylist.addElement(3, 33);
	cout << endl;
	mylist.printList();
	cout << "\n\n";
	cout << "Оператор = : \n";
	cout << "Второй список равен первому\n";
	mylist2 = mylist;
	mylist2.printList();
	cout << "\nКонкотенация: \n";
	mylist2 += mylist;
	mylist2.printList();
	cout << "\nИнверсия: \n";
	!mylist;
	mylist.printList();
	return 0;
}