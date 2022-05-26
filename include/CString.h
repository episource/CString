#pragma  once

#include <algorithm>
#include <functional>
#include <string_view>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define INVALID_STRING_IDX UINT8_MAX

typedef uint8_t CStringHandle;

class CString;

/// Stack based buffer for string content.
class CStringBufferBase {
public:
    /// @brief Allocates an empty CString with maxLength=0 / capacity=1 at the end of the buffer.
    /// @returns A CString. Will be invalid if remaining capacity is too low.
    virtual CString allocate() noexcept = 0;

    /// @brief Allocates an empty CString with given maxLength at the end of the buffer.
    /// @returns A CString. Will be invalid if remaining capacity is too low.
    virtual CString allocate(int maxLength) noexcept = 0;

    /// @brief Allocates an empty CString with a capacity equal to the remaining bytes.
    /// @returns A CString. Will be invalid if remaining capacity is too low.
    virtual CString allocateRemaining() noexcept = 0;

    /// @brief Allocates an empty CString with maxLength=0 / capacity=1 at the end of the buffer.
    /// @returns A CString. Will be invalid if remaining capacity is too low.
    virtual CString push() noexcept = 0;

    /// @brief Allocates an empty CString with given maxLength at the end of the buffer.
    /// @returns A CString. Will be invalid if remaining capacity is too low.
    virtual CString push(int maxLength) noexcept = 0;

    /// @brief Allocates a CString at the end of the buffer and initializes it using the given character. The resulting
    /// string length will be zero if the given character is \0, otherwise it will be 1.
    /// @returns A CString. Will be invalid if remaining capacity is too low.
    virtual CString push(const char c) noexcept = 0;

    /// @brief Allocates a CString at the end of the buffer and initializes it with the content of the given string.
    /// The allocated size is the size of the given string including terminating \0.
    /// @returns A CString. Will be invalid if remaining capacity is too low.
    virtual CString push(const char *string) noexcept = 0;

    /// @brief Allocates a CString at the end of the buffer and initializes it with the content of the given string.
    /// The allocated size is the size of the given string including terminating \0. At most limit characters will be
    /// copied. A \0 will be appended if it was not found within the first limit characters of the given string.
    /// @returns A CString. Will be invalid if remaining capacity is too low.
    virtual CString push(const char *string, int limit) noexcept = 0;

    /// @brief Allocates a CString at the end of the buffer and initializes it with the result of the format operation.
    /// @returns A CString. Will be invalid if remaining capacity is too low.
    virtual CString pushFormat(const char *format, ...) noexcept = 0;

    /// @brief Allocates a CString at the end of the buffer and initializes it with the result of the format operation.
    /// @returns A CString. Will be invalid if remaining capacity is too low.
    virtual CString pushFormatV(const char *format, va_list args) noexcept = 0;

    /// @brief Retrieves the CString that has been pushed/allocated last (top of buffer stack).
    /// @returns A CString. Will be unallocated if buffer is empty.
    virtual CString peek() noexcept = 0;

    /// @brief Unallocates/removes the CString that has been pushed/allocated last (top of buffer stack).
    /// @return true iff the buffer was non-empty, false otherwise.
    virtual bool pop() noexcept = 0;

    /// @brief Appends a single character to the CString on top of the buffer stack.
    /// @details Allocates more buffer area to the CString and appends the given character at the end of the original
    /// CString area. It assumes the last char of the CString area to be the first \0 character. This will be
    /// overwritten by the given character with a new \0 being appended after it. Therefore, a visible change to the
    /// contained string will only oocur if the string's length is aligned with the buffer area's capacity, that is the
    /// first \0 character of the buffer area is the last character of the buffer area prior to this operation.
    /// Otherwise, the CString#rawCapacity() ist still increased with the character being appended at the
    /// end, but no change to the contained string will be observable. Use resizeTopmost() first, to shrink the buffer area
    /// to be aligned with the contained string.
    /// @returns The modified CString or an invalid CString if capacity is too low.
    virtual CString appendToTopmost(const char c) noexcept = 0;

    /// @brief Appends a string to the CString on top of the buffer stack.
    /// @details Allocates more buffer area - as much as needed - to the CString and appends the given string at the end
    /// of the original CString buffer area. It assumes the last char of the CString area to be the first \0 character.
    /// The string is copied to the extended buffer area starting at the last character of the initial buffer area
    /// (a \0 character). As the string to be appended must end with \0, it is guaranteed that the extended buffer area
    /// after this operation will end with a \0 as well.
    /// A visible change to the contained string will only occur if the string's length is aligned with the buffer
    /// area's capacity, that is the first \0 character of the buffer area is the last character of the buffer area
    /// prior to this operation. Otherwise, the CString#rawCapacity() ist still increased by the length of the given
    /// string with the string being appended as described, but this happens after the end of the contained string.
    /// Use resizeTopmost() first, to shrink the buffer area to be aligned with the contained string.
    /// @returns The modified CString or an invalid CString if capacity is too low.
    virtual CString appendToTopmost(const char *string) noexcept = 0;

    /// @brief Appends a string to the CString on top of the buffer stack. At most limit characters are copied. A \0
    /// is appended if it was not found within the first limit characters of the given string.
    /// @details See #appendToTopmost(char*);
    /// @returns The modified CString or an invalid CString if capacity is too low.
    virtual CString appendToTopmost(const char *string, int limit) noexcept = 0;

    /// @brief Appends the result of the format operation to the CString on top of the buffer stack.
    /// @returns The modified CString or an invalid CString if capacity is too low.
    virtual CString appendToTopmostFormat(const char* format, ...) noexcept = 0;

    /// @brief Appends the result of the format operation to the CString on top of the buffer stack.
    /// @returns The modified CString or an invalid CString if capacity is too low.
    virtual CString appendToTopmostFormatV(const char* format, va_list args) noexcept = 0;

    /// @brief Changes the allocation of the top most CString, such that it's buffer areas capacity is equal to the
    /// given maxLength + 1. This methods ensures the last character after resize will be \0.
    /// @returns The modified CString or an invalid CString if this buffer was empty or if capacity
    /// is too low.
    virtual CString resizeTopmost(int maxLength) noexcept = 0;

    /// @brief Moves the given cstring to the top of the buffer stack.
    /// @returns `true` if move was successful, `false` otherwise.
    virtual bool moveToTop(const CString &cstring) noexcept = 0;

    /// @brief Unallocates/removes the CString associated with the given index.
    /// @details This results in moving/copying of buffer content if `index != numStrings() - 1`.
    /// @return `true` if the given CString was found, `false` otherwise.
    virtual bool remove(CString &cstring) noexcept = 0;

    /// @brief Unallocates/removes the CString associated with the given index.
    /// @details This results in moving/copying of buffer content if `index != numStrings() - 1`.
    /// @return `true` if the given CString was found, `false` otherwise.
    virtual bool remove(uint8_t index) noexcept = 0;

    /// @brief Unallocates/removes all CStrings currently allocated.
    /// @return `true` if any CString was removed, `false` otherwise.
    virtual bool removeAll() noexcept = 0;

    /// @brief Retrieves the current index of the string identified by the given cstring.
    /// @returns The index or -1 if cstring is invalid or not allocated.
    virtual uint8_t getIndex(const CString &cstring) const noexcept = 0;

    /// @brief Retrieves the CString associated with the given index.
    /// @return A CString. Will be unallocated if index is invalid.
    virtual CString getCString(uint8_t index) noexcept = 0;

    /// @brief Retrieves the raw string (char*) associated with the given index.
    /// @return A char*. Remains valid at least until next interaction with this buffer.
    virtual char* getRawString(uint8_t index) const noexcept = 0;

    /// @brief Retrieves the raw string (char*) associated with the given cstring.
    /// @return A char*. Remains valid at least until next interaction with this buffer.
    virtual char* getRawString(const CString &cstring) const noexcept = 0;

    /// @brief Retrieves the capacity of the buffer area associated with the given index.
    /// @return Buffer area capacity (equal string capacity).
    virtual int getRawStringCapacity(uint8_t index) const noexcept = 0;

    /// @brief Retrieves the capacity of the buffer area associated with the given cstring.
    /// @return Buffer area capacity (equal string capacity).
    virtual int getRawStringCapacity(const CString &cstring) const noexcept = 0;

    /// @brief Determines the number of CStrings allocated using this buffer.
    /// @returns Number of strings.
    virtual uint8_t numstrings() const noexcept = 0;

    /// @brief Determines the number of CStrings that can still be allocated using this buffer.
    /// @returns Number of strings.
    virtual uint8_t remainingStrings() const noexcept = 0;

    /// @brief Retrieves the number of bytes assigned to this buffer.
    virtual int capacity() const noexcept = 0;

    /// @brief Retrieves the number of bytes that have been allocated to CStrings.
    virtual int allocatedBytes() const noexcept = 0;

    /// @brief Retrieves the number of unallocated bytes.
    virtual int unallocatedBytes() const noexcept = 0;
};

/// Mutable string with variable length buffer area, allocated on the CStringBuffer.
/// The only operation that allocates and returns a new string based on a newly allocated buffer area is `clone`.
/// All other operations return the (modified) instance.
class CString final {
    template<int _capacity, int _maxstrings> friend
    class CStringBuffer;

public:
    /// @brief an invalid CString
    static CString INVALID;

    constexpr CString() noexcept : CString(nullptr, INVALID_STRING_IDX) {};

    /// @brief Determines length of contained string by searching the terminating \0
    /// @returns The number of characters preceding the terminating \0, or -1 if the string is invalid, unallocated or
    /// no terminating \0 was found.
    int length() const noexcept;

    /// @brief Retrieves current index within underlying stack-based buffer
    uint8_t bufferIndex() const noexcept;

    /// @brief Pointer to the underlying buffer area. The returned pointer remains valid until next interaction
    /// with the underlying buffer or any CString allocated using this buffer.
    char *raw() const noexcept;

    /// @brief Capacity of the underlying buffer area, including terminating \0
    int rawCapacity() const noexcept;

    /// @brief Max string length that fits into the underlying buffer area, that is `rawCapacity() - 1`
    int rawMaxLength() const noexcept;

    /// @brief Determines whether a buffer area was allocated to this string.
    bool isAllocated() const noexcept;

    /// @brief Determines whether the current string is allocated, but empty.
    bool isEmpty() const noexcept;

    /// @brief Determines whether the current string is empty or invalid;
    bool isEmptyOrInvalid() const noexcept;

    /// @brief Determines whehter the curent string is empty or unallocated (including invalid).
    bool isEmptyOrUnallocated() const noexcept;

    /// @brief Retrieves whether this CString is invalid. That is it is the result of an CString operation, which
    /// failed.
    bool isInvalid() const noexcept;

    /// @brief Appends the given character to the current CString.
    /// @returns The current CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise. In the latter case, the current CString content remains unchanged.
    CString& append(const char c) noexcept;

    /// @brief Append another string to the current CString.
    /// @returns The current CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise. In the latter case, the current CString content remains unchanged.
    CString& append(const CString &other) noexcept;

    /// @brief Append another string to the current CString.
    /// @returns The current CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise. In the latter case, the current CString content remains unchanged.
    CString& append(const std::string_view &other) noexcept;

    /// @brief Append another string to the current CString.
    /// @returns The current CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise. In the latter case, the current CString content remains unchanged.
    CString& append(const char *string) noexcept;

    /// @brief Append another string to the current CString.
    /// @returns The current CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise. In the latter case, the current CString content remains unchanged.
    CString& append(const char *string, int limit) noexcept;

    /// @brief Appends the result of the sprintf result.
    /// @returns The current CString if the operation was successful (enough buffer available) or an invalid CString
    ///  otherwise. In the latter case, the current CString content remains unchanged.
    CString& appendFormat(const char *format, ...) noexcept;

    /// @brief Appends the result of the sprintf result.
    /// @returns The current CString if the operation was successful (enough buffer available) or an invalid CString
    ///  otherwise. In the latter case, the current CString content remains unchanged.
    CString& appendFormatV(const char *format, va_list args) noexcept;

    /// @brief Appends the given character to the current CString. If not enough buffer is available, appends as much as
    /// possible.
    /// @returns The current CString (modified) with as much data appended as fits.
    CString& appendMost(const char c) noexcept;

    /// @brief Append another string to the current CString. If not enough buffer is available, appends as much as
    /// possible.
    /// @returns The current CString (modified) with as much data appended as fits.
    CString& appendMost(const CString &other) noexcept;

    /// @brief Append another string to the current CString. If not enough buffer is available, appends as much as
    /// possible.
    /// @returns The current CString (modified) with as much data appended as fits.
    CString& appendMost(const std::string_view &other) noexcept;

    /// @brief Append another string to the current CString. If not enough buffer is available, appends as much as
    /// possible.
    /// @returns The current CString (modified) with as much data appended as fits.
    CString& appendMost(const char *string) noexcept;

    /// @brief Append another string to the current CString. If not enough buffer is available, appends as much as
    /// possible.
    /// @returns The current CString (modified) with as much data appended as fits.
    CString& appendMost(const char *string, int limit) noexcept;

    /// @brief Appends the result of the sprintf result. If not enough buffer is available, appends as much as possible.
    /// @returns The current CString (modified) with as much data appended as fits.
    CString& appendMostFormat(const char *format, ...) noexcept;

    /// @brief Appends the result of the sprintf result. If not enough buffer is available, appends as much as possible.
    /// @returns The current CString (modified) with as much data appended as fits.
    CString& appendMostFormatV(const char *format, va_list args) noexcept;

    /// @briefs Clears all content without changing the capacity.
    /// @returns The current CString.
    CString& clear() noexcept;

    /// @brief Clones the current CString, that is a independent copy is allocated using the current buffer.
    /// @returns The cloned CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise.
    CString clone() const noexcept;

    /// @brief Clones the current CString, that is a independent copy is allocated using the current buffer.
    /// @returns The cloned CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise.
    CString clone(int startIndex) const noexcept;

    /// @brief Clones the current CString, that is a independent copy is allocated using the current buffer.
    /// @returns The cloned CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise.
    CString clone(int startIndex, int length) const noexcept;

    /// @brief Clones the current CString, but at most `limit` characters.
    /// @returns The cloned CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise.
    CString cloneWithLimit(int limit) const noexcept;

    /// @brief Clones the current CString, but at most `limit` characters.
    /// @returns The cloned CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise.
    CString cloneWithLimit(int startIndex, int limit) const noexcept;

    /// @brief Compares this string with the other string using `strcmp` semantic.
    int compare(const char* other) const noexcept;

    /// @brief Compares this string with the other string using `strcmp` semantic.
    int compare(const char* other, int otherLengthExcludingNull) const noexcept;

    /// @brief Compares this string with the other string using `strcmp` semantic.
    int compare(const CString &other) const noexcept;

    /// @brief Determines whether the string starts with the given character.
    bool endsWith(const char c) const noexcept;

    /// @brief Determines whether the string starts with the given string.
    bool endsWith(const char* str) const noexcept;

    /// @brief Determines whether the string starts with the given string.
    bool endsWith(const char* str, int strLenExcludingNull) const noexcept;

    /// @brief Determines whether the string starts with the given string.
    bool endsWith(const CString& str) const noexcept;

    /// @brief Reports the index of the first occurrence of the given character or -1 if not found. Search starts at the
    /// given startIndex. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int indexOf(const char c, int startIndex = 0) const noexcept;

    /// @brief Reports the index of the first occurrence of the given string or -1 if not found. Search starts at the
    /// given startIndex. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int indexOf(const char* str, int startIndex = 0) const noexcept;

    /// @brief Reports the index of the first occurrence of the given string or -1 if not found. Search starts at the
    /// given startIndex. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int indexOf(const char* str, int startIndex, int strLenExcludingNull) const noexcept;

    /// @brief Reports the index of the first occurrence of the given string or -1 if not found. Search starts at the
    /// given startIndex. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int indexOf(const CString& str, int startIndex = 0) const noexcept;

    /// @brief Reports the index of the first occurrence of any of the given characters or -1 if not found. Search
    /// starts at the given startIndex. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int indexOfAny(const char* chars, int startIndex = 0) const noexcept;

    /// @brief Reports the index of the first occurrence of any of the given characters or -1 if not found. Search
    /// starts at the given startIndex. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int indexOfAny(const char* chars, int startIndex, int charsLengthExcludingNull) const noexcept;

    /// @brief Reports the index of the first occurrence of any character that matches the given predicate. Search
    /// starts at the given startIndex. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int indexOfAny(std::function<bool(const char)> predicate, int startIndex = 0) const noexcept;

    /// @brief Reports the index of the last occurrence of the given character or -1 if not found. Search starts at the
    /// end of the contained string. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int lastIndexOf(const char c) const noexcept;

    /// @brief Reports the index of the last occurrence of the given character or -1 if not found. Search starts at the
    /// given startIndex. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int lastIndexOf(const char c, int startIndex) const noexcept;

    /// @brief Reports the index of the last occurrence of the given string or -1 if not found. Search starts at the
    /// end of the contained string. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int lastIndexOf(const char* str) const noexcept;

    /// @brief Reports the index of the last occurrence of the given string or -1 if not found. Search starts at the
    /// given startIndex.  The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int lastIndexOf(const char* str, int startIndex) const noexcept;

    /// @brief Reports the index of the last occurrence of the given string or -1 if not found. Search starts at the
    /// given startIndex. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int lastIndexOf(const char* str, int startIndex, int strLengthExcludingNull) const noexcept;

    /// @brief Reports the index of the last occurrence of the given string or -1 if not found. Search starts at the
    /// end of the string. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int lastIndexOf(const CString& str) const noexcept;

    /// @brief Reports the index of the last occurrence of the given string or -1 if not found. Search starts at the
    /// given startIndex. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int lastIndexOf(const CString& str, int startIndex) const noexcept;

    /// @brief Reports the index of the last occurrence of any of the given characters or -1 if not found. Search
    /// starts at the end of the string. The whole buffer area is searched, therefore results after the end of the contained string
    /// might be returned, if the allocated capacity is bigger than the contained string.
    int lastIndexOfAny(const char* chars) const noexcept;

    /// @brief Reports the index of the last occurrence of any of the given characters or -1 if not found. Search
    /// starts at the given startIndex. The whole buffer area is searched, therefore results after the end of the
    /// contained string might be returned, if the allocated capacity is bigger than the contained string.
    int lastIndexOfAny(const char* chars, int startIndex) const noexcept;

    /// @brief Reports the index of the last occurrence of any of the given characters or -1 if not found. Search
    /// starts at the given startIndex. The whole buffer area is searched, therefore results after the end of the
    /// contained string might be returned, if the allocated capacity is bigger than the contained string.
    int lastIndexOfAny(const char* chars, int startIndex, int charsLengthExcludingNull) const noexcept;

    /// @brief Reports the index of the last occurrence of any character that matches the given predicate.  The whole
    /// buffer area is searched, therefore results after the end of the contained string might be returned, if the
    /// allocated capacity is bigger than the contained string.
    int lastIndexOfAny(std::function<bool(const char)> predicate) const noexcept;

    /// @brief Reports the index of the last occurrence of any character that matches the given predicate. Search
    /// starts at the given startIndex.  The whole buffer area is searched, therefore results after the end of the
    /// contained string might be returned, if the allocated capacity is bigger than the contained string.
    int lastIndexOfAny(std::function<bool(const char)> predicate, int startIndex) const noexcept;

    /// @brief Resizes the current CString buffer area by setting the new capacity to `maxLength + 1`.
    /// @returns The current CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise. In the latter case, the current CString content remains unchanged.
    CString& resize(int maxLength) noexcept;

    /// @brief Resizes the current CString using the current `length()` as new maxLength.
    /// @returns The current CString (modified).
    CString& shrinkToFit() noexcept;

    /// @brief Truncates the string such that only the given substring is retained. Use `clone` to retrieve a copy of
    /// the identified substring.
    /// @returns Reference to the current string (modified).
    CString& substring(int startIndex) noexcept;

    /// @brief Truncates the string such that only the given substring is retained. Use `clone` to retrieve a copy of
    /// the identified substring.
    /// @returns Reference to the current string (modified).
    CString& substring(int startIndex, int length) noexcept;

    /// @brief Determines whether the string starts with the given character.
    bool startsWith(const char c) const noexcept;

    /// @brief Determines whether the string starts with the given string.
    bool startsWith(const char* str) const noexcept;

    /// @brief Determines whether the string starts with the given string.
    bool startsWith(const char* str, int strLengthExcludingNull) const noexcept;

    /// @brief Determines whether the string starts with the given string.
    bool startsWith(const CString& str) const noexcept;

    /// @brief Changes all characters of the current string to lower case. Only ASCII characters are changed.
    /// @returns The current CString (modified).
    CString& toLower() noexcept;

    /// @brief Changes all characters of the current string to upper case. Only ASCII characters are changed.
    /// @returns The current CString (modified).
    CString& toUpper() noexcept;

    /// @brief Trims the current CString, that is removes all leading and trailing whitespace.
    /// @returns The current CString (modified).
    CString& trim() noexcept;

    /// @brief Trims the current CString by removing leading and trailing occurrences of the given character.
    /// @returns The current CString (modified).
    CString& trim(char c) noexcept;

    /// @brief Trims the current CString, that is removes all leading and trailing characters that are contained in the
    /// given string (excluding \0).
    /// @returns The current CString (modified).
    CString& trim(const char *chars) noexcept;

    /// @brief Trims the current CString, that is removes all leading and trailing characters that are contained in the
    /// given string (excluding \0).
    /// @returns The current CString (modified).
    CString& trim(const char *chars, int charsLengthExcludingNull) noexcept;

    /// @brief Trims the current CString, that is removes all leading and trailing characters that are matched by the
    /// provided predicate.
    /// @returns The current CString (modified).
    CString& trim(std::function<bool(const char)> isCharToRemove) noexcept;

    /// @brief Trims the beginning of the current CString, that is removes all leading whitespace.
    /// @returns The current CString (modified).
    CString& trimStart() noexcept;

    /// @brief Trims the beginning of the current CString by removing leading occurrences of the given character.
    /// @returns The current CString (modified).
    CString& trimStart(char c) noexcept;

    /// @brief Trims the beginning of the current CString, that is removes all leading characters that are contained
    /// in the given string (excluding \0).
    /// @returns The current CString (modified).
    CString& trimStart(const char *chars) noexcept;

    /// @brief Trims the beginning of the current CString, that is removes all leading characters that are contained
    /// in the given string (excluding \0).
    /// @returns The current CString (modified).
    CString& trimStart(const char *chars, int charsLengthExcludingNull) noexcept;

    /// @brief Trims the beginning of the current CString, that is removes all leading characters that are matched by
    /// the provided predicate.
    /// @returns The current CString (modified).
    CString& trimStart(std::function<bool(const char)> isCharToRemove) noexcept;

    /// @brief Trims the end of the current CString, that is removes all trailing whitespace.
    /// @returns The current CString (modified).
    CString& trimEnd() noexcept;

    /// @brief Trims the end of the current CString by removing trailing occurrences of the given character.
    /// @returns The current CString (modified).
    CString& trimEnd(char c) noexcept;

    /// @brief Trims the end of the current CString, that is removes all trailing characters that are contained
    /// in the given string (excluding \0).
    /// @returns The current CString (modified).
    CString& trimEnd(const char *chars) noexcept;

    /// @brief Trims the end of the current CString, that is removes all trailing characters that are contained
    /// in the given string (excluding \0).
    /// @returns The current CString (modified).
    CString& trimEnd(const char *chars, int charsLengthExcludingNull) noexcept;

    /// @brief Trims the end of the current CString, that is removes all trailing characters that are matched by the
    /// provided predicate.
    /// @returns The current CString (modified).
    CString& trimEnd(std::function<bool(const char)> isCharToRemove) noexcept;

    /// @brief Deallocates the current string. Might involve moving of other CStrings allocated using the same buffer.
    /// @returns The current CString, now unallocated.
    CString& deallocate() noexcept;

    /// @brief Creates a string view based on the buffer area allocated to this string and using the length of the
    /// contained string.
    /// @returns A std::string_view that remains valid at least until the next interaction with any of the strings
    /// allocated on the current buffer.
    const std::string_view asStringView() const noexcept;

    /// @brief Creates a string view based on the buffer area allocated to this string, starting at the given index.
    /// @returns A std::string_view that remains valid at least until the next interaction with any of the strings
    /// allocated on the current buffer.
    const std::string_view asStringView(int startIndex) const noexcept;

    /// @brief Creates a string view based on the buffer area allocated to this string, starting at the given index
    /// and having the given length (or the remaining capacity if the remaining buffer area is shorter).
    /// @returns A std::string_view that remains valid at least until the next interaction with any of the strings
    /// allocated on the current buffer.
    const std::string_view asStringView(int startIndex, int length) const noexcept;

    /// @brief replaces the current string with the given character
    /// @returns The current CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise. In the latter case, the current CString content remains unchanged.
    CString& operator =(const char c) noexcept;

    /// @brief replaces the current string with the given string
    /// @returns The current CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise. In the latter case, the current CString content remains unchanged.
    CString& operator =(const char* str) noexcept;

    /// @brief replaces the current string with the given string
    /// @returns The current CString if the operation was successful (enough buffer available) or an invalid CString
    /// otherwise. In the latter case, the current CString content remains unchanged.
    CString& operator =(const std::string_view str) noexcept;

    /// @brief Same as `append`.
    CString& operator +=(const CString& str) noexcept;

    /// @brief Same as `append`.
    CString& operator +=(const char* str) noexcept;

    /// @brief Same as `append`.
    CString& operator +=(const char cCh) noexcept;

    /// @brief compares content
    bool operator ==(const char *str) const noexcept;

    /// @brief compares content
    bool operator ==(const CString &str) const noexcept;

    /// @brief compares content
    bool operator ==(const std::string_view &str) const noexcept;

    /// @brief compares content
    bool operator !=(const char *str) const noexcept;

    /// @brief compares content
    bool operator !=(const CString &str) const noexcept;

    /// @brief compares content
    bool operator !=(const std::string_view &str) const noexcept;

    /// @brief Retrieves the char at the given index.
    /// @returns A reference to the char at the given index or an reference to \0 if index is invalid.
    char& operator [](int index) noexcept;

    /// @brief see std::string_view
    operator const std::string_view() const noexcept;

private:
    CStringBufferBase * _buf;
    CStringHandle _handle;

    constexpr CString(CStringBufferBase *buf, CStringHandle handle) noexcept : _buf(buf), _handle(handle) {}

    inline int _lengthUnchecked() const noexcept;
    inline int _bufferIndexUnchecked() const noexcept;
    inline char *_rawUnchecked() const noexcept;
    inline int _rawCapacityUnchecked() const noexcept;
    inline int _rawMaxLengthUnchecked() const noexcept;

    CString& _moveToTop() noexcept;
};

template<int _capacity, int _maxstrings = 10>
class CStringBuffer final : CStringBufferBase {
    static_assert(_capacity > 0 && _maxstrings > 0 && _maxstrings < UINT8_MAX);
public:
    constexpr CStringBuffer() noexcept : CStringBuffer(std::make_index_sequence<_maxstrings>{}) {}

    virtual CString allocate() noexcept override {
        return push('\0');
    }

    virtual CString allocate(int maxLength) noexcept override {
        CString initialAllocation = allocate();
        if (!initialAllocation.isAllocated()) {
            return initialAllocation;
        }

        CString resizedAllocation = resizeTopmost(maxLength);
        if (!resizedAllocation.isAllocated()) {
            pop();
            return CString::INVALID;
        }

        return resizedAllocation;
    }

    virtual CString allocateRemaining() noexcept override {
        if (_remaining < 1) {
            return CString::INVALID;
        }
        return allocate(_remaining - 1);
    }

    virtual CString push() noexcept override {
        return allocate();
    }

    virtual CString push(int maxLength) noexcept override {
        return allocate(maxLength);
    }

    virtual CString push(const char c) noexcept override {
        return push(&c, 1);
    }

    virtual CString push(const char *string) noexcept override {
        return push(string, INT_MAX);
    }

    virtual CString push(const char *string, int limit) noexcept override {
        return _pushOrAppendToLast(string, limit, false);
    }

    virtual CString pushFormat(const char *format, ...) noexcept override {
        va_list args;
        va_start(args, format);
        return pushFormatV(format, args);
    }

    virtual CString pushFormatV(const char *format, va_list args) noexcept override {
        if (allocate().isInvalid()) {
            return CString::INVALID;
        }
        return appendToTopmostFormatV(format, args);
    }

    virtual CString peek() noexcept override {
        if (_numstrings == 0) {
            return CString::INVALID;
        }
        return getCString(_numstrings - 1);
    }

    bool pop() noexcept override {
        if (_numstrings == 0) {
            return false;
        }
        return this->remove(this->_numstrings - 1);
    }

    virtual CString appendToTopmost(const char c) noexcept override {
        return appendToTopmost(&c, 1);
    }

    virtual CString appendToTopmost(const char *string) noexcept override {
        return appendToTopmost(string, INT_MAX);
    }

    virtual CString appendToTopmost(const char *string, int limit) noexcept override {
        return _pushOrAppendToLast(string, limit, true);
    }

    virtual CString appendToTopmostFormat(const char* format, ...) noexcept override {
        va_list args;
        va_start(args, format);
        return appendToTopmostFormatV(format, args);
    }

    virtual CString appendToTopmostFormatV(const char* format, va_list args) noexcept override {
        char *dst = _strings[_numstrings] - 1;

        // note: formattedLength does not include terminating \0
        // argument n (2nd) includes terminating \0!
        int requiredLengthExcludingNull = vsnprintf(dst, _remaining + 1, format, args);
        if (requiredLengthExcludingNull > _remaining) {
            *dst = '\0';
            return CString::INVALID;
        }

        _strings[_numstrings] += requiredLengthExcludingNull;
        return peek();
    }

    virtual CString resizeTopmost(int maxLength) noexcept override {
        if (_numstrings == 0) {
            return CString::INVALID;
        }

        uint8_t index = _numstrings - 1;
        int capacityOfLast = getRawStringCapacity(index);
        int newCapacity = maxLength + 1;
        int newRemaining = _remaining + capacityOfLast - newCapacity;

        if (newRemaining < 0) {
            return CString::INVALID;
        }

        char *newLastChar = _strings[index] + maxLength ;
        *newLastChar = '\0';
        _strings[index + 1] = newLastChar + 1;
        _remaining = newRemaining;

        return CString(this, _curHandle);
    }

    virtual bool moveToTop(const CString &cstring) noexcept override {
        if (cstring._buf != this || !cstring.isAllocated()) {
            return false;
        }
        uint8_t oldIndex = getIndex(cstring);
        if (oldIndex == _numstrings - 1) {
            return true;
        }
        uint8_t newIndex = _numstrings - 1;

        int cstringCapacity = cstring.rawCapacity();
        int tailSize = _strings[newIndex + 1] - _strings[oldIndex + 1];

        if (_remaining >= cstringCapacity) {
            memcpy(_strings[newIndex + 1], _strings[oldIndex], cstringCapacity);
            memmove(_strings[oldIndex], _strings[oldIndex+1], tailSize + cstringCapacity);
        } else  {
            // use stack based temporary buffer if too few bytes remain
            constexpr int stackBasedChunkSize = 8;
            char stackTemp[stackBasedChunkSize];
            char *temp = _remaining >= stackBasedChunkSize ? _strings[newIndex + 1] : stackTemp;

            int chunkSize = std::min(cstringCapacity, std::max(_remaining, stackBasedChunkSize));
            for (int remaining = cstringCapacity; remaining > 0;) {
                char *from = _strings[oldIndex] + remaining - chunkSize;
                memcpy(temp, from, chunkSize);
                memmove(from, from + chunkSize, tailSize);
                memcpy(from + tailSize, temp, chunkSize);

                remaining -= chunkSize;
                chunkSize = std::min(chunkSize, remaining);
            }
        }

        for (int i = oldIndex + 1; i < _numstrings; ++i) {
            // update string pointers
            _strings[i] = _strings[i + 1] - cstringCapacity;

            // update associated CString handles
            _handleToStringIdxMap[_findHandleByStringIndex(i)]--;
        }
        // _strings[newIndex+1] unchanged: total length did not change

        // update moved cstring
        _handleToStringIdxMap[cstring._handle] = newIndex;

        return true;
    }

    virtual bool remove(CString &cString) noexcept override {
        return remove(getIndex(cString));
    }

    virtual bool remove(uint8_t index) noexcept override {
        if (index >= _numstrings) {
            return false;
        }

        // note: _strings size is (_maxstrings + 1)
        // => _strings[_numstrings] is valid index
        int capacityToRemove = _strings[index + 1] - _strings[index];
        _remaining += capacityToRemove;

        // if last string is about to be removed, only _numstrings needs to be decremented!
        if (index == _numstrings - 1) {
            _numstrings--;
            _handleToStringIdxMap[_curHandle] = INVALID_STRING_IDX;
            _curHandle = _numstrings == 0 ? INVALID_STRING_IDX : _findHandleByStringIndex(_numstrings - 1);
            return true;
        }

        // note: _strings size is (_maxstrings + 1)
        // => _strings[_numstrings] is valid index
        memmove(_strings[index], _strings[index + 1], _strings[_numstrings] - _strings[index + 1]);

        // invalidate cstring associated with index
        _handleToStringIdxMap[_findHandleByStringIndex(index)] = INVALID_STRING_IDX;

        for (int i = index + 1; i < _numstrings; ++i) {
            // update string pointers
            _strings[i] = _strings[i + 1] - capacityToRemove;

            // update associated CString handles
            _handleToStringIdxMap[_findHandleByStringIndex(i)]--;
        }
        _strings[_numstrings] = nullptr;

        // decrement after for-loop for correct loop condition!
        // reason: next _string pointer needs to be adjusted as well!
        _numstrings--;

        return true;
    }

    virtual bool removeAll() noexcept override {
        if (_numstrings == 0) {
            return false;
        }

        for (int i = 0; i < _maxstrings; ++i) {
            _handleToStringIdxMap[i] = INVALID_STRING_IDX;
            _strings[i+1] = nullptr;
        }

        _buffer[0] = '\0';
        _buffer[_capacity - 1] = '\0';
        _strings[0] = _buffer;
        _numstrings = 0;
        _remaining = _capacity;

        return true;
    }

    virtual uint8_t getIndex(const CString &cstring) const noexcept override {
        if (cstring._buf != this || cstring._handle >= _maxstrings || cstring._handle == INVALID_STRING_IDX) {
            return INVALID_STRING_IDX;
        }

        return _handleToStringIdxMap[cstring._handle];
    }

    virtual CString getCString(uint8_t index) noexcept override {
        if (index >= _numstrings) {
            return CString::INVALID;
        }
        return CString(this, _findHandleByStringIndex(index));
    }

    virtual char *getRawString(uint8_t index) const noexcept override {
        if (index >= _numstrings || index == INVALID_STRING_IDX) {
            return nullptr;
        }
        return _strings[index];
    }

    virtual char *getRawString(const CString &cstring) const noexcept override {
        return getRawString(getIndex(cstring));
    }

    virtual int getRawStringCapacity(uint8_t index) const noexcept override {
        if (index >= _numstrings || index == INVALID_STRING_IDX) {
            return -1;
        }

        // consider terminating \0: subtract 1
        return _strings[index + 1] - _strings[index];
    }

    virtual int getRawStringCapacity(const CString &cstring) const noexcept override {
        return getRawStringCapacity(getIndex(cstring));
    }

    virtual uint8_t numstrings() const noexcept override {
        return _numstrings;
    }

    virtual uint8_t remainingStrings() const noexcept override {
        return _maxstrings - _numstrings;
    }

    virtual int capacity() const noexcept override {
        return _capacity;
    }

    virtual int allocatedBytes() const noexcept override {
        return _capacity - _remaining;
    }

    virtual int unallocatedBytes() const noexcept override {
        return _remaining;
    }

private:
    char _buffer[_capacity]{};
    char *_strings[_maxstrings + 1]{};
    uint8_t _numstrings = 0;
    int _remaining = _capacity;

    CStringHandle _curHandle = INVALID_STRING_IDX;
    uint8_t _handleToStringIdxMap[_maxstrings];

    template<std::size_t... indexes>
    constexpr CStringBuffer(std::index_sequence<indexes...>) noexcept
            : _handleToStringIdxMap{(static_cast<void>(indexes), INVALID_STRING_IDX)...} {
        _buffer[0] = '\0';
        _buffer[_capacity - 1] = '\0';
        _strings[0] = _buffer;
    }

    CStringHandle _nextUnallocatedHandle() noexcept {
        if (_numstrings == _maxstrings) {
            return INVALID_STRING_IDX;
        }

        for (CStringHandle candidateHandle = _curHandle + 1; candidateHandle != _curHandle; ++candidateHandle) {
            if (candidateHandle >= _maxstrings) {
                candidateHandle = 0;
            }

            if (_handleToStringIdxMap[candidateHandle] == INVALID_STRING_IDX) {
                return candidateHandle;
            }
        }

        return INVALID_STRING_IDX;
    }

    CStringHandle _findHandleByStringIndex(uint8_t strIndex) noexcept {
        CStringHandle candidateHandle = _curHandle;
        do {
            if (_handleToStringIdxMap[candidateHandle] == strIndex) {
                return candidateHandle;
            }
        } while ((candidateHandle = candidateHandle == 0 ? _maxstrings - 1 : candidateHandle - 1) != _curHandle);

        return INVALID_STRING_IDX;
    }

    CString _pushOrAppendToLast(const char *string, int limit, bool append) noexcept {
        append &= _numstrings > 0; // append is push if there are no strings yet
        CStringHandle resultHandle = append ? _curHandle : _nextUnallocatedHandle();

        if (append && limit == 0) {
            return CString(this, resultHandle);
        }

        if ((!append && (_numstrings == _maxstrings || _remaining == 0))
            || resultHandle == INVALID_STRING_IDX) {
            return CString::INVALID;
        }

        uint8_t index = append ? _numstrings - 1 : _numstrings;
        int count = 0;
        int remaining = _remaining;
        char *dst = _strings[_numstrings];

        if (append) {
            dst--;
            remaining++;
        }

        bool endOfString = false;
        while (remaining > 1 && count < limit) {
            count++;
            remaining--;

            char cur = *string++;
            *dst++ = cur;

            if (cur == '\0') {
                endOfString = true;
                break;
            }
        }

        // make sure there's \0 at end
        if (!endOfString) {
            if (count < limit && *string == '\0') {
                count++;
                endOfString = true;
            }

            remaining--;
            *dst++ = '\0';
        }

        // commit changes only if there was enough buffer space
        if (endOfString || count == limit) {
            // _strings size is (_maxstrings + 1)!
            _strings[index + 1] = dst;
            _remaining = remaining;

            if (!append) {
                _numstrings++;
                _handleToStringIdxMap[resultHandle] = index;
                _curHandle = resultHandle;
            }

            return CString(this, resultHandle);
        }

        if (append) {
            // restore \0 that has been overwritten
            *(_strings[index] - 1) = '\0';
        }

        return CString::INVALID;
    }
};
