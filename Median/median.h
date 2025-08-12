#pragma once

#include "stacked_queue.h"
#include "sorted_array.h"
#include <cstddef>

template <typename T, size_t _maxSize>
class Median {
	private:
		StackedQueue<T, _maxSize> _que;
		SortedArray<T, _maxSize> _arr;

	public:
		T getMedian() const;
		void addElement(T element);
};

template <typename T, size_t _maxSize>
T Median<T, _maxSize>::getMedian() const {
	return _arr[_arr.size() / 2];
}

template <typename T, size_t _maxSize>
void Median<T, _maxSize>::addElement(T element) {
	if (_arr.size() < _maxSize) { // Not full
		_que.push(element);
		_arr.insert(element);
	} else { // Full
		T removedElement = _que.pop();
		_que.push(element);
		_arr.removedAndInsert(removedElement, element);
	}
}


/*
Fast for small values(0-11)
- Have stacked queue
- Insertion sort
	- Worst case: 55 comparisons
- Get the middle


- Have a stacked queue
- Find the smallest element
	- Repeat 6 times
Number of comparisons: 10 + 9 + 8 + 7 + 6 + 5 = 45

- Have a stacked queue
- Quick Select
*/