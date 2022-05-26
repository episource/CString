#include <unity.h>

#include "TestAppend.h"
#include "TestEndsWith.h"
#include "TestIndexOf.h"
#include "TestStartsWith.h"
#include "TestTrim.h"

void testLength() {
    CStringBuffer<10, 1> buffer;
    CString s1 = buffer.push("123456789");

    TEST_ASSERT_EQUAL_INT(9, s1.length());
    TEST_ASSERT_EQUAL_INT(9, s1.rawMaxLength());

    s1.raw()[5] = '\0';
    TEST_ASSERT_EQUAL_INT(5, s1.length());
    TEST_ASSERT_EQUAL_INT(9, s1.rawMaxLength());

    s1.raw()[0] = '\0';
    TEST_ASSERT_EQUAL_INT(0, s1.length());
    TEST_ASSERT_EQUAL_INT(9, s1.rawMaxLength());
}

void testIsAllocatedOrInvalid() {
    CStringBuffer<10, 2> buffer;
    CString s1 = buffer.push("12");
    CString s2 = buffer.push("34").deallocate();
    CString s3 = CString::INVALID;
    CString s4;

    TEST_ASSERT_EQUAL_INT(true, s1.isAllocated());
    TEST_ASSERT_EQUAL_INT(false, s2.isAllocated());
    TEST_ASSERT_EQUAL_INT(false, s3.isAllocated());
    TEST_ASSERT_EQUAL_INT(false, s4.isAllocated());

    TEST_ASSERT_EQUAL_INT(false, s1.isInvalid());
    TEST_ASSERT_EQUAL_INT(false, s2.isInvalid());
    TEST_ASSERT_EQUAL_INT(true, s3.isInvalid());
    TEST_ASSERT_EQUAL_INT(true, s4.isInvalid());
}

void testDeallocateFirst() {
    CStringBuffer<10, 3> buffer;
    CString s1 = buffer.push("123");
    CString s2 = buffer.push("45");
    CString s3 = buffer.push("6");

    char* s1RawBeforeDeallocate = s1.raw();

    TEST_ASSERT_EQUAL_INT(9, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(1, buffer.unallocatedBytes());

    TEST_ASSERT_EQUAL_INT(0, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s2.bufferIndex());
    TEST_ASSERT_EQUAL_INT(2, s3.bufferIndex());

    s1.deallocate();

    TEST_ASSERT_EQUAL_INT(INVALID_STRING_IDX, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(0, s2.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s3.bufferIndex());

    TEST_ASSERT_EQUAL_INT(nullptr, s1.raw());
    TEST_ASSERT_EQUAL_INT(s1RawBeforeDeallocate, s2.raw());
    TEST_ASSERT_EQUAL_INT(s1RawBeforeDeallocate + s2.rawCapacity(), s3.raw());

    TEST_ASSERT_EQUAL_INT(5, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(5, buffer.unallocatedBytes());
}

void testDeallocateLast() {
    CStringBuffer<10, 3> buffer;
    CString s1 = buffer.push("123");
    CString s2 = buffer.push("45");
    CString s3 = buffer.push("6");

    char* s1RawBeforeDeallocate = s1.raw();

    TEST_ASSERT_EQUAL_INT(9, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(1, buffer.unallocatedBytes());

    TEST_ASSERT_EQUAL_INT(0, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s2.bufferIndex());
    TEST_ASSERT_EQUAL_INT(2, s3.bufferIndex());

    s3.deallocate();

    TEST_ASSERT_EQUAL_INT(INVALID_STRING_IDX, s3.bufferIndex());
    TEST_ASSERT_EQUAL_INT(0, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s2.bufferIndex());

    TEST_ASSERT_EQUAL_INT(nullptr, s3.raw());
    TEST_ASSERT_EQUAL_INT(7, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(3, buffer.unallocatedBytes());
}

void testToLower() {
    CStringBuffer<300, 1> buffer;
    CString s1 = buffer.push("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZäöüßÄÖÜáàâÁÀÂ\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !\"#$%&'()*+´-./0123456789:;<=>?@[\\]^_`{|}~\x7f");
    CString s2 = s1.toLower();

    TEST_ASSERT_EQUAL_INT(true, s1 == s2);

    // only ASCII characters are changed
    TEST_ASSERT_EQUAL_STRING("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzäöüßÄÖÜáàâÁÀÂ\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !\"#$%&'()*+´-./0123456789:;<=>?@[\\]^_`{|}~\x7f", s2.raw());
}

void testToUpper() {
    CStringBuffer<300, 1> buffer;
    CString s1 = buffer.push("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZäöüßÄÖÜáàâÁÀÂ\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !\"#$%&'()*+´-./0123456789:;<=>?@[\\]^_`{|}~\x7f");
    CString s2 = s1.toUpper();

    TEST_ASSERT_EQUAL_INT(true, s1 == s2);

    // only ASCII characters are changed
    TEST_ASSERT_EQUAL_STRING("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZäöüßÄÖÜáàâÁÀÂ\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !\"#$%&'()*+´-./0123456789:;<=>?@[\\]^_`{|}~\x7f", s2.raw());
}

void setUp() {};
void tearDown() {};

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(testLength);
    RUN_TEST(testIsAllocatedOrInvalid);
    RUN_TEST(testToLower);
    RUN_TEST(testToUpper);
    RUN_TEST(testDeallocateFirst);
    RUN_TEST(testDeallocateLast);

    runTestAppend();
    runTestEndsWith();
    runTestIndexOf();
    runTestStartsWith();
    runTestTrim();

    return UNITY_END();
}