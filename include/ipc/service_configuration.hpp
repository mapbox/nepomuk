#ifndef NEPOMUK_IPC_SERVICE_CONFIGURATION_HPP_
#define NEPOMUK_IPC_SERVICE_CONFIGURATION_HPP_

#include <string>

namespace nepomuk
{
namespace ipc
{

class ServiceConfiguration
{
  public:
    ServiceConfiguration(std::string const &configuration_file, std::string dataset);

    std::string const &dataset() const;

  private:
    std::string _dataset;
};

} // namespace ipc
} // namespace nepomuk

#endif // NEPOMUK_IPC_SERVICE_CONFIGURATION_HPP_
