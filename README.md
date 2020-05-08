# fonline-esp8266

> World without transistors that's being held online by transistors

FOnline online players count status for ESP8266 powered chips written in Arduino C++

### Core

- [x] Connects to remote
- [x] Writes 4 required bytes
- [x] Delay in between write and read
- [x] Read data to buffer
- [x] Endian swap
- [x] Online number parse
- [ ] Render onto OLED 128x64 display

### Optional TODO

- [ ] Multiple remotes
- [ ] Replace delay ping comepnsation with a check if write connection's over

### Required libraries

- ESP8266WiFi
