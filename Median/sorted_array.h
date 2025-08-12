#include <cstddef>

template <typename T, size_t _maxSize>
class SortedArray {
	private:
		T _arr[_maxSize];
		size_t _curSize = 0;

		size_t _insertIndex(T element) const;
		void _shiftRight(size_t startIndex, size_t endIndex);
		void _shiftLeft(size_t startIndex, size_t endIndex);
		size_t _find(T element) const;

	public:
		size_t size() const { return _curSize; }
		void insert(T element);
		void removeAndInsert(T removedElement, T insertedElement);
};

// Private ---------------------------------------------------------------------
template <typename T, size_t _maxSize>
size_t SortedArray<T, _maxSize>::_insertIndex(T element) const {
	for (int i = 0; i < _curSize; i++) {
		if (_arr[i] > element) return i;
	}
	return _curSize;
}

template <typename T, size_t _maxSize>
void SortedArray<T, _maxSize>::_shiftRight(size_t startIndex, size_t endIndex) {
	for (int i = startIndex + 1; i < endIndex; i++) {
		T temp = _arr[i - 1];
		_arr[i - 1] = _arr[i];
		_arr[i] = temp;
	}
}

template <typename T, size_t _maxSize>
void SortedArray<T, _maxSize>::_shiftLeft(size_t startIndex, size_t endIndex) {
	for (int i = startIndex - 1; i > endIndex; i--) {
		T temp = _arr[i + 1];
		_arr[i + 1] = _arr[i];
		_arr[i] = temp;
	}
}

template <typename T, size_t _maxSize>
size_t SortedArray<T, _maxSize>::_find(T element) const {
	for (int i = 0; i < _curSize; i++) {
		if (_arr[i] == element) return i;
	}
	return _curSize;
}

// Public ----------------------------------------------------------------------

template <typename T, size_t _maxSize>
void SortedArray<T, _maxSize>::insert(T element) {
	size_t index = _insertIndex(element);
	_shiftRight(index, _curSize);
	_arr[index] = element;
	_curSize++;
}

template <typename T, size_t _maxSize>
void SortedArray<T, _maxSize>::removeAndInsert(T removedElement, T insertedElement) {
	size_t removeIndex = _find(element);
	size_t insertIndex = _insertIndex(element);
	if (removeIndex > insertIndex) {
		_shiftRight(insertIndex, removeIndex);
	} else {
		_shiftLeft(insertIndex, removeIndex);
	}
	_arr[insertIndex] = element;
}