#include <CapacitiveSensor.h>

CapacitiveSensor cs[6] = {
  CapacitiveSensor(2, 3),
  CapacitiveSensor(4, 5),
  CapacitiveSensor(6, 7),
  CapacitiveSensor(8, 9),
  CapacitiveSensor(10, 11),
  CapacitiveSensor(12, 13)
};

bool isTouched[6];
bool wasTouched[6];

void setup() {
  for(int i=0;i<6;i++){
    cs[i].set_CS_AutocaL_Millis(0xFFFFFFFF);
    isTouched[i];
    wasTouched[i];
  }
  Serial.begin(9600);
}

void loop() {
  for(int i=0;i<6;i++){
   long total = cs[i].capacitiveSensor(30);
  

    if (total > 450) {
      isTouched[i] = true;
    } else {
      isTouched[i] = false;
    }

    if (isTouched[i] && !wasTouched[i]) {
      Serial.println(i+1);
    }

    wasTouched[i] = isTouched[i];
  }
  // Serial.print(millis() - start);
  // Serial.print("\t");
  // Serial.println(total1); // print sensor output 1

  delay(10);
}
