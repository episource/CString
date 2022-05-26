#include <unity.h>
#include "CString.h"

void testStartsWithCharacter() {
    CStringBuffer<10, 1> buffer;
    CString s1 = buffer.push("123456789");

    TEST_ASSERT_EQUAL_INT(true, s1.startsWith('1'));
    TEST_ASSERT_EQUAL_INT(false, s1.startsWith('9'));
}

void testStartsWithString() {
    CStringBuffer<10, 1> buffer;
    CString s1 = buffer.push("123456789");

    TEST_ASSERT_EQUAL_INT(true, s1.startsWith("12"));
    TEST_ASSERT_EQUAL_INT(false, s1.startsWith("89"));
}

void testStartsWithStringGivenLength() {
    CStringBuffer<10, 1> buffer;
    CString s1 = buffer.push("123456789");

    TEST_ASSERT_EQUAL_INT(true, s1.startsWith("12x", 2));
    TEST_ASSERT_EQUAL_INT(true, s1.startsWith("789", 0));
}

void testStartsWithCStringSameBuffer() {
    CStringBuffer<20, 3> buffer;
    CString s1 = buffer.push("123456789");
    CString s2 = buffer.push("12");
    CString s3 = buffer.push("89");

    TEST_ASSERT_EQUAL_INT(true, s1.startsWith(s2));
    TEST_ASSERT_EQUAL_INT(false, s1.startsWith(s3));
}

void testStartsWithCStringOtherBuffer() {
    CStringBuffer<10, 1> buffer1;
    CString s1 = buffer1.push("123456789");

    CStringBuffer<10, 2> buffer2;
    CString s2 = buffer2.push("12");
    CString s3 = buffer2.push("89");

    TEST_ASSERT_EQUAL_INT(true, s1.startsWith(s2));
    TEST_ASSERT_EQUAL_INT(false, s1.startsWith(s3));
}

void testStartsWithInvalidCString() {
    CStringBuffer<10, 1> buffer;
    CString s1 = buffer.push("123456789");
    TEST_ASSERT_EQUAL_INT(false, s1.startsWith(CString::INVALID));
}

void testStartsWithUnallocatedCString() {
    CStringBuffer<10, 2> buffer;
    CString s1 = buffer.push("123456789");
    CString s2 = buffer.push("12");
    s2.deallocate();

    TEST_ASSERT_EQUAL_INT(false, s1.startsWith(s2));
}

void runTestStartsWith() {
    const char* prevFile = Unity.TestFile;
    Unity.TestFile = __FILE__;

    RUN_TEST(testStartsWithCharacter);
    RUN_TEST(testStartsWithString);
    RUN_TEST(testStartsWithStringGivenLength);
    RUN_TEST(testStartsWithCStringSameBuffer);
    RUN_TEST(testStartsWithCStringOtherBuffer);
    RUN_TEST(testStartsWithInvalidCString);
    RUN_TEST(testStartsWithUnallocatedCString);

    Unity.TestFile = prevFile;
}