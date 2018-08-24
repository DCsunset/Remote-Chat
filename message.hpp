#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <vector>
#include <cstring>

class Message
{
	public:
		static constexpr unsigned header_size = 4;

		Message(unsigned body_size_val = 0)
			: data_(header_size + body_size_val)
			{
				std::fill(data_.begin(), data_.begin() + header_size, 0);
			}

		char * data()
		{
			return data_.data();
		}
		char * body()
		{
			return data_.data() + header_size;
		}
		size_t size() const
		{
			return data_.size();
		}
		unsigned body_size() const
		{
			return data_.size() - header_size;
		}
		void body_size(unsigned size)
		{
			data_.resize(header_size + size);
		}
		void decode_header()
		{
			unsigned size;
			std::memcpy(&size, data(), header_size);
			body_size(size);
		}
		void encode_header()
		{
			unsigned size = body_size();
			std::memcpy(data(), &size, header_size);
		}

	private:
		std::vector<char> data_;
};

#endif
