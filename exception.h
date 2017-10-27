#pragma once
#include <stdexcept>
#include "misc.h"
namespace lz {

class LZ_EXPORT exception : public std::exception {
public:
  exception(const string& what, const exception& other = NO_EXCEPTION);
  exception(const exception& e);

  virtual ~exception() throw();
  /** Return a description of the error.
    *
    * @return error message
    */
  const char* what() const throw();
  /** Return a description of the error.
    *
    * @return error message
    */
  const char* what() throw();
  /** Return the next exception in the chain (encapsulated exception).
    *
    * @return next exception in the chain
    */
  const exception* other() const throw();
  /** Return a name identifying the exception.
    *
    * @return exception name
    */
  virtual const char* name() const throw();
  /** Clone this object.
    *
    * @return a new copy of this object
    */
  virtual exception* clone() const;
 protected:
  static const exception NO_EXCEPTION;
 private:
  string what_;
  exception* other_;
  exception();
};


/** List of all lz exceptions. */
namespace exceptions {

/** Bad argument was passed to the function.
  */
class LZ_EXPORT invalid_argument : public lz::exception
{
 public:
  invalid_argument(const exception& other = NO_EXCEPTION);
  ~invalid_argument() throw();

  exception* clone() const;
  const char* name() const throw();
};

}

} //namespace lz
