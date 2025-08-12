#include <cstddef>

// Don't push when full or pop when empty
template <typename T, size_t _maxSize>
class StackedQueue {
	private:
		T _arr[_maxSize];
		size_t _curSize = 0;
		size_t _startIndex = 0;

	public:
		size_t size() const { return _curSize; }
		void push(T element);
		T pop();

		void print() const;
};

template <typename T, size_t _maxSize>
void StackedQueue<T, _maxSize>::push(T element) {
	size_t insertIndex = (_startIndex + _curSize) % _maxSize;
	_arr[insertIndex] = element;
	_curSize++;
}

template <typename T, size_t _maxSize>
T StackedQueue<T, _maxSize>::pop() {
	T returnedElement = _arr[_startIndex];
	_startIndex = (_startIndex + 1) % _maxSize;
	_curSize--;
	return returnedElement;
}