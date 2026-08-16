/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * Concurrency and Parallelism
*/

#include <print>

#include <chrono>
#include <thread>

using std::print, std::println;

using std::chrono::steady_clock;
using std::chrono::duration;

using namespace std::this_thread;
using namespace std::chrono_literals;

using std::thread;

void sleepms(const unsigned ms) {
	using std::chrono::milliseconds;
	sleep_for(milliseconds(ms));
}

void fthread(const int n) {
	println("this is t{}", n);
	for (size_t i {}; i < 5; ++i) {
		sleepms(100 * n);
		println("t{}: {}", n, i + 1);
	}
	println("finishing t{}", n);
}



int main() {
	//{
	//	println("\n--- Sleep a process for a specific amount of time ---\n");
	//	
	//	auto t1 = steady_clock::now();
	//	println("sleep for 1.3 seconds");
	//	sleep_for(1s + 300ms);
	//	println("sleep for 2 seconds");
	//	sleep_until(steady_clock::now() + 2s);
	//	duration<double> dur1 = steady_clock::now() - t1;
	//	println("total duration: {:.5}s", dur1.count());
	//
	//	println();
	//}

	
	println("\n--- Use std::thread for concurrency ---\n");
	
	//thread t1(fthread, 1);
	//thread t2(fthread, 2);

	//t1.join();
	//t2.join();

	//t1.detach();
	//t2.detach();

	//println("main() sleep 2 sec");
	//sleepms(2000);

	std::jthread t1(fthread, 1);
	std::jthread t2(fthread, 2);

	println("\nend of main()");	
	println();

}