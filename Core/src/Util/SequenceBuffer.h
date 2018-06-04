#ifndef SEQUENCEBUFFER_H
#define SEQUENCEBUFFER_H

#include <iostream>
#include "Platform.h"

#define EMPTY_SEQUENCE_ENTRY 0xFFFFFFFF

template <typename T>
class CSequenceBuffer
{

public:

	CSequenceBuffer(int size) 
		: m_bufferSize(size),
		m_sequence(0)
	{
		UDP_TRAP(size > 0);

		m_buffer = (T*) malloc(size * sizeof(T));
		m_entrySequence = (uint32_t*)malloc(size * sizeof(uint32_t));

		Reset();
	}

	~CSequenceBuffer()
	{
		free(m_buffer);
	}

	/* 
		Reset the sequence buffer
	*/
	void Reset()
	{
		m_sequence = 0;
		memset(m_entrySequence, 0xFF, sizeof(uint32_t) * m_bufferSize);
	}

	T* Insert(uint16_t sequence)
	{
		uint16_t index = GetIndex(sequence);
		m_entrySequence[index] = sequence;
		return &m_buffer[index];
	}

	void Remove(uint16_t sequence)
	{
		m_entrySequence[GetIndex(sequence)] = EMPTY_SEQUENCE_ENTRY;
	}

	bool Available(uint16_t sequence) const 
	{
		return m_entrySequence[GetIndex(sequence)] == EMPTY_SEQUENCE_ENTRY;
	}

	bool Exists(uint16_t sequence) const 
	{
		return m_entrySequence[GetIndex(sequence)] == uint32_t(sequence);
	}

	T* Find(uint16_t sequence)
	{
		uint16_t index = GetIndex(sequence);
		if (m_entrySequence[index] == uint32_t(sequence))
		{
			return &m_buffer[index];
		}
		
		return NULL;
	}

	uint16_t GetIndex(uint16_t sequence) const
	{
		return sequence % m_bufferSize;
	}

	int GetSize() const
	{
		return m_bufferSize;
	}

private:
	T* m_buffer;
	int m_bufferSize;
	
	uint16_t m_sequence; // Most recent sequence number added to the buffer
	uint32_t * m_entrySequence; // Array of sequence numbers. Set to 0xFFFFFFFF
};

#endif 