#pragma once
#include <QCryptographicHash>

class Hash
{
public:
	Hash() = default;

	Hash(QByteArray hash) :
		m_hash(hash) {};

	Hash(const Hash& r) :
		m_hash(r.m_hash) {};

	Hash(Hash&& r) noexcept :
		m_hash(std::move(r.m_hash)) {};

	bool operator<(const Hash&) const;

	bool operator==(const Hash&) const;
private:
	QByteArray m_hash;
};
