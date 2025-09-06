#include <cstddef>

template <size_t _size>
class MovingAverage {
	private:
		double _arr[_size];
		size_t _startIndex;
		double _sum;

	public:
		MovingAverage(double initalVal = 0) {
			_startIndex = 0;
			_sum = 0;
			for (size_t i = 0; i < _size; i++) {
				_arr[i] = initalVal;
				_sum += initalVal;
			}
		}

		void popAndPush(double newVal) {
			// Pop
			double removedVal = _arr[_startIndex];
			_sum -= removedVal;
			// Push
			_arr[_startIndex] = newVal;
			_sum += newVal;
			// Move starting index
			_startIndex = (_startIndex + 1) % _size;
		}

		double average() const {
			return _sum / _size;
		}
};