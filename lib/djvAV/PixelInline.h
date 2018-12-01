//------------------------------------------------------------------------------
// Copyright (c) 2018 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include <djvCore/Math.h>

namespace djv
{
    namespace AV
    {
        namespace Image
        {
            inline bool U10_S_MSB::operator == (const U10_S_MSB& value) const
            {
                return
                    value.r == r &&
                    value.g == g &&
                    value.b == b;
            }

            inline bool U10_S_MSB::operator != (const U10_S_MSB& value) const
            {
                return !(*this == value);
            }

            inline bool U10_S_LSB::operator == (const U10_S_LSB& value) const
            {
                return
                    value.r == r &&
                    value.g == g &&
                    value.b == b;
            }

            inline bool U10_S_LSB::operator != (const U10_S_LSB& value) const
            {
                return !(*this == value);
            }

            inline Pixel::Pixel()
            {}

            inline Pixel::Pixel(GLenum format, GLenum type) :
                _format(format),
                _type(type)
            {}

            inline GLenum Pixel::getFormat() const
            {
                return _format;
            }

            inline GLenum Pixel::getType() const
            {
                return _type;
            }

            inline bool Pixel::isValid() const
            {
                return _format != 0 && _type != 0;
            }

            inline size_t Pixel::getChannelCount() const
            {
                size_t out = 0;
                switch (_format)
                {
                case GL_RED:  out = 1; break;
                case GL_RG:   out = 2; break;
                case GL_RGB:  out = 3; break;
                case GL_RGBA: out = 4; break;
                }
                return out;
            }

            inline size_t Pixel::getBitDepth() const
            {
                size_t out = 0;
                switch (_type)
                {
                case GL_UNSIGNED_BYTE:           out = 8; break;
                case GL_UNSIGNED_SHORT:          out = 16; break;
                case GL_UNSIGNED_INT:            out = 32; break;
                case GL_HALF_FLOAT:              out = 16; break;
                case GL_FLOAT:                   out = 32; break;
                case GL_UNSIGNED_INT_10_10_10_2: out = 10; break;
                }
                return out;
            }

            inline size_t Pixel::getByteCount() const
            {
                size_t out = 0;
                const size_t channels = getChannelCount();
                switch (_type)
                {
                case GL_UNSIGNED_BYTE:
                case GL_UNSIGNED_SHORT:
                case GL_UNSIGNED_INT:
                case GL_HALF_FLOAT:
                case GL_FLOAT:
                    out = channels * getBitDepth() / 8;
                    break;
                case GL_UNSIGNED_INT_10_10_10_2:
                    out = 4;
                    break;
                }
                return out;
            }

            inline bool Pixel::operator == (const Pixel& other) const
            {
                return other._format == _format && other._type == _type;
            }

            inline bool Pixel::operator != (const Pixel& other) const
            {
                return !(other == *this);
            }

            inline bool Pixel::operator < (const Pixel& other) const
            {
                return _format < other._format || (!(other._format < _format) && _type < other._type);
            }

            inline Pixel getIntPixel(int channelCount, int bitDepth)
            {
                switch (channelCount)
                {
                case 1:
                    switch (bitDepth)
                    {
                    case  8: return Pixel(GL_RED, GL_UNSIGNED_BYTE);
                    case 16: return Pixel(GL_RED, GL_UNSIGNED_SHORT);
                    case 32: return Pixel(GL_RED, GL_UNSIGNED_INT);
                    }
                    break;
                case 2:
                    switch (bitDepth)
                    {
                    case  8: return Pixel(GL_RG, GL_UNSIGNED_BYTE);
                    case 16: return Pixel(GL_RG, GL_UNSIGNED_SHORT);
                    case 32: return Pixel(GL_RG, GL_UNSIGNED_INT);
                    }
                    break;
                case 3:
                    switch (bitDepth)
                    {
                    case  8: return Pixel(GL_RGB, GL_UNSIGNED_BYTE);
                    case 10: return Pixel(GL_RGB, GL_UNSIGNED_INT_10_10_10_2);
                    case 16: return Pixel(GL_RGB, GL_UNSIGNED_SHORT);
                    case 32: return Pixel(GL_RGB, GL_UNSIGNED_INT);
                    }
                    break;
                case 4:
                    switch (bitDepth)
                    {
                    case  8: return Pixel(GL_RGBA, GL_UNSIGNED_BYTE);
                    case 16: return Pixel(GL_RGBA, GL_UNSIGNED_SHORT);
                    case 32: return Pixel(GL_RGBA, GL_UNSIGNED_INT);
                    }
                    break;
                }
                return Pixel();
            }

            inline Pixel getFloatPixel(int channelCount, int bitDepth)
            {
                switch (channelCount)
                {
                case 1:
                    switch (bitDepth)
                    {
                    case 16: return Pixel(GL_RED, GL_HALF_FLOAT);
                    case 32: return Pixel(GL_RED, GL_FLOAT);
                    }
                    break;
                case 2:
                    switch (bitDepth)
                    {
                    case 16: return Pixel(GL_RG, GL_HALF_FLOAT);
                    case 32: return Pixel(GL_RG, GL_FLOAT);
                    }
                    break;
                case 3:
                    switch (bitDepth)
                    {
                    case 16: return Pixel(GL_RGB, GL_HALF_FLOAT);
                    case 32: return Pixel(GL_RGB, GL_FLOAT);
                    }
                    break;
                case 4:
                    switch (bitDepth)
                    {
                    case 16: return Pixel(GL_RGBA, GL_HALF_FLOAT);
                    case 32: return Pixel(GL_RGBA, GL_FLOAT);
                    }
                    break;
                }
                return Pixel();
            }

            inline void U8ToU10(U8_T in, U10_T & out)
            {
                out = in << 2;
            }

            inline void U8ToU16(U8_T in, U16_T & out)
            {
                out = in << 8;
            }

            inline void U8ToU32(U8_T in, U32_T & out)
            {
                out = in << 24;
            }

            inline void U8ToF16(U8_T in, F16_T & out)
            {
                out = in / static_cast<float>(U8Max);
            }

            inline void U8ToF32(U8_T in, F32_T & out)
            {
                out = in / static_cast<float>(U8Max);
            }

            inline void U10ToU8(U10_T in, U8_T & out)
            {
                out = in >> 2;
            }

            inline void U10ToU16(U10_T in, U16_T & out)
            {
                out = in << 6;
            }

            inline void U10ToU32(U10_T in, U32_T & out)
            {
                out = in << 22;
            }

            inline void U10ToF16(U10_T in, F16_T & out)
            {
                out = in / static_cast<float>(U10Max);
            }

            inline void U10ToF32(U10_T in, F32_T & out)
            {
                out = in / static_cast<float>(U10Max);
            }

            inline void U16ToU8(U16_T in, U8_T & out)
            {
                out = in >> 8;
            }

            inline void U16ToU10(U16_T in, U10_T & out)
            {
                out = in >> 6;
            }

            inline void U16ToU32(U16_T in, U32_T & out)
            {
                out = in << 16;
            }

            inline void U16ToF16(U16_T in, F16_T & out)
            {
                out = in / static_cast<float>(U16Max);
            }

            inline void U16ToF32(U16_T in, F32_T & out)
            {
                out = in / static_cast<float>(U16Max);
            }

            inline void U32ToU8(U32_T in, U8_T & out)
            {
                out = in >> 24;
            }

            inline void U32ToU10(U32_T in, U10_T & out)
            {
                out = in >> 22;
            }

            inline void U32ToU16(U32_T in, U16_T & out)
            {
                out = in >> 16;
            }

            inline void U32ToF16(U32_T in, F16_T & out)
            {
                out = in / static_cast<float>(U32Max);
            }

            inline void U32ToF32(U32_T in, F32_T & out)
            {
                out = in / static_cast<float>(U32Max);
            }

            inline void F16ToU8(F16_T in, U8_T & out)
            {
                out = static_cast<U8_T>(Core::Math::clamp(
                    static_cast<uint16_t>(in * U8Max),
                    static_cast<uint16_t>(U8Min),
                    static_cast<uint16_t>(U8Max)));
            }

            inline void F16ToU10(F16_T in, U10_T & out)
            {
                out = static_cast<U10_T>(Core::Math::clamp(
                    static_cast<uint16_t>(in * U10Max),
                    static_cast<uint16_t>(U10Min),
                    static_cast<uint16_t>(U10Max)));
            }

            inline void F16ToU16(F16_T in, U16_T & out)
            {
                out = static_cast<U16_T>(Core::Math::clamp(
                    static_cast<uint32_t>(in * U16Max),
                    static_cast<uint32_t>(U16Min),
                    static_cast<uint32_t>(U16Max)));
            }

            inline void F16ToU32(F16_T in, U32_T & out)
            {
                out = static_cast<U32_T>(Core::Math::clamp(
                    static_cast<uint64_t>(in * U32Max),
                    static_cast<uint64_t>(U32Min),
                    static_cast<uint64_t>(U32Max)));
            }

            inline void F16ToF32(F16_T in, F32_T & out)
            {
                out = in;
            }

            inline void F32ToU8(F32_T in, U8_T & out)
            {
                out = static_cast<U8_T>(Core::Math::clamp(
                    static_cast<uint16_t>(in * U8Max),
                    static_cast<uint16_t>(U8Min),
                    static_cast<uint16_t>(U8Max)));
            }

            inline void F32ToU10(F32_T in, U10_T & out)
            {
                out = static_cast<U10_T>(Core::Math::clamp(
                    static_cast<uint16_t>(in * U10Max),
                    static_cast<uint16_t>(U10Min),
                    static_cast<uint16_t>(U10Max)));
            }

            inline void F32ToU16(F32_T in, U16_T & out)
            {
                out = static_cast<U16_T>(Core::Math::clamp(
                    static_cast<uint32_t>(in * U16Max),
                    static_cast<uint32_t>(U16Min),
                    static_cast<uint32_t>(U16Max)));
            }

            inline void F32ToU32(F32_T in, U32_T & out)
            {
                out = static_cast<U32_T>(Core::Math::clamp(
                    static_cast<uint64_t>(in * U32Max),
                    static_cast<uint64_t>(U32Min),
                    static_cast<uint64_t>(U32Max)));
            }

            inline void F32ToF16(F32_T in, F16_T & out)
            {
                out = in;
            }

        } // namespace Image
    } // namespace AV
} // namespace djv