/**
 *  Product:        Tolk
 *  File:           ScreenReaderDriver.h
 *  Description:    Generic screen reader driver.
 *  Copyright:      (c) 2014, Davy Kager <mail@davykager.nl>
 *  License:        LGPLv3
 */

#ifndef _SCREENREADER_DRIVER_H_
#define _SCREENREADER_DRIVER_H_

class ScreenReaderDriver {
protected:
  ScreenReaderDriver(const wchar_t *screenReaderName, bool speech, bool braille);
public:
  virtual ~ScreenReaderDriver() {}
  virtual bool Speak(const wchar_t *str, bool interrupt = false) = 0;
  virtual bool Braille(const wchar_t *str) = 0;
  virtual bool Silence() = 0;
  virtual bool IsActive() = 0;
  virtual bool Output(const wchar_t *str, bool interrupt = false) = 0;
  const wchar_t * GetName() { return name; }
  bool HasSpeech() { return hasSpeech; }
  bool HasBraille() { return hasBraille; }
private:
  const wchar_t *name;
  const bool hasSpeech;
  const bool hasBraille;
};

#endif // _SCREENREADER_DRIVER_H_