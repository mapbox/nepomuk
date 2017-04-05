#include "timetable/transfer_table.hpp"
#include "gtfs/stop.hpp"
#include "gtfs/transfer.hpp"
#include "timetable/exceptions.hpp"
#include "timetable/transfer_table_factory.hpp"

using namespace transit;
using namespace transit::timetable;
using namespace transit::gtfs;

// make sure we get a new main function here
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(check_input_validity)
{
    // cannot use empty vectors
    std::vector<Transfer> data;
    BOOST_CHECK_THROW(TransferTableFactory::produce(data.begin(), data.end()), InvalidInputError);

    Transfer first = {StopID{1}, StopID{0}, TransferType::LONG, 200};
    data.push_back(first);

    BOOST_CHECK_NO_THROW(TransferTableFactory::produce(data.begin(), data.end()));

    Transfer invalid = {StopID{1}, StopID{2}, TransferType::LONG, boost::none};
    data.push_back(invalid);
    BOOST_CHECK_THROW(TransferTableFactory::produce(data.begin(), data.end()), InvalidInputError);

    data.pop_back();
    Transfer not_sorted = {StopID{0}, StopID{1}, TransferType::LONG, 200};
    data.push_back(not_sorted);
    BOOST_CHECK_THROW(TransferTableFactory::produce(data.begin(), data.end()), InvalidInputError);

    data.pop_back();
    Transfer second = {StopID{1}, StopID{3}, TransferType::LONG, 300};
    Transfer third = {StopID{1}, StopID{4}, TransferType::LONG, 400};
    Transfer fourth = {StopID{2}, StopID{6}, TransferType::LONG, 400};
    Transfer fifth = {StopID{2}, StopID{7}, TransferType::LONG, 400};
    data.push_back(second);
    data.push_back(third);
    data.push_back(fourth);
    data.push_back(fifth);

    BOOST_CHECK_NO_THROW(TransferTableFactory::produce(data.begin(), data.end()));

    auto const transfer_table = TransferTableFactory::produce(data.begin(), data.end());

    auto const range = transfer_table.get(StopID{1});
    BOOST_CHECK_EQUAL(std::distance(range.begin(), range.end()), 3);
    auto const srange = transfer_table.get(StopID{2});
    BOOST_CHECK_EQUAL(std::distance(srange.begin(), srange.end()), 2);
    auto const trange = transfer_table.get(StopID{0});
    BOOST_CHECK_EQUAL(std::distance(trange.begin(), trange.end()), 0);
}
