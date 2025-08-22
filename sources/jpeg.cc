
#include <jpeg-codec-cpp/jpeg.h>
#include <turbojpeg.h>

namespace JpegCodec {

/* clang-format off */

constexpr auto Pitch = 0;
constexpr auto Flags = 0;

JPEG_CODEC_EXPORT ByteContainer Decode(
  const ByteContainer &data,
  int                 &row_count,
  int                 &column_count,
  Color               color,
  Sampling            &sampling
) {
  auto decompressor = tjInitDecompress();
  if (tjDecompressHeader2(
    decompressor,
    const_cast<std::uint8_t *>(data.data()),
    data.size(),
    &column_count,
    &row_count,
    reinterpret_cast<int *>(&sampling)
  )) {
    throw DecodeError("Failed to decode JPEG header: " + std::string(tjGetErrorStr2(decompressor)));
  }
  ByteContainer output(
    row_count    *
    column_count *
    tjPixelSize[static_cast<TJPF>(color)]
  );
  if (tjDecompress2(
    decompressor,
    const_cast<std::uint8_t *>(data.data()),
    data.size(),
    output.data(),
    column_count,
    Pitch,
    row_count,
    static_cast<TJPF>(color),
    Flags)
  ) {
    throw DecodeError("Failed to decode JPEG: " + std::string(tjGetErrorStr2(decompressor)));
  }
  tjDestroy(decompressor);
  return output;
}

ByteContainer Encode(
  const ByteContainer &data,
  int                  row_count,
  int                  column_count,
  Color                color,
  Sampling             sampling,
  int                  quality
) {
  auto compressor = tjInitCompress();

  unsigned char *buffer = nullptr;
  unsigned long  length = 0;

  if (tjCompress2(
    compressor,
    const_cast<std::uint8_t *>(data.data()),
    column_count,
    Pitch,
    row_count,
    static_cast<TJPF>(color),
    &buffer,
    &length,
    static_cast<TJSAMP>(sampling),
    quality,
    Flags
  )) {
    throw EncodeError("Failed to encode JPEG: " + std::string(tjGetErrorStr2(compressor)));
  }
  ByteContainer output(buffer, buffer + length);
  tjFree(buffer);
  tjDestroy(compressor);
  return output;
}

/* clang-format on */
}  // namespace JpegCodec
