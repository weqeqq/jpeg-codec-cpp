# JPEG Codec Library

A C++ library for encoding and decoding JPEG images with support for various color formats and chroma subsampling options.

## Features

- JPEG encoding and decoding
- Support for multiple color formats (RGB, BGR, RGBA, etc.)
- Chroma subsampling options (4:4:4, 4:2:2, 4:2:0, etc.)
- Quality control for encoding
- Cross-platform support (Windows, Linux, macOS)

## Dependencies

- CMake 3.23 or higher
- Git
- libjpeg-turbo (automatically fetched via CMake)

## Building

1. Clone the repository:
   ```bash
   git clone https://github.com/your-repo/jpeg-codec-cpp.git
   cd jpeg-codec-cpp
   ```

2. Create a build directory and run CMake:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. Build the library:
   ```bash
   cmake --build .
   ```

## Usage

### Basic Usage

```cpp
#include <jpeg-codec-cpp/jpeg.h>

int main() {
    // Decode a JPEG image
    std::vector<uint8_t> jpegData = /* your JPEG data */;
    int width, height;
    auto decodedData = JpegCodec::Decode(jpegData, height, width);

    // Encode an image to JPEG
    auto encodedData = JpegCodec::Encode(decodedData, height, width);
    return 0;
}
```

### Advanced Usage

```cpp
#include <jpeg-codec-cpp/jpeg.h>

int main() {
    // Decode with specific color format and sampling
    std::vector<uint8_t> jpegData = /* your JPEG data */;
    int width, height;
    JpegCodec::Sampling sampling;
    auto decodedData = JpegCodec::Decode(jpegData, height, width, JpegCodec::Color::Rgb, sampling);

    // Encode with specific parameters
    auto encodedData = JpegCodec::Encode(
        decodedData,
        height,
        width,
        JpegCodec::Color::Rgb,
        JpegCodec::Sampling::S420,
        85  // Quality level
    );
    return 0;
}
```

## API Reference

### Classes

#### `JpegCodec::Error`
Base error class for JPEG codec operations.

#### `JpegCodec::DecodeError`
Error class for JPEG decoding operations.

#### `JpegCodec::EncodeError`
Error class for JPEG encoding operations.

### Enums

#### `JpegCodec::Color`
Color format enumeration:
- `Rgb`: Red, Green, Blue
- `Bgr`: Blue, Green, Red
- `Rgbx`: Red, Green, Blue, padding
- `Bgrx`: Blue, Green, Red, padding
- `Xbgr`: Padding, Blue, Green, Red
- `Xrgb`: Padding, Red, Green, Blue
- `Gray`: Grayscale
- `Rgba`: Red, Green, Blue, Alpha
- `Bgra`: Blue, Green, Red, Alpha
- `Abgr`: Alpha, Blue, Green, Red
- `Argb`: Alpha, Red, Green, Blue
- `Cmyk`: Cyan, Magenta, Yellow, Key (black)

#### `JpegCodec::Sampling`
Chroma subsampling enumeration:
- `S444`: 4:4:4 sampling (no subsampling)
- `S422`: 4:2:2 sampling (horizontal subsampling)
- `S420`: 4:2:0 sampling (horizontal and vertical subsampling)
- `SGray`: Grayscale sampling
- `S440`: 4:4:0 sampling (vertical subsampling)
- `S411`: 4:1:1 sampling
- `S441`: 4:4:1 sampling

### Functions

#### `Decode`
```cpp
ByteContainer Decode(
    const ByteContainer &data,
    int &row_count,
    int &column_count,
    Color color,
    Sampling &sampling
)
```
Decodes a JPEG image with specified color format and sampling information.

#### `Decode (default)`
```cpp
ByteContainer Decode(
    const ByteContainer &data,
    int &row_count,
    int &column_count
)
```
Decodes a JPEG image with default settings (RGBA color format).

#### `Encode`
```cpp
ByteContainer Encode(
    const ByteContainer &data,
    int row_count,
    int column_count,
    Color color,
    Sampling sampling,
    int quality
)
```
Encodes image data to JPEG format with specified parameters.

#### `Encode (default)`
```cpp
ByteContainer Encode(
    const ByteContainer &data,
    int row_count,
    int column_count
)
```
Encodes image data to JPEG format with default settings (RGBA color format, 4:4:4 sampling, quality 100).

## License

This project is licensed under the MIT License - see the LICENSE file for details.
