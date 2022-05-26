#include "CString.h"
#include <unity.h>

void testIndexOfCharacter() {
    CStringBuffer<25, 1> buffer;
    CString s1 = buffer.push("01234567890123456789").resize(24);

    TEST_ASSERT_EQUAL_INT(0, s1.indexOf('0'));
    TEST_ASSERT_EQUAL_INT(0, s1.indexOf('0', 0));
    TEST_ASSERT_EQUAL_INT(10, s1.indexOf('0', 1));
    TEST_ASSERT_EQUAL_INT(10, s1.indexOf('0', 10));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf('0', 11));

    TEST_ASSERT_EQUAL_INT(9, s1.indexOf('9'));
    TEST_ASSERT_EQUAL_INT(9, s1.indexOf('9', 0));
    TEST_ASSERT_EQUAL_INT(9, s1.indexOf('9', 1));
    TEST_ASSERT_EQUAL_INT(19, s1.indexOf('9', 10));
    TEST_ASSERT_EQUAL_INT(19, s1.indexOf('9', 19));

    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf('x'));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf('0', -1));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf('0', 20));
}

void testLastIndexOfCharacter() {
    CStringBuffer<25, 1> buffer;
    CString s1 = buffer.push("01234567890123456789").resize(24);;

    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf('0'));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf('0', 19));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf('0', 10));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOf('0', 9));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOf('0', 0));

    TEST_ASSERT_EQUAL_INT(19, s1.lastIndexOf('9'));
    TEST_ASSERT_EQUAL_INT(19, s1.lastIndexOf('9', 19));
    TEST_ASSERT_EQUAL_INT(9, s1.lastIndexOf('9', 10));
    TEST_ASSERT_EQUAL_INT(9, s1.lastIndexOf('9', 9));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf('9', 8));

    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf('x'));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf('0', -1));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf('0', 20));
}

void testIndexOfAny() {
    CStringBuffer<25, 1> buffer;
    CString s1 = buffer.push("01234567890123456789").resize(24);;

    TEST_ASSERT_EQUAL_INT(0, s1.indexOfAny("09"));
    TEST_ASSERT_EQUAL_INT(0, s1.indexOfAny("09", 0));
    TEST_ASSERT_EQUAL_INT(9, s1.indexOfAny("09", 1));
    TEST_ASSERT_EQUAL_INT(9, s1.indexOfAny("09", 9));
    TEST_ASSERT_EQUAL_INT(10, s1.indexOfAny("09", 10));
    TEST_ASSERT_EQUAL_INT(19, s1.indexOfAny("09", 11));
    TEST_ASSERT_EQUAL_INT(19, s1.indexOfAny("09", 19));

    TEST_ASSERT_EQUAL_INT(-1, s1.indexOfAny(""));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOfAny("\x0009"));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOfAny("xyz"));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOfAny("09", -1));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOfAny("09", 20));
}

void testLastIndexOfAny() {
    CStringBuffer<25, 1> buffer;
    CString s1 = buffer.push("01234567890123456789").resize(24);;

    TEST_ASSERT_EQUAL_INT(19, s1.lastIndexOfAny("09"));
    TEST_ASSERT_EQUAL_INT(19, s1.lastIndexOfAny("09", 24));
    TEST_ASSERT_EQUAL_INT(19, s1.lastIndexOfAny("09", 19));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOfAny("09", 18));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOfAny("09", 10));
    TEST_ASSERT_EQUAL_INT(9, s1.lastIndexOfAny("09", 9));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOfAny("09", 8));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOfAny("09", 0));

    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOfAny(""));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOfAny("\x0009"));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOfAny("xyz"));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOfAny("09", -1));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOfAny("09", 25));
}

void testIndexOfAnyPredicate() {
    CStringBuffer<25, 1> buffer;
    CString s1 = buffer.push("01234567890123456789").resize(24);;
    std::function<bool(const char)> predicate09 = [](const char c)  {
        return c == '0' || c== '9';
    };
    std::function<bool(const char)> predicateNone = [](const char c) {
        return false;
    };
    std::function<bool(const char)> predicateNull = [](const char c) {
        return c == '\0' ? true : false;
    };

    TEST_ASSERT_EQUAL_INT(0, s1.indexOfAny(predicate09));
    TEST_ASSERT_EQUAL_INT(0, s1.indexOfAny(predicate09, 0));
    TEST_ASSERT_EQUAL_INT(9, s1.indexOfAny(predicate09, 1));
    TEST_ASSERT_EQUAL_INT(9, s1.indexOfAny(predicate09, 9));
    TEST_ASSERT_EQUAL_INT(10, s1.indexOfAny(predicate09, 10));
    TEST_ASSERT_EQUAL_INT(19, s1.indexOfAny(predicate09, 11));
    TEST_ASSERT_EQUAL_INT(19, s1.indexOfAny(predicate09, 19));

    TEST_ASSERT_EQUAL_INT(-1, s1.indexOfAny(predicateNone));

    TEST_ASSERT_EQUAL_INT(20, s1.indexOfAny(predicateNull));
    TEST_ASSERT_EQUAL_INT(24, s1.indexOfAny(predicateNull, 24));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOfAny(predicateNull, -1));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOfAny(predicateNull, 25));
}

void testLastIndexOfAnyPredicate() {
    CStringBuffer<25, 1> buffer;
    CString s1 = buffer.push("01234567890123456789").resize(24);;
    std::function<bool(const char)> predicate09 = [](const char c)  {
        return c == '0' || c== '9';
    };
    std::function<bool(const char)> predicateNone = [](const char c) {
        return false;
    };
    std::function<bool(const char)> predicateNull = [](const char c) {
        return c == '\0' ? true : false;
    };

    TEST_ASSERT_EQUAL_INT(19, s1.lastIndexOfAny(predicate09));
    TEST_ASSERT_EQUAL_INT(19, s1.lastIndexOfAny(predicate09, 19));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOfAny(predicate09, 18));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOfAny(predicate09, 10));
    TEST_ASSERT_EQUAL_INT(9, s1.lastIndexOfAny(predicate09, 9));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOfAny(predicate09, 8));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOfAny(predicate09, 0));

    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOfAny(predicateNone));

    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOfAny(predicateNull, 25));
    TEST_ASSERT_EQUAL_INT(24, s1.lastIndexOfAny(predicateNull, 24));
    TEST_ASSERT_EQUAL_INT(20, s1.lastIndexOfAny(predicateNull, 20));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOfAny(predicateNull, 19));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOfAny(predicateNull, -1));
}

void testIndexOfString() {
    CStringBuffer<25, 1> buffer;
    CString s1 = buffer.push("01234567890123456789").resize(24);;

    TEST_ASSERT_EQUAL_INT(0, s1.indexOf("01234567890123456789"));

    TEST_ASSERT_EQUAL_INT(0, s1.indexOf("01"));
    TEST_ASSERT_EQUAL_INT(0, s1.indexOf("01", 0));
    TEST_ASSERT_EQUAL_INT(10, s1.indexOf("01", 1));
    TEST_ASSERT_EQUAL_INT(10, s1.indexOf("01", 10));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf("01", 11));

    TEST_ASSERT_EQUAL_INT(0, s1.indexOf("01\x00xyz"));
    TEST_ASSERT_EQUAL_INT(0, s1.indexOf("01\x00xyz", 0));
    TEST_ASSERT_EQUAL_INT(10, s1.indexOf("01\x00xyz", 1));
    TEST_ASSERT_EQUAL_INT(10, s1.indexOf("01\x00xyz", 10));

    TEST_ASSERT_EQUAL_INT(0, s1.indexOf("01xyz", 0, 2));
    TEST_ASSERT_EQUAL_INT(10, s1.indexOf("01xyz", 1, 2));
    TEST_ASSERT_EQUAL_INT(10, s1.indexOf("01xyz", 10, 2));

    TEST_ASSERT_EQUAL_INT(0, s1.indexOf(""));
    TEST_ASSERT_EQUAL_INT(1, s1.indexOf("", 1));

    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf("\x0001"));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf("xyz"));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf("01234567890123456789longer"));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf("01", -1));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf("01", 20));
}

void testLastIndexOfString() {
    CStringBuffer<25, 1> buffer;
    CString s1 = buffer.push("01234567890123456789").resize(24);;

    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOf("01234567890123456789"));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOf("01234567890123456789", 24));

    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf("01"));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf("01", 24));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf("01", 19));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf("01", 11));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOf("01", 10));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOf("01", 1));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf("01", 0));

    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf("01\x00xyz"));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf("01\x00xyz", 19));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOf("01\x00xyz", 10));

    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf("01xyz", 19, 2));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOf("01xyz", 10, 2));

    TEST_ASSERT_EQUAL_INT(19, s1.lastIndexOf(""));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf("", 10));

    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf("\x0001"));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf("xyz"));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf("01234567890123456789longer"));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf("01", -1));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf("01", 25));
}

void testIndexOfCStringSameBuffer() {
    CStringBuffer<50, 5> buffer;
    CString s1 = buffer.push("01234567890123456789").resize(24);;
    CString s2 = buffer.push("01");
    CString s3 = buffer.push("xyz");
    CString s4 = buffer.allocate();
    CString s5 = buffer.allocate().deallocate();

    TEST_ASSERT_EQUAL_INT(0, s1.indexOf(s1));

    TEST_ASSERT_EQUAL_INT(0, s1.indexOf(s2));
    TEST_ASSERT_EQUAL_INT(0, s1.indexOf(s2, 0));
    TEST_ASSERT_EQUAL_INT(10, s1.indexOf(s2, 1));
    TEST_ASSERT_EQUAL_INT(10, s1.indexOf(s2, 10));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf(s2, 11));

    TEST_ASSERT_EQUAL_INT(0, s1.indexOf(s4));
    TEST_ASSERT_EQUAL_INT(1, s1.indexOf(s4, 1));

    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf(s2, -1));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf(s2, 20));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf(s3));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf(s5));
    TEST_ASSERT_EQUAL_INT(-1, s1.indexOf(CString::INVALID));
}

void testLastIndexOfCStringSameBuffer() {
    CStringBuffer<50, 5> buffer;
    CString s1 = buffer.push("01234567890123456789").resize(24);;
    CString s2 = buffer.push("01");
    CString s3 = buffer.push("xyz");
    CString s4 = buffer.allocate();
    CString s5 = buffer.allocate().deallocate();

    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOf(s1));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOf(s1, 24));

    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf(s2));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf(s2, 19));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf(s2, 11));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOf(s2, 10));
    TEST_ASSERT_EQUAL_INT(0, s1.lastIndexOf(s2, 1));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf(s2, 0));

    TEST_ASSERT_EQUAL_INT(19, s1.lastIndexOf(s4));
    TEST_ASSERT_EQUAL_INT(10, s1.lastIndexOf(s4, 10));

    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf(s1, -1));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf(s1, 25));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf(s3));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf(s5));
    TEST_ASSERT_EQUAL_INT(-1, s1.lastIndexOf(CString::INVALID));
}


void runTestIndexOf() {
    const char* prevFile = Unity.TestFile;
    Unity.TestFile = __FILE__;

    RUN_TEST(testIndexOfCharacter);
    RUN_TEST(testLastIndexOfCharacter);
    RUN_TEST(testIndexOfAny);
    RUN_TEST(testLastIndexOfAny);
    RUN_TEST(testIndexOfAnyPredicate);
    RUN_TEST(testLastIndexOfAnyPredicate);
    RUN_TEST(testIndexOfString);
    RUN_TEST(testLastIndexOfString);
    RUN_TEST(testIndexOfCStringSameBuffer);
    RUN_TEST(testLastIndexOfCStringSameBuffer);

    Unity.TestFile = prevFile;
}
