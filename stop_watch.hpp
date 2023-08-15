// author : Mauricio Gomes
// license: MIT (https://opensource.org/licenses/MIT)

#ifndef STOP_WATCH_HPP_INCLUDED
#define STOP_WATCH_HPP_INCLUDED

#include "object_factory.hpp"
#include <chrono>

namespace pensar_digital
{
    namespace cpplib
    {

    // StopWatch: measures time intervals in nanoseconds by default (but it is configurable) using std::chrono.
	template <typename Resolution = std::chrono::nanoseconds>
    class StopWatch : public Object
    {
		private:
			typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint;
			inline static const TimePoint ZERO = TimePoint ();
			TimePoint mstart;
			TimePoint mlast;
			TimePoint mmark;
			bool mrunning;
		public:
			typedef long long ELAPSED_TYPE;

			// Constructor
			StopWatch(bool start_now = true) 
				: mstart (start_now ? std::chrono::steady_clock::now() : ZERO), mlast (ZERO), mrunning (start_now) {}

			// Restart the timer
			inline void start () { mstart = std::chrono::steady_clock::now(); mrunning = true; }

			// Reset the timer.
			inline void reset () { mstart = std::chrono::steady_clock::now(); mlast = ZERO; mrunning = true; }

			inline void stop () { mlast = std::chrono::steady_clock::now(); mrunning = false; }

			// Get elapsed time in nanoseconds
			inline ELAPSED_TYPE elapsed ()
			{
				return std::chrono::duration_cast<Resolution> (mrunning ? std::chrono::steady_clock::now() - mstart : mlast - mstart).count ();
			}

			inline void mark () { mmark = std::chrono::steady_clock::now(); }	

			inline ELAPSED_TYPE elapsed_since_mark ()
			{
				return std::chrono::duration_cast<Resolution> (mrunning ? std::chrono::steady_clock::now() - mmark : mlast - mmark).count();
			}

			inline ELAPSED_TYPE now ()
			{
				return std::chrono::duration_cast<Resolution> (std::chrono::steady_clock::now() - ZERO).count();
			}

			String elapsed_formatted(ELAPSED_TYPE elapsed_nanoseconds)
			{
				ELAPSED_TYPE elapsed = elapsed_nanoseconds;
				ELAPSED_TYPE hours = elapsed / 3600000000000;
				elapsed -= hours * 3600000000000;
				ELAPSED_TYPE minutes = elapsed / 60000000000;
				elapsed -= minutes * 60000000000;
				ELAPSED_TYPE seconds = elapsed / 1000000000;
				elapsed -= seconds * 1000000;
				ELAPSED_TYPE milliseconds = elapsed / 1000000;
				elapsed -= milliseconds * 1000000;
				ELAPSED_TYPE microseconds = elapsed / 1000;
				elapsed -= microseconds * 1000;
				String result = pd::pad_left0 (hours, 2) + ":";
				result += pd::pad_left0 (minutes, 2) + ":";
				result += pd::pad_left0 (seconds, 2) + ".";
				result += pd::pad_left0 (milliseconds, 3) + ".";
				result += pd::pad_left0 (microseconds, 3) + ".";
				result += pd::pad_left0 (elapsed, 3);

				return result;
			}

			// Get elapsed as a formatted string (hh:mm:ss.mmm)
			String elapsed_formatted ()
			{
				return elapsed_formatted (elapsed ());
			}
			String elapsed_since_mark_formatted ()
			{
				return elapsed_formatted(elapsed_since_mark ());
			}
	};


	} // namespace cpplib
} // namespace pensar_digitalw
#endif // STOP_WATCH_HPP_INCLUDED
