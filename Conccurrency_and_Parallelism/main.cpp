/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * Concurrency and Parallelism
*/

#include <print>
#include <string>

#include <list>
#include <vector>
#include <deque>

#include <chrono>
#include <random>
#include <algorithm>
#include <optional>
#include <memory>

#include <thread>
#include <future>
#include <execution>
#include <mutex>
#include <atomic>
#include <coroutine>

using std::print, std::println;
using std::string, std::string_view;

using std::list, std::vector, std::deque;

using std::chrono::steady_clock;
using std::chrono::duration;

using std::optional;

using std::make_unique;

using namespace std::this_thread;
using namespace std::chrono_literals;

using std::thread, std::jthread;
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

std::mutex animal_mutex {};

class Animal {
	using friend_t = list<Animal>;
	string_view s_name { "unk" };
	friend_t l_friends {};
public:
	Animal() = delete;
	Animal(const string_view n) : s_name { n } {};
	
	bool operator==(const Animal& o) const {
		return s_name.data() == o.s_name.data();
	}

	bool is_friend(const Animal& o) const {
		for (const auto& a : l_friends) {
			if (a == o) return true;
		}
		return false;
	}

	optional<friend_t::iterator> find_friend(const Animal& o) noexcept {
		for (auto it { l_friends.begin() }; it != l_friends.end(); ++it) {
			if (*it == o) return it;
		}
		return {};
	}

	void display() const noexcept {
		std::lock_guard<std::mutex> l(animal_mutex);
		//animal_mutex.lock();
		auto n_animals { l_friends.size() };
		println("Animal: {}, friends: ", s_name);
		if (!n_animals) println("none");
		else {
			for (const auto& n : l_friends) {
				println("{}", n.s_name);
			}
		}
		println();
		//animal_mutex.unlock();
	}

	bool add_friend(Animal& o) noexcept {
		println("add friend {} -> {}", s_name, o.s_name);
		if (*this == o) return false;
		std::lock_guard<std::mutex> l(animal_mutex);
		//animal_mutex.lock();
		if (!is_friend(o)) {
			l_friends.emplace_back(o);
		}
		if (!o.is_friend(*this)) {
			o.l_friends.emplace_back(*this);
		}
		//animal_mutex.unlock();
		return true;
	}

	bool delete_friend(Animal& o) noexcept {
		println("delete friend {} -> {}", s_name, o.s_name);
		if (*this == o) return false;
		std::lock_guard<std::mutex> l(animal_mutex);
		//animal_mutex.lock();
		if (auto it = find_friend(o)) {
			l_friends.erase(it.value());
		}
		if (auto it = o.find_friend(*this)) {
			o.l_friends.erase(it.value());
		}
		//animal_mutex.unlock();
		return true;
	}
};


std::atomic<bool> ready {};
std::atomic<uint64_t> g_count {};
std::atomic_flag winner {};

constexpr int max_count { 1'000 };
//constexpr int max_threads { 100 };

void countem(int id) {
	while (!ready) std::this_thread::yield();
	for (int i {}; i < max_count; ++i) ++g_count;
	if (!winner.test_and_set()) {
		println("thread {:02} won!", id);
	}
}


constexpr size_t max_threads { 25 };
std::once_flag init_flag;

void do_init(size_t id) {
	print("do init ({}): ", id);
}

void do_print(size_t id) {
	std::call_once(init_flag, do_init, id);
	print("{} ", id);
}


namespace this_thread = std::this_thread;
using guard_t = std::lock_guard<std::mutex>;
using lock_t = std::unique_lock<std::mutex>;

constexpr size_t num_items { 15 };
constexpr auto delay_time { 50ms };

deque<size_t> q {};
std::mutex mtx {};
std::condition_variable cond {};
bool finished {};

void producer() {
	for (size_t i {}; i < num_items; ++i) {
		this_thread::sleep_for(delay_time);
		guard_t x { mtx };
		q.push_back(i);
		cond.notify_all();
	}
	guard_t x { mtx };
	finished = true;
	cond.notify_all();
}

void consumer() {
	while (!finished) {
		lock_t lck { mtx };
		cond.wait(lck, [] {
			return !q.empty() || finished;
		});
		while (!q.empty()) {
			println("Got {} from the queue", q.front());
			q.pop_front();
		}
	}
	println("finished!");
}


constexpr auto consumer_wait { 100ms };
constexpr size_t queue_limit { 5 };
constexpr size_t num_producers { 3 };
constexpr size_t num_consumers { 5 };

deque<string> qs {};
std::mutex q_mutex {};
std::condition_variable cv_producer {};
std::condition_variable cv_consumer {};
bool production_complete {};

void producer_new(const size_t id) {
	for (size_t i {}; i < num_items; ++i) {
		this_thread::sleep_for(delay_time * id);
		lock_t lock { q_mutex };
		cv_producer.wait(lock, [&] {
			return qs.size() < queue_limit;
		});
		qs.push_back(std::format("pid {}, qs {}, item {:02}\n", id, qs.size(), i + 1));
		cv_consumer.notify_all();
	}
}

void consumer_new(const size_t id) {
	while (!production_complete) {
		lock_t lock { q_mutex };
		cv_consumer.wait_for(lock, consumer_wait, [&] {
			return !qs.empty();
		});
		if (!qs.empty()) {
			print("cid {}: {}", id, qs.front());
			qs.pop_front();
		}
		cv_producer.notify_all();
	}
}


class task {
public:
	class promise_type;
	using co_h = std::coroutine_handle<promise_type>;
	co_h handle;

	class promise_type {
	public:
		task get_return_object() {
			return task { co_h::from_promise(*this) };
		}

		std::suspend_always initial_suspend() noexcept { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		void return_void() noexcept {}
		void unhandled_exception() { std::terminate(); }
	};

	explicit task(co_h h) : handle(h) {}

	task(task&& other) noexcept : handle(other.handle) { other.handle = {}; }
	task(const task&) = delete;
	task& operator=(const task&) = delete;
	task& operator=(task&&) = delete;

	~task() {
		if (handle) handle.destroy();
	}
};

class scheduler {
	deque<std::coroutine_handle<>> ready;
	bool f_done { false };
	deque<size_t> queue {};
public:
	void schedule(std::coroutine_handle<> h) {
		ready.push_back(h);
	}

	void run() {
		while (!ready.empty()) {
			auto h = ready.front();
			ready.pop_front();
			h.resume();
		}
	}

	void done(bool f) { f_done = f; }
	bool done() const { return f_done; }

	void q_push(const size_t& n) { queue.push_back(n); }
	bool q_empty() const { return queue.empty(); }
	size_t q_pop() {
		size_t n = queue.front();
		queue.pop_front();
		return n;
	}
};

class yield {
	scheduler& y_sched;
public:
	explicit yield(scheduler& s) noexcept : y_sched(s) {}
	bool await_ready() const noexcept { return false; }
	void await_resume() const noexcept {}

	void await_suspend(std::coroutine_handle<> h) const noexcept {
		y_sched.schedule(h);
	}
};

task worker(scheduler& sched, int id) {
	for (int i{}; i < 3; ++i) {
		println("worker {} step {}", id, i);
		co_await ::yield { sched };
	}
}

task producer(scheduler& sched) {
	for (size_t i {}; i < num_items; ++i) {
		sched.q_push(i);
		println("produced {}", i);
		co_await ::yield { sched };
	}
	sched.done(true);
}

task consumer(scheduler& sched) {
	while (!sched.q_empty() || !sched.done()) {
		if (!sched.q_empty()) {
			println("consumed {}", sched.q_pop());
		}
		co_await ::yield { sched };
	}
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


	//println("\n--- Run STL algorithms in parallel with execution policies ---\n");
	//
	//using dur_t = duration<double>;
	//
	//vector<unsigned> v(20'000'000);
	//std::random_device rng;
	//println("generate randoms");
	//for (auto& i : v) i = rng() % 0xFFFF'FFFF;
	//
	//auto mul2 = [](int n) { return n * 2; };
	//
	//auto t0 = steady_clock::now();
	//std::transform(v.begin(), v.end(), v.begin(), mul2);
	//dur_t dur0 = steady_clock::now() - t0;
	//
	//auto t1 = steady_clock::now();
	//std::transform(execution::seq, v.begin(), v.end(), v.begin(), mul2);
	//dur_t dur1 = steady_clock::now() - t1;
	//
	//auto t2 = steady_clock::now();
	//std::transform(execution::par, v.begin(), v.end(), v.begin(), mul2);
	//dur_t dur2 = steady_clock::now() - t2;
	//
	//auto t3 = steady_clock::now();
	//std::transform(execution::par_unseq, v.begin(), v.end(), v.begin(), mul2);
	//dur_t dur3 = steady_clock::now() - t3;
	//
	//println("no policy: {:.3}s", dur0.count());
	//println("execution::seq: {:.3}s", dur1.count());
	//println("execution::par: {:.3}s", dur2.count());
	//println("execution::par_unseq: {:.3}s", dur3.count());
	//
	//println("\nsort");
	//
	////auto ts0 = steady_clock::now();
	////std::sort(v.begin(), v.end());
	////dur_t durs0 = steady_clock::now() - t0;
	//
	//auto ts2 = steady_clock::now();
	//std::sort(execution::par, v.begin(), v.end());
	//dur_t durs2 = steady_clock::now() - t2;
	//
	////println("no policy: {:.3}s", durs0.count()); // 6.88s
	//println("execution::par: {:.3}s", durs2.count()); // 1.1s


	//println("\n--- Use mutex and lock to safely share data ---\n");
	//
	//auto cat1 = make_unique<Animal>("Felix");
	//auto tiger1 = make_unique<Animal>("Hobbes");
	//auto dog1 = make_unique<Animal>("Astro");
	//auto rabbit1 = make_unique<Animal>("Bugs");
	//
	//auto a1 = async([&] { cat1->add_friend(*tiger1); });
	//auto a2 = async([&] { cat1->add_friend(*rabbit1); });
	//auto a3 = async([&] { rabbit1->add_friend(*dog1); });
	//auto a4 = async([&] { rabbit1->add_friend(*cat1); });
	//
	//a1.wait();
	//a2.wait();
	//a3.wait();
	//a4.wait();
	//
	//auto p1 = async([&] { cat1->display(); });
	//auto p2 = async([&] { tiger1->display(); });
	//auto p3 = async([&] { dog1->display(); });
	//auto p4 = async([&] { rabbit1->display(); });
	//
	//p1.wait();
	//p2.wait();
	//p3.wait();
	//p4.wait();
	//
	//auto a5 = async([&] { cat1->delete_friend(*rabbit1); });
	//a5.wait();
	//
	//auto p5 = async([&] { cat1->display(); });
	//auto p6 = async([&] { rabbit1->display(); });

	
	//println("\n--- Share flags and values with std::atomic ---\n");
	//
	//vector<thread> swarm;
	//println("spawn {} threads", max_threads);
	//for (int i {}; i < max_threads; ++i) {
	//	swarm.emplace_back(countem, i);
	//}
	//ready = true;
	//for (auto& t : swarm) t.join();
	//println("global count: {}", (int)g_count);
	//
	//println("is g_count lock-free? {}", g_count.is_lock_free());


	//println("\n--- Initialize threads with std::call_once ---\n");
	//
	//list<jthread> spawn;
	//for (size_t id {}; id < max_threads; ++id) {
	//	spawn.emplace_back(do_print, id);
	//}


	//println("\n--- Resolve the producer-consumer problem with std::condition_variable ---\n");
	//
	//jthread t1 { producer };
	//jthread t2 { consumer };


	//println("\n--- Implement multiple producers and consumers ---\n");
	//
	//list<std::future<void>> producers;
	//list<std::future<void>> consumers;
	//
	//for (size_t i {}; i < num_producers; ++i) {
	//	producers.emplace_back(async(producer_new, i));
	//}
	//
	//for (size_t i {}; i < num_consumers; ++i) {
	//	consumers.emplace_back(async(consumer_new, i));
	//}
	//
	//for (auto& f : producers) f.wait();
	//production_complete = true;
	//println("producers done.");
	//
	//for (auto& f : consumers) f.wait();
	//println("consumers done.");


	//println("\n--- Suspend and resume execution with coroutines ---\n");
	//
	//scheduler sched {};
	//
	//auto t1 = worker(sched, 1);
	//auto t2 = worker(sched, 2);
	//auto t3 = worker(sched, 3);
	//
	//sched.schedule(t1.handle);
	//sched.schedule(t2.handle);
	//sched.schedule(t3.handle);
	//
	//sched.run();


	println("\n--- Use coroutines for a producer-consumer solution ---\n");

	scheduler sched {};

	auto p1 = producer(sched);
	auto p2 = producer(sched);
	auto c = consumer(sched);

	sched.schedule(p1.handle);
	sched.schedule(p2.handle);
	sched.schedule(c.handle);

	sched.run();


	println("\nend of main()");	
	println();

}