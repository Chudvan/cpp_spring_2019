#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

int count = 1000000;
std::mutex m;
std::condition_variable c;

void ping() {
	while (count) {
		std::unique_lock<std::mutex> lock(m);
		while (count % 2) {
			c.wait(lock);
		}
		std::cout << "ping\n";
		count--;
		c.notify_one();
	}
}

void pong() {
	while (count) {
		std::unique_lock<std::mutex> lock(m);
		while ((count + 1) % 2) {
			c.wait(lock);
		}
		std::cout << "pong\n";
		count--;
		if (!count) return;
		c.notify_one();
	}
}

int main() {
	std::thread t1(ping);
	std::thread t2(pong);
	t1.detach();
	t2.detach();
	system("pause>nul");
	return 0;
}