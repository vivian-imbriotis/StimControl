# StimControl

Datasheet for 05.2400.1122.1024 rotary encoder:  
https://docs.rs-online.com/8598/0900766b8147d30b.pdf

Current issues: Sufficiently rapid rotation will overflow in  
opposite direction. Planned solution: move from arduino-lang  
interrupts to proper AVR-lang interrupts, which are faster.
  
