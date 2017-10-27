/**
 * @file lz_allocator.cc
 * @date 2016/04/11 10:13:23
 * @author lissa,lissaagainster@yahoo.com
 * @copyright lissa
 * @brief 
 */

#ifndef __LZ_ALLOCATOR_H
#define __LZ_ALLOCATOR_H
#include <utility>
#include <cstdio>
#include <cstdarg>
#include "lz_palloc.h"
namespace lz {

template<typename _Ty>
class _allocator_base {
 public:
  typedef _Ty                   value_type;
  typedef value_type*           pointer;
  typedef const value_type*     const_pointer;
  typedef value_type&           reference;
  typedef const value_type&     const_reference;
  typedef size_t                size_type;
  _allocator_base() {
    nref_++;
    /*
    __log(LZ_LOG_DEBUG, NULL, 0, "construct %p:%p+%d", this, pool_, nref_);
    */
  };

  _allocator_base(const _allocator_base<_Ty>& __b) {
    nref_++;
    /*
    __log(LZ_LOG_DEBUG, NULL, 0, "copy construct same %p %p:%p+%d",
         this, &__b, pool_, nref_ );
    */
  };

  template<typename _Ty1>
  _allocator_base(const _allocator_base<_Ty1>& __b) {
    nref_++;
    /*
    __log(LZ_LOG_DEBUG, NULL, 0, "copy construct difference %p %p:%p+%d",
         this, &__b, pool_, nref_ );
    */
  };

  pointer _Allocate(size_t __n, _Ty* = 0, bool try_aligned_allocation = true) {
    if(!pool_) {
      _Init();
    }
    /*
    __log(LZ_LOG_DEBUG, NULL, 0, "allocate memory %p:%p+%d %zu",
         this, pool_, nref_, __n * sizeof(value_type));
    */
    return static_cast<pointer>(lz_palloc(pool_, __n* sizeof(value_type)));
  };

  void _Deallocate(_Ty* __p, size_t __n) {}

  virtual ~_allocator_base() {
    _Destroy();
  };
 protected:
  static void __log(int level, lz_log_t* log,
                   int errno_no,
                   const char* fmt, ...) {
#ifdef DEBUG
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    printf("\t");
    va_end(args);
#endif
  };
 private:
  static lz_log_t log_;
  static lz_log_handler_t handler_;
  static lz_pool_t* pool_;
  static int nref_;
  void _Init() {
    log_.h = &handler_;
    handler_.log_handler = __log;
    pool_ = lz_create_pool(LZ_DEFAULT_POOL_SIZE, &log_);
  };
  void _Destroy() {
    nref_--;
    if (0 == nref_ && pool_) {
      /*
      __log(LZ_LOG_DEBUG, NULL, 0, "destroy pool %p:%p+%d", this, pool_, nref_);
      */
      lz_destroy_pool(pool_);
      pool_ = NULL;
    }
  };
};

template<typename _Ty> int              _allocator_base<_Ty>::nref_ = 0;
template<typename _Ty> lz_pool_t*       _allocator_base<_Ty>::pool_ = NULL;
template<typename _Ty> lz_log_t         _allocator_base<_Ty>::log_;
template<typename _Ty> lz_log_handler_t _allocator_base<_Ty>::handler_;

template<typename _Ty>
class lz_allocator : public _allocator_base<_Ty> {
 public:
  typedef _Ty                   value_type;
  typedef value_type*           pointer;
  typedef const value_type*     const_pointer;
  typedef value_type&           reference;
  typedef const value_type&     const_reference;
  typedef size_t                size_type;
  typedef ptrdiff_t             difference_type;

  template<typename _Ty1>
  struct rebind {
    typedef lz_allocator<_Ty1> other;
  };

  lz_allocator() throw() : _allocator_base<_Ty>() { };

  lz_allocator(const lz_allocator<_Ty>& __alloc) throw() :
      _allocator_base<_Ty>(__alloc) { };

  template<typename _Ty1>
  lz_allocator(const lz_allocator<_Ty1>& __alloc) throw() :
      _allocator_base<_Ty>(__alloc) { };

  ~lz_allocator() { };

  pointer allocate(size_type __n, const void* = 0) {
    return this->_Allocate(__n);
  };

  void deallocate(_Ty* __p, size_t __n) { this->_Deallocate(__p, __n); };

#if __cplusplus >= 201103L
  template<typename _ObjTy, typename ... _Tys>
  void construct (_ObjTy* __p, _Tys&&... __args) {
    ::new((void*)__p) _ObjTy(__args...);
  }
#else
  void construct(pointer __p, const _Ty& __val) {
    ::new((void*)__p) _Ty(__val);
  }
#endif

  template<typename _UTy> void destroy(_UTy* __p) { __p->~_UTy(); };

  size_type max_size() const { return size_t(-1) / sizeof(value_type); }
 protected:
 private:
};

template<typename _Ty1, typename _Ty2> inline
bool operator == (const lz_allocator<_Ty1>&, const lz_allocator<_Ty2>&) {
  return true;
}

template<typename _Ty1, typename _Ty2> inline
bool operator != (const lz_allocator<_Ty1>&, const lz_allocator<_Ty2>&) {
  return false;
}

} /** namespace lz */

#endif /** __LZ_ALLOCATOR_H */
