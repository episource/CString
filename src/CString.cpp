#include "CString.h"

CString CString::INVALID = CString(nullptr, INVALID_STRING_IDX);

int CString::length() const noexcept {
    if (!isAllocated()) {
        return -1;
    }

    return _lengthUnchecked();
}

uint8_t CString::bufferIndex() const noexcept {
    if (isInvalid()) {
        return INVALID_STRING_IDX;
    }
    return _bufferIndexUnchecked();
}

char* CString::raw() const noexcept {
    if (!isAllocated()) {
        return nullptr;
    }
    return _rawUnchecked();
}

int CString::rawCapacity() const noexcept {
    if (!isAllocated()) {
        return 0;
    }
    return _rawCapacityUnchecked();
}

int CString::rawMaxLength() const noexcept {
    return std::max(0, rawCapacity() - 1);
}

bool CString::isAllocated() const noexcept {
    return bufferIndex() != INVALID_STRING_IDX;
}

bool CString::isEmpty() const noexcept {
    return isAllocated() && _rawUnchecked()[0] == '\0';
}

bool CString::isEmptyOrInvalid() const noexcept {
    return isInvalid() || (isAllocated() && _rawUnchecked()[0] == '\0');
}

bool CString::isEmptyOrUnallocated() const noexcept {
    return !isAllocated() || _rawUnchecked()[0] == '\0';
}

bool CString::isInvalid() const noexcept {
    return _buf == nullptr || _handle == INVALID_STRING_IDX ;
}

CString &CString::append(const char c) noexcept {
    return append(&c, 1);
}

CString& CString::append(const CString &other) noexcept {
    return append(other.raw(), other.rawCapacity());
}

CString &CString::append(const std::string_view &other) noexcept {
    return append(other.data(), other.length());
}

CString& CString::append(const char *string) noexcept {
    return append(string, INT_MAX);
}

CString& CString::append(const char *string, int limit) noexcept {
    if (!isAllocated()) {
        return INVALID;
    }

    int len = _lengthUnchecked();
    int initialMaxLen = _rawMaxLengthUnchecked();

    if (len < initialMaxLen) { // try to append inline
        int maxCopy = std::min(initialMaxLen - len, limit);

        const char* src = string;
        char *selfRaw = _rawUnchecked();
        char *dst = selfRaw + len;
        char *end = selfRaw + maxCopy;

        for (;dst < end; ++src, ++dst) {
            *dst = *src;
            if (*src == '\0') {
                // end of string
                return *this;
            }
        }

        *dst = '\0'; // ensure \0 at end
        int copied = src - string;
        if (*src == '\0' || copied == limit) {
            return *this;
        }

        string = src;
        limit -= copied;
    }

    if (_moveToTop().shrinkToFit().isInvalid() || _buf->appendToTopmost(string, limit).isInvalid()) {
        return INVALID;
    }

    return *this;
}

CString &CString::appendFormat(const char *format, ...) noexcept {
    va_list args;
    va_start(args, format);
    return appendFormatV(format, args);
}

CString &CString::appendFormatV(const char *format, va_list args) noexcept {
    if (!isAllocated()) {
        return INVALID;
    }

    int len = _lengthUnchecked();
    int initialMaxLen = _rawMaxLengthUnchecked();

    if (len < initialMaxLen) { // try to append inline
        int maxSizeIncludingNull = initialMaxLen - len + 1;
        char *dst = _rawUnchecked()+len;

        int requiredLengthExcludingNull = vsnprintf(dst, maxSizeIncludingNull, format, args);
        if (requiredLengthExcludingNull < maxSizeIncludingNull) {
            return *this;
        }

        *dst = '\0';

        if (requiredLengthExcludingNull + 1 - maxSizeIncludingNull > _buf->unallocatedBytes()) {
            return INVALID;
        }
    }

    if (_moveToTop().shrinkToFit().isInvalid() || _buf->appendToTopmostFormatV(format, args).isInvalid()) {
        return INVALID;
    }

    return *this;
}

CString &CString::appendMost(const char c) noexcept {
    if (!isAllocated() || _buf->unallocatedBytes() == 0) {
        return *this;
    }

    return append(c);
}

CString &CString::appendMost(const CString &other) noexcept {
    if (!isAllocated() || !other.isAllocated()) {
        return *this;
    }
    return append(other._rawUnchecked(), std::min(_buf->unallocatedBytes(), other.length()));
}

CString &CString::appendMost(const std::string_view &other) noexcept {
    if (!isAllocated()) {
        return *this;
    }
    return append(other.data(), std::min(_buf->unallocatedBytes(), (int)other.length()));
}

CString &CString::appendMost(const char *string) noexcept {
    if (!isAllocated()) {
        return *this;
    }
    return append(string, _buf->unallocatedBytes());
}

CString &CString::appendMost(const char *string, int limit) noexcept {
    if (!isAllocated()) {
        return *this;
    }
    return append(string, std::min(_buf->unallocatedBytes(), limit));
}

CString &CString::appendMostFormat(const char *format, ...) noexcept {
    va_list args;
    va_start(args, format);
    return appendMostFormatV(format, args);
}

CString &CString::appendMostFormatV(const char *format, va_list args) noexcept {
    if (!isAllocated()) {
        return INVALID;
    }

    int len = _lengthUnchecked();
    int maxLen = _rawMaxLengthUnchecked();

    if (len == maxLen) {
        return *this;
    }

    int maxAppendIncludingNull = maxLen - len + 1;
    char *buf = _rawUnchecked();

    int requiredLengthExcludingNull = vsnprintf(buf+len, maxAppendIncludingNull, format, args);
    if (requiredLengthExcludingNull < maxAppendIncludingNull) {
        buf[len+requiredLengthExcludingNull] = '\0';
    } else {
        _rawUnchecked()[maxLen] = '\0';
    }

    return *this;
}

CString &CString::clear() noexcept {
    if (!isAllocated()) {
        return INVALID;
    }

    _rawUnchecked()[0] = '\0';
    return *this;
}

CString CString::clone() const noexcept {
    if (!isAllocated()) {
        return INVALID;
    }
    return _buf->push(_rawUnchecked());
}

CString CString::clone(int startIndex) const noexcept {
    if (!isAllocated() || startIndex < 0 || startIndex >= _rawCapacityUnchecked()) {
        return INVALID;
    }
    return _buf->push(_rawUnchecked() + startIndex, _rawCapacityUnchecked() - startIndex);
}

CString CString::clone(int startIndex, int length) const noexcept {
    if (length + startIndex > rawCapacity()) {
        return INVALID;
    }
    return cloneWithLimit(startIndex, length);
}

CString CString::cloneWithLimit(int limit) const noexcept {
    return cloneWithLimit(0, limit);
}

CString CString::cloneWithLimit(int startIndex, int limit) const noexcept {
    if (!isAllocated() || startIndex < 0 || limit < 0 || startIndex >= _rawCapacityUnchecked()) {
        return INVALID;
    }
    return _buf->push(_rawUnchecked() + startIndex, limit);
}

int CString::compare(const char *other) const noexcept {
    if (raw() == other) {
        return true;
    }
    return compare(other, strlen(other));
}

int CString::compare(const char *other, int otherLengthExcludingNull) const noexcept {
    char* rawSelf = raw();
    if (rawSelf == other) {
        return 0;
    }

    if (rawSelf == nullptr) {
        return -1;
    }
    if (other == nullptr) {
        return 1;
    }

    // prior invocation of raw() includes checks
    int len = _lengthUnchecked();

    if (len < otherLengthExcludingNull) {
        return -1;
    } else if (len > otherLengthExcludingNull) {
        return 1;
    }

    return strncmp(rawSelf, other, std::min(_rawCapacityUnchecked(), otherLengthExcludingNull));
}

int CString::compare(const CString &other) const noexcept {
    if (this == &other) {
        return 0;
    }

    bool selfIsInvalid = this->isInvalid();
    bool otherIsInvalid = other.isInvalid();
    if (selfIsInvalid && otherIsInvalid) {
        return 0;
    }
    if (selfIsInvalid) {
        return -1;
    }
    if (otherIsInvalid) {
        return 1;
    }

    bool selfIsUnallocated = !this->isAllocated();
    bool otherIsUnallocated = !other.isAllocated();
    if (selfIsUnallocated && otherIsUnallocated) {
        return 0;
    }
    if (selfIsUnallocated) {
        return -1;
    }
    if (otherIsUnallocated) {
        return 1;
    }

    return compare(other._rawUnchecked(), other._rawCapacityUnchecked());
}

bool CString::endsWith(const char c) const noexcept {
    int len = length();
    if (len < 0 || _rawCapacityUnchecked() < 1 || len >= _rawCapacityUnchecked()) {
        return -1;
    }
    return _rawUnchecked()[len - 1] == c;
}

bool CString::endsWith(const char *str) const noexcept {
    if (str == nullptr) {
        return false;
    }

    return endsWith(str, strlen(str));
}

bool CString::endsWith(const char *str, int strLenExcludingNull) const noexcept {
    int len = length();
    if (len < strLenExcludingNull) {
        return -1;
    }
    return strncmp(_rawUnchecked() + len - strLenExcludingNull, str, strLenExcludingNull) == 0;
}

bool CString::endsWith(const CString &str) const noexcept {
    return endsWith(str.raw());
}

int CString::indexOf(const char c, int startIndex) const noexcept {
    char anyOf[2] {c, '\0'};
    return indexOfAny(anyOf, startIndex);
}

int CString::indexOf(const char *str, int startIndex) const noexcept {
    return indexOf(str, startIndex, strlen(str));
}

int CString::indexOf(const char *str, int startIndex, int strLenExcludingNull) const noexcept {
    int capacity = rawCapacity();
    if (startIndex < 0 || startIndex >= capacity || capacity - startIndex < strLenExcludingNull) {
        return -1;
    }

    std::string_view::size_type result = asStringView(
            startIndex, capacity - startIndex).find(str, 0, strLenExcludingNull);
    if (result == std::string_view::npos) {
        return -1;
    }
    return result + startIndex;
}

int CString::indexOf(const CString &str, int startIndex) const noexcept {
    int strLen = str.length();
    if (strLen < 0) {
        return -1;
    }

    return indexOf(str._rawUnchecked(), startIndex, strLen);
}

int CString::indexOfAny(const char *chars, int startIndex) const noexcept {
    return indexOfAny(chars, startIndex, strlen(chars));
}

int CString::indexOfAny(const char *chars, int startIndex, int charsLengthExcludingNull) const noexcept {
    if (startIndex < 0 || startIndex >= rawCapacity()) {
        return -1;
    }

    std::string_view::size_type result = asStringView(
            startIndex, _rawCapacityUnchecked() - startIndex).find_first_of(chars, 0, charsLengthExcludingNull);
    return result == std::string_view::npos ? -1 : (int)result + startIndex;
}

int CString::indexOfAny(std::function<bool(const char)> predicate, int startIndex) const noexcept {
    if (startIndex < 0 || startIndex >= rawCapacity()) {
        return -1;
    }

    char *self = _rawUnchecked();
    char *e = self + _rawCapacityUnchecked();
    for (char *c = self + startIndex; c < e; ++c) {
        if (predicate(*c)) {
            return c - self;
        }
    }

    return -1;
}

int CString::lastIndexOf(const char c) const noexcept {
    return lastIndexOf(c, length() - 1);
}

int CString::lastIndexOf(const char c, int startIndex) const noexcept {
    char anyOf[2] {c, '\0'};
    return lastIndexOfAny(anyOf, startIndex);
}

int CString::lastIndexOf(const char *str) const noexcept {
    return lastIndexOf(str, length() - 1);
}

int CString::lastIndexOf(const char *str, int startIndex) const noexcept {
   return lastIndexOf(str, startIndex, strlen(str));
}

int CString::lastIndexOf(const char *str, int startIndex, int strLengthExcludingNull) const noexcept {
    if (startIndex < 0 || startIndex >= rawCapacity()) {
        return -1;
    }

    std::string_view::size_type result = asStringView(
            0, startIndex + 1).rfind(str, startIndex, strLengthExcludingNull);
    if (result == std::string_view::npos) {
        return -1;
    }
    return (int)result;
}

int CString::lastIndexOf(const CString &str) const noexcept {
    int strLen = str.length();
    if (strLen < 0) {
        return -1;
    }

    return lastIndexOf(str._rawUnchecked(), length() - 1, strLen);
}

int CString::lastIndexOf(const CString &str, int startIndex) const noexcept {
    int strLen = str.length();
    if (strLen < 0) {
        return -1;
    }

    return lastIndexOf(str._rawUnchecked(), startIndex, strLen);
}

int CString::lastIndexOfAny(const char *chars) const noexcept {
    return lastIndexOfAny(chars, length() - 1);
}

int CString::lastIndexOfAny(const char *chars, int startIndex) const noexcept {
    return lastIndexOfAny(chars, startIndex, strlen(chars));
}

int CString::lastIndexOfAny(const char *chars, int startIndex, int charsLengthExcludingNull) const noexcept {
    if (startIndex < 0 || startIndex >= rawCapacity()) {
        return -1;
    }

    std::string_view::size_type result = asStringView(
            0, startIndex + 1).find_last_of(chars, startIndex, charsLengthExcludingNull);
    return result == std::string_view::npos ? -1 : (int)result;
}

int CString::lastIndexOfAny(std::function<bool(const char)> predicate) const noexcept {
    return lastIndexOfAny(predicate, length() - 1);
}

int CString::lastIndexOfAny(std::function<bool(const char)> predicate, int startIndex) const noexcept {
    if (startIndex < 0 || startIndex >= rawCapacity()) {
        return -1;
    }

    char *self = raw();
    for (char *c = self + startIndex; c >= self; --c) {
        if (predicate(*c)) {
            return c - self;
        }
    }

    return -1;
}

CString& CString::resize(int maxLength) noexcept {
    if (maxLength < 0 || !_moveToTop().isAllocated() || _buf->resizeTopmost(maxLength).isInvalid()) {
        return INVALID;
    }

    return *this;
}

CString& CString::shrinkToFit() noexcept {
    return resize(length());
}

CString &CString::substring(int startIndex) noexcept {
    return substring(startIndex, rawCapacity() - startIndex);
}

CString &CString::substring(int startIndex, int length) noexcept {
    if (!isAllocated() || startIndex < 0 || length < 0 || startIndex + length > _rawCapacityUnchecked()) {
        return INVALID;
    }

    char *self = _rawUnchecked();
    memmove(self, self+startIndex, length);
    self[length] = '\0';
    return *this;
}

bool CString::startsWith(const char c) const noexcept {
    if (!isAllocated() || _rawCapacityUnchecked() < 1) {
        return false;
    }
    return _rawUnchecked()[0] == c;
}

bool CString::startsWith(const char *str) const noexcept {
    if (str == nullptr) {
        return false;
    }

    int strLen = strlen(str);
    return startsWith(str, strLen);
}

bool CString::startsWith(const char *str, int strLengthExcludingNull) const noexcept {
    return strncmp(raw(), str, std::min(rawCapacity(), strLengthExcludingNull)) == 0;
}

bool CString::startsWith(const CString &str) const noexcept {
    return startsWith(str.raw());
}

CString& CString::toLower() noexcept {
    if (!isAllocated()) {
        return INVALID;
    }

    int count = 0;
    int maxCount = _rawCapacityUnchecked();

    for (char* cur = _rawUnchecked(); *cur != '\0' && count < maxCount; ++cur, ++count) {
        if (*cur >= 'A' && *cur <= 'Z') {
            *cur += 32;
        }
    }

    return *this;
}

CString& CString::toUpper() noexcept {
    if (!isAllocated()) {
        return INVALID;
    }

    int count = 0;
    int maxCount = _rawCapacityUnchecked();

    for (char* cur = _rawUnchecked(); *cur != '\0' && count < maxCount; ++cur, ++count) {
        if (*cur >= 'a' && *cur <= 'z') {
            *cur -= 32;
        }
    }

    return *this;
}

CString& CString::_moveToTop() noexcept {
    if (!isAllocated()) {
        return INVALID;
    }

    if (!_buf->moveToTop(*this)) {
        return INVALID;
    }

    return *this;
}

CString &CString::trim() noexcept {
    return trim(isspace);
}

CString &CString::trim(char c) noexcept {
    char charsToTrim[2] = {c, '\0'};
    return trim(charsToTrim, 1);
}

CString &CString::trim(const char *chars) noexcept {
    return trim(chars, strlen(chars));
}

CString &CString::trim(const char *chars, int charsLengthExcludingNull) noexcept {
    return trimEnd(chars, charsLengthExcludingNull).trimStart(chars, charsLengthExcludingNull);
}

CString &CString::trim(std::function<bool(char)> isCharToRemove) noexcept {
    return trimEnd(isCharToRemove).trimStart(isCharToRemove);
}

CString &CString::trimStart() noexcept {
    return trimStart(isspace);
}

CString &CString::trimStart(char c) noexcept {
    char charsToTrim[2] = {c, '\0'};
    return trimStart(charsToTrim, 1);
}

CString &CString::trimStart(const char *chars) noexcept {
    return trimStart(chars, strlen(chars));
}

CString &CString::trimStart(const char *chars, int charsLengthExcludingNull) noexcept {
    if (!isAllocated()) {
        return INVALID;
    }

    std::string_view::size_type startIndex = asStringView(0, _rawCapacityUnchecked())
            .find_first_not_of(chars, 0, charsLengthExcludingNull);
    if (startIndex == std::string_view::npos) {
        return clear();
    }
    return substring((int)startIndex);
}

CString &CString::trimStart(std::function<bool(char)> isCharToRemove) noexcept {
    if (!isAllocated()) {
        return INVALID;
    }

    char *s = _rawUnchecked();
    char *c = s;
    char *e = s + _rawCapacityUnchecked();

    while (c < e && isCharToRemove(*c)) {
        c++;
    }

    int startIndex = c - s;
    return substring(startIndex);
}

CString &CString::trimEnd() noexcept {
    return trimEnd(isspace);
}

CString &CString::trimEnd(char c) noexcept {
    char charsToTrim[2] = {c, '\0'};
    return trimEnd(charsToTrim, 1);
}

CString &CString::trimEnd(const char *chars) noexcept {
    return trimEnd(chars, strlen(chars));
}

CString &CString::trimEnd(const char *chars, int charsLengthExcludingNull) noexcept {
    int len = length();
    if (len < 0) {
        return INVALID;
    }

    std::string_view::size_type endIndex = asStringView(0, len)
            .find_last_not_of(chars, len - 1, charsLengthExcludingNull);
    if (endIndex == std::string_view::npos) {
        return clear();
    }

    _rawUnchecked()[endIndex + 1] = '\0';
    return *this;
}

CString &CString::trimEnd(std::function<bool(char)> isCharToRemove) noexcept {
    if (!isAllocated()) {
        return INVALID;
    }

    char *s = _rawUnchecked();
    for (char *c = s + _lengthUnchecked() - 1; c >= s && isCharToRemove(*c); --c) {
        *c = '\0';
    }
    return *this;
}

CString &CString::deallocate() noexcept {
    if (_buf == nullptr) {
        return *this;
    }

    if (!_buf->remove(*this)) {
        return INVALID;
    }

    return *this;
}

const std::string_view CString::asStringView() const noexcept {
    return asStringView(0);
}

const std::string_view CString::asStringView(int startIndex) const noexcept {
    int len = length();
    if (len < 0) {
        return std::string_view();
    }

    return asStringView(startIndex, len - startIndex);
}

const std::string_view CString::asStringView(int startIndex, int length) const noexcept {
    if (length < 0 || startIndex < 0 || startIndex >= rawCapacity()) {
        return std::string_view();
    }
    return std::string_view(_rawUnchecked() + startIndex, std::min(length, _rawCapacityUnchecked() - startIndex));
}

CString& CString::operator+=(const char c) noexcept {
    return append(c);
}

CString& CString::operator+=(const CString& str) noexcept {
    return append(str);
}

CString& CString::operator+=(const char *str) noexcept {
    return append(str);
}

CString &CString::operator=(const char c) noexcept {
    return clear().append(c);
}

CString &CString::operator=(const char *str) noexcept {
    return clear().append(str);
}

CString &CString::operator=(const std::string_view str) noexcept {
    return clear().append(str.data(), str.length());
}

bool CString::operator==(const char *str) const noexcept {
    return compare(str) == 0;
}

bool CString::operator==(const CString &str) const noexcept {
    return compare(str) == 0;
}

bool CString::operator==(const std::string_view &str) const noexcept {
    return compare(str.data(), str.length()) == 0;
}

bool CString::operator!=(const char *str) const noexcept {
    return compare(str) != 0;
}

bool CString::operator!=(const CString &str) const noexcept {
    return compare(str) != 0;
}

bool CString::operator!=(const std::string_view &str) const noexcept {
    return compare(str.data(), str.length()) != 0;
}

char& CString::operator[](int index) noexcept {
    if (!isAllocated() || index < 0 || index > _rawMaxLengthUnchecked()) {
        static char NULLBUF;
        NULLBUF = '\0';
        return NULLBUF;
    }
    return _rawUnchecked()[index];
}

CString::operator const std::string_view() const noexcept {
    return asStringView();
}

int CString::_lengthUnchecked() const noexcept {
    char* self = _rawUnchecked();
    char* end = (char*)memchr(self, '\0', _rawCapacityUnchecked());


    if (end == nullptr) {
        return -1;
    }
    return end - self;
}

int CString::_bufferIndexUnchecked() const noexcept {
    return _buf->getIndex(*this);
}

char *CString::_rawUnchecked() const noexcept {
    return _buf->getRawString(*this);
}

int CString::_rawCapacityUnchecked() const noexcept {
    return _buf->getRawStringCapacity(*this);
}

int CString::_rawMaxLengthUnchecked() const noexcept {
    return std::max(0, _rawCapacityUnchecked() - 1);
}
