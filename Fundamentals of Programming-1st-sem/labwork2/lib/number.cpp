#include "number.h"

#include <string>

int maxblocks = 68 - 1;  // max len blocks of array
int cntNumInBlck = 9;    // max len elems of block

int lenNumUint(uint64_t value) {
  int len = 0;

  while (value > 0) {
    value /= 10;
    len++;
  }

  return len;
}

int FindFirstBlckNoZero(const uint2022_t& value) {
  int firstBlckNoZero = 0;

  if (value != from_uint(0)) {
    for (int i = 0; i <= maxblocks; i++) {
      if (value.blocks[i] != 0 && firstBlckNoZero == 0) {
        firstBlckNoZero = i;
      }
    }
  }

  return firstBlckNoZero;
}

std::string ToString(uint2022_t x) {
  if (x == from_uint(0)) return "0";
  int firstBlckNoZero = FindFirstBlckNoZero(x);
  std::string retString;

  for (int i = firstBlckNoZero; i <= maxblocks; i++) {
    if (i == firstBlckNoZero) {
      retString = retString + std::to_string(x.blocks[i]);
    } else {  // add fisrts "0"
      for (int j = cntNumInBlck - lenNumUint(x.blocks[i]); j > 0; j--) {
        retString = retString + "0";
      }

      retString = retString + std::to_string(x.blocks[i]);
    }
  }
  return retString;
}

void shiftLeft(uint2022_t& x) {
  if (x == from_uint(0)) return;

  uint2022_t y = x;

  for (int i = maxblocks; i > 0; i--) {
    x.blocks[i - 1] = y.blocks[i];
  }

  x.blocks[maxblocks] = 0;
}

uint2022_t from_uint(uint32_t i) {
  uint2022_t x;

  if (lenNumUint(i) <= cntNumInBlck) {
    x.blocks[67] = i;
  } else {
    x.blocks[67] = i % uint32_t(pow(10, cntNumInBlck));
    x.blocks[66] = int(i / uint32_t(pow(10, cntNumInBlck)));
  }

  return x;
}

uint2022_t from_string(const char* buff) {
  uint2022_t x;
  std::string str = buff;
  int count = 0;
  uint32_t el = 0;
  int j = maxblocks;

  for (int i = strlen(buff) - 1; i >= 0; i--) {
    el = (int(buff[i]) - 48) * pow(10, count) + el;
    count++;

    if (count == cntNumInBlck || i == 0) {
      x.blocks[j] = el;
      j--;  // next block
      count = 0;
      el = 0;
    }
  }

  return x;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint2022_t t;
  uint32_t el = 0;
  int over = 0;
  size_t lLen;
  size_t rLen;
  size_t maxLen;
  size_t sLen;

  for (int i = maxblocks; i >= 0; i--) {
    lLen = lenNumUint(lhs.blocks[i]);
    rLen = lenNumUint(rhs.blocks[i]);
    maxLen = std::max(lLen, rLen);
    el = lhs.blocks[i] + rhs.blocks[i] + over;
    sLen = lenNumUint(el);
    over = 0;

    if (sLen > maxLen) {
      t.blocks[i] = el % int(pow(10, maxLen));
      ;
      over = el / int(pow(10, maxLen));
    } else {
      t.blocks[i] = el;
    }
  }

  return t;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint2022_t t;
  uint32_t el = 0;
  int over = 0;
  for (int i = maxblocks; i >= 0; i--) {
    if (lhs.blocks[i] >= rhs.blocks[i]) {
      if (lhs.blocks[i] - rhs.blocks[i] + over < 0) {  // if we have -1
        el = 999999999;
        over = -1;
      } else {
        el = lhs.blocks[i] - rhs.blocks[i] + over;
        over = 0;
      }

    } else {
      el = lhs.blocks[i] + uint32_t(pow(10, cntNumInBlck)) - rhs.blocks[i] +
           over;
      over = -1;
    }

    t.blocks[i] = el;
  }

  return t;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint2022_t value;
  uint64_t el = 0;
  uint32_t over = 0;
  int k = 0;

  for (int i = maxblocks; i >= 0; i--) {
    k = i;

    for (int j = maxblocks; j >= 0; j--) {
      el = (uint64_t(lhs.blocks[j]) * uint64_t(rhs.blocks[i])) + uint64_t(over);
      over = 0;
      el = value.blocks[k] + el;
      value.blocks[k] = uint32_t(el % uint32_t(pow(10, cntNumInBlck)));
      over = uint32_t(el / uint32_t(pow(10, cntNumInBlck)));
      if (k - 1 > 0) {
        k--;
      }
    }
    k++;
  }

  return value;
}

bool operator<(const uint2022_t& lhs, const uint2022_t& rhs) {
  std::string Lstr = ToString(lhs), Rstr = ToString(rhs);
  if (lhs == rhs) return false;

  if (Lstr.size() != Rstr.size()) {
    return Lstr.size() < Rstr.size();
  } else {
    for (int i = 0; i <= maxblocks; i++) {
      if (lhs.blocks[i] != rhs.blocks[i]) {
        return lhs.blocks[i] < rhs.blocks[i];
      }
    }

    return false;
  }
}

bool operator<=(const uint2022_t& lhs, const uint2022_t& rhs) {
  return (lhs < rhs || lhs == rhs);
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
  uint2022_t value;
  uint2022_t current;
  uint2022_t rigth = rhs;
  int firstBlckNoZero = FindFirstBlckNoZero(lhs);

  for (int i = firstBlckNoZero; i <= maxblocks; i++) {
    shiftLeft(current);
    current.blocks[67] = lhs.blocks[i];
    int x = 0;
    int l = 0;
    int r = 1000000000;

    while (l <= r) {
      int middle = (l + r) / 2;
      uint2022_t tmp = rigth * from_uint(uint32_t(middle));

      if (tmp <= current) {
        x = middle;
        l = middle + 1;
      } else {
        r = middle - 1;
      }
    }

    if (value != from_uint(0) || x != 0) {
      value.blocks[i] = x;
    }

    current = current - rigth * from_uint(uint32_t(x));
  }

  return value;
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
  for (int i = maxblocks; i >= 0; i--) {
    if (lhs.blocks[i] != rhs.blocks[i]) {
      return false;
    }
  }

  return true;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
  for (int i = maxblocks; i >= 0; i--) {
    if (lhs.blocks[i] != rhs.blocks[i]) {
      return true;
    }
  }

  return false;
}

std::ostream& operator<<(std::ostream& stream, const uint2022_t& value) {
  int firstBlckNoZero = 0;

  if (value == from_uint(0)) {
    stream << 0;
  } else {
    for (int i = 0; i <= maxblocks; i++) {
      if (value.blocks[i] != 0 && firstBlckNoZero == 0) {
        firstBlckNoZero = i;
        stream << value.blocks[i];
      }

      if (firstBlckNoZero < i && firstBlckNoZero > 0) {  // add fisrts "0"
        for (int j = cntNumInBlck - lenNumUint(value.blocks[i]); j > 0; j--) {
          stream << "0";
        }

        stream << value.blocks[i];
      }
    }
  }

  return stream;
}
