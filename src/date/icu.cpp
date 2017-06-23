#include "date/icu.hpp"

#include <unicode/putil.h>

namespace nepomuk
{
namespace date
{
namespace icu
{

// initialise ICU based on the mason data
void init()
{
    static bool called = false;
    if (!called)
    {
        called = true;
        u_setDataDirectory(ICU_DATA_DIRECTORY);
    }
}

} // namespace icu
} // namespace date
} // namespace nepomuk
