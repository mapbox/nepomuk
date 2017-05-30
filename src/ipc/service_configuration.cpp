#include "ipc/service_configuration.hpp"

namespace nepomuk
{
namespace ipc
{

ServiceConfiguration::ServiceConfiguration(std::string const &configuration_file,
                                           std::string dataset)
    : _dataset(dataset)
{
}

std::string const &ServiceConfiguration::dataset() const { return _dataset; }

} // namespace ipc
} // namespace nepomuk
