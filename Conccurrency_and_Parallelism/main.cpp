/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * Concurrency and Parallelism
*/

#include <print>

#include <list>
#include <vector>

#include <chrono>
#include <random>
#include <algorithm>

#include <thread>
#include <future>
#include <execution>

using std::print, std::println;

using std::list, std::vector;

using std::chrono::steady_clock;
using std::chrono::duration;

using namespace std::this_thread;
using namespace std::chrono_literals;

using std::thread;
using std::async;

namespace execution = std::execution;

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


	//println("\n--- Use std::async for concurrency ---\n");
	//
	//constexpr uint64_t max_prime { 0x4FFFFF };
	//constexpr size_t num_threads { 14 };
	//list<std::future<prime_time>> swarm;
	//
	//println("start parallel primes");
	//auto t1 = steady_clock::now();
	//for (size_t i {}; i < num_threads; ++i) {
	//	swarm.emplace_back(
	//		async(launch::async, count_primes, max_prime)
	//	);
	//}
	//
	//for (auto& f : swarm) {
	//	static size_t i {};
	//	auto pt = f.get();
	//	println("primes({:02}): {} {:.5}", ++i, pt.count, pt.dur);
	//}
	//
	//secs dur_total { steady_clock::now() - t1 };
	//println("total duration: {:.5}s", dur_total.count());
	//
	//
	//// async unwrapped
	//std::promise<int> value_promise;
	//std::future<int> value_future = value_promise.get_future();
	//thread thr1(f, std::move(value_promise));
	//thr1.detach();
	//println("value is: {}", value_future.get());


	println("\n--- Run STL algorithms in parallel with execution policies ---\n");

	using dur_t = duration<double>;

	vector<unsigned> v(20'000'000);
	std::random_device rng;
	println("generate randoms");
	for (auto& i : v) i = rng() % 0xFFFF'FFFF;

	auto mul2 = [](int n) { return n * 2; };
	
	auto t0 = steady_clock::now();
	std::transform(v.begin(), v.end(), v.begin(), mul2);
	dur_t dur0 = steady_clock::now() - t0;

	auto t1 = steady_clock::now();
	std::transform(execution::seq, v.begin(), v.end(), v.begin(), mul2);
	dur_t dur1 = steady_clock::now() - t1;
	
	auto t2 = steady_clock::now();
	std::transform(execution::par, v.begin(), v.end(), v.begin(), mul2);
	dur_t dur2 = steady_clock::now() - t2;
	
	auto t3 = steady_clock::now();
	std::transform(execution::par_unseq, v.begin(), v.end(), v.begin(), mul2);
	dur_t dur3 = steady_clock::now() - t3;
	
	println("no policy: {:.3}s", dur0.count());
	println("execution::seq: {:.3}s", dur1.count());
	println("execution::par: {:.3}s", dur2.count());
	println("execution::par_unseq: {:.3}s", dur3.count());

	println("\nsort");

	//auto ts0 = steady_clock::now();
	//std::sort(v.begin(), v.end());
	//dur_t durs0 = steady_clock::now() - t0;

	auto ts2 = steady_clock::now();
	std::sort(execution::par, v.begin(), v.end());
	dur_t durs2 = steady_clock::now() - t2;

	//println("no policy: {:.3}s", durs0.count()); // 6.88s
	println("execution::par: {:.3}s", durs2.count()); // 1.1s


	println("\nend of main()");	
	println();

}