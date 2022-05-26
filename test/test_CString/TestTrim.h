#include "CString.h"
#include <unity.h>

void testTrimStartWhitespace() {
    CStringBuffer<30, 1> buffer;
    CString s1 = buffer.push(" \t\n\v\f\r123\r\f\v\n\t ");

    TEST_ASSERT_EQUAL_INT(15, s1.length());
    TEST_ASSERT_EQUAL_INT(16, s1.rawCapacity());

    CString s2 = s1.trimStart();
    TEST_ASSERT_EQUAL_INT(9, s2.length());
    TEST_ASSERT_EQUAL_INT(16, s2.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s2.raw());
    TEST_ASSERT_EQUAL_STRING("123\r\f\v\n\t ", s2.raw());
}

void testTrimStartSingleCharacter() {
    CStringBuffer<30, 1> buffer;
    CString s1 = buffer.push("01234567899876543210");

    TEST_ASSERT_EQUAL_INT(20, s1.length());
    TEST_ASSERT_EQUAL_INT(21, s1.rawCapacity());

    CString s2 = s1.trimStart('9');
    TEST_ASSERT_EQUAL_INT(20, s2.length());
    TEST_ASSERT_EQUAL_INT(21, s2.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s2.raw());
    TEST_ASSERT_EQUAL_STRING("01234567899876543210", s2.raw());

    CString s3 = s1.trimStart('0');
    TEST_ASSERT_EQUAL_INT(19, s3.length());
    TEST_ASSERT_EQUAL_INT(21, s3.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s3);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s3.raw());
    TEST_ASSERT_EQUAL_STRING("1234567899876543210", s3.raw());

    CString s4 = s1.trimStart('1');
    TEST_ASSERT_EQUAL_INT(18, s4.length());
    TEST_ASSERT_EQUAL_INT(21, s4.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s4);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s4.raw());
    TEST_ASSERT_EQUAL_STRING("234567899876543210", s4.raw());
}

void testTrimStartCharacterSet() {
    CStringBuffer<30, 1> buffer;
    CString s1 = buffer.push("01234567899876543210");

    TEST_ASSERT_EQUAL_INT(20, s1.length());
    TEST_ASSERT_EQUAL_INT(21, s1.rawCapacity());

    CString s2 = s1.trimStart("123456789");
    TEST_ASSERT_EQUAL_INT(20, s2.length());
    TEST_ASSERT_EQUAL_INT(21, s2.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s2.raw());
    TEST_ASSERT_EQUAL_STRING("01234567899876543210", s2.raw());

    CString s3 = s1.trimStart("01");
    TEST_ASSERT_EQUAL_INT(18, s3.length());
    TEST_ASSERT_EQUAL_INT(21, s3.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s3);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s3.raw());
    TEST_ASSERT_EQUAL_STRING("234567899876543210", s3.raw());

    CString s4 = s1.trimStart("32");
    TEST_ASSERT_EQUAL_INT(16, s4.length());
    TEST_ASSERT_EQUAL_INT(21, s4.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s4);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s4.raw());
    TEST_ASSERT_EQUAL_STRING("4567899876543210", s4.raw());

    CString s5 = s1.trimStart("0123456789");
    TEST_ASSERT_EQUAL_INT(0, s5.length());
    TEST_ASSERT_EQUAL_INT(21, s5.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s5);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s5.raw());
    TEST_ASSERT_EQUAL_STRING("", s5.raw());
}

void testTrimStartPredicate() {
    CStringBuffer<30, 1> buffer;
    CString s1 = buffer.push("01234567899876543210");

    std::function<bool(const char)> predicateNot0 = [](const char c) {
        return c != '0';
    };
    std::function<bool(const char)> predicate01 = [](const char c) {
        return c == '0' || c == '1';
    };
    std::function<bool(const char)> predicateNone = [](const char c) {
        return false;
    };
    std::function<bool(const char)> predicateAny = [](const char c) {
        return true;
    };

    TEST_ASSERT_EQUAL_INT(20, s1.length());
    TEST_ASSERT_EQUAL_INT(21, s1.rawCapacity());

    CString s2 = s1.trimStart(predicateNone);
    TEST_ASSERT_EQUAL_INT(20, s2.length());
    TEST_ASSERT_EQUAL_INT(21, s2.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s2.raw());
    TEST_ASSERT_EQUAL_STRING("01234567899876543210", s2.raw());

    CString s3 = s1.trimStart(predicateNot0);
    TEST_ASSERT_EQUAL_INT(20, s3.length());
    TEST_ASSERT_EQUAL_INT(21, s3.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s3);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s3.raw());
    TEST_ASSERT_EQUAL_STRING("01234567899876543210", s3.raw());

    CString s4 = s1.trimStart(predicate01);
    TEST_ASSERT_EQUAL_INT(18, s4.length());
    TEST_ASSERT_EQUAL_INT(21, s4.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s4);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s4.raw());
    TEST_ASSERT_EQUAL_STRING("234567899876543210", s4.raw());

    CString s5 = s1.trimStart(predicateNot0);
    TEST_ASSERT_EQUAL_INT(1, s5.length());
    TEST_ASSERT_EQUAL_INT(21, s5.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s5);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s5.raw());
    TEST_ASSERT_EQUAL_STRING("0", s5.raw());

    CString s6 = s1.trimStart(predicateAny);
    TEST_ASSERT_EQUAL_INT(0, s6.length());
    TEST_ASSERT_EQUAL_INT(21, s6.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s6);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s6.raw());
    TEST_ASSERT_EQUAL_STRING("", s6.raw());
}

void testTrimEndWhitespace() {
    CStringBuffer<30, 1> buffer;
    CString s1 = buffer.push(" \t\n\v\f\r123\r\f\v\n\t ");

    TEST_ASSERT_EQUAL_INT(15, s1.length());
    TEST_ASSERT_EQUAL_INT(16, s1.rawCapacity());

    CString s2 = s1.trimEnd();
    TEST_ASSERT_EQUAL_INT(9, s2.length());
    TEST_ASSERT_EQUAL_INT(16, s2.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s2.raw());
    TEST_ASSERT_EQUAL_STRING(" \t\n\v\f\r123", s2.raw());
}

void testTrimEndSingleCharacter() {
    CStringBuffer<30, 1> buffer;
    CString s1 = buffer.push("01234567899876543210");

    TEST_ASSERT_EQUAL_INT(20, s1.length());
    TEST_ASSERT_EQUAL_INT(21, s1.rawCapacity());

    CString s2 = s1.trimEnd('9');
    TEST_ASSERT_EQUAL_INT(20, s2.length());
    TEST_ASSERT_EQUAL_INT(21, s2.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s2.raw());
    TEST_ASSERT_EQUAL_STRING("01234567899876543210", s2.raw());

    CString s3 = s1.trimEnd('0');
    TEST_ASSERT_EQUAL_INT(19, s3.length());
    TEST_ASSERT_EQUAL_INT(21, s3.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s3);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s3.raw());
    TEST_ASSERT_EQUAL_STRING("0123456789987654321", s3.raw());

    CString s4 = s1.trimEnd('1');
    TEST_ASSERT_EQUAL_INT(18, s4.length());
    TEST_ASSERT_EQUAL_INT(21, s4.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s4);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s4.raw());
    TEST_ASSERT_EQUAL_STRING("012345678998765432", s4.raw());
}

void testTrimEndCharacterSet() {
    CStringBuffer<30, 1> buffer;
    CString s1 = buffer.push("01234567899876543210");

    TEST_ASSERT_EQUAL_INT(20, s1.length());
    TEST_ASSERT_EQUAL_INT(21, s1.rawCapacity());

    CString s2 = s1.trimEnd("123456789");
    TEST_ASSERT_EQUAL_INT(20, s2.length());
    TEST_ASSERT_EQUAL_INT(21, s2.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s2.raw());
    TEST_ASSERT_EQUAL_STRING("01234567899876543210", s2.raw());

    CString s3 = s1.trimEnd("10");
    TEST_ASSERT_EQUAL_INT(18, s3.length());
    TEST_ASSERT_EQUAL_INT(21, s3.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s3);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s3.raw());
    TEST_ASSERT_EQUAL_STRING("012345678998765432", s3.raw());

    CString s4 = s1.trimEnd("23");
    TEST_ASSERT_EQUAL_INT(16, s4.length());
    TEST_ASSERT_EQUAL_INT(21, s4.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s4);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s4.raw());
    TEST_ASSERT_EQUAL_STRING("0123456789987654", s4.raw());

    CString s5 = s1.trimEnd("0123456789");
    TEST_ASSERT_EQUAL_INT(0, s5.length());
    TEST_ASSERT_EQUAL_INT(21, s5.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s5);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s5.raw());
    TEST_ASSERT_EQUAL_STRING("", s5.raw());
}

void testTrimEndPredicate() {
    CStringBuffer<30, 1> buffer;
    CString s1 = buffer.push("01234567899876543210");

    std::function<bool(const char)> predicateNot0 = [](const char c) {
        return c != '0';
    };
    std::function<bool(const char)> predicate01 = [](const char c) {
        return c == '0' || c == '1';
    };
    std::function<bool(const char)> predicateNone = [](const char c) {
        return false;
    };
    std::function<bool(const char)> predicateAny = [](const char c) {
        return true;
    };

    TEST_ASSERT_EQUAL_INT(20, s1.length());
    TEST_ASSERT_EQUAL_INT(21, s1.rawCapacity());

    CString s2 = s1.trimEnd(predicateNone);
    TEST_ASSERT_EQUAL_INT(20, s2.length());
    TEST_ASSERT_EQUAL_INT(21, s2.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s2.raw());
    TEST_ASSERT_EQUAL_STRING("01234567899876543210", s2.raw());

    CString s3 = s1.trimEnd(predicateNot0);
    TEST_ASSERT_EQUAL_INT(20, s3.length());
    TEST_ASSERT_EQUAL_INT(21, s3.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s3);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s3.raw());
    TEST_ASSERT_EQUAL_STRING("01234567899876543210", s3.raw());

    CString s4 = s1.trimEnd(predicate01);
    TEST_ASSERT_EQUAL_INT(18, s4.length());
    TEST_ASSERT_EQUAL_INT(21, s4.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s4);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s4.raw());
    TEST_ASSERT_EQUAL_STRING("012345678998765432", s4.raw());

    CString s5 = s1.trimEnd(predicateNot0);
    TEST_ASSERT_EQUAL_INT(1, s5.length());
    TEST_ASSERT_EQUAL_INT(21, s5.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s5);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s5.raw());
    TEST_ASSERT_EQUAL_STRING("0", s5.raw());

    CString s6 = s1.trimEnd(predicateAny);
    TEST_ASSERT_EQUAL_INT(0, s6.length());
    TEST_ASSERT_EQUAL_INT(21, s6.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s6);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s6.raw());
    TEST_ASSERT_EQUAL_STRING("", s6.raw());
}

void testTrimWhitespace() {
    CStringBuffer<30, 1> buffer;
    CString s1 = buffer.push(" \t\n\v\f\r123\r\f\v\n\t ");

    TEST_ASSERT_EQUAL_INT(15, s1.length());
    TEST_ASSERT_EQUAL_INT(16, s1.rawCapacity());

    CString s2 = s1.trim();
    TEST_ASSERT_EQUAL_INT(3, s2.length());
    TEST_ASSERT_EQUAL_INT(16, s2.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s2.raw());
    TEST_ASSERT_EQUAL_STRING("123", s2.raw());
}

void testTrimSingleCharacter() {
    CStringBuffer<30, 1> buffer;
    CString s1 = buffer.push("01234567899876543210");

    TEST_ASSERT_EQUAL_INT(20, s1.length());
    TEST_ASSERT_EQUAL_INT(21, s1.rawCapacity());

    CString s2 = s1.trim('9');
    TEST_ASSERT_EQUAL_INT(20, s2.length());
    TEST_ASSERT_EQUAL_INT(21, s2.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s2.raw());
    TEST_ASSERT_EQUAL_STRING("01234567899876543210", s2.raw());

    CString s3 = s1.trim('0');
    TEST_ASSERT_EQUAL_INT(18, s3.length());
    TEST_ASSERT_EQUAL_INT(21, s3.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s3);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s3.raw());
    TEST_ASSERT_EQUAL_STRING("123456789987654321", s3.raw());

    CString s4 = s1.trim('1');
    TEST_ASSERT_EQUAL_INT(16, s4.length());
    TEST_ASSERT_EQUAL_INT(21, s4.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s4);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s4.raw());
    TEST_ASSERT_EQUAL_STRING("2345678998765432", s4.raw());
}

void testTrimCharacterSet() {
    CStringBuffer<30, 1> buffer;
    CString s1 = buffer.push("01234567899876543210");

    TEST_ASSERT_EQUAL_INT(20, s1.length());
    TEST_ASSERT_EQUAL_INT(21, s1.rawCapacity());

    CString s2 = s1.trim("123456789");
    TEST_ASSERT_EQUAL_INT(20, s2.length());
    TEST_ASSERT_EQUAL_INT(21, s2.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s2.raw());
    TEST_ASSERT_EQUAL_STRING("01234567899876543210", s2.raw());

    CString s3 = s1.trim("01");
    TEST_ASSERT_EQUAL_INT(16, s3.length());
    TEST_ASSERT_EQUAL_INT(21, s3.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s3);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s3.raw());
    TEST_ASSERT_EQUAL_STRING("2345678998765432", s3.raw());

    CString s4 = s1.trim("32");
    TEST_ASSERT_EQUAL_INT(12, s4.length());
    TEST_ASSERT_EQUAL_INT(21, s4.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s4);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s4.raw());
    TEST_ASSERT_EQUAL_STRING("456789987654", s4.raw());

    CString s5 = s1.trim("0123456789");
    TEST_ASSERT_EQUAL_INT(0, s5.length());
    TEST_ASSERT_EQUAL_INT(21, s5.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s5);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s5.raw());
    TEST_ASSERT_EQUAL_STRING("", s5.raw());
}

void testTrimPredicate() {
    CStringBuffer<30, 1> buffer;
    CString s1 = buffer.push("01234567899876543210");

    std::function<bool(const char)> predicateNot0 = [](const char c) {
        return c != '0';
    };
    std::function<bool(const char)> predicate01 = [](const char c) {
        return c == '0' || c == '1';
    };
    std::function<bool(const char)> predicateNot8 = [](const char c) {
        return c != '8';
    };
    std::function<bool(const char)> predicateNone = [](const char c) {
        return false;
    };
    std::function<bool(const char)> predicateAny = [](const char c) {
        return true;
    };

    TEST_ASSERT_EQUAL_INT(20, s1.length());
    TEST_ASSERT_EQUAL_INT(21, s1.rawCapacity());

    CString s2 = s1.trim(predicateNone);
    TEST_ASSERT_EQUAL_INT(20, s2.length());
    TEST_ASSERT_EQUAL_INT(21, s2.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s2);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s2.raw());
    TEST_ASSERT_EQUAL_STRING("01234567899876543210", s2.raw());

    CString s3 = s1.trim(predicateNot0);
    TEST_ASSERT_EQUAL_INT(20, s3.length());
    TEST_ASSERT_EQUAL_INT(21, s3.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s3);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s3.raw());
    TEST_ASSERT_EQUAL_STRING("01234567899876543210", s3.raw());

    CString s4 = s1.trim(predicate01);
    TEST_ASSERT_EQUAL_INT(16, s4.length());
    TEST_ASSERT_EQUAL_INT(21, s4.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s4);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s4.raw());
    TEST_ASSERT_EQUAL_STRING("2345678998765432", s4.raw());

    CString s5 = s1.trim(predicateNot8);
    TEST_ASSERT_EQUAL_INT(4, s5.length());
    TEST_ASSERT_EQUAL_INT(21, s5.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s5);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s5.raw());
    TEST_ASSERT_EQUAL_STRING("8998", s5.raw());

    CString s6 = s1.trim(predicateAny);
    TEST_ASSERT_EQUAL_INT(0, s6.length());
    TEST_ASSERT_EQUAL_INT(21, s6.rawCapacity());
    TEST_ASSERT_EQUAL_INT(true, s1 == s6);
    TEST_ASSERT_EQUAL_PTR(s1.raw(), s6.raw());
    TEST_ASSERT_EQUAL_STRING("", s6.raw());
}

void runTestTrim() {
    const char* prevFile = Unity.TestFile;
    Unity.TestFile = __FILE__;

    RUN_TEST(testTrimStartWhitespace);
    RUN_TEST(testTrimStartSingleCharacter);
    RUN_TEST(testTrimStartCharacterSet);
    RUN_TEST(testTrimStartPredicate);
    RUN_TEST(testTrimEndWhitespace);
    RUN_TEST(testTrimEndSingleCharacter);
    RUN_TEST(testTrimEndCharacterSet);
    RUN_TEST(testTrimEndPredicate);
    RUN_TEST(testTrimWhitespace);
    RUN_TEST(testTrimSingleCharacter);
    RUN_TEST(testTrimCharacterSet);
    RUN_TEST(testTrimPredicate);

    Unity.TestFile = prevFile;
}
