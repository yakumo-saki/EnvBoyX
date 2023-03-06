#include "unity.h"
#include <Arduino.h>
#include "TimeUtil.h"

void setUp(void) {
  // set stuff up here
  String s = "s";
}

void tearDown(void) {
  // clean stuff up here
}

void test_timeUtil_simple(void) {
  TEST_ASSERT_TRUE(betweenTime("10:00","09:00","11:00"));

  // 境界値
  TEST_ASSERT_TRUE(betweenTime("10:00","10:00","11:00"));
  TEST_ASSERT_TRUE(betweenTime("11:00","10:00","11:00"));
}


void test_timeUtil_overday(void) {
  TEST_ASSERT_TRUE(betweenTime("03:00","23:00","05:00"));

  // 境界値
  TEST_ASSERT_TRUE(betweenTime("03:00","23:00","03:00"));
  TEST_ASSERT_TRUE(betweenTime("23:00","23:00","03:00"));
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_timeUtil_simple);
  RUN_TEST(test_timeUtil_overday);
  return UNITY_END();
}



/**
  * For Arduino framework
  */
void setup() {
  // Wait ~2 seconds before the Unity test runner
  // establishes connection with a board Serial interface
  delay(2000);

  runUnityTests();
}
void loop() {}