#pragma once
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <utility>

class Sort {
	size_t numberOfThreads;
	size_t memory;
	size_t maxNumbers;
	size_t numbersPerThread;
	size_t amountOfParts;
	std::vector<std::vector<uint64_t>> bufFiles;
	std::vector<std::unique_ptr<std::mutex>> mutexes;
	std::condition_variable c;
	bool statusFull;
	std::queue<size_t> freeThreads;

public:

	Sort() : numberOfThreads(2)
		, memory(8 * 1024 * 1024)
		, maxNumbers(memory / (sizeof(uint64_t)))
		, numbersPerThread(2)
		, amountOfParts(0)
		, statusFull(false)
	{
		createMutex();
		createBuf();
	}

	void operator()(std::string& input, std::string& output) {

		for (size_t i = 0; i < numberOfThreads; i++) {
			freeThreads.push(i);
		}

		std::ifstream ifile(input, std::ios::binary);
		if (!ifile) {
			throw std::runtime_error("can't open " + input + " file.");
		}
		while (!ifile.eof()) {
			std::vector<uint64_t> bufFile;
			bufFile.resize(numbersPerThread);
			if (!ifile.read(reinterpret_cast<char *>(bufFile.data()), numbersPerThread * sizeof(uint64_t))) {
				if (!ifile.eof()) {
					throw std::runtime_error("can't read " + input + " file.");
				}
			}

			size_t bytesInThread = static_cast<size_t>(ifile.gcount());
			size_t numbersInThread = bytesInThread / sizeof(uint64_t);
			bufFile.resize(numbersInThread);

			std::unique_lock<std::mutex> lock(*(mutexes[0]));
			if (freeThreads.empty())
				statusFull = true;
			while (freeThreads.empty()) {
				c.wait(lock);
			}

			if (ifile.eof() && statusFull)
				break;

			size_t threadId = freeThreads.front();
			freeThreads.pop();
			bufFiles[threadId].swap(bufFile);

			std::thread thread(std::bind(&Sort::sortAndSave, this, getFileName(amountOfParts), threadId));
			thread.detach();
			amountOfParts++;
		}

		std::unique_lock<std::mutex> lock(*(mutexes[0]));
		while (freeThreads.size() < numberOfThreads) {
			c.wait(lock);
		}

		std::ofstream ofile(output, std::ios::binary);
		if (!ofile) {
			throw std::runtime_error("can't open " + output + " file.");
		}

		mergeAndSave(ofile);

		removeTemp();

	};

private:

	std::string getFileName(size_t n) {
		std::string file = "temp_" + std::to_string(n) + ".tmp";
		return file;
	}

	void removeTemp() {
		for (int i = 0; i < amountOfParts; i++) {
			std::remove(getFileName(i).c_str());
		}
	}

	void createMutex() {
		mutexes.resize(numberOfThreads + 1);
		for (size_t i = 0; i < numberOfThreads + 1; i++) {
			mutexes[i] = std::make_unique<std::mutex>();
		}
	}

	void createBuf() {
		bufFiles.resize(numberOfThreads);
		for (size_t i = 0; i < numberOfThreads; i++) {
			bufFiles[i].resize(numbersPerThread);
		}
	}

	void sortAndSave(std::string& ofname, size_t threadId) {
		std::unique_lock<std::mutex> lock(*mutexes[threadId + 1]);
		auto &part = bufFiles[threadId];
		std::sort(std::begin(part), std::end(part));
		std::ofstream ofile(ofname, std::ios::binary);
		if (!ofile) {
			throw std::runtime_error("can't open " + ofname + " file.");
		}
		ofile.write(reinterpret_cast<char *>(part.data()), part.size() * sizeof(uint64_t));
		freeThreads.push(threadId);
		c.notify_all();
	}

	uint64_t getNum(std::ifstream& infile) {
		uint64_t num;
		if (!infile.read(reinterpret_cast<char *>(&num), sizeof(uint64_t))) {
			if (!infile.eof()) {
				throw std::runtime_error("can't read temp file.");
			}
		}
		return num;
	}

	void writeNum(std::ofstream& ofile, uint64_t n) {
		if (!ofile.write(reinterpret_cast<char *>(&n), sizeof(uint64_t))) {
			throw std::runtime_error("can't write in output file.");
		}
	}

	void mergeAndSave(std::ofstream& ofile) {

		std::vector<std::ifstream> tempFiles;
		for (int i = 0; i < amountOfParts; i++) {
			tempFiles.emplace_back(getFileName(i));
			if (!tempFiles.back()) {
				throw std::runtime_error("can't read temp file.");
			}
		}

		using Pair = std::pair<uint64_t, size_t>;

		auto comparator = [](const Pair& x, const Pair& y) {
			return x.first > y.first;
		};

		std::priority_queue<Pair, std::vector<Pair>, decltype(comparator)> pQueue(comparator);

		for (int i = 0; i < amountOfParts; i++) {
			uint64_t curNum = getNum(tempFiles[i]);
			pQueue.push(std::make_pair(curNum, i));
		}

		while (!pQueue.empty()) {
			Pair curPair = pQueue.top();
			pQueue.pop();
			uint64_t num = curPair.first;
			size_t id = curPair.second;
			writeNum(ofile, num);
			auto &curFile = tempFiles[id];
			uint64_t curNum = getNum(curFile);
			if (curFile.eof())
				continue;
			pQueue.push(std::make_pair(curNum, id));
		}
	}
};