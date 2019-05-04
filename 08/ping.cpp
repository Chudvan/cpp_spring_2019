#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

const int count = 500000;
bool status = false;
std::mutex m;
std::condition_variable c;

void ping() {
	for(int i = 0; i < count; i++){
		std::unique_lock<std::mutex> lock(m);
		while (status) {
			c.wait(lock);
		}
		std::cout << "ping\n";
		status = true;
		c.notify_one();
	}
}

void pong() {
	for (int i = 0; i < count; i++) {
		std::unique_lock<std::mutex> lock(m);
		while (!status) {
			c.wait(lock);
		}
		std::cout << "pong\n";
		status = false;
		c.notify_one();
	}
}

int main() {
	std::thread t1(ping);
	std::thread t2(pong);
	t1.join();
	t2.join();
	return 0;
}