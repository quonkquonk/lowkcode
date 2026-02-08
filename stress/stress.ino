#include <Arduino.h>

static const uint32_t TEST_DURATION_MS = 10000;

bool isPrime(uint32_t n) {
  if (n < 2) return false;
  if (n == 2) return true;
  if ((n & 1) == 0) return false;  // even numbers

  for (uint32_t i = 3; i * i <= n; i += 2) {
    if (n % i == 0) return false;
  }
  return true;
}

void setup() {
  Serial.begin(9600);
  delay(2000);

  Serial.println("ESP32 Prime Benchmark");
  Serial.println("Running for 10 seconds...");
  delay(1000);

  uint32_t startTime = millis();
  uint32_t count = 0;
  uint32_t n = 2;

  while (millis() - startTime < TEST_DURATION_MS) {
    if (isPrime(n)) {
      count++;
    }
    n++;
  }

  uint32_t elapsed = millis() - startTime;

  Serial.println();
  Serial.print("Time (ms): ");
  Serial.println(elapsed);
  Serial.print("Primes found: ");
  Serial.println(count);
  Serial.print("Last number tested: ");
  Serial.println(n - 1);
}

void loop() {
  // nothing
}
