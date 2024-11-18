#ifndef BYTE_ARRAY_HPP
#define BYTE_ARRAY_HPP

#include "constant.hpp"

#include <memory> // std::unique_ptr

namespace pensar_digital
{
	namespace cpplib
	{
		class ByteArray;
		using ByteArrayPtr = std::unique_ptr<ByteArray>;

		class ByteArray
		{
		private:
			Byte* mdata;
			size_t msize;

		public:
			ByteArray(const size_t size = 0) : mdata(new Byte[size]), msize(size) {}
			ByteArray(const ByteArray& other) : mdata(new Byte[other.msize]), msize(other.msize)
			{
				for (size_t i = 0; i < msize; ++i)
					mdata[i] = other.mdata[i];
			}
			ByteArray(ByteArray&& other) noexcept : mdata(other.mdata), msize(other.msize)
			{
				other.mdata = nullptr;
				other.msize = 0;
			}
			~ByteArray() { delete[] mdata; }

			Byte& operator[] (const size_t index) { return mdata[index]; }
			const Byte& operator[] (const size_t index) const { return mdata[index]; }

			ByteArray& operator= (const ByteArray& other)
			{
				if (this != &other)
				{
					delete[] mdata;
					mdata = new Byte[other.msize];
					msize = other.msize;
					memcpy(mdata, other.mdata, msize);
				}
				return *this;
			}

			ByteArray& operator= (ByteArray&& other) noexcept
			{
				if (this != &other)
				{
					delete[] mdata;
					mdata = other.mdata;
					msize = other.msize;
					other.mdata = nullptr;
					other.msize = 0;
				}
				return *this;
			}

			ByteArray& copy (const ByteArray& ba, const size_t offset = 0)
			{
				if ((ba.size () + offset) > msize)
				{
					delete[] mdata;
					msize = ba.size() + offset;
					mdata = new Byte[msize];
				}
				memcpy(mdata + offset, ba.data (), size);
				return *this;
			}

			ByteArray& append (const ByteArray& ba)
			{
				i
				Byte* new_data = new Byte[msize + ba.size()];
				memcpy(new_data, mdata, msize);
				memcpy(new_data + msize, ba.data(), ba.size());	
				delete[] mdata;	
				mdata = new_data;
				msize += ba.size();	
				return *this;
			}


				size_t size() const { return msize; }
			Byte* data() { return mdata; }
		};
	}
}
#endif