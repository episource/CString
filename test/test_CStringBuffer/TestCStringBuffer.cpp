#include "CString.h"
#include <unity.h>

void testAllocate() {
    CStringBuffer<10, 1> buffer;
    CString s = buffer.allocate();
    TEST_ASSERT_EQUAL_INT(1, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(9, buffer.unallocatedBytes());

    TEST_ASSERT_EQUAL_INT(false, s.isInvalid());
    TEST_ASSERT_EQUAL_INT(true, s.isAllocated());
    TEST_ASSERT_EQUAL_INT(true, s.isEmpty());
    TEST_ASSERT_EQUAL_INT(0, s.rawMaxLength());
    TEST_ASSERT_EQUAL_INT(1, s.rawCapacity());
}

void testAllocateTwiceUsingSingleStringBuffer() {
    CStringBuffer<10, 1> buffer;
    CString s1 = buffer.allocate();
    TEST_ASSERT_EQUAL_INT(1, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(9, buffer.unallocatedBytes());
    CString s2 = buffer.allocate();
    TEST_ASSERT_EQUAL_INT(1, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(9, buffer.unallocatedBytes());

    TEST_ASSERT_EQUAL_INT(false, s1.isInvalid());
    TEST_ASSERT_EQUAL_INT(true, s1.isAllocated());
    TEST_ASSERT_EQUAL_INT(true, s2.isInvalid());
    TEST_ASSERT_EQUAL_INT(false, s2.isAllocated());
    TEST_ASSERT_EQUAL_INT(1, buffer.numstrings());
}

void testAllocateRemaining() {
    CStringBuffer<10, 2> buffer;
    CString s1 = buffer.allocate();
    TEST_ASSERT_EQUAL_INT(1, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(9, buffer.unallocatedBytes());
    CString s2 = buffer.allocateRemaining();
    TEST_ASSERT_EQUAL_INT(10, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, buffer.unallocatedBytes());

    TEST_ASSERT_EQUAL_INT(false, s1.isInvalid());
    TEST_ASSERT_EQUAL_INT(true, s1.isAllocated());
    TEST_ASSERT_EQUAL_INT(false, s2.isInvalid());
    TEST_ASSERT_EQUAL_INT(true, s2.isAllocated());
    TEST_ASSERT_EQUAL_INT(9, s2.rawCapacity());
}

void testAllocateNoCapacityRemains() {
    CStringBuffer<10, 2> buffer;
    CString s1 = buffer.allocateRemaining();
    TEST_ASSERT_EQUAL_INT(10, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, buffer.unallocatedBytes());
    CString s2 = buffer.allocate();
    TEST_ASSERT_EQUAL_INT(10, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, buffer.unallocatedBytes());

    TEST_ASSERT_EQUAL_INT(false, s1.isInvalid());
    TEST_ASSERT_EQUAL_INT(true, s1.isAllocated());
    TEST_ASSERT_EQUAL_INT(true, s2.isInvalid());
    TEST_ASSERT_EQUAL_INT(false, s2.isAllocated());
}

void testPushWithZeroLimit() {
    CStringBuffer<10, 2> buffer;
    CString s1 = buffer.push("12345689", 0);
    TEST_ASSERT_EQUAL_INT(1, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(9, buffer.unallocatedBytes());

    TEST_ASSERT_EQUAL_INT(false, s1.isInvalid());
    TEST_ASSERT_EQUAL_INT(true, s1.isAllocated());

    TEST_ASSERT_EQUAL_INT(0, s1.length());
    TEST_ASSERT_EQUAL_INT(1, s1.rawCapacity());
    TEST_ASSERT_EQUAL_INT(0, s1.rawMaxLength());
}

void testAppendEmptyStringWithBufferCompletelyAllocated() {
    CStringBuffer<10, 2> buffer;
    CString s1 = buffer.allocateRemaining();
    TEST_ASSERT_EQUAL_INT(10, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(10, s1.rawCapacity());

    CString s2 = buffer.appendToTopmost("");
    TEST_ASSERT_EQUAL_INT(false, s2.isInvalid());
    TEST_ASSERT_EQUAL_INT(true, s2.isAllocated());
    TEST_ASSERT_EQUAL_INT(true, s1.raw() == s2.raw());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_INT(false, s1 != s2);
    TEST_ASSERT_EQUAL_INT(10, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(10, s1.rawCapacity());
}

void testAppendNonEmptyStringWithBufferCompletelyAllocated() {
    CStringBuffer<10, 2> buffer;
    CString s1 = buffer.allocateRemaining();
    TEST_ASSERT_EQUAL_INT(10, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(10, s1.rawCapacity());

    CString s2 = buffer.appendToTopmost("123456789");
    TEST_ASSERT_EQUAL_INT(true, s2.isInvalid());
    TEST_ASSERT_EQUAL_INT(false, s2.isAllocated());
    TEST_ASSERT_EQUAL_INT(false, s1.raw() == s2.raw());
    TEST_ASSERT_EQUAL_INT(false, s1 == s2);
    TEST_ASSERT_EQUAL_INT(true, s1 != s2);
    TEST_ASSERT_EQUAL_INT(10, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(10, s1.rawCapacity());
}

void testAppendToEmptyBufferArea() {
    CStringBuffer<10, 2> buffer;
    CString s1 = buffer.allocate(6);
    TEST_ASSERT_EQUAL_INT(7, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(3, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(7, s1.rawCapacity());
    TEST_ASSERT_EQUAL_INT(0, s1.length());
    TEST_ASSERT_EQUAL_INT(0, s1[0]);

    CString s2 = buffer.appendToTopmost("123");
    TEST_ASSERT_EQUAL_INT(false, s2.isInvalid());
    TEST_ASSERT_EQUAL_INT(true, s2.isAllocated());
    TEST_ASSERT_EQUAL_INT(true, s1.raw() == s2.raw());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_INT(false, s1 != s2);
    TEST_ASSERT_EQUAL_INT(10, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(10, s1.rawCapacity());
    TEST_ASSERT_EQUAL_INT(0, s1.length());
    TEST_ASSERT_EQUAL_INT(0, s1[0]);
    TEST_ASSERT_EQUAL_STRING("123", s1.raw()+6);
    TEST_ASSERT_EQUAL_INT(0, s1[9]);
}

void testMoveToTopRemainingCapacityIsMoreThanStringToMove() {
    CStringBuffer<50, 4> buffer;
    CString s1 = buffer.push("1234");
    CString s2 = buffer.push("567");
    CString s3 = buffer.push("89");
    CString s4 = buffer.push("0");

    TEST_ASSERT_EQUAL_INT(14, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(36, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s2.bufferIndex());
    TEST_ASSERT_EQUAL_INT(2, s3.bufferIndex());
    TEST_ASSERT_EQUAL_INT(3, s4.bufferIndex());

    bool result = buffer.moveToTop(s1);
    TEST_ASSERT_EQUAL_INT(true, result);
    TEST_ASSERT_EQUAL_INT(3, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(0, s2.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s3.bufferIndex());
    TEST_ASSERT_EQUAL_INT(2, s4.bufferIndex());
    TEST_ASSERT_EQUAL_STRING("1234", s1.raw());
    TEST_ASSERT_EQUAL_STRING("567", s2.raw());
    TEST_ASSERT_EQUAL_STRING("89", s3.raw());
    TEST_ASSERT_EQUAL_STRING("0", s4.raw());
}

void testMoveToTopNoCapacityRemaining() {
    CStringBuffer<14, 4> buffer;
    CString s1 = buffer.push("1234");
    CString s2 = buffer.push("567");
    CString s3 = buffer.push("89");
    CString s4 = buffer.push("0");

    TEST_ASSERT_EQUAL_INT(14, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s2.bufferIndex());
    TEST_ASSERT_EQUAL_INT(2, s3.bufferIndex());
    TEST_ASSERT_EQUAL_INT(3, s4.bufferIndex());

    bool result = buffer.moveToTop(s1);
    TEST_ASSERT_EQUAL_INT(true, result);
    TEST_ASSERT_EQUAL_INT(3, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(0, s2.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s3.bufferIndex());
    TEST_ASSERT_EQUAL_INT(2, s4.bufferIndex());
    TEST_ASSERT_EQUAL_STRING("1234", s1.raw());
    TEST_ASSERT_EQUAL_STRING("567", s2.raw());
    TEST_ASSERT_EQUAL_STRING("89", s3.raw());
    TEST_ASSERT_EQUAL_STRING("0", s4.raw());
}

void testMoveToTopNoChange() {
    CStringBuffer<9, 2> buffer;
    CString s1 = buffer.push("1234");
    CString s2 = buffer.push("567");

    TEST_ASSERT_EQUAL_INT(9, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s2.bufferIndex());

    bool result = buffer.moveToTop(s2);
    TEST_ASSERT_EQUAL_INT(true, result);
    TEST_ASSERT_EQUAL_INT(0, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s2.bufferIndex());
    TEST_ASSERT_EQUAL_STRING("1234", s1.raw());
    TEST_ASSERT_EQUAL_STRING("567", s2.raw());
}

void testIncreaseSize() {
    CStringBuffer<10, 2> buffer;
    CString s1 = buffer.push("12345");
    TEST_ASSERT_EQUAL_INT(5, s1.length());
    TEST_ASSERT_EQUAL_INT(5, s1.rawMaxLength());
    TEST_ASSERT_EQUAL_INT(6, s1.rawCapacity());

    CString s2 = buffer.resizeTopmost(9);
    TEST_ASSERT_EQUAL_INT(10, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(true, s1.raw() == s2.raw());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_STRING("12345", s2.raw());
    TEST_ASSERT_EQUAL_INT(5, s2.length());
    TEST_ASSERT_EQUAL_INT(9, s1.rawMaxLength());
    TEST_ASSERT_EQUAL_INT(10, s1.rawCapacity());
}

void testReduceSizeShrinkToFit() {
    CStringBuffer<10, 2> buffer;
    CString s1 = buffer.allocateRemaining();
    strcpy(s1.raw(), "12345");

    TEST_ASSERT_EQUAL_INT(5, s1.length());
    TEST_ASSERT_EQUAL_INT(9, s1.rawMaxLength());
    TEST_ASSERT_EQUAL_INT(10, s1.rawCapacity());

    CString s2 = buffer.resizeTopmost(5);
    TEST_ASSERT_EQUAL_INT(6, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(4, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(true, s1.raw() == s2.raw());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_STRING("12345", s2.raw());
    TEST_ASSERT_EQUAL_INT(0, s2[5]);
    TEST_ASSERT_EQUAL_INT(5, s2.length());
    TEST_ASSERT_EQUAL_INT(5, s1.rawMaxLength());
    TEST_ASSERT_EQUAL_INT(6, s1.rawCapacity());
}

void testReduceSizeShortenString() {
    CStringBuffer<10, 2> buffer;
    CString s1 = buffer.allocateRemaining();
    strcpy(s1.raw(), "123456789");

    TEST_ASSERT_EQUAL_INT(9, s1.length());
    TEST_ASSERT_EQUAL_INT(9, s1.rawMaxLength());
    TEST_ASSERT_EQUAL_INT(10, s1.rawCapacity());

    CString s2 = buffer.resizeTopmost(5);
    TEST_ASSERT_EQUAL_INT(6, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(4, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(true, s1.raw() == s2.raw());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_STRING("12345", s2.raw());
    TEST_ASSERT_EQUAL_INT(0, s2[5]);
    TEST_ASSERT_EQUAL_INT(5, s2.length());
    TEST_ASSERT_EQUAL_INT(5, s1.rawMaxLength());
    TEST_ASSERT_EQUAL_INT(6, s1.rawCapacity());
}

void testIncreaseSizeBeyondCapacity() {
    CStringBuffer<10, 2> buffer;
    CString s1 = buffer.allocateRemaining();
    CString s2 = buffer.resizeTopmost(10);
    TEST_ASSERT_EQUAL_INT(true, s2.isInvalid());
}

void testRemoveFirstString() {
    CStringBuffer<50, 4> buffer;
    CString s1 = buffer.push("1234");
    CString s2 = buffer.push("567");
    CString s3 = buffer.push("89");
    CString s4 = buffer.push("0");

    TEST_ASSERT_EQUAL_INT(14, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(36, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s2.bufferIndex());
    TEST_ASSERT_EQUAL_INT(2, s3.bufferIndex());
    TEST_ASSERT_EQUAL_INT(3, s4.bufferIndex());

    bool result = buffer.remove(s1);

    TEST_ASSERT_EQUAL_INT(9, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(41, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(true, result);
    TEST_ASSERT_EQUAL_INT(INVALID_STRING_IDX, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(false, s1.isAllocated());
    TEST_ASSERT_EQUAL_INT(0, s2.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s3.bufferIndex());
    TEST_ASSERT_EQUAL_INT(2, s4.bufferIndex());
    TEST_ASSERT_EQUAL_STRING(0, s1.raw());
    TEST_ASSERT_EQUAL_STRING("567", s2.raw());
    TEST_ASSERT_EQUAL_STRING("89", s3.raw());
    TEST_ASSERT_EQUAL_STRING("0", s4.raw());
}

void testRemoveLastString() {
    CStringBuffer<50, 4> buffer;
    CString s1 = buffer.push("1234");
    CString s2 = buffer.push("567");
    CString s3 = buffer.push("89");
    CString s4 = buffer.push("0");

    TEST_ASSERT_EQUAL_INT(14, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(36, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(0, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s2.bufferIndex());
    TEST_ASSERT_EQUAL_INT(2, s3.bufferIndex());
    TEST_ASSERT_EQUAL_INT(3, s4.bufferIndex());

    bool result = buffer.remove(s4);

    TEST_ASSERT_EQUAL_INT(12, buffer.allocatedBytes());
    TEST_ASSERT_EQUAL_INT(38, buffer.unallocatedBytes());
    TEST_ASSERT_EQUAL_INT(true, result);
    TEST_ASSERT_EQUAL_INT(0, s1.bufferIndex());
    TEST_ASSERT_EQUAL_INT(1, s2.bufferIndex());
    TEST_ASSERT_EQUAL_INT(2, s3.bufferIndex());
    TEST_ASSERT_EQUAL_INT(INVALID_STRING_IDX, s4.bufferIndex());
    TEST_ASSERT_EQUAL_INT(false, s4.isAllocated());
    TEST_ASSERT_EQUAL_STRING("1234", s1.raw());
    TEST_ASSERT_EQUAL_STRING("567", s2.raw());
    TEST_ASSERT_EQUAL_STRING("89", s3.raw());
    TEST_ASSERT_EQUAL_STRING(0, s4.raw());
}

void setUp() {};
void tearDown() {};

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(testAllocate);
    RUN_TEST(testAllocateTwiceUsingSingleStringBuffer);
    RUN_TEST(testAllocateRemaining);
    RUN_TEST(testAllocateNoCapacityRemains);
    RUN_TEST(testPushWithZeroLimit);
    RUN_TEST(testAppendEmptyStringWithBufferCompletelyAllocated);
    RUN_TEST(testAppendNonEmptyStringWithBufferCompletelyAllocated);
    RUN_TEST(testAppendToEmptyBufferArea);
    RUN_TEST(testMoveToTopRemainingCapacityIsMoreThanStringToMove);
    RUN_TEST(testMoveToTopNoCapacityRemaining);
    RUN_TEST(testMoveToTopNoChange);
    RUN_TEST(testIncreaseSize);
    RUN_TEST(testReduceSizeShortenString);
    RUN_TEST(testReduceSizeShrinkToFit);
    RUN_TEST(testIncreaseSizeBeyondCapacity);
    RUN_TEST(testRemoveFirstString);
    RUN_TEST(testRemoveLastString);

    return UNITY_END();
}