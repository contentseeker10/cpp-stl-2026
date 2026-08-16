/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * Concurrency and Parallelism
*/

#include <print>

#include <list>

#include <chrono>
#include <thread>
#include <future>

using std::print, std::println;

using std::list;

using std::chrono::steady_clock;
using std::chrono::duration;

using namespace std::this_thread;
using namespace std::chrono_literals;

using std::thread;
using std::async;

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

using launch = std::launch;
using secs = std::chrono::duration<double>;

struct prime_time {
	secs dur {};
	uint64_t count {};
};

prime_time count_primes(const uint64_t max) {
	prime_time res {};
	constexpr auto isprime = [](uint64_t n) {
		if (n == 2) return true;
		if (n < 2 || n % 2 == 0) return false;
		for (uint64_t i { 3 }; i <= n / i; i += 2) {
			if (n % i == 0) return false;
		}
		return true;
	};
	res.count = max >= 2 ? 1 : 0;
	uint64_t start { 3 };
	uint64_t end { max };
	auto t1 = steady_clock::now();
	for (uint64_t i { start }; i <= end; ++i) {
		if (isprime(i)) ++res.count;
	}
	res.dur = steady_clock::now() - t1;
	return res;
}

void f(std::promise<int> value) {
	println("\nthis is f()");
	value.set_value(47);
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

	
	//println("\n--- Use std::thread for concurrency ---\n");
	//
	//thread t1(fthread, 1);
	//thread t2(fthread, 2);
	//
	//t1.join();
	//t2.join();
	//
	//t1.detach();
	//t2.detach();
	//
	//println("main() sleep 2 sec");
	//sleepms(2000);
	//
	//std::jthread t1(fthread, 1);
	//std::jthread t2(fthread, 2);


	println("\n--- Use std::async for concurrency ---\n");

	constexpr uint64_t max_prime { 0x4FFFFF };
	constexpr size_t num_threads { 14 };
	list<std::future<prime_time>> swarm;

	println("start parallel primes");
	auto t1 = steady_clock::now();
	for (size_t i {}; i < num_threads; ++i) {
		swarm.emplace_back(
			async(launch::async, count_primes, max_prime)
		);
	}

	for (auto& f : swarm) {
		static size_t i {};
		auto pt = f.get();
		println("primes({:02}): {} {:.5}", ++i, pt.count, pt.dur);
	}

	secs dur_total { steady_clock::now() - t1 };
	println("total duration: {:.5}s", dur_total.count());


	// async unwrapped
	std::promise<int> value_promise;
	std::future<int> value_future = value_promise.get_future();
	thread thr1(f, std::move(value_promise));
	thr1.detach();
	println("value is: {}", value_future.get());


	println("\nend of main()");	
	println();

}