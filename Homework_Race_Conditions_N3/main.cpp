#include <iostream>
#include <thread>
#include <mutex>

class Data
{
public:
	int int_value;
	char char_value;
	double double_value;
	bool bool_value;
	std::mutex mutex, mx;
	Data(int int_value, char char_value, double double_value, bool bool_value) :
	int_value(int_value), char_value(char_value), double_value(double_value), bool_value(bool_value) {}

	void printdata()
	{
		std::cout << "Data:" << std::endl;
		std::cout << int_value << std::endl;
		std::cout << char_value << std::endl;
		std::cout << double_value << std::endl;
		std::cout << bool_value << std::endl;
		std::cout << std::endl;
	}
};

void sc_lock_swap(Data& d, Data& d1)
{
	std::scoped_lock lock(d1.mutex, d.mutex);
	std::swap(d1.int_value, d.int_value);
	std::swap(d1.char_value, d.char_value);
	std::swap(d1.double_value, d.double_value);
	std::swap(d1.bool_value, d.bool_value);
}
void unique_lock_swap(Data& d, Data& d1)
{
	std::unique_lock lock(d1.mutex);
	std::unique_lock lock1(d.mutex);
	std::swap(d1.int_value, d.int_value);
	std::swap(d1.char_value, d.char_value);
	std::swap(d1.double_value, d.double_value);
	std::swap(d1.bool_value, d.bool_value);
}
void lock_swap(Data& d, Data& d1)
{
	std::lock(d.mx, d1.mx);
	std::swap(d1.int_value, d.int_value);
	std::swap(d1.char_value, d.char_value);
	std::swap(d1.double_value, d.double_value);
	std::swap(d1.bool_value, d.bool_value);
	d.mx.unlock();
	d1.mx.unlock();
}

int main()
{
	Data d1(1, 'a', 1.5, 0);
	Data d2(2, 'b', 3.5, 1);
	d1.printdata();
	d2.printdata();
	std::thread t1(sc_lock_swap, std::ref(d1), std::ref(d2));
	std::thread t3(lock_swap, std::ref(d1), std::ref(d2));
	std::thread t2(unique_lock_swap, std::ref(d1), std::ref(d2));
	t1.join();
	t2.join();
	t3.join();
	d1.printdata();
	d2.printdata();
	return 0;
}