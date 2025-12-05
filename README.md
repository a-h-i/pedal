Guitar Pedal
===========
A small guitar effects pedal project using teensy4.1.
It is actually an effects board and not a pedal.

## Runtime model

```
+------------------------+        +------------------+
|   Audio interrupt      |        |    loop()        |
|  (Teensy Audio Lib)    |        | (UI / control)   |
+------------------------+        +------------------+
         |                                   |
         |  audio blocks (128 samples)       |
         v                                   v
      FxEngine::processBlock()         Controls::update()

```

Separation between audio processing and UI/control logic.

In the control we read pots, footswitch, etc. and update params and LEDs.

In the audio processing loop we read blocks, process them and write them back. No blocking or allocation.

## Audio processing graph concept

```
[Codec In] → AudioInputI2S → FxProcessor → AudioOutputI2S → [Codec Out]
```

