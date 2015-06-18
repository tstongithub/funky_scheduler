#include "logging.hpp"

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

namespace logging = boost::log;

void init_logging()
{
#ifdef NDEBUG
  logging::core::get()->
    set_filter(logging::trivial::severity >= logging::trivial::info);
#endif
}
