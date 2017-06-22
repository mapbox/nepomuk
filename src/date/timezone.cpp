#include "date/timezone.hpp"

// map timezone strings to their respective UTC offsets
#include <unicode/timezone.h>
#include <unicode/utypes.h>
#include <unicode/calendar.h>

#include <iostream>

namespace nepomuk
{
namespace date
{

boost::optional<std::int32_t> Timezone::offset(std::string const &timezone)
{
	auto *zone = icu::TimeZone::createTimeZone(timezone.c_str());
	std::int32_t standard_offset, daylight_saving_time_offset;
    UDate curDate;
    UErrorCode success = U_ZERO_ERROR;

    Calendar* calendar = Calendar::createInstance(success);
    curDate = calendar->getNow();

	zone->getOffset(curDate,true,standard_offset,daylight_saving_time_offset,success);

    if( success != U_ZERO_ERROR )
        std::cout << "OFfsets: " << standard_offset << " " << daylight_saving_time_offset << std::endl;
	
	return boost::none;
}

} // namespace date
} // namespace nepomuk
