#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

class QString;

class Brightness
{
public:
    static int getBrightness();
    static bool setBrightness(int brightness);

private:
    static QString runWmiCommand(const QString &command);
};

#endif // BRIGHTNESS_H
