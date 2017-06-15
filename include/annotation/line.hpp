#ifndef NEPOMUK_ANNOTATION_NAME_FROM_ID_HPP_
#define NEPOMUK_ANNOTATION_NAME_FROM_ID_HPP_

#include "id/dictionary.hpp"
#include "id/line.hpp"
#include "tool/container/dictionary.hpp"

#include <boost/optional.hpp>

#include <map>
#include <vector>

namespace nepomuk
{
namespace annotation
{

struct LineInfo
{
    boost::optional<DictionaryID> headsign;
    boost::optional<DictionaryID> name;
    boost::optional<DictionaryID> abbreviation;
    boost::optional<DictionaryID> text_color;
    boost::optional<DictionaryID> background_color;
};

// access to all things named in Nepomuk
class Line final
{
  public:
    LineInfo const &operator()(LineID const line) const;

  private:
    std::vector<LineInfo> line_information;

    friend class LineFactory;
};

} // namespace annotation
} // namespace nepomuk

#endif // NEPOMUK_ANNOTATION_NAME_FROM_ID_HPP_
