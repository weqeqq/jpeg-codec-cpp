#include <jpeg-codec-cpp/jpeg.h>
#include <turbojpeg.h>

#include <cstdint>
#include <memory>
#include <vector>

namespace JpegCodec {

namespace {

constexpr auto Pitch = 0;
constexpr auto Flags = 0;

struct TjDecompressDeleter {
  void operator()(tjhandle handle) const noexcept {
    if (handle) {
      tjDestroy(handle);
    }
  }
};

struct TjCompressDeleter {
  void operator()(tjhandle handle) const noexcept {
    if (handle) {
      tjDestroy(handle);
    }
  }
};

struct TjBufferDeleter {
  void operator()(unsigned char* buffer) const noexcept {
    if (buffer) {
      tjFree(buffer);
    }
  }
};

using TjDecompressor = std::unique_ptr<void, TjDecompressDeleter>;
using TjCompressor = std::unique_ptr<void, TjCompressDeleter>;
using TjBuffer = std::unique_ptr<unsigned char, TjBufferDeleter>;

}  // anonymous namespace

std::vector<std::uint8_t> Decode(const std::uint8_t* data, std::size_t length,
                                 std::size_t& row_count,
                                 std::size_t& column_count, Color color,
                                 Sampling& sampling) {
  TjDecompressor decompressor(tjInitDecompress());
  if (!decompressor) {
    throw DecodeError("Failed to initialize decompressor");
  }

  int width = 0;
  int height = 0;
  int subsamp = 0;

  if (tjDecompressHeader2(decompressor.get(), const_cast<std::uint8_t*>(data),
                          length, &width, &height, &subsamp) != 0) {
    throw DecodeError("Failed to decode JPEG header: " +
                      std::string(tjGetErrorStr2(decompressor.get())));
  }

  column_count = static_cast<std::size_t>(width);
  row_count = static_cast<std::size_t>(height);
  sampling = static_cast<Sampling>(subsamp);

  const auto pixel_size = tjPixelSize[static_cast<TJPF>(color)];
  std::vector<std::uint8_t> output(row_count * column_count * pixel_size);

  if (tjDecompress2(decompressor.get(), const_cast<std::uint8_t*>(data), length,
                    output.data(), width, Pitch, height,
                    static_cast<TJPF>(color), Flags) != 0) {
    throw DecodeError("Failed to decode JPEG: " +
                      std::string(tjGetErrorStr2(decompressor.get())));
  }

  return output;
}

std::vector<std::uint8_t> Encode(const std::uint8_t* data,
                                 std::size_t row_count,
                                 std::size_t column_count, Color color,
                                 Sampling sampling, int quality) {
  TjCompressor compressor(tjInitCompress());
  if (!compressor) {
    throw EncodeError("Failed to initialize compressor");
  }

  unsigned char* raw_buffer = nullptr;
  unsigned long buffer_size = 0;

  const int width = static_cast<int>(column_count);
  const int height = static_cast<int>(row_count);

  if (tjCompress2(compressor.get(), const_cast<std::uint8_t*>(data), width,
                  Pitch, height, static_cast<TJPF>(color), &raw_buffer,
                  &buffer_size, static_cast<TJSAMP>(sampling), quality,
                  Flags) != 0) {
    throw EncodeError("Failed to encode JPEG: " +
                      std::string(tjGetErrorStr2(compressor.get())));
  }
  TjBuffer buffer(raw_buffer);

  return std::vector<std::uint8_t>(buffer.get(), buffer.get() + buffer_size);
}

}  // namespace JpegCodec
