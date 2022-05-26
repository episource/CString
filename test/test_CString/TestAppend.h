#include "CString.h"
#include <unity.h>

void testAppendAfterClearNeedsResize() {
    CStringBuffer<10, 1> buffer;
    CString text = buffer.allocate();

    text.append("123");
    text.clear();
    text.append("123456");
    TEST_ASSERT_EQUAL_STRING("123456", text.raw());
}

void runTestAppend() {
    const char* prevFile = Unity.TestFile;
    Unity.TestFile = __FILE__;

    RUN_TEST(testAppendAfterClearNeedsResize);

    Unity.TestFile = prevFile;
}