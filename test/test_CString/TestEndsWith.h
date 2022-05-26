#include "CString.h"
#include <unity.h>

void testEndsWithCharacter() {
    CStringBuffer<10, 1> buffer;
    CString s1 = buffer.push("123456789");

    TEST_ASSERT_EQUAL_INT(false, s1.endsWith('1'));
    TEST_ASSERT_EQUAL_INT(true, s1.endsWith('9'));
}

void testEndsWithString() {
    CStringBuffer<10, 1> buffer;
    CString s1 = buffer.push("123456789");

    TEST_ASSERT_EQUAL_INT(false, s1.endsWith("12"));
    TEST_ASSERT_EQUAL_INT(true, s1.endsWith("89"));
}

void testEndsWithStringGivenLength() {
    CStringBuffer<10, 1> buffer;
    CString s1 = buffer.push("123456789");

    TEST_ASSERT_EQUAL_INT(true, s1.endsWith("89x", 2));
    TEST_ASSERT_EQUAL_INT(true, s1.endsWith("12x", 0));
}

void testEndsWithCStringSameBuffer() {
    CStringBuffer<20, 3> buffer;
    CString s1 = buffer.push("123456789");
    CString s2 = buffer.push("12");
    CString s3 = buffer.push("89");

    TEST_ASSERT_EQUAL_INT(false, s1.endsWith(s2));
    TEST_ASSERT_EQUAL_INT(true, s1.endsWith(s3));
}

void testEndsWithCStringOtherBuffer() {
    CStringBuffer<10, 1> buffer1;
    CString s1 = buffer1.push("123456789");

    CStringBuffer<10, 2> buffer2;
    CString s2 = buffer2.push("12");
    CString s3 = buffer2.push("89");

    TEST_ASSERT_EQUAL_INT(false, s1.endsWith(s2));
    TEST_ASSERT_EQUAL_INT(true, s1.endsWith(s3));
}

void testEndsWithInvalidCString() {
    CStringBuffer<10, 1> buffer;
    CString s1 = buffer.push("123456789");
    TEST_ASSERT_EQUAL_INT(false, s1.endsWith(CString::INVALID));
}

void testEndsWithUnallocatedCString() {
    CStringBuffer<10, 2> buffer;
    CString s1 = buffer.push("123456789");
    CString s2 = buffer.push("89");
    s2.deallocate();

    TEST_ASSERT_EQUAL_INT(false, s1.endsWith(s2));
}

void runTestEndsWith() {
    const char* prevFile = Unity.TestFile;
    Unity.TestFile = __FILE__;

    RUN_TEST(testEndsWithCharacter);
    RUN_TEST(testEndsWithString);
    RUN_TEST(testEndsWithStringGivenLength);
    RUN_TEST(testEndsWithCStringSameBuffer);
    RUN_TEST(testEndsWithCStringOtherBuffer);
    RUN_TEST(testEndsWithInvalidCString);
    RUN_TEST(testEndsWithUnallocatedCString);

    Unity.TestFile = prevFile;
}
