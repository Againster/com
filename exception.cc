#include "exception.h"
namespace lz {

const exception exception::NO_EXCEPTION;

exception::exception() : what_(""), other_(NULL){}

exception::exception(const string& what, const exception& other) :
    what_(what),
    other_(&other != &NO_EXCEPTION ? other.clone() : NULL) {
  /** @FIXED : Here the message of other_ was lost */
  if (other_) {
    what_ += string("\n") + other_->what();
  }
}

exception::exception(const exception& e) :
    std::exception(), what_(e.what()),
    other_(e.other_ == NULL ? NULL : e.other_->clone()){}

exception::~exception() throw(){ delete (other_);}

const char* exception::what() const throw(){ return (what_.c_str());}

const char* exception::what() throw(){ return (what_.c_str());}

const exception* exception::other() const throw(){ return (other_);}

const char* exception::name() const throw(){ return "exception";}

exception* exception::clone() const{ return new exception(*this);}


namespace exceptions{

/** invalid_argument */
invalid_argument::~invalid_argument() throw() {}
invalid_argument::invalid_argument(const exception& other)
  : exception("Invalid argument.", other) {}
exception* invalid_argument::clone() const{ return new invalid_argument(*this);}
const char* invalid_argument::name() const throw(){ return "invalid_argument";}


} /** namespace exceptions */

} /** namespace lz */
