#include <cstddef>

template <size_t _size>
class MovingAverage {
	private:
		int _arr[_size];
		size_t _startIndex;
		int _sum;

	public:
		MovingAverage(int initalVal = 0) {
			_startIndex = 0;
			_sum = 0;
			for (size_t i = 0; i < _size; i++) {
				_arr[i] = initalVal;
				_sum += initalVal;
			}
		}

		void popAndPush(int newVal) {
			// Pop
			int removedVal = _arr[_startIndex];
			_sum -= removedVal;
			// Push
			_arr[_startIndex] = newVal;
			_sum += newVal;
			// Move starting index
			_startIndex = (_startIndex + 1) % _size;
		}

		int average() const {
			return _sum / _size;
		}
};