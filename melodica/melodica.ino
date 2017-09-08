#include <usbh_midi.h>
#include <usbhub.h>
#include <SPI.h>

#define OUTPUT_power 0

USB Usb;
USBH_MIDI Midi(&Usb);


int fnumhs[128] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x18, 0x18, 0x18, 0x18, 0x18, 0x20, 0x20, 0x20, 0x20, 0x28, 0x11, 0x11, 0x19, 0x19, 0x19, 0x19, 0x19, 0x21, 0x21, 0x21, 0x21, 0x29, 0x12, 0x12, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x22, 0x22, 0x22, 0x22, 0x2A, 0x13, 0x13, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x23, 0x23, 0x23, 0x23, 0x2B, 0x14, 0x14, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x24, 0x24, 0x24, 0x24, 0x2C, 0x15, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14,  0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14};
int fnumls[128] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x65, 0x7A, 0x11, 0x29, 0x42, 0x5D, 0x79, 0x17, 0x37, 0x59, 0x7D, 0x22, 0x65, 0x7A, 0x11, 0x29, 0x42, 0X5D, 0X79, 0X17, 0X37, 0X59, 0X7D, 0X22, 0X65, 0X7A, 0X11, 0X29, 0X42, 0X5D, 0X79, 0X17, 0X37, 0X59, 0X7D, 0X22, 0X65, 0X7A, 0X11, 0X29, 0X42, 0X5D, 0X79, 0x17, 0x37, 0x59, 0x7D, 0x22, 0x65, 0x7A, 0x11, 0x29, 0x42, 0x5D, 0x79, 0x17, 0x37, 0x59, 0x7D, 0x22, 0x65, 0x00};

void setup() {
  Serial.begin(9600);

  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);

  set_ss_pin(HIGH);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.setDataMode(SPI_MODE0);
  SPI.begin();

  init_825();
  set_tone();
  set_ch();

  if (Usb.Init() == -1) {
    while(1);
  }
  delay(200);

}

void loop() {
  Usb.Task();

  if (Usb.getUsbTaskState() == USB_STATE_RUNNING) {
    getTone();
  }
}

void getTone() {
  char buf[20];
  uint8_t bufMidi[64];
  uint16_t rcvd;
  if (Midi.RecvData(&rcvd, bufMidi) == 0 ) {
    
    if (bufMidi[0] == 8) {

        keyoff();
            
      //noTone(pin);
    }else if (bufMidi[0] == 9) {
      int note = bufMidi[2];
      Serial.println(note);

      keyon(fnumhs[note],fnumls[note]);

      //tone_play();
      //delay(10000);
    }
  }
} 
