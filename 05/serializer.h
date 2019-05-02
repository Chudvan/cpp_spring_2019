#pragma once
#include <iostream>

enum class Error
{
	NoError,
	CorruptedArchive
};

class Serializer
{
	static constexpr char Separator = ' ';
public:
	explicit Serializer(std::ostream& out)
		: out_(out)
	{
	}

	template <class T>
	Error save(T& object)
	{
		return object.serialize(*this);
	}

	template <class... ArgsT>
	Error operator()(ArgsT&&... args)
	{
		return process(std::forward<ArgsT>(args)...);
	}

private:
	std::ostream& out_;

	Error out_object(uint64_t val) {
		out_ << val << Separator;
		return Error::NoError;
	}

	Error out_object(bool val) {
		out_ << (val ? "true" : "false") << Separator;
		return Error::NoError;
	}

	template <class T>
	Error out_object(T&& val) {
		return Error::CorruptedArchive;
	}

	template <class T>
	Error process(T&& val) {
		return out_object(std::forward<T>(val));
	}

	template <class T, class... Args>
	Error process(T&& val, Args&&... args) {
		if (out_object(std::forward<T>(val)) == Error::CorruptedArchive)
			return Error::CorruptedArchive;
		return process(std::forward<Args>(args)...);
	}
};

class Deserializer
{
public:
	explicit Deserializer(std::istream& in)
		: in_(in)
	{
	}

	template <class... ArgsT>
	Error operator()(ArgsT&&... args)
	{
		return process(std::forward<ArgsT>(args)...);
	}

	template <class T>
	Error load(T& object)
	{
		return object.serialize(*this);
	}
private:
	std::istream& in_;

	template <class T>
	Error process(T&& value) {
		return in_object(std::forward<T>(value));
	}

	template <class T, class... Args>
	Error process(T&& value, Args&&... args) {
		if (in_object(std::forward<T>(value)) == Error::CorruptedArchive)
			return Error::CorruptedArchive;
		return(process(std::forward<Args>(args)...));
	}

	Error in_object(bool& value)
	{
		std::string text;
		in_ >> text;

		if (text == "true")
			value = true;
		else if (text == "false")
			value = false;
		else
			return Error::CorruptedArchive;

		return Error::NoError;
	}

	Error in_object(uint64_t& value)
	{
		std::string text;
		in_ >> text;
		uint64_t buffer = 0;
		for (auto sym : text) {
			if (sym >= '0' && sym <= '9') {
				buffer = buffer * 10 + (sym - '0');
			}
			else 
				return Error::CorruptedArchive;
		}
		value = buffer;
		return Error::NoError;
	}
};