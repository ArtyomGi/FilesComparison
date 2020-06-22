#include "Hash.h"

bool Hash::operator<(const Hash& r) const
{
	return m_hash < r.m_hash;
}

bool Hash::operator==(const Hash& r) const
{
	return m_hash == r.m_hash;
}