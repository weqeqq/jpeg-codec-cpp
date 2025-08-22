
#pragma once
/**
 * @file
 * @brief JPEG Codec library for encoding and decoding JPEG images
 */
/* clang-format off */
#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef JPEG_CODEC_SHARED
    #ifdef JPEG_CODEC_BUILD
      #define JPEG_CODEC_EXPORT __declspec(dllexport)
    #else
      #define JPEG_CODEC_EXPORT __declspec(dllimport)
    #endif
  #else
    #define JPEG_CODEC_EXPORT
  #endif
#else
  #ifdef JPEG_CODEC_SHARED
    #define JPEG_CODEC_EXPORT __attribute__((visibility("default")))
  #else
    #define JPEG_CODEC_EXPORT
  #endif
#endif
/* clang-format on */

#include <cstdint>
#include <stdexcept>
#include <vector>
/**
 * @namespace JpegCodec
 * @brief Main namespace for JPEG codec functionality
 */
namespace JpegCodec {
/**
 * @brief Base error class for JPEG codec operations
 */
struct Error : std::runtime_error {
  /**
   * @brief Construct a new Error object
   * @param message Error message describing the problem
   */
  Error(const std::string &message)
      : std::runtime_error("JpegCodecError: " + message) {}
};
/**
 * @brief Error class for JPEG decoding operations
 */
struct DecodeError : Error {
  using Error::Error;
};
/**
 * @brief Error class for JPEG encoding operations
 */
struct EncodeError : Error {
  using Error::Error;
};
/**
 * @brief Color format enumeration for image data
 */
enum class Color {
  Rgb,   ///< Red, Green, Blue
  Bgr,   ///< Blue, Green, Red
  Rgbx,  ///< Red, Green, Blue, padding
  Bgrx,  ///< Blue, Green, Red, padding
  Xbgr,  ///< Padding, Blue, Green, Red
  Xrgb,  ///< Padding, Red, Green, Blue
  Gray,  ///< Grayscale
  Rgba,  ///< Red, Green, Blue, Alpha
  Bgra,  ///< Blue, Green, Red, Alpha
  Abgr,  ///< Alpha, Blue, Green, Red
  Argb,  ///< Alpha, Red, Green, Blue
  Cmyk,  ///< Cyan, Magenta, Yellow, Key (black)
};
/**
 * @brief Chroma subsampling enumeration for JPEG compression
 */
enum class Sampling {  //
  S444,                ///< 4:4:4 sampling (no subsampling)
  S422,                ///< 4:2:2 sampling (horizontal subsampling)
  S420,                ///< 4:2:0 sampling (horizontal and vertical subsampling)
  SGray,               ///< Grayscale sampling
  S440,                ///< 4:4:0 sampling (vertical subsampling)
  S411,                ///< 4:1:1 sampling
  S441                 ///< 4:4:1 sampling
};
/**
 * @brief Type alias for byte container using std::vector<std::uint8_t>
 */
using ByteContainer = std::vector<std::uint8_t>;
/* clang-format off */
/**
 * @brief Decode a JPEG image with specified color format and sampling information
 * @param data Input JPEG data as a byte container
 * @param row_count Output parameter for image height in pixels
 * @param column_count Output parameter for image width in pixels
 * @param color Desired output color format
 * @param sampling Output parameter for detected chroma subsampling
 * @return Decoded image data as a byte container
 * @throws DecodeError if decoding fails
 */
JPEG_CODEC_EXPORT ByteContainer Decode(
  const ByteContainer &data,
  int                 &row_count,
  int                 &column_count,
  Color               color,
  Sampling            &sampling
);
/**
 * @brief Decode a JPEG image with default settings
 * @param data Input JPEG data as a byte container
 * @param row_count Output parameter for image height in pixels
 * @param column_count Output parameter for image width in pixels
 * @return Decoded image data as a byte container
 * @throws DecodeError if decoding fails
 */
inline ByteContainer Decode(
  const ByteContainer &data,
  int                 &row_count,
  int                 &column_count
) {
  Sampling sampling;
  return Decode(data, row_count, column_count, Color::Rgba, sampling);
}
/**
 * @brief Encode image data to JPEG format with specified parameters
 * @param data Input image data as a byte container
 * @param row_count Image height in pixels
 * @param column_count Image width in pixels
 * @param color Input image color format
 * @param sampling Chroma subsampling method to use
 * @param quality JPEG quality (0-100, higher values mean better quality)
 * @return Encoded JPEG data as a byte container
 * @throws EncodeError if encoding fails
 */
JPEG_CODEC_EXPORT ByteContainer Encode(
  const ByteContainer &data,
  int                  row_count,
  int                  column_count,
  Color                color,
  Sampling             sampling,
  int                  quality
);
/**
 * @brief Encode image data to JPEG format with default settings
 * @param data Input image data as a byte container
 * @param row_count Image height in pixels
 * @param column_count Image width in pixels
 * @return Encoded JPEG data as a byte container
 * @throws EncodeError if encoding fails
 */
inline ByteContainer Encode(
  const ByteContainer &data,
  int                  row_count,
  int                  column_count
) {
  return Encode(
    data,
    row_count,
    column_count,
    Color::Rgba,
    Sampling::S444,
    100
  );
}
/* clang-format on */
}  // namespace JpegCodec
