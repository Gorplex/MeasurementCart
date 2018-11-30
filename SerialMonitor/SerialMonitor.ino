#define IN  Serial1
#define OUT Serial


void setup() {
  IN.begin(9600);
  OUT.begin(9600);
}

void loop() {
  if(IN.available()){
    OUT.write(IN.read());
  }
}
