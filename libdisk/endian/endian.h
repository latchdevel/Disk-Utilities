// endian.h
// 
// This provides the endian conversion functions form endian.h on Windows, Linux, *BSD, and Mac OS X. 
// You still need to use -std=gnu99 instead of -std=c99 for gcc. The functions might actually be macros. 
// Functions: htobe16, htole16, be16toh, le16toh, htobe32, htole32, be32toh, le32toh, htobe64, htole64, be64toh, le64toh.
//
// I, Mathias Panzenböck, place this file hereby into the public domain. Use
// it at your own risk for whatever you like. In case there are
// jurisdictions that don't support putting things in the public domain you
// can also consider it to be "dual licensed" under the BSD, MIT and Apache
// licenses, if you want to. This code is trivial anyway. Consider it an
// example on how to get the endian conversion functions on different
// platforms.
// https://gist.github.com/panzi/6856583

/**
* @file   endianness.h
* @brief  Convert Endianness of shorts, longs, long longs, regardless of architecture/OS
*
* Defines (without pulling in platform-specific network include headers):
* bswap16, bswap32, bswap64, ntoh16, hton16, ntoh32 hton32, ntoh64, hton64
*
* Should support linux / macos / solaris / windows.
* Supports GCC (on any platform, including embedded), MSVC2015, and clang,
* and should support intel, solaris, and ibm compilers as well.
*
* Released under MIT license
*/

#ifndef _ENDIANNESS_H
#define _ENDIANNESS_H

#include <stdlib.h>
#include <stdint.h>

/* Detect platform endianness at compile time */

// If boost were available on all platforms, could use this instead to detect endianness
// #include <boost/predef/endian.h>

/* Define custom __has macros, because normal ones still breaks in some cases*/
#ifdef __has_builtin
#define compiler_macro__has_builtin __has_builtin
#else
#define compiler_macro__has_builtin(x) 0
#endif

#ifdef __has_include
#define compiler_macro__has_include __has_include
#else
#define compiler_macro__has_include(x) 0
#endif

// When available, these headers can improve platform endianness detection
#if compiler_macro__has_include(<endian.h>)
#  include <endian.h> // gnu libc normally provides, linux
#elif compiler_macro__has_include(<machine/endian.h>)
#  include <machine/endian.h> //open bsd, macos
#elif compiler_macro__has_include(<sys/param.h>)
#  include <sys/param.h> // mingw, some bsd (not open/macos)
#elif compiler_macro__has_include(<sys/isadefs.h>)
#  include <sys/isadefs.h> // solaris
#endif

#if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#  if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) || \
      defined(__BYTE_ORDER) && (__BYTE_ORDER == __BIG_ENDIAN) || \
      defined(_BYTE_ORDER) && (_BYTE_ORDER == _BIG_ENDIAN) || \
      defined(BYTE_ORDER) && (BYTE_ORDER) == BIG_ENDIAN || \
      defined(_BIG_ENDIAN) || \
      defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || \
      defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__) || \
      defined(_M_PPC)
     //#warning "__BIG_ENDIAN__"
#    define __BIG_ENDIAN__
#  endif
#  if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) /* gcc */ || \
        defined(__BYTE_ORDER) && (__BYTE_ORDER == __LITTLE_ENDIAN) /* linux header */ || \
        defined(_BYTE_ORDER) && (_BYTE_ORDER == _LITTLE_ENDIAN) || \
        defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN) /* mingw header */ ||  \
        defined(_LITTLE_ENDIAN) || /* solaris */ \
        defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || \
        defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || \
        defined(_M_IX86) || defined(_M_X64) || defined(_M_IA64) || /* msvc for intel processors */ \
        defined(_M_ARM) /* msvc code on arm executes in little endian mode */
     //#warning "__LITTLE_ENDIAN__"
#    define __LITTLE_ENDIAN__
#  endif
#endif

#if defined(__LITTLE_ENDIAN__) && defined(__BIG_ENDIAN__)
#  error Both __LITTLE_ENDIAN__ and __BIG_ENDIAN__ have been defined!!!
#endif

/* Define byte-swap functions, using fast processor-native built-ins where possible */

#if defined(_MSC_VER) // needs to be first because msvc doesn't short-circuit after failing defined(__has_builtin)
#  define bswap16(x)     _byteswap_ushort((x))
#  define bswap32(x)     _byteswap_ulong((x))
#  define bswap64(x)     _byteswap_uint64((x))
#elif (compiler_macro__has_builtin(bswap16)) || /* for clang or future gcc */ \
    (__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
#  define bswap16(x)     __builtin_bswap16((x))
#  define bswap32(x)     __builtin_bswap32((x))
#  define bswap64(x)     __builtin_bswap64((x))
#else
    /* even in this case, compilers often optimize by using native instructions */
    static inline uint16_t bswap16(uint16_t x) {
           return (( x  >> 8 ) & 0xffu ) | (( x  & 0xffu ) << 8 );
       }
    static inline uint32_t bswap32(uint32_t x) {
        return ((( x & 0xff000000u ) >> 24 ) |
                (( x & 0x00ff0000u ) >> 8  ) |
                (( x & 0x0000ff00u ) << 8  ) |
                (( x & 0x000000ffu ) << 24 ));
    }
    static inline uint64_t bswap64(uint64_t x) {
        return ((( x & 0xff00000000000000ull ) >> 56 ) |
                (( x & 0x00ff000000000000ull ) >> 40 ) |
                (( x & 0x0000ff0000000000ull ) >> 24 ) |
                (( x & 0x000000ff00000000ull ) >> 8  ) |
                (( x & 0x00000000ff000000ull ) << 8  ) |
                (( x & 0x0000000000ff0000ull ) << 24 ) |
                (( x & 0x000000000000ff00ull ) << 40 ) |
                (( x & 0x00000000000000ffull ) << 56 ));
    }
#endif


/* Defines network - host byte swaps as needed depending upon platform endianness */
// note that network order is big endian)

#if defined(__LITTLE_ENDIAN__)
#  define ntoh16(x)     bswap16((x))
#  define hton16(x)     bswap16((x))
#  define ntoh32(x)     bswap32((x))
#  define hton32(x)     bswap32((x))
#  define ntoh64(x)     bswap64((x))
#  define hton64(x)     bswap64((x))
#elif defined(__BIG_ENDIAN__)
#  define ntoh16(x)     (x)
#  define hton16(x)     (x)
#  define ntoh32(x)     (x)
#  define hton32(x)     (x)
#  define ntoh64(x)     (x)
#  define hton64(x)     (x)
#  else
#    warning "UNKNOWN Platform / endianness; network / host byte swaps not defined."
#endif

#endif // _ENDIANNESS_H