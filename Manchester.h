inline bool manchesterDecode(unsigned long val, char *ch) {
  *ch = 0;

  for (int i = 0; i < 8; i++) {
    unsigned long t = val & 0xc000;
    *ch <<= 1;
    if (t == 0x4000) {
      *ch |= 0x1;
    } else if (t != 0x8000) {
      return false;
    }
    val <<= 2;
  }

  return true;
}

inline bool checkWord(unsigned long val) {
  return (val & 0xc0000) == 0x80000 && (val & 0x3) == 0x1;
}

inline bool decodeWord(unsigned long val, char *ch) {
  return manchesterDecode(val >> 2, ch);
}
