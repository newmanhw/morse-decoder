// Morse Decoder
// Author: Newman Waters

#define BUTTON_PIN 7    // Morse BUTTON_PIN
#define SPEAKER_PIN 8   // SPEAKER_PIN=

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);
  Serial.begin(115200);
}

float DashDuration = 200.0;
boolean PrevBUTTON_PINState = false;
long StartSymbolTime, StartPauseTime;
boolean BUTTON_PINState;
String symbol = "";

void loop() {
  BUTTON_PINState = !digitalRead(BUTTON_PIN);

  if (BUTTON_PINState) {
    if (BUTTON_PINState != PrevBUTTON_PINState) {
      StartSymbolTime = millis();
      DecodePause(StartPauseTime);
    }
    digitalWrite(SPEAKER_PIN, HIGH);
  } else {
    if (BUTTON_PINState != PrevBUTTON_PINState) {
      StartPauseTime = millis();
      Decode(StartSymbolTime);
    }
    digitalWrite(SPEAKER_PIN, LOW);
  }

  if (abs(millis() - StartPauseTime) > DashDuration * 10) {
    DecodePause(StartPauseTime);
  }

  PrevBUTTON_PINState = BUTTON_PINState;
}

void Decode(long startTime) {
  char symbolChar = '?';
  long duration = abs(millis() - startTime); // Duration of the symbol (dot or dash)
  float DotDuration = DashDuration / 3.0;

  if (duration <= 2) return; // Debounce suppression

  if (duration <= DotDuration) symbolChar = '.';
  else if (duration > DashDuration) symbolChar = '-';
  else if ((duration > (DashDuration + DotDuration) / 1.9) && duration <= DashDuration) symbolChar = '-';
  else symbolChar = '.';

  if (symbolChar == '-') {
    if (duration > DashDuration) DashDuration++;
    if (duration < DashDuration) DashDuration--;
  } else if (symbolChar == '.') {
    if (duration > DashDuration / 3.0) DashDuration++;
    if (duration < DashDuration / 3.0) DashDuration--;
  }
  symbol += symbolChar;
}

void DecodePause(long startTime) {
  if (symbol == "") return;

  char symbolChar = '?';
  long duration = abs(millis() - startTime);
  if (duration > DashDuration - DashDuration / 40) {
    DecodeSymbol();
  }
  if (duration > DashDuration * 2) {
    DecodeSymbol();
    PrintToSerial(" "); 
    Serial.print(" ");
  }
}

void DecodeSymbol() {
  static String letters[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
    ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "E"
  };
  int i = 0;
  while (letters[i] != "E") {
    if (letters[i] == symbol) {
      Serial.print((char)('A' + i));
      break;
    }
    i++;
  }
  if (letters[i] == "E") {
    Serial.print(symbol);
  }
  symbol = "";
}


