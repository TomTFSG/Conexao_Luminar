#include <FastLED.h>
#include <CapacitiveSensor.h>

#define LED_PIN     2
#define NUM_LEDS    15
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

CapacitiveSensor cs[3] = {
  CapacitiveSensor(4, 5),  // Sensor for LED 13
  CapacitiveSensor(6, 7),  // Sensor for LED 14
  CapacitiveSensor(8, 9)   // Sensor for LED 15
};

bool isTouched[3] = {false, false, false};
bool wasTouched[3] = {false, false, false};

class Light {
private:
  int r, g, b;
  int id;
  
public:
  int x, y;
  Light() : r(0), g(0), b(0) {}
  Light(int i, int x, int y) : r(0), g(0), b(0), id(i), x(x), y(y) {}
  
  void nearClicked(int nR, int nG, int nB) {
    r = min(255, r + nR * 0.6);
    g = min(255, g + nG * 0.6);
    b = min(255, b + nB * 0.6);
  }
  
  void clicked(int nR, int nG, int nB) {
    turnOn(nR, nG, nB);
  }

  void turnOn(int nR, int nG, int nB) {
    r = min(255, r + nR);
    g = min(255, g + nG);
    b = min(255, b + nB);
  }
  
  void update() {
    if (r > 0) r--;
    if (g > 0) g--;
    if (b > 0) b--;
    
    leds[id] = CRGB(r, g, b);
  }
};

Light pix[NUM_LEDS];

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  Serial.begin(9600);

  for(int i = 0; i < 3; i++){
    cs[i].set_CS_AutocaL_Millis(0xFFFFFFFF);
  }

  pix[0] = Light(0, 0, 0);
  pix[1] = Light(1, 1, 0);
  pix[2] = Light(2, 2, 0);
  pix[3] = Light(3, 3, 0);
  pix[4] = Light(4, 4, 0);
  pix[5] = Light(5, 4, 1);
  pix[6] = Light(6, 3, 1);
  pix[7] = Light(7, 2, 1);
  pix[8] = Light(8, 1, 1);
  pix[9] = Light(9, 0, 1);
  pix[10] = Light(10, 0, 2);
  pix[11] = Light(11, 1, 2);
  pix[12] = Light(12, 2, 2);
  pix[13] = Light(13, 3, 2);
  pix[14] = Light(14, 4, 2);
}

void loop() {
  if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    int id = message.toInt() - 1;

    if (id >= 0 && id < NUM_LEDS) {
      int newR = random(0, 256);
      int newG = random(0, 256);
      int newB = random(0, 256);
      Serial.println(id);
      pix[id].clicked(newR, newG, newB);
      for (int k = 0; k < NUM_LEDS; k++) {
        if (k != id) {
          int dx = pix[id].x - pix[k].x;
          int dy = pix[id].y - pix[k].y;
          float distance = dx * dx + dy * dy;  // Avoid sqrt for faster distance calculation

          if (distance < 4) {  // Distance squared comparison to avoid sqrt
            pix[k].nearClicked(newR, newG, newB);
          }
        }
      }
    }
  }

  for(int i = 0; i < 3; i++){
    long total = cs[i].capacitiveSensor(10); // Lower sample number for faster reading
    if (total > 450) {
      isTouched[i] = true;
    } else {
      isTouched[i] = false;
    }

    if (isTouched[i] && !wasTouched[i]) {
      int id = 13 + i; // LEDs 13, 14, 15
      int newR = random(0, 256);
      int newG = random(0, 256);
      int newB = random(0, 256);
      pix[id].clicked(newR, newG, newB);
      for (int k = 0; k < NUM_LEDS; k++) {
        if (k != id) {
          int dx = pix[id].x - pix[k].x;
          int dy = pix[id].y - pix[k].y;
          float distance = dx * dx + dy * dy;

          if (distance < 4) {
            pix[k].nearClicked(newR, newG, newB);
          }
        }
      }
    }

    wasTouched[i] = isTouched[i];
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    pix[i].update();
  }

  FastLED.show();
  delay(5);
}
