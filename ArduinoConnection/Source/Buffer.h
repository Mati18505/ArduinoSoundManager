#pragma once
#include <cassert>

class Buffer {
public:
	Buffer() = default;

	explicit Buffer(const std::vector<std::byte>& buffer)
		: buffer(buffer)
	{}
	template<typename T>
	explicit Buffer(const T& data)
	{
		T dataCpy{ data };
		buffer = { reinterpret_cast<std::byte*>(&dataCpy), reinterpret_cast<std::byte*>(&dataCpy + std::size(dataCpy)) };
	}

	void ZeroInitialize()
	{
		std::ranges::fill(buffer.begin(), buffer.end(), std::byte(0));
	}

	template<typename T>
	const T ReadAs(size_t offset = 0)
	{
		assert(offset + sizeof(T) < GetSize());
		return *((T*)((size_t*)buffer.data() + offset));
	}

	void Write(const std::vector<std::byte>& newBuffer)
	{
		buffer = newBuffer;
	}

	std::byte& operator[](size_t index)
	{
		return buffer.at(index);
	}

	std::byte operator[](size_t index) const
	{
		return buffer.at(index);
	}

	size_t GetSize() const { return buffer.size(); }
	std::byte* DataPtr() { return buffer.data(); }
	const std::byte* DataPtr() const { return buffer.data(); }

private:
	std::vector<std::byte> buffer;
};