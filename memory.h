#ifndef MEMORY_H
#define MEMORY_H
#include "base_definitions.h"
#include <unordered_map>
#include <vector>

namespace vm {
	//-----------------------------------------
	// class Memory
	//-----------------------------------------
	class Memory {
	public:
		Memory();
		inline uchar& operator[](uint index);
		inline uchar const& operator[](uint index) const;
		inline void operator=(std::vector<uchar>& buf);
		std::vector<uchar> get(pRegister pc, unsigned int size);
		inline void Resize(size_t nMemSize, uint8 i8DefaultFill = 0);
		inline size_t GetSize() const
		{
			return static_cast<size_t>(m_buffer.size());
		}
		
		//assignments
		template <typename Type>
		inline Type& At(size_t);

		template <typename Type>
		inline Type const& At(size_t) const;

	private:
		std::vector<uchar> m_buffer;
	};

	typedef Memory* MemoryPtr;

	#define StackDefSize 64
}

#include <iostream>
inline uchar& vm::Memory::operator[](uint index)
{
	return m_buffer[index];
}

inline uchar const & vm::Memory::operator[](uint index) const
{
	return m_buffer[index];
}

inline void vm::Memory::operator=(std::vector<uchar> &buf)
{
	m_buffer.assign(buf.begin(), buf.end());
}

inline void vm::Memory::Resize(size_t nMemSize, uint8 i8DefaultFill)
{
	m_buffer.resize(nMemSize, i8DefaultFill);
}

template <typename Type>
inline Type& vm::Memory::At(size_t idx)
{
	if (idx == 0) // null is reserved
		throw ExceptionError("Segmentation fault.");
    else if (idx > m_buffer.size() - sizeof(Type))
		throw ExceptionError("Runtime error: access out of memory bounds.");
	return reinterpret_cast<Type&>(m_buffer[idx]);
}

template <typename Type>
inline Type const& vm::Memory::At(size_t idx) const
{
	if (idx == 0) // null is reserved
		throw ExceptionError("Segmentation fault.");
	else if (idx > m_buffer.size() - sizeof(Type))
		throw ExceptionError("Runtime error: out of memory bounds read.");
	return reinterpret_cast<Type const&>(m_buffer[idx]);
}

#endif
