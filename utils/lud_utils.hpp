#ifndef LUD_UTILS_HEADER
#define LUD_UTILS_HEADER

#include <string>
#include <string_view>
#include <optional>
#include <iostream>
#include <fstream>
#include <vector>
#include <bit>

namespace Lud {

class Slurper 
{
public:
	Slurper();
	Slurper(const std::string_view& filename, std::ios_base::openmode mode);
	Slurper(const std::string& filename, std::ios_base::openmode mode);
	Slurper(const char* filename, std::ios_base::openmode mode);

	Slurper(const Slurper& slurper) = delete;
	Slurper(Slurper&& slurper) noexcept = default;

	Slurper& operator=(const Slurper& other) = delete;
	Slurper& operator=(Slurper&& other);

	~Slurper();


	std::vector<std::string> ReadLinesAsVector();

	std::optional<std::string> ReadLine();
	std::optional<std::string> Read(size_t chars);

	template<class T> std::optional<T> ReadStructure();
	template<class T> void ReadToStructure(T& t);

	size_t Where();
	size_t Move(size_t pos);
	size_t Move(size_t offset, std::ios_base::seekdir dir);

	void Reset();

	bool IsOpen() const;


	bool Open(const std::string_view& filename, std::ios_base::openmode mode);
	bool Open(const std::string& filename, std::ios_base::openmode mode);
	bool Open(const char* filename, std::ios_base::openmode mode);

	void Close();

	bool HasSpace(size_t size);

private:
	std::ifstream file;
};

}

#ifdef LUD_SLURPER_IMPLEMENTATION

namespace Lud {

Slurper::Slurper()
{

}

Slurper::Slurper(const std::string_view& filename, std::ios_base::openmode mode = std::ios_base::in) 
	: Slurper()
{ 
	Open(filename, mode); 
}
Slurper::Slurper(const std::string& filename, std::ios_base::openmode mode = std::ios_base::in) 
	: Slurper()
{ 
	Open(filename, mode); 
}
Slurper::Slurper(const char* filename, std::ios_base::openmode mode = std::ios_base::in) 
	: Slurper()
{ 
	Open(filename, mode); 
}

Slurper::~Slurper()
{
	Close();
}

Slurper& Slurper::operator=(Slurper&& other)
{
	*this = std::move(other);
	return *this;
}


bool Lud::Slurper::Open(const std::string_view& filename, std::ios_base::openmode mode = std::ios_base::in) 
{
	if (IsOpen()) {
		Close();
	}
	file.open(std::string(filename), mode);
	return IsOpen();
}
bool Lud::Slurper::Open(const std::string& filename, std::ios_base::openmode mode = std::ios_base::in) 
{ 
	if (IsOpen()) {
		Close();
	}
	file.open(filename, mode);
	return IsOpen();
} 
bool Lud::Slurper::Open(const char* filename, std::ios_base::openmode mode = std::ios_base::in) 
{ 
	if (IsOpen()) {
		Close();
	}
	file.open(filename, mode);
	return IsOpen();
}

bool Slurper::IsOpen() const
{
	return file.is_open();
}

void Slurper::Reset() 
{
	file.clear();
	file.seekg(0, std::ios::beg);
}


void Slurper::Close()
{
	file.close();
}

std::optional<std::string> Slurper::Read(const size_t chars)
{
	if (!HasSpace(chars)) {
		return std::nullopt;
	}
	char* buffer = new char[chars];

	file.read(buffer, chars);
	std::string text(buffer);

	delete[] buffer;

	return text;
}

template<class T> std::optional<T> Slurper::ReadStructure()
{
	T t;
	if (!HasSpace(sizeof(t))) {
		return std::nullopt;
	}
	ReadToStructure<T>(t);
	return t;

}

template<class T> void Slurper::ReadToStructure(T& t)
{
	file.read(std::bit_cast<char*>(&t), sizeof(t));
}

std::optional<std::string> Slurper::ReadLine()
{
	std::string line;
	if (std::getline(file, line)) {
		return line;
	}
	return std::nullopt;
}

std::vector<std::string> Slurper::ReadLinesAsVector()
{
	std::vector<std::string> lines;

	for(std::string line; std::getline(file, line);) {
		lines.emplace_back(line);
	}
	return lines;
}

size_t Slurper::Where()  
{
	return file.tellg();
}

size_t Slurper::Move(size_t pos)
{
	const size_t current = Where();
	file.seekg(pos);
	return current;
}

size_t Slurper::Move(size_t offset, std::ios_base::seekdir dir)
{
	const size_t current = Where();
	file.seekg(offset, dir);
	return current;
}

bool Slurper::HasSpace(size_t size)
{
	const size_t current = Move(0, std::ios::end);
	const size_t length = Where();

	return size < length - current;
}


}
#endif//LUD_SLURPER_IMPLEMENATION


#endif//LUD_UTILS_HEADER